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

#if !defined(ARCADIA_ENGINE_SCENEMANAGER_H_INCLUDED)
#define ARCADIA_ENGINE_SCENEMANAGER_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/SceneManager.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_Engine Arcadia_Engine;
typedef struct Arcadia_Engine_Scene Arcadia_Engine_Scene;
typedef struct Arcadia_Engine_SceneManager Arcadia_Engine_SceneManager;
typedef struct Arcadia_Signal Arcadia_Signal;

Arcadia_declareObjectType(u8"Arcadia.Engine.SceneManager", Arcadia_Engine_SceneManager,
                          u8"Arcadia.Object");

struct Arcadia_Engine_SceneManagerDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_SceneManager {
  Arcadia_Object _parent;
  Arcadia_Engine* engine;
  Arcadia_Engine_Scene* scene;
  Arcadia_Signal* sceneChangedEvent;
};

Arcadia_Engine_Scene*
Arcadia_Engine_SceneManager_getScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self
  );

void
Arcadia_Engine_SceneManager_setScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_SceneManager* self,
    Arcadia_Engine_Scene* scene
  );

Arcadia_Engine_SceneManager*
Arcadia_Engine_SceneManager_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine
  );

#endif // ARCADIA_ENGINE_SCENEMANAGER_H_INCLUDED
