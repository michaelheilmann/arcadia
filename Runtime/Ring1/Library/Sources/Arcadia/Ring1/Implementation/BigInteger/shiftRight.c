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
#include "Arcadia/Ring1/Implementation/BigInteger/shiftRight.h"

#include "Arcadia/Ring1/Include.h"

void
Arcadia_BigInteger_shiftRightInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer16Value other
  )
{ Arcadia_BigInteger_shiftRightInteger64(thread, self, other); }

void
Arcadia_BigInteger_shiftRightInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer32Value other
  )
{ Arcadia_BigInteger_shiftRightInteger64(thread, self, other); }

void
Arcadia_BigInteger_shiftRightInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer64Value other
  )
{
  if (other < Arcadia_Integer64Value_Literal(0)) {
    if (other == Arcadia_Integer64Value_Minimum) {
      Arcadia_BigInteger_shiftLeftNatural64(thread, self, ((Arcadia_Natural64Value)(-(other + 1))) + 1);
    } else {
      Arcadia_BigInteger_shiftLeftNatural64(thread, self, (Arcadia_Natural64Value)(-other));
    }
  } else {
    Arcadia_BigInteger_shiftRightNatural64(thread, self, (Arcadia_Natural64Value)other);
  }
}

void
Arcadia_BigInteger_shiftRightInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Integer8Value other
  )
{ Arcadia_BigInteger_shiftRightInteger64(thread, self, other); }

void
Arcadia_BigInteger_shiftRightNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{ Arcadia_BigInteger_shiftRightNatural64(thread, self, other); }

void
Arcadia_BigInteger_shiftRightNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{ Arcadia_BigInteger_shiftRightNatural64(thread, self, other); }

void
Arcadia_BigInteger_shiftRightNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural64Value other
  )
{
  if (other == 0) {
    return;
  }
  if (Arcadia_BigInteger_isZero(thread, self)) {
    return;
  }
  Arcadia_SizeValue limpShift = other / Arcadia_BigInteger_BitsPerLimp;
  Arcadia_SizeValue bitShift = other % Arcadia_BigInteger_BitsPerLimp;

  if (limpShift >= self->numberOfLimps) {
    // Special case: all the contents are shift to out to the right.
    Arcadia_BigInteger_setZero(thread, self);
    return;
  }

  Arcadia_SizeValue sourceIndex = limpShift;
  Arcadia_SizeValue targetIndex = 0;

  if (!bitShift) {
    // If the shift is limp-aligned.
    while (sourceIndex < self->numberOfLimps) {
      self->limps[targetIndex] = self->limps[sourceIndex];
      targetIndex++;
      sourceIndex++;
    }
    self->numberOfLimps -= limpShift;
  } else {
    // If the shift is not limp-aligned.
    while (sourceIndex < self->numberOfLimps - 1) {
      self->limps[targetIndex] = self->limps[sourceIndex];
      self->limps[targetIndex] >>= bitShift;
      self->limps[targetIndex] |= self->limps[sourceIndex + 1] << (Arcadia_BigInteger_BitsPerLimp - bitShift);
      targetIndex++;
      sourceIndex++;
    }
    self->limps[targetIndex] = self->limps[sourceIndex];
    self->limps[targetIndex] >>= bitShift;

    self->numberOfLimps -= limpShift;

    _Arcadia_BigInteger_stripLeadingZeroes(thread, &self->limps, &self->numberOfLimps);
    if (self->numberOfLimps == 0) self->sign = 0;
  }
}

void
Arcadia_BigInteger_shiftRightNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{ Arcadia_BigInteger_shiftRightNatural64(thread, self, other); }
