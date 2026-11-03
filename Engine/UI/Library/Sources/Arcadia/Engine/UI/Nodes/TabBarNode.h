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

#if !defined(ARCADIA_ENGINE_UI_NODES_TABBARNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_TABBARNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Engine/UI/Nodes/ButtonNode.h"
#include "Arcadia/Engine/UI/Nodes/LayoutPanelNode.h"

// A layout panel with tab-selection behavior. Children are ordinary buttons.
// Render, pointer capture, layout and focus are inherited from LayoutPanelNode.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.TabBarNode", Arcadia_Engine_UI_TabBarNode,
                          u8"Arcadia.Engine.UI.LayoutPanelNode")

struct Arcadia_Engine_UI_TabBarNodeDispatch {
  Arcadia_Engine_UI_LayoutPanelNodeDispatch _parent;
};

struct Arcadia_Engine_UI_TabBarNode {
  Arcadia_Engine_UI_LayoutPanelNode _parent;
  Arcadia_List* activationConnections;
  Arcadia_List* enabledConnections;
  Arcadia_Engine_UI_ButtonNode* selected;
  Arcadia_Signal* selectionChanged;
  Arcadia_BooleanValue vertical;
};

/// @brief Create a horizontal or vertical tab bar with a StackLayout.
/// @remarks The bar is initially empty and has no selection. Add ordinary
///          ButtonNode children through the Container interface. It owns only
///          tab headers, not application content or content switching.
Arcadia_Engine_UI_TabBarNode*
Arcadia_Engine_UI_TabBarNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap
  );

/// @brief Select a member button, or NULL to clear selection.
/// @remarks The button must be enabled. Re-selecting the same button is a no-op.
///          Focus and selection are separate; programmatic selection does not
///          move keyboard focus. Invalid selection leaves state unchanged.
void
Arcadia_Engine_UI_TabBarNode_select
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_Engine_UI_ButtonNode* button
  );

/// @brief Return the selected button, or NULL if none is selected.
Arcadia_Engine_UI_ButtonNode*
Arcadia_Engine_UI_TabBarNode_getSelected
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  );

/// @brief The signal fires after changing selected state, with this bar as sender.
/// @remarks Subscribers may query getSelected. Re-entrant selection is permitted:
///          the innermost completed change wins. Notification is not cancellable;
///          callback failure does not roll back an already committed selection.
///          No signal is sent on repeat selection or when a non-selected
///          button is disabled or removed.
Arcadia_Signal*
Arcadia_Engine_UI_TabBarNode_getSelectionChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  );

#endif // ARCADIA_ENGINE_UI_NODES_TABBARNODE_H_INCLUDED
