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
    Arcadia_SizeValue lastLine,
    Arcadia_FilePath* sourceFilePath
  )
{
  Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFile_create(thread, sourceFilePath);
  Arcadia_ByteArray* contents = Arcadia_Languages_InputFile_getContents(thread, inputFile);
  Arcadia_SizeValue n = Arcadia_ByteArray_getSize(thread, contents);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Languages_InputFile_getLine(thread, inputFile, 0));
  Arcadia_Tests_assertTrue(thread, lastLine == Arcadia_Languages_InputFile_getLine(thread, inputFile, n));
  Arcadia_Tests_assertTrue(thread, lastLine == Arcadia_Languages_InputFile_getLine(thread, inputFile, n + 1000));
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Languages_InputFile_getLine(thread, inputFile, i);
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
  onTest(thread, 63, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Statements.mil")));
  onTest(thread, 21, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Variables.mil")));
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
