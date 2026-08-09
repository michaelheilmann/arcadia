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

#if !defined(ARCADIA_RING1_BIGINTEGER_GREATERTHAN_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_GREATERTHAN_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"
#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

typedef struct Arcadia_BigInteger Arcadia_BigInteger;
typedef Arcadia_BigInteger* Arcadia_BigIntegerValue;

// Get if this BigInteger is greater than another BigInteger.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThan
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  );

// Get if this BigInteger is greater than an Integer16.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer16Value other
  );

// Get if this BigInteger is greater than an Integer32.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer32Value other
  );

// Get if this BigInteger is greater than an Integer64.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer64Value other
  );

// Get if this BigInteger is greater than an Integer8.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer8Value other
  );

// Get if this BigInteger is greater than a Natural16.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  );

// Get if this BigInteger is greater than a Natural32.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  );

// Get if this BigInteger is greater than a Natural64.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural64Value other
  );

// Get if this BigInteger is greater than to a Natural8.
Arcadia_BooleanValue
Arcadia_BigInteger_isGreaterThanNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  );

#endif // ARCADIA_RING1_BIGINTEGER_GREATERTHAN_H_INCLUDED
