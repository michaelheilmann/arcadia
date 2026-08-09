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

#include "Arcadia.Ring1.Tests.ObjectTypeTests/MyObjectB.h"

static void
Arcadia_MyObjectB_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectB* self
  );

static void
Arcadia_MyObjectB_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectB* self
  );

static void
Arcadia_MyObjectB_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectBDispatch* self
  );

static void
Arcadia_MyObjectB_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectB* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MyObjectB_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MyObjectB_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectB_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MyObjectB_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MyObjectB", Arcadia_MyObjectB,
                         u8"Arcadia.MyObjectA", Arcadia_MyObjectA,
                         &_typeOperations);

static void
Arcadia_MyObjectB_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectB* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MyObjectB);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MyObjectB);
}

static void
Arcadia_MyObjectB_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectB* self
  )
{ }

static void
Arcadia_MyObjectB_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectBDispatch* self
  )
{ }

static void
Arcadia_MyObjectB_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectB* self
  )
{ }

Arcadia_MyObjectB*
Arcadia_MyObjectB_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MyObjectB);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MyObjectB);
}
