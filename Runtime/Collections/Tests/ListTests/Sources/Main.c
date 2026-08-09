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

#include "Arcadia.Ring2.Tests.ListTests/removeAt.h"
#include "Arcadia/Collections/Include.h"

// We use integers for testing.
// Append range [1,7]. Assert the range was added. Assert getSize returns 7. Assert isEmpty returns false.
// Clear the list. Assert getSize returns 0. Assert isEmpty returns true.
static void
listTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* l = (Arcadia_List*)Arcadia_ArrayList_create(thread);

  for (Arcadia_Integer32Value i = 1, n = 7; i <= n; ++i) {
    Arcadia_List_insertBackInteger32Value(thread, l, i);
    Arcadia_Tests_assertTrue(thread, i == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)l));
  }
  Arcadia_Tests_assertTrue(thread, 7 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)l));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False == Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)l));
  for (Arcadia_Integer32Value i = 1, n = 7; i <= n; ++i) {
    Arcadia_Tests_assertTrue(thread, i == Arcadia_List_getInteger32ValueAt(thread, l, i - 1));
  }
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)l);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)l));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)l));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&listTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Collections_Tests_ListTests_removeAt)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
