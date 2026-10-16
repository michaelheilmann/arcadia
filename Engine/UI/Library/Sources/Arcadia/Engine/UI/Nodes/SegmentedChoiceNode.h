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

#if !defined(ARCADIA_ENGINE_UI_NODES_SEGMENTEDCHOICENODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_SEGMENTEDCHOICENODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetNode.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.SegmentedChoiceNode", Arcadia_Engine_UI_SegmentedChoiceNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_SegmentedChoiceNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch parent;
};

struct Arcadia_Engine_UI_SegmentedChoiceNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_Engine_Visuals_BackendContext* backendContext;
  Arcadia_Engine_Visuals_MeshNode* mesh;
  Arcadia_Engine_Visuals_MaterialNode* material;
  Arcadia_Engine_Visuals_ModelResource* modelResource;
  Arcadia_ADL_Definitions* generatedDefinitions;
  Arcadia_List* labelNodes;
  Arcadia_Integer32Value left;
  Arcadia_Integer32Value bottom;
  Arcadia_Natural32Value width;
  Arcadia_Natural32Value height;
  Arcadia_Natural32Value numberOfSegments;
  Arcadia_Natural32Value selectedIndex;
  Arcadia_Natural32Value pixelSize;
  Arcadia_Natural32Value borderWidth;
  Arcadia_Natural8Value borderRed;
  Arcadia_Natural8Value borderGreen;
  Arcadia_Natural8Value borderBlue;
  Arcadia_Natural32Value hoveredIndex;
  Arcadia_Real32Value lastCanvasWidth;
  Arcadia_Real32Value lastCanvasHeight;
  Arcadia_BooleanValue enabled;
  Arcadia_BooleanValue pressed;
  Arcadia_BooleanValue focused;
  Arcadia_Signal* changed;
};

Arcadia_Engine_UI_SegmentedChoiceNode*
Arcadia_Engine_UI_SegmentedChoiceNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_Natural32Value numberOfSegments,
    Arcadia_Natural32Value selectedIndex
  );

Arcadia_Engine_UI_SegmentedChoiceNode*
Arcadia_Engine_UI_SegmentedChoiceNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_SegmentedChoiceDefinition* source
  );

void
Arcadia_Engine_UI_SegmentedChoiceNode_setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_BooleanValue hovered
  );

void
Arcadia_Engine_UI_SegmentedChoiceNode_setSelectedIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Natural32Value selectedIndex
  );

Arcadia_Natural32Value
Arcadia_Engine_UI_SegmentedChoiceNode_getSelectedIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  );

Arcadia_Signal*
Arcadia_Engine_UI_SegmentedChoiceNode_getChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  );

void
Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Natural32Value index,
    Arcadia_String* label
  );

#endif // ARCADIA_ENGINE_UI_NODES_SEGMENTEDCHOICENODE_H_INCLUDED
