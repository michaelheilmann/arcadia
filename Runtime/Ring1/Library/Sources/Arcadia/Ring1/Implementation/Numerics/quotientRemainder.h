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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NUMERICS_QUOTIENTREMAINDER_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NUMERICS_QUOTIENTREMAINDER_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"

#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

#include "Arcadia/Ring1/Implementation/Size.h"

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value dividend,
    Arcadia_Integer16Value divisor,
    Arcadia_Integer16Value* quotient,
    Arcadia_Integer16Value* remainder
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderIntegerl32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value dividend,
    Arcadia_Integer32Value divisor,
    Arcadia_Integer32Value* quotient,
    Arcadia_Integer32Value* remainder
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value dividend,
    Arcadia_Integer64Value divisor,
    Arcadia_Integer64Value* quotient,
    Arcadia_Integer64Value* remainder
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value dividend,
    Arcadia_Integer8Value divisor,
    Arcadia_Integer8Value* quotient,
    Arcadia_Integer8Value* remainder
  );



// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value dividend,
    Arcadia_Natural16Value divisor,
    Arcadia_Natural16Value* quotient,
    Arcadia_Natural16Value* remainder
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value dividend,
    Arcadia_Natural32Value divisor,
    Arcadia_Natural32Value* quotient,
    Arcadia_Natural32Value* remainder
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value dividend,
    Arcadia_Natural64Value divisor,
    Arcadia_Natural64Value* quotient,
    Arcadia_Natural64Value* remainder
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_quotientRemainder*
void
Arcadia_quotientRemainderNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value dividend,
    Arcadia_Natural8Value divisor,
    Arcadia_Natural8Value *quotient,
    Arcadia_Natural8Value* remainder
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_NUMERICS_QUOTIENTREMAINDER_H_INCLUDED
