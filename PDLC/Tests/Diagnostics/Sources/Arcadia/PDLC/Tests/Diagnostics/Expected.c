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

#include "Expected.h"
#include "Arcadia/PDLC/Tests/Include.h"
#include <stdlib.h>

static void
rejects
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Status status,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Tests_assertFalse(thread, Arcadia_PDLC_Tests_matchesDiagnostic(thread, diagnostics, status, expected));
  volatile Arcadia_BooleanValue rejected = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_PDLC_Tests_assertDiagnostic(thread, diagnostics, status, expected);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    rejected = Arcadia_Thread_getStatus(thread) == Arcadia_Status_TestFailed;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
  Arcadia_Tests_assertTrue(thread, rejected);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
expectations
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Assets/MyClass.pdl");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_FilePath* path = Arcadia_FilePath_parseGeneric(thread, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path);
  Arcadia_Languages_InputFile* file = Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, name, path);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)file);
  Arcadia_String* symbol = Arcadia_String_createFromCxxString(thread, "Missing");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)symbol);
  Arcadia_Languages_Diagnostic* diagnostic = (void*)Arcadia_PDLC_Diagnostics_SymbolIsNotDefinedDiagnostic_create
    (
      thread, Arcadia_Languages_DiagnosticType_Error, file, 3, symbol
    );
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostic);
  Arcadia_Languages_Diagnostics_add(thread, context->diagnostics, diagnostic);
  Arcadia_PDLC_Tests_ExpectedDiagnostic expected = {
    "ExpectedDiagnostics", "Arcadia.PDLC.Diagnostics.SymbolIsNotDefinedDiagnostic", "Assets/MyClass.pdl", 3,
    "semantical error: symbol `Missing` not defined\n", Arcadia_Status_SemanticalError
  };
  Arcadia_PDLC_Tests_assertDiagnostic(thread, context->diagnostics, Arcadia_Status_SemanticalError, &expected);
  // Exercise the real raising assertion, not just a mock predicate, above 255 slots.
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, 700 + i);
  Arcadia_PDLC_Tests_ExpectedDiagnostic wrong = expected;
  wrong.typeName = "Arcadia.PDLC.Diagnostics.SymbolIsAlreadyDefinedDiagnostic";
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  wrong = expected; wrong.fileSuffix = "Elsewhere/MyClass.pdl";
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  wrong = expected; wrong.fileSuffix = "sets/MyClass.pdl";
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  wrong = expected; wrong.offset = 4;
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  wrong = expected; wrong.message = "semantical error: symbol `Other` not defined\n";
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  rejects(thread, context->diagnostics, Arcadia_Status_Success, &expected);
  rejects(thread, context->diagnostics, Arcadia_Status_AllocationFailed, &expected);
  diagnostic->type = Arcadia_Languages_DiagnosticType_Warning;
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &expected);
  diagnostic->type = Arcadia_Languages_DiagnosticType_Error;
  diagnostic->inputPosition = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &expected);
  diagnostic->inputPosition = Arcadia_Value_makeSizeValue(3);
  wrong = expected; wrong.offset = ARCADIA_PDLC_TESTS_NO_POSITION;
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  wrong = expected; wrong.fileSuffix = NULL;
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &wrong);
  diagnostic->inputFile = NULL;
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &expected);
  diagnostic->inputFile = file;
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)context->diagnostics->diagnostics);
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &expected);
  Arcadia_Languages_Diagnostics_add(thread, context->diagnostics, diagnostic);
  Arcadia_Languages_Diagnostics_add(thread, context->diagnostics, diagnostic);
  rejects(thread, context->diagnostics, Arcadia_Status_SemanticalError, &expected);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)context->diagnostics->diagnostics);
  Arcadia_Languages_Diagnostics_add(thread, context->diagnostics, diagnostic);
  wrong = expected; wrong.status = Arcadia_Status_ValueRaised;
  Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  rejects(thread, context->diagnostics, Arcadia_Status_ValueRaised, &wrong);
  Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)symbol));
  rejects(thread, context->diagnostics, Arcadia_Status_ValueRaised, &wrong);
  Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)Arcadia_PDLC_CompilationFailedException_create(thread)));
  Arcadia_PDLC_Tests_assertDiagnostic(thread, context->diagnostics, Arcadia_Status_ValueRaised, &wrong);
  Arcadia_Thread_setRaisedValue(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  for (size_t i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, i) == 999 - i);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_PDLC_Tests_runExpectedDiagnostics
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&expectations) ? EXIT_SUCCESS : EXIT_FAILURE;
}
