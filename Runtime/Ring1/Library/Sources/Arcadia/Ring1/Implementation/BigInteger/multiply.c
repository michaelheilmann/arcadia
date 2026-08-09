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
#include "Arcadia/Ring1/Implementation/BigInteger/multiply.h"

#include "Arcadia/Ring1/Include.h"
#include <assert.h>

// This basically asserts that the digit multiplication is sound.
#define PRODUCT_MAXIMUM ((uint64_t)UINT32_MAX * (uint64_t)UINT32_MAX)
#define CARRY_MAXIMUM (PRODUCT_MAXIMUM >> 32)
Arcadia_StaticAssert(PRODUCT_MAXIMUM + CARRY_MAXIMUM + (uint64_t)UINT32_MAX <= UINT64_MAX, "");
#undef CARRY_MAXIMUM
#undef PRODUCT_MAXIMUM

void
Arcadia_BigInteger_multiply3
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* result,
    Arcadia_BigInteger* a,
    Arcadia_BigInteger* b
  )
{
  // Special case: If a or b are zero, then the result is zero.
  if (Arcadia_BigInteger_isZero(thread, a) || Arcadia_BigInteger_isZero(thread, b)) {
    Arcadia_BigInteger_setZero(thread, result);
    return;
  }
  // Both a and b are non-zero.
  assert(!Arcadia_BigInteger_isZero(thread, a));
  assert(!Arcadia_BigInteger_isZero(thread, b));

  // Determine which operand has the smaller length.
  Arcadia_BigInteger* large = a->numberOfLimps < b->numberOfLimps ? b : a;
  Arcadia_BigInteger* small = a->numberOfLimps < b->numberOfLimps ? a : b;

  Arcadia_SizeValue largeLength = large->numberOfLimps;
  Arcadia_SizeValue smallLength = small->numberOfLimps;
  Arcadia_SizeValue temporaryLength = largeLength + smallLength; // TODO: Use safeadd

  // We cannot perform the computation on result's storage as result == a or result == b might hold.
  Arcadia_BigInteger* oldResult = result;
  result = Arcadia_BigInteger_create(thread);
  result->sign = a->sign != b->sign ? -1 : +1;
  Arcadia_Memory_reallocateUnmanaged(thread, (void**)&result->limps, sizeof(Arcadia_BigInteger_Limp) * temporaryLength);
  Arcadia_Memory_fill(thread, result->limps, sizeof(Arcadia_BigInteger_Limp) * temporaryLength, 0);
  result->numberOfLimps = temporaryLength;

  Arcadia_SizeValue smallIndex = 0, resultStartIndex = 0;

  // Sum over the blocks that exists in both operands.
  while (smallIndex < smallLength) {
    if (small->limps[smallIndex]) {
      uint64_t carry = 0;
      Arcadia_SizeValue largeIndex = 0;
      Arcadia_SizeValue resultIndex = resultStartIndex;
      do {
        uint64_t product = ((uint64_t)result->limps[resultIndex]) + carry + ((uint64_t)large->limps[largeIndex]) * ((uint64_t)small->limps[smallIndex]);
        carry = product >> 32;
        result->limps[resultIndex] = (uint32_t)product;
        resultIndex++;
        largeIndex++;
      } while(largeIndex < largeLength);
      result->limps[resultIndex] = (uint32_t)carry;
    }
    smallIndex++;
    resultStartIndex++;
  }
  while (result->numberOfLimps > 1 && result->limps[result->numberOfLimps - 1] == 0) {
    result->numberOfLimps--;
  }
  Arcadia_BigInteger_swap(thread, result, oldResult);
}

void
Arcadia_BigInteger_multiply
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{
  Arcadia_BigInteger_multiply3(thread, self, self, other);
}

void
Arcadia_BigInteger_multiplyInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer16Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger16(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer32Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger32(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer64Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer8Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural16(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural32(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural64Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_multiplyNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, otherBigInteger, other);
  Arcadia_BigInteger_multiply(thread, self, otherBigInteger);
}
