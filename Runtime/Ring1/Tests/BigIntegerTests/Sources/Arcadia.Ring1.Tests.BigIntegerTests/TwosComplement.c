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

#include "Arcadia.Ring1.Tests.BigIntegerTests/TwosComplement.h"

static void
test
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value value
  )
{
  Arcadia_BigInteger* x = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* y = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setInteger64(thread, x, value);
  Arcadia_BigInteger_Limp* p = NULL;
  Arcadia_SizeValue n;
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    Arcadia_BigInteger_toTwosComplement(thread, x, &p, &n);
    Arcadia_BigInteger_fromTwosComplement(thread, y, p, n);
    Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, x, y));
    Arcadia_Memory_deallocateUnmanaged(thread, p);
    p = NULL;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (p) {
      Arcadia_Memory_deallocateUnmanaged(thread, p);
      p = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Ring1_Tests_BigInteger_twosComplementOperations
  (
    Arcadia_Thread* thread
  )
{
  test(thread, 0);
  test(thread, -7);
  test(thread, +7);
  test(thread, Arcadia_Integer64Value_Minimum);
  test(thread, Arcadia_Integer64Value_Maximum);
}
