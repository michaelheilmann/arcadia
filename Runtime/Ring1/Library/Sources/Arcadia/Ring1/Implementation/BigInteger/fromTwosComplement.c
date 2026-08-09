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
Arcadia_BigInteger_fromTwosComplement
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger_Limp const* limps,
    Arcadia_SizeValue numberOfLimps
  )
{
  // Currently assumes and only supports little endian.
  Arcadia_BigInteger_Limp mostSignificandLimp = limps[numberOfLimps - 1];

  // If the most significand limp is zero, then the number is positive or zero.
  // We strip of trailing zero limps.
  if (mostSignificandLimp == 0) {
    while (numberOfLimps > 1 && limps[numberOfLimps - 1] == 0) {
      numberOfLimps--;
    }
    mostSignificandLimp = limps[numberOfLimps - 1];
    if (mostSignificandLimp == 0) {
      // The number is zero.
      Arcadia_BigInteger_setZero(thread, self);
    } else {
      // We can copy the limps.
      Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->limps, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps);
      #if Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_BigEndian
        Arcadia_Process_copyMemory(Arcadia_Thread_getProcess(thread), self->limps, limps, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps);
        #if Arcadia_Configuration_BigInteger_LimpSize == 1
          Arcadia_Process_reverseMemory8(Arcadia_Thread_getProcess(thread), self->limps, numberOfLimps);
        #elif Arcadia_Configuration_BigInteger_LimpSize == 2
          Arcadia_Process_reverseMemory16(Arcadia_Thread_getProcess(thread), self->limps, numberOfLimps);
        #elif Arcadia_Configuration_BigInteger_LimpSize == 4
          Arcadia_Process_reverseMemory32(Arcadia_Thread_getProcess(thread), self->limps, numberOfLimps);
        #else
          #error("unknown/unsupported limp size")
        #endif
      #elif Arcadia_Configuration_BigInteger_LimpOrder == Arcadia_Configuration_BigInteger_LimpOrder_LittleEndian
        Arcadia_Memory_copy(thread, self->limps, limps, sizeof(Arcadia_BigInteger_Limp) * numberOfLimps);
      #else
        #error("unknown/unsupported limp order")
      #endif
      self->numberOfLimps = numberOfLimps;
      self->sign = 1;
    }
  } else {
    Arcadia_SizeValue n = numberOfLimps;
    Arcadia_BigInteger_Limp* p = Arcadia_Memory_allocateUnmanaged(thread, sizeof(Arcadia_BigInteger_Limp) * n);
    Arcadia_Memory_copy(thread, p, limps, sizeof(Arcadia_BigInteger_Limp) * n);
    // The value is positive or negative.
    Arcadia_BooleanValue isNegative = mostSignificandLimp & (1 << (Arcadia_BigInteger_BitsPerLimp - 1));
    if (isNegative) {
      Arcadia_SizeValue i = 0;
      // copy the zero limps
      while (p[i] == 0) { // This is fine as there must be a at least one non-zero limp.
        i++;
      }
      p[i] = -(Arcadia_Integer32Value)p[i];
      i++;
      while (i < n) {
        p[i] = ~(p[i]);
      }
      if (p[n - 1] & (1 << (Arcadia_BigInteger_BitsPerLimp - 1)) == 0) {
        n++;
        Arcadia_Memory_reallocateUnmanaged(thread, (void**)&p, sizeof(Arcadia_BigInteger_Limp) * n);
        p[n - 1] = Arcadia_BigInteger_Limp_Maximum;
      }
    }
    self->sign = isNegative ? -1 : +1;
    self->limps = p;
    self->numberOfLimps = n;
  }
}
