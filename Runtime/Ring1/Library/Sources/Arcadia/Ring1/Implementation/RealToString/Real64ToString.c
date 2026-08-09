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
#include "Arcadia/Ring1/Implementation/RealToString/Real64ToString.h"

#include "Arcadia/Ring1/Implementation/RealToString/Common.i"
#include "Arcadia/Ring1/Implementation/Real64_getBits.h"

#include "Arcadia/Ring1/Implementation/RealToString/BigTablesReal64.i"

static inline uint32_t
decimalLength17
  (
    const uint64_t v
  )
{
  // This is slightly faster than a loop.
  // The average output length is 16.38 digits, so we check high-to-low.
  // Function precondition: v is not an 18, 19, or 20-digit number.
  // (17 digits are sufficient for round-tripping.)
  assert(v < 100000000000000000L);
  if (v >= 10000000000000000L) { return 17; }
  if (v >= 1000000000000000L) { return 16; }
  if (v >= 100000000000000L) { return 15; }
  if (v >= 10000000000000L) { return 14; }
  if (v >= 1000000000000L) { return 13; }
  if (v >= 100000000000L) { return 12; }
  if (v >= 10000000000L) { return 11; }
  if (v >= 1000000000L) { return 10; }
  if (v >= 100000000L) { return 9; }
  if (v >= 10000000L) { return 8; }
  if (v >= 1000000L) { return 7; }
  if (v >= 100000L) { return 6; }
  if (v >= 10000L) { return 5; }
  if (v >= 1000L) { return 4; }
  if (v >= 100L) { return 3; }
  if (v >= 10L) { return 2; }
  return 1;
}

// A floating decimal representing m * 10^e.
typedef struct FloatingDecimal64 {
  bool sign; // true ~ +, false ~ -
  uint64_t mantissa;
  // Decimal exponent's range is -324 to 308 inclusive, and can fit in a short if needed.
  int32_t exponent;
} FloatingDecimal64;

static void
toChars64
  (
    Arcadia_Thread* thread,
    FloatingDecimal64 const* src,
    Buffer* dst
  );

static void
toString64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value src,
    Buffer* dst
  );

static void
toChars64
  (
    Arcadia_Thread* thread,
    FloatingDecimal64 const* src,
    Buffer* dst
  )
{
  if (!src->sign) {
    Buffer_append(thread, dst, u8"-", sizeof(u8"-") - 1);
  }
  uint64_t output = src->mantissa;
  const uint32_t outputLength = decimalLength17(output);

  uint32_t i = 0;
  if ((output >> 32) != 0) {
    const uint64_t q = div1e8(thread, output);
    uint32_t output2 = ((uint32_t)output) - 100000000 * ((uint32_t)q);
    output = q;

    const uint32_t c = output2 % 10000;
    output2 /= 10000;
    const uint32_t d = output2 % 10000;
    const uint32_t c0 = (c % 100) << 1;
    const uint32_t c1 = (c / 100) << 1;
    const uint32_t d0 = (d % 100) << 1;
    const uint32_t d1 = (d / 100) << 1;
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength /* - i */ - 1]), DIGIT_TABLE + c0, 2);
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength /* - i */ - 3]), DIGIT_TABLE + c1, 2);
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength /* - i */ - 5]), DIGIT_TABLE + d0, 2);
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength /* - i */ - 7]), DIGIT_TABLE + d1, 2);
    i += 8;
  }
  uint32_t output2 = (uint32_t)output;
  while (output2 >= 10000) {
  #ifdef __clang__ // https://bugs.llvm.org/show_bug.cgi?id=38217
    const uint32_t c = output2 - 10000 * (output2 / 10000);
  #else
    const uint32_t c = output2 % 10000;
  #endif
    output2 /= 10000;
    const uint32_t c0 = (c % 100) << 1;
    const uint32_t c1 = (c / 100) << 1;
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength - i - 1]), DIGIT_TABLE + c0, 2);
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength - i - 3]), DIGIT_TABLE + c1, 2);
    i += 4;
  }
  if (output2 >= 100) {
    const uint32_t c = (output2 % 100) << 1;
    output2 /= 100;
    Arcadia_Memory_copy(thread, &(dst->p[dst->n + outputLength - i - 1]), DIGIT_TABLE + c, 2);
    i += 2;
  }
  if (output2 >= 10) {
    const uint32_t c = output2 << 1;
    // We can't use memcpy here: the decimal dot goes between these two digits.
    dst->p[dst->n + outputLength - i] = DIGIT_TABLE[c + 1];
    dst->p[dst->n] = DIGIT_TABLE[c];
  } else {
    dst->p[dst->n] = (char)('0' + output2);
  }

  // Print decimal point if needed.
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

  if (exp >= 100) {
    const int32_t c = exp % 10;
    Arcadia_Memory_copy(thread, &(dst->p[dst->n]), DIGIT_TABLE + 2 * (exp / 10), 2);
    dst->p[dst->n + 2] = (char)('0' + c);
    dst->n += 3;
  } else if (exp >= 10) {
    Arcadia_Memory_copy(thread, &(dst->p[dst->n]), DIGIT_TABLE + 2 * exp, 2);
    dst->n += 2;
  } else {
    dst->p[dst->n++] = (char)('0' + exp);
  }
}

