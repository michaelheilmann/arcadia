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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Objects/ByteArrayBuilder.h"

#include "Arcadia/Ring1/Objects/ByteArrayNativeImpl.h"
#include "Arcadia/Ring1/Include.h"
#include <stdint.h>

static void
Arcadia_ByteArrayBuilder_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  );

static void
Arcadia_ByteArrayBuilder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilderDispatch* self
  );

static void
Arcadia_ByteArrayBuilder_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  );

static void
Arcadia_ByteArrayBuilder_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  );

static void
Arcadia_ByteArrayBuilder_syncStorage
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{
  Arcadia_ByteArrayNativeImpl* storage = (Arcadia_ByteArrayNativeImpl*)self->storage;
  self->p = (char*)storage->bytes;
  self->sz = storage->size;
  self->cp = storage->capacity;
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteArrayBuilder_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ByteArrayBuilder_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ByteArrayBuilder_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ByteArrayBuilder_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteArrayBuilder", Arcadia_ByteArrayBuilder,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ByteArrayBuilder_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ByteArrayBuilder);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->p = NULL;
  self->sz = 0;
  self->cp = 0;
  self->storage = NULL;
  // The native-backed ByteArray owns the allocation; this builder is an API facade.
  self->storage = Arcadia_ByteArrayFactory_createResizable(thread, 0);
  Arcadia_ByteArrayBuilder_syncStorage(thread, self);
  Arcadia_LeaveConstructor(Arcadia_ByteArrayBuilder);
}

static void
Arcadia_ByteArrayBuilder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilderDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ByteArrayBuilder_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{/* Storage is owned by the managed ByteArray, not by this facade. */}

static void
Arcadia_ByteArrayBuilder_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{
  if (self->storage) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->storage);
  }
}

Arcadia_ByteArrayBuilder*
Arcadia_ByteArrayBuilder_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ByteArrayBuilder);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ByteArrayBuilder);
}

Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_endsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  return Arcadia_Memory_endsWith(thread, self->p, self->sz, bytes, numberOfBytes);
}

Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_startsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  return Arcadia_Memory_startsWith(thread, self->p, self->sz, bytes, numberOfBytes);
}

void
Arcadia_ByteArrayBuilder_insertBackBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ByteArrayBuilder_insertBytes(thread, self, self->sz, bytes, numberOfBytes);
}

void
Arcadia_ByteArrayBuilder_insertFrontBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ByteArrayBuilder_insertBytes(thread, self, Arcadia_SizeValue_Literal(0), bytes, numberOfBytes);
}

void
Arcadia_ByteArrayBuilder_insertBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self,
    Arcadia_SizeValue index,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!self || !bytes || index > self->sz) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!numberOfBytes) {
    return;
  }
  Arcadia_SizeValue sumHigh, sumLow;
  Arcadia_safeAddFullSizeValue(thread, self->sz, numberOfBytes, &sumHigh, &sumLow);
  if (sumHigh) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  // Remember the source offset before resize: the allocation may relocate.
  // Self-insertion is supported even when the source straddles the insertion.
  // A pointer into unused capacity cannot be a valid source range: resize
  // could relocate the allocation before the invalid source is accessed.
  uintptr_t address = (uintptr_t)bytes;
  uintptr_t base = (uintptr_t)self->p;
  Arcadia_BooleanValue alias = address >= base && address - base < self->cp;
  Arcadia_SizeValue sourceOffset = 0;
  if (alias) {
    sourceOffset = (Arcadia_SizeValue)(address - base);
    if (sourceOffset >= self->sz || numberOfBytes > self->sz - sourceOffset) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (address < base && numberOfBytes > base - address) {
    // A source range from before this allocation must not overlap it.
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue oldSize = self->sz;
  Arcadia_ByteArray_resize(thread, self->storage, sumLow);
  Arcadia_ByteArrayBuilder_syncStorage(thread, self);
  if (index < oldSize) {
    Arcadia_Memory_copy(thread, self->p + index + numberOfBytes, self->p + index, oldSize - index);
  }
  if (!alias) {
    Arcadia_Memory_copy(thread, self->p + index, bytes, numberOfBytes);
  } else if (sourceOffset + numberOfBytes <= index) {
    // Source lies entirely before the inserted gap.
    Arcadia_Memory_copy(thread, self->p + index, self->p + sourceOffset, numberOfBytes);
  } else if (sourceOffset >= index) {
    // Source has moved right by the inserted length.
    Arcadia_Memory_copy(thread, self->p + index, self->p + sourceOffset + numberOfBytes, numberOfBytes);
  } else {
    // Source crosses the gap. Each source segment is copied from its new location.
    Arcadia_SizeValue beforeGap = index - sourceOffset;
    Arcadia_Memory_copy(thread, self->p + index, self->p + sourceOffset, beforeGap);
    Arcadia_Memory_copy(thread, self->p + index + beforeGap,
                        self->p + index + numberOfBytes, numberOfBytes - beforeGap);
  }
}

Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self,
    Arcadia_ByteArrayBuilder const* other
  )
{
  if (self == other) {
    return Arcadia_BooleanValue_True;
  }
  if (self->sz == other->sz) {
    return self->sz == 0 || !Arcadia_Memory_compare(thread, self->p, other->p, self->sz);
  } else {
    return Arcadia_BooleanValue_False;
  }
}

Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_isEqualTo_pn
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!self || !bytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->sz == numberOfBytes) {
    return numberOfBytes == 0 || !Arcadia_Memory_compare(thread, self->p, bytes, numberOfBytes);
  } else {
    return Arcadia_BooleanValue_False;
  }
}

void
Arcadia_ByteArrayBuilder_clear
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{
  Arcadia_ByteArray_resize(thread, self->storage, 0);
  Arcadia_ByteArrayBuilder_syncStorage(thread, self);
}

Arcadia_SizeValue
Arcadia_ByteArrayBuilder_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self
  )
{
  return Arcadia_ByteArray_getSize(thread, self->storage);
}

Arcadia_SizeValue
Arcadia_ByteArrayBuilder_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self
  )
{ return Arcadia_ByteArray_getSize(thread, self->storage); }

Arcadia_Natural8Value const*
Arcadia_ByteArrayBuilder_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self
  )
{ return Arcadia_ByteArray_getBytes(thread, self->storage); }

Arcadia_Natural8Value
Arcadia_ByteArrayBuilder_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self,
    Arcadia_SizeValue index
  )
{
  return Arcadia_ByteArray_getAt(thread, self->storage, index);
}

Arcadia_ByteArray*
Arcadia_ByteArrayBuilder_toByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{ return Arcadia_ByteArrayFactory_copyImmutable(thread, self->storage); }
