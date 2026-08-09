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
#include "Arcadia/Ring1/Implementation/Objects/ByteArraySliceImpl.h"

#include "Arcadia/Ring1/Include.h"

static void
Arcadia_ByteArraySliceImpl_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl* self
  );

static void
Arcadia_ByteArraySliceImpl_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl* self
  );

static void
Arcadia_ByteArraySliceImpl_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImplDispatch* self
  );

static void
Arcadia_ByteArraySliceImpl_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl* self
  );

static Arcadia_Natural8Value
Arcadia_ByteArraySliceImpl_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self,
    Arcadia_SizeValue index
  );

static Arcadia_Natural8Value const*
Arcadia_ByteArraySliceImpl_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  );


static Arcadia_SizeValue
Arcadia_ByteArraySliceImpl_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  );


static Arcadia_SizeValue
Arcadia_ByteArraySliceImpl_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  );

static Arcadia_BooleanValue
Arcadia_ByteArraySliceImpl_isEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteArraySliceImpl_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_ByteArraySliceImpl_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ByteArraySliceImpl_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ByteArraySliceImpl_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteArraySliceImpl", Arcadia_ByteArraySliceImpl,
                         u8"Arcadia.ByteArray", Arcadia_ByteArray,
                         &_typeOperations);

static void
Arcadia_ByteArraySliceImpl_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ByteArraySliceImpl);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->byteArray = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_ByteArray_getType(thread));
  self->start = Arcadia_ValueStack_getSizeValue(thread, 2);
  self->size = Arcadia_ValueStack_getSizeValue(thread, 1);
  // Example:We have and array [0,1,2,4,5]. Then the slices (4,1) and (5,0) are both valid.
  if (self->start + self->size > Arcadia_ByteArray_getSize(thread, self->byteArray)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_ByteArraySliceImpl);
}

static void
Arcadia_ByteArraySliceImpl_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl* self
  ) 
{/*Intentionally empty.*/}

static void
Arcadia_ByteArraySliceImpl_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImplDispatch* self
  )
{
  ((Arcadia_ByteArrayDispatch*)self)->getAt = (Arcadia_Natural8Value (*)(Arcadia_Thread*, const Arcadia_ByteArray*, Arcadia_SizeValue)) & Arcadia_ByteArraySliceImpl_getAt;
  ((Arcadia_ByteArrayDispatch*)self)->getBytes = (Arcadia_Natural8Value const* (*)(Arcadia_Thread*, const Arcadia_ByteArray*)) & Arcadia_ByteArraySliceImpl_getBytes;
  ((Arcadia_ByteArrayDispatch*)self)->getNumberOfBytes = (Arcadia_SizeValue (*)(Arcadia_Thread*, const Arcadia_ByteArray*)) & Arcadia_ByteArraySliceImpl_getNumberOfBytes;
  ((Arcadia_ByteArrayDispatch*)self)->getSize = (Arcadia_SizeValue (*)(Arcadia_Thread*, const Arcadia_ByteArray*)) & Arcadia_ByteArraySliceImpl_getSize;  
  ((Arcadia_ByteArrayDispatch*)self)->isEmpty = (Arcadia_BooleanValue(*)(Arcadia_Thread*, const Arcadia_ByteArray*)) & Arcadia_ByteArraySliceImpl_isEmpty;
}

static void
Arcadia_ByteArraySliceImpl_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl* self
  )
{
  if (self->byteArray) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->byteArray);
  }
}

Arcadia_ByteArraySliceImpl*
Arcadia_ByteArraySliceImpl_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* byteArray,
    Arcadia_SizeValue start,
    Arcadia_SizeValue size
  )
{
  _Arcadia_BeginCreate(Arcadia_ByteArraySliceImpl);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)byteArray);
  Arcadia_ValueStack_pushSizeValue(thread, start);
  Arcadia_ValueStack_pushSizeValue(thread, size);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_ByteArraySliceImpl);
}

static Arcadia_Natural8Value
Arcadia_ByteArraySliceImpl_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self,
    Arcadia_SizeValue index
  )
{ 
  if (index >= self->size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_ByteArray_getAt(thread, self->byteArray, self->start + index);
}

static Arcadia_Natural8Value const*
Arcadia_ByteArraySliceImpl_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  )
{ return Arcadia_ByteArray_getBytes(thread, self->byteArray) + self->start; }

static Arcadia_SizeValue
Arcadia_ByteArraySliceImpl_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  )
{ return self->size; }

static Arcadia_SizeValue
Arcadia_ByteArraySliceImpl_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  )
{ return self->size; }

static Arcadia_BooleanValue
Arcadia_ByteArraySliceImpl_isEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArraySliceImpl const* self
  )
{ return Arcadia_SizeValue_Literal(0) == self->size; }
