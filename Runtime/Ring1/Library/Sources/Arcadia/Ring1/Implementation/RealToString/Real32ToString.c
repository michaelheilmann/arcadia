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
#include "Arcadia/Ring1/Implementation/RealToString/Real32ToString.h"

#include "Arcadia/Ring1/Implementation/RealToString/Common.i"
#include "Arcadia/Ring1/Implementation/Real32_getBits.h"

#include "Arcadia/Ring1/Implementation/RealToString/TablesReal32.i"

// A floating decimal representing m * 10^e.
typedef struct FloatingDecimal32 {
  bool sign; // true ~ +, false ~ -
  uint32_t mantissa;
  // Decimal exponent's range is -45 to 38 inclusive, and can fit in a short if needed.
  int32_t exponent;
} FloatingDecimal32;

static inline uint32_t decimalLength9(const uint32_t v) {
  // Function precondition: v is not a 10-digit number.
  // (f2s: 9 digits are sufficient for round-tripping.)
  // (d2fixed: We print 9-digit blocks.)
  assert(v < 1000000000);
  if (v >= 100000000) { return 9; }
  if (v >= 10000000) { return 8; }
  if (v >= 1000000) { return 7; }
  if (v >= 100000) { return 6; }
  if (v >= 10000) { return 5; }
  if (v >= 1000) { return 4; }
  if (v >= 100) { return 3; }
  if (v >= 10) { return 2; }
  return 1;
}

// It seems to be slightly faster to avoid uint128_t here, although the generated code for uint128_t looks slightly nicer.
static inline uint32_t mulShift32(const uint32_t m, const uint64_t factor, const int32_t shift) {
  assert(shift > 32);

  // The casts here help MSVC to avoid calls to the __allmul library
  // function.
  const uint32_t factorLo = (uint32_t)(factor);
  const uint32_t factorHi = (uint32_t)(factor >> 32);
  const uint64_t bits0 = (uint64_t)m * factorLo;
  const uint64_t bits1 = (uint64_t)m * factorHi;

#if Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
  // On 32-bit platforms we can avoid a 64-bit shift-right since we only
  // need the upper 32 bits of the result and the shift value is > 32.
  const uint32_t bits0Hi = (uint32_t)(bits0 >> 32);
  uint32_t bits1Lo = (uint32_t)(bits1);
  uint32_t bits1Hi = (uint32_t)(bits1 >> 32);
  bits1Lo += bits0Hi;
  bits1Hi += (bits1Lo < bits0Hi);
  if (shift >= 64) {
    // s2f can call this with a shift value >= 64, which we have to handle.
    // This could now be slower than the !defined(RYU_32_BIT_PLATFORM) case.
    return (uint32_t)(bits1Hi >> (shift - 64));
  } else {
    const int32_t s = shift - 32;
    return (bits1Hi << (32 - s)) | (bits1Lo >> s);
  }
#else // #if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
  const uint64_t sum = (bits0 >> 32) + bits1;
  const uint64_t shiftedSum = sum >> (shift - 32);
  assert(shiftedSum <= UINT32_MAX);
  return (uint32_t)shiftedSum;
#endif
}

static inline uint32_t
mulPowFiveInvDivPowTwoNatural32
  (
    Arcadia_Thread* thread,
    const uint32_t m,
    const uint32_t q,
    const int32_t j
  )
{ return mulShift32(m, REAL32_POW5_INV_SPLIT[q], j); }

static inline uint32_t
mulPowFiveDivPowTwoNatural32
  (
    Arcadia_Thread* thread,
    const uint32_t m,
    const uint32_t q,
    const int32_t j
  )
{ return mulShift32(m, REAL32_POW5_SPLIT[q], j); }

