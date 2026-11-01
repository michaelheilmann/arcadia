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

#include "Arcadia/Engine/Tests/Nodes/Cases.h"
#include "Arcadia/Engine/Include.h"

#include <math.h>
#include <stdlib.h>

static void
testClock
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue rootBase = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_Clock* clock = Arcadia_Engine_Clock_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)clock);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)clock, _Arcadia_Engine_Clock_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Clock_getPaused(thread, clock));
  Arcadia_Tests_assertEqualReal64(thread, 0.0, Arcadia_Engine_Clock_getTick(thread, clock));

  Arcadia_Engine_Clock* another = Arcadia_Engine_Clock_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)another);
  Arcadia_Tests_assertTrue(thread, another != clock);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Clock_getPaused(thread, another));

  Arcadia_Engine_Clock_setPaused(thread, clock, Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Clock_getPaused(thread, clock));
  Arcadia_Engine_Clock_setPaused(thread, clock, Arcadia_BooleanValue_False);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Clock_getPaused(thread, clock));
  Arcadia_Engine_Clock_setPaused(thread, clock, Arcadia_BooleanValue_False);
  Arcadia_Real64Value tick = Arcadia_Engine_Clock_getTick(thread, clock);
  Arcadia_Tests_assertTrue(thread, isfinite(tick) && tick >= 0.0);
  tick = Arcadia_Engine_Clock_getTick(thread, clock);
  Arcadia_Tests_assertTrue(thread, isfinite(tick) && tick >= 0.0);

  Arcadia_Engine_Clock_setPaused(thread, clock, Arcadia_BooleanValue_True);
  Arcadia_Tests_assertEqualReal64(thread, 0.0, Arcadia_Engine_Clock_getTick(thread, clock));
  Arcadia_Engine_Clock_setPaused(thread, clock, Arcadia_BooleanValue_True);
  Arcadia_Engine_Clock_setPaused(thread, clock, Arcadia_BooleanValue_False);
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Clock_getPaused(thread, clock));
  tick = Arcadia_Engine_Clock_getTick(thread, clock);
  Arcadia_Tests_assertTrue(thread, isfinite(tick) && tick >= 0.0);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Clock_getPaused(thread, another));
  Arcadia_Tests_assertEqualReal64(thread, 0.0, Arcadia_Engine_Clock_getTick(thread, another));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - rootBase);
}

int
Arcadia_Engine_Tests_Nodes_clock
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testClock) ? EXIT_SUCCESS : EXIT_FAILURE;
}
