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

#if !defined(ARCADIA_RING1_BIGINTEGER_AND_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_AND_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/BigInteger/BigInteger.h"

void
Arcadia_BigInteger_and3
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* result,
    Arcadia_BigInteger* a,
    Arcadia_BigInteger* b
  );

// Compute the bitwise and of this big integer and another big integer.
// Algorithm:
// Compute the minimal two's complement bit sequences representing the values of the big integers.
// If required, perform minimal sign extension the sequences such that they are of the same length.
// Compute the bitwise and sequence of the sequences.
// Assign the value of the bitwise and sequence to the result big integer.
void
Arcadia_BigInteger_and
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  );

#endif // ARCADIA_RING1_BIGINTEGER_AND_H_INCLUDED
