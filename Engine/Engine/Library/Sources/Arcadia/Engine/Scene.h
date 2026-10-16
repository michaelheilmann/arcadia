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

#if !defined(ARCADIA_ENGINE_SCENE_H_INCLUDED)
#define ARCADIA_ENGINE_SCENE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Scene.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine Arcadia_Engine;
typedef struct Arcadia_Engine_Input_KeyboardKeyEvent Arcadia_Engine_Input_KeyboardKeyEvent;
typedef struct Arcadia_Engine_Input_MouseButtonEvent Arcadia_Engine_Input_MouseButtonEvent;
typedef struct Arcadia_Engine_Input_MousePointerEvent Arcadia_Engine_Input_MousePointerEvent;
typedef struct Arcadia_Engine_Scene Arcadia_Engine_Scene;
typedef struct Arcadia_Engine_SceneManager Arcadia_Engine_SceneManager;
typedef struct Arcadia_Signal Arcadia_Signal;

Arcadia_declareObjectType(u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                          u8"Arcadia.Object");

struct Arcadia_Engine_SceneDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*updateAudials)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value);

  void (*updateLogics)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value);

  void (*updateVisuals)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value);

  void (*handleKeyboardKeyEvent)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*);

  void (*handleMouseButtonEvent)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MouseButtonEvent*);

  void (*handleMousePointerEvent)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MousePointerEvent*);
};

struct Arcadia_Engine_Scene {
  Arcadia_Object _parent;
  Arcadia_Engine* engine;
  Arcadia_Engine_SceneManager* sceneManager;
  Arcadia_Signal* applicationQuitRequestSignal;
};

void
Arcadia_Engine_Scene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Scene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick
  );

void
Arcadia_Engine_Scene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Scene_handleKeyboardKeyEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

void
Arcadia_Engine_Scene_handleMouseButtonEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

void
Arcadia_Engine_Scene_handleMousePointerEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Scene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

#endif // ARCADIA_ENGINE_SCENE_H_INCLUDED
