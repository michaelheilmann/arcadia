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

#include "Arcadia.Ring1.Tests.BigIntegerTests/And.h"

static void
testFixture
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value x,
    Arcadia_Integer64Value y,
    Arcadia_Integer64Value z
  )
{
  Arcadia_BigInteger* px = Arcadia_BigInteger_create(thread),
                    * py = Arcadia_BigInteger_create(thread),
                    * pz = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, px, x);
  Arcadia_BigInteger_setInteger64(thread, py, y);
  Arcadia_BigInteger_and3(thread, pz, px, py);
  Arcadia_Tests_assertTrue(thread, z == Arcadia_BigInteger_toInteger64(thread, pz));
}

void
Arcadia_Ring1_Tests_BigInteger_andOperations
  (
    Arcadia_Thread* thread
  )
{
#define Define(x,y, z) \
  testFixture(thread, Arcadia_Integer64Value_Literal(x), Arcadia_Integer64Value_Literal(y), Arcadia_Integer64Value_Literal(z));

  Define(0, 0, 0);
  Define(1, 0, 0);
  Define(0, 1, 0);
  Define(1, 1, 1);

  Define(2, 3, 2); // 2 ~ 10, 3 ~~ 11 => 2 & 3 ~ 2
  Define(3, 2, 2);

#undef Define
}
