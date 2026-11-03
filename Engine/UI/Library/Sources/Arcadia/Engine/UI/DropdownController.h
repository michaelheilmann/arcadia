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

#if !defined(ARCADIA_ENGINE_UI_DROPDOWNCONTROLLER_H_INCLUDED)
#define ARCADIA_ENGINE_UI_DROPDOWNCONTROLLER_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Engine/UI/Nodes/ButtonNode.h"
#include "Arcadia/Engine/UI/Nodes/CanvasNode.h"
#include "Arcadia/Engine/UI/Nodes/ScrollPanelNode.h"

// Behavior-only composition: the anchor and every option are ordinary buttons.
// The popup is an ordinary scroll panel described using existing ADL nodes.
// Selection values are the option buttons' unique widget names.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.DropdownController", Arcadia_Engine_UI_DropdownController,
                          u8"Arcadia.Object")

struct Arcadia_Engine_UI_DropdownControllerDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_UI_DropdownController {
  Arcadia_Object _parent;
  Arcadia_Engine_UI_CanvasNode* canvas;
  Arcadia_Engine_UI_ButtonNode* anchor;
  Arcadia_Engine_UI_ScrollPanelNode* popup;
  Arcadia_Signal* selectionChanged;
  // Strongly retain and later finalize activation subscriptions.
  Arcadia_List* connections;
  Arcadia_Natural64Value selectedIndex;
  Arcadia_Natural64Value highlightedIndex;
  Arcadia_Integer32Value requestedPopupWidth;
  Arcadia_Integer32Value requestedPopupHeight;
};

// The popup must contain only buttons with nonempty, unique widget names.
// The option set is fixed after construction; buttons may be enabled/disabled.
// An empty popup is valid and cannot open. No UI.Dropdown ADL type is required.
Arcadia_Engine_UI_DropdownController*
Arcadia_Engine_UI_DropdownController_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_Engine_UI_ButtonNode* anchor,
    Arcadia_Engine_UI_ScrollPanelNode* popup
  );

void
Arcadia_Engine_UI_DropdownController_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

void
Arcadia_Engine_UI_DropdownController_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_DropdownController_isOpen
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

// Call after a canvas resize or anchor layout change, including while open.
void
Arcadia_Engine_UI_DropdownController_reposition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

// SelectedIndex is Natural64_Maximum when no selection exists.
// Invalid or disabled indices are rejected without changing state.
void
Arcadia_Engine_UI_DropdownController_select
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Natural64Value index
  );

Arcadia_Natural64Value
Arcadia_Engine_UI_DropdownController_getSelectedIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

// Values are option widget names, and remain stable across label changes.
Arcadia_String*
Arcadia_Engine_UI_DropdownController_getSelectedValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

Arcadia_String*
Arcadia_Engine_UI_DropdownController_getSelectedLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

void
Arcadia_Engine_UI_DropdownController_selectValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_String* value
  );

void
Arcadia_Engine_UI_DropdownController_setEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_BooleanValue enabled
  );

Arcadia_Signal*
Arcadia_Engine_UI_DropdownController_getSelectionChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  );

#endif // ARCADIA_ENGINE_UI_DROPDOWNCONTROLLER_H_INCLUDED
