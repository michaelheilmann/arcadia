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

#if !defined(ARCADIA_ENGINE_DEMO_SCENEMANGER_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCENEMANGER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Engine/Include.h"
typedef struct Arcadia_Engine_Demo_Scene Arcadia_Engine_Demo_Scene;

/// The scene manager is referenced by any scene.
/// A scene is "active" if sceneManager.scene = scene.
/// A scene can - at any point in time - activate another scene by setting sceneManager.scene to the desired scene.
Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.SceneManager", Arcadia_Engine_Demo_SceneManager,
                          u8"Arcadia.Object")

struct Arcadia_Engine_Demo_SceneManagerDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Engine_Demo_SceneManager {
  Arcadia_Object parent;
  // The engine.
  Arcadia_Engine* engine;
  // The current scene.
  Arcadia_Engine_Demo_Scene* scene;
  // When a scene truly changed.
  Arcadia_Signal* sceneChangedEvent;
};

/// @return The active scene or null.
/// @default The default is null.
Arcadia_Engine_Demo_Scene*
Arcadia_Engine_Demo_SceneManager_getScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self
  );

/// @param scene The new active scene if any or null.
void
Arcadia_Engine_Demo_SceneManager_setScene
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_SceneManager* self,
    Arcadia_Engine_Demo_Scene* scene
  );

Arcadia_Engine_Demo_SceneManager*
Arcadia_Engine_Demo_SceneManager_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine
  );

#endif // ARCADIA_ENGINE_DEMO_SCENEMANGER_H_INCLUDED