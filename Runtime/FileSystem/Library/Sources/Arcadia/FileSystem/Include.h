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

#if !defined(ARCADIA_FILESYSTEM_INCLUDE_H_INCLUDED)
#define ARCADIA_FILESYSTEM_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_FILESYSTEM_MODULE")
#define ARCADIA_FILESYSTEM_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_FILESYSTEM_EXPORT")
#define ARCADIA_FILESYSTEM_EXPORT (1)

#include "Arcadia/FileSystem/DirectoryIterator.h"
#include "Arcadia/FileSystem/ExistingFilePolicy.h"
#include "Arcadia/FileSystem/FileAccessMode.h"
#include "Arcadia/FileSystem/FileHandle.h"
#include "Arcadia/FileSystem/FileHandleExtensions.h"
#include "Arcadia/FileSystem/FilePath.h"
#include "Arcadia/FileSystem/FileSystem.h"
#include "Arcadia/FileSystem/FileType.h"
#include "Arcadia/FileSystem/NonExistingFilePolicy.h"

#undef ARCADIA_FILESYSTEM_EXPORT
#pragma pop_macro("ARCADIA_FILESYSTEM_EXPORT")

#undef ARCADIA_FILESYSTEM_MODULE
#pragma pop_macro("ARCADIA_FILESYSTEM_MODULE")

#endif // ARCADIA_FILESYSTEM_INCLUDE_H_INCLUDED
