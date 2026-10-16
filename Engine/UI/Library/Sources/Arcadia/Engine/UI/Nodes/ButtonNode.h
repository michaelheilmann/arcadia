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

#if !defined(ARCADIA_ENGINE_UI_NODES_BUTTONNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_BUTTONNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetNode.h"
#include "Arcadia/Engine/UI/TextUtilities.h"
#include "Arcadia/FontIO/AtlasGlyphInformation.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.ButtonNode", Arcadia_Engine_UI_ButtonNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_ButtonNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch parent;
};

struct Arcadia_Engine_UI_ButtonNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_ADL_UI_ButtonDefinition* source;
  Arcadia_ADL_Definitions* generatedDefinitions;
  Arcadia_Engine_Visuals_MeshNode* mesh;
  Arcadia_Engine_Visuals_MaterialNode* material;
  Arcadia_Engine_Visuals_BackendContext* backendContext;
  Arcadia_Engine_Visuals_ModelResource* modelResource;
  Arcadia_Engine_Visuals_ModelNode* labelModelNode;
  Arcadia_FontIO_AtlasGlyphInformation* labelGlyphs;
  Arcadia_SizeValue numberOfLabelGlyphs;
  Arcadia_Engine_UI_TextLine* labelLines;
  Arcadia_SizeValue numberOfLabelLines;
  Arcadia_Integer32Value labelLineTop;
  Arcadia_Integer32Value labelLineBottom;
  Arcadia_Integer32Value labelFontAtlasWidth;
  Arcadia_Integer32Value labelFontAtlasHeight;
  Arcadia_Real32Value lastCanvasWidth;
  Arcadia_Real32Value lastCanvasHeight;
  Arcadia_BooleanValue enabled;
  Arcadia_BooleanValue hovered;
  Arcadia_BooleanValue pressed;
  Arcadia_BooleanValue focused;
  Arcadia_Signal* activated;
};

Arcadia_Engine_UI_ButtonNode*
Arcadia_Engine_UI_ButtonNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_ButtonDefinition* source
  );

void
Arcadia_Engine_UI_ButtonNode_setEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_BooleanValue enabled
  );

void
Arcadia_Engine_UI_ButtonNode_setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_BooleanValue hovered
  );

Arcadia_Signal*
Arcadia_Engine_UI_ButtonNode_getActivatedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  );

#endif // ARCADIA_ENGINE_UI_NODES_BUTTONNODE_H_INCLUDED
