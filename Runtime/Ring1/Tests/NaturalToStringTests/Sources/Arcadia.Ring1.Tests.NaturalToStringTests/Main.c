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
testFixture
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value v
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromNatural64(thread, v);
  Arcadia_Natural64Value w = Arcadia_RuntimeUTF8String_toNatural64(thread, u);
  Arcadia_Tests_assertTrue(thread, v == w);
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  testFixture(thread, Arcadia_Natural64Value_Minimum);
  testFixture(thread, Arcadia_Natural64Value_Maximum);
  testFixture(thread, Arcadia_Natural64Value_Literal(0));
  testFixture(thread, Arcadia_Natural64Value_Literal(1));
  testFixture(thread, Arcadia_Natural64Value_Literal(123));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
