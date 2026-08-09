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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/BigInteger/toNatural.h"

#include "Arcadia/Ring1/Include.h"

#define isZero(thread, self) ((self)->limps[0] == 0 && (self)->numberOfLimps == 1)

Arcadia_Natural16Value
Arcadia_BigInteger_toNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, self);
  if (v > Arcadia_Natural16Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural16Value)v;
}

Arcadia_Natural32Value
Arcadia_BigInteger_toNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, self);
  if (v > Arcadia_Natural32Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural32Value)v;
}

Arcadia_Natural64Value
Arcadia_BigInteger_toNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (isZero(thread, self)) {
    return Arcadia_Natural64Value_Literal(0);
  }
  if (self->numberOfLimps > 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Natural64Value v = 0;
  for (size_t i = self->numberOfLimps; i > 0; --i) {  // This loop is executed exactly one times or two times.
    v <<= 32;
    v += ((Arcadia_Natural64Value)self->limps[i - 1]);
  }

  if (self->sign == -1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  } else {
    return v;
  }
}

Arcadia_Natural64Value
Arcadia_BigInteger_toNatural64WithTruncation
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BooleanValue* truncated
  )
 {
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (isZero(thread, self)) {
    *truncated = Arcadia_BooleanValue_False;
    return Arcadia_Natural64Value_Literal(0);
  }
  if (self->sign == -1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue truncatedLimps = self->numberOfLimps > 2 ?  self->numberOfLimps - 2 : 0;
  *truncated = truncatedLimps;
  Arcadia_Natural64Value v = 0;
  for (size_t i = self->numberOfLimps; i > truncatedLimps; --i) {  // This loop is executed exactly one times or two times.
    v <<= 32;
    v += ((Arcadia_Natural64Value)self->limps[i - 1]);
  }
  if (self->sign == -1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  } else {
    return v;
  }
}

Arcadia_Natural8Value
Arcadia_BigInteger_toNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  Arcadia_Natural64Value v = Arcadia_BigInteger_toNatural64(thread, self);
  if (v > Arcadia_Natural8Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural8Value)v;
}
