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

#include "Arcadia.Ring1.Tests.BigIntegerTests/PowersOf.h"

static inline void
testRegression20250619
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, x, 0);
    Arcadia_Tests_assertTrue(thread, 1 == Arcadia_BigInteger_toInteger32(thread, x));
  }
  {
    Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, x, 1);
    Arcadia_Tests_assertTrue(thread, 5 == Arcadia_BigInteger_toInteger32(thread, x));
  }
  {
    Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, x, 2);
    Arcadia_Tests_assertTrue(thread, 25 == Arcadia_BigInteger_toInteger32(thread, x));
  }
  {
    Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, x, 3);
    Arcadia_Tests_assertTrue(thread, 125 == Arcadia_BigInteger_toInteger32(thread, x));
  }
  {
    Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, x, 4);
    Arcadia_Tests_assertTrue(thread, 625 == Arcadia_BigInteger_toInteger32(thread, x));
  }
}

// Test x^2 * x^5 = x^10
static inline void
testProductOfPowers
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value power
  )
{
  Arcadia_BigInteger* leftOperandBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setPowerOfFiveNatural64(thread, leftOperandBigInteger, power);
  Arcadia_Value leftOperandValue = Arcadia_Value_makeBigIntegerValue(leftOperandBigInteger);

  Arcadia_BigInteger* rightOperandBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setPowerOfTwoNatural64(thread, rightOperandBigInteger, power);
  Arcadia_Value rightOperandValue = Arcadia_Value_makeBigIntegerValue(rightOperandBigInteger);

  Arcadia_BigInteger* expectedResultBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setPowerOfTenNatural64(thread, expectedResultBigInteger, power);
  Arcadia_Value expectedResultValue = Arcadia_Value_makeBigIntegerValue(expectedResultBigInteger);

  Arcadia_BigInteger* receivedResultBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_multiply3(thread, receivedResultBigInteger, leftOperandBigInteger, rightOperandBigInteger);
  Arcadia_Tests_assertTrue(thread, Arcadia_Integer8Value_Literal(0) == Arcadia_BigInteger_compareTo(thread, receivedResultBigInteger, expectedResultBigInteger));
};

void
Arcadia_Ring1_Tests_BigInteger_powersOfOperations
  (
    Arcadia_Thread* thread
  )
{
  {
    typedef struct Test {
      Arcadia_Natural64Value a;
    } Test;
    static const Test tests[] = {
      { 0 },
      { 1 },
      { 7 },
      { 24 },
      { 40 },
      { 1024 },
      { 5012 },
      { 5012*8 },
    };
    static const Arcadia_SizeValue numberOfTests = sizeof(tests) / sizeof(Test);
    for (Arcadia_SizeValue i = 0, n = numberOfTests; i < n; ++i) {
      const Test* test = &(tests[i]);
      testProductOfPowers(thread, test->a);
    }
  }
  testRegression20250619(thread);
}
