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
#include "Arcadia/Engine/UI/Events/WidgetSizeChangedEvent.h"

#include "Arcadia/Engine/UI/Include.h"

static void
Arcadia_Engine_UI_WidgetSizeChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetSizeChangedEvent* self
  );

static void
Arcadia_Engine_UI_WidgetSizeChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetSizeChangedEventDispatch* self
  );

static void
Arcadia_Engine_UI_WidgetSizeChangedEvent_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetSizeChangedEvent* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_WidgetSizeChangedEvent_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_WidgetSizeChangedEvent_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_WidgetSizeChangedEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.WidgetSizeChangedEvent", Arcadia_Engine_UI_WidgetSizeChangedEvent,
                         u8"Arcadia.Engine.UI.WidgetEvent", Arcadia_Engine_UI_WidgetEvent,
                         &_typeOperations);

static void
Arcadia_Engine_UI_WidgetSizeChangedEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetSizeChangedEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_WidgetSizeChangedEvent);
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
  self->horizontalSize = Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->verticalSize = Arcadia_ValueStack_getInteger32Value(thread, 1);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_WidgetSizeChangedEvent);
}

static void
Arcadia_Engine_UI_WidgetSizeChangedEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetSizeChangedEventDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_UI_WidgetSizeChangedEvent_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetSizeChangedEvent* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_UI_WidgetSizeChangedEvent*
Arcadia_Engine_UI_WidgetSizeChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_UI_WidgetNode* source,
    Arcadia_Integer32Value horizontalSize,
    Arcadia_Integer32Value verticalSize
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_WidgetSizeChangedEvent);
  Arcadia_ValueStack_pushNatural64Value(thread, timestamp);
  if (source) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, horizontalSize);
  Arcadia_ValueStack_pushInteger32Value(thread, verticalSize);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Engine_UI_WidgetSizeChangedEvent);
}
