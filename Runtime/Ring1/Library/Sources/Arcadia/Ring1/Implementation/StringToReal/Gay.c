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
#include "Arcadia/Ring1/Implementation/StringToReal/Gay.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Lemire.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Result.h"

// When perform a decimal to binary conversion,
// then this is the maximum number of decimal digits in the significand we accept during parsing.
// https://www.exploringbinary.com/maximum-number-of-decimal-digits-in-binary-floating-point-numbers/
// determines 767 digits. We add two more digits and obtain 769 digits. This is also the value used by https://github.com/fastfloat.
#define Arcadia_Real64Value_MaximumNumberOfParsedDecimalDigits (769)

// When perform a deciaml digit to binary conversion,
// then this is the maximum number of decimal digits in the significand we are accepting during parsing.
// https://www.exploringbinary.com/maximum-number-of-decimal-digits-in-binary-floating-point-numbers/
// determines 112 digits. We add two more digits and obtain 114 digits. This is also the value used by https://github.com/fastfloat.
#define Arcadia_Real32Value_MaximumNumberOfParsedDecimalDigits (114)

typedef enum Arcadia_NumberLiteralApproximation_SpecialValue {
  Arcadia_NumberLiteralApproximation_SpecialValue_Infinity,
  Arcadia_NumberLiteralApproximation_SpecialValue_NoNumber,
  Arcadia_NumberLiteralApproximation_SpecialValue_Regular,
} Arcadia_NumberLiteralApproximation_SpecialValue;

// scientificSignificand * 10^scientificExponent * 10^exponent
typedef struct Arcadia_NumberLiteralApproximation {
  struct {
    Arcadia_Natural64Value scientificSignificand;
    Arcadia_Integer32Value scientificExponent;
    Arcadia_BooleanValue truncated;
    /// The number of digits in the significand we considered.
    /// There is at least one digit.
    Arcadia_SizeValue digitsConsidered;
  } scientific;

  /// if the significand is negative.
  Arcadia_BooleanValue negative;
  /// The exponent.
  /// An Integer32 is sufficient as
  /// - values too small are assigned negative = <given>, significand = 0, and exponent = 0 and specialValue = Regular and
  /// - values too big are assigned negative = <given>, significand <retained>, and exponent = <max> and specialValue = Infinity.
  Arcadia_Integer32Value exponent;
  Arcadia_NumberLiteralApproximation_SpecialValue specialValue;
} Arcadia_NumberLiteralApproximation;

