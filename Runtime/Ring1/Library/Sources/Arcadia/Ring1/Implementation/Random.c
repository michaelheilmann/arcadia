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
#include "Arcadia/Ring1/Implementation/Random.h"

#include "Arcadia/Ring1/Include.h"

static void
Arcadia_Random_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Random* self
  );

static void
Arcadia_Random_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_RandomDispatch* self
  );

static void
Arcadia_Random_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Random* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Random_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Random_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Random_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Random", Arcadia_Random,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Random_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Random* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Random);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Random);
}

static void
Arcadia_Random_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_RandomDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Random_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Random* self
  )
{/*Intentionally empty.*/}

Arcadia_Random*
Arcadia_Random_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Random);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Random);
}
