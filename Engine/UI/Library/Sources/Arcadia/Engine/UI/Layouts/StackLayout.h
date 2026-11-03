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

#if !defined(ARCADIA_ENGINE_UI_LAYOUTS_STACKLAYOUT_H_INCLUDED)
#define ARCADIA_ENGINE_UI_LAYOUTS_STACKLAYOUT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif

#include "Arcadia/Engine/UI/Layout.h"
#include "Arcadia/Engine/UI/Nodes/StackPanelNodeAlignment.h"

// A layout strategy: it holds configuration, not child widgets.
Arcadia_declareObjectType(u8"Arcadia.Engine.UI.StackLayout", Arcadia_Engine_UI_StackLayout,
                          u8"Arcadia.Object")

struct Arcadia_Engine_UI_StackLayoutDispatch {
  Arcadia_ObjectDispatch _parent;
};

// Compatibility with StackPanelNode's per-child layout slots. The container
// owns these records; the layout only borrows them for the duration of a call.
typedef struct Arcadia_Engine_UI_StackLayoutSlot {
  Arcadia_Integer32Value marginLeft;
  Arcadia_Integer32Value marginBottom;
  Arcadia_Integer32Value marginRight;
  Arcadia_Integer32Value marginTop;
  Arcadia_BooleanValue hasAlignmentOverride;
  Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment;
} Arcadia_Engine_UI_StackLayoutSlot;

typedef struct Arcadia_Engine_UI_StackLayoutSettings {
  Arcadia_BooleanValue vertical;
  Arcadia_Integer32Value gap;
  Arcadia_Integer32Value paddingLeft;
  Arcadia_Integer32Value paddingBottom;
  Arcadia_Integer32Value paddingRight;
  Arcadia_Integer32Value paddingTop;
  Arcadia_Engine_UI_StackPanelNodeAlignment mainAlignment;
  Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment;
} Arcadia_Engine_UI_StackLayoutSettings;

struct Arcadia_Engine_UI_StackLayout {
  Arcadia_Object _parent;
  Arcadia_Engine_UI_StackLayoutSettings settings;
};

Arcadia_Engine_UI_StackLayout*
Arcadia_Engine_UI_StackLayout_create
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap
  );

void
Arcadia_Engine_UI_StackLayout_setPadding
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackLayout* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  );

void
Arcadia_Engine_UI_StackLayout_setAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackLayout* self,
    Arcadia_Engine_UI_StackPanelNodeAlignment mainAlignment,
    Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment
  );

// Transitional adapter: preserve StackPanelNode's current geometry and
// setSize/setPosition semantics until its ADL definitions are migrated.
// Slots and settings remain owned by the caller and are not retained.
void
Arcadia_Engine_UI_StackLayout_arrangeLegacy
  (
    Arcadia_Thread* thread,
    Arcadia_Object* container,
    Arcadia_Engine_UI_StackLayoutSettings const* settings,
    Arcadia_Engine_UI_StackLayoutSlot const* slots,
    Arcadia_SizeValue slotCount,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

#endif // ARCADIA_ENGINE_UI_LAYOUTS_STACKLAYOUT_H_INCLUDED
