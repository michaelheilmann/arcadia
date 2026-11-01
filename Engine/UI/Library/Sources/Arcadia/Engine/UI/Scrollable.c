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
#include "Arcadia/Engine/UI/Scrollable.h"

Arcadia_defineInterfaceType(u8"Arcadia.Engine.UI.Scrollable", Arcadia_Engine_UI_Scrollable

                         , NULL
                         )

void
Arcadia_Engine_UI_Scrollable_scrollBy
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value deltaX,
    Arcadia_Integer32Value deltaY
  )
{
  Arcadia_InterfaceCall(Arcadia_Engine_UI_Scrollable, scrollBy, self, deltaX, deltaY);
}

void
Arcadia_Engine_UI_Scrollable_scrollTo
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_InterfaceCall(Arcadia_Engine_UI_Scrollable, scrollTo, self, x, y);
}
