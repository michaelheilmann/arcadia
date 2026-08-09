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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/Events/WidgetPositionChangedEvent.h"

#include "Arcadia/Engine/UI/Include.h"

static void
Arcadia_Engine_UI_WidgetPositionChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self
  );

static void
Arcadia_Engine_UI_WidgetPositionChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEventDispatch* self
  );

static void
Arcadia_Engine_UI_WidgetPositionChangedEvent_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_WidgetPositionChangedEvent_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_WidgetPositionChangedEvent_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_WidgetPositionChangedEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.WidgetPositionChangedEvent", Arcadia_Engine_UI_WidgetPositionChangedEvent,
                         u8"Arcadia.Engine.UI.WidgetEvent", Arcadia_Engine_UI_WidgetEvent,
                         &_typeOperations);

static void
Arcadia_Engine_UI_WidgetPositionChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_WidgetPositionChangedEvent);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value timestamp = Arcadia_ValueStack_getValue(thread, 4);
    Arcadia_Value source = Arcadia_ValueStack_getValue(thread, 3);
    Arcadia_ValueStack_pushValue(thread, &timestamp);
    Arcadia_ValueStack_pushValue(thread, &source);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->horizontalPosition = Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->verticalPosition = Arcadia_ValueStack_getInteger32Value(thread, 1);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_WidgetPositionChangedEvent);
}

static void
Arcadia_Engine_UI_WidgetPositionChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEventDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_UI_WidgetPositionChangedEvent_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_UI_WidgetPositionChangedEvent*
Arcadia_Engine_UI_WidgetPositionChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_UI_WidgetNode* source,
    Arcadia_Integer32Value horizontalPosition,
    Arcadia_Integer32Value verticalPosition
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_WidgetPositionChangedEvent);
  Arcadia_ValueStack_pushNatural64Value(thread, timestamp);
  if (source) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, horizontalPosition);
  Arcadia_ValueStack_pushInteger32Value(thread, verticalPosition);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Engine_UI_WidgetPositionChangedEvent);
}
