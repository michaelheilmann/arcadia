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

#if !defined(ARCADIA_ENGINE_CLOCK_H_INCLUDED)
#define ARCADIA_ENGINE_CLOCK_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"

// A managed, pause-aware elapsed-time clock. Not safe for concurrent use.
// Its time source is the operating system's monotonic high-resolution clock.
Arcadia_declareObjectType(u8"Arcadia.Engine.Clock", Arcadia_Engine_Clock,
                          u8"Arcadia.Object");

struct Arcadia_Engine_ClockDispatch {
  Arcadia_ObjectDispatch _parent;
};

// Create a paused clock owned by Arcadia's object system.
Arcadia_Engine_Clock*
Arcadia_Engine_Clock_create
  (
    Arcadia_Thread* thread
  );

// Idempotently transition between paused and running.
// Resuming resets the time baseline so paused time is never accumulated.
void
Arcadia_Engine_Clock_setPaused
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self,
    Arcadia_BooleanValue paused
  );

Arcadia_BooleanValue
Arcadia_Engine_Clock_getPaused
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self
  );

// Elapsed seconds since the previous running poll (or resume).
// While paused, return zero without advancing the clock. Backwards readings
// yield zero without moving the baseline backwards.
Arcadia_Real64Value
Arcadia_Engine_Clock_getTick
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Clock* self
  );

#endif // ARCADIA_ENGINE_CLOCK_H_INCLUDED
