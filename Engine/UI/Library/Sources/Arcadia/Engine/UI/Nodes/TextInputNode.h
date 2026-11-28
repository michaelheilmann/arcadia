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

#if !defined(ARCADIA_ENGINE_UI_NODES_TEXTINPUTNODE_H_INCLUDED)
#define ARCADIA_ENGINE_UI_NODES_TEXTINPUTNODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("include Arcadia/Engine/UI/Include.h instead")
#endif

#include "Arcadia/Engine/UI/WidgetNode.h"
#include "Arcadia/Engine/UI/TextEditState.h"
#include "Arcadia/Engine/UI/Nodes/LabelNode.h"

/// Interaction layer of a single-line text input. Native text delivery is
/// independent of physical key events; insertText is its explicit entry point.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.TextInputNode", Arcadia_Engine_UI_TextInputNode,
                          u8"Arcadia.Engine.UI.WidgetNode")

struct Arcadia_Engine_UI_TextInputNodeDispatch {
  Arcadia_Engine_UI_WidgetNodeDispatch parent;
};

struct Arcadia_Engine_UI_TextInputNode {
  Arcadia_Engine_UI_WidgetNode parent;
  Arcadia_Engine_UI_TextEditState* editor;
  Arcadia_BooleanValue enabled;
  Arcadia_BooleanValue readOnly;
  Arcadia_BooleanValue focused;
  Arcadia_Natural8Value shiftSides;
  Arcadia_Engine_UI_LabelNode* label;
  Arcadia_Engine_Visuals_BackendContext* backendContext;
  Arcadia_Real32Value scrollX;
  Arcadia_BooleanValue dragging;
};

Arcadia_Engine_UI_TextInputNode*
Arcadia_Engine_UI_TextInputNode_create(Arcadia_Thread* thread);

Arcadia_Engine_UI_TextEditState*
Arcadia_Engine_UI_TextInputNode_getEditor(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self);

void
Arcadia_Engine_UI_TextInputNode_setEnabled(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_BooleanValue enabled);

void
Arcadia_Engine_UI_TextInputNode_setReadOnly(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_BooleanValue readOnly);

/// Returns whether input changed the draft. Disabled, unfocused and read-only
/// nodes reject user text without mutating their editing state.
Arcadia_BooleanValue
Arcadia_Engine_UI_TextInputNode_insertText(Arcadia_Thread* thread, Arcadia_Engine_UI_TextInputNode* self, Arcadia_String* text);

#endif // ARCADIA_ENGINE_UI_NODES_TEXTINPUTNODE_H_INCLUDED
