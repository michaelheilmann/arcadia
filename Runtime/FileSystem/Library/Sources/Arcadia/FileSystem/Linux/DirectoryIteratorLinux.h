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

#if !defined(ARCADIA_FILESYSTEM_LINUX_DIRECTORYITERATORLINUX_H_INCLUDED)
#define ARCADIA_FILESYSTEM_LINUX_DIRECTORYITERATORLINUX_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/FileSystem/DirectoryIterator.h"

#if (Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Macos)
	#include <sys/types.h>
	#include <sys/dir.h>
#else
	#include <dirent.h>
#endif

Arcadia_declareObjectType(u8"Arcadia.DirectoryIteratorLinux", Arcadia_DirectoryIteratorLinux,
                          u8"Arcadia.DirectoryIterator");

struct Arcadia_DirectoryIteratorLinuxDispatch {
  Arcadia_DirectoryIteratorDispatch _parent;
};

struct Arcadia_DirectoryIteratorLinux {
  Arcadia_DirectoryIterator _parent;
  DIR *dir;
  struct dirent *dirent;
};

Arcadia_DirectoryIteratorLinux*
Arcadia_DirectoryIteratorLinux_create
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_FILESYSTEM_LINUX_DIRECTORYITERATORLINUX_H_INCLUDED
