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
#include "Arcadia/Ring1/Implementation/BigInteger/divide.h"

#include "Arcadia/Ring1/Include.h"
#include <assert.h>

static Arcadia_Natural32Value
addDivisor
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* lhs,
    Arcadia_SizeValue lhsStartIndex,
    Arcadia_BigInteger* rhs)
{
#if !defined(NDEBUG)
  Arcadia_SizeValue lhsLength = lhs->numberOfLimps;
#endif
  Arcadia_SizeValue rhsLength = rhs->numberOfLimps;

  assert(lhsLength >= 0);
  assert(rhsLength >= 0);
  assert(lhsLength >= rhsLength);

  // Repairs the dividend, if the last subtract was too much
  Arcadia_Natural64Value carry = Arcadia_Natural64Value_Literal(0);

  for (Arcadia_SizeValue i = 0; i < rhsLength; i++) {
    Arcadia_Natural32Value* lhsValue = &(lhs->limps[lhsStartIndex + i]);

    Arcadia_Natural64Value digit = *lhsValue + carry + rhs->limps[i];
    *lhsValue = (Arcadia_Natural32Value)digit;
    carry = digit >> 32;
  }

  return (Arcadia_Natural32Value)(carry);
}

// subtract rhs q times from lhs[:lhsStartIndex]
static Arcadia_Natural32Value
subtractDivisor
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* lhs,
    Arcadia_SizeValue lhsStartIndex,
    Arcadia_BigInteger* rhs,
    Arcadia_Natural64Value q
  )
{
#if !defined(NDEBUG)
  Arcadia_SizeValue lhsLength = (Arcadia_Integer64Value)lhs->numberOfLimps - lhsStartIndex;
#endif
  Arcadia_SizeValue rhsLength = rhs->numberOfLimps;

  assert(q <= Arcadia_Natural32Value_Maximum);
  assert(lhsLength >= rhsLength);

  Arcadia_Natural64Value carry = 0;
  for (Arcadia_SizeValue i = 0; i < rhsLength; ++i) {
    carry += rhs->limps[i] * q;
    // digit are the lower bits of carry
    Arcadia_Natural32Value digit = (Arcadia_Natural32Value)carry;
    carry >>= 32;

    if (lhs->limps[lhsStartIndex + i] < digit) {
      carry++;
    }
    lhs->limps[lhsStartIndex + i] = lhs->limps[lhsStartIndex + i] - digit;
  }
  return (Arcadia_Natural32Value)carry;
}

static Arcadia_BooleanValue
divideGuessTooBig
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value q,
    Arcadia_Natural64Value valHi,
    Arcadia_Natural32Value valLo,
    Arcadia_Natural32Value divHi,
    Arcadia_Natural32Value divLo
  )
{
  assert(q <= 0xFFFFFFFF);

  // We multiply the two most significant limbs (divHi and divLo) of the divisor with the current guess for the quotient (q).
  // If those are bigger than the three most significant limbs of the current dividend (valHi and valLo) we return true, which means the current guess is still too big.

  Arcadia_Natural64Value chkHi = divHi * q;
  Arcadia_Natural64Value chkLo = divLo * q;

  chkHi += (chkLo >> 32);
  chkLo &= Arcadia_Natural32Value_Maximum;

  if (chkHi < valHi)
    return false;

  if (chkHi > valHi)
    return true;

  if (chkLo < valLo)
    return false;

  if (chkLo > valLo)
    return true;

  return false;
}

