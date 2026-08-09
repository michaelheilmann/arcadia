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
#include "Arcadia/Ring1/Implementation/BigInteger/setNatural.h"

#include "Arcadia/Ring1/Include.h"

void
Arcadia_BigInteger_setNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!other) {
    Arcadia_BigInteger_setZero(thread, self);
    return;
  } else {
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * 1);
    self->limps[0] = other;
    self->numberOfLimps = 1;
    self->sign = 1;
  }
}

void
Arcadia_BigInteger_setNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!other) {
    Arcadia_BigInteger_setZero(thread, self);
    return;
  } else {
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * 1);
    self->limps[0] = other;
    self->numberOfLimps = 1;
    self->sign = 1;
  }
}

void
Arcadia_BigInteger_setNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural64Value other
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BigInteger_Limp hi = (other & 0xffffffff0000ffff) >> 32;
  Arcadia_BigInteger_Limp lo = (other & 0x00000000ffffffff) >> 0;
  if (hi) {
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * 2);
    self->limps[1] = hi;
    self->limps[0] = lo;
    self->numberOfLimps = 2;
  } else {
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * 1);
    self->limps[0] = lo;
    self->numberOfLimps = 1;
  }
  self->sign = other ? +1 : 0;
}

void
Arcadia_BigInteger_setNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!other) {
    Arcadia_BigInteger_setZero(thread, self);
  } else {
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * 1);
    self->limps[0] = other;
    self->numberOfLimps = 1;
    self->sign = 1;
  }
}
