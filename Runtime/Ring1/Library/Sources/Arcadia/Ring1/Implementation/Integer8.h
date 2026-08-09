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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_INTEGER8_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_INTEGER8_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef int8_t Arcadia_Integer8Value;

#define Arcadia_Integer8Value_Minimum (INT8_MIN)

#define Arcadia_Integer8Value_Maximum (INT8_MAX)

#define Arcadia_Integer8Value_NumberOfBytes (sizeof(int8_t))
Arcadia_StaticAssert(Arcadia_Integer8Value_NumberOfBytes == 1, "Arcadia.Integer8Value.NumberOfBytes must be 1");

#define Arcadia_Integer8Value_NumberOfBits (Arcadia_Integer8Value_NumberOfBytes * 8)

#define Arcadia_Integer8Value_Literal(x) INT8_C(x)

Arcadia_declareScalarType(Arcadia_Integer8);

/// The minimum value of an Arcadia.Integer8Value is -128,
/// the maximum value of an Arcadia.Integer8Value is +127.
/// These are three decimal digits.
/// Hence the number of decimal digits which can be safely stored in an Arcadia.Integer8Value is 2.
#define Arcadia_Integer8Value_MaximumNumberOfDecimalDigits (2)

#endif // ARCADIA_RING1_IMPLEMENTATION_INTEGER8_H_INCLUDED
