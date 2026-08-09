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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NATURAL64_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NATURAL64_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef uint64_t Arcadia_Natural64Value;

#define Arcadia_Natural64Value_Minimum (UINT64_C(0))

#define Arcadia_Natural64Value_Maximum (UINT64_MAX)

#define Arcadia_Natural64Value_NumberOfBytes (sizeof(uint64_t))

Arcadia_StaticAssert(Arcadia_Natural64Value_NumberOfBytes == 8, "Arcadia.Natural64Value.NumberOfBytes must be 8");

#define Arcadia_Natural64Value_NumberOfBits (Arcadia_Natural64Value_NumberOfBytes * 8)

#define Arcadia_Natural64Value_Literal(x) UINT64_C(x)

Arcadia_declareScalarType(Arcadia_Natural64);

/// The maximum number of decimal digits which can safely be stored in an Arcadia_Natural64Value value.
/// The maximum value of an Arcadia_Natural64Value is Arcadia_Natural64Value_Maximum = 18 446 744 073 709 551 615.
/// These are 20 decimal digits.
/// Hence the number decimal digits which can safely be stored in an Arcadia_Natural64Value value is 19.
#define Arcadia_Natural64Value_MaximumNumberOfDecimalDigits (19)

/// The maximum number of hexadecimal digits which can safely be stored in an Arcadia_Natural64Value value.
/// The maximum value of an Arcadia_Natural64Value is Arcadia_Natural64Value_Maximum = ff ff ff ff ff ff ff ff.
/// These are 16 hexadecimal digits.
/// Hence the number hexadecimal digits which can safely be stored in an Arcadia_Natural64Value value is 16.
#define Arcadia_Natural64Value_MaximumNumberOfHexadecimalDigits (16)

#endif // ARCADIA_RING1_IMPLEMENTATION_NATURAL64_H_INCLUDED
