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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/BigInteger/fromDecimalString.h"

#include "Arcadia/Ring1/Include.h"

static Arcadia_Natural8Value
codePointToDigit
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value codePoint
  )
{
  if ('0' <= codePoint && codePoint <= '9') {
    codePoint -= '0';
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural8Value)codePoint;
}

void
Arcadia_BigInteger_fromDecimalString
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (bytes == NULL || numberOfBytes == 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue negative = Arcadia_BooleanValue_False;
  Arcadia_Natural8Value const* start = bytes;
  Arcadia_Natural8Value const* end = bytes + numberOfBytes;
  Arcadia_Natural8Value const* current = start;
  if (*current == '+') {
    ++current;
  } else if (*current == '-') {
    ++current;
    negative = Arcadia_BooleanValue_True;
  }
  if (current == end) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BigInteger* powerOfTen = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* result = Arcadia_BigInteger_create(thread);
  // Execute the 64 bit fast path for as long as possible.
  while ((end - current) >= Arcadia_Natural64Value_MaximumNumberOfDecimalDigits) {
    Arcadia_Natural64Value value = 0;
    for (Arcadia_SizeValue i = 0; i < Arcadia_Natural64Value_MaximumNumberOfDecimalDigits; ++i) {
      Arcadia_Natural8Value j = codePointToDigit(thread, *current);
      j -= '0';
      value = value * 10 +j;
      current++;
    }
    // Given a decimal string ('+'|'-')? u v w and assuming we have ready u already
    // and stored its value in result. Now we read v into value. We have to multiply
    // result by 10^|v| and add v to result.
    Arcadia_BigInteger_setPowerOfTenNatural32(thread, powerOfTen, Arcadia_Natural64Value_MaximumNumberOfDecimalDigits);
    Arcadia_BigInteger_multiply(thread, result, powerOfTen);
    Arcadia_BigInteger_addNatural64(thread, result, value);
  }
  // Fast path for 32 bit can only be executed once as otherwise the 64 bit fast path would have been executed.
  if ((end - current) >= Arcadia_Natural32Value_MaximumNumberOfDecimalDigits) {
    Arcadia_Natural32Value value = 0;
    for (Arcadia_SizeValue i = 0; i < Arcadia_Natural32Value_MaximumNumberOfDecimalDigits; ++i) {
      Arcadia_Natural8Value j = codePointToDigit(thread, *current);
      value = value * 10 + j;
      current++;
    }
    // Given a decimal string ('+'|'-')? u v w and assuming we have ready u already
    // and stored its value in result. Now we read v into value. We have to multiply
    // result by 10^|v| and add v to result.
    Arcadia_BigInteger_setPowerOfTenNatural32(thread, powerOfTen, Arcadia_Natural32Value_MaximumNumberOfDecimalDigits);
    Arcadia_BigInteger_multiply(thread, result, powerOfTen);
    Arcadia_BigInteger_addNatural32(thread, result, value);
  }
  // Fast path for 16 bit can only be executed once.
  if ((end - current) >= Arcadia_Natural16Value_MaximumNumberOfDecimalDigits) {
    Arcadia_Natural16Value value = 0;
    for (Arcadia_SizeValue i = 0; i < Arcadia_Natural16Value_MaximumNumberOfDecimalDigits; ++i) {
      Arcadia_Natural8Value j = codePointToDigit(thread, *current);
      value = value * 10 + j;
      current++;
    }
    // Given a decimal string ('+'|'-')? u v w and assuming we have ready u already
    // and stored its value in result. Now we read v into value. We have to multiply
    // result by 10^|v| and add v to result.
    Arcadia_BigInteger_setPowerOfTenNatural32(thread, powerOfTen, Arcadia_Natural16Value_MaximumNumberOfDecimalDigits);
    Arcadia_BigInteger_multiply(thread, result, powerOfTen);
    Arcadia_BigInteger_addNatural16(thread, result, value);
  }
  // Fast path for 8 bit can only be executed once.
  if ((end - current) >= Arcadia_Natural8Value_MaximumNumberOfDecimalDigits) {
    Arcadia_Natural8Value value = 0;
    for (Arcadia_SizeValue i = 0; i < Arcadia_Natural8Value_MaximumNumberOfDecimalDigits; ++i) {
      Arcadia_Natural8Value j = codePointToDigit(thread, *current);
      value = value * 10 + j;
      current++;
    }
    // Given a decimal string ('+'|'-')? u v w and assuming we have ready u already
    // and stored its value in result. Now we read v into value. We have to multiply
    // result by 10^|v| and add v to result.
    Arcadia_BigInteger_setPowerOfTenNatural32(thread, powerOfTen, Arcadia_Natural8Value_MaximumNumberOfDecimalDigits);
    Arcadia_BigInteger_multiply(thread, result, powerOfTen);
    Arcadia_BigInteger_addNatural8(thread, result, value);
  }
  // The last remaining digits (cannot be more than 7).
  if ((end - current)) {
    Arcadia_Natural8Value value = 0;
    Arcadia_SizeValue numberOfDigits = end - current;
    for (Arcadia_SizeValue i = 0; i < numberOfDigits; ++i) {
      Arcadia_Natural8Value j = codePointToDigit(thread, *current);
      value = value * 10 + j;
      current++;
    }
    // Given a decimal string ('+'|'-')? u v w and assuming we have ready u already
    // and stored its value in result. Now we read v into value. We have to multiply
    // result by 10^|v| and add v to result.
    Arcadia_BigInteger_setPowerOfTenNatural32(thread, powerOfTen, numberOfDigits);
    Arcadia_BigInteger_multiply(thread, result, powerOfTen);
    Arcadia_BigInteger_addNatural8(thread, result, value);
  }
  if (negative) {
    Arcadia_BigInteger_multiplyInteger8(thread, result, Arcadia_Integer8Value_Literal(-1));
  }
  Arcadia_BigInteger_copy(thread, self, result);
}
