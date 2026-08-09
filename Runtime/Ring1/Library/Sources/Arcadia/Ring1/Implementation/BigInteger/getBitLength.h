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

#if !defined(ARCADIA_RING1_BIGINTEGER_GETBITLENGTH_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_GETBITLENGTH_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Size.h"

typedef struct Arcadia_BigInteger Arcadia_BigInteger;
typedef Arcadia_BigInteger* Arcadia_BigIntegerValue;

// Get the length, in Bits, of a big integer.
// The length, in Bits, of a big integer is given by
// n * l - m
// where
// - n is the number of limps
// - m is the number of leading zeroes of the most significand limp
// - l is the number of Bits per limp
Arcadia_SizeValue
Arcadia_BigInteger_getBitLength
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  );

#endif // ARCADIA_RING1_BIGINTEGER_GETBITLENGTH_H_INCLUDED
