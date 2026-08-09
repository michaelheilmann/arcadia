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

#include "Arcadia/Ring2/Include.h"
#include <stdlib.h>
#include <string.h>

// (1) Assert two different Arcadia.String object Arcadia.Value objects a and b are considered as equal
//     - that is, Arcadia.Value.isEqualTo(&a,&b) returns Arcadia.Boolean.True - if
//     the values of their Bytes are equal. Also assert that
//     Arcadia.Value.isNotEqualTo(&a,&b) returns Arcadia.Boolean.False in that case.
// (2) Assert two different Arcadia.String object Arcadia.Vallue objects a and b are considered as NOT equal
//     - that is, Arcadia.Object.isEqualTo(a,b) returns Arcadia.Boolean.False - if
//     the values of their Bytes are NOT equal. Also assert that
//     Arcadia.Value.isNotEqualTo(&a, &b) returns Arcadia.Boolean.True in that case.
static void
testEqualsTo
  (
    Arcadia_Thread* thread
  )
{
  // (1)
  {
    Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x")),
                  b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x"));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &a, &b));
    Arcadia_Tests_assertFalse(thread, Arcadia_Value_isNotEqualTo(thread, &a, &b));
  }
  // (2)
  {
    Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x")),
                  b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"y"));
    Arcadia_Tests_assertFalse(thread, Arcadia_Value_isEqualTo(thread, &a, &b));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isNotEqualTo(thread, &a, &b));
  }
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testEqualsTo)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
