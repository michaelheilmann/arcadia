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
#include "Arcadia/Ring1/Objects/ByteArrayNativeImpl.h"
#include "Arcadia/Ring1/Include.h"
#include <stdint.h>

static void
Arcadia_ByteArrayNativeImpl_construct(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self);

static void
Arcadia_ByteArrayNativeImpl_destruct(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self);

static void
Arcadia_ByteArrayNativeImpl_initializeDispatch(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImplDispatch* self);

static Arcadia_Natural8Value
Arcadia_ByteArrayNativeImpl_getAt(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl const* self, Arcadia_SizeValue index)
{
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->bytes[index];
}

static Arcadia_Natural8Value const*
Arcadia_ByteArrayNativeImpl_getBytes(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl const* self)
{ return self->bytes; }

static Arcadia_Natural8Value*
Arcadia_ByteArrayNativeImpl_getMutableBytes(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self)
{ return self->bytes; }

static Arcadia_SizeValue
Arcadia_ByteArrayNativeImpl_getSize(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl const* self)
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ByteArrayNativeImpl_isEmpty(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl const* self)
{ return self->size == 0; }

static Arcadia_BooleanValue
Arcadia_ByteArrayNativeImpl_isMutable(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl const* self)
{ return Arcadia_BooleanValue_True; }

static Arcadia_BooleanValue
Arcadia_ByteArrayNativeImpl_isResizable(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl const* self)
{ return self->resizable; }

static void
Arcadia_ByteArrayNativeImpl_setAt(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self,
                                  Arcadia_SizeValue index, Arcadia_Natural8Value value)
{
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->bytes[index] = value;
}

static void
Arcadia_ByteArrayNativeImpl_resize(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self,
                                   Arcadia_SizeValue size)
{
  if (!self->resizable) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (size > self->capacity) {
    // Grow geometrically to avoid one allocation per appended byte. No field is
    // changed until reallocation succeeds, preserving the old allocation on error.
    Arcadia_SizeValue capacity = self->capacity;
    if (!capacity) {
      capacity = 1;
    }
    while (capacity < size) {
      if (capacity > SIZE_MAX / 2) {
        capacity = size;
        break;
      }
      capacity *= 2;
    }
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->bytes, capacity);
    self->capacity = capacity;
  }
  if (size > self->size) {
    Arcadia_Memory_fillZero(thread, self->bytes + self->size, size - self->size);
  }
  self->size = size;
}

static void
Arcadia_ByteArrayNativeImpl_insertBytes(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self,
                                       Arcadia_SizeValue index, void const* bytes, Arcadia_SizeValue numberOfBytes)
{
  if (!self->resizable) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!bytes || index > self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!numberOfBytes) {
    return;
  }
  Arcadia_SizeValue high, size;
  Arcadia_safeAddFullSizeValue(thread, self->size, numberOfBytes, &high, &size);
  if (high) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }

  // Capture offsets before geometric growth possibly moves the allocation.
  // Reject a pointer inside unused capacity, or a range crossing the allocation.
  uintptr_t source = (uintptr_t)bytes;
  uintptr_t base = (uintptr_t)self->bytes;
  Arcadia_BooleanValue alias = source >= base && source - base < self->capacity;
  Arcadia_SizeValue offset = 0;
  if (alias) {
    offset = (Arcadia_SizeValue)(source - base);
    if (offset >= self->size || numberOfBytes > self->size - offset) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (source < base && numberOfBytes > base - source) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_SizeValue oldSize = self->size;
  Arcadia_ByteArrayNativeImpl_resize(thread, self, size);
  if (index < oldSize) {
    Arcadia_Memory_copy(thread, self->bytes + index + numberOfBytes,
                        self->bytes + index, oldSize - index);
  }
  if (!alias) {
    Arcadia_Memory_copy(thread, self->bytes + index, bytes, numberOfBytes);
  } else if (offset + numberOfBytes <= index) {
    Arcadia_Memory_copy(thread, self->bytes + index, self->bytes + offset, numberOfBytes);
  } else if (offset >= index) {
    Arcadia_Memory_copy(thread, self->bytes + index,
                        self->bytes + offset + numberOfBytes, numberOfBytes);
  } else {
    Arcadia_SizeValue beforeGap = index - offset;
    Arcadia_Memory_copy(thread, self->bytes + index, self->bytes + offset, beforeGap);
    Arcadia_Memory_copy(thread, self->bytes + index + beforeGap,
                        self->bytes + index + numberOfBytes, numberOfBytes - beforeGap);
  }
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteArrayNativeImpl_construct,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ByteArrayNativeImpl_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ByteArrayNativeImpl_initializeDispatch,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteArrayNativeImpl", Arcadia_ByteArrayNativeImpl,
                         u8"Arcadia.ByteArray", Arcadia_ByteArray,
                         &_typeOperations);

