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

#if !defined(ARCADIA_GAMES_ANIMA_MENUSCENE_H_INCLUDED)
#define ARCADIA_GAMES_ANIMA_MENUSCENE_H_INCLUDED

#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/UI/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Games.Anima.MenuScene", Arcadia_Games_Anima_MenuScene,
                          u8"Arcadia.Engine.Scene");

struct Arcadia_Games_Anima_MenuSceneDispatch {
  Arcadia_Engine_SceneDispatch parent;
};

typedef enum Arcadia_Games_Anima_MenuMode {
  Arcadia_Games_Anima_MenuMode_Start,
  Arcadia_Games_Anima_MenuMode_Options,
} Arcadia_Games_Anima_MenuMode;

struct Arcadia_Games_Anima_MenuScene {
  Arcadia_Engine_Scene parent;
  Arcadia_Engine_Application* application;
  Arcadia_Games_Anima_MenuMode mode;
  Arcadia_ADL_Definitions* definitions;
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  Arcadia_Engine_Visuals_EnterPassNode* enterPassNode;
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;
  Arcadia_Engine_Audials_StreamingSoundSourceNode* ambienceNode;
  Arcadia_Real32Value ambienceBaseVolume;
  Arcadia_Engine_UI_CanvasNode* canvasNode;
  Arcadia_Engine_UI_StackPanelNode* startRoot;
  Arcadia_Engine_UI_StackPanelNode* optionsRoot;
  Arcadia_Engine_UI_StackPanelNode* optionsContentRoot;
  Arcadia_Engine_UI_StackPanelNode* optionsFooterRoot;
  Arcadia_Engine_UI_StackPanelNode* resolutionRow;
  Arcadia_Engine_UI_StackPanelNode* resolutionSelector;
  Arcadia_Engine_UI_StackPanelNode* vsyncRow;
  Arcadia_Engine_UI_StackPanelNode* brightnessRow;
  Arcadia_Engine_UI_StackPanelNode* masterVolumeRow;
  Arcadia_Engine_UI_StackPanelNode* musicVolumeRow;
  Arcadia_Engine_UI_StackPanelNode* effectsVolumeRow;
  Arcadia_Engine_UI_StackPanelNode* uiVolumeRow;
  Arcadia_Engine_UI_StackPanelNode* voiceVolumeRow;
  Arcadia_Engine_UI_StackPanelNode* mouseSensitivityRow;
  Arcadia_Engine_UI_StackPanelNode* subtitlesRow;
  Arcadia_Engine_UI_StackPanelNode* highContrastRow;
  Arcadia_Engine_UI_ButtonNode* continueButton;
  Arcadia_Engine_UI_ButtonNode* newGameButton;
  Arcadia_Engine_UI_ButtonNode* optionsButton;
  Arcadia_Engine_UI_ButtonNode* exitButton;
  Arcadia_Engine_UI_LabelNode* resolutionLabel;
  Arcadia_Engine_UI_LabelNode* masterVolumeValueLabel;
  Arcadia_Engine_UI_LabelNode* musicVolumeValueLabel;
  Arcadia_Engine_UI_LabelNode* effectsVolumeValueLabel;
  Arcadia_Engine_UI_LabelNode* uiVolumeValueLabel;
  Arcadia_Engine_UI_LabelNode* voiceVolumeValueLabel;
  Arcadia_Engine_UI_SegmentedChoiceNode* optionsTabs;
  Arcadia_Engine_UI_ToggleNode* vsyncToggle;
  Arcadia_Engine_UI_SliderNode* brightnessSlider;
  Arcadia_Engine_UI_SliderNode* masterVolumeSlider;
  Arcadia_Engine_UI_SliderNode* musicVolumeSlider;
  Arcadia_Engine_UI_SliderNode* effectsVolumeSlider;
  Arcadia_Engine_UI_SliderNode* uiVolumeSlider;
  Arcadia_Engine_UI_SliderNode* voiceVolumeSlider;
  Arcadia_Engine_UI_SliderNode* mouseSensitivitySlider;
  Arcadia_Engine_UI_ToggleNode* subtitlesToggle;
  Arcadia_Engine_UI_ToggleNode* highContrastToggle;
  Arcadia_Engine_UI_ButtonNode* applyButton;
  Arcadia_Engine_UI_ButtonNode* resetButton;
  Arcadia_Engine_UI_ButtonNode* backButton;
  Arcadia_Engine_UI_ButtonNode* resolutionPreviousButton;
  Arcadia_Engine_UI_ButtonNode* resolutionNextButton;
  Arcadia_Engine_Visuals_DisplayDevice* displayDevice;
  Arcadia_List* availableResolutionModes;
  Arcadia_List* availableResolutionLabels;
  Arcadia_SizeValue selectedResolutionIndex;
};

Arcadia_Games_Anima_MenuScene*
Arcadia_Games_Anima_MenuScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager,
    Arcadia_Engine_Application* application
  );

#endif // ARCADIA_GAMES_ANIMA_MENUSCENE_H_INCLUDED
