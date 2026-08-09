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

#if !defined(ARCADIA_FILESYSTEM_IMPLEMENTATION_DEFAULTFILESYSTEM_H_INCLUDED)
#define ARCADIA_FILESYSTEM_IMPLEMENTATION_DEFAULTFILESYSTEM_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/FileSystem/FileSystem.h"

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#endif

/// @code
/// class DefaultFileSystem {
///   constructor()
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DefaultFileSystem", Arcadia_DefaultFileSystem,
                          u8"Arcadia.FileSystem");


struct Arcadia_DefaultFileSystemDispatch {
  Arcadia_FileSystemDispatch _parent;
};

struct Arcadia_DefaultFileSystem {
  Arcadia_FileSystem _parent;

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem

  HANDLE stdin;
  HANDLE stdout;
  HANDLE stderr;

#endif

};

Arcadia_DefaultFileSystem*
Arcadia_DefaultFileSystem_getOrCreate
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_FILESYSTEM_IMPLEMENTATION_DEFAULTFILESYSTEM_H_INCLUDED
