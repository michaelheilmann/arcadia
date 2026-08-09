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

#if !defined(ARCADIA_STARSHIP_ORIENTATIONCHANGEINPUTEVENT_H_INCLUDED)
#define ARCADIA_STARSHIP_ORIENTATIONCHANGEINPUTEVENT_H_INCLUDED

#include "Arcadia/Engine/Include.h"

/// @brief Event requesting a "orientation change" of an object.
/// The event specifies a rotation.
Arcadia_declareObjectType(u8"Arcadia.Starship.OrientationChangeInputEvent", Arcadia_Starship_OrientationChangeInputEvent,
                          u8"Arcadia.Engine.Event");

struct Arcadia_Starship_OrientationChangeInputEventDispatch {
  Arcadia_Engine_EventDispatch parent;
};

struct Arcadia_Starship_OrientationChangeInputEvent {
  Arcadia_Engine_Event parent;

  /// @brief The rotation value.
  Arcadia_Math_QuaternionReal32* rotation;
};

/// @param timestamp The timestamp of the event.
/// @param rotation The rotation value.
/// @return A pointer to the "orientation change input" event.
Arcadia_Starship_OrientationChangeInputEvent*
Arcadia_Starship_OrientationChangeInputEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Math_QuaternionReal32* rotation
  );

#endif // ARCADIA_STARSHIP_ORIENTATIONCHANGEINPUTEVENT_H_INCLUDED