void
Arcadia_NumeralApproximation_fromNumeral
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* p,
    Arcadia_SizeValue n,
    Arcadia_BigInteger** bigScientificSignificand,
    Arcadia_NumberLiteralApproximation* target,
    Arcadia_NumberLiteral const* source
  )
{
  Arcadia_BigInteger* powerOfTen = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, powerOfTen, 10);
  (*bigScientificSignificand) = Arcadia_BigInteger_create(thread);
  target->negative = Arcadia_BooleanValue_False;
  if (source->significand.sign.length) {
    if (p[source->significand.sign.start] == '-') {
      target->negative = Arcadia_BooleanValue_True;
    }
  }
  target->specialValue = Arcadia_NumberLiteralApproximation_SpecialValue_Regular;
  target->scientific.scientificSignificand = Arcadia_Natural64Value_Literal(0);
  target->scientific.scientificExponent = Arcadia_Integer32Value_Literal(0);
  target->scientific.truncated = Arcadia_BooleanValue_False;
  target->scientific.digitsConsidered = 0;
  // (1) compute significand part.
  // We can - at most - store 19 decimal digits in a Natural64.
  Arcadia_SizeValue k, l;
  k = source->significand.integral.start + source->significand.integral.leadingZeroes.length;
  l = k + source->significand.integral.length - source->significand.integral.leadingZeroes.length;
  Arcadia_SizeValue i, j;
  // Squeeze in up to 19 integral digits.
  for (j = 0, i = k, n = l; j <= Arcadia_Natural64Value_MaximumNumberOfDecimalDigits && i < n; ++i, ++j) {
    Arcadia_Natural8Value digit = ((Arcadia_Natural64Value)p[i]) - '0';
    target->scientific.scientificSignificand *= 10;
    target->scientific.scientificSignificand += digit;
    Arcadia_BigInteger_multiply(thread, (*bigScientificSignificand), powerOfTen);
    Arcadia_BigInteger_addNatural64(thread, (*bigScientificSignificand), digit);
    target->scientific.digitsConsidered++;
  }
  if (i != n) {
    // Do we have even more integral digits?
    // Skip the remaining integral digits and fractional digits and increase the exponent by the number of skipped integral digits.
    target->scientific.scientificExponent += n - i;
    target->scientific.truncated = Arcadia_BooleanValue_True;
  } else {
    // The integral part was stored completely in the Natural64.
    // squeeze up to 19 fractional digits
    k = source->significand.fractional.start;
    l = k + source->significand.fractional.length - source->significand.fractional.trailingZeroes.length;
    for (i = k, n = l; j < Arcadia_Natural64Value_MaximumNumberOfDecimalDigits && i < n; ++j, ++i) {
      Arcadia_Natural8Value digit = ((Arcadia_Natural64Value)p[i]) - '0';
      target->scientific.scientificSignificand *= 10;
      target->scientific.scientificSignificand += digit;
      Arcadia_BigInteger_multiply(thread, (*bigScientificSignificand), powerOfTen);
      Arcadia_BigInteger_addNatural64(thread, (*bigScientificSignificand), digit);
      target->scientific.digitsConsidered++;
      target->scientific.scientificExponent -= 1; // Cannot as we do not do this more than 19 times.
    }
    if (i != n) {
      // Do we have even more fractional digits?
      // Skip the remaining fractional.
      target->scientific.truncated = Arcadia_BooleanValue_True;
    }
  }
  // (2) compute the exponent part.
  k = source->exponent.integral.start + source->exponent.integral.leadingZeroes.length;
  l = k + source->exponent.integral.length - source->exponent.integral.leadingZeroes.length;
  target->exponent = 0;
  for (j = 0, i = k, n = l; j < Arcadia_Integer32Value_MaximumNumberOfDecimalDigits && i < n; ++i, ++j) {
    target->exponent *= 10;
    target->exponent += ((Arcadia_Natural64Value)p[i]) - '0';
    //target->exponentscientific.scientificExponent -= 1; // Cannot as we do not do this more than 19 times.
  }
  target->exponent *= source->exponent.sign.length == 1 ? (p[source->exponent.sign.start] == '-' ? -1 : +1) : +1;
  if (i != n) {
    // We cannot represent the exponent anyway.
    if (target->exponent < 0) {
      target->exponent = 0;
      target->scientific.scientificSignificand = 0;
      target->scientific.truncated = Arcadia_BooleanValue_False;
    } else {
      target->specialValue = Arcadia_NumberLiteralApproximation_SpecialValue_Infinity;
    }
  }
}

typedef struct RoundDownInfo RoundDownInfo;
typedef struct RoundInfo RoundInfo;
typedef struct RoundNearestTieEvenInfo RoundNearestTieEvenInfo;


struct RoundDownInfo {
  Arcadia_Integer32Value shift;
};

struct RoundNearestTieEvenInfo {
  Arcadia_Integer32Value shift;
  Arcadia_BooleanValue truncated;
  Arcadia_BooleanValue isOdd;
  Arcadia_BooleanValue isHalfway;
  Arcadia_BooleanValue isAbove;
  Arcadia_Integer32Value order;
  Arcadia_BooleanValue (*cb)(Arcadia_Thread*, RoundInfo*);
};

struct RoundInfo {
  int dummy;
  union {
    RoundDownInfo roundDown;
    RoundNearestTieEvenInfo roundNearestTieEven;
  };
};

#define Rounding_RoundDown (1)
#define Rounding_RoundNearestTieToEven (2)

#define Arcadia_Real64Value_HiddenBitMask (UINT64_C(0x0010000000000000))

