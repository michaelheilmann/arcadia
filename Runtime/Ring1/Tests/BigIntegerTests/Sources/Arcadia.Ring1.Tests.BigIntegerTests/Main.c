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

#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/DecimalString.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/Additive.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/And.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/Multiplicative.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/Or.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/PowersOf.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/QuotientRemainder.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/Relational.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/SetInteger.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/Shift.h"
#include "Arcadia.Ring1.Tests.BigIntegerTests/TwosComplement.h"

static void
expectScalarConversionFailure
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* value,
    Arcadia_BooleanValue signedResult
  )
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    if (signedResult) Arcadia_BigInteger_toInteger64(thread, value);
    else Arcadia_BigInteger_toNatural64(thread, value);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ConversionFailed, status);
  }
}

static void
testScalarConversionSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* value = Arcadia_BigInteger_create(thread);
  Arcadia_BooleanValue truncated = true;
  // Canonical zero has no readable limbs, including immediately after create.
  Arcadia_Tests_assertEqualSize(thread, 0, value->numberOfLimps);
  Arcadia_Tests_assertEqualNatural64(thread, 0, Arcadia_BigInteger_toNatural64(thread, value));
  Arcadia_Tests_assertEqualInteger64(thread, 0, Arcadia_BigInteger_toInteger64(thread, value));
  Arcadia_Tests_assertEqualNatural64(thread, 0, Arcadia_BigInteger_toNatural64WithTruncation(thread, value, &truncated));
  Arcadia_Tests_assertFalse(thread, truncated);
  Arcadia_BigInteger_setNatural64(thread, value, UINT64_MAX);
  Arcadia_Tests_assertEqualNatural64(thread, UINT64_MAX, Arcadia_BigInteger_toNatural64(thread, value));
  expectScalarConversionFailure(thread, value, true);
  Arcadia_BigInteger_setInteger64(thread, value, INT64_MIN);
  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isNegative(thread, value));
  Arcadia_Tests_assertEqualSize(thread, 2, value->numberOfLimps);
  Arcadia_Tests_assertEqualInteger64(thread, INT64_MIN, Arcadia_BigInteger_toInteger64(thread, value));
  expectScalarConversionFailure(thread, value, false);
  Arcadia_BigInteger_setInteger64(thread, value, INT64_MAX);
  Arcadia_Tests_assertEqualInteger64(thread, INT64_MAX, Arcadia_BigInteger_toInteger64(thread, value));
  Arcadia_BigInteger_setPowerOfTwoNatural8(thread, value, 64);
  expectScalarConversionFailure(thread, value, true);
  expectScalarConversionFailure(thread, value, false);
  Arcadia_Tests_assertEqualNatural64(thread, UINT64_C(4294967296), Arcadia_BigInteger_toNatural64WithTruncation(thread, value, &truncated));
  Arcadia_Tests_assertTrue(thread, truncated);
  Arcadia_BigInteger_setZero(thread, value);
  Arcadia_Tests_assertEqualNatural64(thread, 0, Arcadia_BigInteger_toNatural64(thread, value));
  Arcadia_Tests_assertEqualInteger64(thread, 0, Arcadia_BigInteger_toInteger64(thread, value));
}

static int
runScalarConversionSafety
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testScalarConversionSafety) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// Regression.
static void
testZeroInitialization
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, x, Arcadia_Natural8Value_Literal(0));
}

// Regression.
static void
testDecimalToInteger64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Integer64Value u = Arcadia_Integer64Value_Literal(9007199254740992);
  Arcadia_Tests_assertTrue(thread, u <= Arcadia_Natural64Value_Maximum);
  Arcadia_Tests_assertTrue(thread, u <= Arcadia_Integer64Value_Maximum);
  Arcadia_BigInteger* bigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_fromDecimalString(thread, bigInteger, u8"9007199254740992", sizeof(u8"9007199254740992") - 1);
  Arcadia_Integer64Value v = Arcadia_BigInteger_toInteger64(thread, bigInteger);
  Arcadia_Tests_assertTrue(thread, u == v);
}

// Regression.
static void
testDecimalToNatural64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural64Value u = Arcadia_Natural64Value_Literal(9007199254740992);
  Arcadia_Tests_assertTrue(thread, u <= Arcadia_Natural64Value_Maximum);
  Arcadia_Tests_assertTrue(thread, u <= Arcadia_Integer64Value_Maximum);
  Arcadia_BigInteger* bigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_fromDecimalString(thread, bigInteger, u8"9007199254740992", sizeof(u8"9007199254740992") - 1);
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, bigInteger);
  Arcadia_Tests_assertTrue(thread, u == v);
}

