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

#if !defined(ARCADIA_ENGINE_DEMO_SCENES_TEXTRENDERINGSCENE_H_INCLUDED)
#define ARCADIA_ENGINE_DEMO_SCENES_TEXTRENDERINGSCENE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/Examples/TextRendering/TextRun.h"
#include "Arcadia/FontIO/AtlasBitmapFont.h"
#include "Arcadia/FontIO/AtlasGlyphInformation.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Demo.TextRenderingScene", Arcadia_Engine_Demo_TextRenderingScene,
                          u8"Arcadia.Engine.Demo.Scene");

struct Arcadia_Engine_Demo_TextRenderingSceneDispatch {
  Arcadia_Engine_Demo_SceneDispatch parent;
};

struct Arcadia_Engine_Demo_TextRenderingScene {
  Arcadia_Engine_Demo_Scene parent;

  Arcadia_ADL_Definitions* definitions;

  // The camera node.
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  // The enter pass node.
  Arcadia_Engine_Visuals_EnterPassNode* enterPassNode;
  // The viewport node.
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;

  // The text runs, in drawing order (from top to bottom of the canvas).
  Arcadia_List* runs;

  // The application-owned font cache shared by text runs and scenes.
  Arcadia_Engine_FontCache* fontCache;

  // The canvas size for which the glyph meshes were last positioned.
  Arcadia_Integer32Value textCanvasWidth;
  Arcadia_Integer32Value textCanvasHeight;

  // The frames-per-second counter rendered as a fixed text run.
  Arcadia_Engine_FPSCounter fpsCounter;
};

Arcadia_Engine_Demo_TextRenderingScene*
Arcadia_Engine_Demo_TextRenderingScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager,
    Arcadia_Engine_FontCache* fontCache
  );

#endif // ARCADIA_ENGINE_DEMO_SCENES_TEXTRENDERINGSCENE_H_INCLUDED
