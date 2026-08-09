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

#include "Arcadia.Ring1.Tests.BigIntegerTests/DecimalString.h"

// string v to big integer to string w and assert v and w are equal
static void
testRoundTrip
  (
    Arcadia_Thread* thread
  )
{
  static const Arcadia_Natural8Value* bytes = u8"1982";
  static const Arcadia_SizeValue numberOfBytes = sizeof(u8"1982") - 1;
  Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_fromDecimalString(thread, x, bytes, numberOfBytes);
  Arcadia_RuntimeUTF8String* s = Arcadia_BigInteger_toDecimalString(thread, x);
  Arcadia_Tests_assertTrue(thread, numberOfBytes == s->numberOfBytes);
  Arcadia_Tests_assertTrue(thread, Arcadia_Integer32Value_Literal(0) == Arcadia_Memory_compare(thread, bytes, s->bytes, numberOfBytes));
}

static void
testFixture
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value naturalValue,
    Arcadia_RuntimeUTF8String* expectedValueString
  )
{
  Arcadia_BigInteger* v = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural32(thread, v, naturalValue);
  Arcadia_RuntimeUTF8String* receivedValueString = Arcadia_BigInteger_toDecimalString(thread, v);
  Arcadia_Value x = Arcadia_Value_makeRuntimeUTF8StringValue(receivedValueString);
  Arcadia_Value y = Arcadia_Value_makeRuntimeUTF8StringValue(expectedValueString);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEqualTo(thread, &x, &y));
}

void
Arcadia_Ring1_Tests_BigInteger_decimalStringOperations
  (
    Arcadia_Thread* thread
  )
{
  testFixture(thread, 0, Arcadia_RuntimeUTF8String_create(thread, u8"0", sizeof(u8"0") - 1));
  testFixture(thread, 1, Arcadia_RuntimeUTF8String_create(thread, u8"1", sizeof(u8"1") - 1));
  testFixture(thread, 12, Arcadia_RuntimeUTF8String_create(thread, u8"12", sizeof(u8"12") - 1));
  testFixture(thread, 123, Arcadia_RuntimeUTF8String_create(thread, u8"123", sizeof(u8"123") - 1));
  testRoundTrip(thread);
}
