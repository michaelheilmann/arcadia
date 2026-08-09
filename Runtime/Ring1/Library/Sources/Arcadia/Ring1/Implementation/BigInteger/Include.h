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

#if !defined(ARCADIA_RING1_BIGINTEGER_INCLUDE_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_INCLUDE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/BigInteger/add.h"
#include "Arcadia/Ring1/Implementation/BigInteger/and.h"
#include "Arcadia/Ring1/Implementation/BigInteger/compareTo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/compareByMagnitudeTo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/countSignificandBits.h"
#include "Arcadia/Ring1/Implementation/BigInteger/divide.h"
#include "Arcadia/Ring1/Implementation/BigInteger/equalTo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/fromTwosComplement.h"
#include "Arcadia/Ring1/Implementation/BigInteger/getBitLength.h"
#include "Arcadia/Ring1/Implementation/BigInteger/getHigh64.h"
#include "Arcadia/Ring1/Implementation/BigInteger/greaterThan.h"
#include "Arcadia/Ring1/Implementation/BigInteger/greaterThanOrEqualTo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/lowerThan.h"
#include "Arcadia/Ring1/Implementation/BigInteger/lowerThanOrEqualTo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/multiply.h"
#include "Arcadia/Ring1/Implementation/BigInteger/notEqualTo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/or.h"
#include "Arcadia/Ring1/Implementation/BigInteger/fromDecimalString.h"
#include "Arcadia/Ring1/Implementation/BigInteger/setInteger.h"
#include "Arcadia/Ring1/Implementation/BigInteger/setNatural.h"
#include "Arcadia/Ring1/Implementation/BigInteger/setPowerOfFive.h"
#include "Arcadia/Ring1/Implementation/BigInteger/setPowerOfTen.h"
#include "Arcadia/Ring1/Implementation/BigInteger/setPowerOfTwo.h"
#include "Arcadia/Ring1/Implementation/BigInteger/shiftLeft.h"
#include "Arcadia/Ring1/Implementation/BigInteger/shiftRight.h"
#include "Arcadia/Ring1/Implementation/BigInteger/subtract.h"
#include "Arcadia/Ring1/Implementation/BigInteger/toInteger.h"
#include "Arcadia/Ring1/Implementation/BigInteger/toNatural.h"
#include "Arcadia/Ring1/Implementation/BigInteger/toDecimalString.h"
#include "Arcadia/Ring1/Implementation/BigInteger/toTwosComplement.h"
#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/Void.h"

#include "Arcadia/Ring1/Implementation/BigInteger/BigInteger.h"

Arcadia_BigInteger*
Arcadia_BigInteger_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_BigInteger_swap
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  );

void
Arcadia_BigInteger_copy
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  );

void
Arcadia_BigInteger_setZero
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  );

Arcadia_BooleanValue
Arcadia_BigInteger_isZero
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  );

Arcadia_BooleanValue
Arcadia_BigInteger_isPositive
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  );

Arcadia_BooleanValue
Arcadia_BigInteger_isNegative
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  );

void
Arcadia_BigInteger_visit
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

void
Arcadia_BigInteger_ensureGray
  (
    Arcadia_Thread* thread,
    Arcadia_BigIntegerValue self
  );

#endif

/// @return A pointer to an "foreign value" type of name "Arcadia.ImmutableByteArray".
Arcadia_TypeValue
_Arcadia_BigIntegerValue_getType
  (
    Arcadia_Thread* thread
  );

void
Arcadia_BigInteger_toStdoutDebug
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  );

void
_Arcadia_BigInteger_stripLeadingZeroes
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger_Limp** limps,
    Arcadia_SizeValue* numberOfLimps
  );

#endif // ARCADIA_RING1_BIGINTEGER_INCLUDE_H_INCLUDED
