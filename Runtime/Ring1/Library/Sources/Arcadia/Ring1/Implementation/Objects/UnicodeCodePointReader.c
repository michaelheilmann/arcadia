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
#include "Arcadia/Ring1/Implementation/Objects/UnicodeCodePointReader.h"

#include "Arcadia/Ring1/Include.h"

static void
Arcadia_UnicodeCodePointReader_constructorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  );

static void
Arcadia_UnicodeCodePointReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReaderDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_UnicodeCodePointReader_constructorImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_UnicodeCodePointReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.UnicodeCodePointReader", Arcadia_UnicodeCodePointReader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_UnicodeCodePointReader_constructorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_UnicodeCodePointReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_UnicodeCodePointReader);
}

static void
Arcadia_UnicodeCodePointReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReaderDispatch* self
  )
{/*Intentionally empty.*/}

void
Arcadia_UnicodeCodePointReader_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  )
{ Arcadia_VirtualCall(Arcadia_UnicodeCodePointReader, nextValue, self); }

Arcadia_Natural32Value
Arcadia_UnicodeCodePointReader_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_UnicodeCodePointReader, getValue, self); }

Arcadia_BooleanValue
Arcadia_UnicodeCodePointReader_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_UnicodeCodePointReader, hasValue, self); }

Arcadia_BooleanValue
Arcadia_UnicodeCodePointReader_hasError
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_UnicodeCodePointReader, hasError, self); }

void
Arcadia_UnicodeCodePointReader_getByteRange
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* self,
    Arcadia_SizeValue* start,
    Arcadia_SizeValue* length
  )
{ Arcadia_VirtualCall(Arcadia_UnicodeCodePointReader, getByteRange, self, start, length); }
