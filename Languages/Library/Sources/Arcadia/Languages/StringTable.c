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

#define ARCADIA_LANGUAGES_MODULE (1)
#include "Arcadia/Languages/StringTable.h"

#include <string.h>

struct Arcadia_Languages_StringTable_Node {
  Arcadia_Languages_StringTable_Node* next;
  // We need to compare the hash of the Bytes in a string buffer with the hash of the Bytes in a string.
  // The hash function of string buffer and string cannot be used for that.
  Arcadia_SizeValue hash;
  Arcadia_String* string;
};

static void
Arcadia_Languages_StringTable_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  );

static void
Arcadia_Languages_StringTable_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTableDispatch* self
  );

static void
Arcadia_Languages_StringTable_maybeResize_nojump
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  );

static Arcadia_SizeValue
Arcadia_Languages_StringTable_hashBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

static void
Arcadia_Languages_StringTable_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  );

static void
Arcadia_Languages_StringTable_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  );

static Arcadia_Languages_StringTable*
Arcadia_Languages_StringTable_create
  (
    Arcadia_Thread* thread
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_StringTable_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Languages_StringTable_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_StringTable_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_StringTable_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.StringTable", Arcadia_Languages_StringTable,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Languages_StringTable_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_StringTable);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->buckets = NULL;
  self->size = Arcadia_SizeValue_Literal(0);
  self->capacity = Arcadia_SizeValue_Literal(0);
  static Arcadia_SizeValue const g_defaultCapacity = 8;
  self->buckets = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Languages_StringTable_Node*) * g_defaultCapacity);
  for (Arcadia_SizeValue i = 0, n = g_defaultCapacity; i < n; ++i) {
    self->buckets[i] = NULL;
  }
  self->capacity = g_defaultCapacity;
  Arcadia_LeaveConstructor(Arcadia_Languages_StringTable);
}

static void
Arcadia_Languages_StringTable_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTableDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_StringTable_maybeResize_nojump
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  )
{
  if (self->size >= self->capacity) {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_SizeValue maximumCapacity = Arcadia_SizeValue_Maximum / sizeof(Arcadia_Languages_StringTable_Node*);
      Arcadia_SizeValue oldCapacity = self->capacity;
      Arcadia_SizeValue newCapacity;
      if (oldCapacity > maximumCapacity / 2) {
        if (oldCapacity == maximumCapacity) {
          return; // Cannot grow. Still a success.
        }
        newCapacity = maximumCapacity;
      } else {
        newCapacity = oldCapacity * 2;
      }
      Arcadia_Languages_StringTable_Node** oldBuckets = self->buckets;
      Arcadia_Languages_StringTable_Node** newBuckets = NULL;
      newBuckets = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Languages_StringTable_Node*) * newCapacity);
      for (Arcadia_SizeValue i = 0, n = newCapacity; i < n; ++i) {
        newBuckets[i] = NULL;
      }
      for (Arcadia_SizeValue oldIndex = 0, n = oldCapacity; oldIndex < n; ++oldIndex) {
        while (oldBuckets[oldIndex]) {
          Arcadia_Languages_StringTable_Node* node = oldBuckets[oldIndex];
          oldBuckets[oldIndex] = oldBuckets[oldIndex]->next;
          Arcadia_Value v = Arcadia_Value_makeObjectReferenceValue(node->string);
          Arcadia_SizeValue hash = Arcadia_Value_getHash(thread, &v);
          Arcadia_SizeValue newIndex = hash % newCapacity;
          node->next = newBuckets[newIndex];
          newBuckets[newIndex] = node;
        }
      }
      Arcadia_Memory_deallocateUnmanaged(thread, oldBuckets);
      self->buckets = newBuckets;
      self->capacity = newCapacity;
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      // Fail silently.
    }
  }
}

static Arcadia_SizeValue
Arcadia_Languages_StringTable_hashBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_SizeValue hash = numberOfBytes;
  for (Arcadia_SizeValue i = 0, n = numberOfBytes; i < n; ++i) {
    hash = hash * 37 + bytes[i];
  }
  return hash;
}

static void
Arcadia_Languages_StringTable_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    Arcadia_Languages_StringTable_Node* node = self->buckets[i];
    while (node) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)node->string);
      node = node->next;
    }
  }
}

static void
Arcadia_Languages_StringTable_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self
  )
{
  for (Arcadia_SizeValue i = 0, n = self->capacity; i < n; ++i) {
    while (self->buckets[i]) {
      Arcadia_Languages_StringTable_Node* node = self->buckets[i];
      self->buckets[i] = self->buckets[i]->next;
      Arcadia_Memory_deallocateUnmanaged(thread, node);
    }
  }
}

static Arcadia_Languages_StringTable*
Arcadia_Languages_StringTable_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Languages_StringTable);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Languages_StringTable);
}

Arcadia_String*
Arcadia_Languages_StringTable_getOrCreateString
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self,
    Arcadia_StringBuilder* stringBuffer
  )
{ return Arcadia_Languages_StringTable_getOrCreateStringFromBytes(thread, self, Arcadia_StringBuilder_getBytes(thread, stringBuffer), Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuffer)); }

Arcadia_String*
Arcadia_Languages_StringTable_getOrCreateStringFromBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_SizeValue hash = Arcadia_Languages_StringTable_hashBytes(thread, bytes, numberOfBytes);
  Arcadia_SizeValue index = hash % self->capacity;
  for (Arcadia_Languages_StringTable_Node* node = self->buckets[index]; NULL != node; node = node->next) {
    Arcadia_Value nodeValue = Arcadia_Value_makeObjectReferenceValue(node->string);
    if (node->hash == hash &&
      Arcadia_String_getNumberOfBytes(thread, node->string) == numberOfBytes) {
      if (!Arcadia_Memory_compare(thread, Arcadia_String_getBytes(thread, node->string), bytes, numberOfBytes)) {
        return node->string;
      }
    }
  }
  Arcadia_String* string = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, bytes, numberOfBytes)));
  Arcadia_Languages_StringTable_Node* node = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_Languages_StringTable_Node));
  node->string = string;
  node->hash = hash;
  node->next = self->buckets[index];
  self->buckets[index] = node;
  self->size++;

  Arcadia_Languages_StringTable_maybeResize_nojump(thread, self);

  return string;
}

Arcadia_String*
Arcadia_Languages_StringTable_getOrCreateStringFromCxxString
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* self,
    const char* string
  )
{ return Arcadia_Languages_StringTable_getOrCreateStringFromBytes(thread, self, string, strlen(string)); }

static Arcadia_Languages_StringTable* g_instance = NULL;

static void
Arcadia_Languages_StringTable_destroyCallback
  (
    void* observer,
    void* observed
  )
{ g_instance = NULL; }

Arcadia_Languages_StringTable*
Arcadia_Languages_StringTable_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_Languages_StringTable* instance = Arcadia_Languages_StringTable_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*)) &Arcadia_Languages_StringTable_destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}
