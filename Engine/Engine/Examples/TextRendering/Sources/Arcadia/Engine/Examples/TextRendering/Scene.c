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

#include "Arcadia/Engine/Examples/TextRendering/Scene.h"

#include "Arcadia/Engine/Examples/TextRendering/SceneManager.h"

static void
Arcadia_Engine_Demo_Scene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self
  );

static void
Arcadia_Engine_Demo_Scene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneDispatch* self
  );

static void
Arcadia_Engine_Demo_Scene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self
  );

static void
Arcadia_Engine_Demo_Scene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_Scene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_Scene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Demo_Scene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_Demo_Scene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_Demo_Scene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_Scene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_Scene_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_Scene_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_Scene_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_Scene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_Scene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Engine_Demo_Scene_typeOperations);

static void
Arcadia_Engine_Demo_Scene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_Scene);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->applicationQuitRequestSignal = Arcadia_Signal_create(thread);
  self->engine = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_getType(thread));
  self->sceneManager = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Demo_SceneManager_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_Scene);
}

static void
Arcadia_Engine_Demo_Scene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneDispatch* self
  )
{
  self->updateAudials = &Arcadia_Engine_Demo_Scene_updateAudialsImpl;
  self->updateLogics = &Arcadia_Engine_Demo_Scene_updateLogicsImpl;
  self->updateVisuals = &Arcadia_Engine_Demo_Scene_updateVisualsImpl;

  self->handleKeyboardKeyEvent = &Arcadia_Engine_Demo_Scene_handleKeyboardKeyEventImpl;
  self->handleMouseButtonEvent = &Arcadia_Engine_Demo_Scene_handleMouseButtonEventImpl;
  self->handleMousePointerEvent = &Arcadia_Engine_Demo_Scene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_Demo_Scene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self
  )
{
  if (self->applicationQuitRequestSignal) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->applicationQuitRequestSignal);
  }
  if (self->engine) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->engine);
  }
  if (self->sceneManager) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sceneManager);
  }
}

static void
Arcadia_Engine_Demo_Scene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_Scene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_Scene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{/*Intentionally empty.*/ }

static void
Arcadia_Engine_Demo_Scene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_Scene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_Scene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{/*Intentionally empty.*/}

void
Arcadia_Engine_Demo_Scene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Demo_Scene, updateAudials, self, tick, width, height); }

void
Arcadia_Engine_Demo_Scene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Demo_Scene, updateLogics, self, tick); }

void
Arcadia_Engine_Demo_Scene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Demo_Scene, updateVisuals, self, tick, width, height); }

void
Arcadia_Engine_Demo_Scene_handleKeyboardKeyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Demo_Scene, handleKeyboardKeyEvent, self, event); }

void
Arcadia_Engine_Demo_Scene_handleMouseButtonEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Demo_Scene, handleMouseButtonEvent, self, event); }

void
Arcadia_Engine_Demo_Scene_handleMousePointerEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Demo_Scene, handleMousePointerEvent, self, event); }