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

#if !defined(ARCADIA_COLLECTIONS_SET_H_INCLUDED)
#define ARCADIA_COLLECTIONS_SET_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Collection.h"
typedef struct Arcadia_List Arcadia_List;

Arcadia_declareObjectType(u8"Arcadia.Set", Arcadia_Set,
                          u8"Arcadia.Collection");

struct Arcadia_SetDispatch {
  Arcadia_CollectionDispatch _parent;

  void (*add)(Arcadia_Thread* thread, Arcadia_Set* self, Arcadia_Value value, Arcadia_Value* oldValue);
  Arcadia_BooleanValue(*contains)(Arcadia_Thread* thread, Arcadia_Set* self, Arcadia_Value value);
  Arcadia_Value(*get)(Arcadia_Thread* thread, Arcadia_Set* self, Arcadia_Value value);
  void (*remove)(Arcadia_Thread* thread, Arcadia_Set* self, Arcadia_Value value, Arcadia_Value* oldValue);
  void (*getAll)(Arcadia_Thread*, Arcadia_Set*, Arcadia_List*);
};

struct Arcadia_Set {
  Arcadia_Collection parent;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Set_add
void
Arcadia_Set_add
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Set_contains
Arcadia_BooleanValue
Arcadia_Set_contains
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Set_get
Arcadia_Value
Arcadia_Set_get
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Set_remove
void
Arcadia_Set_remove
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  );

void
Arcadia_Set_getAll
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_List* target
  );

#endif // ARCADIA_COLLECTIONS_SET_H_INCLUDED
