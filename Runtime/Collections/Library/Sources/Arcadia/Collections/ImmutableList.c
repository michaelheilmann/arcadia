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
#include "Arcadia/Collections/ImmutableList.h"

static void
Arcadia_ImmutableList_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static void
Arcadia_ImmutableList_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static void
Arcadia_ImmutableList_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static void
Arcadia_ImmutableList_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static void
Arcadia_ImmutableList_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static Arcadia_Value
Arcadia_ImmutableList_getAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_SizeValue index
  );

static Arcadia_SizeValue
Arcadia_ImmutableList_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static Arcadia_BooleanValue
Arcadia_ImmutableList_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  );

static void
Arcadia_ImmutableList_insertAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  );

static void
Arcadia_ImmutableList_insertBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_Value value
  );

static void
Arcadia_ImmutableList_insertFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_Value value
  );

static void
Arcadia_ImmutableList_removeAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_SizeValue index,
    Arcadia_SizeValue count
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ImmutableList_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ImmutableList_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ImmutableList_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ImmutableList_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ImmutableList", Arcadia_ImmutableList,
                         u8"Arcadia.List", Arcadia_List,
                         &_typeOperations);

static void
Arcadia_ImmutableList_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ImmutableList);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->size = 0;
  self->elements = NULL;
  Arcadia_List* other = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  self->size = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)other);
  self->elements = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * self->size);
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    self->elements[i] = Arcadia_List_getAt(thread, other, i);
  }
  Arcadia_LeaveConstructor(Arcadia_ImmutableList);
}

static void
Arcadia_ImmutableList_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableList_clearImpl;
  ((Arcadia_CollectionDispatch*)self)->getSize = (Arcadia_SizeValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableList_getSizeImpl;
  ((Arcadia_CollectionDispatch*)self)->isImmutable = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ImmutableList_isImmutableImpl;
  ((Arcadia_ListDispatch*)self)->getAt = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_List*, Arcadia_SizeValue)) & Arcadia_ImmutableList_getAtImpl;
  ((Arcadia_ListDispatch*)self)->insertAt = (void (*)(Arcadia_Thread*, Arcadia_List*, Arcadia_SizeValue, Arcadia_Value)) & Arcadia_ImmutableList_insertAtImpl;
  ((Arcadia_ListDispatch*)self)->insertBack = (void (*)(Arcadia_Thread*, Arcadia_List*, Arcadia_Value)) & Arcadia_ImmutableList_insertBackImpl;
  ((Arcadia_ListDispatch*)self)->insertFront = (void (*)(Arcadia_Thread*, Arcadia_List*, Arcadia_Value)) & Arcadia_ImmutableList_insertFrontImpl;
  ((Arcadia_ListDispatch*)self)->removeAt = (void (*)(Arcadia_Thread*, Arcadia_List*, Arcadia_SizeValue, Arcadia_SizeValue)) & Arcadia_ImmutableList_removeAtImpl;
}

static void
Arcadia_ImmutableList_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{
  if (self->elements) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->elements);
    self->elements = NULL;
  }
}

static void
Arcadia_ImmutableList_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_Value_visit(thread, self->elements + i);
  }
}

static void
Arcadia_ImmutableList_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_Value
Arcadia_ImmutableList_getAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_SizeValue index
  )
{
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return *(self->elements + index);
}

static Arcadia_SizeValue
Arcadia_ImmutableList_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ImmutableList_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self
  )
{ return Arcadia_BooleanValue_True; }

static void
Arcadia_ImmutableList_insertAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static void
Arcadia_ImmutableList_insertBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_Value value
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static void
Arcadia_ImmutableList_insertFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_Value value
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static void
Arcadia_ImmutableList_removeAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* self,
    Arcadia_SizeValue index,
    Arcadia_SizeValue count
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

Arcadia_ImmutableList*
Arcadia_ImmutableList_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value argument
  )
{
  _Arcadia_BeginCreate(Arcadia_ImmutableList);
  Arcadia_ValueStack_pushValue(thread, &argument);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_ImmutableList);
}
