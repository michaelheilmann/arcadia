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
#include "Arcadia/Collections/ArrayStack.h"

#include "Arcadia/Collections/Include.h"

static Arcadia_BooleanValue g_initialized = Arcadia_BooleanValue_False;

static Arcadia_SizeValue g_minimumCapacity = -1;
static Arcadia_SizeValue g_maximumCapacity = -1;

static void
Arcadia_ArrayStack_ensureFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self,
    Arcadia_SizeValue requiredFreeCapacity
  );

static void
Arcadia_ArrayStack_ensureInitialized
  (
    Arcadia_Thread* thread
  );

static void
Arcadia_ArrayStack_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static void
Arcadia_ArrayStack_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static void
Arcadia_ArrayStack_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static void
Arcadia_ArrayStack_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStackDispatch* self
  );

static void
Arcadia_ArrayStack_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static Arcadia_SizeValue
Arcadia_ArrayStack_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static Arcadia_BooleanValue
Arcadia_ArrayStack_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static Arcadia_Value
Arcadia_ArrayStack_peekImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );


static Arcadia_Value
Arcadia_ArrayStack_peekAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self,
    Arcadia_SizeValue index
  );

static void
Arcadia_ArrayStack_popImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  );

static void
Arcadia_ArrayStack_pushImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self,
    Arcadia_Value value
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ArrayStack_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ArrayStack_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ArrayStack_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ArrayStack_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ArrayStack", Arcadia_ArrayStack,
                         u8"Arcadia.Stack", Arcadia_Stack,
                         &_typeOperations);

static void
Arcadia_ArrayStack_ensureFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self,
    Arcadia_SizeValue requiredFreeCapacity
  )
{
  Arcadia_SizeValue newAvailableFreeCapacity = self->capacity - self->size;
  Arcadia_SizeValue oldCapacity = self->capacity;
  Arcadia_SizeValue newCapacity = oldCapacity;
  while (requiredFreeCapacity > newAvailableFreeCapacity) {
    oldCapacity = self->capacity;
    newCapacity = oldCapacity;
    if (oldCapacity > g_maximumCapacity / 2) {
      // If oldCapacity > maximumCapacity / 2 holds then oldCapacity * 2 > maximumCapacity holds.
      // Consequently, we cannot double the capacity. Try to saturate the capacity.
      if (oldCapacity == g_maximumCapacity) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      } else {
        newCapacity = g_maximumCapacity;
      }
    } else {
      newCapacity = oldCapacity * 2;
    }
    newAvailableFreeCapacity = newCapacity - self->size;
  }
  Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->elements, sizeof(Arcadia_Value) * newCapacity);
  self->capacity = newCapacity;
}

static void
Arcadia_ArrayStack_ensureInitialized
  (
    Arcadia_Thread* thread
  )
{
  if (!g_initialized) {
    g_minimumCapacity = 8;
    g_maximumCapacity = SIZE_MAX / sizeof(Arcadia_Value);
    if (g_maximumCapacity > Arcadia_Integer32Value_Maximum) {
      g_maximumCapacity = Arcadia_Integer32Value_Maximum;
    }
    if (g_minimumCapacity > g_maximumCapacity) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    g_initialized = Arcadia_BooleanValue_True;
  }
}

static void
Arcadia_ArrayStack_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{
  Arcadia_ArrayStack_ensureInitialized(thread);
  Arcadia_EnterConstructor(Arcadia_ArrayStack);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elements = NULL;
  self->capacity = 0;
  self->size = 0;
  self->capacity = g_minimumCapacity;
  self->elements = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * self->capacity);
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    Arcadia_Value_setVoidValue(self->elements + i, Arcadia_VoidValue_Void);
  }
  Arcadia_LeaveConstructor(Arcadia_ArrayStack);
}

static void
Arcadia_ArrayStack_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStackDispatch* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ArrayStack_clearImpl;
  ((Arcadia_CollectionDispatch*)self)->getSize = (Arcadia_SizeValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ArrayStack_getSizeImpl;
  ((Arcadia_CollectionDispatch*)self)->isImmutable = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ArrayStack_isImmutableImpl;
  ((Arcadia_StackDispatch*)self)->peek = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Stack*)) & Arcadia_ArrayStack_peekImpl;
  ((Arcadia_StackDispatch*)self)->peekAt = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Stack*, Arcadia_SizeValue)) & Arcadia_ArrayStack_peekAtImpl;
  ((Arcadia_StackDispatch*)self)->pop = (void (*)(Arcadia_Thread*, Arcadia_Stack*)) & Arcadia_ArrayStack_popImpl;
  ((Arcadia_StackDispatch*)self)->push = (void (*)(Arcadia_Thread*, Arcadia_Stack*, Arcadia_Value)) & Arcadia_ArrayStack_pushImpl;
}

static void
Arcadia_ArrayStack_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{
  if (self->elements) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->elements);
    self->elements = NULL;
  }
}

static void
Arcadia_ArrayStack_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{
  if (self->elements) {
    for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
      Arcadia_Value_visit(thread, self->elements + i);
    }
  }
}

static void
Arcadia_ArrayStack_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{ self->size = 0; }

static Arcadia_SizeValue
Arcadia_ArrayStack_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ArrayStack_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{ return Arcadia_BooleanValue_False; }

static Arcadia_Value
Arcadia_ArrayStack_peekImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{
  if (0 == self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->elements[self->size - 1];
}

static Arcadia_Value
Arcadia_ArrayStack_peekAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self,
    Arcadia_SizeValue index
  )
{
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->elements[self->size - index - 1];
}

static void
Arcadia_ArrayStack_popImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self
  )
{
  if (0 == self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  --self->size;
}

static void
Arcadia_ArrayStack_pushImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayStack* self,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    return;
  }
  if (self->capacity == self->size) {
    Arcadia_ArrayStack_ensureFreeCapacity(thread, self, Arcadia_SizeValue_Literal(1));
  }
  self->elements[self->size++] = value;
}

Arcadia_ArrayStack*
Arcadia_ArrayStack_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ArrayStack);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ArrayStack);
}
