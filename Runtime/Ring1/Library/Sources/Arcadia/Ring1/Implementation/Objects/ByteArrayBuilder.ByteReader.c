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
#include "Arcadia/Ring1/Implementation/Objects/ByteArrayBuilder.ByteReader.h"

#include "Arcadia/Ring1/Include.h"

static void
Arcadia_ByteArrayBuilder_ByteReader_construct
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static void
Arcadia_ByteArrayBuilder_ByteReader_initializeDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static void
Arcadia_ByteArrayBuilder_ByteReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_ByteReader_hasValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_ByteReader_hasErrorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static Arcadia_Natural8Value
Arcadia_ByteArrayBuilder_ByteReader_getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static void
Arcadia_ByteArrayBuilder_ByteReader_nextValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteArrayBuilder_ByteReader_construct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ByteArrayBuilder_ByteReader_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ByteArrayBuilder_ByteReader_initializeDispatch,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteArrayBuilder.ByteReader", Arcadia_ByteArrayBuilder_ByteReader,
                         u8"Arcadia.ByteReader", Arcadia_ByteReader,
                         &_typeOperations);

static void
Arcadia_ByteArrayBuilder_ByteReader_construct
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ByteArrayBuilder_ByteReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->source = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ByteArrayBuilder_getType(thread));
  self->index = 0;
  Arcadia_LeaveConstructor(Arcadia_ByteArrayBuilder_ByteReader);
}

static void
Arcadia_ByteArrayBuilder_ByteReader_initializeDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{
  ((Arcadia_ByteReaderDispatch*)self)->getValue = (Arcadia_Natural8Value (*)(Arcadia_Thread*, Arcadia_ByteReader*)) & Arcadia_ByteArrayBuilder_ByteReader_getValueImpl;
  ((Arcadia_ByteReaderDispatch*)self)->hasError = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_ByteReader*)) & Arcadia_ByteArrayBuilder_ByteReader_hasErrorImpl;
  ((Arcadia_ByteReaderDispatch*)self)->hasValue = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_ByteReader*)) & Arcadia_ByteArrayBuilder_ByteReader_hasValueImpl;
  ((Arcadia_ByteReaderDispatch*)self)->nextValue = (void(*)(Arcadia_Thread*, Arcadia_ByteReader*)) & Arcadia_ByteArrayBuilder_ByteReader_nextValueImpl;

}

static void
Arcadia_ByteArrayBuilder_ByteReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{/*Intentionally empty.*/}

static Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_ByteReader_hasValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{ 
  return self->index < self->source->sz;
}

static Arcadia_BooleanValue
Arcadia_ByteArrayBuilder_ByteReader_hasErrorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{ return Arcadia_BooleanValue_False; }

static Arcadia_Natural8Value
Arcadia_ByteArrayBuilder_ByteReader_getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{
  if (!Arcadia_ByteReader_hasValue(thread, (Arcadia_ByteReader*)self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->source->p[self->index];
}

static void
Arcadia_ByteArrayBuilder_ByteReader_nextValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder_ByteReader* self
  )
{
  if (!Arcadia_ByteReader_hasValue(thread, (Arcadia_ByteReader*)self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->index++;
}

Arcadia_ByteArrayBuilder_ByteReader*
Arcadia_ByteArrayBuilder_ByteReader_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* source
  )
{
  _Arcadia_BeginCreate(Arcadia_ByteArrayBuilder_ByteReader);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_ByteArrayBuilder_ByteReader); 
}
