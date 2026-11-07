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

#if !defined(ARCADIA_ENGINE_UI_NODES_LAYOUTPANELNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_LAYOUTPANELNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Engine/UI/WidgetNode.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/UI/Layout.h"
#include "Arcadia/Engine/UI/Nodes/PanelNode.h"
#include "Arcadia/Engine/UI/Nodes/StackPanelNodeAlignment.h"

typedef struct Arcadia_Engine_UI_StackLayoutSlot Arcadia_Engine_UI_StackLayoutSlot;

// An unstyled container. It owns children; its layout strategy positions them.
// This is distinct from the existing model-backed PanelNode.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.LayoutPanelNode", Arcadia_Engine_UI_LayoutPanelNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_LayoutPanelNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch _parent;
};

struct Arcadia_Engine_UI_LayoutPanelNode {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_List* children;
  // Parallel to children; each entry is a Slot or Void for non-widget children.
  Arcadia_List* childConnections;
  // Numeric child-slot settings are owned by the container, not the layout.
  // Their indices follow children and childConnections through removal/reordering.
  Arcadia_Engine_UI_StackLayoutSlot* childLayoutSlots;
  Arcadia_SizeValue childLayoutSlotCapacity;
  Arcadia_Object* layout;
  Arcadia_Engine_UI_PanelNode* backgroundPanelNode;
  Arcadia_Natural32Value backgroundBorderWidth;
  Arcadia_Object* hoveredChild;
  Arcadia_Object* pressedChild;
  Arcadia_Object* focusedChild;
  Arcadia_Real32Value lastCanvasHeight;
  Arcadia_BooleanValue focused;
  Arcadia_BooleanValue layoutDirty;
  Arcadia_BooleanValue layoutInProgress;
  Arcadia_BooleanValue hasArranged;
  Arcadia_Integer32Value arrangedLeft, arrangedBottom, arrangedWidth, arrangedHeight;
};

Arcadia_Engine_UI_LayoutPanelNode*
Arcadia_Engine_UI_LayoutPanelNode_create
  (
    Arcadia_Thread* thread
  );

/// @brief Attach a layout strategy; NULL restores fixed-size, no-layout behavior.
/// @remarks The panel retains the strategy through its managed object graph.
void
Arcadia_Engine_UI_LayoutPanelNode_setLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* layout
  );

Arcadia_Engine_UI_LayoutPanelNode*
Arcadia_Engine_UI_LayoutPanelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_PanelDefinition* source
  );

/// @brief Switch between content-derived and explicit requested sizing.
/// @remarks LayoutPanelNode defaults to Content. Fixed retains its requested
///          width and height even when tab/page changes alter its children.
void
Arcadia_Engine_UI_LayoutPanelNode_setSizePolicy
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Engine_UI_WidgetNodeSizePolicy policy
  );

/// @brief Set per-child margins without changing a widget's requested size.
void
Arcadia_Engine_UI_LayoutPanelNode_setChildMargin
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  );

/// @brief Override the panel's cross-axis alignment for one child.
void
Arcadia_Engine_UI_LayoutPanelNode_setChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Engine_UI_StackPanelNodeAlignment alignment
  );

void
Arcadia_Engine_UI_LayoutPanelNode_clearChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self,
    Arcadia_Object* child
  );

/// @brief Borrow numeric child slots (one per child); valid until the panel mutates.
Arcadia_Engine_UI_StackLayoutSlot const*
Arcadia_Engine_UI_LayoutPanelNode_getChildLayoutSlots
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  );

/// @brief Mark the layout stale after changing a strategy's configuration.
void
Arcadia_Engine_UI_LayoutPanelNode_invalidateLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  );

/// @brief Recalculate geometry only when the panel or a descendant is dirty.
void
Arcadia_Engine_UI_LayoutPanelNode_updateLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  );

/// @brief Clear child hover state when the pointer leaves this panel.
void
Arcadia_Engine_UI_LayoutPanelNode_clearHover
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  );

Arcadia_Object*
Arcadia_Engine_UI_LayoutPanelNode_getLayout
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* self
  );

#endif // ARCADIA_ENGINE_UI_NODES_LAYOUTPANELNODE_H_INCLUDED
