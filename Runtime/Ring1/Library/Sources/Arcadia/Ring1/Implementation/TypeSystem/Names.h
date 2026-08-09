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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_NAMES_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_NAMES_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#include "Arcadia/Ring1/Implementation/Module.h"
#include <stdint.h>

typedef struct Arcadia_Name Arcadia_Name;

/// @brief Visit this name.
/// @param thread A pointer to this thread.
/// @param self A pointer to this name.
void
Arcadia_Name_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Name* self
  );

/// @brief Get the hash value of this name.
/// @param thread A pointer to this thread.
/// @param self A pointer to this name.
/// @return The hash value of this name.
size_t
Arcadia_Name_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_Name* self
  );

/// @brief Get the number of Bytes of this name.
/// @param thread A pointer to this thread.
/// @param self A pointer to this name.
/// @return The number of Bytes of this name.
size_t
Arcadia_Name_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Name* self
  );

/// @brief Get the Bytes of this name.
/// @param thread A pointer to this thread.
/// @param self A pointer to this name.
/// @return A pointer to the Bytes of this name.
/// @warning The Bytes are valid as long as the name is live.
const char*
Arcadia_Name_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Name* self
  );

/// @brief Get or create a name for the given Bytes.
/// @param thread A pointer to this thread.
/// @param bytes A pointer to an array of @a numberOfBytes Bytes.
/// @param numberOfBytes The number of Bytes in the array pointed to by @a bytes.
/// @return A pointer to the name.
Arcadia_Name*
Arcadia_Names_getOrCreateName
  (
    Arcadia_Thread* thread,
    char const* bytes,
    size_t numberOfBytes
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_NAMES_H_INCLUDED
