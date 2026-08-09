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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_UNICODE_UNICODE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_UNICODE_UNICODE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"

#define Arcadia_Unicode_CodePoint_Last (Arcadia_Natural32Value_Literal(0x0010ffff))
Arcadia_StaticAssert(Arcadia_Unicode_CodePoint_Last <= Arcadia_Natural32Value_Maximum - 3, "<internal error>");

#endif // ARCADIA_RING1_IMPLEMENTATION_UNICODE_UNICODE_H_INCLUDED
