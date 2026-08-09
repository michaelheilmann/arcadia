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

#include "Arcadia/Ring1/Include.h"

static void
countTrailingZeroesTests
  (
    Arcadia_Thread* thread
  )
{
#define Do(Type, VALUE, EXPECTED) \
  { \
    Arcadia_SizeValue expected = EXPECTED; \
    Arcadia_SizeValue received = Arcadia_countTrailingZeroes##Type##Value(thread, VALUE); \
    Arcadia_Tests_assertTrue(thread, received == expected); \
  }
  Do(Natural64, Arcadia_Natural64Value_Literal(0), Arcadia_SizeValue_Literal(64));
  Do(Natural64, Arcadia_Natural64Value_Literal(1), Arcadia_SizeValue_Literal(0));
  Do(Natural64, Arcadia_Natural64Value_Literal(2), Arcadia_SizeValue_Literal(1));
  Do(Natural64, Arcadia_Natural64Value_Literal(3), Arcadia_SizeValue_Literal(0));
  Do(Natural64, Arcadia_Natural64Value_Literal(4), Arcadia_SizeValue_Literal(2));
  Do(Natural64, Arcadia_Natural64Value_Maximum,    Arcadia_SizeValue_Literal(0));

  Do(Natural32, Arcadia_Natural32Value_Literal(0), Arcadia_SizeValue_Literal(32));
  Do(Natural32, Arcadia_Natural32Value_Literal(1), Arcadia_SizeValue_Literal(0));
  Do(Natural32, Arcadia_Natural32Value_Literal(2), Arcadia_SizeValue_Literal(1));
  Do(Natural32, Arcadia_Natural32Value_Literal(3), Arcadia_SizeValue_Literal(0));
  Do(Natural32, Arcadia_Natural32Value_Literal(4), Arcadia_SizeValue_Literal(2));
  Do(Natural32, Arcadia_Natural32Value_Maximum,    Arcadia_SizeValue_Literal(0));

  Do(Natural16, Arcadia_Natural16Value_Literal(0), Arcadia_SizeValue_Literal(16));
  Do(Natural16, Arcadia_Natural16Value_Literal(1), Arcadia_SizeValue_Literal(0));
  Do(Natural16, Arcadia_Natural16Value_Literal(2), Arcadia_SizeValue_Literal(1));
  Do(Natural16, Arcadia_Natural16Value_Literal(3), Arcadia_SizeValue_Literal(0));
  Do(Natural16, Arcadia_Natural16Value_Literal(4), Arcadia_SizeValue_Literal(2));
  Do(Natural16, Arcadia_Natural16Value_Maximum,    Arcadia_SizeValue_Literal(0));

  Do(Natural8, Arcadia_Natural8Value_Literal(0),   Arcadia_SizeValue_Literal(8));
  Do(Natural8, Arcadia_Natural8Value_Literal(1),   Arcadia_SizeValue_Literal(0));
  Do(Natural8, Arcadia_Natural8Value_Literal(2),   Arcadia_SizeValue_Literal(1));
  Do(Natural8, Arcadia_Natural8Value_Literal(3),   Arcadia_SizeValue_Literal(0));
  Do(Natural8, Arcadia_Natural8Value_Literal(4),   Arcadia_SizeValue_Literal(2));
  Do(Natural8, Arcadia_Natural8Value_Maximum,      Arcadia_SizeValue_Literal(0));

#undef Do
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&countTrailingZeroesTests)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
