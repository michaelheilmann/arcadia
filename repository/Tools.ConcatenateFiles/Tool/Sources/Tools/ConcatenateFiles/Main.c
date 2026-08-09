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

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"

#include <string.h>
#include <stdlib.h>

static void
main1
  (
    Arcadia_Process* process,
    int argc,
    char** argv
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  if (argc < 3) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  for (int argi = 1; argi < argc - 1; ++argi) {
    Arcadia_String* filePathString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, argv[argi], strlen(argv[argi]))));
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseNative(thread, filePathString);
    Arcadia_FileHandle_openForReading(thread, fileHandle, filePath);
    char bytes[5012];
    Arcadia_SizeValue bytesToRead = 5012;
    Arcadia_SizeValue bytesRead = 0;
    do {
      Arcadia_FileHandle_read(thread, fileHandle, bytes, bytesToRead, &bytesRead);
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteBuffer, bytes, bytesRead);
    } while (bytesRead > 0);
    Arcadia_FileHandle_close(thread, fileHandle);
  }
  Arcadia_String* filePathString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, argv[argc - 1], strlen(argv[argc - 1]))));
  Arcadia_FilePath* filePath = Arcadia_FilePath_parseNative(thread, filePathString);
  Arcadia_FileHandle_openForWriting(thread, fileHandle, filePath);
  Arcadia_SizeValue bytesWritten;
  Arcadia_FileHandle_write(thread, fileHandle, byteBuffer->p, byteBuffer->sz, &bytesWritten);
  Arcadia_FileHandle_close(thread, fileHandle);
}

// Return 0 on success.
// Return 2 on failure.
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
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(Arcadia_Process_getThread(process), &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    main1(process, argc, argv);
  }
  Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
  Arcadia_Status status = Arcadia_Thread_getStatus(Arcadia_Process_getThread(process));
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    // Return 2 on failure.
    return 2;
  }
  // Return 0 on success.
  return 0;
}
