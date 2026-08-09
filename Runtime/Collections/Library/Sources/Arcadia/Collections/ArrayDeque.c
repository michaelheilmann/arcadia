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
#include "Arcadia/Collections/ArrayDeque.h"

#include "Arcadia/Collections/Include.h"

static Arcadia_BooleanValue g_initialized = Arcadia_BooleanValue_False;

static Arcadia_SizeValue g_minimumCapacity = -1;
static Arcadia_SizeValue g_maximumCapacity = -1;

// Macro-like function to compute max(x, y).
static inline Arcadia_SizeValue
MAX
  (
    Arcadia_SizeValue x,
    Arcadia_SizeValue y
  );

// Macro-like function to compute min(self, other)
static inline Arcadia_SizeValue
MIN
  (
    Arcadia_SizeValue x,
    Arcadia_SizeValue y
  );

// Macro-like function to compute index % capacity.
static inline Arcadia_SizeValue
MOD
  (
    Arcadia_SizeValue index,
    Arcadia_SizeValue capacity
  );

static inline Arcadia_SizeValue
MAX
  (
    Arcadia_SizeValue x,
    Arcadia_SizeValue y
  )
{ return x > y ? x : y; }

static inline Arcadia_SizeValue
MIN
  (
    Arcadia_SizeValue x,
    Arcadia_SizeValue y
  )
{ return x < y ? x : y; }

static inline Arcadia_SizeValue
MOD
  (
    Arcadia_SizeValue index,
    Arcadia_SizeValue capacity
  )
{ return index % capacity; }

struct Arcadia_ArrayDequeDispatch {
  Arcadia_DequeDispatch _parent;
};

struct Arcadia_ArrayDeque {
  Arcadia_Deque _parent;
  /// @brief A pointer to an array of @a capacity @a Arcadia_Value elements.
  Arcadia_Value* elements;
  /// @brief The capacity, in elements, of the array pointed to by @a array.
  Arcadia_SizeValue capacity;
  /// @brief The number of elements in this array.
  Arcadia_SizeValue size;
  /// @brief The position to read the next element from.
  Arcadia_SizeValue read;
};

static void
Arcadia_ArrayDeque_ensureFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue requiredFreeCapacity
  );

static void
Arcadia_ArrayDeque_ensureInitialized
  (
    Arcadia_Thread* thread
  );

static void
Arcadia_ArrayDeque_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static void
Arcadia_ArrayDeque_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDequeDispatch* self
  );

static void
Arcadia_ArrayDeque_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static void
Arcadia_ArrayDeque_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static void
Arcadia_ArrayDeque_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static Arcadia_SizeValue
Arcadia_ArrayDeque_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static Arcadia_BooleanValue
Arcadia_ArrayDeque_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static void
Arcadia_ArrayDeque_insertFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_Value value
  );

static void
Arcadia_ArrayDeque_insertBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_Value value
  );

static void
Arcadia_ArrayDeque_insertAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  );

static Arcadia_Value
Arcadia_ArrayDeque_getFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static Arcadia_Value
Arcadia_ArrayDeque_getBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static Arcadia_Value
Arcadia_ArrayDeque_getAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue index
  );

static void
Arcadia_ArrayDeque_removeFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static void
Arcadia_ArrayDeque_removeBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  );

static void
Arcadia_ArrayDeque_removeAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue index
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ArrayDeque_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ArrayDeque_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ArrayDeque_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ArrayDeque_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ArrayDeque", Arcadia_ArrayDeque,
                         u8"Arcadia.Deque", Arcadia_Deque,
                         &_typeOperations);

static void
Arcadia_ArrayDeque_ensureFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
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
  if (oldCapacity != newCapacity) {
    Arcadia_Value* oldElements = self->elements;
    Arcadia_Value* newElements = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * newCapacity);
    for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
      newElements[MOD(self->read + i, newCapacity)] = oldElements[MOD(self->read + i, self->capacity)];
    }
    Arcadia_Memory_deallocateUnmanaged(thread, oldElements);
    self->elements = newElements;
    self->capacity = newCapacity;
  }
}

