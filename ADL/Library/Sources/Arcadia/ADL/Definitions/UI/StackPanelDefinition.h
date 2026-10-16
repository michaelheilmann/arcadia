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

#if !defined(ARCADIA_ADL_DEFINITIONS_UI_STACKPANELDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_UI_STACKPANELDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareObjectType(u8"Arcadia.ADL.UI.StackPanelDefinition", Arcadia_ADL_UI_StackPanelDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_UI_StackPanelDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_UI_StackPanelDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_Integer32Value left;
  Arcadia_Integer32Value bottom;
  Arcadia_Natural32Value width;
  Arcadia_Natural32Value height;
  Arcadia_BooleanValue vertical;
  Arcadia_Integer32Value gap;
  Arcadia_Integer32Value paddingLeft;
  Arcadia_Integer32Value paddingBottom;
  Arcadia_Integer32Value paddingRight;
  Arcadia_Integer32Value paddingTop;
  Arcadia_String* mainAlignment;
  Arcadia_String* crossAlignment;
  Arcadia_String* backgroundModelName;
  Arcadia_Natural32Value backgroundBorderWidth;
  Arcadia_List* childReferences;
};

Arcadia_ADL_UI_StackPanelDefinition*
Arcadia_ADL_UI_StackPanelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap,
    Arcadia_Integer32Value paddingLeft,
    Arcadia_Integer32Value paddingBottom,
    Arcadia_Integer32Value paddingRight,
    Arcadia_Integer32Value paddingTop,
    Arcadia_String* mainAlignment,
    Arcadia_String* crossAlignment,
    Arcadia_String* backgroundModelName,
    Arcadia_Natural32Value backgroundBorderWidth
  );

#endif // ARCADIA_ADL_DEFINITIONS_UI_STACKPANELDEFINITION_H_INCLUDED