// this converts a native floating-point number to an extended-precision float.
static void to_extended(Arcadia_Thread* thread, Arcadia_ToReal64_Result* result, Arcadia_Real64Value value) {
  //using equiv_uint = equiv_uint_t<T>;
  uint64_t exponent_mask = Arcadia_Real64Value_ExponentBitsMask;
  uint64_t mantissa_mask = Arcadia_Real64Value_SignificandBitsMask;
  uint64_t hidden_bit_mask = Arcadia_Real64Value_HiddenBitMask;

  int32_t bias = Arcadia_Real64Value_NumberOfExplicitSignificandBits -
                 (Arcadia_Real64Value_MinimalExponent - 1);
  uint64_t bits;
  Arcadia_Memory_copy(thread, &bits, &value, sizeof(Arcadia_Real64Value));
  if ((bits & exponent_mask) == 0) {
    // denormal
    result->exponent = 1 - bias;
    result->significand = bits & mantissa_mask;
  } else {
    // normal
    result->exponent = (int32_t)((bits & exponent_mask) >> Arcadia_Real64Value_NumberOfExplicitSignificandBits);
    result->exponent -= bias;
    result->significand = (bits & mantissa_mask) | hidden_bit_mask;
  }
}

// get the extended precision value of the halfway point between b and b+u.
// we are given a native float that represents b, so we need to adjust it halfway between b and b+u.
static void
to_extended_halfway
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    Arcadia_Real64Value value
  )
{
  to_extended(thread, result, value);
  result->significand <<= 1;
  result->significand += 1;
  result->exponent -= 1;
}

static Arcadia_BooleanValue
doTie1
  (
    Arcadia_Thread* thread,
    RoundInfo* info
  )
{
  return info->roundNearestTieEven.isAbove
      || (info->roundNearestTieEven.isHalfway && info->roundNearestTieEven.truncated)
      || (info->roundNearestTieEven.isOdd && info->roundNearestTieEven.isHalfway);
}

static Arcadia_BooleanValue
doTie2
  (
    Arcadia_Thread* thread,
    RoundInfo* info
  )
{
  if (info->roundNearestTieEven.order > 0) {
    return Arcadia_BooleanValue_True;
  } else if (info->roundNearestTieEven.order < 0) {
    return Arcadia_BooleanValue_False;
  } else {
    return info->roundNearestTieEven.isOdd;
  }
}

static void
doRoundNearestTieToEven
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    RoundInfo* info
  )
{
  uint64_t const mask = (info->roundNearestTieEven.shift == 64) ? UINT64_MAX : (UINT64_C(1) << info->roundNearestTieEven.shift) - 1;
  uint64_t const halfway = (info->roundNearestTieEven.shift == 0) ? 0 : UINT64_C(1) << (info->roundNearestTieEven.shift - 1);
  uint64_t truncated_bits = result->significand & mask;
  bool isAbove = truncated_bits > halfway;
  bool isHalfway = truncated_bits == halfway;

  // shift digits into position
  if (info->roundNearestTieEven.shift == 64) {
    result->significand = 0;
  } else {
    result->significand >>= info->roundNearestTieEven.shift;
  }
  result->exponent += info->roundNearestTieEven.shift;

  bool isOdd = (result->significand & 1) == 1;
  info->roundNearestTieEven.isOdd = isOdd;
  info->roundNearestTieEven.isHalfway = isHalfway;
  info->roundNearestTieEven.isAbove = isAbove;
  result->significand += (uint64_t)(info->roundNearestTieEven.cb(thread, info));
}

static void
doRoundDown
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    RoundInfo* info
  )
{
  if (info->roundDown.shift == 64) {
    result->significand = 0;
  } else {
    result->significand >>= info->roundDown.shift;
  }
  result->exponent += info->roundDown.shift;
}

