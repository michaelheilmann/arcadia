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
#include "NativeGenerated/Type.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Test_NativeDeclared* self
  )
{
  Arcadia_EnterConstructor(Test_NativeDeclared);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->value = 41;
  self->kind = Test_NativeDeclared_Kind_One;
  Arcadia_LeaveConstructor(Test_NativeDeclared);
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Test.NativeDeclared", Test_NativeDeclared,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);

Arcadia_Natural64Value
Test_NativeDeclared_read
  (
    Arcadia_Thread* thread,
    Test_NativeDeclared* self
  )
{
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  return self->value + self->kind;
}
