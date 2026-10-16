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

#include "Arcadia/Engine/Include.h"

#include "Arcadia/DDL/Include.h"
#include "Arcadia/Engine/UI/Examples/Button/Application.h"
#include "Arcadia/Engine/UI/Examples/Button/Scenes/ButtonScene.h"

#include <stdlib.h>

static void
onQuitRequested
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (3 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_Examples_Button_Application* application =
    (Arcadia_Engine_UI_Examples_Button_Application*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Engine_UI_Examples_Button_Application_getType(thread));
  Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_getType(thread));
  Arcadia_Engine_Application_setQuitRequested(thread, (Arcadia_Engine_Application*)application, Arcadia_BooleanValue_True);
}

static void
onSceneChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (4 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_Examples_Button_Application* self = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_Engine_UI_Examples_Button_Application_getType(thread));
  Arcadia_Engine_Scene* newScene = NULL;
  if (Arcadia_ValueStack_isObjectReferenceValue(thread, 1)) {
    newScene = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Scene_getType(thread));
  }
  if (self->sceneOnQuitRequestedSlot) {
    Arcadia_Slot_disconnect(thread, self->sceneOnQuitRequestedSlot);
  }
  if (newScene) {
    self->sceneOnQuitRequestedSlot = Arcadia_Signal_connect(thread, newScene->applicationQuitRequestSignal, (Arcadia_Object*)self, &onQuitRequested);
  }
}

static void
main1
  (
    Arcadia_Process* process,
    int argc,
    char** argv
  )
{
  Arcadia_Engine_UI_Examples_Button_Application* application = NULL;
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    application = Arcadia_Engine_UI_Examples_Button_Application_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)application);
    Arcadia_Engine_Application_startup(thread, (Arcadia_Engine_Application*)application);
    Arcadia_Signal_connect(thread, application->sceneManager->sceneChangedEvent, (Arcadia_Object*)application, &onSceneChanged);
    Arcadia_Engine_SceneManager_setScene(thread, application->sceneManager,
      (Arcadia_Engine_Scene*)Arcadia_Engine_UI_Examples_Button_ButtonScene_create(thread, ((Arcadia_Engine_Application*)application)->engine, application->sceneManager));

    Arcadia_Process_stepARMS(process);

    Arcadia_Natural64Value oldTick = Arcadia_getTickCount(thread);
    Arcadia_Natural64Value newTick = oldTick;
    Arcadia_Natural64Value deltaTick = 0;
    while (!Arcadia_Engine_Application_getQuitRequested(thread, (Arcadia_Engine_Application*)application)) {
      Arcadia_Process_stepARMS(process);
      Arcadia_Engine_BackendContext_update(thread, (Arcadia_Engine_BackendContext*)((Arcadia_Engine_Application*)application)->engine->audialsBackendContext);
      Arcadia_Engine_BackendContext_update(thread, (Arcadia_Engine_BackendContext*)((Arcadia_Engine_Application*)application)->engine->visualsBackendContext);
      // The scene logic updates once per iteration; the loop below only renders, so its
      // per-window calls are audials and visuals only.
      Arcadia_Engine_Scene_updateLogics(thread, Arcadia_Engine_SceneManager_getScene(thread, application->sceneManager), deltaTick);

      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Application*)application)->windows); i < n; ++i) {
        Arcadia_Engine_Visuals_Window* window = (Arcadia_Engine_Visuals_Window*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, ((Arcadia_Engine_Application*)application)->windows, i, _Arcadia_Engine_Visuals_Window_getType(thread));
        Arcadia_Engine_Visuals_Window_beginRender(thread, window);
        Arcadia_Integer32Value width, height;
        Arcadia_Engine_Visuals_Window_getCanvasSize(thread, window, &width, &height);
        Arcadia_Engine_Scene* scene = Arcadia_Engine_SceneManager_getScene(thread, application->sceneManager);
        Arcadia_Engine_Scene_updateAudials(thread, scene, deltaTick, width, height);
        Arcadia_Engine_Scene_updateVisuals(thread, scene, deltaTick, width, height);
        Arcadia_Engine_Visuals_Window_endRender(thread, window);
      }

      Arcadia_Engine_Event* event;
      // Drain the whole queue. Dequeuing a single event per iteration lets the backlog, and
      // with it the input latency, grow whenever more than one event arrives per iteration,
      // which mouse-look regularly does.
      while (NULL != (event = Arcadia_Engine_dequeEvent(thread, ((Arcadia_Engine_Application*)application)->engine))) {
        if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)event, _Arcadia_Engine_Visuals_WindowClosedEvent_getType(thread))) {
          Arcadia_Engine_UI_Examples_Button_Application_onWindowClosedEvent(thread, application, (Arcadia_Engine_Visuals_WindowClosedEvent*)event);
        }
        if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)event, _Arcadia_Engine_Input_MouseButtonEvent_getType(thread))) {
          Arcadia_Engine_Scene_handleMouseButtonEvent(thread, Arcadia_Engine_SceneManager_getScene(thread, application->sceneManager), (Arcadia_Engine_Input_MouseButtonEvent*)event);
        }
        if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)event, _Arcadia_Engine_Input_MousePointerEvent_getType(thread))) {
          Arcadia_Engine_Scene_handleMousePointerEvent(thread, Arcadia_Engine_SceneManager_getScene(thread, application->sceneManager), (Arcadia_Engine_Input_MousePointerEvent*)event);
        }
        if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)event, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread))) {
          Arcadia_Engine_Scene_handleKeyboardKeyEvent(thread, Arcadia_Engine_SceneManager_getScene(thread, application->sceneManager), (Arcadia_Engine_Input_KeyboardKeyEvent*)event);
        }
      }
      oldTick = newTick;
      newTick = Arcadia_getTickCount(thread);
      deltaTick = (newTick > oldTick) ? newTick - oldTick : 0;
    }
    Arcadia_Engine_Application_shutdown(thread, (Arcadia_Engine_Application*)application);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)application);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (application) {
      Arcadia_Engine_Application_shutdown(thread, (Arcadia_Engine_Application*)application);
      Arcadia_Object_unlock(thread, (Arcadia_Object*)application);
    }
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return EXIT_FAILURE;
  }
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(Arcadia_Process_getThread(process), &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    main1(process, argc, argv);
  }
  Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
  Arcadia_Status status = Arcadia_Thread_getStatus(Arcadia_Process_getThread(process));
  Arcadia_Process_relinquish(process);
  return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
