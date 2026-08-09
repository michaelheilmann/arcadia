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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_SIZE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_SIZE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/_declareScalarType.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#if Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC
#include <stddef.h>
#endif
#include <stdint.h>

typedef size_t Arcadia_SizeValue;

#define Arcadia_SizeValue_Minimum ((size_t)0)

#define Arcadia_SizeValue_Maximum (SIZE_MAX)

#define Arcadia_SizeValue_NumberOfBytes (sizeof(size_t))
Arcadia_StaticAssert(Arcadia_SizeValue_NumberOfBytes == 4 || Arcadia_SizeValue_NumberOfBytes == 8, "Arcadia.SizeValue.NumberOfBytes must be one of 4 or 8");

#define Arcadia_SizeValue_NumberOfBits (Arcadia_SizeValue_NumberOfBytes * 8)

#define Arcadia_SizeValue_Literal(x) ((size_t)x)

Arcadia_declareScalarType(Arcadia_Size);

#endif // ARCADIA_RING1_IMPLEMENTATION_SIZE_H_INCLUDED
