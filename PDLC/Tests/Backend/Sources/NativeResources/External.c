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

#include "NativeResources/External.h"
#define TEST_PRIVATE (1)
#include "Test/NativeEnumBox.h"
#undef TEST_PRIVATE

static const Arcadia_Type_Operations enumOperations = {
  Arcadia_Type_Operations_Initializer,
};

Arcadia_defineEnumerationType(u8"Test.External.Kind", Test_External_Kind,
                              &enumOperations);
Arcadia_defineEnumerationType(u8"Test.External.UnsignedKind", Test_External_UnsignedKind,
                              &enumOperations);

Test_External_Kind
nativeEnumEcho
  (
    Arcadia_Thread* thread,
    Test_NativeEnumBox* self,
    Test_External_Kind value
  )
{
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  return value;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Test_External* self
  )
{
  Arcadia_EnterConstructor(Test_External);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(Test_External);
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Test.External", Test_External,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);

Test_External*
Test_External_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Test_External);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Test_External);
}

Arcadia_Natural64Value
Test_External_getExternalValue
  (
    Arcadia_Thread* thread,
    Test_External* self
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Test_External_getType(thread)));
  return 37;
}
