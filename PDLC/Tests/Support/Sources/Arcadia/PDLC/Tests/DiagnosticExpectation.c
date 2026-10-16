// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/PDLC/Tests/Include.h"
#include <stdio.h>
#include <string.h>

static Arcadia_BooleanValue
hasTypeName
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    const char* expected
  )
{
  Arcadia_Name* name = Arcadia_Type_getName(thread, Arcadia_Object_getType(thread, object));
  size_t size = strlen(expected);
  return Arcadia_Name_getNumberOfBytes(thread, name) == size
      && !memcmp(Arcadia_Name_getBytes(thread, name), expected, size);
}

Arcadia_BooleanValue
Arcadia_PDLC_Tests_matchesDiagnostic
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Status status,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
  Arcadia_BooleanValue matches = Arcadia_BooleanValue_False;
  if (status != expected->status || Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics) != 1) goto done;
  if (status == Arcadia_Status_ValueRaised) {
    Arcadia_Value raised = Arcadia_Thread_getRaisedValue(thread);
    if (!Arcadia_Value_isObjectReferenceValue(&raised)
        || !hasTypeName(thread, Arcadia_Value_getObjectReferenceValue(&raised), "Arcadia.PDLC.CompilationFailedException")) goto done;
  }
  Arcadia_Languages_Diagnostic* diagnostic = (void*)Arcadia_List_getObjectReferenceValueAt(thread, diagnostics->diagnostics, 0);
  if (Arcadia_Languages_Diagnostic_getType(thread, diagnostic) != Arcadia_Languages_DiagnosticType_Error
      || !hasTypeName(thread, (Arcadia_Object*)diagnostic, expected->typeName)) goto done;
  if (expected->offset == ARCADIA_PDLC_TESTS_NO_POSITION) {
    if (!Arcadia_Value_isVoidValue(&diagnostic->inputPosition)) goto done;
  } else {
    if (!Arcadia_Value_isSizeValue(&diagnostic->inputPosition)
        || Arcadia_Value_getSizeValue(&diagnostic->inputPosition) != expected->offset) goto done;
  }
  Arcadia_FilePath* path = diagnostic->inputFile ? Arcadia_Languages_InputFile_getPath(thread, diagnostic->inputFile) : NULL;
  Arcadia_BooleanValue fileSystem = hasTypeName(thread, (Arcadia_Object*)diagnostic, "Arcadia.PDLC.Diagnostics.FileSystemDiagnostic");
  if (fileSystem) path = ((Arcadia_PDLC_Diagnostics_FileSystemDiagnostic*)diagnostic)->path;
  if (expected->fileSuffix) {
    if (!path) goto done;
    Arcadia_String* actual = Arcadia_FilePath_toGeneric(thread, path);
    size_t size = Arcadia_String_getNumberOfBytes(thread, actual);
    size_t suffix = strlen(expected->fileSuffix);
    const char* bytes = (const char*)Arcadia_String_getBytes(thread, actual);
    if (size < suffix || memcmp(bytes + size - suffix, expected->fileSuffix, suffix)
        || (size > suffix && bytes[size - suffix - 1] != '/')) goto done;
  } else if (path) goto done;
  Arcadia_String* message = fileSystem
    ? ((Arcadia_PDLC_Diagnostics_FileSystemDiagnostic*)diagnostic)->message
    : Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic);
  matches = expected->message && Arcadia_String_getNumberOfBytes(thread, message) == strlen(expected->message)
      && !memcmp(Arcadia_String_getBytes(thread, message), expected->message, strlen(expected->message));
done:
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return matches;
}

void
Arcadia_PDLC_Tests_assertDiagnostic
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Status status,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
  Arcadia_BooleanValue matches = Arcadia_PDLC_Tests_matchesDiagnostic(thread, diagnostics, status, expected);
  if (!matches) {
    fprintf(stderr, "Diagnostic expectation failed [%s]: status expected=%u actual=%u; expected type=%s file=%s offset=%zu; diagnostic count=%zu\n",
      expected->fixture, (unsigned)expected->status, (unsigned)status, expected->typeName,
      expected->fileSuffix ? expected->fileSuffix : "<none>", expected->offset,
      Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics));
    fprintf(stderr, "Expected exact message: %s\n", expected->message ? expected->message : "<none>");
    // Assertion reporting must not load a source file/line map: a nonexistent or
    // wrong file is itself a possible mismatch, not a secondary I/O exception.
    for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics); ++i) {
      Arcadia_Languages_Diagnostic* diagnostic = (void*)Arcadia_List_getObjectReferenceValueAt(thread, diagnostics->diagnostics, i);
      Arcadia_Name* typeName = Arcadia_Type_getName(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)diagnostic));
      fprintf(stderr, "Actual diagnostic[%zu]: type=%.*s severity=%u ", i,
        (int)Arcadia_Name_getNumberOfBytes(thread, typeName), Arcadia_Name_getBytes(thread, typeName), (unsigned)diagnostic->type);
      Arcadia_FilePath* path = diagnostic->inputFile ? Arcadia_Languages_InputFile_getPath(thread, diagnostic->inputFile) : NULL;
      if (hasTypeName(thread, (Arcadia_Object*)diagnostic, "Arcadia.PDLC.Diagnostics.FileSystemDiagnostic"))
        path = ((Arcadia_PDLC_Diagnostics_FileSystemDiagnostic*)diagnostic)->path;
      if (path) {
        Arcadia_String* text = Arcadia_FilePath_toGeneric(thread, path);
        fprintf(stderr, "file=%.*s ", (int)Arcadia_String_getNumberOfBytes(thread, text), (const char*)Arcadia_String_getBytes(thread, text));
      } else fputs("file=<none> ", stderr);
      if (Arcadia_Value_isSizeValue(&diagnostic->inputPosition)) fprintf(stderr, "offset=%zu ", Arcadia_Value_getSizeValue(&diagnostic->inputPosition));
      else fputs("offset=<none> ", stderr);
      Arcadia_String* text = Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic);
      fprintf(stderr, "message=%.*s\n", (int)Arcadia_String_getNumberOfBytes(thread, text), (const char*)Arcadia_String_getBytes(thread, text));
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  if (!matches) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}