static void
toChars32
  (
    Arcadia_Thread* thread,
    FloatingDecimal32 const* src,
    Buffer* dst
  )
{
  if (!src->sign) {
    Buffer_append(thread, dst, u8"-", sizeof(u8"-") - 1);
  }
  uint32_t output = src->mantissa;
  const uint32_t outputLength = decimalLength9(output);

  uint32_t i = 0;
  while (output >= 10000) {
    const uint32_t c = output % 10000;
    output /= 10000;
    const uint32_t c0 = (c % 100) << 1;
    const uint32_t c1 = (c / 100) << 1;
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength - i - 1]), DIGIT_TABLE + c0, 2);
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength - i - 3]), DIGIT_TABLE + c1, 2);
    i += 4;
  }
  if (output >= 100) {
    const uint32_t c = (output % 100) << 1;
    output /= 100;
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength - i - 1]), DIGIT_TABLE + c, 2);
    i += 2;
  }
  if (output >= 10) {
    const uint32_t c = output << 1;
    // We can't use memcpy here, the decimal dot goes between these two digits.
    dst->p[dst->n + outputLength - i] =  DIGIT_TABLE[c + 1];
    dst->p[dst->n] = DIGIT_TABLE[c];
  } else {
    dst->p[dst->n] = (char)('0' + output);
  }
  // Print decimal point if neded.
  if (outputLength > 1) {
    dst->p[dst->n + 1] = '.';
    dst->n += outputLength + 1;
  } else {
    ++dst->n;
  }
  // Print the exponent.
  dst->p[dst->n++] = 'E';
  int32_t exp = src->exponent + (int32_t)outputLength - 1;
  if (exp < 0) {
    dst->p[dst->n++] = '-';
    exp = -exp;
  }
  if (exp >= 10) {
    Arcadia_Memory_copy(thread, &(dst->p[dst->n]), DIGIT_TABLE + 2 * exp, 2);
    dst->n += 2;
  } else {
    dst->p[dst->n++] = (char)('0' + exp);
  }
}

