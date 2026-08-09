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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_INTEGER16_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_INTEGER16_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef int16_t Arcadia_Integer16Value;

#define Arcadia_Integer16Value_Minimum (INT16_MIN)

#define Arcadia_Integer16Value_Maximum (INT16_MAX)

#define Arcadia_Integer16Value_NumberOfBytes (sizeof(int16_t))
Arcadia_StaticAssert(Arcadia_Integer16Value_NumberOfBytes == 2, "Arcadia.Integer16Value.NumberOfBytes must be 2");

#define Arcadia_Integer16Value_NumberOfBits (Arcadia_Integer16Value_NumberOfBytes * 8)

#define Arcadia_Integer16Value_Literal(x) INT16_C(x)

Arcadia_declareScalarType(Arcadia_Integer16);

/// The minimum value of an Arcadia.Integer16Value is -32 768,
/// the maximum value of an Arcadia.Integer16Value is +32 767.
/// These are five decimal digits.
/// Hence the number of decimal digits which can be safely stored in an Arcadia.Integer16Value is 4.
#define Arcadia_Integer16Value_MaximumNumberOfDecimalDigits (4)

#endif // ARCADIA_RING1_IMPLEMENTATION_INTEGER16_H_INCLUDED
