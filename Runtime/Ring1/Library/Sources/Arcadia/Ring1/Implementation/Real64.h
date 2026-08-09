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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_REAL64_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_REAL64_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <float.h>
#include <math.h>

typedef double Arcadia_Real64Value;

#define Arcadia_Real64Value_Minimum (-DBL_MAX)

#define Arcadia_Real64Value_Maximum (+DBL_MAX)

#define Arcadia_Real64Value_NumberOfBytes (sizeof(double))
Arcadia_StaticAssert(Arcadia_Real64Value_NumberOfBytes == 8, "R.Real64Value.NumberOfBytes must be 8");

// the number of bits of a double (https://en.wikipedia.org/wiki/Double-precision_floating-point_format)
#define Arcadia_Real64Value_NumberOfBits (Arcadia_Real64Value_NumberOfBytes * 8)

#define Arcadia_Real64Value_Literal(x) (x)

Arcadia_declareScalarType(Arcadia_Real64);

// TODO: Add to documentation.
#define Arcadia_Real64Value_NegativeInfinity (-INFINITY)

// TODO: Add to documentation.
#define Arcadia_Real64Value_PositiveInfinity (+INFINITY)

// The number of bits of the signifcand including the implicit 1 bit.
// Usually 53 = DBL_MANT_DIG.
// TODO: Add to documentation.
#define Arcadia_Real64Value_NumberOfSignificandBitsIncludingImplicit (DBL_MANT_DIG)
// The number of bits of the significand.
// Usually 52 = Arcadia_Real64Value_NumberOfSignificandBitsIncludingImplicit - 1.
// TODO: Add to documentation.
#define Arcadia_Real64Value_NumberOfExplicitSignificandBits (52)
// The shift of the bits of the significand.
// In other terms, if the 64 bits have indices from 0 to 63, then this is the index of the LSB of the significand.
// Usually 0.
// TODO: Add to documentation.
#define Arcadia_Real64Value_SignificandBitsShift (0)

// The number of bits of the exponent.
// Usually 11.
// TODO: Add to documentation.
#define Arcadia_Real64Value_NumberOfExponentBits (11)
// The shift of the bits of the exponent.
// In other terms, if the 64 bits have indices from 0 to 63, then this is the index of the LSB of the exponent.
// Usually 52.
// TODO: Add to documentation.
#define Arcadia_Real64Value_ExponentBitsShift Arcadia_Real64Value_NumberOfExplicitSignificandBits

// The number of bits of the sign.
// Usually 1.
// TODO: Add to documentation.
#define Arcadia_Real64Value_NumberOfSignBits (1)
// The shift of the bits of the sign.
// In other terms, if the 64 bits have indices from 0 to 63, then this is the index of the LSB of the sign.
// Usually 63.
// TODO: Add to documentation.
#define Arcadia_Real64Value_SignBitsShift (Arcadia_Real64Value_NumberOfExponentBits + Arcadia_Real64Value_NumberOfExplicitSignificandBits)

Arcadia_StaticAssert(Arcadia_Real64Value_NumberOfBits ==
                     Arcadia_Real64Value_NumberOfSignBits +
                     Arcadia_Real64Value_NumberOfExponentBits +
                     Arcadia_Real64Value_NumberOfExplicitSignificandBits, "unsupported floating-point format");

// The bits mask for the bits of the significand.
// TODO: Add to documentation.
#define Arcadia_Real64Value_SignificandBitsMask ((((UINT64_C(1) << Arcadia_Real64Value_NumberOfExplicitSignificandBits) - 1)) << Arcadia_Real64Value_SignificandBitsShift)
// The bits mask for the bits of the exponent.
// TODO: Add to documentation.
#define Arcadia_Real64Value_ExponentBitsMask (((UINT64_C(1) << Arcadia_Real64Value_NumberOfExponentBits) - 1) << Arcadia_Real64Value_ExponentBitsShift)
// The bits mask for the bits of the sign.
// TODO: Add to documentation.
#define Arcadia_Real64Value_SignBitsMask (((UINT64_C(1) << Arcadia_Real64Value_NumberOfSignBits) - 1) << Arcadia_Real64Value_SignBitsShift)

// The minimal unbiased exponent.
#define Arcadia_Real64Value_MinimalExponent (-1022)
// The maximal unbiased exponent.
#define Arcadia_Real64Value_MaximalExponent (+1023)
// The exponent bias.
#define Arcadia_Real64Value_ExponentBias (+1023)

#endif // ARCADIA_RING1_IMPLEMENTATION_REAL64_H_INCLUDED
