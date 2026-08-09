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

#include "Arcadia.Ring1.Tests.SubStringTests/findLastOccurrenceTests.h"

static void
assertEqual
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* a,
    Arcadia_RuntimeUTF8String* b
  )
{
  Arcadia_Value a1 = Arcadia_Value_makeRuntimeUTF8StringValue(a),
                b1 = Arcadia_Value_makeRuntimeUTF8StringValue(b);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &a1, &b1));
}

static void
fixture
  (
    Arcadia_Thread* thread,
    const char* p,
    size_t n,
    size_t start, 
    Arcadia_Value length,
    const char* q,
    size_t m,
    Arcadia_BooleanValue error
  )
{
  Arcadia_RuntimeUTF8String* string = Arcadia_RuntimeUTF8String_create(thread, p, n);
  if (!error) {
    Arcadia_RuntimeUTF8String* receivedSubString = Arcadia_RuntimeUTF8String_substring(thread, string, start, length);
    Arcadia_RuntimeUTF8String* expectedSubString = Arcadia_RuntimeUTF8String_create(thread, q, m);
    assertEqual(thread, receivedSubString, expectedSubString);
  } else {
    Arcadia_JumpTarget jt;
    Arcadia_Thread_pushJumpTarget(thread, &jt);
    if (Arcadia_JumpTarget_save(&jt)) {
      Arcadia_RuntimeUTF8String_substring(thread, string, start, length);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

void
subStringTests1
  (
    Arcadia_Thread* thread
  )
{
  {
    fixture(thread, u8"a", sizeof(u8"a") - 1, 1, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"", sizeof(u8"") - 1, 0, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"a", sizeof(u8"a") - 1, 0, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"a", sizeof(u8"a") - 1, 0, Arcadia_Value_makeSizeValue(1), u8"a", sizeof(u8"a") - 1, Arcadia_BooleanValue_False);
  }
  {
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 0, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 1, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 2, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_False);
  }
  {
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 0, Arcadia_Value_makeSizeValue(1), u8"a", sizeof(u8"a") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 1, Arcadia_Value_makeSizeValue(1), u8"b", sizeof(u8"b") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 0, Arcadia_Value_makeSizeValue(2), u8"ab", sizeof(u8"ab") - 1, Arcadia_BooleanValue_False);
  }
}

void
subStringTests2
  (
    Arcadia_Thread* thread
  )
{
  {
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 0, Arcadia_Value_makeSizeValue(1), u8"a", sizeof(u8"a") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 1, Arcadia_Value_makeSizeValue(1), u8"b", sizeof(u8"b") - 1, Arcadia_BooleanValue_False);
    fixture(thread, u8"ab", sizeof(u8"ab") - 1, 0, Arcadia_Value_makeSizeValue(2), u8"ab", sizeof(u8"ab") - 1, Arcadia_BooleanValue_False);
  }
}

void
subStringTests3
  (
    Arcadia_Thread* thread
  )
{
  {
    fixture(thread, u8"", sizeof(u8"") - 1, 1, Arcadia_Value_makeSizeValue(0), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_True);
    fixture(thread, u8"", sizeof(u8"") - 1, 1, Arcadia_Value_makeSizeValue(1), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_True);
    fixture(thread, u8"a", sizeof(u8"a") - 1, 0, Arcadia_Value_makeSizeValue(2), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_True);
    fixture(thread, u8"a", sizeof(u8"a") - 1, 1, Arcadia_Value_makeSizeValue(1), u8"", sizeof(u8"") - 1, Arcadia_BooleanValue_True);
  }
}
