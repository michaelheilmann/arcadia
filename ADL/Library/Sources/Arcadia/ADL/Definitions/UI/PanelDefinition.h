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

#if !defined(ARCADIA_ADL_DEFINITIONS_UI_PANELDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_UI_PANELDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE) || 1 != ARCADIA_ADL_PRIVATE
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif

#include "Arcadia/ADL/Definition.h"
#include "Arcadia/Collections/Include.h"

// A generic container definition with an explicitly selected layout strategy.
// The first supported strategy is UI.StackLayout.
// Alignment values belong to ADL and do not depend on Engine/UI enums.
typedef enum Arcadia_ADL_UI_PanelChildCrossAlignment {
  Arcadia_ADL_UI_PanelChildCrossAlignment_Start,
  Arcadia_ADL_UI_PanelChildCrossAlignment_Center,
  Arcadia_ADL_UI_PanelChildCrossAlignment_End,
  Arcadia_ADL_UI_PanelChildCrossAlignment_Stretch,
} Arcadia_ADL_UI_PanelChildCrossAlignment;

typedef struct Arcadia_ADL_UI_PanelChildLayout {
  Arcadia_Integer32Value marginLeft, marginBottom, marginRight, marginTop;
  Arcadia_ADL_UI_PanelChildCrossAlignment crossAlignment;
  Arcadia_BooleanValue hasCrossAlignment;
  Arcadia_BooleanValue configured;
} Arcadia_ADL_UI_PanelChildLayout;
Arcadia_declareObjectType(u8"Arcadia.ADL.UI.PanelDefinition", Arcadia_ADL_UI_PanelDefinition,
                          u8"Arcadia.ADL.Definition")

struct Arcadia_ADL_UI_PanelDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_UI_PanelDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_Integer32Value left, bottom;
  Arcadia_Natural32Value width, height;
  Arcadia_String* layoutType;
  // Optional: "Content" (default) or "Fixed"; belongs to the panel, not the layout strategy.
  Arcadia_String* sizePolicy;
  Arcadia_BooleanValue vertical;
  Arcadia_Integer32Value gap;
  Arcadia_Integer32Value paddingLeft, paddingBottom, paddingRight, paddingTop;
  Arcadia_String* mainAlignment;
  Arcadia_String* crossAlignment;
  // Optional model-backed background, independent of the layout strategy.
  Arcadia_String* backgroundModelName;
  Arcadia_Natural32Value backgroundBorderWidth;
  Arcadia_List* childReferences;
  // One unmanaged POD slot per child reference, if overrides were declared.
  // The definition owns this memory; it contains no managed object pointers.
  Arcadia_ADL_UI_PanelChildLayout* childLayouts;
  Arcadia_SizeValue childLayoutCount;
};

Arcadia_ADL_UI_PanelDefinition*
Arcadia_ADL_UI_PanelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  );

#endif // ARCADIA_ADL_DEFINITIONS_UI_PANELDEFINITION_H_INCLUDED
