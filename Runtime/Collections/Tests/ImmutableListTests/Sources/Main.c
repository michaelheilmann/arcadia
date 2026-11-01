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

#include "Arcadia/Testing/Include.h"

#include <stdlib.h>
#include <string.h>

#include "Arcadia/Collections/Include.h"

// Create an array list with the string objects "x" and "y".
// Create an immutable list from the array list.
// Assert the immutable list has size 2, is immutable, and getAt returns non-void values.
// Assert mutating operations raise an "Operation Invalid" error.
static void
immutableListTest
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBack(thread, list, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x")));
  Arcadia_List_insertBack(thread, list, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"y")));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(2) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list));

  Arcadia_ImmutableList* immutableList = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(list));
  Arcadia_Tests_assertTrue(thread, NULL != immutableList);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(2) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableList));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Collection_isImmutable(thread, (Arcadia_Collection*)immutableList));
  Arcadia_Value a = Arcadia_List_getAt(thread, (Arcadia_List*)immutableList, Arcadia_SizeValue_Literal(0)),
                b = Arcadia_List_getAt(thread, (Arcadia_List*)immutableList, Arcadia_SizeValue_Literal(1));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isVoidValue(&a));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isVoidValue(&b));

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_List_insertBack(thread, (Arcadia_List*)immutableList, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"z")));
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Status_OperationInvalid == Arcadia_Thread_getStatus(thread));
  Arcadia_Thread_setStatus(thread, 0);
}

// Create an array list with eight string objects.
// Create an immutable list from the array list.
// Assert the immutable list has size 8 and every element is retrievable.
// Assert an immutable list created from an empty array list has size 0.
// Assert getAt at the size raises an "Operation Invalid" error.
static void
immutableListManyElementsTest
  (
    Arcadia_Thread* thread
  )
{
  static const char* values[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
  const Arcadia_SizeValue n = sizeof(values) / sizeof(values[0]);
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_List_insertBack(thread, list, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, values[i])));
  }
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list));

  Arcadia_ImmutableList* immutableList = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(list));
  Arcadia_Tests_assertTrue(thread, NULL != immutableList);
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableList));
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Value value = Arcadia_List_getAt(thread, (Arcadia_List*)immutableList, i);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isVoidValue(&value));
  }

  Arcadia_List* empty = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ImmutableList* immutableEmpty = Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(empty));
  Arcadia_Tests_assertTrue(thread, NULL != immutableEmpty);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableEmpty));

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_List_getAt(thread, (Arcadia_List*)immutableList, n);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Status_OperationInvalid == Arcadia_Thread_getStatus(thread));
  Arcadia_Thread_setStatus(thread, 0);
}

static int
runImmutability
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&immutableListTest) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runManyElements
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&immutableListManyElementsTest) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Immutability", &runImmutability },
    { "ManyElements", &runManyElements },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
