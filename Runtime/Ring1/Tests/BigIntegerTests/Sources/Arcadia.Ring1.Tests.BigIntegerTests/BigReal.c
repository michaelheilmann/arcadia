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
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia.Ring1.Tests.BigIntegerTests/BigReal.h"

static Arcadia_BigReal*
parse
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_BigReal* result = Arcadia_BigReal_create(thread);
  Arcadia_BigReal_fromDecimalString(thread, result, bytes, numberOfBytes);
  return result;
}

static void
testCanonical
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* input,
    Arcadia_SizeValue inputLength,
    Arcadia_Natural8Value const* expected,
    Arcadia_SizeValue expectedLength
  )
{
  Arcadia_BigReal* value = parse(thread, input, inputLength);
  Arcadia_RuntimeUTF8String* received = Arcadia_BigReal_toScientificString(thread, value);
  Arcadia_Tests_assertEqualBytes(thread, expected, expectedLength,
                                received->bytes, received->numberOfBytes);
  Arcadia_BigReal* reparsed = parse(thread, received->bytes, received->numberOfBytes);
  Arcadia_Tests_assertTrue(thread, Arcadia_BigReal_isEqualTo(thread, value, reparsed));
}

static void
testInvalid
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_BigReal* value = parse(thread, u8"7.5", sizeof(u8"7.5") - 1);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_BigReal_fromDecimalString(thread, value, bytes, numberOfBytes);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
    // A failed reparse must leave the previously parsed value untouched.
    Arcadia_RuntimeUTF8String* retained = Arcadia_BigReal_toScientificString(thread, value);
    Arcadia_Tests_assertEqualBytes(thread, u8"75e-1", sizeof(u8"75e-1") - 1,
                                  retained->bytes, retained->numberOfBytes);
  }
}

static void
testComparison
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* aBytes,
    Arcadia_SizeValue aLength,
    Arcadia_Natural8Value const* bBytes,
    Arcadia_SizeValue bLength,
    Arcadia_Integer8Value expected
  )
{
  Arcadia_BigReal* a = parse(thread, aBytes, aLength);
  Arcadia_BigReal* b = parse(thread, bBytes, bLength);
  Arcadia_Tests_assertEqualInteger64(thread, expected, Arcadia_BigReal_compareTo(thread, a, b));
  Arcadia_Tests_assertEqualInteger64(thread, -expected, Arcadia_BigReal_compareTo(thread, b, a));
  Arcadia_Tests_assertTrue(thread, (expected < 0) == Arcadia_BigReal_isLowerThan(thread, a, b));
  Arcadia_Tests_assertTrue(thread, (expected <= 0) == Arcadia_BigReal_isLowerThanOrEqualTo(thread, a, b));
  Arcadia_Tests_assertTrue(thread, (expected > 0) == Arcadia_BigReal_isGreaterThan(thread, a, b));
  Arcadia_Tests_assertTrue(thread, (expected >= 0) == Arcadia_BigReal_isGreaterThanOrEqualTo(thread, a, b));
  Arcadia_Tests_assertTrue(thread, (expected == 0) == Arcadia_BigReal_isEqualTo(thread, a, b));
}

void
Arcadia_Ring1_Tests_BigInteger_bigRealOperations
  (
    Arcadia_Thread* thread
  )
{
#define Canonical(input, expected) \
  testCanonical(thread, u8##input, sizeof(u8##input) - 1, u8##expected, sizeof(u8##expected) - 1)
  Canonical("0", "0");
  Canonical("-0.000e999", "0");
  Canonical("+01.000", "1");
  Canonical("12.3400", "1234e-2");
  Canonical("-00123.4500e+3", "-12345e1");
  Canonical("1e-100", "1e-100");
  Canonical("1e100", "1e100");
  Canonical("9007199254740993", "9007199254740993");
  Canonical("1e123456789012345678901234567890", "1e123456789012345678901234567890");
#undef Canonical
#define Compare(a, b, expected) \
  testComparison(thread, u8##a, sizeof(u8##a) - 1, u8##b, sizeof(u8##b) - 1, expected)
  // All four DDLS numeric-bound predicates share the exact comparator.
  Compare("0", "-0.0", 0);
  Compare("+01", "1.0e0", 0);
  Compare("0", "0.0001", -1);
  Compare("-0.1", "-0", -1);
  Compare("-3", "10", -1);
  Compare("10.0001", "10", +1);
  Compare("5", "6", -1);
  Compare("9.999", "10", -1);
  Compare("1.0001", "1", +1);
  Compare("9007199254740992", "9007199254740993", -1);
  Compare("1e100", "9e99", +1);
  Compare("1e-100", "1e-99", -1);
  Compare("12e1", "119", +1);
  Compare("12e1", "121", -1);
  Compare("-12e1", "-119", -1);
  Compare("-12e1", "-121", +1);
  Compare("1e123456789012345678901234567890",
          "9e123456789012345678901234567889", +1);
  Compare("1e-123456789012345678901234567890",
          "1e-123456789012345678901234567889", -1);
  Compare("100e10", "1e12", 0);
  Compare("-0e-999999999999999999999999", "0e999999999999999999999999", 0);
#undef Compare

  Arcadia_BigReal* a = parse(thread, u8"9007199254740992", sizeof(u8"9007199254740992") - 1);
  Arcadia_BigReal* b = parse(thread, u8"9007199254740993", sizeof(u8"9007199254740993") - 1);
  Arcadia_Tests_assertFalse(thread, Arcadia_BigReal_isEqualTo(thread, a, b));

  Arcadia_BigReal* x = parse(thread, u8"1.25", sizeof(u8"1.25") - 1);
  Arcadia_BigReal* y = parse(thread, u8"-0.2", sizeof(u8"-0.2") - 1);
  Arcadia_BigReal_multiply3(thread, x, x, y); // Destination may alias an input.
  Arcadia_RuntimeUTF8String* product = Arcadia_BigReal_toScientificString(thread, x);
  Arcadia_Tests_assertEqualBytes(thread, u8"-25e-2", sizeof(u8"-25e-2") - 1,
                                product->bytes, product->numberOfBytes);

  Arcadia_BigReal* zero = parse(thread, u8"0e100", sizeof(u8"0e100") - 1);
  Arcadia_BigReal_multiply3(thread, y, y, zero);
  Arcadia_RuntimeUTF8String* zeroProduct = Arcadia_BigReal_toScientificString(thread, y);
  Arcadia_Tests_assertEqualBytes(thread, u8"0", sizeof(u8"0") - 1,
                                zeroProduct->bytes, zeroProduct->numberOfBytes);

#define Invalid(input) testInvalid(thread, u8##input, sizeof(u8##input) - 1)
  Invalid("");
  Invalid("+");
  Invalid(".");
  Invalid("e2");
  Invalid("1e");
  Invalid("1e+");
  Invalid("1.2.3");
  Invalid("1e2e3");
  Invalid("1a");
#undef Invalid
  testInvalid(thread, NULL, 0);
}
