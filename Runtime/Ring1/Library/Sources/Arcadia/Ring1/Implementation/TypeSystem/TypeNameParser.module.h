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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPENAMEPARSER_MODULE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPENAMEPARSER_MODULE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#if defined(ARCADIA_RING1_EXPORT)
  #error("this file is not supposed to be exported")
#endif
#include "Arcadia/Ring1/Implementation/Configure.h"
#include "Arcadia/Ring1/Implementation/Size.h"

/// @brief Get if a sequence of Bytes is an UTF8 string.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @error Arcadia_Status_ArgumentValueInvald @a bytes is a  null pointer
/// @error Arcadia_Status_EncodingError the Bytes are not a type name
void
Arcadia_Names_parseTypeName
  (
    Arcadia_Thread* thread,
    const char *bytes,
    size_t numberOfBytes
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPENAMEPARSER_MODULE_H_INCLUDED
