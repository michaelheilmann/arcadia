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
#include "Arcadia/Engine/Visuals/WindowPositionChangedEvent.h"

#include "Arcadia/Engine/Include.h"

static void
Arcadia_Engine_Visuals_WindowPositionChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowPositionChangedEvent* self
  );

static void
Arcadia_Engine_Visuals_WindowPositionChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowPositionChangedEventDispatch* self
  );

static void
Arcadia_Engine_Visuals_WindowPositionChangedEvent_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowPositionChangedEvent* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_WindowPositionChangedEvent_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_WindowPositionChangedEvent_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_WindowPositionChangedEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.WindowPositionChangedEvent", Arcadia_Engine_Visuals_WindowPositionChangedEvent,
                         u8"Arcadia.Engine.Event", Arcadia_Engine_Event,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_WindowPositionChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowPositionChangedEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_WindowPositionChangedEvent);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural64Value(thread, Arcadia_ValueStack_getNatural64Value(thread, 4));
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->window = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Engine_Visuals_Window_getType(thread));
  self->horizontalPosition = Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->verticalPosition = Arcadia_ValueStack_getInteger32Value(thread, 1);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_WindowPositionChangedEvent);
}

static void
Arcadia_Engine_Visuals_WindowPositionChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowPositionChangedEventDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_WindowPositionChangedEvent_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_WindowPositionChangedEvent* self
  )
{
  if (self->window) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->window);
  }
}

Arcadia_Engine_Visuals_WindowPositionChangedEvent*
Arcadia_Engine_Visuals_WindowPositionChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_Visuals_Window* window,
    Arcadia_Integer32Value horizontalPosition,
    Arcadia_Integer32Value verticalPosition
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_WindowPositionChangedEvent);
  Arcadia_ValueStack_pushNatural64Value(thread, timestamp);
  if (window) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, window);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, horizontalPosition);
  Arcadia_ValueStack_pushInteger32Value(thread, verticalPosition);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_WindowPositionChangedEvent);
}
