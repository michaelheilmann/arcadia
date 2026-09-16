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

#include "Arcadia/MILC/Include.h"

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
  Arcadia_MILC_Context* context = Arcadia_MILC_Context_create(thread);
  Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFile_create(thread, sourceFilePath);
  Arcadia_UnicodeCodePointReader* reader =
    (Arcadia_UnicodeCodePointReader*)
    Arcadia_ByteReader_UnicodeCodePointReader_create
      (
        thread,
        (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_Languages_InputFile_getContents(thread, inputFile))
      );
  Arcadia_MILC_Parser* parser = Arcadia_MILC_Parser_getInstance(thread, context);
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, reader);
  Arcadia_MILC_Parser_setFile(thread, parser, inputFile);
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
    Arcadia_Thread_raise(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_CompilationFailedException_create(thread)));
  }
}

void
main1
  (
    Arcadia_Process* process,
    int argc,
    char** argv
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Procedures.mil")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Classes.mil")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Statements1.mil")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Statements2.mil")));
  onTest(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Variables.mil")));
}

int
main
  (
    int argc,
    char** argv
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
    main1(process, argc, argv);
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
