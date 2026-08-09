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

#if !defined(ARCADIA_FILESYSTEM_DEFAULTFILEHANDLE_H_INCLUDED)
#define ARCADIA_FILESYSTEM_DEFAULTFILEHANDLE_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/FileHandle.h"
typedef struct Arcadia_FilePath Arcadia_FilePath;
typedef struct Arcadia_FileSystem Arcadia_FileSystem;

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <limits.h> // SSIZE_MAX
  #include <unistd.h> // STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO

#else

  #error("environment not (yet) supported")

#endif

Arcadia_declareObjectType(u8"Arcadia.DefaultFileHandle", Arcadia_DefaultFileHandle,
                          u8"Arcadia.FileHandle");

struct Arcadia_DefaultFileHandleDispatch {
  Arcadia_FileHandleDispatch _parent;
};

struct Arcadia_DefaultFileHandle {
  Arcadia_FileHandle _parent;
  Arcadia_FileSystem* fileSystem;
  Arcadia_Natural8Value flags;

  #if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

    HANDLE fd;

  #elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
        Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

    int fd;

  #else

    #error("environment not (yet) supported")

  #endif

};

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_create
Arcadia_DefaultFileHandle*
Arcadia_DefaultFileHandle_create
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem
  );

#endif // ARCADIA_FILESYSTEM_DEFAULTFILEHANDLE_H_INCLUDED
