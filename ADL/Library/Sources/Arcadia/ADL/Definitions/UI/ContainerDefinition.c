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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Definitions/UI/ContainerDefinition.h"

#include "Arcadia/ADL/Reference.h"

Arcadia_defineInterfaceType(u8"Arcadia.ADL.UI.ContainerDefinition", Arcadia_ADL_UI_ContainerDefinition, NULL)

Arcadia_List*
Arcadia_ADL_UI_ContainerDefinition_getChildReferences
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_InterfaceCallWithReturn(Arcadia_ADL_UI_ContainerDefinition, getChildReferences, self);
}

void
Arcadia_ADL_UI_ContainerDefinition_addChildReference
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_String* childName
  )
{
  Arcadia_List* childReferences = Arcadia_ADL_UI_ContainerDefinition_getChildReferences(thread, self);
  Arcadia_ADL_Reference* childReference = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions, childName);
  Arcadia_List_insertBackObjectReferenceValue(thread, childReferences, (Arcadia_Object*)childReference);
}
