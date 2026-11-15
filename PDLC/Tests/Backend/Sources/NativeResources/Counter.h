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

#if !defined(NATIVERESOURCES_COUNTER_H_INCLUDED)
#define NATIVERESOURCES_COUNTER_H_INCLUDED

#if !defined(TEST_PRIVATE) || 1 != TEST_PRIVATE
  #error("include the Test aggregate instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareInterfaceType(u8"Test.NativeCounter", Test_NativeCounter)

struct Test_NativeCounterDispatch {
  Arcadia_InterfaceDispatch parent;
  Arcadia_Natural64Value (*read)(Arcadia_Thread* thread, Arcadia_Object* self);
  void (*write)(Arcadia_Thread* thread, Arcadia_Object* self, Arcadia_Natural64Value value);
};

Arcadia_Natural64Value
Test_NativeCounter_read
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

void
Test_NativeCounter_write
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value value
  );

#endif // NATIVERESOURCES_COUNTER_H_INCLUDED
