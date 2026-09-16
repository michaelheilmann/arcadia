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

#if !defined(ARCADIA_MILC_FILETYPE_H_INCLUDED)
#define ARCADIA_MILC_FILETYPE_H_INCLUDED

#include "Arcadia/Languages/Include.h"

/// @brief Enumeration of types of files the compiler is operating on.
Arcadia_declareEnumerationType(u8"Arcadia.MILC.FileType", Arcadia_MILC_FileType);

enum Arcadia_MILC_FileType {
  Arcadia_MILC_FileType_ConfigurationFile,
  Arcadia_MILC_FileType_ModuleDirectory,
  Arcadia_MILC_FileType_CompilationUnit,
};

#endif // ARCADIA_MILC_FILETYPE_H_INCLUDED