static void
doRound
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    RoundInfo *roundInfo
  )
{
  void (*cb)(Arcadia_Thread*, Arcadia_ToReal64_Result*, RoundInfo*) = NULL;
  switch (roundInfo->dummy) {
    case Rounding_RoundDown: {
      cb = &doRoundDown;
    } break;
    case Rounding_RoundNearestTieToEven: {
      cb = &doRoundNearestTieToEven;
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  int32_t significandShift = 64 - Arcadia_Real64Value_NumberOfExplicitSignificandBits - 1;
  if (-result->exponent >= significandShift) {
    // We have a denormal float.
    int32_t shift = -result->exponent + 1;
    roundInfo->roundDown.shift = Arcadia_minimumInteger32Value(thread, shift, 64);
    cb(thread, result, roundInfo);
    // check for round-up: if rounding-nearest carried us to the hidden bit.
    result->exponent = (result->significand < (UINT64_C(1) << Arcadia_Real64Value_NumberOfExplicitSignificandBits))
                       ? 0
                       : 1;
  } else {
    // We have a normal float. Uuse the default shift.
    roundInfo->roundDown.shift = significandShift;
    cb(thread, result, roundInfo);

    // check for carry
    if (result->significand >=(UINT64_C(2) << Arcadia_Real64Value_NumberOfExplicitSignificandBits)) {
      result->significand = (UINT64_C(1) << Arcadia_Real64Value_NumberOfExplicitSignificandBits);
      result->exponent++;
    }

    // check for infinite: we could have carried to an infinite power
    result->significand &= ~(UINT64_C(1) << Arcadia_Real64Value_NumberOfExplicitSignificandBits);
    if (result->exponent >= 0x7ff) {
      result->exponent = 0x7ff;
      result->significand = 0;
    }
  }
}

static Arcadia_BooleanValue
onRoundNonNegative
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    Arcadia_Integer32Value exponent,
    Arcadia_BigInteger* bigScientificSignificand,
    Arcadia_NumberLiteralApproximation* approximation
  )
{
  // Firstly, we have obviously w >= 2^64 or e >=
  Arcadia_BigInteger* powerOfTen = Arcadia_BigInteger_create(thread);
  // We can do that because the precondition here is that the exponent is positive.
  Arcadia_BigInteger_setPowerOfTenNatural32(thread, powerOfTen, (Arcadia_Natural32Value)exponent);
  Arcadia_BigInteger_multiply(thread, bigScientificSignificand, powerOfTen);
  Arcadia_BooleanValue isTruncated;
  result->significand = Arcadia_BigInteger_getHigh64(thread, bigScientificSignificand, &isTruncated);
  Arcadia_Integer32Value bias = Arcadia_Real64Value_NumberOfExplicitSignificandBits - (Arcadia_Real64Value_MinimalExponent - 1);
  Arcadia_Integer32Value bitLength = (Arcadia_Integer32Value)Arcadia_BigInteger_getBitLength(thread, bigScientificSignificand);
  result->exponent = bitLength - 64 + bias;

  // round
  RoundInfo roundInfo = { .dummy = Rounding_RoundNearestTieToEven, .roundNearestTieEven.truncated = isTruncated };
  roundInfo.roundNearestTieEven.cb = &doTie1;
  doRound(thread, result, &roundInfo);
#if _DEBUG
  if (result->exponent == 2047 && result->significand == 0) {
    Arcadia_logf(Arcadia_LogFlags_Trace, "infinity\n");
  }
#endif

  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
onRoundNegative
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    Arcadia_Integer32Value exponent,
    Arcadia_BigInteger* bigScientificSignificand,
    Arcadia_NumberLiteralApproximation* approximation
  )
{
  Arcadia_ToReal64_Result resultB = *result;
  // round
  RoundInfo roundInfo = { .dummy = Rounding_RoundDown };
  doRound(thread, &resultB, &roundInfo);

  Arcadia_Real64Value b;
  Arcadia_ToReal64_BitsToValue(thread, &b, &resultB);
  Arcadia_ToReal64_Result resultTheoretical;
  to_extended_halfway(thread, &resultTheoretical, b);
  Arcadia_BigInteger* theoreticalBigInteger = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, theoreticalBigInteger, resultTheoretical.significand);

  Arcadia_Integer32Value exponentReal = exponent;
  Arcadia_Integer32Value exponentTheoretical = resultTheoretical.exponent;

  Arcadia_Integer32Value pow2Exp = exponentTheoretical - exponentReal;
  Arcadia_Natural32Value pow5Exp = (Arcadia_Natural32Value)-exponentReal;

  if (pow5Exp != 0) {
    Arcadia_BigInteger* powerOfFive = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfFiveNatural32(thread, powerOfFive, pow5Exp);
    Arcadia_BigInteger_multiply(thread, theoreticalBigInteger, powerOfFive);
  }
  if (pow2Exp > 0) {
    Arcadia_BigInteger* powerOfTwo = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfTwoNatural32(thread, powerOfTwo, (Arcadia_Natural32Value)+pow2Exp);
    Arcadia_BigInteger_multiply(thread, theoreticalBigInteger, powerOfTwo);
  } else if (pow2Exp < 0) {
    Arcadia_BigInteger* powerOfTwo = Arcadia_BigInteger_create(thread);
    Arcadia_BigInteger_setPowerOfTwoNatural32(thread, powerOfTwo, (Arcadia_Natural32Value)-pow2Exp);
    Arcadia_BigInteger_multiply(thread, bigScientificSignificand, powerOfTwo);
  }

  Arcadia_Integer32Value order = Arcadia_BigInteger_compareTo(thread, bigScientificSignificand, theoreticalBigInteger);
  roundInfo.roundNearestTieEven.order = order;
  roundInfo.roundNearestTieEven.cb = &doTie2;
  roundInfo.dummy = Rounding_RoundNearestTieToEven;
  doRound(thread, result, &roundInfo);

  return Arcadia_BooleanValue_True;
}

