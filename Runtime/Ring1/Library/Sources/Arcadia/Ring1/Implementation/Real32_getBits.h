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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_REAL32_GETBITS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_REAL32_GETBITS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Real32.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_RealValue_getBits
Arcadia_Natural32Value
Arcadia_Real32Value_getBits
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value self
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_REAL32_GETBITS_H_INCLUDED
