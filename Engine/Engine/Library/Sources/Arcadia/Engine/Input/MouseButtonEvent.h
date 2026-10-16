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

#if !defined(ARCADIA_ENGINE_INPUT_MOUSEBUTTONEVENT_H_INCLUDED)
#define ARCADIA_ENGINE_INPUT_MOUSEBUTTONEVENT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Input/MouseButtonEvent.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Event.h"
#include "Arcadia/Engine/Input/MouseButtonAction.h"

typedef struct Arcadia_Engine_Input_MouseButtonEvent Arcadia_Engine_Input_MouseButtonEvent;

Arcadia_declareObjectType(u8"Arcadia.Engine.Input.MouseButtonEvent", Arcadia_Engine_Input_MouseButtonEvent,
                          u8"Arcadia.Engine.Event");

struct Arcadia_Engine_Input_MouseButtonEventDispatch {
  Arcadia_Engine_EventDispatch _parent;
};

struct Arcadia_Engine_Input_MouseButtonEvent {
  Arcadia_Engine_Event _parent;
  Arcadia_Natural64Value button;
  Arcadia_Engine_Input_MouseButtonAction action;
  Arcadia_Integer32Value x;
  Arcadia_Integer32Value y;
};

Arcadia_Engine_Input_MouseButtonEvent*
Arcadia_Engine_Input_MouseButtonEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_Input_MouseButtonAction action,
    Arcadia_Natural64Value button,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );

#endif // ARCADIA_ENGINE_INPUT_MOUSEBUTTONEVENT_H_INCLUDED
