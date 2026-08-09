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

#if !defined(R_INTEGER64VALUE_H_INCLUDED)
#define R_INTEGER64VALUE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef int64_t Arcadia_Integer64Value;

#define Arcadia_Integer64Value_Minimum (INT64_MIN)

#define Arcadia_Integer64Value_Maximum (INT64_MAX)

#define Arcadia_Integer64Value_NumberOfBytes (sizeof(int64_t))
Arcadia_StaticAssert(Arcadia_Integer64Value_NumberOfBytes == 8, "Arcadia.Integer64Value.NumberOfByes must be 8");

#define Arcadia_Integer64Value_NumberOfBits (Arcadia_Integer64Value_NumberOfBytes * 8)

#define Arcadia_Integer64Value_Literal(x) INT64_C(x)

Arcadia_declareScalarType(Arcadia_Integer64);

/// The minimum value of an Arcadia.Integer64Value is -9 223 372 036 854 775 808,
/// the maximum value of an Arcadia.Integer64Value is +9 223 372 036 854 775 807.
/// These are 19 decimal digits.
/// Hence the number of decimal digits which can be safely stored in an Arcadia.Integer64Value is 18.
#define Arcadia_Integer64Value_MaximumNumberOfDecimalDigits (18)

#endif // R_INTEGER64VALUE_H_INCLUDED
