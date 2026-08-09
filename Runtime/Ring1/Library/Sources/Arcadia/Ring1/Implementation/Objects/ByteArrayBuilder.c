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
#include "Arcadia/Ring1/Implementation/Objects/ByteArrayBuilder.h"

#include "Arcadia/Ring1/Include.h"

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

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteArrayBuilder_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ByteArrayBuilder_destruct,
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
  self->p = Arcadia_Memory_allocateUnmanaged(thread, 0);
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
{
  if (self->p) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->p);
    self->p = NULL;
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
  if (self->sz < numberOfBytes) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue d = self->sz - numberOfBytes;
  return !Arcadia_Memory_compare(thread, self->p + d, bytes, numberOfBytes);
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
  if (self->sz < numberOfBytes) {
    return Arcadia_BooleanValue_False;
  }
  return !Arcadia_Memory_compare(thread, self->p, bytes, numberOfBytes);
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
  // TODO: Use Arcadia_Arrays_* maybe?
  Arcadia_SizeValue freeCapacity = self->cp - self->sz;
  if (freeCapacity < numberOfBytes) {
    Arcadia_SizeValue additionalCapacity = numberOfBytes - freeCapacity;
    Arcadia_SizeValue oldCapacity = self->cp;
    if (SIZE_MAX - oldCapacity < additionalCapacity) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_SizeValue newCapacity = oldCapacity + additionalCapacity;
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->p, newCapacity);
    self->cp = newCapacity;
  }
  if (index < self->sz) {
    Arcadia_Memory_copy(thread, self->p + index, self->p + index + numberOfBytes, self->sz - index);
  }
  Arcadia_Memory_copy(thread, self->p + index, bytes, numberOfBytes);
  self->sz += numberOfBytes;
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
    return !Arcadia_Memory_compare(thread, self->p, other->p, self->sz);
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
    return !Arcadia_Memory_compare(thread, self->p, bytes, numberOfBytes);
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
  self->sz = 0;
}

Arcadia_SizeValue
Arcadia_ByteArrayBuilder_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self
  )
{
  return self->sz;
}

Arcadia_SizeValue
Arcadia_ByteArrayBuilder_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self
  )
{ return self->sz; }

Arcadia_Natural8Value const*
Arcadia_ByteArrayBuilder_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self
  )
{ return self->p; }

Arcadia_Natural8Value
Arcadia_ByteArrayBuilder_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder const* self,
    Arcadia_SizeValue index
  )
{
  if (index >= self->sz) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return *(self->p + index);
}

Arcadia_ByteArray*
Arcadia_ByteArrayBuilder_toByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* self
  )
{ return Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, self), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, self))); }
