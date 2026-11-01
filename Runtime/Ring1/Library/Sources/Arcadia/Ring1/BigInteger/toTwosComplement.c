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
#include "Arcadia/Ring1/BigInteger/toTwosComplement.h"

#include "Arcadia/Ring1/Include.h"

static Arcadia_BigInteger_Limp
magnitudeLimp
  (
    Arcadia_BigInteger* self,
    Arcadia_SizeValue index
  )
{
#if Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian
  return self->limps[index];
#elif Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_BigEndian
  return self->limps[self->numberOfLimps - 1 - index];
#else
  #error("unknown/unsupported limp order")
#endif
}

void
Arcadia_BigInteger_toTwosComplement
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger_Limp** limps,
    Arcadia_SizeValue* numberOfLimps
  )
{
  if (!self || !limps || !numberOfLimps) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_BigInteger_isZero(thread, self)) {
    Arcadia_BigInteger_Limp* result = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp));
    result[0] = 0;
    *limps = result;
    *numberOfLimps = 1;
    return;
  }

  Arcadia_SizeValue size = self->numberOfLimps;
  Arcadia_BooleanValue negative = self->sign < 0;
  Arcadia_BigInteger_Limp signBit = (Arcadia_BigInteger_Limp)((Arcadia_BigInteger_DoubleLimp)1 << (Arcadia_BigInteger_BitsPerLimp - 1));
  Arcadia_BigInteger_Limp high = magnitudeLimp(self, size - 1);
  if (negative) {
    // Carry reaches the highest limb only when every lower magnitude limb is 0.
    Arcadia_BooleanValue carry = true;
    for (Arcadia_SizeValue i = 0; i + 1 < size; ++i) {
      if (magnitudeLimp(self, i)) {
        carry = false;
        break;
      }
    }
    high = carry ? (Arcadia_BigInteger_Limp)(0u - high) : (Arcadia_BigInteger_Limp)~high;
  }
  Arcadia_BooleanValue extend = negative ? (high & signBit) == 0 : (high & signBit) != 0;
  if (size > Arcadia_BigInteger_MaximumNumberOfLimps - (Arcadia_SizeValue)extend) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  // Compute the size first: no reallocating an unowned temporary on failure.
  Arcadia_BigInteger_Limp* result = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * (size + extend));
  Arcadia_BigInteger_DoubleLimp carry = 1;
  for (Arcadia_SizeValue i = 0; i < size; ++i) {
    Arcadia_BigInteger_Limp input = magnitudeLimp(self, i);
    if (negative) {
      Arcadia_BigInteger_DoubleLimp digit = (Arcadia_BigInteger_DoubleLimp)(Arcadia_BigInteger_Limp)~input + carry;
      result[i] = (Arcadia_BigInteger_Limp)digit;
      carry = digit >> Arcadia_BigInteger_BitsPerLimp;
    } else {
      result[i] = input;
    }
  }
  if (extend) result[size] = negative ? Arcadia_BigInteger_Limp_Maximum : 0;
  *limps = result;
  *numberOfLimps = size + extend;
}
