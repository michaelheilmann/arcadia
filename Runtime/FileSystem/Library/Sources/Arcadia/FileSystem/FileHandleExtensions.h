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

#if !defined(ARCADIA_FILESYSTEM_FILEHANDLEEXTENSIONS_H_INCLUDED)
#define ARCADIA_FILESYSTEM_FILEHANDLEEXTENSIONS_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/FileSystem/FileHandle.h"
typedef struct Arcadia_ByteArrayBuilder Arcadia_ByteArrayBuilder;
typedef struct Arcadia_String Arcadia_String;
typedef struct Arcadia_StringBuilder Arcadia_StringBuilder;

/// @brief Write the contents of a Byte buffer to this file handle.
/// @param self A pointer to this file handle.
/// @param source A pointer to the Byte buffer.
void
Arcadia_FileHandle_writeByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_ByteArrayBuilder* source
  );

/// @brief Write the contents of a string buffer to this file handle.
/// @param self A pointer to this file handle.
/// @param source A pointer to the string.
void
Arcadia_FileHandle_writeString
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_String* source
  );

/// @brief Write the contents of a string buffer to this file handle.
/// @param self A pointer to this file handle.
/// @param source A pointer to the string buffer.
void
Arcadia_FileHandle_writeStringBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_StringBuilder* source
  );

#endif // ARCADIA_FILESYSTEM_FILEHANDLEEXTENSIONS_H_INCLUDED
