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

#include "Arcadia/Testing/Include.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Tests/Include.h"

static void
onTest
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* sourceFilePath
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* absoluteSourceFilePath = NULL;
  if (Arcadia_FilePath_isRelative(thread, sourceFilePath)) {
    absoluteSourceFilePath = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
    Arcadia_FilePath_append(thread, absoluteSourceFilePath, sourceFilePath);
  } else {
    absoluteSourceFilePath = sourceFilePath;
  }
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, sourceFilePath, Arcadia_BooleanValue_False), sourceFilePath);
  Arcadia_UnicodeCodePointReader* reader =
    (Arcadia_UnicodeCodePointReader*)
    Arcadia_ByteReader_UnicodeCodePointReader_create
      (
        thread,
        (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_Languages_InputFile_getContents(thread, inputFile))
      );
  Arcadia_PDLC_Parser* parser = Arcadia_PDLC_Parser_getInstance(thread, context);
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, reader);
  Arcadia_PDLC_Parser_setFile(thread, parser, inputFile);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
  }
  Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
  if (Arcadia_Thread_getStatus(thread) || Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics)) {
    Arcadia_Thread_raise(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_PDLC_CompilationFailedException_create(thread)));
  }
}

/// Run the parser over @c sourceFilePath and verify that a syntactical error was reported as a
/// diagnostic. Raising the status without adding a diagnostic used to make the parser fail without
/// telling the user what is wrong.
static void
onExpectSyntacticalError
  (
    Arcadia_Thread* thread,
    const char* sourcePath,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_String* sourceString = Arcadia_String_createFromCxxString(thread, sourcePath);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sourceString);
  Arcadia_FilePath* sourceFilePath = Arcadia_FilePath_parseGeneric(thread, sourceString);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sourceFilePath);
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, sourceFilePath, Arcadia_BooleanValue_False), sourceFilePath);
  Arcadia_ByteReader* byteReader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_Languages_InputFile_getContents(thread, inputFile));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)byteReader);
  Arcadia_UnicodeCodePointReader* reader =
    (Arcadia_UnicodeCodePointReader*)
    Arcadia_ByteReader_UnicodeCodePointReader_create
      (
        thread,
        byteReader
      );
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)reader);
  Arcadia_PDLC_Parser* parser = Arcadia_PDLC_Parser_getInstance(thread, context);
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, reader);
  Arcadia_PDLC_Parser_setFile(thread, parser, inputFile);
  volatile Arcadia_BooleanValue failed = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    // The parser raises `Arcadia_Status_SyntacticalError` without running off the end of `run`.
    Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    failed = Arcadia_BooleanValue_True;
  }
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_PDLC_Tests_assertDiagnostic(thread, context->diagnostics, status, expected);
  Arcadia_Tests_assertTrue(thread, failed);
  Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

void
main1
  (
    Arcadia_Process* process
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Procedures.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Classes.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/NativeHeaders.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/NativeEnumerations.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/SemanticModels.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/AbstractMethods.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Interfaces.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Statements1.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Statements2.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Statements3.pdl")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Variables.pdl")));
  static const struct {
    const char* source;
    Arcadia_PDLC_Tests_ExpectedDiagnostic diagnostic;
  } failures[] = {
#define SYNTAX(Name, Type, File, Offset, Message) \
    { "Assets/" #Name ".pdl", { #Name, Type, File, Offset, Message, Arcadia_Status_SyntacticalError } },
#include "NegativeCases.i"
#undef SYNTAX
  };
  for (size_t i = 0; i < sizeof(failures) / sizeof(failures[0]); ++i) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    onExpectSyntacticalError(thread, failures[i].source, &failures[i].diagnostic);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  }
}

static int
runTests
  (
    void
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return EXIT_FAILURE;
  }
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    main1(process);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  thread = NULL;
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "SyntacticalAnalysis", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
