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

#if !defined(ARCADIA_RING1_BIGINTEGER_TONATURAL_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_TONATURAL_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

typedef struct Arcadia_BigInteger Arcadia_BigInteger;
typedef Arcadia_BigInteger* Arcadia_BigIntegerValue;

// Convert this BigInteger into a Natural16.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Natural16Value
Arcadia_BigInteger_toNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

// Convert this BigInteger into a Natural32.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Natural32Value
Arcadia_BigInteger_toNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

// Convert this BigInteger into a Natural64.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Natural64Value
Arcadia_BigInteger_toNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

// Convert this BigInteger intoa  Natural64.
// If the value is negative, conversion fails.
// If the magnitude is greater than what can be stored in a Natural64, then lower order bits are dropped and *truncated is set to true.
// Otherwise *truncated is set to false.
Arcadia_Natural64Value
Arcadia_BigInteger_toNatural64WithTruncation
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self,
    Arcadia_BooleanValue* truncated
  );

// Convert this BigInteger into a Natural8.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Natural8Value
Arcadia_BigInteger_toNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

#endif // ARCADIA_RING1_BIGINTEGER_TONATURAL_H_INCLUDED
