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

#if !defined(ARCADIA_ENGINE_UI_EXAMPLES_BUTTON_SCENES_BUTTONSCENE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_EXAMPLES_BUTTON_SCENES_BUTTONSCENE_H_INCLUDED

#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/UI/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.Examples.Button.ButtonScene", Arcadia_Engine_UI_Examples_Button_ButtonScene,
                          u8"Arcadia.Engine.Scene");

struct Arcadia_Engine_UI_Examples_Button_ButtonSceneDispatch {
  Arcadia_Engine_SceneDispatch parent;
};

struct Arcadia_Engine_UI_Examples_Button_ButtonScene {
  Arcadia_Engine_Scene parent;

  Arcadia_ADL_Definitions* definitions;
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  Arcadia_Engine_Visuals_EnterPassNode* enterPassNode;
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;
  Arcadia_Engine_UI_CanvasNode* canvasNode;
  Arcadia_Engine_UI_StackPanelNode* rootPanelNode;
  Arcadia_Engine_UI_StackPanelNode* bodyPanelNode;
  Arcadia_Engine_UI_StackPanelNode* leftColumnPanelNode;
  Arcadia_Engine_UI_StackPanelNode* buttonRowPanelNode;
  Arcadia_Engine_UI_StackPanelNode* toggleRowPanelNode;
  Arcadia_Engine_UI_StackPanelNode* sliderRowPanelNode;
  Arcadia_Engine_UI_StackPanelNode* choiceRowPanelNode;
  Arcadia_Engine_UI_StackPanelNode* scrollColumnPanelNode;
  Arcadia_Engine_UI_LabelNode* helpLabelNode;
  Arcadia_Engine_UI_ButtonNode* buttonNode;
  Arcadia_Engine_UI_ButtonNode* disabledButtonNode;
  Arcadia_Engine_UI_LabelNode* toggleLabelNode;
  Arcadia_Engine_UI_ToggleNode* toggleNode;
  Arcadia_Engine_UI_LabelNode* sliderLabelNode;
  Arcadia_Engine_UI_SliderNode* sliderNode;
  Arcadia_Engine_UI_LabelNode* segmentedChoiceLabelNode;
  Arcadia_Engine_UI_SegmentedChoiceNode* segmentedChoiceNode;
  Arcadia_Engine_UI_LabelNode* scrollPanelLabelNode;
  Arcadia_Engine_UI_ScrollPanelNode* scrollPanelNode;
  Arcadia_Engine_UI_LabelNode* scrollPanelFirstLabelNode;
  Arcadia_Engine_UI_LabelNode* scrollPanelSecondLabelNode;
  Arcadia_Engine_UI_LabelNode* scrollPanelThirdLabelNode;
  Arcadia_Engine_UI_LabelNode* scrollPanelFourthLabelNode;
};

Arcadia_Engine_UI_Examples_Button_ButtonScene*
Arcadia_Engine_UI_Examples_Button_ButtonScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager
  );

#endif // ARCADIA_ENGINE_UI_EXAMPLES_BUTTON_SCENES_BUTTONSCENE_H_INCLUDED
