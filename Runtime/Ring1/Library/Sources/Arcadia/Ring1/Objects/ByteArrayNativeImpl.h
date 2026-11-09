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

#if !defined(ARCADIA_RING1_OBJECTS_BYTEARRAYNATIVEIMPL_H_INCLUDED)
#define ARCADIA_RING1_OBJECTS_BYTEARRAYNATIVEIMPL_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Objects/ByteArray.h"

// Internal native-backed implementation. Factory clients use Arcadia_ByteArray.
Arcadia_declareObjectType(u8"Arcadia.ByteArrayNativeImpl", Arcadia_ByteArrayNativeImpl,
                          u8"Arcadia.ByteArray");

struct Arcadia_ByteArrayNativeImplDispatch {
  Arcadia_ByteArrayDispatch _parent;
};

struct Arcadia_ByteArrayNativeImpl {
  Arcadia_ByteArray _parent;
  Arcadia_Natural8Value* bytes;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
  Arcadia_BooleanValue resizable;
};

Arcadia_ByteArrayNativeImpl*
Arcadia_ByteArrayNativeImpl_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue size,
    Arcadia_BooleanValue resizable
  );

#endif // ARCADIA_RING1_OBJECTS_BYTEARRAYNATIVEIMPL_H_INCLUDED
