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
#include "Arcadia/FileSystem/Linux/getHomeFolder.h"

#include "Arcadia/FileSystem/Include.h"

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
    Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  #include <string.h> // strlen
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
Arcadia_DefaultFileSystem_getHomeFolderHelper
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
  size_t n = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (n == -1) {
    n = 0x4000; // = all zeroes with the 14th bit set (1 << 14)
  }
  char* p = malloc(n);
  if (NULL == p) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  struct passwd pwd;
  struct passwd* ppwd;
  int s = getpwuid_r(getuid(), &pwd, p, n, &ppwd);
  if (ppwd == NULL) {
    // if s is 0 the no entry for this user id was found.
    free(p);
    p = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_FilePath* filePath = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_String* filePathString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, ppwd->pw_dir, strlen(ppwd->pw_dir))));
    filePath = Arcadia_FilePath_parseUnix(thread, filePathString);
    Arcadia_Thread_popJumpTarget(thread);
    free(p);
    p = NULL;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(p);
    p = NULL;
    Arcadia_Thread_jump(thread);
  }
  return filePath;
}

#endif
