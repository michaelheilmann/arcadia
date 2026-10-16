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

#if !defined(ARCADIA_ADL_DEFINITIONS_UI_CONTAINERDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_UI_CONTAINERDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif

#include "Arcadia/ADL/Definition.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareInterfaceType(u8"Arcadia.ADL.UI.ContainerDefinition", Arcadia_ADL_UI_ContainerDefinition)

struct Arcadia_ADL_UI_ContainerDefinitionDispatch {
  Arcadia_InterfaceDispatch parent;
  Arcadia_List* (*getChildReferences)(Arcadia_Thread* thread, Arcadia_Object* self);
};

Arcadia_List*
Arcadia_ADL_UI_ContainerDefinition_getChildReferences
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

void
Arcadia_ADL_UI_ContainerDefinition_addChildReference
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_String* childName
  );

#endif // ARCADIA_ADL_DEFINITIONS_UI_CONTAINERDEFINITION_H_INCLUDED
