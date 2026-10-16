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

#if !defined(ARCADIA_ENGINE_UI_NODES_LABELNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_LABELNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetNode.h"
#include "Arcadia/Engine/UI/TextUtilities.h"
#include "Arcadia/FontIO/AtlasGlyphInformation.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.LabelNode", Arcadia_Engine_UI_LabelNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_LabelNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch parent;
};

struct Arcadia_Engine_UI_LabelNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_String* text;
  Arcadia_String* horizontalAlignment;
  Arcadia_String* verticalAlignment;
  Arcadia_Integer32Value left;
  Arcadia_Integer32Value bottom;
  Arcadia_Natural32Value width;
  Arcadia_Natural32Value height;
  Arcadia_Natural8Value red;
  Arcadia_Natural8Value green;
  Arcadia_Natural8Value blue;
  Arcadia_Natural8Value alpha;
  Arcadia_Natural32Value pixelSize;
  Arcadia_ADL_Definitions* generatedDefinitions;
  Arcadia_Engine_Visuals_ModelNode* modelNode;
  Arcadia_FontIO_AtlasGlyphInformation* glyphs;
  Arcadia_SizeValue numberOfGlyphs;
  Arcadia_Engine_UI_TextLine* lines;
  Arcadia_SizeValue numberOfLines;
  Arcadia_Integer32Value fontAtlasWidth;
  Arcadia_Integer32Value fontAtlasHeight;
  Arcadia_Integer32Value lineTop;
  Arcadia_Integer32Value lineBottom;
  Arcadia_Engine_Visuals_BackendContext* backendContext;
};

Arcadia_Engine_UI_LabelNode*
Arcadia_Engine_UI_LabelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_String* text,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_String* horizontalAlignment,
    Arcadia_String* verticalAlignment,
    Arcadia_Natural8Value red,
    Arcadia_Natural8Value green,
    Arcadia_Natural8Value blue,
    Arcadia_Natural8Value alpha
  );

Arcadia_Engine_UI_LabelNode*
Arcadia_Engine_UI_LabelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_LabelDefinition* source
  );

void
Arcadia_Engine_UI_LabelNode_setText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_String* text
  );

void
Arcadia_Engine_UI_LabelNode_setPixelSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Natural32Value pixelSize
  );

#endif // ARCADIA_ENGINE_UI_NODES_LABELNODE_H_INCLUDED
