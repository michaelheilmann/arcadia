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
testFixtureStringToInteger64
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* p,
    Arcadia_SizeValue n
  )
{
  if (n > Arcadia_SizeValue_Maximum - 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_ProcessStatus_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  char* q = malloc(n + 1);
  if (!q) {
    Arcadia_Thread_setStatus(thread, Arcadia_ProcessStatus_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  memcpy(q, p, n);
  q[n] = '\0';
  Arcadia_Integer64Value u;
  if (1 != sscanf(q, "%"SCNd64, &u)) {
    free(q);
    q = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_ProcessStatus_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Integer64Value v;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    v = Arcadia_toInteger64(thread, p, n);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(q);
    q = NULL;
    Arcadia_Thread_jump(thread);
  }
  free(q);
  q = NULL;
  Arcadia_Tests_assertTrue(thread, u == v);
}

static void
stringToInteger64
  (
    Arcadia_Thread* thread
  )
{
  testFixtureStringToInteger64(thread, u8"0", sizeof(u8"0") - 1);
  testFixtureStringToInteger64(thread, u8"00", sizeof(u8"00") - 1);

  testFixtureStringToInteger64(thread, u8"+0", sizeof(u8"+0") - 1);
  testFixtureStringToInteger64(thread, u8"+00", sizeof(u8"+00") - 1);

  testFixtureStringToInteger64(thread, u8"-0", sizeof(u8"-0") - 1);
  testFixtureStringToInteger64(thread, u8"-00", sizeof(u8"-00") - 1);

  testFixtureStringToInteger64(thread, u8"+00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
                                sizeof(u8"+00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001") - 1);
  testFixtureStringToInteger64(thread, u8"-00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
                                sizeof(u8"-00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001") - 1);
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&stringToInteger64)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


static void
expectConversionFailure
  (
    Arcadia_Thread* thread,
    char const* text,
    Arcadia_SizeValue width
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    const Arcadia_Natural8Value* bytes = (const Arcadia_Natural8Value*)text;
    Arcadia_SizeValue size = strlen(text);
    switch (width) {
      case 8: Arcadia_toInteger8(thread, bytes, size); break;
      case 16: Arcadia_toInteger16(thread, bytes, size); break;
      case 32: Arcadia_toInteger32(thread, bytes, size); break;
      case 64: Arcadia_toInteger64(thread, bytes, size); break;
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_ConversionFailed);
    Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  }
}

static void
testGrammarAndBounds
  (
    Arcadia_Thread* thread
  )
{
  static char const* const invalid[] = {
    "", "+", "-", ".", "+.", "1.", ".5", "1.5", "-1.0",
    "1e2", "1E-2", "1^2", "1.0e2", "1e", "1x", " 1", "1 ",
    "9223372036854775808", "-9223372036854775809",
  };
  static const Arcadia_SizeValue widths[] = { 8, 16, 32, 64 };
  for (Arcadia_SizeValue i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    for (Arcadia_SizeValue j = 0; j < sizeof(widths) / sizeof(widths[0]); ++j) {
      expectConversionFailure(thread, invalid[i], widths[j]);
    }
  }
  expectConversionFailure(thread, "128", 8);
  expectConversionFailure(thread, "-129", 8);
  expectConversionFailure(thread, "32768", 16);
  expectConversionFailure(thread, "-32769", 16);
  expectConversionFailure(thread, "2147483648", 32);
  expectConversionFailure(thread, "-2147483649", 32);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger8(thread, u8"-128", 4) == Arcadia_Integer8Value_Minimum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger8(thread, u8"127", 3) == Arcadia_Integer8Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger16(thread, u8"-32768", 6) == Arcadia_Integer16Value_Minimum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger16(thread, u8"32767", 5) == Arcadia_Integer16Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger32(thread, u8"-2147483648", 11) == Arcadia_Integer32Value_Minimum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger32(thread, u8"2147483647", 10) == Arcadia_Integer32Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger64(thread, u8"-9223372036854775808", 20) == Arcadia_Integer64Value_Minimum);
  Arcadia_Tests_assertTrue(thread, Arcadia_toInteger64(thread, u8"9223372036854775807", 19) == Arcadia_Integer64Value_Maximum);
}

static int
runGrammarAndBounds
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testGrammarAndBounds) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "StringToIntegerTests", &runTests },
    { "GrammarAndBounds", &runGrammarAndBounds },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
