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

#if !defined(ARCADIA_COLLECTIONS_MAP_H_INCLUDED)
#define ARCADIA_COLLECTIONS_MAP_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Collection.h"
typedef struct Arcadia_List Arcadia_List;

Arcadia_declareObjectType(u8"Arcadia.Map", Arcadia_Map,
                          u8"Arcadia.Collection");

struct Arcadia_MapDispatch {
  Arcadia_CollectionDispatch _parent;

  Arcadia_Value(*get)(Arcadia_Thread* thread, Arcadia_Map* self, Arcadia_Value key);
  void (*set)(Arcadia_Thread* thread, Arcadia_Map* self, Arcadia_Value key, Arcadia_Value value, Arcadia_Value* oldKey, Arcadia_Value* oldValue);
  void (*remove)(Arcadia_Thread* thread, Arcadia_Map* self, Arcadia_Value key, Arcadia_Value* oldKey, Arcadia_Value* oldValue);
  Arcadia_List* (*getKeys)(Arcadia_Thread* thread, Arcadia_Map* self);
  Arcadia_List* (*getValues)(Arcadia_Thread* thread, Arcadia_Map* self);
};

struct Arcadia_Map {
  Arcadia_Collection parent;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Map_get
Arcadia_Value
Arcadia_Map_get
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Map_set
void
Arcadia_Map_set
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key,
    Arcadia_Value value,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Map_remove
void
Arcadia_Map_remove
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  );

Arcadia_List*
Arcadia_Map_getKeys
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self
  );

Arcadia_List*
Arcadia_Map_getValues
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self
  );

/*@extension*/
static inline Arcadia_BooleanValue
Arcadia_Map_getBooleanValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key
  )
{
  Arcadia_Value value = Arcadia_Map_get(thread, self, key);
  if (!Arcadia_Value_isBooleanValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue booleanValue = Arcadia_Value_getBooleanValue(&value);
  return booleanValue;
}

/*@extension*/
static inline Arcadia_Natural64Value
Arcadia_Map_getNatural64ValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key
  )
{
  Arcadia_Value value = Arcadia_Map_get(thread, self, key);
  if (!Arcadia_Value_isNatural64Value(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Natural64Value natural64Value = Arcadia_Value_getNatural64Value(&value);
  return natural64Value;
}

/*@extension*/
static inline Arcadia_Object*
Arcadia_Map_getObjectReferenceValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key,
    Arcadia_Type* type
  )
{
  Arcadia_Value value = Arcadia_Map_get(thread, self, key);
  if (!Arcadia_Value_isObjectReferenceValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* objectValue = Arcadia_Value_getObjectReferenceValue(&value);
  if (!Arcadia_Object_isInstanceOf(thread, objectValue, type)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return objectValue;
}

#endif // ARCADIA_COLLECTIONS_MAP_H_INCLUDED