static void
toString32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value value,
    Buffer* buffer
  )
{
  Arcadia_Natural32Value bits = Arcadia_Real32Value_getBits(thread, value);
  // (1) Extract the exponents bits, the significand bits, and the sign bits.
  Arcadia_Natural32Value ieeeExponentBits = (Arcadia_Real32Value_ExponentBitsMask & bits) >> Arcadia_Real32Value_ExponentBitsShift;
  Arcadia_Natural32Value ieeeSignificandBits = (Arcadia_Real32Value_SignificandBitsMask & bits) >> Arcadia_Real32Value_SignificandBitsShift;
  Arcadia_Natural32Value ieeeSignBits = (Arcadia_Real32Value_SignBitsMask & bits) >> Arcadia_Real32Value_SignBitsShift;
  // (2) Handle the cases of positive infinity, negative infinity, not a number.
  if (ieeeExponentBits == ((1u << Arcadia_Real32Value_NumberOfExponentBits) - 1u)) {
    if (ieeeSignificandBits == 0) {
      // exponent = 2^r - 1 and significand = 0 => infinity (where r is the number of exponent bits)
      Buffer_append(thread, buffer, ieeeSignBits ? positiveInfinityString : negativeInfinityString, ieeeSignBits ? sizeof(positiveInfinityString) - 1 : sizeof(negativeInfinityString) - 1);
      return;
    } else {
      // exponent = 2^r - 1 and significand > 0 => not a number (where r is the number of exponent bits)
      Buffer_append(thread, buffer, nanString, sizeof(nanString) - 1);
      return;
    }
  } else if (ieeeExponentBits == 0x0 && ieeeSignificandBits == 0x0) {
    // exponent = 0 and significand = 0 => signed zero (where r is the number of exponent bits)
    // exoonent if significand > 0 => denormalized (where r is the number of exponent bits)
    Buffer_append(thread, buffer, ieeeSignBits ? positiveZero : negativeZero, ieeeSignBits ? sizeof(positiveZero) - 1 : sizeof(negativeZero) - 1);
    return;
  }

  // (3) Compute m2 and e2.
  Arcadia_Natural64Value m2;
  Arcadia_Integer32Value e2;
  if (!ieeeExponentBits) {
    // This is the denormalized case.
    e2 = Arcadia_Integer32Value_Literal(1) - Arcadia_Real32Value_ExponentBias - Arcadia_Real32Value_NumberOfExplicitSignificandBits - 2;
    m2 = ieeeSignificandBits;
  } else {
    // This is the normalized case.
    e2 = ((Arcadia_Integer32Value)ieeeExponentBits) - Arcadia_Real32Value_ExponentBias - Arcadia_Real32Value_NumberOfExplicitSignificandBits - 2;
    m2 = (Arcadia_Natural32Value_Literal(1) << Arcadia_Real32Value_NumberOfExplicitSignificandBits) | ieeeSignificandBits;
  }
  const Arcadia_BooleanValue even = (m2 & 1) == 0;
  const Arcadia_BooleanValue acceptBounds = even;

  // (4) Compute mv, mp, mm.
  // Implicit bool -> int conversion. True is 1, false is 0.
  const uint32_t mmShift = ieeeSignificandBits != 0 || ieeeExponentBits <= 1;
  const uint32_t mv = 4 * m2;
  const uint32_t mp = 4 * m2 + 2;
  const uint32_t mm = 4 * m2 - 1 - mmShift;

  uint32_t vr, vp, vm;
  int32_t e10;
  bool vmIsTrailingZeros = false;
  bool vrIsTrailingZeros = false;
  uint8_t lastRemovedDigit = 0;
  // (5) Convert to decimal power base using 128-bit arithmetic.
  if (e2 >= 0) {
    // This expression is slightly faster than max(0, log10Pow2(e2) - 1).
    // e2 becomes 1 if e2 is greater than 3. In the case of e2 greater than 3 lo10Pow2(e2) is at least 1 so the subtraction cannot underflow.
    const uint32_t q = log10Pow2(e2)/*- (e2 > 3)*/;
    e10 = (int32_t)q;
    const int32_t k = REAL32_POW5_INV_BITCOUNT + pow5bits((int32_t)q) - 1;
    const int32_t i = -e2 + (int32_t)q + k;
    vr = mulPowFiveInvDivPowTwoNatural32(thread, mv, q, i);
    vp = mulPowFiveInvDivPowTwoNatural32(thread, mp, q, i);
    vm = mulPowFiveInvDivPowTwoNatural32(thread, mm, q, i);
    if (q != 0 && (vp - 1) / 10 <= vm / 10) {
      // We need toknow one removed digit even if ware not going to loop below.
      // We could use q = X - 1 above, except that would require 33 bits for the result,
      // and we've found that 32 arithmetic is faster even on 64-bit machines.
      const int32_t l = REAL32_POW5_INV_BITCOUNT + pow5bits((int32_t)(q - 1)) - 1;
      lastRemovedDigit = (uint8_t)(mulPowFiveInvDivPowTwoNatural32(thread, mv, q - 1, -e2 + (int32_t)q - 1 + l) % 10);
    }
    if (q <= 9) {
      // The largest power of 5 that fits in 24 bits is 5^10 but q <= 9 seems to be safe as well.
      // Onkly one of mp, mv, and mm can be a multiple of five, if any.
      if (mv % 5 == 0) {
        vmIsTrailingZeros = Arcadia_isDivisibleByPowerOfFiveNatural32(thread, mv, q);
      } else if (acceptBounds) {
        vmIsTrailingZeros = Arcadia_isDivisibleByPowerOfFiveNatural32(thread, mm, q);
      } else {
        vp -= Arcadia_isDivisibleByPowerOfFiveNatural32(thread, mp, q);
      }
    }
  } else {
    const uint32_t q = log10Pow5(-e2) - (-e2 > 1);
    e10 = (int32_t)q + e2;
    const int32_t i = -e2 - (int32_t)q;
    const int32_t k = pow5bits(i) - REAL32_POW5_BITCOUNT;
    int32_t j = (int32_t)q - k;
    vr = mulPowFiveDivPowTwoNatural32(thread, mv, i, j); // multiply by power of five, divide by power of two
    vp = mulPowFiveDivPowTwoNatural32(thread, mp, i, j);
    vm = mulPowFiveDivPowTwoNatural32(thread, mm, i, j);
    if (q != 0 && (vp -1) / 10 <= vm / 10) {
      j = (int32_t)q - 1 - pow5bits(i + 1) - REAL32_POW5_BITCOUNT;
      lastRemovedDigit = (uint8_t)(mulPowFiveDivPowTwoNatural32(thread, mv, (uint32_t)(i + 1), j) % 10);
    }
    if (q <= 1) {
      // {vr, vp, vm} is trailing zeros if {mv, mp, mm} has at least q trailing 0 bits.
      // mv = 4 * m2, so it always has at least two trailing 0 bits.
      vrIsTrailingZeros = true;
      if (acceptBounds) {
        // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit iff mmShift = 1.
        vmIsTrailingZeros = mmShift == 1;
      } else {
        // mp = mv + 2, so it always has at least one trailing 0 bit.
        --vp;
      }
    } else if (q < 31) {
      vrIsTrailingZeros = Natural32_multipleOfPowerOf2(thread, mv, q - 1);
    }
  }

  // Step 4: Find the shortest decimal representation in the interval of valid representiations.
  int32_t removed = 0;
  uint32_t output;
  if (vmIsTrailingZeros || vrIsTrailingZeros) {
    // General case, which happens rarely (~4.0%).
    while (vp / 10 > vm / 10) {
      vmIsTrailingZeros &= (vm % 10) == 0;
      vrIsTrailingZeros &= lastRemovedDigit == 0;
      lastRemovedDigit = (uint8_t)(vr % 10);
      vr /= 10;
      vp /= 10;
      vm /= 10;
      ++removed;
    }
    if (vmIsTrailingZeros) {
      while (vm % 10 == 0) {
        vrIsTrailingZeros &= lastRemovedDigit == 0;
        lastRemovedDigit = (uint8_t)(vr % 10);
        vr /= 10;
        vp /= 10;
        vm /= 10;
        ++removed;
      }
    }
    if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0) {
      // Round even if the exact number is ....50..0.
      lastRemovedDigit = 4;
    }
    // We need to take vr + 1 if vr is outside bounds or we need to round up.
    output = vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || lastRemovedDigit >= 5);
  } else {
    // Specialized for the common case (~96.0%). Percentages below are relative to this.
    // Loop iterations below (approximately):
    // 0: 13.6%, 1: 70.7%, 2: 14.1%, 3: 1.39%, 4: 0.14%, 5+: 0.0.1%
    while (vp / 10 > vm / 10) {
      lastRemovedDigit = (uint8_t)(vr % 10);
      vr /= 10;
      vp /= 10;
      vm /= 10;
      ++removed;
    }
    // We ned to take vr + 1 if vr is outside bounds or we need to round up.
    output = vr + (vr == vm || lastRemovedDigit >= 5);
  }
  const int32_t exp = e10 + removed;
  FloatingDecimal32 fd;
  fd.sign = !ieeeSignBits;
  fd.exponent = exp;
  fd.mantissa = output;

  toChars32(thread, &fd, buffer);
}

void
Arcadia_Real32Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value value,
    void* context,
    void (*function)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)
  )
{
  Buffer buffer;
  Buffer_init(thread, &buffer, 16);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    toString32(thread, value, &buffer);
    (*function)(thread, context, buffer.p, buffer.n);
    Arcadia_Thread_popJumpTarget(thread);
    Buffer_uninit(thread, &buffer);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Buffer_uninit(thread, &buffer);
    Arcadia_Thread_jump(thread);
  }
}
