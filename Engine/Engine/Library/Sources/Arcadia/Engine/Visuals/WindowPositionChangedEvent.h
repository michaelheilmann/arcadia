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

#if !defined(ARCADIA_VISUALS_EVENTS_WINDOWPOSITIONCHANGEDEVENT_H_INCLUDED)
#define ARCADIA_VISUALS_EVENTS_WINDOWPOSITIONCHANGEDEVENT_H_INCLUDED

#include "Arcadia/Engine/Event.h"
typedef struct Arcadia_Engine_Visuals_Window Arcadia_Engine_Visuals_Window;

// https://michaelheilmann.com/Arcadia/Engine/#Arcadia_Engine_Visuals_WindowPositionChangedEvent
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.WindowPositionChangedEvent", Arcadia_Engine_Visuals_WindowPositionChangedEvent,
                          u8"Arcadia.Engine.Event");

struct Arcadia_Engine_Visuals_WindowPositionChangedEventDispatch {
  Arcadia_Engine_EventDispatch _parent;
};

struct Arcadia_Engine_Visuals_WindowPositionChangedEvent {
  Arcadia_Engine_Event _parent;
  Arcadia_Engine_Visuals_Window* window;
  Arcadia_Integer32Value horizontalPosition;
  Arcadia_Integer32Value verticalPosition;
};

Arcadia_Engine_Visuals_WindowPositionChangedEvent*
Arcadia_Engine_Visuals_WindowPositionChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp,
    Arcadia_Engine_Visuals_Window* window,
    Arcadia_Integer32Value horizontalPosition,
    Arcadia_Integer32Value verticalPosition
  );

#endif // ARCADIA_VISUALS_EVENTS_WINDOWPOSITIONCHANGEDEVENT_H_INCLUDED
