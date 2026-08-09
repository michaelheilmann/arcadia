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

// We use integers for testing.
// Append range [1,7]. Assert the range was added. Assert getSize returns 7. Assert isEmpty returns false.
// Clear the list. Assert getSize returns 0. Assert isEmpty returns true.
static void
mapTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Map* m = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Value k, v;
  Arcadia_Value_setObjectReferenceValue(&k, (Arcadia_ObjectReferenceValue)Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"siteAddress", sizeof(u8"siteAddress") - 1)));
  Arcadia_Value_setObjectReferenceValue(&v, (Arcadia_ObjectReferenceValue)Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"https://michaelheilmann.com", sizeof(u8"https://michaelheilmann.com") - 1)));
  Arcadia_Map_set(thread, m, k, v, NULL, NULL);
  Arcadia_Value k2, v2;
  Arcadia_Value_setObjectReferenceValue(&k2, (Arcadia_ObjectReferenceValue)Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"siteAddress", sizeof(u8"siteAddress") - 1)));
  v2 = Arcadia_Map_get(thread, m, k2);
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&mapTest1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