// Regression.
static void
testDecimalToNatural64WithTruncation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural64Value u = Arcadia_Natural64Value_Literal(9007199254740992);
  Arcadia_Tests_assertTrue(thread, u <= Arcadia_Natural64Value_Maximum);
  Arcadia_Tests_assertTrue(thread, u <= Arcadia_Integer64Value_Maximum);
  Arcadia_BigInteger* bigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_fromDecimalString(thread, bigInteger, u8"9007199254740992", sizeof(u8"9007199254740992") - 1);
  Arcadia_BooleanValue w;
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64WithTruncation(thread, bigInteger, &w);
  Arcadia_Tests_assertTrue(thread, !w && u == v);
}

// Regression.
static void
testMultiplyInteger8
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* a = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, a, 3);
  Arcadia_BigInteger_multiplyInteger8(thread, a, 10);
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, a);
  Arcadia_Tests_assertTrue(thread, v == 30);
}

// Regression.
static void
testAddInteger8
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* a = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, a, 3);
  Arcadia_BigInteger_addInteger8(thread, a, 27);
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, a);
  Arcadia_Tests_assertTrue(thread, v == 30);
}

// Regression.
static void
testAddThenMultiply
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* a = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_addInteger8(thread, a, 3);
  Arcadia_BigInteger_multiplyInteger8(thread, a, 10);
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, a);
  Arcadia_Tests_assertTrue(thread, v == 30);
}

// Regression.
static void
testDecimalAccumulation
  (
    Arcadia_Thread* thread
  )
{
  char const* p = "9007199254740992";
  Arcadia_SizeValue n = strlen(p);
  Arcadia_BigInteger* p10 = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, p10, 10);
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);

  Arcadia_Natural64Value u1 = Arcadia_RuntimeUTF8String_toNatural64(thread, Arcadia_RuntimeUTF8String_create(thread, p, n));
  Arcadia_Natural64Value u2 = 0;
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Natural8Value digit = ((Arcadia_Natural64Value)p[i]) - '0';
    u2 *= 10;
    u2 += digit;
    Arcadia_BigInteger_multiply(thread, v, p10);
    Arcadia_BigInteger_addNatural64(thread, v, digit);
  }
  Arcadia_Tests_assertTrue(thread, u1 == u2);
  Arcadia_Tests_assertTrue(thread, u1 == Arcadia_BigInteger_toNatural64(thread, v));
  Arcadia_BigInteger_toStdoutDebug(thread, v);
}

// Regression.
static void
testLargePowerOfTen
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, v, 1);
  Arcadia_BigInteger* p10 = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setPowerOfTenNatural16(thread, p10, 309);
  Arcadia_BigInteger_multiply(thread, v, p10);
  Arcadia_BigInteger_toStdoutDebug(thread, v);
}

// Regression. Natural8.Maximum == 1 << Natural8.NumberOfbits - 1.
// @todo: Add this to the subtraction test suite.
static void
testNatural8Maximum
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, v, Arcadia_Natural8Value_Maximum);
  Arcadia_BigInteger* w = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, w, Arcadia_Natural64Value_Literal(1));
  Arcadia_BigInteger_shiftLeftNatural8(thread, w, Arcadia_Natural8Value_NumberOfBits);
  Arcadia_BigInteger_subtractInteger8(thread, w, Arcadia_Integer8Value_Literal(1));
  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, v, w));
}

// Regression. Natural16.Maximum == 1 << Natural16.NumberOfbits - 1.
// @todo: Add this to the subtraction test suite.
static void
testNatural16Maximum
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural16(thread, v, Arcadia_Natural16Value_Maximum);
  Arcadia_BigInteger* w = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, w, Arcadia_Natural64Value_Literal(1));
  Arcadia_BigInteger_shiftLeftNatural8(thread, w, Arcadia_Natural16Value_NumberOfBits);
  Arcadia_BigInteger_subtractInteger8(thread, w, Arcadia_Integer8Value_Literal(1));
  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, v, w));
}

// Regression. Natural32.Maximum == 1 << Natural32.NumberOfbits - 1.
// @todo: Add this to the subtraction test suite.
static void
testNatural32Maximum
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural32(thread, v, Arcadia_Natural32Value_Maximum);
  Arcadia_BigInteger* w = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, w, Arcadia_Natural64Value_Literal(1));
  Arcadia_BigInteger_shiftLeftNatural8(thread, w, Arcadia_Natural32Value_NumberOfBits);
  Arcadia_BigInteger_subtractInteger8(thread, w, Arcadia_Integer8Value_Literal(1));
  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, v, w));
}

// Regression. Natural64.Maximum == 1 << Natural64.NumberOBits - 1.
// @todo: Add this to the subtraction test suite.
static void
testNatural64Maximum
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, v, Arcadia_Natural64Value_Maximum);
  Arcadia_BigInteger* w = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, w, Arcadia_Natural64Value_Literal(1));
  Arcadia_BigInteger_shiftLeftNatural8(thread, w, Arcadia_Natural64Value_NumberOfBits);
  Arcadia_BigInteger_subtractInteger8(thread, w, Arcadia_Integer8Value_Literal(1));
  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, v, w));
}

