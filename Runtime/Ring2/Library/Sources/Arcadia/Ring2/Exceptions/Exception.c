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

#define ARCADIA_RING2_MODULE (1)
#include "Arcadia/Ring2/Exceptions/Exception.h"

static void
_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Exception* self
  );

static void
Arcadia_Exception_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ExceptionDispatch* self
  );

static void
_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Exception* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Exception_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Exception", Arcadia_Exception,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Exception* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Exception);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_Exception);
}

static void
Arcadia_Exception_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ExceptionDispatch* self
  )
{/*Intentionally empty.*/}

static void
_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Exception* self
  )
{/*Intentionally empty.*/}
