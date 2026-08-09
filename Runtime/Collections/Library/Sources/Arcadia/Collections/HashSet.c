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
#include "Arcadia/Collections/HashSet.h"

#include "Arcadia/Collections/Include.h"

static Arcadia_BooleanValue g_initialized = Arcadia_BooleanValue_False;

static Arcadia_SizeValue g_minimumCapacity = -1;
static Arcadia_SizeValue g_maximumCapacity = -1;

struct _Arcadia_HashSet_Node {
  _Arcadia_HashSet_Node* next;
  Arcadia_SizeValue hash;
  Arcadia_Value value;
};

static void
Arcadia_HashSet_ensureFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_SizeValue requiredFreeCapacity
  );

static void
Arcadia_HashSet_ensureInitialized
  (
    Arcadia_Thread* thread
  );

static void
Arcadia_HashSet_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  );

static void
Arcadia_HashSet_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSetDispatch* self
  );

static void
Arcadia_HashSet_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  );

static void
Arcadia_HashSet_visit
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  );

static void
Arcadia_HashSet_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  );

static Arcadia_SizeValue
Arcadia_HashSet_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  );

static Arcadia_BooleanValue
Arcadia_HashSet_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  );

static void
Arcadia_HashSet_getAllImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_List* target
  );

static void
Arcadia_HashSet_addImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  );

static Arcadia_BooleanValue
Arcadia_HashSet_containsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value
  );

static Arcadia_Value
Arcadia_HashSet_getImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value
  );

static void
Arcadia_HashSet_removeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_HashSet_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_HashSet_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_HashSet_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_HashSet_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.HashSet", Arcadia_HashSet,
                         u8"Arcadia.Set", Arcadia_Set,
                         &_typeOperations);

static void
Arcadia_HashSet_ensureFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
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
  _Arcadia_HashSet_Node** oldBuckets = self->buckets;
  _Arcadia_HashSet_Node** newBuckets = Arcadia_Memory_allocateUnmanaged(thread, sizeof(_Arcadia_HashSet_Node*) * newCapacity);
  for (Arcadia_SizeValue i = 0, n = newCapacity; i < n; ++i) {
    newBuckets[i] = NULL;
  }
  for (Arcadia_SizeValue i = 0, n = oldCapacity; i < n; ++i) {
    while (oldBuckets[i]) {
      _Arcadia_HashSet_Node* node = oldBuckets[i];
      oldBuckets[i] = oldBuckets[i]->next;
      Arcadia_SizeValue j = node->hash % newCapacity;
      node->next = newBuckets[j];
      newBuckets[j] = node;
    }
  }
  Arcadia_Memory_deallocateUnmanaged(thread, oldBuckets);
  self->buckets = newBuckets;
  self->capacity = newCapacity;
}

static void
Arcadia_HashSet_ensureInitialized
  (
    Arcadia_Thread* thread
  )
{
  if (!g_initialized) {
    g_minimumCapacity = 8;
    g_maximumCapacity = SIZE_MAX / sizeof(_Arcadia_HashSet_Node*);
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
Arcadia_HashSet_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    while (self->buckets[i]) {
      _Arcadia_HashSet_Node* node = self->buckets[i];
      self->buckets[i] = self->buckets[i]->next;
      Arcadia_Memory_deallocateUnmanaged(thread, node);
      node = NULL;
    }
  }
  if (self->buckets) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->buckets);
    self->buckets = NULL;
  }
}

static void
Arcadia_HashSet_visit
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  )
{
  if (self->buckets) {
    for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
      _Arcadia_HashSet_Node* node = self->buckets[i];
      while (node) {
        Arcadia_Value_visit(thread, &node->value);
        node = node->next;
      }
    }
  }
}

static void
Arcadia_HashSet_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  )
{
  Arcadia_HashSet_ensureInitialized(thread);
  Arcadia_EnterConstructor(Arcadia_HashSet);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->buckets = NULL;
  self->capacity = 0;
  self->size = 0;
  self->capacity = g_minimumCapacity;
  self->buckets = Arcadia_Memory_allocateUnmanaged(thread, sizeof(_Arcadia_HashSet_Node*) * self->capacity);
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    self->buckets[i] = NULL;
  }
  Arcadia_LeaveConstructor(Arcadia_HashSet);
}

