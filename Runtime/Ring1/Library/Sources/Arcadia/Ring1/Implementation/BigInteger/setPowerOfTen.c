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
#include "Arcadia/Ring1/Implementation/BigInteger/setPowerOfTen.h"

#include "Arcadia/Ring1/Include.h"

static const uint32_t PowersOfTenTable[] = {
  1, // 10^0
  10, // 10^1
  100, // 10^2
  1000, // 10^3
  10000, // 10^4
  100000, // 10^5
  1000000, // 10^6
  10000000, // 10^7
  100000000, // 10^8
  1000000000, // 10^9 (the next power of ten 10^10 is not representable as uint32_t anymore)
  /*10000000000,*/ // 10^10
};

Arcadia_StaticAssert(UINT32_MAX >= 1000000000, "<error>");
#define PowersOfTenTableSize (sizeof(PowersOfTenTable) / sizeof(uint32_t))

void
Arcadia_BigInteger_setPowerOfTenNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{ Arcadia_BigInteger_setPowerOfTenNatural64(thread, self, other); }

void
Arcadia_BigInteger_setPowerOfTenNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{ Arcadia_BigInteger_setPowerOfTenNatural64(thread, self, other); }

void
Arcadia_BigInteger_setPowerOfTenNatural64
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
  Arcadia_BigInteger* a = Arcadia_BigInteger_create(thread),
                    * b = Arcadia_BigInteger_create(thread);
  Arcadia_StaticAssert(10 == PowersOfTenTableSize, "<internal error>");
  Arcadia_BigInteger_setNatural64(thread, a, Arcadia_Natural64Value_Literal(1));
  if (other > 9) {
    do {
      other -= 9;
      Arcadia_BigInteger_setNatural32(thread, b, PowersOfTenTable[9]);
      Arcadia_BigInteger_multiply3(thread, a, a, b);
    } while (other > 9);
  }
  if (other) {
    Arcadia_BigInteger_setNatural32(thread, b, PowersOfTenTable[other]);
    Arcadia_BigInteger_multiply3(thread, a, a, b);
  }
  Arcadia_BigInteger_swap(thread, self, a);
}

void
Arcadia_BigInteger_setPowerOfTenNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{ Arcadia_BigInteger_setPowerOfTenNatural64(thread, self, other); }
