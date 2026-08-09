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

#if !defined(ARCADIA_ENGINE_VISUALS_CANVASEVENT_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_CANVASEVENT_H_INCLUDED

#include "Arcadia/Engine/Event.h"

// https://michaelheilmann.com/Arcadia/Engine/#Arcadia_Engine_Visuals_CanvasEvent
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.CanvasEvent", Arcadia_Engine_Visuals_CanvasEvent,
                          u8"Arcadia.Engine.Event");

struct Arcadia_Engine_Visuals_CanvasEventDispatch {
  Arcadia_Engine_EventDispatch parent;
};

struct Arcadia_Engine_Visuals_CanvasEvent {
  Arcadia_Engine_Event _parent;
};

#endif // ARCADIA_ENGINE_VISUALS_CANVASEVENT_H_INCLUDED
