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

#if !defined(ARCADIA_ENGINE_INPUT_MOUSEPOINTEREVENTEXTENSIONS_H_INCLUDED)
#define ARCADIA_ENGINE_INPUT_MOUSEPOINTEREVENTEXTENSIONS_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Input/MousePointerEventExtensions.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Engine/Input/MousePointerEvent.h"
#include "Arcadia/Engine/Input/MousePointerAction.h"

// Native helper: the MousePointerEvent.h declaration is owned by PDLC and
// regeneration must not overwrite this scroll factory.
static inline Arcadia_Engine_Input_MousePointerEvent*
Arcadia_Engine_Input_MousePointerEvent_createScrolled
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y,
    Arcadia_Integer32Value wheelDeltaX,
    Arcadia_Integer32Value wheelDeltaY
  )
{
  Arcadia_Engine_Input_MousePointerEvent* event = Arcadia_Engine_Input_MousePointerEvent_create
    (thread, timestamp, Arcadia_Engine_Input_MousePointerAction_Scrolled, x, y);
  event->wheelDeltaX = wheelDeltaX;
  event->wheelDeltaY = wheelDeltaY;
  return event;
}

#endif // ARCADIA_ENGINE_INPUT_MOUSEPOINTEREVENTEXTENSIONS_H_INCLUDED
