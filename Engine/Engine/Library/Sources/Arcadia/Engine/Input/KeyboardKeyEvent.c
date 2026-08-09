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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Input/KeyboardKeyEvent.h"

static void
Arcadia_Engine_Input_KeyboardKeyEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  );

static void
Arcadia_Engine_Input_KeyboardKeyEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEventDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_Engine_Input_KeyboardKeyEvent_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*) & Arcadia_Engine_Input_KeyboardKeyEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Input.KeyboardKeyEvent", Arcadia_Engine_Input_KeyboardKeyEvent,
                         u8"Arcadia.Engine.Event", Arcadia_Engine_Event,
                         &_typeOperations);

static void
Arcadia_Engine_Input_KeyboardKeyEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Input_KeyboardKeyEvent);
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural64Value(thread, Arcadia_ValueStack_getNatural64Value(thread, 3));
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->action = Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->key = Arcadia_ValueStack_getInteger32Value(thread, 1);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Input_KeyboardKeyEvent);
}

static void
Arcadia_Engine_Input_KeyboardKeyEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEventDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Input_KeyboardKeyEvent*
Arcadia_Engine_Input_KeyboardKeyEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_Input_KeyboardKeyAction action,
    Arcadia_Engine_Input_KeyboardKey key
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Input_KeyboardKeyEvent);
  Arcadia_ValueStack_pushNatural64Value(thread, timestamp);
  Arcadia_ValueStack_pushInteger32Value(thread, action);
  Arcadia_ValueStack_pushInteger32Value(thread, key);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Engine_Input_KeyboardKeyEvent);
}

Arcadia_Engine_Input_KeyboardKeyAction
Arcadia_Engine_Input_KeyboardKeyEvent_getAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{ return self->action; }

Arcadia_Engine_Input_KeyboardKey
Arcadia_Engine_Input_KeyboardKeyEvent_getKey
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{ return self->key; }
