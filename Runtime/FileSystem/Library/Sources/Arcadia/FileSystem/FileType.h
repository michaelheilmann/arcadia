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

#if !defined(ARCADIA_FILESYSTEM_FILETYPE_H_INCLUDED)
#define ARCADIA_FILESYSTEM_FILETYPE_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/Ring2/Implementation/Configure.h"
#include "Arcadia/Ring1/Include.h"

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileType
Arcadia_declareEnumerationType("Arcadia.FileType", Arcadia_FileType);

enum Arcadia_FileType {

  Arcadia_FileType_Unknown = 0,

  Arcadia_FileType_Directory,

  Arcadia_FileType_Regular,

};

#endif // ARCADIA_FILESYSTEM_FILETYPE_H_INCLUDED
