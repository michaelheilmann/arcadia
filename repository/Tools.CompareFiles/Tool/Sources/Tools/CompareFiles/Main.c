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

#include "Arcadia/FileSystem/Include.h"

#include <string.h>
#include <stdlib.h>

static Arcadia_BooleanValue
main1
  (
    Arcadia_Thread* thread,
    int argc,
    char** argv
  )
{
  if (argc < 3) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_FilePath* firstFile = Arcadia_FilePath_parseNative(thread, Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, argv[1], strlen(argv[1]))));
  Arcadia_FilePath* secondFile = Arcadia_FilePath_parseNative(thread, Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, argv[2], strlen(argv[2]))));
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  if (!Arcadia_FileSystem_regularFileExists(thread, fileSystem, firstFile)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_FileSystem_regularFileExists(thread, fileSystem, secondFile)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ByteArrayBuilder* firstByteBuffer = Arcadia_FileSystem_getFileContents(thread, fileSystem, firstFile);
  Arcadia_ByteArrayBuilder* secondByteBuffer = Arcadia_FileSystem_getFileContents(thread, fileSystem, secondFile);
  return Arcadia_ByteArrayBuilder_isEqualTo(thread, firstByteBuffer, secondByteBuffer);
}

// Returns 2 on failure.
// Returns 0 if there are no differences.
// Returns 1 if there are differences.
int
main
  (
    int argc,
    char** argv
  )
{
  Arcadia_BooleanValue areEqual = Arcadia_BooleanValue_False;
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return EXIT_FAILURE;
  }
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    areEqual = main1(thread, argc, argv);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    return 2;
  }
  return areEqual ? 0 : 1;
}
