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

#include "Arcadia/Starship/OrientationChangeInputEvent.h"

static void
Arcadia_Starship_OrientationChangeInputEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_OrientationChangeInputEvent* self
  );

static void
Arcadia_Starship_OrientationChangeInputEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_OrientationChangeInputEventDispatch* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Starship_OrientationChangeInputEvent_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Starship_OrientationChangeInputEvent_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Starship_OrientationChangeInputEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Starship_OrientationChangeInputEvent_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Starship_OrientationChangeInputEvent_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Starship.OrientationChangeInputEvent", Arcadia_Starship_OrientationChangeInputEvent,
                         u8"Arcadia.Engine.Event", Arcadia_Engine_Event,
                         &_Arcadia_Starship_OrientationChangeInputEvent_typeOperations);

static void
Arcadia_Starship_OrientationChangeInputEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_OrientationChangeInputEvent* self
  )
{ 
  Arcadia_EnterConstructor(Arcadia_Starship_OrientationChangeInputEvent);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value timestamp = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &timestamp);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->rotation = (Arcadia_Math_QuaternionReal32*)Arcadia_Object_clone(thread, Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Math_QuaternionReal32_getType(thread)));
  //
  Arcadia_LeaveConstructor(Arcadia_Starship_OrientationChangeInputEvent);
}

static void
Arcadia_Starship_OrientationChangeInputEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_OrientationChangeInputEventDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Starship_OrientationChangeInputEvent*
Arcadia_Starship_OrientationChangeInputEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Math_QuaternionReal32* rotation
  )
{
  _Arcadia_BeginCreate(Arcadia_Starship_OrientationChangeInputEvent);
  Arcadia_ValueStack_pushNatural64Value(thread, timestamp);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, rotation);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Starship_OrientationChangeInputEvent);   
}
