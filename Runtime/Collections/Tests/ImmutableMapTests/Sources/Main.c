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

// Create a hash map with one entry.
// Create an immutable map from the hash map.
// Assert the immutable map has size 1, is immutable, and get returns a non-void value.
// Assert getKeys and getValues each return a list of size 1.
// Assert mutating operations raise an "Operation Invalid" error.
static void
immutableMapTest
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Map* map = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Value key = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"siteAddress")),
                value = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"https://michaelheilmann.com"));
  Arcadia_Map_set(thread, map, key, value, NULL, NULL);

  Arcadia_ImmutableMap* immutableMap = Arcadia_ImmutableMap_create(thread, Arcadia_Value_makeObjectReferenceValue(map));
  Arcadia_Tests_assertTrue(thread, NULL != immutableMap);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(1) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableMap));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Collection_isImmutable(thread, (Arcadia_Collection*)immutableMap));
  Arcadia_Value value2 = Arcadia_Map_get(thread, (Arcadia_Map*)immutableMap, key);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isVoidValue(&value2));

  Arcadia_List* keys = Arcadia_Map_getKeys(thread, (Arcadia_Map*)immutableMap);
  Arcadia_List* values = Arcadia_Map_getValues(thread, (Arcadia_Map*)immutableMap);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(1) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)keys));
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(1) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)values));

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Map_set(thread, (Arcadia_Map*)immutableMap, key, value, NULL, NULL);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Status_OperationInvalid == Arcadia_Thread_getStatus(thread));
  Arcadia_Thread_setStatus(thread, 0);
}

// Create a hash map with eight entries.
// Create an immutable map from the hash map.
// Assert the immutable map has size 8, is immutable, and every key retrieves a non-void value.
// Assert get of a key not in the map is a void value.
// Assert getKeys and getValues each return a list of size 8.
// Assert an immutable map created from an empty hash map has size 0.
static void
immutableMapManyEntriesTest
  (
    Arcadia_Thread* thread
  )
{
  static const char* keys[]   = { "a", "b", "c", "d", "e", "f", "g", "h" };
  static const char* values[] = { "0", "1", "2", "3", "4", "5", "6", "7" };
  const Arcadia_SizeValue n = sizeof(keys) / sizeof(keys[0]);
  Arcadia_Map* map = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Map_set(thread, map,
                    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, keys[i])),
                    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, values[i])),
                    NULL, NULL);
  }
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)map));

  Arcadia_ImmutableMap* immutableMap = Arcadia_ImmutableMap_create(thread, Arcadia_Value_makeObjectReferenceValue(map));
  Arcadia_Tests_assertTrue(thread, NULL != immutableMap);
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableMap));
  Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_True == Arcadia_Collection_isImmutable(thread, (Arcadia_Collection*)immutableMap));
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Value key = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, keys[i]));
    Arcadia_Value value = Arcadia_Map_get(thread, (Arcadia_Map*)immutableMap, key);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isVoidValue(&value));
  }
  Arcadia_Value missing = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"not-present"));
  Arcadia_Value found = Arcadia_Map_get(thread, (Arcadia_Map*)immutableMap, missing);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&found));

  Arcadia_List* mapKeys = Arcadia_Map_getKeys(thread, (Arcadia_Map*)immutableMap);
  Arcadia_List* mapValues = Arcadia_Map_getValues(thread, (Arcadia_Map*)immutableMap);
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapKeys));
  Arcadia_Tests_assertTrue(thread, n == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mapValues));

  Arcadia_Map* empty = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_ImmutableMap* immutableEmpty = Arcadia_ImmutableMap_create(thread, Arcadia_Value_makeObjectReferenceValue(empty));
  Arcadia_Tests_assertTrue(thread, NULL != immutableEmpty);
  Arcadia_Tests_assertTrue(thread, Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)immutableEmpty));
}

static int
runImmutability
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&immutableMapTest) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runManyEntries
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&immutableMapManyEntriesTest) ? EXIT_SUCCESS : EXIT_FAILURE;
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
    { "ManyEntries", &runManyEntries },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