// TODO: Subsume this under an a tool called environment compatibility tester.
static void
checkTwosComplementIntegers
  (
    Arcadia_Thread* thread
  )
{
  {
    // Every implicit conversion as if by assignment is allowed. (https://en.cppreference.com/w/c/language/cast.html)
    // If the target type can represent the value, the value is unchanged. (https://en.cppreference.com/w/c/language/conversion.html)
    int8_t x;
    uint8_t z;
    x = 0;
    z = (uint8_t)x;
    Arcadia_Tests_assertTrue(thread, z == 0);
  }
  {
    // Every implicit conversion as if by assignment is allowed. (https://en.cppreference.com/w/c/language/cast.html)
    // If the target type can represent the value, the value is unchanged. (https://en.cppreference.com/w/c/language/conversion.html)
    int8_t x;
    uint8_t z;
    x = INT8_MAX;
    z = (uint8_t)x;
    Arcadia_Tests_assertTrue(thread, z == 127);
  }
  {
    // Every implicit conversion as if by assignment is allowed.* (https://en.cppreference.com/w/c/language/cast.html)
    // If the target type is unsigned, the value 2^b, where b is the number of value bits in the target type, is
    // repeatedly subtracted or added to the source value until the result fits in the target type.
    // (https://en.cppreference.com/w/c/language/conversion.html)
    int8_t x;
    uint8_t z;
    x = INT8_MIN;
    z = (uint8_t)x;
    Arcadia_Tests_assertTrue(thread, z == (x + UINT8_MAX + 1));
  }
  {
    // Every implicit conversion as if by assignment is allowed.* (https://en.cppreference.com/w/c/language/cast.html)
    // If the target type is unsigned, the value 2^b, where b is the number of value bits in the target type, is
    // repeatedly subtracted or added to the source value until the result fits in the target type.
    // (https://en.cppreference.com/w/c/language/conversion.html)
    int8_t x;
    uint8_t z;
    x = INT8_MIN + 3;
    z = (uint8_t)x;
    Arcadia_Tests_assertTrue(thread, z == (x + UINT8_MAX + 1));
  }
}

static int
runNativeTwosComplement
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&checkTwosComplementIntegers) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runZeroInitialization
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testZeroInitialization) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDecimalToInteger64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDecimalToInteger64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDecimalToNatural64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDecimalToNatural64) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDecimalToNatural64WithTruncation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDecimalToNatural64WithTruncation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runMultiplyInteger8
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMultiplyInteger8) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAddInteger8
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAddInteger8) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAddThenMultiply
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAddThenMultiply) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDecimalAccumulation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDecimalAccumulation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runLargePowerOfTen
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testLargePowerOfTen) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNatural8Maximum
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNatural8Maximum) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNatural16Maximum
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNatural16Maximum) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNatural32Maximum
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNatural32Maximum) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNatural64Maximum
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNatural64Maximum) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runDecimalStringOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_decimalStringOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSetIntegerOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_setIntegerOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAdditiveOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_additiveOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAndOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_andOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runMultiplicativeOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_multiplicativeOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runOrOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_orOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runPowersOfOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_powersOfOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runQuotientRemainderOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_quotientRemainderOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runRelationalOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_relationalOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runShiftOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_shiftOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runTwosComplementOperations
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_twosComplementOperations) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "NativeTwosComplement", &runNativeTwosComplement },
    { "ZeroInitialization", &runZeroInitialization },
    { "ScalarConversionSafety", &runScalarConversionSafety },
    { "DecimalToInteger64", &runDecimalToInteger64 },
    { "DecimalToNatural64", &runDecimalToNatural64 },
    { "DecimalToNatural64WithTruncation", &runDecimalToNatural64WithTruncation },
    { "MultiplyInteger8", &runMultiplyInteger8 },
    { "AddInteger8", &runAddInteger8 },
    { "AddThenMultiply", &runAddThenMultiply },
    { "DecimalAccumulation", &runDecimalAccumulation },
    { "LargePowerOfTen", &runLargePowerOfTen },
    { "Natural8Maximum", &runNatural8Maximum },
    { "Natural16Maximum", &runNatural16Maximum },
    { "Natural32Maximum", &runNatural32Maximum },
    { "Natural64Maximum", &runNatural64Maximum },
    { "DecimalStringOperations", &runDecimalStringOperations },
    { "SetIntegerOperations", &runSetIntegerOperations },
    { "AdditiveOperations", &runAdditiveOperations },
    { "AndOperations", &runAndOperations },
    { "MultiplicativeOperations", &runMultiplicativeOperations },
    { "OrOperations", &runOrOperations },
    { "PowersOfOperations", &runPowersOfOperations },
    { "QuotientRemainderOperations", &runQuotientRemainderOperations },
    { "RelationalOperations", &runRelationalOperations },
    { "ShiftOperations", &runShiftOperations },
    { "TwosComplementOperations", &runTwosComplementOperations },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
