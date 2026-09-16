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
#include "Arcadia/Engine/Examples/TextRendering/Scene.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/UI/Include.h"
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
  // The model node.
  Arcadia_Engine_Visuals_ModelNode* modelNode;
  // The viewport node.
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;

  // The text to render.
  Arcadia_String* text;

  // The dimensions, in pixels, of the font atlas installed as the ambient color texture of the model.
  Arcadia_Integer32Value fontAtlasWidth;
  Arcadia_Integer32Value fontAtlasHeight;

  // The glyph information of the code points of the text, in order.
  // This is unmanaged memory; it is deallocated when the scene is destroyed.
  Arcadia_FontIO_AtlasGlyphInformation* glyphs;
  // The number of glyph information records pointed to by `glyphs`.
  Arcadia_SizeValue numberOfGlyphs;

  // The canvas size for which the glyph mesh was last positioned.
  Arcadia_Integer32Value textCanvasWidth;
  Arcadia_Integer32Value textCanvasHeight;
};

Arcadia_Engine_Demo_TextRenderingScene*
Arcadia_Engine_Demo_TextRenderingScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  );

#endif // ARCADIA_ENGINE_DEMO_SCENES_TEXTRENDERINGSCENE_H_INCLUDED