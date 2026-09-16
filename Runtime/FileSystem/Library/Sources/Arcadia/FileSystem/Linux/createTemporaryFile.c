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

#define ARCADIA_FILESYSTEM_MODULE (1)
#include "Arcadia/FileSystem/Linux/createTemporaryFile.h"

#include "Arcadia/FileSystem/Include.h"

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
    Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  #include <stdlib.h> // getenv, malloc, free
  #include <string.h> // memcpy, strlen
  #include <sys/stat.h> // stat
  #include <errno.h> // errno
  #include <linux/limits.h> // PATH_MAX
  #include <pwd.h> // struct passwd
  #include <sys/types.h>
  #include <fcntl.h>
  #include <unistd.h> // getcwd, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO

#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
    Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

Arcadia_FilePath*
Arcadia_DefaultFileSystem_createTemporaryFile
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
  static const char suffix[] = "/Arcadia.XXXXXX";
  const char* temporaryDirectory = getenv("TMPDIR");
  if (!temporaryDirectory || !temporaryDirectory[0]) {
    temporaryDirectory = "/tmp";
  }
  size_t temporaryDirectoryLength = strlen(temporaryDirectory);
  size_t suffixLength = sizeof(suffix); // Includes the zero terminator.
  char* path = malloc(temporaryDirectoryLength + suffixLength);
  if (!path) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  memcpy(path, temporaryDirectory, temporaryDirectoryLength);
  memcpy(path + temporaryDirectoryLength, suffix, suffixLength);

  int fileDescriptor = mkstemp(path);
  if (-1 == fileDescriptor) {
    free(path);
    path = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_FilePath* filePath = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_String* filePathString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, path, strlen(path))));
    filePath = Arcadia_FilePath_parseUnix(thread, filePathString);
    Arcadia_Thread_popJumpTarget(thread);
    if (-1 == close(fileDescriptor)) {
      free(path);
      path = NULL;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    fileDescriptor = -1;
    free(path);
    path = NULL;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    close(fileDescriptor);
    fileDescriptor = -1;
    free(path);
    path = NULL;
    Arcadia_Thread_jump(thread);
  }
  return filePath;
}

#endif
