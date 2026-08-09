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

#include "Arcadia.Ring1.Tests.RealTypes/realTypes.h"

void
Arcadia_Ring1_Tests_RealTypes_real32
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Real32Value_NumberOfExplicitSignificandBits == 23);
  Arcadia_Tests_assertTrue(thread, Arcadia_Real32Value_SignificandBitsMask == 0b11111111111111111111111);
  Arcadia_Tests_assertTrue(thread, Arcadia_Real32Value_ExponentBitsMask == 0b1111111100000000000000000000000);
  Arcadia_Tests_assertTrue(thread, Arcadia_Real32Value_SignBitsMask == 0b10000000000000000000000000000000);
}

void
Arcadia_Ring1_Tests_RealTypes_real64
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Real64Value_NumberOfExplicitSignificandBits == 52);
  Arcadia_Tests_assertTrue(thread, Arcadia_Real64Value_SignificandBitsMask == 0b1111111111111111111111111111111111111111111111111111);
  Arcadia_Tests_assertTrue(thread, Arcadia_Real64Value_ExponentBitsMask == 0b111111111110000000000000000000000000000000000000000000000000000);
  Arcadia_Tests_assertTrue(thread, Arcadia_Real64Value_SignBitsMask == 0b1000000000000000000000000000000000000000000000000000000000000000);
}
