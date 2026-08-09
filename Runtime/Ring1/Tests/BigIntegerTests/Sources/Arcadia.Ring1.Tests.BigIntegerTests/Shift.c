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

#include "Arcadia.Ring1.Tests.BigIntegerTests/Shift.h"

static void
checkPowerOfTwo
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value v,
    Arcadia_Natural64Value p
  )
{
  Arcadia_BigInteger* temporary = NULL;
  Arcadia_BigInteger* bigIntegerValue = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural8(thread, bigIntegerValue, v);

  Arcadia_BigInteger* bigIntegerShiftedValue = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_copy(thread, bigIntegerShiftedValue, bigIntegerValue);
  Arcadia_BigInteger_shiftLeftNatural64(thread, bigIntegerShiftedValue, p);

  temporary = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger* bigIntegerMultipliedValue = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setPowerOfTwoNatural8(thread, temporary, p);
  Arcadia_BigInteger_multiply3(thread, bigIntegerMultipliedValue, bigIntegerValue, temporary);

  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, bigIntegerShiftedValue, bigIntegerMultipliedValue));

  Arcadia_BigInteger* bigIntegerBackShiftedValue = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_copy(thread, bigIntegerBackShiftedValue, bigIntegerShiftedValue);
  Arcadia_BigInteger_shiftRightNatural64(thread, bigIntegerBackShiftedValue, p);
  Arcadia_Tests_assertTrue(thread, Arcadia_BigInteger_isEqualTo(thread, bigIntegerBackShiftedValue, bigIntegerValue));
}

void
Arcadia_Ring1_Tests_BigInteger_shiftOperations
  (
    Arcadia_Thread* thread
  )
{
  checkPowerOfTwo(thread, 1, 0);
  checkPowerOfTwo(thread, 1, 1);
  checkPowerOfTwo(thread, 1, 2);
  checkPowerOfTwo(thread, 1, 3);
  checkPowerOfTwo(thread, 1, 4);

  checkPowerOfTwo(thread, 1, 32);
  checkPowerOfTwo(thread, 1, 33);
  checkPowerOfTwo(thread, 1, 34);
  checkPowerOfTwo(thread, 1, 35);
  checkPowerOfTwo(thread, 1, 36);

  checkPowerOfTwo(thread, 1, 62);
  checkPowerOfTwo(thread, 1, 63);
  checkPowerOfTwo(thread, 1, 64);
  checkPowerOfTwo(thread, 1, 65);
  checkPowerOfTwo(thread, 1, 66);
}