static void
Arcadia_HashSet_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSetDispatch* self
  )
{
  ((Arcadia_CollectionDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_HashSet_clearImpl;
  ((Arcadia_CollectionDispatch*)self)->getSize = (Arcadia_SizeValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_HashSet_getSizeImpl;
  ((Arcadia_CollectionDispatch*)self)->isImmutable = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Collection*)) & Arcadia_HashSet_isImmutableImpl;
  ((Arcadia_SetDispatch*)self)->getAll = (void(*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_List*)) & Arcadia_HashSet_getAllImpl;
  ((Arcadia_SetDispatch*)self)->add = (void (*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value, Arcadia_Value*)) & Arcadia_HashSet_addImpl;
  ((Arcadia_SetDispatch*)self)->contains = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value)) & Arcadia_HashSet_containsImpl;
  ((Arcadia_SetDispatch*)self)->get = (Arcadia_Value(*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value)) & Arcadia_HashSet_getImpl;
  ((Arcadia_SetDispatch*)self)->remove = (void (*)(Arcadia_Thread*, Arcadia_Set*, Arcadia_Value, Arcadia_Value*)) & Arcadia_HashSet_removeImpl;
}

static void
Arcadia_HashSet_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    while (self->buckets[i]) {
      _Arcadia_HashSet_Node* node = self->buckets[i];
      self->buckets[i] = self->buckets[i]->next;
      Arcadia_Memory_deallocateUnmanaged(thread, node);
      node = NULL;
    }
  }
  self->size = 0;
}

static Arcadia_SizeValue
Arcadia_HashSet_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  )
{
  return self->size;
}

static Arcadia_BooleanValue
Arcadia_HashSet_isImmutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self
  )
{ return Arcadia_BooleanValue_False; }

static void
Arcadia_HashSet_getAllImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_List* target
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    for (_Arcadia_HashSet_Node* node = self->buckets[i]; NULL != node; node = node->next) {
      Arcadia_List_insertBack(thread, target, node->value);
    }
  }
}

static void
Arcadia_HashSet_addImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &value);
  Arcadia_SizeValue index = hash % self->capacity;
  _Arcadia_HashSet_Node* node = self->buckets[index];
  while (node) {
    if (Arcadia_Value_isEqualTo(thread, &value, &node->value)) {
      Arcadia_Value oldValueBackup = node->value;
      if (oldValue) *oldValue = oldValueBackup;
      node->value = value;
      return;
    }
    node = node->next;
  }
  node = Arcadia_Memory_allocateUnmanaged(thread, sizeof(_Arcadia_HashSet_Node));
  node->value = value;
  node->hash = hash;
  node->next = self->buckets[index];
  self->buckets[index] = node;
  self->size++;
  if (oldValue) *oldValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  return;
}

static Arcadia_BooleanValue
Arcadia_HashSet_containsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &value);
  Arcadia_SizeValue index = hash % self->capacity;
  for (_Arcadia_HashSet_Node* node = self->buckets[index]; NULL != node; node = node->next) {
    if (hash == node->hash && Arcadia_Value_isEqualTo(thread, &value, &node->value)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_Value
Arcadia_HashSet_getImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &value);
  Arcadia_SizeValue index = hash % self->capacity;
  _Arcadia_HashSet_Node** previous = &self->buckets[index];
  _Arcadia_HashSet_Node* current = self->buckets[index];
  while (current) {
    if (hash == current->hash && Arcadia_Value_isEqualTo(thread, &value, &current->value)) {
      return current->value;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  return Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}

static void
Arcadia_HashSet_removeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  )
{
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &value);
  Arcadia_SizeValue index = hash % self->capacity;
  _Arcadia_HashSet_Node** previous = &self->buckets[index];
  _Arcadia_HashSet_Node* current = self->buckets[index];
  while (current) {
    if (hash == current->hash && Arcadia_Value_isEqualTo(thread, &value, &current->value)) {
      Arcadia_Value oldValueBackup = current->value;
      *previous = current->next;
      self->size--;
      Arcadia_Memory_deallocateUnmanaged(thread, current);
      if (oldValue) *oldValue = oldValueBackup;
      return;
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  if (oldValue) *oldValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  return;
}

Arcadia_HashSet*
Arcadia_HashSet_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_HashSet);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_HashSet);
}

Arcadia_Value
Arcadia_HashSet_findFirst
  (
    Arcadia_Thread* thread,
    Arcadia_HashSet* self,
    Arcadia_Value context,
    Arcadia_BooleanValue(*predicate)(Arcadia_Thread* thread, Arcadia_Value context, Arcadia_Value element)
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    _Arcadia_HashSet_Node* node = self->buckets[i];
    while (node) {
      if ((*predicate)(thread, context, node->value)) {
        return node->value;
      }
      node = node->next;
    }
  }
  return Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}
