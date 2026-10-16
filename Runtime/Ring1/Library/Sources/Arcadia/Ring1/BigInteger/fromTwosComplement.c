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
#include "Arcadia/Ring1/BigInteger/fromTwosComplement.h"

#include "Arcadia/Ring1/Include.h"

void
Arcadia_BigInteger_fromTwosComplement
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger_Limp const* limps,
    Arcadia_SizeValue numberOfLimps
  )
{
  if (!self || !limps || !numberOfLimps) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (numberOfLimps > Arcadia_BigInteger_MaximumNumberOfLimps) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BigInteger_Limp signBit = (Arcadia_BigInteger_Limp)((Arcadia_BigInteger_DoubleLimp)1 << (Arcadia_BigInteger_BitsPerLimp - 1));
  Arcadia_BooleanValue negative = (limps[numberOfLimps - 1] & signBit) != 0;
  Arcadia_BigInteger_Limp* magnitude = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps);
  Arcadia_BigInteger_DoubleLimp carry = 1;
  // Input is little endian, independently of the internal magnitude order.
  for (Arcadia_SizeValue i = 0; i < numberOfLimps; ++i) {
    if (negative) {
      Arcadia_BigInteger_DoubleLimp digit = (Arcadia_BigInteger_DoubleLimp)(Arcadia_BigInteger_Limp)~limps[i] + carry;
      magnitude[i] = (Arcadia_BigInteger_Limp)digit;
      carry = digit >> Arcadia_BigInteger_BitsPerLimp;
    } else {
      magnitude[i] = limps[i];
    }
  }
  while (numberOfLimps && !magnitude[numberOfLimps - 1]) --numberOfLimps;
#if Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_BigEndian
  for (Arcadia_SizeValue i = 0; i < numberOfLimps / 2; ++i) {
    Arcadia_BigInteger_Limp temporary = magnitude[i];
    magnitude[i] = magnitude[numberOfLimps - 1 - i];
    magnitude[numberOfLimps - 1 - i] = temporary;
  }
#elif Arcadia_Configuration_BigInteger_LimpOrder != Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian
  #error("unknown/unsupported limp order")
#endif
  // Publish after all reads, including when the input aliases self->limps.
  if (self->limps) Arcadia_Memory_deallocateUnmanaged(thread, self->limps);
  self->limps = magnitude;
  self->numberOfLimps = numberOfLimps;
  self->sign = numberOfLimps ? (negative ? -1 : +1) : 0;
}
