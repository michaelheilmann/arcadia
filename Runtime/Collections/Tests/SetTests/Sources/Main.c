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
#include <string.h>

#include "Arcadia/Collections/Include.h"

// Add a string object x with string "x".
// Add a string object y with string "y".
static void
setTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Set* v = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x")),
                b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x"));
  Arcadia_Set_add(thread, v, a, NULL);
  Arcadia_Set_add(thread, v, b, NULL);
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&setTest1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
