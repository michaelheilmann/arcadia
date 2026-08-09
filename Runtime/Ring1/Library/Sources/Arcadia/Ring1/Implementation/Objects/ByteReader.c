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
#include "Arcadia/Ring1/Implementation/Objects/ByteReader.h"

#include "Arcadia/Ring1/Include.h"

static void
Arcadia_ByteReader_construct
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

static void
Arcadia_ByteReader_initializeDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

static void
Arcadia_ByteReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteReader_construct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ByteReader_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ByteReader_initializeDispatch,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteReader", Arcadia_ByteReader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ByteReader_construct
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ByteReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_ByteReader);
}

static void
Arcadia_ByteReader_initializeDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ByteReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{/*Intentionally empty.*/}

Arcadia_BooleanValue
Arcadia_ByteReader_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_ByteReader, hasValue, self); }

Arcadia_BooleanValue
Arcadia_ByteReader_hasError
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_ByteReader, hasError, self); }

Arcadia_Natural8Value
Arcadia_ByteReader_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_ByteReader, getValue, self); }

void
Arcadia_ByteReader_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* self
  )
{ Arcadia_VirtualCall(Arcadia_ByteReader, nextValue, self); }
