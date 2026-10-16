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

// Create a hash set with the string objects "x" and "y".
// Create an immutable set from the hash set.
// Assert the immutable set has size 2, is immutable, contains "x", and get "y" is not a void value.
// Assert getAll adds two values to a target list.
// Assert mutating operations raise an "Operation Invalid" error.
static void
immutableSetTest
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Set* set = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  Arcadia_Value a = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x")),
                b = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"y")),
                c = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"x"));
  Arcadia_Set_add(thread, set, a, NULL);
  Arcadia_Set_add(thread, set, b, NULL);
  Arcadia_Set_add(thread, set, c, NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(2) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)set));

  Arcadia_ImmutableSet* immutableSet = Arcadia_ImmutableSet_create(thread, Arcadia_Value_makeObjectReferenceValue(set));
  Arcadia_Tests_assertTrue(thread, NULL != immutableSet);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(2) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableSet));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Collection_isImmutable(thread, (Arcadia_Collection*)immutableSet));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Set_contains(thread, (Arcadia_Set*)immutableSet, a));
  Arcadia_Value found = Arcadia_Set_get(thread, (Arcadia_Set*)immutableSet, b);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isVoidValue(&found));

  Arcadia_List* target = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Set_getAll(thread, (Arcadia_Set*)immutableSet, target);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(2) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)target));

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Set_add(thread, (Arcadia_Set*)immutableSet, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"z")), NULL);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Status_OperationInvalid == Arcadia_Thread_getStatus(thread));
  Arcadia_Thread_setStatus(thread, 0);
}

// Create a hash set with eight string objects.
// Create an immutable set from the hash set.
// Assert the immutable set has size 8 and contains every element (by value).
// Assert get of a value not in the set is a void value.
// Assert an immutable set created from an empty hash set has size 0.
static void
immutableSetManyElementsTest
  (
    Arcadia_Thread* thread
  )
{
  static const char* values[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
  const Arcadia_SizeValue n = sizeof(values) / sizeof(values[0]);
  Arcadia_Set* set = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Set_add(thread, set, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, values[i])), NULL);
  }
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)set));

  Arcadia_ImmutableSet* immutableSet = Arcadia_ImmutableSet_create(thread, Arcadia_Value_makeObjectReferenceValue(set));
  Arcadia_Tests_assertTrue(thread, NULL != immutableSet);
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableSet));
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, values[i]));
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Set_contains(thread, (Arcadia_Set*)immutableSet, value));
  }
  Arcadia_Value missing = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"not-present"));
  Arcadia_Value found = Arcadia_Set_get(thread, (Arcadia_Set*)immutableSet, missing);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&found));

  Arcadia_Set* empty = (Arcadia_Set*)Arcadia_HashSet_create(thread);
  Arcadia_ImmutableSet* immutableEmpty = Arcadia_ImmutableSet_create(thread, Arcadia_Value_makeObjectReferenceValue(empty));
  Arcadia_Tests_assertTrue(thread, NULL != immutableEmpty);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableEmpty));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&immutableSetTest)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&immutableSetManyElementsTest)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
