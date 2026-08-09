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

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"

static void
testFixture32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value v
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromReal32(thread, v);
  Arcadia_Real32Value w = Arcadia_RuntimeUTF8String_toReal32(thread, u);
  Arcadia_Tests_assertTrue(thread, v == w);
}

static void
testFixture64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value v
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromReal64(thread, v);
  Arcadia_Real64Value w = Arcadia_RuntimeUTF8String_toReal64(thread, u);
  Arcadia_Tests_assertTrue(thread, v == w);
}

static void
testReal32
  (
    Arcadia_Thread* thread
  )
{
  testFixture32(thread, Arcadia_Real32Value_Minimum);
  testFixture32(thread, Arcadia_Real32Value_Maximum);
  testFixture32(thread, Arcadia_Real32Value_Literal(0.));
}

static void
testReal64
  (
    Arcadia_Thread* thread
  )
{
  testFixture64(thread, Arcadia_Real64Value_Minimum);
  testFixture64(thread, Arcadia_Real64Value_Maximum);
  testFixture64(thread, Arcadia_Real64Value_Literal(0.));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testReal32)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testReal64)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
