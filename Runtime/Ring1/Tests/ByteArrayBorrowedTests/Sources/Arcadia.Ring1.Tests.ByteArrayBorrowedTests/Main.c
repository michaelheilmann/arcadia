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

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Ring1/Include.h"
#include <stdlib.h>

static void
expectUnsupported(Arcadia_Thread* thread, Arcadia_ByteArray* a, Arcadia_BooleanValue resize)
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    if (resize) {
      Arcadia_ByteArray_resize(thread, a, 2);
    } else {
      Arcadia_ByteArray_setAt(thread, a, 0, 99);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationInvalid, actual);
  }
}

static void
expectMutablePointerUnsupported(Arcadia_Thread* thread, Arcadia_ByteArray* array)
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    (void)Arcadia_ByteArray_getMutableBytes(thread, array);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationInvalid, actual);
  }
}

static void
testMutableBorrow(Arcadia_Thread* thread)
{
  Arcadia_Natural8Value bytes[] = { 1, 2, 3 };
  Arcadia_ByteArray* borrowed = Arcadia_ByteArrayFactory_borrowMutable(thread, bytes, 3);
  Arcadia_Object_lock(thread, (Arcadia_Object*)borrowed);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isMutable(thread, borrowed));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, borrowed));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, borrowed));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getBytes(thread, borrowed) == bytes);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread, borrowed) == bytes);
  Arcadia_ByteArray_setAt(thread, borrowed, 1, 41);
  Arcadia_Tests_assertEqualSize(thread, 41, bytes[1]);
  bytes[2] = 57;
  Arcadia_Tests_assertEqualSize(thread, 57, Arcadia_ByteArray_getAt(thread, borrowed, 2));
  expectUnsupported(thread, borrowed, Arcadia_BooleanValue_True);

  Arcadia_ByteArray* copy = Arcadia_ByteArrayFactory_copyMutable(thread, borrowed);
  Arcadia_Object_lock(thread, (Arcadia_Object*)copy);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getBytes(thread, copy) != bytes);
  Arcadia_ByteArray_setAt(thread, copy, 0, 99);
  Arcadia_Tests_assertEqualSize(thread, 1, bytes[0]);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertEqualSize(thread, 41, Arcadia_ByteArray_getAt(thread, borrowed, 1));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)copy);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)borrowed);
}

static void
testReadOnlyBorrow(Arcadia_Thread* thread)
{
  static Arcadia_Natural8Value const bytes[] = { 11, 22, 33 };
  Arcadia_ByteArray* borrowed = Arcadia_ByteArrayFactory_borrowReadOnly(thread, bytes, 3);
  Arcadia_Object_lock(thread, (Arcadia_Object*)borrowed);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, borrowed));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, borrowed));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, borrowed));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getBytes(thread, borrowed) == bytes);
  expectUnsupported(thread, borrowed, Arcadia_BooleanValue_False);
  expectUnsupported(thread, borrowed, Arcadia_BooleanValue_True);
  expectMutablePointerUnsupported(thread, borrowed);

  Arcadia_ByteArray* snapshot = Arcadia_ByteArrayFactory_copyImmutable(thread, borrowed);
  Arcadia_Object_lock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getBytes(thread, snapshot) != bytes);
  Arcadia_Tests_assertEqualBytes(thread, bytes, 3,
                                 Arcadia_ByteArray_getBytes(thread, snapshot),
                                 Arcadia_ByteArray_getSize(thread, snapshot));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)borrowed);
}

static int runMutableBorrow(void)
{ return Arcadia_Tests_safeExecute(&testMutableBorrow) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int runReadOnlyBorrow(void)
{ return Arcadia_Tests_safeExecute(&testReadOnlyBorrow) ? EXIT_SUCCESS : EXIT_FAILURE; }

int main(int argc, char** argv)
{
  static const Arcadia_Testing_Case cases[] = {
    { "MutableBorrow", &runMutableBorrow },
    { "ReadOnlyBorrow", &runReadOnlyBorrow },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
