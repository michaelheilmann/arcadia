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
#include "Arcadia/ADL/Definitions/UI/ButtonDefinition.h"

#include "Arcadia/ADL/Definitions/Visuals/ColorDefinition.h"
#include "Arcadia/ADL/Reference.h"

static void
linkColorReference
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* reference
  )
{
  if (!reference) return;
  Arcadia_ADL_Reference_resolve(thread, reference);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)reference->definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_UI_ButtonDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
  )
{
  linkColorReference(thread, self->backgroundColor);
  linkColorReference(thread, self->hoveredBackgroundColor);
  linkColorReference(thread, self->pressedBackgroundColor);
  linkColorReference(thread, self->disabledBackgroundColor);
  linkColorReference(thread, self->focusedBackgroundColor);
}

Arcadia_ADL_Reference*
Arcadia_ADL_UI_createOptionalColorReferenceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name
  )
{
  return name ? Arcadia_ADL_Reference_create(thread, definitions, name) : NULL;
}
