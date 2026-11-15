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
#include "Arcadia/Engine/UI/Layout.h"

Arcadia_defineInterfaceType(u8"Arcadia.Engine.UI.Layout", Arcadia_Engine_UI_Layout,
                            NULL)

void
Arcadia_Engine_UI_Layout_measure
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight,
    Arcadia_Integer32Value* desiredWidth,
    Arcadia_Integer32Value* desiredHeight
  )
{ Arcadia_InterfaceCall(Arcadia_Engine_UI_Layout, measure, self, container, availableWidth, availableHeight, desiredWidth, desiredHeight); }

void
Arcadia_Engine_UI_Layout_arrange
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_InterfaceCall(Arcadia_Engine_UI_Layout, arrange, self, container, left, bottom, width, height); }
