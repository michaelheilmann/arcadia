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
#include "Arcadia/Engine/UI/WidgetEvent.h"

#include "Arcadia/Engine/UI/Include.h"

static void
Arcadia_Engine_UI_WidgetEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetEvent* self
  );

static void
Arcadia_Engine_UI_WidgetEvent_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetEvent* self
  );

static void
Arcadia_Engine_UI_WidgetEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetEventDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_WidgetEvent_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_WidgetEvent_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_WidgetEvent_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.WidgetEvent", Arcadia_Engine_UI_WidgetEvent,
                         u8"Arcadia.Engine.UI.Event", Arcadia_Engine_UI_Event,
                         &_typeOperations);

static void
Arcadia_Engine_UI_WidgetEvent_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_WidgetEvent);
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
  self->source = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_WidgetNode_getType(thread));
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_WidgetEvent);
}

static void
Arcadia_Engine_UI_WidgetEvent_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetEvent* self
  )
{
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
}

static void
Arcadia_Engine_UI_WidgetEvent_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetEventDispatch* self
  )
{/*Intentionally empty.*/}
