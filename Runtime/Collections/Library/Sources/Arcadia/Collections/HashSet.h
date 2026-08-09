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

#if !defined(ARCADIA_COLLECTIONS_HASHSET_H_INCLUDED)
#define ARCADIA_COLLECTIONS_HASHSET_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Set.h"

typedef struct _Arcadia_HashSet_Node _Arcadia_HashSet_Node;

Arcadia_declareObjectType(u8"Arcadia.HashSet", Arcadia_HashSet,
                          u8"Arcadia.Set");

struct Arcadia_HashSetDispatch {
  Arcadia_SetDispatch _parent;
};

struct Arcadia_HashSet {
  Arcadia_Set _parent;
  _Arcadia_HashSet_Node** buckets;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_HashSet_create
Arcadia_HashSet*
Arcadia_HashSet_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Value
Arcadia_HashSet_findFirst
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value context,
    Arcadia_BooleanValue (*predicate)(Arcadia_Thread* thread, Arcadia_Value context, Arcadia_Value element)
  );

#endif // ARCADIA_COLLECTIONS_HASHSET_H_INCLUDED