static void
Arcadia_ArrayDeque_ensureInitialized
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
Arcadia_ArrayDeque_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  Arcadia_ArrayDeque_ensureInitialized(thread);
  Arcadia_EnterConstructor(Arcadia_ArrayDeque);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 == _numberOfArguments) {
    Arcadia_Deque* other = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Deque_getType(thread));
    Arcadia_SizeValue size = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)other);
    self->elements = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * size);
    for (Arcadia_SizeValue i = 0; i < size; ++i) {
      self->elements[i] = Arcadia_Deque_getAt(thread, other, i);
    }
    self->read = 0;
    self->size = size;
    self->capacity = size;
  } else if (0 == _numberOfArguments) {
    self->elements = NULL;
    self->read = 0;
    self->size = 0;
    self->capacity = g_minimumCapacity;
    self->elements = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Value) * self->capacity);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_ArrayDeque);
}

static void
Arcadia_ArrayDeque_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDequeDispatch* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ArrayDeque_clearImpl;
  ((Arcadia_CollectionDispatch*)self)->getSize = (Arcadia_SizeValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ArrayDeque_getSizeImpl;
  ((Arcadia_CollectionDispatch*)self)->isImmutable = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_ArrayDeque_isImmutableImpl;
  ((Arcadia_DequeDispatch*)self)->getAt = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_SizeValue)) & Arcadia_ArrayDeque_getAtImpl;
  ((Arcadia_DequeDispatch*)self)->getBack = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Deque*)) & Arcadia_ArrayDeque_getBackImpl;
  ((Arcadia_DequeDispatch*)self)->getFront = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Deque*)) & Arcadia_ArrayDeque_getFrontImpl;
  ((Arcadia_DequeDispatch*)self)->insertAt = (void(*)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_SizeValue, Arcadia_Value)) & Arcadia_ArrayDeque_insertAtImpl;
  ((Arcadia_DequeDispatch*)self)->insertBack = (void(*)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_Value)) & Arcadia_ArrayDeque_insertBackImpl;
  ((Arcadia_DequeDispatch*)self)->insertFront = (void(*)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_Value)) & Arcadia_ArrayDeque_insertFrontImpl;
  ((Arcadia_DequeDispatch*)self)->removeAt = (void(*)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_SizeValue)) & Arcadia_ArrayDeque_removeAtImpl;
  ((Arcadia_DequeDispatch*)self)->removeBack = (void(*)(Arcadia_Thread*, Arcadia_Deque*)) & Arcadia_ArrayDeque_removeBackImpl;
  ((Arcadia_DequeDispatch*)self)->removeFront = (void(*)(Arcadia_Thread*, Arcadia_Deque*)) & Arcadia_ArrayDeque_removeFrontImpl;
}

static void
Arcadia_ArrayDeque_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  if (self->elements) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->elements);
    self->elements = NULL;
  }
}

static void
Arcadia_ArrayDeque_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->size; i < n; ++i) {
    Arcadia_Value_visit(thread, &self->elements[MOD(self->read + i, self->capacity)]);
  }
}

static void
Arcadia_ArrayDeque_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  self->size = 0;
  self->read = 0;
}

static Arcadia_SizeValue
Arcadia_ArrayDeque_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ArrayDeque_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{ return Arcadia_BooleanValue_False; }

static void
Arcadia_ArrayDeque_insertFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_Value value
  )
{ Arcadia_Deque_insertAt(thread, (Arcadia_Deque*)self, 0, value); }

static void
Arcadia_ArrayDeque_insertBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_Value value
  )
{ Arcadia_Deque_insertAt(thread, (Arcadia_Deque*)self, self->size, value);}

