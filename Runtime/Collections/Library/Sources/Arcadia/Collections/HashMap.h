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

#if !defined(ARCADIA_COLLECTIONS_HASHMAP_H_INCLUDED)
#define ARCADIA_COLLECTIONS_HASHMAP_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Map.h"

typedef struct _Arcadia_HashMap_Node _Arcadia_HashMap_Node;

Arcadia_declareObjectType(u8"Arcadia.HashMap", Arcadia_HashMap,
                          u8"Arcadia.Map");

struct Arcadia_HashMapDispatch {
  Arcadia_MapDispatch _parent;
};

struct Arcadia_HashMap {
  Arcadia_Map _parent;
  _Arcadia_HashMap_Node** buckets;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_HashMap_create
Arcadia_HashMap*
Arcadia_HashMap_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  );

#endif // ARCADIA_COLLECTIONS_HASHMAP_H_INCLUDED
