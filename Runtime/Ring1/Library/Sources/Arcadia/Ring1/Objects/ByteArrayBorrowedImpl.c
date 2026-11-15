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
#include "Arcadia/Ring1/Objects/ByteArrayBorrowedImpl.h"
#include "Arcadia/Ring1/Include.h"

static void
construct(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl* self)
{
  Arcadia_EnterConstructor(Arcadia_ByteArrayBorrowedImpl);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->bytes = NULL;
  self->size = 0;
  self->mutable = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(Arcadia_ByteArrayBorrowedImpl);
}

static Arcadia_Natural8Value
getAt(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl const* self, Arcadia_SizeValue index)
{
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->bytes[index];
}

static Arcadia_Natural8Value const*
getBytes(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl const* self)
{ return self->bytes; }

static Arcadia_Natural8Value*
getMutableBytes(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl* self)
{
  if (!self->mutable) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural8Value*)self->bytes;
}

static Arcadia_SizeValue
getSize(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl const* self)
{ return self->size; }

static Arcadia_BooleanValue
isEmpty(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl const* self)
{ return self->size == 0; }

static Arcadia_BooleanValue
isMutable(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl const* self)
{ return self->mutable; }

static void
setAt(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImpl* self,
      Arcadia_SizeValue index, Arcadia_Natural8Value value)
{
  if (!self->mutable) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  ((Arcadia_Natural8Value*)self->bytes)[index] = value;
}

static void
initializeDispatch(Arcadia_Thread* thread, Arcadia_ByteArrayBorrowedImplDispatch* self)
{
  Arcadia_ByteArrayDispatch* d = (Arcadia_ByteArrayDispatch*)self;
  d->getAt = (Arcadia_Natural8Value (*)(Arcadia_Thread*, Arcadia_ByteArray const*, Arcadia_SizeValue))&getAt;
  d->getBytes = (Arcadia_Natural8Value const* (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&getBytes;
  d->getMutableBytes = (Arcadia_Natural8Value* (*)(Arcadia_Thread*, Arcadia_ByteArray*))&getMutableBytes;
  d->getNumberOfBytes = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&getSize;
  d->getSize = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&getSize;
  d->isEmpty = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&isEmpty;
  d->isMutable = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&isMutable;
  d->setAt = (void (*)(Arcadia_Thread*, Arcadia_ByteArray*, Arcadia_SizeValue, Arcadia_Natural8Value))&setAt;
  // Base dispatch supplies isContiguous=true, isResizable=false and invalid resize.
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&construct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatch,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteArrayBorrowedImpl", Arcadia_ByteArrayBorrowedImpl,
                         u8"Arcadia.ByteArray", Arcadia_ByteArray,
                         &_typeOperations);

Arcadia_ByteArrayBorrowedImpl*
Arcadia_ByteArrayBorrowedImpl_create(Arcadia_Thread* thread,
                                     Arcadia_Natural8Value const* bytes,
                                     Arcadia_SizeValue size,
                                     Arcadia_BooleanValue mutable)
{
  if (size && !bytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  _Arcadia_BeginCreate(Arcadia_ByteArrayBorrowedImpl);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_ByteArrayBorrowedImpl* result =
    _Arcadia_EndCreate0(thread, _Arcadia_ByteArrayBorrowedImpl_getType(thread), oldValueStackSize);
  result->bytes = bytes;
  result->size = size;
  result->mutable = mutable;
  return result;
}