static void
Arcadia_ArrayDeque_insertAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  )
{
  Arcadia_ArrayDeque_ensureFreeCapacity(thread, self, 1);

  Arcadia_SizeValue const capacity = self->capacity; // The capacity does not change anymore for the rest of this
                                                     // function but is frequently referenced, hence it is cached
                                                     // here.

  // The goal of the following algorithm is to minimize the number of shift operations when inserting into an index
  // smaller than size.

  // Assume the deque is not empty.
  // Treating the deque as an infinite array,
  // ..., a[0],...,a[read],...,a[read+index], ..., a[read+size-1]
  // then
  // a) the index of the first element is "read" and
  // b) the index of the last element is "read + size - 1" and
  // c) "read <= index <= read + size".
  //
  // So to insert at index "index", one can either shift up the elements
  // "a[read + index],... a[read+size-1]" or
  // shift down the elements
  // "a[read] ... a[read + index-1]"
  // to make room for the new element.
  //
  // The choice made here is to minimize the number of shifts:
  // if "index < size / 2" then "a[read] ... a[read + index - 1]" is shifted down
  // and otherwise "a[read + index],... a[read+size-1]" is shifted up.
  if (index < self->size / 2) {
    // We shift down:
    // If a' ist the new queue and a is the old queue, then the operation
    // can be described as follows:
    // - a'[read-1] = a[read], ..., a'[read - 1 + index] = a[read + index]
    // - a'[read+index] = newElement
    // - read' = read - 1
    // - size' = size + 1
    // As read' = read - 1, one can also write
    // - a'[read'] = a[read' + 1], ..., a'[read + index] = a[read' + index + 1]
    // which is the form used here.
    //Arcadia_SizeValue offset = self->read == 0 ? self->capacity - 1 : MOD(self->read - 1, capacity);
    for (Arcadia_SizeValue j = 0; j < index; ++j) {
      self->elements[MOD(self->read + j, capacity)] = self->elements[MOD(self->read + j + 1, capacity)];
    }
    self->read = self->read == 0 ? self->capacity - 1 : MOD(self->read - 1, capacity);
  }  else {
    // We shift up:
    // If a' ist the new queue and a is the old queue, then the operation
    // can be described as follows:
    // - a'[read + size] = a[read + size - 1], ..., a'[read + index + 1] = a[read + index]
    // - a'[read + index] = newElement
    // - read' = read
    // - size' = size+1
    Arcadia_SizeValue offset = self->read;
    for (Arcadia_SizeValue j = self->size; j > index; --j) {
      self->elements[MOD(offset + j, capacity)] = self->elements[MOD(offset + j - 1, capacity)];
    }
  }
  self->elements[MOD(self->read + index, capacity)] = value;
  self->size++;
}

static Arcadia_Value
Arcadia_ArrayDeque_getFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  return Arcadia_Deque_getAt(thread, (Arcadia_Deque*)self, 0);
}

static Arcadia_Value
Arcadia_ArrayDeque_getBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  if (!self || !self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid),
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Deque_getAt(thread, (Arcadia_Deque*)self, self->size - 1);
}

static Arcadia_Value
Arcadia_ArrayDeque_getAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue index
  )
{
  if (!self || index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid),
    Arcadia_Thread_jump(thread);
  }
  return self->elements[MOD(self->read + index, self->capacity)];
}

static void
Arcadia_ArrayDeque_removeFrontImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  Arcadia_Deque_removeAt(thread, (Arcadia_Deque*)self, 0);
}

static void
Arcadia_ArrayDeque_removeBackImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self
  )
{
  if (!self || !self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid),
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Deque_removeAt(thread, (Arcadia_Deque*)self, self->size - 1);
}

static void
Arcadia_ArrayDeque_removeAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ArrayDeque* self,
    Arcadia_SizeValue index
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue const capacity = self->capacity;
  if (index < self->size / 2) {
    // - a'[read] = a[read-1], ..., a'[read + index] = a[read + index - 1]
    // - read' = read + 1
    // - size' = size - 1
    Arcadia_SizeValue offset = self->read;
    for (Arcadia_SizeValue j = index; j > 0; --j) {
      self->elements[MOD(offset + j, capacity)] = self->elements[MOD(offset + j - 1, capacity)];
    }
    self->read = MOD(self->read + 1, capacity);
  } else {
    // - a'[read+index] = a[read+index+1], ..., a'[read+size-2] = a[read+size-1]
    // - read' = read
    // - size' = size - 1
    // shift a[i+1],..,a[n-1] left one position
    for (Arcadia_SizeValue j = index; j < self->size - 1; ++j) {
      self->elements[MOD(self->read + j, capacity)] = self->elements[MOD(self->read + j + 1, capacity)];
    }
  }
  self->size--;
}

Arcadia_ArrayDeque*
Arcadia_ArrayDeque_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ArrayDeque);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ArrayDeque);
}
