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

#if !defined(ARCADIA_ENGINE_INPUT_KEYBOARDKEYEVENT_H_INCLUDED)
#define ARCADIA_ENGINE_INPUT_KEYBOARDKEYEVENT_H_INCLUDED

#include "Arcadia/Engine/Event.h"
#include "Arcadia/Engine/Input/KeyboardKey.h"
#include "Arcadia/Engine/Input/KeyboardKeyAction.h"

// https://michaelheilmann.com/Arcadia/Engine/#Arcadia_Engine_Input_KeyboardKeyEvent
Arcadia_declareObjectType(u8"Arcadia.Engine.Input.KeyboardKeyEvent", Arcadia_Engine_Input_KeyboardKeyEvent,
                          u8"Arcadia.Engine.Event");

struct Arcadia_Engine_Input_KeyboardKeyEventDispatch {
  Arcadia_Engine_EventDispatch _parent;
};

struct Arcadia_Engine_Input_KeyboardKeyEvent {
  Arcadia_Engine_Event _parent;
  Arcadia_Engine_Input_KeyboardKey key;
  Arcadia_Engine_Input_KeyboardKeyAction action;
};

/// @brief Create a keyboard key event.
/// @param key The keyboard key.
/// @param action The action of the keyboard key.
/// #Arcadia_Visuals_KeyboardKeyAction_Pressed if the keyboard key was pressed,
/// #Arcadia_Visuals_KeyboardKeyAction_Released if the keyboard key was released.
/// @return The keyboard key event.
Arcadia_Engine_Input_KeyboardKeyEvent*
Arcadia_Engine_Input_KeyboardKeyEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_Input_KeyboardKeyAction action,
    Arcadia_Engine_Input_KeyboardKey key
  );

Arcadia_Engine_Input_KeyboardKeyAction
Arcadia_Engine_Input_KeyboardKeyEvent_getAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  );

Arcadia_Engine_Input_KeyboardKey
Arcadia_Engine_Input_KeyboardKeyEvent_getKey
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  );

#endif // ARCADIA_ENGINE_INPUT_KEYBOARDKEYEVENT_H_INCLUDED
