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

#if !defined(ARCADIA_ENGINE_UI_NODES_STACKPANELNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_STACKPANELNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/UI/WidgetNode.h"
#include "Arcadia/Engine/UI/Nodes/StackPanelNodeAlignment.h"
#include "Arcadia/Engine/UI/Nodes/PanelNode.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.UI.StackPanelNode", Arcadia_Engine_UI_StackPanelNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_StackPanelNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch parent;
};

typedef struct Arcadia_Engine_UI_StackPanelNodeSlot {
  Arcadia_Integer32Value marginLeft;
  Arcadia_Integer32Value marginBottom;
  Arcadia_Integer32Value marginRight;
  Arcadia_Integer32Value marginTop;
  Arcadia_BooleanValue hasAlignmentOverride;
  Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment;
} Arcadia_Engine_UI_StackPanelNodeSlot;

struct Arcadia_Engine_UI_StackPanelNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_Engine_UI_PanelNode* backgroundPanelNode;
  Arcadia_Natural32Value backgroundBorderWidth;
  Arcadia_List* children;
  Arcadia_Engine_UI_StackPanelNodeSlot* slots;
  Arcadia_SizeValue slotCapacity;
  Arcadia_Object* hoveredChild;
  Arcadia_Object* pressedChild;
  Arcadia_Object* focusedChild;
  Arcadia_BooleanValue vertical;
  Arcadia_Integer32Value gap;
  Arcadia_Integer32Value paddingLeft;
  Arcadia_Integer32Value paddingBottom;
  Arcadia_Integer32Value paddingRight;
  Arcadia_Integer32Value paddingTop;
  Arcadia_Engine_UI_StackPanelNodeAlignment mainAlignment;
  Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment;
  Arcadia_Real32Value lastCanvasHeight;
  Arcadia_BooleanValue focused;
};

Arcadia_Engine_UI_StackPanelNode*
Arcadia_Engine_UI_StackPanelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap
  );

Arcadia_Engine_UI_StackPanelNode*
Arcadia_Engine_UI_StackPanelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_StackPanelDefinition* source
  );

void
Arcadia_Engine_UI_StackPanelNode_setPadding
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  );

void
Arcadia_Engine_UI_StackPanelNode_setAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Engine_UI_StackPanelNodeAlignment mainAlignment,
    Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment
  );

void
Arcadia_Engine_UI_StackPanelNode_setChildMargin
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  );

void
Arcadia_Engine_UI_StackPanelNode_setChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment
  );

void
Arcadia_Engine_UI_StackPanelNode_clearChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child
  );

#endif // ARCADIA_ENGINE_UI_NODES_STACKPANELNODE_H_INCLUDED
