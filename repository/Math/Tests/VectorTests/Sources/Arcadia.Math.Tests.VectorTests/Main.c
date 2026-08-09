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

#include "Arcadia/Math/Include.h"

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Math_Vector3Real32 *u, *v, *w;
  Arcadia_Value t;

  // u must be equal to itself.
  u = Arcadia_Math_Vector3Real32_create(thread);
  t = Arcadia_Value_makeObjectReferenceValue(u);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));

  // u and v are created with the same component values => u and v must be equal..
  v = Arcadia_Math_Vector3Real32_create(thread);
  t = Arcadia_Value_makeObjectReferenceValue(v);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));

  // w is clone of u => w and u must be equal.
  w = (Arcadia_Math_Vector3Real32*)Arcadia_Object_clone(thread, (Arcadia_Object*)u);
  t = Arcadia_Value_makeObjectReferenceValue(w);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));
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
