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

#define ARCADIA_COLLECTIONS_MODULE (1)
#include "Arcadia/Collections/ImmutableMap.h"

#include "Arcadia/Collections/Include.h"

static void
Arcadia_ImmutableMap_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static void
Arcadia_ImmutableMap_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMapDispatch* self
  );

static void
Arcadia_ImmutableMap_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static void
Arcadia_ImmutableMap_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static void
Arcadia_ImmutableMap_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static Arcadia_SizeValue
Arcadia_ImmutableMap_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static Arcadia_BooleanValue
Arcadia_ImmutableMap_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static Arcadia_Value
Arcadia_ImmutableMap_getImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self,
    Arcadia_Value key
  );

static void
Arcadia_ImmutableMap_removeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self,
    Arcadia_Value key,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  );

static void
Arcadia_ImmutableMap_setImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self,
    Arcadia_Value key,
    Arcadia_Value value,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  );

static Arcadia_List*
Arcadia_ImmutableMap_getKeysImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static Arcadia_List*
Arcadia_ImmutableMap_getValuesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ImmutableMap_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ImmutableMap_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ImmutableMap_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ImmutableMap_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ImmutableMap", Arcadia_ImmutableMap,
                         u8"Arcadia.Map", Arcadia_Map,
                         &_typeOperations);

static void
Arcadia_ImmutableMap_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ImmutableMap);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->keys = NULL;
  self->values = NULL;
  self->size = 0;
  Arcadia_Map* other = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Map_getType(thread));
  Arcadia_List* keys = Arcadia_Map_getKeys(thread, other);
  self->size = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)keys);
  self->keys = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * self->size);
  self->values = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * self->size);
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_Value key = Arcadia_List_getAt(thread, keys, i);
    Arcadia_Value value = Arcadia_Map_get(thread, other, key);
    self->keys[i] = key;
    self->values[i] = value;
  }
  Arcadia_LeaveConstructor(Arcadia_ImmutableMap);
}

static void
Arcadia_ImmutableMap_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMapDispatch* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableMap_clearImpl;
  ((Arcadia_CollectionDispatch*)self)->getSize = (Arcadia_SizeValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableMap_getSizeImpl;
  ((Arcadia_CollectionDispatch*)self)->isImmutable = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableMap_isImmutableImpl;
  ((Arcadia_MapDispatch*)self)->get = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Map*, Arcadia_Value)) & Arcadia_ImmutableMap_getImpl;
  ((Arcadia_MapDispatch*)self)->remove = (void (*)(Arcadia_Thread*, Arcadia_Map*, Arcadia_Value, Arcadia_Value*, Arcadia_Value*)) & Arcadia_ImmutableMap_removeImpl;
  ((Arcadia_MapDispatch*)self)->set = (void (*)(Arcadia_Thread*, Arcadia_Map*, Arcadia_Value, Arcadia_Value, Arcadia_Value*, Arcadia_Value*)) & Arcadia_ImmutableMap_setImpl;
  ((Arcadia_MapDispatch*)self)->getKeys = (Arcadia_List * (*)(Arcadia_Thread*, Arcadia_Map*)) & Arcadia_ImmutableMap_getKeysImpl;
  ((Arcadia_MapDispatch*)self)->getValues = (Arcadia_List * (*)(Arcadia_Thread*, Arcadia_Map*)) & Arcadia_ImmutableMap_getValuesImpl;
}

static void
Arcadia_ImmutableMap_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{
  if (self->keys) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->keys);
    self->keys = NULL;
  }
  if (self->values) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->values);
    self->values = NULL;
  }
}

static void
Arcadia_ImmutableMap_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_Value_visit(thread, self->keys + i);
    Arcadia_Value_visit(thread, self->values + i);
  }
}

static void
Arcadia_ImmutableMap_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_SizeValue
Arcadia_ImmutableMap_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ImmutableMap_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{ return Arcadia_BooleanValue_True; }

static Arcadia_Value
Arcadia_ImmutableMap_getImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self,
    Arcadia_Value key
  )
{
  if (Arcadia_Value_isVoidValue(&key)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    if (Arcadia_Value_isEqualTo(thread, &self->keys[i], &key)) {
      return self->values[i];
    }
  }
  return Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}

static void
Arcadia_ImmutableMap_removeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self,
    Arcadia_Value key,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static void
Arcadia_ImmutableMap_setImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self,
    Arcadia_Value key,
    Arcadia_Value value,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_List*
Arcadia_ImmutableMap_getKeysImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_List_insertBack(thread, list, self->keys[i]);
  }
  return list;
}

static Arcadia_List*
Arcadia_ImmutableMap_getValuesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableMap* self
  )
{
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_List_insertBack(thread, list, self->values[i]);
  }
  return list;
}

Arcadia_ImmutableMap*
Arcadia_ImmutableMap_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  _Arcadia_BeginCreate(Arcadia_ImmutableMap);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_ImmutableMap);
}
