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

// Regression.
static void
test0
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, x, Arcadia_Natural8Value_Literal(0));
}

// Regression.
static void
test1
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
test2
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
test3
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
test3_1
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
test3_2
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
test4
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
test5
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
test6
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
test7
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
test8
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
test9
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
test10
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

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&checkTwosComplementIntegers)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test0)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test3)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test3_1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test3_2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test4)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test5)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test6)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test7)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test8)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test9)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test10)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_decimalStringOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_setIntegerOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_additiveOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_andOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_multiplicativeOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_orOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_powersOfOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_quotientRemainderOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_relationalOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_shiftOperations)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_BigInteger_twosComplementOperations)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
