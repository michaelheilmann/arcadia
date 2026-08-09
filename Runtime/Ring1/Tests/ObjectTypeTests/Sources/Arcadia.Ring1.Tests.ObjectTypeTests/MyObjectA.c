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

#include "Arcadia.Ring1.Tests.ObjectTypeTests/MyObjectA.h"

static void
Arcadia_MyObjectA_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  );

static void
Arcadia_MyObjectA_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  );

static void
Arcadia_MyObjectA_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectADispatch* self
  );

static void
Arcadia_MyObjectA_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MyObjectA_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MyObjectA_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectA_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MyObjectA_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MyObjectA", Arcadia_MyObjectA,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_MyObjectA_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MyObjectA);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MyObjectA);
}

static void
Arcadia_MyObjectA_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  )
{ }

static void
Arcadia_MyObjectA_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectADispatch* self
  )
{ }

static void
Arcadia_MyObjectA_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  )
{ }

Arcadia_MyObjectA*
Arcadia_MyObjectA_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MyObjectA);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MyObjectA);
}
