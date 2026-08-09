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
#include "Arcadia/Languages/SyntacticalErrorException.h"

static void
Arcadia_Languages_SyntacticalErrorException_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorException* self
  );

static void
Arcadia_Languages_SyntacticalErrorException_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorExceptionDispatch* self
  );

static void
Arcadia_Languages_SyntacticalErrorException_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorException* self
  );

static void
Arcadia_Languages_SyntacticalErrorException_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorException* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_SyntacticalErrorException_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Languages_SyntacticalErrorException_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_SyntacticalErrorException_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_SyntacticalErrorException_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.SyntacticalErrorException", Arcadia_Languages_SyntacticalErrorException,
                         u8"Arcadia.Exception", Arcadia_Exception,
                         &_typeOperations);

static void
Arcadia_Languages_SyntacticalErrorException_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorException* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_SyntacticalErrorException);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Languages_SyntacticalErrorException);
}

static void
Arcadia_Languages_SyntacticalErrorException_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorExceptionDispatch* self
  )
{ }

static void
Arcadia_Languages_SyntacticalErrorException_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorException* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_SyntacticalErrorException_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_SyntacticalErrorException* self
  )
{/*Intentionally empty.*/}

Arcadia_Languages_SyntacticalErrorException*
Arcadia_Languages_SyntacticalErrorException_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Languages_SyntacticalErrorException);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Languages_SyntacticalErrorException);
}
