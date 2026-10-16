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
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"

static void
testFixture32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value v
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromReal32(thread, v);
  Arcadia_Real32Value w = Arcadia_RuntimeUTF8String_toReal32(thread, u);
  Arcadia_Tests_assertTrue(thread, v == w);
}

static void
testFixture64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value v
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromReal64(thread, v);
  Arcadia_Real64Value w = Arcadia_RuntimeUTF8String_toReal64(thread, u);
  Arcadia_Tests_assertTrue(thread, v == w);
}

// Assert that the decimal representation of v is exactly the bytes in expected.
// This is used for the values whose representation is fixed (signed zeroes,
// infinities, not a number); a round-trip comparison v == w cannot detect a
// lost sign bit for zero, since +0. == -0. compares equal.
static void
testText32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value v,
    Arcadia_Natural8Value const* expected,
    Arcadia_SizeValue expectedNumberOfBytes
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromReal32(thread, v);
  Arcadia_Tests_assertTrue(thread, Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, u) == expectedNumberOfBytes);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Memory_compare(thread, Arcadia_RuntimeUTF8String_getBytes(thread, u), expected, expectedNumberOfBytes));
}

static void
testText64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value v,
    Arcadia_Natural8Value const* expected,
    Arcadia_SizeValue expectedNumberOfBytes
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromReal64(thread, v);
  Arcadia_Tests_assertTrue(thread, Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, u) == expectedNumberOfBytes);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Memory_compare(thread, Arcadia_RuntimeUTF8String_getBytes(thread, u), expected, expectedNumberOfBytes));
}

static void
testReal32
  (
    Arcadia_Thread* thread
  )
{
  testFixture32(thread, Arcadia_Real32Value_Minimum);
  testFixture32(thread, Arcadia_Real32Value_Maximum);
  testFixture32(thread, Arcadia_Real32Value_Literal(0.));
  testText32(thread, Arcadia_Real32Value_Literal(0.), u8"+0", sizeof(u8"+0") - 1);
  testText32(thread, Arcadia_Real32Value_Literal(-0.), u8"-0", sizeof(u8"-0") - 1);
  testText32(thread, Arcadia_Real32Value_PositiveInfinity, u8"<positive infinity>", sizeof(u8"<positive infinity>") - 1);
  testText32(thread, Arcadia_Real32Value_NegativeInfinity, u8"<negative infinity>", sizeof(u8"<negative infinity>") - 1);
  testText32(thread, NAN, u8"<not a number>", sizeof(u8"<not a number>") - 1);
}

static void
testReal64
  (
    Arcadia_Thread* thread
  )
{
  testFixture64(thread, Arcadia_Real64Value_Minimum);
  testFixture64(thread, Arcadia_Real64Value_Maximum);
  testFixture64(thread, Arcadia_Real64Value_Literal(0.));
  testText64(thread, Arcadia_Real64Value_Literal(0.), u8"+0", sizeof(u8"+0") - 1);
  testText64(thread, Arcadia_Real64Value_Literal(-0.), u8"-0", sizeof(u8"-0") - 1);
  testText64(thread, Arcadia_Real64Value_PositiveInfinity, u8"<positive infinity>", sizeof(u8"<positive infinity>") - 1);
  testText64(thread, Arcadia_Real64Value_NegativeInfinity, u8"<negative infinity>", sizeof(u8"<negative infinity>") - 1);
  testText64(thread, NAN, u8"<not a number>", sizeof(u8"<not a number>") - 1);
}

static int
runReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testReal32) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testReal64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Real32", &runReal32 },
    { "Real64", &runReal64 },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
