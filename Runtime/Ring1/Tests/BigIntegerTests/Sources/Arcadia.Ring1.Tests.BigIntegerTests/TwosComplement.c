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
  Arcadia_BigInteger_Limp* volatile owned = NULL;
  Arcadia_SizeValue n;
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    Arcadia_BigInteger_Limp* p = NULL;
    Arcadia_BigInteger_toTwosComplement(thread, x, &p, &n);
    owned = p;
    Arcadia_BigInteger_DoubleLimp signBit = (Arcadia_BigInteger_DoubleLimp)1 << (Arcadia_BigInteger_BitsPerLimp - 1);
    Arcadia_Tests_assertTrue(thread, n > 0);
    Arcadia_Tests_assertTrue(thread, ((p[n - 1] & signBit) != 0) == (value < 0));
    Arcadia_BigInteger_fromTwosComplement(thread, y, p, n);
    Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, x, y));
    Arcadia_Memory_deallocateUnmanaged(thread, p);
    owned = NULL;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (owned) {
      Arcadia_Memory_deallocateUnmanaged(thread, owned);
      owned = NULL;
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
  const Arcadia_Integer64Value values[] = {
    0, -1, 1, -7, 7, INT32_MIN, INT32_MAX,
    INT64_C(2147483648), -INT64_C(2147483649),
    INT64_C(4294967295), -INT64_C(4294967295),
    INT64_C(4294967296), -INT64_C(4294967296),
    INT64_MIN, INT64_MAX,
  };
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    test(thread, values[i]);
  }

  Arcadia_BigInteger* source = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* result = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, source, UINT64_MAX);
  Arcadia_BigInteger_Limp* volatile bytes = NULL;
  Arcadia_SizeValue size;
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_BigInteger_Limp* encoded = NULL;
    Arcadia_BigInteger_toTwosComplement(thread, source, &encoded, &size);
    bytes = encoded;
    Arcadia_Tests_assertEqualSize(thread, 3, size);
    Arcadia_Tests_assertEqualNatural64(thread, UINT32_MAX, encoded[0]);
    Arcadia_Tests_assertEqualNatural64(thread, UINT32_MAX, encoded[1]);
    Arcadia_Tests_assertEqualNatural64(thread, 0, encoded[2]);
    // Replace an already populated destination repeatedly; its old buffer is owned.
    for (unsigned i = 0; i < 16; ++i) {
      Arcadia_BigInteger_fromTwosComplement(thread, result, encoded, size);
      Arcadia_Tests_assertEqualNatural64(thread, UINT64_MAX, Arcadia_BigInteger_toNatural64(thread, result));
    }
    Arcadia_Memory_deallocateUnmanaged(thread, encoded);
    bytes = NULL;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (bytes) Arcadia_Memory_deallocateUnmanaged(thread, bytes);
    Arcadia_Thread_jump(thread);
  }
}
