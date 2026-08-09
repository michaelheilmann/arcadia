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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEARRAYSLICEIMPL_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEARRAYSLICEIMPL_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Objects/ByteArray.h"

/// @code
/// class Arcadia.ByteArraySliceImpl extends Arcadia.ByteArray {
///   construct(byteArray:Arcadia.ByteArray, start, size:Arcadia.Size)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.ByteArraySliceImpl", Arcadia_ByteArraySliceImpl,
                          u8"Arcadia.ByteArray");

struct Arcadia_ByteArraySliceImplDispatch {
  Arcadia_ByteArrayDispatch _parent;
};

struct Arcadia_ByteArraySliceImpl {
  Arcadia_ByteArray _parent;
  Arcadia_ByteArray* byteArray;
  Arcadia_SizeValue start;
  Arcadia_SizeValue size;
};

Arcadia_ByteArraySliceImpl*
Arcadia_ByteArraySliceImpl_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* byteArray,
    Arcadia_SizeValue start,
    Arcadia_SizeValue size
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTS_BYTEARRAYSLICEIMPL_H_INCLUDED
