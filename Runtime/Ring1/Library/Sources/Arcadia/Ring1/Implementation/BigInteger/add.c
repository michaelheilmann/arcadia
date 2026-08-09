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
#include "Arcadia/Ring1/Implementation/BigInteger/add.h"

#include "Arcadia/Ring1/Include.h"
#include <assert.h>

void
Arcadia_BigInteger_add3
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* result,
    Arcadia_BigInteger* a,
    Arcadia_BigInteger* b
  )
{
  // Special case: a is zero; result is b.
  if (Arcadia_BigInteger_isZero(thread, a)) {
    Arcadia_BigInteger_copy(thread, result, b);
    return;
  }
  // Special case: b is zero; result is a.
  if (Arcadia_BigInteger_isZero(thread, b)) {
    Arcadia_BigInteger_copy(thread, result, a);
    return;
  }
  // Both a and b are non-zero.
  assert(!Arcadia_BigInteger_isZero(thread, a));
  assert(!Arcadia_BigInteger_isZero(thread, b));

  if (a->sign == b->sign) {
    // a and b have the same sign and are non-zero.
    // Strategy: Add their magnitudes.

    // Determine which operand has the smaller length.
    Arcadia_BigInteger* large = a->numberOfLimps < b->numberOfLimps ? b : a;
    Arcadia_BigInteger* small = a->numberOfLimps < b->numberOfLimps ? a : b;

    Arcadia_SizeValue largeLength = large->numberOfLimps;
    Arcadia_SizeValue smallLength = small->numberOfLimps;

    // The output will be at least as long a the largest input.
    Arcadia_BigInteger* temporary = Arcadia_BigInteger_create(thread);
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&temporary->limps, sizeof(Arcadia_BigInteger_Limp) * largeLength);
    temporary->numberOfLimps = largeLength;

    Arcadia_BigInteger_DoubleLimp carry = 0;
    Arcadia_SizeValue index = 0;

    // Sum over the blocks that exists in both operands.
    while (index < smallLength) {
      Arcadia_BigInteger_DoubleLimp sum = carry + large->limps[index] + small->limps[index];
      carry = sum >> Arcadia_BigInteger_BitsPerLimp;
      temporary->limps[index] = (Arcadia_BigInteger_Limp)sum;
      index++;
    }
    // Sum over the limps that only exist in the large operands.
    while (index < largeLength) {
      Arcadia_BigInteger_DoubleLimp sum = carry + large->limps[index];
      carry = sum >> Arcadia_BigInteger_BitsPerLimp;
      temporary->limps[index] = (Arcadia_BigInteger_Limp)sum;
      index++;
    }

    if (carry) {
      // If there is stil a cary, we must append a limp.
      Arcadia_Memory_reallocateUnmanaged(thread, (void**)&temporary->limps, sizeof(Arcadia_BigInteger_Limp) * (temporary->numberOfLimps + 1));
      temporary->limps[index] = carry;
      temporary->numberOfLimps++;
    }
    // Also use the sign of the operands.
    temporary->sign = a->sign;

    Arcadia_BigInteger_copy(thread, result, temporary);
  } else {
    // a and b have different signs and are non-zero.
    // Strategy:
    // - Subtract the smaller magnitude from the larger magnitude.
    // - Use the sign of the larger magnitude.
    // - If the difference is 0, then set the sign to 0.
    Arcadia_BigInteger* large = a;
    Arcadia_BigInteger* small = b;
    Arcadia_Integer8Value comparedByMagnitude = Arcadia_BigInteger_compareByMagnitudeTo(thread, large, small);
    if (Arcadia_Integer8Value_Literal(0) == comparedByMagnitude) {
      Arcadia_BigInteger_setZero(thread, result);
      return;
    }
    if (comparedByMagnitude < Arcadia_Integer8Value_Literal(0)) {
      Arcadia_swapPointer(thread, (void**)&large, (void**)&small);
    }

    Arcadia_SizeValue largeLength = large->numberOfLimps;
    Arcadia_SizeValue smallLength = small->numberOfLimps;

    // The output will be at least as long a the largest input.
    Arcadia_BigInteger* temporary = Arcadia_BigInteger_create(thread);
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&temporary->limps, sizeof(Arcadia_BigInteger_Limp) * largeLength);
    temporary->numberOfLimps = largeLength;

    // Invariant magnitude(large) >= magnitude(small).
    Arcadia_BigInteger_DoubleLimp borrow = 0; // The borrow is either 1
    Arcadia_SizeValue index = 0;

    // Difference over the blocks that exists in both operands.
    while (index < smallLength) {
      Arcadia_BigInteger_DoubleLimp x = large->limps[index];
      Arcadia_BigInteger_DoubleLimp y = small->limps[index] + borrow; // This never overflows.
      borrow = (x < y);
      temporary->limps[index] = x < y ? (Arcadia_BigInteger_Limp_Maximum - y) + y : x - y; // In base ten, this would be x < y ? 10 + x - y : x - y.
      index++;
    }
    // Diference over the limps that only exist in the large operands and if borrow is non-zero.
    while (borrow && index < largeLength) {
      Arcadia_BigInteger_DoubleLimp x = large->limps[index];
      Arcadia_BigInteger_DoubleLimp y = borrow;
      borrow = (x < y);
      temporary->limps[index] = x < y ? (Arcadia_BigInteger_Limp_Maximum - y) + y : x - y; // In base ten, this would be x < y ? 10 + x - y : x - y.
      index++;
    }
    // Difference over the limps that only exist in the large operand and if borrow is 0.
    while (index < largeLength) {
      Arcadia_BigInteger_DoubleLimp x = large->limps[index];
      temporary->limps[index] = x;
      index++;
    }

    _Arcadia_BigInteger_stripLeadingZeroes(thread, &temporary->limps, &temporary->numberOfLimps);

    // Also use the sign of the larger operand.
    temporary->sign = temporary->numberOfLimps ? large->sign : UINT32_C(0);

    Arcadia_BigInteger_copy(thread, result, temporary);
  }
}

void
Arcadia_BigInteger_add
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{
  Arcadia_BigInteger_add3(thread, self, self, other);
}

void
Arcadia_BigInteger_addInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer16Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger16(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer32Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger32(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer64Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer8Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural16(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural32(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural64Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_addNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, otherBigInteger, other);
  Arcadia_BigInteger_add(thread, self, otherBigInteger);
}
