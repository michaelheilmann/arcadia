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

#include "Arcadia.Ring1.Tests.BigIntegerTests/SetInteger.h"

// Regression
static void
regressionTest1
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value value
  )
{
  Arcadia_BigInteger* a = Arcadia_BigInteger_create(thread),
                    * b = Arcadia_BigInteger_create(thread);
  if (Arcadia_Integer8Value_Minimum <= value && value <= Arcadia_Integer8Value_Maximum) {
    Arcadia_BigInteger_setInteger8(thread, a, (Arcadia_Integer8Value)value);
  } else if (Arcadia_Integer16Value_Minimum <= value && value <= Arcadia_Integer16Value_Maximum) {
    Arcadia_BigInteger_setInteger16(thread, a, (Arcadia_Integer16Value)value);
  } else if (Arcadia_Integer32Value_Minimum <= value && value <= Arcadia_Integer32Value_Maximum) {
    Arcadia_BigInteger_setInteger32(thread, a, (Arcadia_Integer32Value)value);
  } else {
    Arcadia_BigInteger_setInteger64(thread, a, (Arcadia_Integer64Value)value);
  }
  Arcadia_BigInteger_setInteger64(thread, b, value);
  Arcadia_Tests_assertTrue(thread, value == Arcadia_BigInteger_toInteger64(thread, a) && value == Arcadia_BigInteger_toInteger64(thread, b));
}

void
Arcadia_Ring1_Tests_BigInteger_setIntegerOperations
  (
    Arcadia_Thread* thread
  )
{
  regressionTest1(thread,  0);

  regressionTest1(thread, +1);
  regressionTest1(thread, -1);

  regressionTest1(thread, +2);
  regressionTest1(thread, -2);

  regressionTest1(thread, Arcadia_Integer8Value_Minimum);
  regressionTest1(thread, Arcadia_Integer8Value_Maximum);

  regressionTest1(thread, Arcadia_Integer16Value_Minimum);
  regressionTest1(thread, Arcadia_Integer16Value_Maximum);

  regressionTest1(thread, Arcadia_Integer32Value_Minimum);
  regressionTest1(thread, Arcadia_Integer32Value_Maximum);

  regressionTest1(thread, Arcadia_Integer64Value_Minimum);
  regressionTest1(thread, Arcadia_Integer64Value_Maximum);
}
