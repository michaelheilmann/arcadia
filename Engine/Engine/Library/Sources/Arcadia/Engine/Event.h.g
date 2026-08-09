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

#ifndef ARCADIA_ENGINE_EVENT_H_INCLUDED
#define ARCADIA_ENGINE_EVENT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Event.h.g` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareClassType("Arcadia.Engine.Event", Arcadia_Engine_Event,
                         "Arcadia.Object");

struct Arcadia_Engine_EventDispatch {
  Arcadia_ObjectDispatch  _parent;
};

struct Arcadia_Engine_Event {
  Arcadia_Object  _parent;

  Arcadia_Natural64 timestamp;
};

#endif // ARCADIA_ENGINE_EVENT_H_INCLUDED
