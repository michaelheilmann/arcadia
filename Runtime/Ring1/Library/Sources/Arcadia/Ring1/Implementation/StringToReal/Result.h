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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_STRINGTOREAL_RESULT_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_STRINGTOREAL_RESULT_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Real64.h"
#include "Arcadia/Ring1/Implementation/BigInteger/Include.h"
#include "Arcadia/Ring1/Implementation/NumberLiteral.h"

typedef struct Arcadia_ToReal64_Result {

  /// The significand bits.
  Arcadia_Natural64Value significand;

  /// The exponent bits.
  Arcadia_Integer32Value exponent;

  /// Wether the number is positive or negative.
  Arcadia_BooleanValue negative;

  /// The result is not valid.
  /// Another method should be tried.
  Arcadia_BooleanValue failed;

} Arcadia_ToReal64_Result;

/// Given the decimal sign integral . fractional * 10^exponent without leading or trailing zeroes,
/// then this representation is sign integral fractional * 10^(exponent-shift) where shift = digits(fractional).
typedef struct Approximation {
  /// The significand as a Natural64 value.
  /// integral fractional
  Arcadia_Natural64Value significand;

  /// If not all digits of the significand can be stored in State::significand, then this is set to true.
  Arcadia_BooleanValue significandTruncated;

  /// The exponent as an Integer64Value.
  /// exponent
  Arcadia_Integer64Value exponent;

  /// If not all digits of the exponent can be stored in State::exponent, then this is set to true.
  Arcadia_BooleanValue exponentTruncated;

  /// Wether the number is negative (Arcadia_BooleanValue_True) or positive (Arcadia_BooleanValue_True).
  Arcadia_BooleanValue negative;

  /// The exponent shift.
  /// shift
  Arcadia_Integer64Value shift;
} Approximation;

/// Perform a quick check wether we can go for +/- 0 or +/- infinity.
/// - If we were decided for either, then result.exponent, result.significand, result.negative accordingly.
///   Furthermore, set result.failure to Arcadia_BooleanValue_False.
/// - If we were not able to decide, set result.failure to Arcadia_BooleanValue_True.
static inline void
fastFlow
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* target,
    Approximation const* source
  )
{
  Arcadia_Integer64Value e = source->exponent + source->shift;
  // THEOREM
  // 2^-1074 is the least positive value representable by a double.
  // PROOF:
  // Subnormal number 2^-1022 * 0.fraction. The least (non-zero) fraction is 51 bits of value 0 followed by 1 bit of value 1.
  // In other terms, 2^-1022 * 2^-52 = 2^-1074.

  // THEOREM
  // (2^1 + 2^-52) * 2^1023 is the greatest positive value representable by a double.
  // PROOF:
  // Normal number 1.fraction * 2^1023.
  // The greatest fraction is when all 52 bits are one hence (2^1 + 2^-52) * 2^1023.

  // If we could tell by looking at the exponent s.exponent = e-|w| of vw * 10^(e-|w|) that vw * 10^(e-|w|) < 20^-1074, then we could return zero immediatly.
  // That is, we want to determine a value t for e-|w| such that we may return 0 if e-|w| < t.
  // vw * 10^(e-|w|) < 2^-1074 | log10
  // log10(vw) * (e-|w|) < log10(2^-1074)
  // (e-|w|) < log10(2^-1074) - log10(vw) | vw is at most 2^64 - 1 as s.significand is a 64 bit unsigned integer
  // (e-|w|) < log10(2^-1074) - log10(2^64-1) | conservative approximation of log10(2^-1074) - log10(2^64-1)
  // (e-|w|) < -342.57...
  // As the least value of e-|w| smaller than -342.57... is -343 (e-|w| is an integer), t is -342.
  if (source->significand == Arcadia_Natural64Value_Literal(0) || e < Arcadia_Integer64Value_Literal(-342)) {
    target->significand = 0;
    target->exponent = 0;
    target->negative = source->negative;
    target->failed = Arcadia_BooleanValue_False;
    return;
  }
  // If we could tell by looking at the exponent s.exponent = e-|w| of vw * 10^(e-|w|) that vw * 10^(e-|w|) > (2^1 + 2^-52) * 2^1023, then we could return infinity immediatly.
  // That is, we want to determine a value t for e-|w| such that we may return infinity if e-|w| > t.
  // vw * 10^(e-|w|) > (2^1 + 2^-52) * 2^1023 | log10
  // log10(vw) + (e-|w|) > log10((2^1 + 2^-52) * 2^1023) | the least non-zero value of vw is 1
  // log10(1) (e-|w|) > log10((2^1 + 2^-52 * 2^1023)
  // (e-|w|) > log10((2^1 + 2^-52) * 2^1023)
  // (e-|e|) > 308.2547...
  // As the least value of e-|w| greater than 308.2547...is 309 (e-|w| is an integer), t is +308.
  if (e > Arcadia_Integer64Value_Literal(+308)) {
    target->significand = 0;
    target->exponent = 0x7ff;
    target->negative = source->negative;
    target->failed = Arcadia_BooleanValue_False;
    return;
  }
  target->failed = Arcadia_BooleanValue_True;
}

/// Given the decimal sign integral . fractional * 10^exponent without leading or trailing zeroes,
/// then this representation is sign integral fractional * 10^(exponent-shift) where shift = digits(fractional).
typedef struct State {
  /// The underlaying number literal.
  Arcadia_NumberLiteral literal;
  Approximation a;
} State;

void
Arcadia_ToReal64_BitsToValue
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value* target,
    Arcadia_ToReal64_Result* source
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_STRINGTOREAL_RESULT_H_INCLUDED
