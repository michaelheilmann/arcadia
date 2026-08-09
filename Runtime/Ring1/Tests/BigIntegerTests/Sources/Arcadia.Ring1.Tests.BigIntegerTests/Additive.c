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

#include "Arcadia.Ring1.Tests.BigIntegerTests/Additive.h"

static inline Arcadia_Value
Arcadia_Value_fromBigIntegerValue
  (
    Arcadia_BigIntegerValue v
  )
{
  Arcadia_Value w = Arcadia_Value_Initializer();
  Arcadia_Value_setBigIntegerValue(&w, v);
  return w;
}

#define Test_Op_add (7)
#define Test_Op_subtract (8)

static inline void
Arcadia_Test_BigInteger_assertAdditive
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value expectedResult,
    Arcadia_Natural8Value operation,
    Arcadia_Integer64Value leftOperand,
    Arcadia_Integer64Value rightOperand
  )
{
  Arcadia_BigInteger* leftOperandBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, leftOperandBigInteger, leftOperand);
  Arcadia_Value leftOperandValue = Arcadia_Value_fromBigIntegerValue(leftOperandBigInteger);

  Arcadia_BigInteger* rightOperandBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, rightOperandBigInteger, rightOperand);
  Arcadia_Value rightOperandValue = Arcadia_Value_fromBigIntegerValue(rightOperandBigInteger);

  Arcadia_BigInteger* expectedResultBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, expectedResultBigInteger, expectedResult);
  Arcadia_Value expectedResultValue = Arcadia_Value_fromBigIntegerValue(expectedResultBigInteger);

  switch (operation) {
    case Test_Op_add: {
      Arcadia_BigInteger* receivedResultBigInteger = Arcadia_BigInteger_create(thread);
      Arcadia_BigInteger_add3(thread, receivedResultBigInteger, leftOperandBigInteger, rightOperandBigInteger);
      Arcadia_Tests_assertTrue(thread, Arcadia_Integer8Value_Literal(0) == Arcadia_BigInteger_compareTo(thread, receivedResultBigInteger, expectedResultBigInteger));
    } break;
    case Test_Op_subtract: {
      Arcadia_BigInteger* receivedResultBigInteger = Arcadia_BigInteger_create(thread);
      Arcadia_BigInteger_subtract3(thread, receivedResultBigInteger, leftOperandBigInteger, rightOperandBigInteger);
      Arcadia_Tests_assertTrue(thread, Arcadia_Integer8Value_Literal(0) == Arcadia_BigInteger_compareTo(thread, receivedResultBigInteger, expectedResultBigInteger));
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
};

void
Arcadia_Ring1_Tests_BigInteger_additiveOperations
  (
    Arcadia_Thread* thread
  )
{
  typedef struct Test {
    Arcadia_Integer32Value result;
    int operation;
    Arcadia_Integer32Value a, b;
  } Test;
  static const Test tests[] = {
    { 0, Test_Op_add, 0, 0 },
    { 0, Test_Op_subtract, 0, 0 },

    { 12, Test_Op_add, 5, 7 }, // also tests 7 + 5
    { -2, Test_Op_subtract, 5, 7 },

    { 12, Test_Op_add, 5, 7 }, // also tests 7 + 5
    { -2, Test_Op_subtract, 5, 7 },
  };
  static const Arcadia_SizeValue numberOfTests = sizeof(tests) / sizeof(Test);
  Arcadia_Test_BigInteger_assertAdditive(thread, -2, Test_Op_subtract, 5, 7);
  for (Arcadia_SizeValue i = 0, n = numberOfTests; i < n; ++i) {
    const Test* test = &(tests[i]);
    Arcadia_Test_BigInteger_assertAdditive(thread, test->result, test->operation, test->a, test->b);
  }
}
