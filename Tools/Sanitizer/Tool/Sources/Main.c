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

#include "Arcadia/Sanitizer/Sanitizer.h"
#include <stdlib.h>
#include <string.h>

static void
writeText
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* output,
    char const* text
  )
{
  Arcadia_SizeValue length = strlen(text);
  char const* cursor = text;
  while (length) {
    Arcadia_SizeValue count = 0;
    Arcadia_FileHandle_write(thread, output, cursor, length, &count);
    if (!count) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    cursor += count;
    length -= count;
  }
}

static int
run
  (
    Arcadia_Thread* thread,
    int argc,
    char** argv
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* output = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, output);
  if ((argc < 2 || argc > 3) || (strcmp(argv[1], "--check") && strcmp(argv[1], "--fix"))) {
    writeText(thread, output, "usage: Arcadia.Sanitizer (--check|--fix) [file-or-directory]\n");
    return 2;
  }
  Arcadia_BooleanValue fix = !strcmp(argv[1], "--fix") ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
  Arcadia_String* pathString = Arcadia_String_createFromCxxString(thread, argc == 3 ? argv[2] : ".");
  Arcadia_FilePath* path = Arcadia_FilePath_getFullPath(thread, Arcadia_FilePath_parseNative(thread, pathString));
  if (Arcadia_FileSystem_isSymbolicLink(thread, fileSystem, path)) {
    writeText(thread, output, "refusing to traverse a symbolic-link root\n");
    return 2;
  }
  Arcadia_Sanitizer_Statistics statistics = { 0 };
  Arcadia_Sanitizer_scan(thread, fileSystem, path, output, fix, &statistics);
  return !fix && statistics.changed ? 1 : 0;
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
    return 2;
  }
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  int result = 2;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    result = run(thread, argc, argv);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  thread = NULL;
  Arcadia_Process_relinquish(process);
  return status ? 2 : result;
}
