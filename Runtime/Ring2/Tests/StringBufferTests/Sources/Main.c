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

#include "Arcadia/Ring2/Include.h"
#include <stdlib.h>
#include <string.h>

static void
testRemoveCodePointsFront
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* a = Arcadia_StringBuilder_create(thread),
                      * b = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"");
  Arcadia_StringBuilder_removeCodePointsFront(thread, a, 0);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"1");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"1");
  Arcadia_StringBuilder_removeCodePointsFront(thread, a, 0);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"1");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"");
  Arcadia_StringBuilder_removeCodePointsFront(thread, a, 1);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"12");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"2");
  Arcadia_StringBuilder_removeCodePointsFront(thread, a, 1);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"123");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"23");
  Arcadia_StringBuilder_removeCodePointsFront(thread, a, 1);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"12345");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"45");
  Arcadia_StringBuilder_removeCodePointsFront(thread, a, 3);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));
}

static void
testRemoveCodePointsBack
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* a = Arcadia_StringBuilder_create(thread),
                      * b = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"");
  Arcadia_StringBuilder_removeCodePointsBack(thread, a, 0);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"1");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"1");
  Arcadia_StringBuilder_removeCodePointsBack(thread, a, 0);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"1");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"");
  Arcadia_StringBuilder_removeCodePointsBack(thread, a, 1);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"12");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"1");
  Arcadia_StringBuilder_removeCodePointsBack(thread, a, 1);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"123");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"12");
  Arcadia_StringBuilder_removeCodePointsBack(thread, a, 1);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));

  Arcadia_StringBuilder_clear(thread, a);
  Arcadia_StringBuilder_insertBackCxxString(thread, a, u8"12345");
  Arcadia_StringBuilder_clear(thread, b);
  Arcadia_StringBuilder_insertBackCxxString(thread, b, u8"12");
  Arcadia_StringBuilder_removeCodePointsBack(thread, a, 3);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_StringBuilder_compareTo(thread, a, Arcadia_Value_makeObjectReferenceValue(b)));
}

static int
runRemoveCodePointsBack
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRemoveCodePointsBack) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRemoveCodePointsFront
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testRemoveCodePointsFront) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "RemoveCodePointsBack", &runRemoveCodePointsBack },
    { "RemoveCodePointsFront", &runRemoveCodePointsFront },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