static void
toString64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value value,
    Buffer* buffer
  )
{
  Arcadia_Natural64Value bits = Arcadia_Real64Value_getBits(thread, value);
  // (1) Extract the exponents bits, the significand bits, and the sign bits.
  Arcadia_Natural64Value ieeeExponentBits = (Arcadia_Real64Value_ExponentBitsMask & bits) >> Arcadia_Real64Value_ExponentBitsShift;
  Arcadia_Natural64Value ieeeSignificandBits = (Arcadia_Real64Value_SignificandBitsMask & bits) >> Arcadia_Real64Value_SignificandBitsShift;
  Arcadia_Natural64Value ieeeSignBits = (Arcadia_Real64Value_SignBitsMask & bits) >> Arcadia_Real64Value_SignBitsShift;
  // (2) Handle the cases of positive infinity, negative infinity, not a number.
  if (ieeeExponentBits == ((1u << Arcadia_Real64Value_NumberOfExponentBits) - 1u)) {
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
    e2 = Arcadia_Integer32Value_Literal(1) - Arcadia_Real64Value_ExponentBias - Arcadia_Real64Value_NumberOfExplicitSignificandBits - 2;
    m2 = ieeeSignificandBits;
  } else {
    // This is the normalized case.
    e2 = ((Arcadia_Integer32Value)ieeeExponentBits) - Arcadia_Real64Value_ExponentBias - Arcadia_Real64Value_NumberOfExplicitSignificandBits - 2;
    m2 = (Arcadia_Natural64Value_Literal(1) << Arcadia_Real64Value_NumberOfExplicitSignificandBits) | ieeeSignificandBits;
  }
  const Arcadia_BooleanValue even = (m2 & 1) == 0;
  const Arcadia_BooleanValue acceptBounds = even;

  // (4) Compute mv, mp, mm.
  // Implicit bool -> int conversion. True is 1, false is 0.
  const uint32_t mmShift = ieeeSignificandBits != 0 || ieeeExponentBits <= 1;
  const uint64_t mv = 4 * m2;
#if 0
  const uint64_t mp = 4 * mv + 2;
  const uint64_t mm = mv - 1 - mmShift;
#endif

  uint64_t vr, vp, vm;
  int32_t e10;
  bool vmIsTrailingZeros = false;
  bool vrIsTrailingZeros = false;
  // (5) Convert to decimal power base using 128-bit arithmetic.
  if (e2 >= 0) {
    // This expression is slightly faster than max(0, log10Pow2(e2) - 1).
    // e2 becomes 1 if e2 is greater than 3. In the case of e2 greater than 3 lo10Pow2(e2) is at least 1 so the subtraction cannot underflow.
    const uint32_t q = log10Pow2(e2) - (e2 > 3);
    e10 = (int32_t)q;
    const int32_t k = REAL64_POW5_INV_BITCOUNT + pow5bits((int32_t)q) - 1;
    const int32_t i = -e2 + (int32_t)q + k;
    vr = mulShiftAll64(thread, m2, REAL64_POW5_INV_SPLIT[q], i, &vp, &vm, mmShift);
    if (q <= 21) {
      // This should use q <= 22 = floor(log_5(2^53)), but Ulf Jack thinks 21 is also safe.
      // Smaller values may still be safe, but it's more difficult to reason about them.
      // Only one of mp, mv, and mm can be a multiple of 5, if any.
      const uint32_t mvMod5 = ((uint32_t)mv) - 5 * ((uint32_t)div5(thread, mv));
      if (mvMod5 == 0) {
        vrIsTrailingZeros = Arcadia_isDivisibleByPowerOfFiveNatural64(thread, mv, q);
      } else if (acceptBounds) {
        // Same as min(e2 + (~mm & 1), pow5Factor(mm)) >= q
        // <=> e2 + (~mm & 1) >= q && pow5Factor(mm) >= q
        // <=> true && pow5Factor(mm) >= q, since e2 >= q.
        vmIsTrailingZeros = Arcadia_isDivisibleByPowerOfFiveNatural64(thread, mv - 1 - mmShift, q);
      } else {
        // Same as min(e2 + 1, pow5Factor(mp)) >= q.
        vp -= Arcadia_isDivisibleByPowerOfFiveNatural64(thread, mv + 2, q);
      }
    }
  } else {
    const uint32_t q = log10Pow5(-e2) - (-e2 > 1);
    e10 = (int32_t)q + e2;
    const int32_t i = -e2 - (int32_t)q;
    const int32_t k = pow5bits(i) - REAL64_POW5_BITCOUNT;
    const int32_t j = (int32_t)q - k;
    vr = mulShiftAll64(thread, m2, REAL64_POW5_SPLIT[i], j, &vp, &vm, mmShift);
    if (q <= 1) {
      // {vr,vp,vm} is trailing zeros if {mv,mp,mm} has at least q trailing 0 bits.
      // mv = 4 * m2, so it always has at least two trailing 0 bits.
      vrIsTrailingZeros = true;
      if (acceptBounds) {
        // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit iff mmShift == 1.
        vmIsTrailingZeros = mmShift == 1;
      } else {
        // mp = mv + 2, so it always has at least one trailing 0 bit.
        --vp;
      }
    } else if (q < 63) {
      // TODO(ulfjack): Use a tighter bound here.
      // We want to know if the full product has at least q trailing zeros.
      // We need to compute min(p2(mv), p5(mv) - e2) >= q
      // <=> p2(mv) >= q && p5(mv) - e2 >= q
      // <=> p2(mv) >= q (because -e2 >= q)
      vrIsTrailingZeros = Natural64_isMultipleOfPowerOf2(thread, mv, q);
    }
  }
  // (6) Find the shortest decimal representation in the interval of valid representations.
  int32_t removed = 0;
  uint8_t lastRemovedDigit = 0;
  uint64_t output;
  // On average, we remove ~2 digits.
  if (vmIsTrailingZeros || vrIsTrailingZeros) {
    // General case, which happens rarely (~0.7%).
    for (;;) {
      const uint64_t vpDiv10 = div10(thread, vp);
      const uint64_t vmDiv10 = div10(thread, vm);
      if (vpDiv10 <= vmDiv10) {
        break;
      }
      const uint32_t vmMod10 = ((uint32_t)vm) - 10 * ((uint32_t)vmDiv10);
      const uint64_t vrDiv10 = div10(thread, vr);
      const uint32_t vrMod10 = ((uint32_t)vr) - 10 * ((uint32_t)vrDiv10);
      vmIsTrailingZeros &= vmMod10 == 0;
      vrIsTrailingZeros &= lastRemovedDigit == 0;
      lastRemovedDigit = (uint8_t)vrMod10;
      vr = vrDiv10;
      vp = vpDiv10;
      vm = vmDiv10;
      ++removed;
    }
    if (vmIsTrailingZeros) {
      for (;;) {
        const uint64_t vmDiv10 = div10(thread, vm);
        const uint32_t vmMod10 = ((uint32_t)vm) - 10 * ((uint32_t)vmDiv10);
        if (vmMod10 != 0) {
          break;
        }
        const uint64_t vpDiv10 = div10(thread, vp);
        const uint64_t vrDiv10 = div10(thread, vr);
        const uint32_t vrMod10 = ((uint32_t)vr) - 10 * ((uint32_t)vrDiv10);
        vrIsTrailingZeros &= lastRemovedDigit == 0;
        lastRemovedDigit = (uint8_t)vrMod10;
        vr = vrDiv10;
        vp = vpDiv10;
        vm = vmDiv10;
        ++removed;
      }
    }
    if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0) {
      // Round even if the exact number is .....50..0.
      lastRemovedDigit = 4;
    }
    // We need to take vr + 1 if vr is outside bounds or we need to round up.
    output = vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || lastRemovedDigit >= 5);
  } else {
    // Specialized for the common case (~99.3%). Percentages below are relative to this.
    bool roundUp = false;
    const uint64_t vpDiv100 = div100(thread, vp);
    const uint64_t vmDiv100 = div100(thread, vm);
    if (vpDiv100 > vmDiv100) { // Optimization: remove two digits at a time (~86.2%).
      const uint64_t vrDiv100 = div100(thread, vr);
      const uint32_t vrMod100 = ((uint32_t)vr) - 100 * ((uint32_t)vrDiv100);
      roundUp = vrMod100 >= 50;
      vr = vrDiv100;
      vp = vpDiv100;
      vm = vmDiv100;
      removed += 2;
    }
    // Loop iterations below (approximately), without optimization above:
    // 0: 0.03%, 1: 13.8%, 2: 70.6%, 3: 14.0%, 4: 1.40%, 5: 0.14%, 6+: 0.02%
    // Loop iterations below (approximately), with optimization above:
    // 0: 70.6%, 1: 27.8%, 2: 1.40%, 3: 0.14%, 4+: 0.02%
    for (;;) {
      const uint64_t vpDiv10 = div10(thread, vp);
      const uint64_t vmDiv10 = div10(thread, vm);
      if (vpDiv10 <= vmDiv10) {
        break;
      }
      const uint64_t vrDiv10 = div10(thread, vr);
      const uint32_t vrMod10 = ((uint32_t)vr) - 10 * ((uint32_t)vrDiv10);
      roundUp = vrMod10 >= 5;
      vr = vrDiv10;
      vp = vpDiv10;
      vm = vmDiv10;
      ++removed;
    }
    // We need to take vr + 1 if vr is outside bounds or we need to round up.
    output = vr + (vr == vm || roundUp);
  }
  const int32_t exp = e10 + removed;

  FloatingDecimal64 fd;
  fd.sign = !ieeeSignBits;
  fd.exponent = exp;
  fd.mantissa = output;

  toChars64(thread, &fd, buffer);
}

void
Arcadia_Real64Value_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value value,
    void* context,
    void (*function)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)
  )
{
  Buffer buffer;
  Buffer_init(thread, &buffer, 25);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    toString64(thread, value, &buffer);
    (*function)(thread, context, buffer.p, buffer.n);
    Arcadia_Thread_popJumpTarget(thread);
    Buffer_uninit(thread, &buffer);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Buffer_uninit(thread, &buffer);
    Arcadia_Thread_jump(thread);
  }
}