static void
Arcadia_ByteArrayNativeImpl_construct(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self)
{
  Arcadia_EnterConstructor(Arcadia_ByteArrayNativeImpl);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->size = 0;
  self->capacity = 0;
  self->bytes = NULL;
  self->resizable = Arcadia_ValueStack_getBooleanValue(thread, 1);
  Arcadia_SizeValue size = Arcadia_ValueStack_getSizeValue(thread, 2);
  // Allocate even for empty buffers, matching the existing ByteArrayBuilder contract.
  self->bytes = Arcadia_Memory_allocateUnmanaged(thread, size);
  self->size = size;
  self->capacity = size;
  Arcadia_Memory_fillZero(thread, self->bytes, size);
  Arcadia_LeaveConstructor(Arcadia_ByteArrayNativeImpl);
}

static void
Arcadia_ByteArrayNativeImpl_destruct(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImpl* self)
{
  if (self->bytes) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->bytes);
    self->bytes = NULL;
  }
}

static void
Arcadia_ByteArrayNativeImpl_initializeDispatch(Arcadia_Thread* thread, Arcadia_ByteArrayNativeImplDispatch* self)
{
  Arcadia_ByteArrayDispatch* d = (Arcadia_ByteArrayDispatch*)self;
  d->getAt = (Arcadia_Natural8Value (*)(Arcadia_Thread*, Arcadia_ByteArray const*, Arcadia_SizeValue))&Arcadia_ByteArrayNativeImpl_getAt;
  d->getBytes = (Arcadia_Natural8Value const* (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_ByteArrayNativeImpl_getBytes;
  d->getMutableBytes = (Arcadia_Natural8Value* (*)(Arcadia_Thread*, Arcadia_ByteArray*))&Arcadia_ByteArrayNativeImpl_getMutableBytes;
  d->getNumberOfBytes = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_ByteArrayNativeImpl_getSize;
  d->getSize = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_ByteArrayNativeImpl_getSize;
  d->isEmpty = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_ByteArrayNativeImpl_isEmpty;
  d->isMutable = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_ByteArrayNativeImpl_isMutable;
  d->isResizable = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_ByteArrayNativeImpl_isResizable;
  d->setAt = (void (*)(Arcadia_Thread*, Arcadia_ByteArray*, Arcadia_SizeValue, Arcadia_Natural8Value))&Arcadia_ByteArrayNativeImpl_setAt;
  d->resize = (void (*)(Arcadia_Thread*, Arcadia_ByteArray*, Arcadia_SizeValue))&Arcadia_ByteArrayNativeImpl_resize;
  d->insertBytes = (void (*)(Arcadia_Thread*, Arcadia_ByteArray*, Arcadia_SizeValue, void const*, Arcadia_SizeValue))&Arcadia_ByteArrayNativeImpl_insertBytes;
}

Arcadia_ByteArrayNativeImpl*
Arcadia_ByteArrayNativeImpl_create(Arcadia_Thread* thread, Arcadia_SizeValue size, Arcadia_BooleanValue resizable)
{
  _Arcadia_BeginCreate(Arcadia_ByteArrayNativeImpl);
  Arcadia_ValueStack_pushSizeValue(thread, size);
  Arcadia_ValueStack_pushBooleanValue(thread, resizable);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_ByteArrayNativeImpl);
}
