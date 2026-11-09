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

#define TEST_PRIVATE (1)
#include "NativeResources/Counter.h"

Arcadia_defineInterfaceType(u8"Test.NativeCounter", Test_NativeCounter, NULL)

Arcadia_Natural64Value
Test_NativeCounter_read
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_InterfaceCallWithReturn(Test_NativeCounter, read, self);
}

void
Test_NativeCounter_write
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value value
  )
{
  Arcadia_InterfaceCall(Test_NativeCounter, write, self, value);
}
