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

#if !defined(ARCADIA_STARSHIP_POSITIONCHANGEINPUTEVENT_H_INCLUDED)
#define ARCADIA_STARSHIP_POSITIONCHANGEINPUTEVENT_H_INCLUDED

#include "Arcadia/Engine/Include.h"

/// @brief Event requesting a "position change" of an object.
/// The event specifies a translation.
Arcadia_declareObjectType(u8"Arcadia.Starship.PositionChangeInputEvent", Arcadia_Starship_PositionChangeInputEvent,
                          u8"Arcadia.Engine.Event");

struct Arcadia_Starship_PositionChangeInputEventDispatch {
  Arcadia_Engine_EventDispatch parent;
};

struct Arcadia_Starship_PositionChangeInputEvent {
  Arcadia_Engine_Event parent;

  /// @brief The translation value.
  Arcadia_Math_Vector3Real32* translation;
};

/// @param timestamp The timestamp of the event.
/// @param translation The translation value.
/// @return A pointer to the "position change input" event.
Arcadia_Starship_PositionChangeInputEvent*
Arcadia_Starship_PositionChangeInputEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Math_Vector3Real32* translation
  );

#endif // ARCADIA_STARSHIP_POSITIONCHANGEINPUTEVENT_H_INCLUDED