void
Arcadia_gayReal64
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    const Arcadia_Natural8Value* p,
    Arcadia_SizeValue n,
    const Approximation* a,
    Arcadia_NumberLiteral const* numberLiteral
  )
{
  fastFlow(thread, result, a);
  if (!result->failed) {
    return;
  }

  //
  Arcadia_SizeValue lz = Arcadia_countLeadingZeroesNatural64Value(thread, a->significand);
  Arcadia_Natural64Value w = a->significand << lz;
  Arcadia_Natural64Value upper, lower;
  Arcadia_computeProductApproximation(thread, w, a->exponent + a->shift, &upper, &lower);
  int hilz = (int)(upper >> 63) ^ 1;
  result->significand = upper << hilz;
  int bias = Arcadia_Real64Value_NumberOfExplicitSignificandBits - (-1023);
  result->exponent = (int32_t)(Arcadia_power((int32_t)(a->exponent + a->shift)) + bias - hilz - lz - 62);

  Arcadia_BigInteger* bigScientificSignificand = NULL;
  Arcadia_NumberLiteralApproximation numberLiteralApproximation;
  Arcadia_NumeralApproximation_fromNumeral(thread, p, n, &bigScientificSignificand, &numberLiteralApproximation, numberLiteral);
  Arcadia_Integer64Value scientificExponent = 0;
  {
    Arcadia_Natural64Value v = numberLiteralApproximation.scientific.scientificSignificand;
    while (v >= 1000) {
      v = v / 1000;
      scientificExponent += 3;
    }
    while (v >= 100) {
      v = v / 100;
      scientificExponent += 2;
    }
    while (v >= 10) {
      v = v / 10;
      scientificExponent += 1;
    }
    scientificExponent += a->exponent + a->shift;
  }
  Arcadia_BigInteger_toStdoutDebug(thread, bigScientificSignificand);
  Arcadia_Integer32Value exponent = scientificExponent + 1 - numberLiteralApproximation.scientific.digitsConsidered;
  result->negative = numberLiteralApproximation.negative;
  if (exponent >= 0) {
    if (onRoundNonNegative(thread, result, exponent, bigScientificSignificand, &numberLiteralApproximation)) {
      result->failed = false;
    } else {
      result->failed = true;
    }
  } else {
    if (onRoundNegative(thread, result, exponent, bigScientificSignificand, &numberLiteralApproximation)) {
      result->failed = false;
    } else {
      result->failed = true;
    }
  }
}
