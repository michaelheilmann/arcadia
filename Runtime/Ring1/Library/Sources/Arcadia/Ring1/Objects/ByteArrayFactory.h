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

#if !defined(ARCADIA_RING1_OBJECTS_BYTEARRAYFACTORY_H_INCLUDED)
#define ARCADIA_RING1_OBJECTS_BYTEARRAYFACTORY_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Objects/ByteArray.h"

// Factories return ByteArray objects; concrete storage types are internal details.
// Immutable RuntimeByteArray wrapping is zero-copy and kept alive by GC visitation.
Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createImmutable(Arcadia_Thread* thread, Arcadia_RuntimeByteArray* source);

// Newly allocated byte arrays are zero initialized. They own their native storage.
Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createMutable(Arcadia_Thread* thread, Arcadia_SizeValue size);

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createResizable(Arcadia_Thread* thread, Arcadia_SizeValue size);

// Copies always own independent storage, regardless of the source's capabilities.
Arcadia_ByteArray*
Arcadia_ByteArrayFactory_copyMutable(Arcadia_Thread* thread, Arcadia_ByteArray* source);

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_copyResizable(Arcadia_Thread* thread, Arcadia_ByteArray* source);

// Always produces an independent immutable snapshot, including for mutable sources.
// The source must be contiguous and remains rooted during the copy.
Arcadia_ByteArray*
Arcadia_ByteArrayFactory_copyImmutable(Arcadia_Thread* thread, Arcadia_ByteArray* source);

#endif // ARCADIA_RING1_OBJECTS_BYTEARRAYFACTORY_H_INCLUDED
