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

#if !defined(ARCADIA_FILESYSTEM_WINDOWS_DIRECTORYITERATORWINDOWS_H_INCLUDED)
#define ARCADIA_FILESYSTEM_WINDOWS_DIRECTORYITERATORWINDOWS_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/FileSystem/DirectoryIterator.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Arcadia_declareObjectType(u8"Arcadia.DirectoryIteratorWindows", Arcadia_DirectoryIteratorWindows,
                          u8"Arcadia.DirectoryITerator");

struct Arcadia_DirectoryIteratorWindowsDispatch {
  Arcadia_DirectoryIteratorDispatch _parent;
};

struct Arcadia_DirectoryIteratorWindows {
  Arcadia_DirectoryIterator _parent;
  Arcadia_FilePath* path;
  HANDLE handle;
  WIN32_FIND_DATAA data;
};

Arcadia_DirectoryIteratorWindows*
Arcadia_DirectoryIteratorWindows_create
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path
  );

#endif // ARCADIA_FILESYSTEM_WINDOWS_DIRECTORYITERATORWINDOWS_H_INCLUDED
