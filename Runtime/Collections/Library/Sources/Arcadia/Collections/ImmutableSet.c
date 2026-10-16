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
#include "Arcadia/Collections/ImmutableSet.h"

#include "Arcadia/Collections/Include.h"

static void
Arcadia_ImmutableSet_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  );

static void
Arcadia_ImmutableSet_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSetDispatch* self
  );

static void
Arcadia_ImmutableSet_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  );

static void
Arcadia_ImmutableSet_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  );

static void
Arcadia_ImmutableSet_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  );

static Arcadia_SizeValue
Arcadia_ImmutableSet_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  );

static Arcadia_BooleanValue
Arcadia_ImmutableSet_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  );

static void
Arcadia_ImmutableSet_addImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  );

static Arcadia_BooleanValue
Arcadia_ImmutableSet_containsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value
  );

static Arcadia_Value
Arcadia_ImmutableSet_getImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value
  );

static void
Arcadia_ImmutableSet_removeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  );

static void
Arcadia_ImmutableSet_getAllImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_List* target
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ImmutableSet_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ImmutableSet_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ImmutableSet_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ImmutableSet_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ImmutableSet", Arcadia_ImmutableSet,
                         u8"Arcadia.Set", Arcadia_Set,
                         &_typeOperations);

static void
Arcadia_ImmutableSet_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ImmutableSet);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elements = NULL;
  self->size = 0;
  Arcadia_Set* other = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Set_getType(thread));
  Arcadia_List* elements = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Set_getAll(thread, other, elements);
  self->size = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)elements);
  self->elements = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * self->size);
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    self->elements[i] = Arcadia_List_getAt(thread, elements, i);
  }
  Arcadia_LeaveConstructor(Arcadia_ImmutableSet);
}

static void
Arcadia_ImmutableSet_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSetDispatch* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableSet_clearImpl;
  ((Arcadia_CollectionDispatch*)self)->getSize = (Arcadia_SizeValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableSet_getSizeImpl;
  ((Arcadia_CollectionDispatch*)self)->isImmutable = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableSet_isImmutableImpl;
  ((Arcadia_SetDispatch*)self)->add = (void (*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value, Arcadia_Value*)) & Arcadia_ImmutableSet_addImpl;
  ((Arcadia_SetDispatch*)self)->contains = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value)) & Arcadia_ImmutableSet_containsImpl;
  ((Arcadia_SetDispatch*)self)->get = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value)) & Arcadia_ImmutableSet_getImpl;
  ((Arcadia_SetDispatch*)self)->remove = (void (*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value, Arcadia_Value*)) & Arcadia_ImmutableSet_removeImpl;
  ((Arcadia_SetDispatch*)self)->getAll = (void (*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_List*)) & Arcadia_ImmutableSet_getAllImpl;
}

static void
Arcadia_ImmutableSet_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  )
{
  if (self->elements) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->elements);
    self->elements = NULL;
  }
}

static void
Arcadia_ImmutableSet_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_Value_visit(thread, self->elements + i);
  }
}

static void
Arcadia_ImmutableSet_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_SizeValue
Arcadia_ImmutableSet_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  )
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ImmutableSet_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self
  )
{ return Arcadia_BooleanValue_True; }

static void
Arcadia_ImmutableSet_addImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_BooleanValue
Arcadia_ImmutableSet_containsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    if (Arcadia_Value_isEqualTo(thread, &self->elements[i], &value)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_Value
Arcadia_ImmutableSet_getImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    if (Arcadia_Value_isEqualTo(thread, &self->elements[i], &value)) {
      return self->elements[i];
    }
  }
  return Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}

static void
Arcadia_ImmutableSet_removeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static void
Arcadia_ImmutableSet_getAllImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableSet* self,
    Arcadia_List* target
  )
{
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_List_insertBack(thread, target, self->elements[i]);
  }
}

Arcadia_ImmutableSet*
Arcadia_ImmutableSet_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  _Arcadia_BeginCreate(Arcadia_ImmutableSet);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_ImmutableSet);
}
