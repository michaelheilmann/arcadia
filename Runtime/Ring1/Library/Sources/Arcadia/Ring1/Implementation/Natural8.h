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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NATURAL8_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NATURAL8_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef uint8_t Arcadia_Natural8Value;

#define Arcadia_Natural8Value_Minimum (UINT8_C(0))

#define Arcadia_Natural8Value_Maximum (UINT8_MAX)

#define Arcadia_Natural8Value_NumberOfBytes (sizeof(uint8_t))

Arcadia_StaticAssert(Arcadia_Natural8Value_NumberOfBytes == 1, "Arcadia.Natural8Value.NumberOfBytes must be 1");

#define Arcadia_Natural8Value_NumberOfBits (Arcadia_Natural8Value_NumberOfBytes * 8)

#define Arcadia_Natural8Value_Literal(x) UINT8_C(x)

Arcadia_declareScalarType(Arcadia_Natural8);

/// The maximum number of decimal digits which can safely be stored in an Arcadia_Natural8Value value.
/// The maximum vaalue of an Arcadia.Natural8 value is Arcadia_Natural8Value_Maximum = 255.
/// These are three decimal digits.
/// Hence the number decimal digitis which can safely be stored in an Arcadia_Natural8Value value is 2.
#define Arcadia_Natural8Value_MaximumNumberOfDecimalDigits (2)

/// The maximum number of hexadecimal digits which can safely be stored in an Arcadia_Natural8Value value.
/// The maximum vaalue of an Arcadia.Natural8Value is Arcadia_Natural8Value_Maximum = ff.
/// These are two hexadecimal digits.
/// Hence the number of hexadecimal digits which can safely be stored in an Arcadia_Natural8Value is 2.
#define Arcadia_Natural8Value_MaximumNumberOfHexadecimalDigits (2)

#endif // ARCADIA_RING1_IMPLEMENTATION_NATURAL8_H_INCLUDED
