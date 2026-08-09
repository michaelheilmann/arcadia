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

// Create a deque.
// Assert the create function does not return null.
// Assert the size of the deque is zero.
static void
dequeTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Deque* a = (Arcadia_Deque*)Arcadia_ArrayDeque_create(thread);
  Arcadia_Tests_assertTrue(thread, NULL != a);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));
}

// Create a deque.
//
// Assert the create function does not return null.
// Assert the size of the deque is zero.
static void
dequeTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Value v;
  Arcadia_Deque* a = (Arcadia_Deque*)Arcadia_ArrayDeque_create(thread);
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(1));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(2));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(3));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(4));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(5));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(6));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(7));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(8));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(9));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 9 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(8) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 8 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(7) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 7 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(6) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 6 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(5) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 5 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(4) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 4 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(3) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 3 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(2) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 2 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(1) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));

  v = Arcadia_Deque_getFront(thread, a);
  Arcadia_Deque_removeFront(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isInteger32Value(&v) && 1 == Arcadia_Value_getInteger32Value(&v));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a));
}

static void
dequeTest3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Deque* a = (Arcadia_Deque*)Arcadia_ArrayDeque_create(thread);
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(1));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(2));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(3));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(4));
  Arcadia_Deque_insertFront(thread, a, Arcadia_Value_makeInteger32Value(5));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&dequeTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&dequeTest2)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
