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
#include "Arcadia/Ring1/Implementation/BigInteger/toTwosComplement.h"

#include "Arcadia/Ring1/Include.h"
#include <assert.h>

void
Arcadia_BigInteger_toTwosComplement
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger_Limp** limps,
    Arcadia_SizeValue* numberOfLimps
  )
{
  // How to convert a sign magnitude representation consisting of the sign bit and the magnitude bits to its corresponding two's complement form.
  // [1] the sign magnitude representation is zero. Its two's complement representation is zero.
  // [2] the sign magnitude representation is positive. Its two's complement representation is the bits of the magnitude.
  //     If the MSB is 1, add a new MSB with 0.
  // [3] the sign magnitude representation is negative. Its two's complementation representation is computed by
  //     negating the magnitude bits and adding one to them. If the MSB is 0, add a new MSB with 1. This shortcut allows
  //     to convert a number to its two's complement without first forming its ones' complement.

  // Obviously, [1] and [2] can be implemented using word operations.
  // [3] requires a little more thought:
  // Given words x = a[0] ... a[k - 1], a[k], a[k+1] ..., a[k + n - 1]
  // where a[0] to a[k - 1] are all zeroes, we can simply copy those.
  // a[k] is the first word which contains the first one bit. We can
  // simply to the per-word to two's complement conversion by inverting
  // and adding 1 on that word. In C, this can be done by casting this
  // to signed and negating then casting back to unsigned. a[k+1] ...
  // a[k + n - 1] are simply inverted and copied.

  if (self->sign == 0) {
    // case [1]
    Arcadia_SizeValue numberOfLimps0 = 1;
    Arcadia_BigInteger_Limp* limps0 = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
    limps0[0] = 0;
    *limps = limps0;
    *numberOfLimps = numberOfLimps0;
  } else if (self->sign == 1) {
    // case [2]
    Arcadia_SizeValue numberOfLimps0 = self->numberOfLimps;
    Arcadia_BigInteger_Limp* limps0 = NULL;
    limps0 = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
  #if Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian
    Arcadia_Memory_copy(thread, limps0, self->limps, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
  #elif Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_BigEndian
    Arcadia_Memory_copy(thread, limps0, self->limps, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
    for (Arcadia_SizeValue i = 0; i < numberOfLimps0 / 2; ++i) {
      Arcadia_BigInteger_Limp temporary = limps0[numberOfLimps0 - 1 - i];
      limps0[numberOfLimps0 - 1 - i] = limps0[i];
      limps0[i] = temporary;
    }
  #else
    #error("unknown/unsupported limp order")
  #endif
    if (limps0[numberOfLimps0 - 1] & (1 << (Arcadia_BigInteger_BitsPerLimp - 1)) == 1) {
      numberOfLimps0++;
      Arcadia_Memory_reallocateUnmanaged(thread, (void**)&limps0, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
      limps0[numberOfLimps0 - 1] = 0;
    }
    *limps = limps0;
    *numberOfLimps = numberOfLimps0;
  } else /*if (self->sign == -1)*/ {
    // case [3]
    Arcadia_SizeValue numberOfLimps0 = self->numberOfLimps;
    Arcadia_BigInteger_Limp* limps0 = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
    Arcadia_Memory_copy(thread, limps0, self->limps, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
  #if Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian
  #elif Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_BigEndian
    for (Arcadia_SizeValue i = 0; i < numberOfLimps0 / 2; ++i) {
      Arcadia_BigInteger_Limp temporary = limps0[numberOfLimps0 - 1 - i];
      limps0[numberOfLimps0 - 1 - i] = limps0[i];
      limps0[i] = temporary;
    }
  #else
    #error("unknown/unsupported limp order")
  #endif
  #if Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian
    Arcadia_SizeValue i = 0;
    // copy the zero limps
    while (limps0[i] == 0) { // This is fine as there must be a at least one non-zero limp.
     // limps0[i] = self->limps0[i];
      i++;
    }
    limps0[i] = -(Arcadia_Integer32Value)limps0[i];
    i++;
    while (i < numberOfLimps0) {
      limps0[i] = ~(limps0[i]);
    }
    if (limps0[numberOfLimps0 - 1] & (1 << (Arcadia_BigInteger_BitsPerLimp - 1)) == 0) {
      numberOfLimps0++;
      Arcadia_Memory_reallocateUnmanaged(thread, (void**)&limps0, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
      limps0[numberOfLimps0 - 1] = Arcadia_BigInteger_Limp_Maximum;
    }
  #elif Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_BigEndian
    Arcadia_SizeValue i = numberOf;
    // copy the zero limps
    while (limps0[i] == 0) { // This is fine as there must be a at least one non-zero limp.
     // limps0[i] = self->limps0[i];
      i++;
    }
    limps0[i] = -(Arcadia_Integer32Value)limps0[i];
    i++;
    while (i < numberOfLimps0) {
      limps0[i] = ~(limps0[i]);
    }
    if (limps0[numberOfLimps0 - 1] & (1 << (Arcadia_BigInteger_BitsPerLimp - 1)) == 0) {
      numberOfLimps0++;
      Arcadia_Process_reallocateUnmanaged(Arcadia_Thread_getProcess(thread), &limps0, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps0);
      limps0[numberOfLimps0 - 1] = Arcadia_BigInteger_Limp_Maximum;
    }
  #else
    #error("unknown/unsupported limp order")
  #endif
    *limps = limps0;
    *numberOfLimps = numberOfLimps0;
  }
}
