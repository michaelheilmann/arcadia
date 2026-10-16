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
#include "Arcadia/Engine/UI/ValueControl.h"

Arcadia_defineInterfaceType(u8"Arcadia.Engine.UI.ValueControl", Arcadia_Engine_UI_ValueControl

                         , NULL
                         )

Arcadia_Value
Arcadia_Engine_UI_ValueControl_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_InterfaceCallWithReturn(Arcadia_Engine_UI_ValueControl, getValue, self);
}

void
Arcadia_Engine_UI_ValueControl_setValue
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  )
{
  Arcadia_InterfaceCall(Arcadia_Engine_UI_ValueControl, setValue, self, value);
}
