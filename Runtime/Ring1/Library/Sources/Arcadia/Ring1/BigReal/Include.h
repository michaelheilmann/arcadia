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

#if !defined(ARCADIA_RING1_BIGREAL_INCLUDE_H_INCLUDED)
#define ARCADIA_RING1_BIGREAL_INCLUDE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/BigInteger/Include.h"

typedef struct Arcadia_BigReal Arcadia_BigReal;

// Exact finite decimal: significand * 10^exponent.
// Both components are arbitrary-precision BigIntegers.
// Canonical zero has both components zero; a nonzero significand
// is not divisible by ten. Instances own their component objects.
struct Arcadia_BigReal {
  Arcadia_BigInteger* significand;
  Arcadia_BigInteger* exponent;
};

Arcadia_BigReal*
Arcadia_BigReal_create
  (
    Arcadia_Thread* thread
  );

// Accepts optional sign, decimal point and signed decimal exponent.
// Requires at least one significand digit and exponent digits if 'e' is present.
// Errors with ArgumentValueInvalid for malformed text.
void
Arcadia_BigReal_fromDecimalString
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

// Writes a compact exact scientific representation M[eE], without expanding E.
// Zero is represented as "0", nonzero values with exponent zero as "M".
Arcadia_RuntimeUTF8String*
Arcadia_BigReal_toScientificString
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self
  );

Arcadia_BooleanValue
Arcadia_BigReal_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  );

// Exact total ordering for finite decimal numbers.
// Returns -1, 0, or +1. Handles arbitrary-size exponents without
// materializing powers of ten or using floating-point approximations.
Arcadia_Integer8Value
Arcadia_BigReal_compareTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  );

Arcadia_BooleanValue
Arcadia_BigReal_isLowerThan
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  );

Arcadia_BooleanValue
Arcadia_BigReal_isLowerThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  );

Arcadia_BooleanValue
Arcadia_BigReal_isGreaterThan
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  );

Arcadia_BooleanValue
Arcadia_BigReal_isGreaterThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* self,
    Arcadia_BigReal* other
  );

// Exact multiplication; result may alias either operand.
void
Arcadia_BigReal_multiply3
  (
    Arcadia_Thread* thread,
    Arcadia_BigReal* result,
    Arcadia_BigReal* a,
    Arcadia_BigReal* b
  );

#endif // ARCADIA_RING1_BIGREAL_INCLUDE_H_INCLUDED
