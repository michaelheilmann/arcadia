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
clampTests
  (
    Arcadia_Thread* thread
  )
{
#define Do(Type, Suffix) \
  Arcadia_Tests_assertTrue(thread, Type##Value_Literal(0) == Arcadia_clamp##Suffix##Value(thread, Type##Value_Literal(0), Type##Value_Minimum,    Type##Value_Maximum)); \
  Arcadia_Tests_assertTrue(thread, Type##Value_Literal(0) == Arcadia_clamp##Suffix##Value(thread, Type##Value_Minimum,    Type##Value_Literal(0), Type##Value_Maximum)); \
  Arcadia_Tests_assertTrue(thread, Type##Value_Literal(0) == Arcadia_clamp##Suffix##Value(thread, Type##Value_Maximum,    Type##Value_Minimum,    Type##Value_Literal(0))); \
  Arcadia_Tests_assertTrue(thread, Type##Value_Minimum    == Arcadia_clamp##Suffix##Value(thread, Type##Value_Maximum,    Type##Value_Minimum,    Type##Value_Minimum)); \
  Arcadia_Tests_assertTrue(thread, Type##Value_Maximum    == Arcadia_clamp##Suffix##Value(thread, Type##Value_Minimum,    Type##Value_Maximum,    Type##Value_Maximum));

  Do(Arcadia_Integer16, Integer16)
  Do(Arcadia_Integer32, Integer32)
  Do(Arcadia_Integer64, Integer64)
  Do(Arcadia_Integer8, Integer8)

  Do(Arcadia_Natural16, Natural16)
  Do(Arcadia_Natural32, Natural32)
  Do(Arcadia_Natural64, Natural64)
  Do(Arcadia_Natural8, Natural8)

  Do(Arcadia_Size, Size)

#undef Do
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&clampTests)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
