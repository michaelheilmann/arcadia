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

#define ARCADIA_ENGINE_AUDIALS_IMPLEMENTATION_PRIVATE (1)
#include "Arcadia/Audials/Implementation/OpenAL/Backend.h"

#include "Arcadia/Audials/Implementation/OpenAL/BackendContext.h"

static Arcadia_String*
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  );

static Arcadia_Engine_Audials_BackendContext*
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_createBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendDispatch* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  );

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Audials_Implementation_OpenAL_Backend_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_Backend_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_Backend_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_Backend_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_Implementation_OpenAL_Backend_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Audials_Implementation_OpenAL_Backend_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Audials_Implementation_OpenAL_Backend_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.OpenAL.Backend", Arcadia_Engine_Audials_Implementation_OpenAL_Backend,
                         u8"Arcadia.Audials.Backend", Arcadia_Engine_Audials_Backend,
                         &_Arcadia_Engine_Audials_Implementation_OpenAL_Backend_typeOperations);

static Arcadia_String*
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  )
{ return Arcadia_String_createFromCxxString(thread, u8"Arcadia Audials OpenAL Backend"); }

static Arcadia_Engine_Audials_BackendContext*
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_createBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  )
{
  return (Arcadia_Engine_Audials_BackendContext*)Arcadia_Engine_Audials_Implementation_OpenAL_BackendContext_getOrCreate(thread);
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_Backend);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_OpenAL_Backend);
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_BackendDispatch* self
  )
{
  ((Arcadia_Engine_BackendDispatch*)self)->createBackendContext = (Arcadia_Engine_BackendContext * (*)(Arcadia_Thread*, Arcadia_Engine_Backend*)) & Arcadia_Engine_Audials_Implementation_OpenAL_Backend_createBackendContextImpl;
  ((Arcadia_Engine_BackendDispatch*)self)->getName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_Engine_Backend*)) & Arcadia_Engine_Audials_Implementation_OpenAL_Backend_getNameImpl;
}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_OpenAL_Backend* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Audials_Implementation_OpenAL_Backend*
Arcadia_Engine_Audials_Implementation_OpenAL_Backend_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_Implementation_OpenAL_Backend);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_Implementation_OpenAL_Backend);
}
