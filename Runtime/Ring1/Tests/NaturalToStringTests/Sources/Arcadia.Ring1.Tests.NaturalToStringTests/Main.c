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
testFixture
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value v
  )
{
  Arcadia_RuntimeUTF8String* u = Arcadia_RuntimeUTF8String_createFromNatural64(thread, v);
  Arcadia_Natural64Value w = Arcadia_RuntimeUTF8String_toNatural64(thread, u);
  Arcadia_Tests_assertTrue(thread, v == w);
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  testFixture(thread, Arcadia_Natural64Value_Minimum);
  testFixture(thread, Arcadia_Natural64Value_Maximum);
  testFixture(thread, Arcadia_Natural64Value_Literal(0));
  testFixture(thread, Arcadia_Natural64Value_Literal(1));
  testFixture(thread, Arcadia_Natural64Value_Literal(123));
}

static void
testNatural16Registration
  (
    Arcadia_Thread* thread
  )
{
  // Natural16 is a scalar, not an object with an empty operations table.
  // Value comparison triggers its real runtime registration in Debug builds.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_TypeValue type = _Arcadia_Natural16Value_getType(thread);
  Arcadia_Tests_assertTrue(thread, type != NULL);
  Arcadia_Natural16Value values[] = { 0, 1, UINT16_MAX };
  for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    Arcadia_Value value = Arcadia_Value_makeNatural16Value(values[i]);
    Arcadia_Value same = Arcadia_Value_makeNatural16Value(values[i]);
    Arcadia_Value widened = Arcadia_Value_makeNatural32Value(values[i]);
    Arcadia_Value different = Arcadia_Value_makeNatural32Value((Arcadia_Natural32Value)values[i] + 1);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &value, &same));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &value, &widened));
    Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isEqualTo(thread, &value, &different));
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isNotEqualTo(thread, &value, &different));
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, type == _Arcadia_Natural16Value_getType(thread));
}

static void
testNatural64ParsingBoundaries
  (
    Arcadia_Thread* thread
  )
{
  const struct {
    const char* text;
    Arcadia_Natural64Value value;
    Arcadia_Status status;
  } cases[] = {
    { "0", 0, Arcadia_Status_Success },
    { "+000000000000000000000000000000000000000", 0, Arcadia_Status_Success },
    { "999999999999999999", UINT64_C(999999999999999999), Arcadia_Status_Success },
    { "1000000000000000000", UINT64_C(1000000000000000000), Arcadia_Status_Success },
    { "9999999999999999999", UINT64_C(9999999999999999999), Arcadia_Status_Success },
    { "10000000000000000000", UINT64_C(10000000000000000000), Arcadia_Status_Success },
    { "18446744073709551615", UINT64_MAX, Arcadia_Status_Success },
    { "+00018446744073709551615", UINT64_MAX, Arcadia_Status_Success },
    { "18446744073709551616", 0, Arcadia_Status_ConversionFailed },
    { "99999999999999999999", 0, Arcadia_Status_ConversionFailed },
    { "100000000000000000000", 0, Arcadia_Status_ConversionFailed },
    { "-1", 0, Arcadia_Status_ConversionFailed },
  };
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_RuntimeUTF8String* text = Arcadia_RuntimeUTF8String_create(thread, cases[i].text, strlen(cases[i].text));
    Arcadia_ValueStack_pushRuntimeUTF8StringValue(thread, text);
    volatile Arcadia_Natural64Value result = 0;
    volatile Arcadia_BooleanValue jumped = false;
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) {
      result = Arcadia_RuntimeUTF8String_toNatural64(thread, text);
    } else {
      jumped = true;
    }
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, cases[i].status, status);
    Arcadia_Tests_assertTrue(thread, jumped == (cases[i].status != Arcadia_Status_Success));
    if (!jumped) Arcadia_Tests_assertEqualNatural64(thread, cases[i].value, result);
    Arcadia_Tests_assertEqualSize(thread, base + 1, Arcadia_ValueStack_getSize(thread));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static int
runNatural64ParsingBoundaries
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNatural64ParsingBoundaries) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNatural16Registration
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNatural16Registration) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNatural64RoundTrip
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Natural16Registration", &runNatural16Registration },
    { "Natural64RoundTrip", &runNatural64RoundTrip },
    { "Natural64ParsingBoundaries", &runNatural64ParsingBoundaries },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
