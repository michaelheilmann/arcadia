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

#if !defined(ARCADIA_RING1_BIGINTEGER_TOINTEGER_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_TOINTEGER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"

typedef struct Arcadia_BigInteger Arcadia_BigInteger;
typedef Arcadia_BigInteger* Arcadia_BigIntegerValue;

// Convert this BigInteger into a Integer16.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Integer16Value
Arcadia_BigInteger_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

// Convert this BigInteger into a Integer32.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Integer32Value
Arcadia_BigInteger_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

// Convert this BigInteger into a Integer64.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Integer64Value
Arcadia_BigInteger_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

// Convert this BigInteger into a Integer8.
// Raise an Arcadia_Status_ConversionFailed error if the conversion fails.
Arcadia_Integer8Value
Arcadia_BigInteger_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

#endif // ARCADIA_RING1_BIGINTEGER_TOINTEGER_H_INCLUDED
