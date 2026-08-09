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
#include "Arcadia/Engine/Visuals/CanvasDPIChangedEvent.h"

static void
Arcadia_Engine_Visuals_CanvasDPIChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasDPIChangedEvent* self
  );

static void
Arcadia_Engine_Visuals_CanvasDPIChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasDPIChangedEventDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_CanvasDPIChangedEvent_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_CanvasDPIChangedEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.CanvasDPIChangedEvent", Arcadia_Engine_Visuals_CanvasDPIChangedEvent,
                         u8"Arcadia.Engine.Visuals.CanvasEvent", Arcadia_Engine_Visuals_CanvasEvent,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_CanvasDPIChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasDPIChangedEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_CanvasDPIChangedEvent);
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural64Value(thread, Arcadia_ValueStack_getNatural64Value(thread, 2));
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->horizontalDpi = Arcadia_ValueStack_getInteger32Value(thread, 1);
  self->verticalDpi = Arcadia_ValueStack_getInteger32Value(thread, 0);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_CanvasDPIChangedEvent);
}

static void
Arcadia_Engine_Visuals_CanvasDPIChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasDPIChangedEventDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Visuals_CanvasDPIChangedEvent*
Arcadia_Engine_Visuals_CanvasDPIChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Integer32Value horizontalDpi,
    Arcadia_Integer32Value verticalDpi
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_CanvasDPIChangedEvent);
  Arcadia_ValueStack_pushNatural64Value(thread, timestamp);
  Arcadia_ValueStack_pushInteger32Value(thread, horizontalDpi);
  Arcadia_ValueStack_pushInteger32Value(thread, verticalDpi);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_CanvasDPIChangedEvent);
}
