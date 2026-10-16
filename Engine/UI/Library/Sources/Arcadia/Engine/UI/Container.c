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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/Container.h"

Arcadia_defineInterfaceType(u8"Arcadia.Engine.UI.Container", Arcadia_Engine_UI_Container

                         , NULL
                         )

Arcadia_Natural64Value
Arcadia_Engine_UI_Container_getChildCount(Arcadia_Thread* thread, Arcadia_Object* self)
{
  Arcadia_InterfaceCallWithReturn(Arcadia_Engine_UI_Container, getChildCount, self);
}

Arcadia_Object*
Arcadia_Engine_UI_Container_getChildAt(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Natural64Value index)
{
  Arcadia_InterfaceCallWithReturn(Arcadia_Engine_UI_Container, getChildAt, self, index);
}

void
Arcadia_Engine_UI_Container_addChild(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Object* child)
{
  Arcadia_InterfaceCall(Arcadia_Engine_UI_Container, addChild, self, child);
}

void
Arcadia_Engine_UI_Container_removeChild(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Object* child)
{
  Arcadia_InterfaceCall(Arcadia_Engine_UI_Container, removeChild, self, child);
}

