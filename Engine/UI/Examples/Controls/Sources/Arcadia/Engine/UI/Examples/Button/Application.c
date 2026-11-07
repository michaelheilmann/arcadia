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

#include "Arcadia/Engine/UI/Examples/Button/Application.h"

#include "Arcadia/Audials/Implementation/Include.h"
#include "Arcadia/Visuals/Implementation/Include.h"

static void
Arcadia_Engine_UI_Examples_Button_Application_startupImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  );

static void
Arcadia_Engine_UI_Examples_Button_Application_shutdownImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  );

static void
Arcadia_Engine_UI_Examples_Button_Application_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  );

static void
Arcadia_Engine_UI_Examples_Button_Application_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ApplicationDispatch* self
  );

static void
Arcadia_Engine_UI_Examples_Button_Application_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_Examples_Button_Application_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_Examples_Button_Application_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_Examples_Button_Application_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.Examples.Button.Application", Arcadia_Engine_UI_Examples_Button_Application,
                         u8"Arcadia.Engine.Application", Arcadia_Engine_Application,
                         &_typeOperations);

static void
Arcadia_Engine_UI_Examples_Button_Application_startupImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  )
{
  ((Arcadia_Engine_Application*)self)->configuration = (Arcadia_DDL_Node*)Arcadia_Engine_ApplicationConfiguration_loadConfiguration(thread, ((Arcadia_Engine_Application*)self)->configurationDirectoryName);
  Arcadia_Engine_Audials_Implementation_registerBackends(thread, ((Arcadia_Engine_Application*)self)->engine->audialsBackendTypes);
  Arcadia_Engine_ApplicationHelper_startupAudials(thread, ((Arcadia_Engine_Application*)self)->engine, ((Arcadia_Engine_Application*)self)->configuration);
  Arcadia_Engine_Visuals_Implementation_registerBackends(thread, ((Arcadia_Engine_Application*)self)->engine->visualsBackendTypes);
  Arcadia_Engine_ApplicationHelper_startupVisuals(thread, ((Arcadia_Engine_Application*)self)->engine, ((Arcadia_Engine_Application*)self)->configuration, ((Arcadia_Engine_Application*)self)->windows);
}

static void
Arcadia_Engine_UI_Examples_Button_Application_shutdownImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Engine_ApplicationConfiguration_saveConfiguration(thread, ((Arcadia_Engine_Application*)self)->configurationDirectoryName,
                                                                      ((Arcadia_Engine_Application*)self)->configuration);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
  }

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Application*)self)->windows); i < n; ++i) {
    Arcadia_Engine_Visuals_Window* window = (Arcadia_Engine_Visuals_Window*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, ((Arcadia_Engine_Application*)self)->windows, i, _Arcadia_Engine_Visuals_Window_getType(thread));
    Arcadia_Engine_Visuals_Window_close(thread, window);
  }

  while (Arcadia_Engine_dequeEvent(thread, ((Arcadia_Engine_Application*)self)->engine)) {
  }

  if (((Arcadia_Engine_Application*)self)->engine && ((Arcadia_Engine_Application*)self)->engine->audialsBackendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)((Arcadia_Engine_Application*)self)->engine->audialsBackendContext);
    ((Arcadia_Engine_Application*)self)->engine->audialsBackendContext = NULL;
  }
  if (((Arcadia_Engine_Application*)self)->engine && ((Arcadia_Engine_Application*)self)->engine->visualsBackendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)((Arcadia_Engine_Application*)self)->engine->visualsBackendContext);
    ((Arcadia_Engine_Application*)self)->engine->visualsBackendContext = NULL;
  }
}

static void
Arcadia_Engine_UI_Examples_Button_Application_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_Examples_Button_Application);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Application_setConfigurationDirectoryName(thread, (Arcadia_Engine_Application*)self,
    Arcadia_String_createFromCxxString(thread, u8"ButtonExample"));
  ((Arcadia_Engine_Application*)self)->configuration = NULL;
  self->sceneManager = Arcadia_Engine_SceneManager_create(thread, ((Arcadia_Engine_Application*)self)->engine);
  self->sceneOnQuitRequestedSlot = NULL;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_Examples_Button_Application);
}

static void
Arcadia_Engine_UI_Examples_Button_Application_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ApplicationDispatch* self
  )
{
  ((Arcadia_Engine_ApplicationDispatch*)self)->startup = (void (*)(Arcadia_Thread*, Arcadia_Engine_Application*))&Arcadia_Engine_UI_Examples_Button_Application_startupImpl;
  ((Arcadia_Engine_ApplicationDispatch*)self)->shutdown = (void (*)(Arcadia_Thread*, Arcadia_Engine_Application*))&Arcadia_Engine_UI_Examples_Button_Application_shutdownImpl;
}

static void
Arcadia_Engine_UI_Examples_Button_Application_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self
  )
{
  if (self->sceneManager) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sceneManager);
  }
}

Arcadia_Engine_UI_Examples_Button_Application*
Arcadia_Engine_UI_Examples_Button_Application_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_Examples_Button_Application);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_Examples_Button_Application);
}

static Arcadia_BooleanValue
filterWindows
  (
    Arcadia_Thread* thread,
    Arcadia_Value context,
    Arcadia_Value value
  )
{
  return Arcadia_Value_isEqualTo(thread, &context, &value) ? Arcadia_BooleanValue_False : Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_Examples_Button_Application_onWindowClosedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  )
{
  Arcadia_List_filter(thread, ((Arcadia_Engine_Application*)self)->windows, Arcadia_Value_makeObjectReferenceValue(event->window), &filterWindows);
  Arcadia_Engine_Application_setQuitRequested(thread, (Arcadia_Engine_Application*)self, Arcadia_BooleanValue_True);
}
