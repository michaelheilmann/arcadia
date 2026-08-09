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
#include "Arcadia/Ring1/Implementation/StringToReal/Clinger.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Result.h"

static const Arcadia_Real64Value PowersOfTenReal64Value[] = {
  1e0,    // 10^0
  1e1,    // 10^1
  1e2,    // 10^2
  1e3,    // 10^3
  1e4,    // 10^4
  1e5,    // 10^5
  1e6,    // 10^6
  1e7,    // 10^7
  1e8,    // 10^8
  1e9,    // 10^9
  1e10,   // 10^10
  1e11,   // 10^11
  1e12,   // 10^12
  1e13,   // 10^13
  1e14,   // 10^14
  1e15,   // 10^15
  1e16,   // 10^16
  1e17,   // 10^17
  1e18,   // 10^18
  1e19,   // 10^19
  1e20,   // 10^20
  1e21,   // 10^21
  1e22,   // 10^22
};

void
Arcadia_clingerReal64
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    const Arcadia_Natural8Value* p,
    Arcadia_SizeValue n,
    Approximation const* a,
    Arcadia_NumberLiteral const* numberLiteral
  )
{
  Arcadia_Integer32Value e = a->exponent + a->shift;
  // TODO: The logic below is most likely not correct as it does not take into account the "shift".
  // TODO: Provide a test case for this.
  const Arcadia_Integer64Value MaximalExponent = +22;
  const Arcadia_Integer64Value MinimalExponent = -22;
  const Arcadia_Integer64Value MaximalDecimalSignificandDigits = 17;

  if (a->significandTruncated || a->exponentTruncated) {
    result->failed = Arcadia_BooleanValue_True;
    return;
  }
  if (e < MinimalExponent || e > MaximalExponent) {
    result->failed = Arcadia_BooleanValue_True;
    return;
  }
  Arcadia_SizeValue l = 0;
  l += numberLiteral->significand.integral.length - numberLiteral->significand.integral.leadingZeroes.length; // The total number of integral digits (excluding leading zeroes).
  l += numberLiteral->significand.fractional.length - numberLiteral->significand.fractional.trailingZeroes.length; // The total number of fractional digits (excluding trailing zeroes).
  if (l > MaximalDecimalSignificandDigits) {
    // TODO: Determine if this check is sufficiently strict.
    result->failed = Arcadia_BooleanValue_True;
    return;
  }
  Arcadia_Real64Value v = (Arcadia_Real64Value)a->significand;
  if (e >= 0) {
    v *= PowersOfTenReal64Value[+e];
  } else {
    v /= PowersOfTenReal64Value[-e];
  }
  v = a->negative ? -v : +v;
  // Unfortunate but inevitable.
  // We do not return floats, rather we return their bit patterns.
  Arcadia_Natural64Value u;
  Arcadia_Memory_copy(thread, &u, &v, 8);
  result->negative = (Arcadia_Real64Value_SignBitsMask & u) >> Arcadia_Real64Value_SignBitsShift;
  result->exponent = (Arcadia_Real64Value_ExponentBitsMask & u) >> Arcadia_Real64Value_ExponentBitsShift;
  result->significand = (Arcadia_Real64Value_SignificandBitsMask & u) >> Arcadia_Real64Value_SignificandBitsShift;
  result->failed = Arcadia_BooleanValue_False;
}
