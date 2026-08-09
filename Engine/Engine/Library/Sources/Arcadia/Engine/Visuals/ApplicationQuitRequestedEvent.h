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

#if !defined(ARCADIA_ENGINE_VISUALS_APPLICATIONQUITREQUESTEDEVENT_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_APPLICATIONQUITREQUESTEDEVENT_H_INCLUDED

#include "Arcadia/Engine/Visuals/ApplicationEvent.h"

// The opaque type of an "application" message.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.ApplicationQuitRequestedEvent", Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent,
                          u8"Arcadia.Engine.Visuals.ApplicationEvent");

struct Arcadia_Engine_Visuals_ApplicationQuitRequestedEventDispatch {
  Arcadia_Engine_Visuals_ApplicationEventDispatch parent;
};

struct Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent {
  Arcadia_Engine_Visuals_ApplicationEvent _parent;
};

Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent*
Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value timestamp
  );

#endif // ARCADIA_ENGINE_VISUALS_APPLICATIONQUITREQUESTEDEVENT_H_INCLUDED
