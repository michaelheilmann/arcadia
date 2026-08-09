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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NATURAL32_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NATURAL32_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef uint32_t Arcadia_Natural32Value;

#define Arcadia_Natural32Value_Minimum (UINT32_C(0))

#define Arcadia_Natural32Value_Maximum (UINT32_MAX)

#define Arcadia_Natural32Value_NumberOfBytes (sizeof(uint32_t))

Arcadia_StaticAssert(Arcadia_Natural32Value_NumberOfBytes == 4, "Arcadia.Natural32Value.NumberOfBytes must be 4");

#define Arcadia_Natural32Value_NumberOfBits (Arcadia_Natural32Value_NumberOfBytes * 8)

#define Arcadia_Natural32Value_Literal(x) UINT32_C(x)

Arcadia_declareScalarType(Arcadia_Natural32);

/// The maximum number of decimal digits which can safely be stored in an Arcadia_Natural32Value value.
/// The maximum value of an Arcadia_Natural32Value is Arcadia_Natural32Value_Maximum = 4 294 967 295.
/// These are 10 decimal digits.
/// Hence the number decimal digits which can safely be stored in an Arcadia_Natural16Value value is 9.
#define Arcadia_Natural32Value_MaximumNumberOfDecimalDigits (9)

/// The maximum number of hexadecimal digits which can safely be stored in an Arcadia_Natural32Value value.
/// The maximum value of an Arcadia_Natural32Value is Arcadia_Natural32Value_Maximum = ff ff ff ff.
/// These are 8 hexadecimal digits.
/// Hence the number hexadecimal digits which can safely be stored in an Arcadia_Natural16Value value is 8.
#define Arcadia_Natural32Value_MaximumNumberOfHexadecimalDigits (8)

#endif // ARCADIA_RING1_IMPLEMENTATION_NATURAL32_H_INCLUDED
