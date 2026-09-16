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

#if !defined(ARCADIA_ENGINE_DEMO_SCENES_MAINMENUSCENE_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCENES_MAINMENUSCENE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Examples/CheckerboardTexture/Scene.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/UI/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.MainMenuScene", Arcadia_Engine_Demo_MainMenuScene,
                          u8"Arcadia.Engine.Demo.Scene");

struct Arcadia_Engine_Demo_MainMenuSceneDispatch {
  Arcadia_Engine_Demo_SceneDispatch parent;
};

struct Arcadia_Engine_Demo_MainMenuScene {
  Arcadia_Engine_Demo_Scene parent;

  // @todo This is should be inter-scene not intra-scene.
  Arcadia_ADL_Definitions* definitions;

  // The viewport.
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;
  // A single camera, re-attached to the respective viewport / model combination for rendering.
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  // A single context, re-update with the information for the respective viewport / model combination for rendering.
  Arcadia_Engine_Visuals_EnterPassNode* enterPassNode;

#if 0
  // The models, thee of them.
  Arcadia_Engine_Visuals_ModelNode* modelNode;
#endif
#if 1
  // The 'W' (index 0), 'A' (index 1), 'S' (index 2), and 'D' (index 3) latches.
  // The 'Q' (index 4) and 'E' (index 5) latches.
  Arcadia_BooleanValue latches[6];
#endif


  // The user interface.
  Arcadia_Engine_UI_CanvasNode* uiCanvasNode;
};

Arcadia_Engine_Demo_MainMenuScene*
Arcadia_Engine_Demo_MainMenuScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  );

#endif // ARCADIA_ENGINE_DEMO_SCENES_MAINMENUSCENE_H_INCLUDED
