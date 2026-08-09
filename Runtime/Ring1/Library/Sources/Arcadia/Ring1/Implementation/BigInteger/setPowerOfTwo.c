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
#include "Arcadia/Ring1/Implementation/BigInteger/setPowerOfTwo.h"

#include "Arcadia/Ring1/Include.h"

static const uint32_t PowersOfTwoTable[] = {
  1, // 2^0
  2, // 2^1
  4, // 2^2
  8, // 2^3
  16, // 2^4
  32, // 2^5
  64, // 2^6
  128, // 2^7
  256, // 2^8
  512, // 2^9
  1024, // 2^10
  2048, // 2^11
  4096, // 2^12
  8192, // 2^13
  16384, // 2^14
  32768, // 2^15
  65536, // 2^16
  131072, // 2^17
  262144, // 2^18
  524288, // 2^19
  1048576, // 2^20
  2097152, // 2^21
  4194304, // 2^22
  8388608, // 2^23
  16777216, // 2^24
  33554432, // 2^25
  67108864, // 2^26
  134217728, // 2^27
  268435456, // 2^28
  536870912, // 2^29
  1073741824, // 2^30
  2147483648, // 2^31 (the next power of ten 2^32 is not representable as uint32_t anymore)
  /*4294967296,*/ // 2^32
};

Arcadia_StaticAssert(UINT32_MAX >= 2147483648, "<error>");
#define PowersOfTwoTableSize (sizeof(PowersOfTwoTable) / sizeof(uint32_t))

void
Arcadia_BigInteger_setPowerOfTwoNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural16Value other
  )
{ Arcadia_BigInteger_setPowerOfTwoNatural64(thread, self, other); }

void
Arcadia_BigInteger_setPowerOfTwoNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural32Value other
  )
{ Arcadia_BigInteger_setPowerOfTwoNatural64(thread, self, other); }

void
Arcadia_BigInteger_setPowerOfTwoNatural64
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
  Arcadia_StaticAssert(32 == PowersOfTwoTableSize, "<internal error>");
  Arcadia_BigInteger_setNatural64(thread, a, Arcadia_Natural64Value_Literal(1));
  if (other > 31) {
    do {
      other -= 31;
      Arcadia_BigInteger_setNatural32(thread, b, PowersOfTwoTable[31]);
      Arcadia_BigInteger_multiply3(thread, a, a, b);
    } while (other > 31);
  }
  if (other) {
    Arcadia_BigInteger_setNatural32(thread, b, PowersOfTwoTable[other]);
    Arcadia_BigInteger_multiply3(thread, a, a, b);
  }
  Arcadia_BigInteger_swap(thread, self, a);
}

void
Arcadia_BigInteger_setPowerOfTwoNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_Natural8Value other
  )
{ Arcadia_BigInteger_setPowerOfTwoNatural64(thread, self, other); }