// The division with remainder of two numbers dividend and divisor is defined as
// that there exist two uniquely identified numbers a and b such that dividend =
// a * divisor + b with 0 <= b < |divisor| holds.
void
Arcadia_BigInteger_divide3
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* a,
    Arcadia_BigInteger* b,
    Arcadia_BigInteger* quotient,
    Arcadia_BigInteger* remainder
  )
{
  // Special case: a is zero; result is zero.
  if (Arcadia_BigInteger_isZero(thread, a)) {
    if (quotient) Arcadia_BigInteger_setZero(thread, quotient);
    if (remainder) Arcadia_BigInteger_setZero(thread, remainder);
    return;
  }
  // Special case: b is zero; raise a "division by zero" error.
  if (Arcadia_BigInteger_isZero(thread, b)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_DivisionByZero);
    Arcadia_Thread_jump(thread);
  }
  // Both a and b are non-zero.
  assert(!Arcadia_BigInteger_isZero(thread, a));
  assert(!Arcadia_BigInteger_isZero(thread, b));
  // This will be zero under integer division.
  if (a->numberOfLimps < b->numberOfLimps) {
    if (quotient) {
      Arcadia_BigInteger_setZero(thread, quotient);
    }
    if (remainder) {
      Arcadia_BigInteger_copy(thread, remainder, a);
    }
    return;
  }
  // a and b are of length 1.
  if (a->numberOfLimps == 1 && b->numberOfLimps == 1) {
#if Arcadia_Configuration_BigInteger_LimpSize == 4
    Arcadia_Natural32Value quotientNatural, remainderNatural;
    Arcadia_quotientRemainderNatural32(thread, a->limps[0], b->limps[0], &quotientNatural, &remainderNatural);
    if (quotient) {
      Arcadia_BigInteger_setNatural32(thread, quotient, quotientNatural);
    }
    if (remainder) {
      Arcadia_BigInteger_setNatural32(thread, remainder, remainderNatural);
    }
#elif Arcadia_Configuration_BigInteger_LimpSize == 2
    Arcadia_Natural16Value quotientNatural, remainderNatural;
    Arcadia_quotientRemainderNatural16(thread, a->limps[0], b->limps[0], &quotientNatural, &remainderNatural);
    if (quotient) {
      Arcadia_BigInteger_setNatural16(thread, quotient, quotientNatural);
    }
    if (remainder) {
      Arcadia_BigInteger_setNatural16(thread, remainder, remainderNatural);
    }
#elif Arcadia_Configuration_BigIneger_KimpSize == 1
    Arcadia_Natural8Value quotientNatural, remainderNatural;
    Arcadia_quotientRemainderNatural8(thread, a->limps[0], b->limps[0], &quotientNatural, &remainderNatural);
    if (quotient) {
      Arcadia_BigInteger_setNatural8(thread, quotient, quotientNatural);
    }
    if (remainder) {
      Arcadia_BigInteger_setNatural8(thread, remainder, remainderNatural);
    }
#else
  #error("unknown/unsupported limp size")
#endif
    return;
  }
  Arcadia_BigInteger *quotient1 = NULL,
                     *remainder1 = NULL;
  quotient1 = Arcadia_BigInteger_create(thread);
  remainder1 = Arcadia_BigInteger_create(thread);
  // b is of length 1.
  if (b->numberOfLimps == 1) {
    Arcadia_SizeValue quotientLength = a->numberOfLimps;
    quotient1->sign = a->sign != b->sign ? -1 : +1;
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&quotient1->limps, sizeof(Arcadia_BigInteger_Limp) * quotientLength);
    Arcadia_Memory_fill(thread, quotient1->limps, sizeof(Arcadia_BigInteger_Limp) * quotientLength, 0);
    quotient1->numberOfLimps = quotientLength;

    Arcadia_Natural64Value bValue = b->limps[0];
    Arcadia_Natural64Value carry = 0;

    for (Arcadia_SizeValue i = quotientLength; i > 0; --i) {
      Arcadia_Natural64Value aValue = (carry << 32) | a->limps[i - 1];
      Arcadia_Natural64Value digit;
      Arcadia_quotientRemainderNatural64(thread, aValue, bValue, &digit, &carry);
      if (digit == 0 && i - 1 == (quotientLength - 1)) {
        quotientLength--;
      } else {
        quotient1->limps[i - 1] = (Arcadia_Natural32Value)digit;
      }
    }

    Arcadia_BigInteger_setNatural32(thread, remainder1, carry);
  } else {
    Arcadia_BigInteger_copy(thread, remainder1, a);
    Arcadia_SizeValue remainderLength = a->numberOfLimps;
    Arcadia_SizeValue quotientLength = (a->numberOfLimps - b->numberOfLimps) + 1; // a->numberOfLimps >= b->numberOfLimps by the above.
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&quotient1->limps, sizeof(Arcadia_BigInteger_Limp) * quotientLength);
    Arcadia_Memory_fill(thread, quotient1->limps, sizeof(Arcadia_BigInteger_Limp) * quotientLength, 0);
    quotient1->sign = 0;

    // We accumulate as many bits from b into (divHi, divLo).
    Arcadia_BigInteger_Limp divHi = b->limps[b->numberOfLimps - 1];
    Arcadia_BigInteger_Limp divLo = b->limps[b->numberOfLimps - 2];
  #if Arcadia_Configuration_BigInteger_LimpSize == 4
    Arcadia_SizeValue shiftLeft = Arcadia_countLeadingZeroesInteger32Value(thread, divHi);
    Arcadia_SizeValue shiftRight = 32 - shiftLeft;
  #elif Arcadia_Configuration_BigInteger_LimpSize == 2
    Arcadia_SizeValue shiftLeft = Arcadia_countLeadingZeroesInteger16Value(thread, divHi);
    Arcadia_SizeValue shiftRight = 16 - shiftLeft;
  #elif Arcadia_Configuration_BigInteger_LimpSize == 1
    Arcadia_SizeValue shiftLeft = Arcadia_countLeadingZeroesInteger8Value(thread, divHi);
    Arcadia_SizeValue shiftRight = 8 - shiftLeft;
  #else
    #error("unknown/unsupported limp size")
  #endif
    if (shiftLeft > 0) {
      divHi = (divHi << shiftLeft) | (divLo >> shiftRight);
      divLo <<= shiftLeft;
      if (b->numberOfLimps > 2) {
        divLo |= (b->limps[b->numberOfLimps - 3] >> shiftRight);
      }
    }

    // Then we divide the bits of a by these accumulated bits.
    for (Arcadia_Integer64Value i = a->numberOfLimps; i >= b->numberOfLimps; --i) {
      Arcadia_SizeValue n = i - b->numberOfLimps;
      Arcadia_Natural32Value t = i < a->numberOfLimps ? remainder1->limps[i] : 0;
      Arcadia_Natural64Value valHi = (((Arcadia_Natural64Value)t) << 32) | remainder1->limps[i - 1];
      Arcadia_Natural32Value valLo = i > 1 ? remainder1->limps[i - 2] : 0;

      if (shiftLeft > 0) {
        valHi = (valHi << shiftLeft) | (valLo >> shiftRight);
        valLo <<= shiftLeft;

        if (i > 2) {
          valLo |= (remainder1->limps[i - 3] >> shiftRight);
        }
      }

      Arcadia_Natural64Value digit = valHi / divHi;
      if (digit > Arcadia_Natural32Value_Maximum) {
        digit = Arcadia_Natural32Value_Maximum;
      }
      while (divideGuessTooBig(thread, digit, valHi, valLo, divHi, divLo)) {
        digit--;
      }
      if (digit > 0) {
        Arcadia_Natural32Value carry = subtractDivisor(thread, remainder1, n, b, digit);
        if (carry != t) {
          assert(carry == t + 1);
          // Guess was exactly one too high.
          carry = addDivisor(thread, remainder1, n, b);
          digit--;
          assert(carry == 1);
        }
      }
      // We have the digit.
      if (quotientLength != 0) {
        if ((digit == 0) && (n == (quotientLength - 1))) {
          /*quotientLength--;*/
        } else {
          quotient1->limps[n] = (Arcadia_Natural32Value)digit;
        }
      }
      if (i < remainderLength) {
        remainderLength--;
      }
    }
    quotient1->sign = a->sign != b->sign ? -1 : +1;
    quotient1->numberOfLimps = quotientLength;
    remainder1->numberOfLimps = remainderLength;
  }
  // Check leading zero limps in the quotient.
  for (Arcadia_SizeValue i = quotient1->numberOfLimps; i > 1; --i) {
    if (quotient1->limps[i - 1] == 0) {
      quotient1->numberOfLimps--;
    } else {
      // As soon as we find a non-zero limp, stop.
      break;
    }
  }
  if (quotient1->numberOfLimps == 1 && quotient1->limps[0] == 0) {
    quotient1->sign = 0;
  }
  // Check leading zero limps in the remainder.
  for (Arcadia_SizeValue i = remainder1->numberOfLimps; i > 1; --i) {
    if (remainder1->limps[i - 1] == 0) {
      remainder1->numberOfLimps--;
    } else {
      // As soon as we find a non-zero limp, stop.
      break;
    }
  }
  if (remainder1->numberOfLimps == 1 && remainder1->limps[0] == 0) {
    remainder1->sign = 0;
  }

  if (quotient) Arcadia_BigInteger_copy(thread, quotient, quotient1);
  if (remainder) Arcadia_BigInteger_copy(thread, remainder, remainder1);
}

void
Arcadia_BigInteger_divide
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{
  Arcadia_BigInteger_divide3(thread, self, other, self, NULL);
}

void
Arcadia_BigInteger_divideInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer16Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger16(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer32Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger32(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer64Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer8Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger8(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural16(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural32(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural64Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}

void
Arcadia_BigInteger_divideNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{
  Arcadia_BigInteger* otherBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, otherBigInteger, other);
  Arcadia_BigInteger_divide(thread, self, otherBigInteger);
}
