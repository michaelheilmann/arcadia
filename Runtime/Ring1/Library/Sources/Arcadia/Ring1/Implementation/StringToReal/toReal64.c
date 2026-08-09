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
#include "Arcadia/Ring1/Implementation/StringToReal/toReal64.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Result.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Clinger.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Gay.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Lemire.h"
#include <assert.h>

static Arcadia_Integer64Value
decimalDigitsToInteger64Value
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* decimalDigits,
    Arcadia_SizeValue length
  );

static Arcadia_Integer64Value
decimalDigitsToInteger64Value
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value* decimalDigits,
    Arcadia_SizeValue length
  )
{
  Arcadia_BigInteger* q = Arcadia_BigInteger_create(thread);
  for (Arcadia_SizeValue i  = 0, n = length; i < n; ++i) {
    Arcadia_Natural8Value digit = (Arcadia_Natural8Value)decimalDigits[i] - (Arcadia_Natural8Value)'0';
    Arcadia_BigInteger_multiplyInteger8(thread, q, 10);
    Arcadia_BigInteger_addInteger8(thread, q, digit);
  }
  return Arcadia_BigInteger_toInteger64(thread, q);
}

static inline Arcadia_Integer64Value
Arcadia_NumberLiteral_approximateExponent
  (
    Arcadia_Thread* thread,
    const Arcadia_NumberLiteral* self,
    const char* p,
    Arcadia_BooleanValue* approximated
  )
{
  Arcadia_Integer64Value v = 0;
  Arcadia_SizeValue i, j;
  for (i = self->exponent.integral.start + self->exponent.integral.leadingZeroes.length, j = 0;
    i < self->exponent.integral.start + self->exponent.integral.leadingZeroes.length + self->exponent.integral.length && j < 19; ++i) {
    v = v * 10 + (p[i] - '0');
  }
  if (self->exponent.sign.length) {
    v = p[self->exponent.sign.start] == '-' ? -v : +v;
  }
  *approximated = i < self->exponent.integral.start + self->exponent.integral.leadingZeroes.length + self->exponent.integral.length;
  return v;
}

static inline Arcadia_Natural64Value
Arcadia_NumberLiteral_approximateSignificand
  (
    Arcadia_Thread* thread,
    const Arcadia_NumberLiteral* self,
    const char* p,
    Arcadia_Integer64Value* exponent,
    Arcadia_BooleanValue* approximated
  )
{
  Arcadia_SizeValue integralDigitsCount = self->significand.integral.length - self->significand.integral.leadingZeroes.length; // The total number of integral digits (excluding leading zeroes).
  Arcadia_SizeValue fractionalDigitsCount = self->significand.fractional.length - self->significand.fractional.trailingZeroes.length; // The total number of fractional digits (excluding trailing zeroes).

  // If there are more then 19 digits in the significand, then we truncate the significand and recompute the exponent accordingly.
  Arcadia_Natural64Value significand1 = 0;
  Arcadia_Integer32Value exponent1 = 0;
  Arcadia_SizeValue i, n;
  for (n = 0, i = 0; n < 19 && i < integralDigitsCount; ++i, ++n) {
    significand1 = significand1 * 10 + (p[self->significand.integral.start + self->significand.integral.leadingZeroes.length + i] - '0');
  }
  if (i < integralDigitsCount) {
    // If not all integral digits were consumed and there are k integral digits left, we basically divide the number by 10^k and increase the exponent by k and set approximate to true.
    *approximated = true;
    exponent1 += integralDigitsCount - i;
    *exponent = exponent1;
    return significand1;
  } else {
    for (i = 0; n < 19 && i < fractionalDigitsCount; ++i, ++n) {
      significand1 = significand1 * 10 + (p[self->significand.fractional.start + i] - '0');
    }
    exponent1 = exponent1 - i;
    // If not all fractional digits were consumed and there are k fractional digits left, we set approximate to true.
    *approximated = i < fractionalDigitsCount;
    *exponent = exponent1;
    return significand1;
  }
}

#define withClinger (true)
#define withLemire (true)
#define withGay (true)

static void
Approximation_fromNumeral
  (
    Arcadia_Thread* thread,
    Approximation* target,
    Arcadia_NumberLiteral const* source,
    const Arcadia_Natural8Value *p,
    Arcadia_SizeValue n
  )
{
  target->negative = source->significand.sign.length ? '-' == p[source->significand.sign.start] : Arcadia_BooleanValue_False;
  target->significand = Arcadia_NumberLiteral_approximateSignificand(thread, source, p, &target->shift, &target->significandTruncated);
  target->exponent = Arcadia_NumberLiteral_approximateExponent(thread, source, p, &target->exponentTruncated);
}

Arcadia_Real64Value
Arcadia_toReal64
  (
    Arcadia_Thread* thread,
    const Arcadia_Natural8Value *p,
    Arcadia_SizeValue n
  )
{
  State s;

  if (!Arcadia_parseNumberLiteral(thread, &s.literal, p, n)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }

  // We have the big integers "exponent := 10^(e-|w|)" and "significant := vw".
  Approximation_fromNumeral(thread, &s.a, &s.literal, p, n);

  Arcadia_ToReal64_Result result;
  // If the significand is zero, then the result is zero. The exponent does not matter anymore. [I]
  if (!s.a.significandTruncated && s.a.significand == Arcadia_Natural64Value_Literal(0)) {
    result.negative = s.a.negative;
    result.significand = Arcadia_Natural64Value_Literal(0);
    result.exponent = Arcadia_Integer32Value_Literal(0);
    result.failed = false;
    Arcadia_Real64Value v;
    Arcadia_ToReal64_BitsToValue(thread, &v, &result);
    return v;
  } else {
    result.failed = true;
     // Cannot handle this. Resort to Clinger's method.
  }
  /* ~~ Clinger's method ~~ */
  if (result.failed && withClinger) {
    Arcadia_clingerReal64(thread, &result, p, n, &s.a, &s.literal);
  }
  /* ~~ Lemire's method ~~*/
  if (result.failed && withLemire && !s.a.exponentTruncated) {
    assert(!s.a.exponentTruncated);
    Arcadia_lemireReal64(thread, &result, &s); // Actually, this never fails hence result.failed is always false.
    if (!result.failed && s.a.significandTruncated) {
      // Path to re-use Lemire's algorithm even if the number has too many digits in the significand a described in Lemire 2022.
      Arcadia_ToReal64_Result result1;
      State s1 = s;
      s1.a.significand += 1;
      Arcadia_lemireReal64(thread, &result1, &s1);
      if (!result1.failed) {
        if (result1.exponent != result.exponent || result1.negative != result.negative || result1.significand != result.significand) {
          result.failed = true;
          // Cannot handle this. Report to Gay's method.
        }
      }
    }
  }
  /* ~~ Gay's method ~~ */
  if (result.failed && withGay) {
    Arcadia_gayReal64(thread, &result, p, n, &s.a, &s.literal);
  }
  if (result.failed) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Real64Value v;
  Arcadia_ToReal64_BitsToValue(thread, &v, &result);
  return v;
}
