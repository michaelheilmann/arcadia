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

#ifndef ARCADIA_ENGINE_VISUALS_CANVASDPICHANGEDEVENT_H_INCLUDED
#define ARCADIA_ENGINE_VISUALS_CANVASDPICHANGEDEVENT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/CanvasDPIChangedEvent.h.g` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareClassType("Arcadia.Engine.Visuals.CanvasDPIChangedEvent", Arcadia_Engine_Visuals_CanvasDPIChangedEvent,
                         "Arcadia.Engine.Visuals.CanvasEvent");

struct Arcadia_Engine_Visuals_CanvasDPIChangedEventDispatch {
  Arcadia_Engine_Visuals_CanvasEventDispatch  _parent;
};

struct Arcadia_Engine_Visuals_CanvasDPIChangedEvent {
  Arcadia_Engine_Visuals_CanvasEvent  _parent;

  Arcadia_Integer32 horizontalDpi;
  Arcadia_Integer32 verticalDpi;
};

#endif // ARCADIA_ENGINE_VISUALS_CANVASDPICHANGEDEVENT_H_INCLUDED
