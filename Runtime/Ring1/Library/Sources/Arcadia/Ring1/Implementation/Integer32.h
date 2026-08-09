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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_INTEGER32_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_INTEGER32_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include <stdint.h>

typedef int32_t Arcadia_Integer32Value;

#define Arcadia_Integer32Value_Minimum (INT32_MIN)

#define Arcadia_Integer32Value_Maximum (INT32_MAX)

#define Arcadia_Integer32Value_NumberOfBytes (sizeof(int32_t))
Arcadia_StaticAssert(Arcadia_Integer32Value_NumberOfBytes == 4, "Arcadia.Integer32Value.NumberOfBytes must be 4");

#define Arcadia_Integer32Value_NumberOfBits (Arcadia_Integer32Value_NumberOfBytes * 8)

#define Arcadia_Integer32Value_Literal(x) INT32_C(x)

Arcadia_declareScalarType(Arcadia_Integer32);

/// The minimum value of an Arcadia.Integer32Value is -2 147 483 648,
/// the maximum value of an Arcadia.Integer32Value is +2 147 483 647.
/// These are 10 decimal digits.
/// Hence the number of decimal digits which can be safely stored in an Arcadia.Integer32Value is 9.
#define Arcadia_Integer32Value_MaximumNumberOfDecimalDigits (9)

#endif // ARCADIA_RING1_IMPLEMENTATION_INTEGER32_H_INCLUDED
