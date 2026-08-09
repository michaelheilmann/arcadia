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
#include <stdlib.h>

static void
main1
  (
    Arcadia_Process* process,
    int argi,
    char** argv
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);
  Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);

#define Emit(String) \
  Arcadia_ByteArrayBuilder_clear(thread, byteBuffer); \
\
  Arcadia_Unicode_Encoder_encodeString \
    ( \
      thread, \
      encoder, \
      Arcadia_String_create \
        ( \
          thread, \
          Arcadia_Value_makeRuntimeUTF8StringValue \
            ( \
              Arcadia_RuntimeUTF8String_create \
                ( \
                  thread, \
                  String, \
                  sizeof(String) - 1 \
                ) \
            ) \
        ), \
      byteBuffer \
    ); \
\
  Arcadia_FileHandle_writeByteBuffer(thread, fileHandle, byteBuffer);

  // Test some escape sequences according to VT102, ECMA - 48, ISO / IEC 6429, ANSI X3.64.
  // `\033[38;2;<red>;<green>;<blue>;m` where <red>, <green>, <blue> are a integer literal within the range of [0,255] without signs or leading zeroes.
  // Sets the RGB color of the foreground.
  Emit(u8"\033[38;2;255;255;255m");
  Emit(u8"this is white\n");
  Emit(u8"\033[38;2;255;0;0m");
  Emit(u8"this is red\n");

#undef Emit
}

int
main
  (
    int argc,
    char **argv
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
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
