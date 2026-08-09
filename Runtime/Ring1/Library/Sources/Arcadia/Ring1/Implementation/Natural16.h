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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NATURAL16_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NATURAL16_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef uint16_t Arcadia_Natural16Value;

#define Arcadia_Natural16Value_Minimum (UINT16_C(0))

#define Arcadia_Natural16Value_Maximum (UINT16_MAX)

#define Arcadia_Natural16Value_NumberOfBytes (sizeof(uint16_t))

Arcadia_StaticAssert(Arcadia_Natural16Value_NumberOfBytes == 2, "Arcadia.Natural16Value.NumberOfBytes must be 2");

#define Arcadia_Natural16Value_NumberOfBits (Arcadia_Natural16Value_NumberOfBytes * 8)

#define Arcadia_Natural16Value_Literal(x) UINT16_C(x)

Arcadia_declareScalarType(Arcadia_Natural16);

// The maximum number of decimal digits which can safely be stored in an Arcadia_Natural16Value value.
// The maximum value of an Arcadia_Natural16Value value is Arcadia_Natural16Value_Maximum = 65 535.
// These are 5 decimal digits.
// Hence the number decimal digitis which can safely be stored in an Arcadia_Natural16Value value is 4.
#define Arcadia_Natural16Value_MaximumNumberOfDecimalDigits (4)

// The maximum number of hexadecimal digits which can safely be stored in an Arcadia_Natural16Value value.
// The maximum value of an Arcadia_Natural16Value is Arcadia_Natural16Value_Maximum = ff ff.
// These are 4 hexadecimal digits.
// Hence the number hexadecimal digits which can safely be stored in an Arcadia_Natural16Value value is 4.
#define Arcadia_Natural16Value_MaximumNumberOfHexadecimalDigits (4)

#endif // ARCADIA_RING1_IMPLEMENTATION_NATURAL16_H_INCLUDED
