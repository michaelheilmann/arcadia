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

#include "Arcadia.Ring1.Tests.BigIntegerTests/Relational.h"

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

#define Test_Op_isEqualTo (1)
#define Test_Op_isGreaterThan (5)
#define Test_Op_isGreaterThanOrEqualTo (6)
#define Test_Op_isLowerThan (3)
#define Test_Op_isLowerThanOrEqualTo (4)
#define Test_Op_isNotEqualTo (2)

static inline void
Arcadia_Test_BigInteger_assertRelational
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue expectedResult,
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

  switch (operation) {
    case Test_Op_isEqualTo: {
      Arcadia_Tests_assertTrue(thread, expectedResult == Arcadia_Value_isEqualTo(thread, &leftOperandValue, &rightOperandValue));
      Arcadia_Tests_assertTrue(thread, expectedResult == !Arcadia_Value_isNotEqualTo(thread, &leftOperandValue, &rightOperandValue));
    } break;
    case Test_Op_isGreaterThan: {
      Arcadia_BooleanValue receivedResult = Arcadia_Value_isGreaterThan(thread, &leftOperandValue, &rightOperandValue);
      Arcadia_Tests_assertTrue(thread, expectedResult == receivedResult);
      if (expectedResult) {
        // "isGreaterThan(x,y)" true implies "isLowerThanOrEqualTo(x,y)" and "isLowerThan(x,y)" false.
        Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isLowerThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
        Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isLowerThan(thread, &leftOperandValue, &rightOperandValue));
      } else {
        // "isGreaterThan(x,y)" false implies "isLowerThanOrEqualTo(x,y)" true.
        Arcadia_Tests_assertTrue(thread, Arcadia_Value_isLowerThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
      }
    } break;
    case Test_Op_isGreaterThanOrEqualTo: {
      Arcadia_Tests_assertTrue(thread, expectedResult == Arcadia_Value_isGreaterThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
      if (expectedResult) {
        // "isGreaterThanOrEqualTo(x,y)" true implies "isLowerThan(x,y)" false.
        Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isLowerThan(thread, &leftOperandValue, &rightOperandValue));
      }
    } break;
    case Test_Op_isLowerThan: {
      Arcadia_BooleanValue receivedResult = Arcadia_Value_isLowerThan(thread, &leftOperandValue, &rightOperandValue);
      Arcadia_Tests_assertTrue(thread, expectedResult == receivedResult);
      if (expectedResult) {
        // "isLowerThan(x,y)" true implies "isGreaterThanOrEqualTo(x,y)" and "isGreaterThan(x,y)" false.
        Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isGreaterThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
        Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isGreaterThan(thread, &leftOperandValue, &rightOperandValue));
      } else {
        // "isLowerThan(x,y)" false implies "isGreaterThanOrEqualTo(x,y)" true.
        Arcadia_Tests_assertTrue(thread, Arcadia_Value_isGreaterThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
      }
    } break;
    case Test_Op_isLowerThanOrEqualTo: {
      Arcadia_Tests_assertTrue(thread, expectedResult == Arcadia_Value_isLowerThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
      if (expectedResult) {
        // "isLowerThanOrEqualTo(x,y)" true implies "isGreaterThan(x,y)" false.
        Arcadia_Tests_assertTrue(thread, !Arcadia_Value_isGreaterThan(thread, &leftOperandValue, &rightOperandValue));
      }
    } break;
    case Test_Op_isNotEqualTo: {
      Arcadia_Tests_assertTrue(thread, expectedResult == Arcadia_Value_isLowerThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
      Arcadia_Tests_assertTrue(thread, expectedResult == !Arcadia_Value_isLowerThanOrEqualTo(thread, &leftOperandValue, &rightOperandValue));
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
};

void
Arcadia_Ring1_Tests_BigInteger_relationalOperations
  (
    Arcadia_Thread* thread
  )
{
  typedef struct Test {
    Arcadia_BooleanValue result;
    int operation;
    Arcadia_Integer32Value a;
    Arcadia_Integer32Value b;
  } Test;
  static const Test tests[] = {
    /* two positive numbers */
    { .result = Arcadia_BooleanValue_True, Test_Op_isEqualTo, 2, 2 }, // 0

    { .result = Arcadia_BooleanValue_True, Test_Op_isGreaterThan, 2, 1 },
    { .result = Arcadia_BooleanValue_False, Test_Op_isGreaterThan, 2, 2 },

    { .result = Arcadia_BooleanValue_True, Test_Op_isGreaterThanOrEqualTo, 2, 1 },
    { .result = Arcadia_BooleanValue_True, Test_Op_isGreaterThanOrEqualTo, 2, 2 },

    { .result = Arcadia_BooleanValue_True, Test_Op_isLowerThan, 1, 2 },
    { .result = Arcadia_BooleanValue_False, Test_Op_isLowerThan, 2, 2 },

    { .result = Arcadia_BooleanValue_True, Test_Op_isLowerThanOrEqualTo, 1, 2 },
    { .result = Arcadia_BooleanValue_True, Test_Op_isLowerThanOrEqualTo, 2, 2 },

    /* two negative numbers */
    { .result = Arcadia_BooleanValue_True, Test_Op_isEqualTo, -2, -2 }, // 9

    { .result = Arcadia_BooleanValue_True, Test_Op_isGreaterThan, -1, -2 },
    { .result = Arcadia_BooleanValue_False, Test_Op_isGreaterThan, -2, -2 },

    { .result = Arcadia_BooleanValue_True, Test_Op_isGreaterThanOrEqualTo, -1, -2 },
    { .result = Arcadia_BooleanValue_True, Test_Op_isGreaterThanOrEqualTo, -2, -2 },

    { .result = Arcadia_BooleanValue_True, Test_Op_isLowerThan, -2, -1 },
    { .result = Arcadia_BooleanValue_False, Test_Op_isLowerThan, -2, -2 },

    { .result = Arcadia_BooleanValue_True, Test_Op_isLowerThanOrEqualTo, -2, -1 },
    { .result = Arcadia_BooleanValue_True, Test_Op_isLowerThanOrEqualTo, -2, -2 },
  };
  static const Arcadia_SizeValue numberOfTests = sizeof(tests) / sizeof(Test);
  for (Arcadia_SizeValue i = 0, n = numberOfTests; i < n; ++i) {
    const Test* test = &(tests[i]);
    Arcadia_Test_BigInteger_assertRelational(thread, test->result, test->operation, test->a, test->b);
  }
}
