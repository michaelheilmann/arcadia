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

#if !defined(ARCADIA_ENGINE_DEMO_SCENES_MAINSCENE_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCENES_MAINSCENE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Demo/Scene.h"
#include "Arcadia/ADL/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.MainScene", Arcadia_Engine_Demo_MainScene,
                          u8"Arcadia.Engine.Demo.Scene");

struct Arcadia_Engine_Demo_MainSceneDispatch {
  Arcadia_Engine_Demo_SceneDispatch parent;
};

struct Arcadia_Engine_Demo_MainScene {
  Arcadia_Engine_Demo_Scene parent;

  // @todo This is should be inter-scene not intra-scene.
  Arcadia_ADL_Definitions* definitions;

  // Two viewports, three of them.
  Arcadia_Engine_Visuals_ViewportNode* viewportNodes[3];
  // A single camera, re-attached to the respective viewport / model combination for rendering.
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  // A single context, re-update with the information for the respective viewport / model combination for rendering.
  Arcadia_Engine_Visuals_EnterPassNode* enterPassNode;

  // The models, thee of them.
  Arcadia_Engine_Visuals_ModelNode* modelNode[3];

  // The sound source for some background sound effects.
  Arcadia_Engine_Audials_SoundSourceNode* soundSourceNode;
};

Arcadia_Engine_Demo_MainScene*
Arcadia_Engine_Demo_MainScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  );

#endif // ARCADIA_ENGINE_DEMO_SCENES_MAINSCENE_H_INCLUDED
