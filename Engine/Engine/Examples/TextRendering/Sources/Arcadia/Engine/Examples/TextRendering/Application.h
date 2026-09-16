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

#if !defined(ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED

#include "Arcadia/DDL/Include.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Examples/TextRendering/Scene.h"
#include "Arcadia/Engine/Examples/TextRendering/SceneManager.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.Application", Arcadia_Engine_Demo_Application,
                          u8"Arcadia.Engine.Application");

struct Arcadia_Engine_Demo_ApplicationDispatch {
  Arcadia_Engine_ApplicationDispatch parent;
};

struct Arcadia_Engine_Demo_Application {
  Arcadia_Engine_Application parent;
  /// @brief A pointer to the signal slot or null.
  Arcadia_Slot* sceneOnQuitRequestedSlot;
  /// @brief A pointer to the scene manager.
  Arcadia_Engine_Demo_SceneManager* sceneManager;
};

Arcadia_Engine_Demo_Application*
Arcadia_Engine_Demo_Application_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_Engine_Demo_Application_onWindowClosedEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_Application* self,
    Arcadia_Engine_Visuals_WindowClosedEvent* event
  );

#endif // ARCADIA_ENGINE_DEMO_APPLICATION_H_INCLUDED