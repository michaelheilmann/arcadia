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

#if !defined(ARCADIA_ENGINE_UI_NODES_SCROLLPANELNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_SCROLLPANELNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetNode.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.ScrollPanelNode", Arcadia_Engine_UI_ScrollPanelNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_ScrollPanelNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch parent;
};

struct Arcadia_Engine_UI_ScrollPanelNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_Integer32Value left;
  Arcadia_Integer32Value bottom;
  Arcadia_Natural32Value width;
  Arcadia_Natural32Value height;
  Arcadia_Natural32Value contentWidth;
  Arcadia_Natural32Value contentHeight;
  Arcadia_Integer32Value scrollX;
  Arcadia_Integer32Value scrollY;
  Arcadia_Engine_Visuals_ViewportNode* viewportNode;
  Arcadia_Engine_Visuals_CameraNode* cameraNode;
  Arcadia_ADL_Definitions* generatedDefinitions;
  Arcadia_Engine_Visuals_MeshNode* scrollbarMesh;
  Arcadia_Engine_Visuals_MaterialNode* scrollbarMaterial;
  Arcadia_Engine_Visuals_ModelResource* scrollbarModelResource;
  Arcadia_Real32Value lastCanvasHeight;
  Arcadia_List* children;
  Arcadia_Object* hoveredChild;
  Arcadia_Object* pressedChild;
  Arcadia_Object* focusedChild;
  Arcadia_BooleanValue focused;
  Arcadia_BooleanValue draggingScrollbar;
  Arcadia_Integer32Value scrollbarDragOffset;
};

Arcadia_Engine_UI_ScrollPanelNode*
Arcadia_Engine_UI_ScrollPanelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  );

Arcadia_Engine_UI_ScrollPanelNode*
Arcadia_Engine_UI_ScrollPanelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_ScrollPanelDefinition* source
  );

void
Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Integer32Value* x,
    Arcadia_Integer32Value* y
  );

void
Arcadia_Engine_UI_ScrollPanelNode_setContentSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  );

#endif // ARCADIA_ENGINE_UI_NODES_SCROLLPANELNODE_H_INCLUDED
