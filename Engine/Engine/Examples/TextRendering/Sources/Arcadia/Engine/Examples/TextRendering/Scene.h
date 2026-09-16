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

#if !defined(ARCADIA_ENGINE_DEMO_SCENE_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCENE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
typedef struct Arcadia_Engine_Demo_SceneManager Arcadia_Engine_Demo_SceneManager;

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Demo_SceneDispatch {
  Arcadia_ObjectDispatch parent;

  void (*updateAudials)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value);
  void (*updateLogics)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value tick);
  void (*updateVisuals)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value);

  void (*handleKeyboardKeyEvent)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*);
  void (*handleMouseButtonEvent)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MouseButtonEvent*);
  void (*handleMousePointerEvent)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MousePointerEvent*);
};

struct Arcadia_Engine_Demo_Scene {
  Arcadia_Object parent;
  Arcadia_Engine* engine;
  Arcadia_Engine_Demo_SceneManager* sceneManager;
  /// The "application quit request" event.
  Arcadia_Signal* applicationQuitRequestSignal;
};

/// @param tick The tick, in milliseconds.
void
Arcadia_Engine_Demo_Scene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

/// @param tick The tick, in milliseconds.
void
Arcadia_Engine_Demo_Scene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick
  );

/// @param tick The tick, in milliseconds.
void
Arcadia_Engine_Demo_Scene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Demo_Scene_handleKeyboardKeyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

void
Arcadia_Engine_Demo_Scene_handleMouseButtonEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

void
Arcadia_Engine_Demo_Scene_handleMousePointerEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

#endif // ARCADIA_ENGINE_DEMO_SCENE_H_INCLUDED