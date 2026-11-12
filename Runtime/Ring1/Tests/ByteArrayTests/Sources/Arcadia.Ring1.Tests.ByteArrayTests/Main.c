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

// Deliberately non-contiguous logical storage for factory-copy regressions.
// The physical slots are separated by padding and expose no native byte span.
Arcadia_declareObjectType(u8"Arcadia.Tests.StridedByteArray", Arcadia_TestStridedByteArray,
                          u8"Arcadia.ByteArray");

struct Arcadia_TestStridedByteArrayDispatch {
  Arcadia_ByteArrayDispatch _parent;
};

struct Arcadia_TestStridedByteArray {
  Arcadia_ByteArray _parent;
  Arcadia_Natural8Value storage[6];
  Arcadia_BooleanValue failReads;
};

static void
Arcadia_TestStridedByteArray_construct
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArray* self
  )
{
  Arcadia_EnterConstructor(Arcadia_TestStridedByteArray);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->storage[0] = 11;
  self->storage[1] = 0;
  self->storage[2] = 22;
  self->storage[3] = 0;
  self->storage[4] = 33;
  self->storage[5] = 0;
  self->failReads = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(Arcadia_TestStridedByteArray);
}

static Arcadia_Natural8Value
Arcadia_TestStridedByteArray_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArray const* self,
    Arcadia_SizeValue index
  )
{
  if (index >= 3) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->failReads && index == 1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->storage[index * 2];
}

static Arcadia_Natural8Value const*
Arcadia_TestStridedByteArray_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArray const* self
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_SizeValue
Arcadia_TestStridedByteArray_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArray const* self
  )
{ return 3; }

static Arcadia_BooleanValue
Arcadia_TestStridedByteArray_isEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArray const* self
  )
{ return Arcadia_BooleanValue_False; }

static Arcadia_BooleanValue
Arcadia_TestStridedByteArray_isContiguous
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArray const* self
  )
{ return Arcadia_BooleanValue_False; }

static void
Arcadia_TestStridedByteArray_initializeDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_TestStridedByteArrayDispatch* self
  )
{
  Arcadia_ByteArrayDispatch* dispatch = (Arcadia_ByteArrayDispatch*)self;
  dispatch->getAt = (Arcadia_Natural8Value (*)(Arcadia_Thread*, Arcadia_ByteArray const*, Arcadia_SizeValue))&Arcadia_TestStridedByteArray_getAt;
  dispatch->getBytes = (Arcadia_Natural8Value const* (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_TestStridedByteArray_getBytes;
  dispatch->getNumberOfBytes = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_TestStridedByteArray_getSize;
  dispatch->getSize = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_TestStridedByteArray_getSize;
  dispatch->isEmpty = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_TestStridedByteArray_isEmpty;
  dispatch->isContiguous = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_ByteArray const*))&Arcadia_TestStridedByteArray_isContiguous;
}

static const Arcadia_ObjectType_Operations _stridedObjectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_TestStridedByteArray_construct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_TestStridedByteArray_initializeDispatch,
};

static const Arcadia_Type_Operations _stridedTypeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_stridedObjectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Tests.StridedByteArray", Arcadia_TestStridedByteArray,
                         u8"Arcadia.ByteArray", Arcadia_ByteArray,
                         &_stridedTypeOperations);

static Arcadia_TestStridedByteArray*
Arcadia_TestStridedByteArray_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_TestStridedByteArray);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_TestStridedByteArray);
}

// Expected runtime failures must be isolated from the test runner's jump target.
static void
expectStatus(Arcadia_Thread* thread, Arcadia_ByteArray* array,
             Arcadia_BooleanValue resize, Arcadia_SizeValue index,
             Arcadia_Status expected)
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    if (resize) {
      Arcadia_ByteArray_resize(thread, array, index);
    } else {
      Arcadia_ByteArray_setAt(thread, array, index, 42);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, expected, status);
  }
}

static void
testFactory(Arcadia_Thread* thread)
{
  Arcadia_ByteArray* fixed = Arcadia_ByteArrayFactory_createMutable(thread, 3);
  Arcadia_Object_lock(thread, (Arcadia_Object*)fixed);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isMutable(thread, fixed));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, fixed));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, fixed));
  Arcadia_Tests_assertEqualSize(thread, 3, Arcadia_ByteArray_getSize(thread, fixed));
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArray_getAt(thread, fixed, 0));
  Arcadia_ByteArray_setAt(thread, fixed, 1, 77);
  Arcadia_Tests_assertEqualSize(thread, 77, Arcadia_ByteArray_getAt(thread, fixed, 1));
  expectStatus(thread, fixed, Arcadia_BooleanValue_False, 3, Arcadia_Status_ArgumentValueInvalid);
  expectStatus(thread, fixed, Arcadia_BooleanValue_True, 4, Arcadia_Status_OperationInvalid);

  Arcadia_ByteArray* grow = Arcadia_ByteArrayFactory_copyResizable(thread, fixed);
  Arcadia_Object_lock(thread, (Arcadia_Object*)grow);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isResizable(thread, grow));
  Arcadia_ByteArray_setAt(thread, grow, 1, 33);
  Arcadia_Tests_assertEqualSize(thread, 77, Arcadia_ByteArray_getAt(thread, fixed, 1));
  Arcadia_ByteArray_resize(thread, grow, 11);
  Arcadia_Tests_assertEqualSize(thread, 11, Arcadia_ByteArray_getSize(thread, grow));
  Arcadia_Tests_assertEqualSize(thread, 33, Arcadia_ByteArray_getAt(thread, grow, 1));
  for (Arcadia_SizeValue i = 3; i < 11; ++i) {
    Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArray_getAt(thread, grow, i));
  }
  Arcadia_ByteArray_resize(thread, grow, 1);
  Arcadia_ByteArray_resize(thread, grow, 10);
  for (Arcadia_SizeValue i = 1; i < 10; ++i) {
    Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArray_getAt(thread, grow, i));
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)grow);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)fixed);
}

static void
testImmutable(Arcadia_Thread* thread)
{
  Arcadia_Natural8Value const bytes[] = { 10, 20, 30, 40 };
  Arcadia_RuntimeByteArray* nativeValue = Arcadia_RuntimeByteArray_create(thread, bytes, 4);
  Arcadia_ByteArray* immutable = Arcadia_ByteArrayFactory_createImmutable(thread, nativeValue);
  Arcadia_Object_lock(thread, (Arcadia_Object*)immutable);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, immutable));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, immutable));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, immutable));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getBytes(thread, immutable) == Arcadia_RuntimeByteArray_getBytes(thread, nativeValue));
  expectStatus(thread, immutable, Arcadia_BooleanValue_False, 0, Arcadia_Status_OperationInvalid);
  Arcadia_ByteArray* writable = Arcadia_ByteArrayFactory_copyMutable(thread, immutable);
  Arcadia_Object_lock(thread, (Arcadia_Object*)writable);
  Arcadia_ByteArray_setAt(thread, writable, 0, 99);
  Arcadia_Tests_assertEqualSize(thread, 10, Arcadia_ByteArray_getAt(thread, immutable, 0));
  Arcadia_Tests_assertEqualSize(thread, 99, Arcadia_ByteArray_getAt(thread, writable, 0));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)writable);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)immutable);
}

static void
testReaders(Arcadia_Thread* thread)
{
  Arcadia_ByteArray* array = Arcadia_ByteArrayFactory_createResizable(thread, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)array);
  Arcadia_ByteArray_setAt(thread, array, 0, 17);
  Arcadia_ByteArray_setAt(thread, array, 1, 23);
  Arcadia_ByteReader* reader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, array);
  Arcadia_Object_lock(thread, (Arcadia_Object*)reader);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteReader_hasValue(thread, reader));
  Arcadia_Tests_assertEqualSize(thread, 17, Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteReader_nextValue(thread, reader);
  Arcadia_ByteArray_setAt(thread, array, 1, 31);
  Arcadia_Tests_assertEqualSize(thread, 31, Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteArray_resize(thread, array, 1);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteReader_hasValue(thread, reader));
  Arcadia_ByteArray_resize(thread, array, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteReader_hasValue(thread, reader));
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)reader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)array);
}

// Read operations against a view whose backing has shrunk must fail cleanly.
static void
expectSliceInvalid(Arcadia_Thread* thread, Arcadia_ByteArray* slice, Arcadia_BooleanValue bulk)
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    if (bulk) {
      (void)Arcadia_ByteArray_getBytes(thread, slice);
    } else {
      (void)Arcadia_ByteArray_getAt(thread, slice, 0);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, actual);
  }
}

static void
testSliceResize(Arcadia_Thread* thread)
{
  Arcadia_ByteArray* backing = Arcadia_ByteArrayFactory_createResizable(thread, 4);
  Arcadia_Object_lock(thread, (Arcadia_Object*)backing);
  Arcadia_ByteArray_setAt(thread, backing, 2, 91);
  Arcadia_ByteArray* slice = Arcadia_ByteArray_slice(thread, backing, 2, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)slice);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, slice));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, slice));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, slice));
  Arcadia_Tests_assertEqualSize(thread, 91, Arcadia_ByteArray_getAt(thread, slice, 0));
  Arcadia_Tests_assertEqualSize(thread, 91, Arcadia_ByteArray_getBytes(thread, slice)[0]);
  Arcadia_ByteArray_resize(thread, backing, 3);
  expectSliceInvalid(thread, slice, Arcadia_BooleanValue_False);
  expectSliceInvalid(thread, slice, Arcadia_BooleanValue_True);
  Arcadia_ByteArray_resize(thread, backing, 4);
  Arcadia_Tests_assertEqualSize(thread, 91, Arcadia_ByteArray_getAt(thread, slice, 0));
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArray_getAt(thread, slice, 1));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)slice);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)backing);
}

// Readers and slices must trace their backing storage across a collection.
// Only the retaining readers/slice are locked: the sources are not locked here.
static void
testReaderAndSliceGCLifetimes(Arcadia_Thread* thread)
{
  Arcadia_ByteArray* array = Arcadia_ByteArrayFactory_createResizable(thread, 3);
  Arcadia_ByteArray_setAt(thread, array, 0, 11);
  Arcadia_ByteArray_setAt(thread, array, 1, 22);
  Arcadia_ByteArray_setAt(thread, array, 2, 33);
  Arcadia_ByteReader* arrayReader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, array);
  Arcadia_Object_lock(thread, (Arcadia_Object*)arrayReader);

  Arcadia_ByteArray* slice = Arcadia_ByteArray_slice(thread, array, 1, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)slice);
  Arcadia_ByteReader* sliceReader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, slice);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sliceReader);

  Arcadia_Natural8Value const sourceBytes[] = { 44, 55, 66 };
  Arcadia_RuntimeByteArray* runtime = Arcadia_RuntimeByteArray_create(thread, sourceBytes, 3);
  Arcadia_ByteReader* runtimeReader = (Arcadia_ByteReader*)Arcadia_RuntimeByteArray_ByteReader_create(thread, runtime);
  Arcadia_Object_lock(thread, (Arcadia_Object*)runtimeReader);

  Arcadia_ByteArrayBuilder* builder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, sourceBytes, 3);
  Arcadia_ByteReader* builderReader = (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, builder);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builderReader);

  // The managed sources are reachable through their retained objects, not
  // through explicit object locks on those sources.
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);

  Arcadia_Tests_assertEqualSize(thread, 11, Arcadia_ByteReader_getValue(thread, arrayReader));
  Arcadia_ByteReader_nextValue(thread, arrayReader);
  Arcadia_Tests_assertEqualSize(thread, 22, Arcadia_ByteReader_getValue(thread, arrayReader));
  Arcadia_Tests_assertEqualSize(thread, 22, Arcadia_ByteReader_getValue(thread, sliceReader));
  Arcadia_Tests_assertEqualSize(thread, 33, Arcadia_ByteArray_getAt(thread, slice, 1));
  Arcadia_Tests_assertEqualSize(thread, 44, Arcadia_ByteReader_getValue(thread, runtimeReader));
  Arcadia_Tests_assertEqualSize(thread, 44, Arcadia_ByteReader_getValue(thread, builderReader));

  Arcadia_Object_unlock(thread, (Arcadia_Object*)builderReader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)runtimeReader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sliceReader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)slice);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)arrayReader);
}

static void
expectMutableBorrowInvalid(Arcadia_Thread* thread, Arcadia_ByteArray* array)
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    (void)Arcadia_ByteArray_getMutableBytes(thread, array);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationInvalid, status);
  }
}

static void
testNativeMutableBorrow(Arcadia_Thread* thread)
{
  Arcadia_ByteArray* fixed = Arcadia_ByteArrayFactory_createMutable(thread, 4);
  Arcadia_Object_lock(thread, (Arcadia_Object*)fixed);
  Arcadia_Natural8Value* writable = Arcadia_ByteArray_getMutableBytes(thread, fixed);
  Arcadia_Tests_assertTrue(thread, writable == Arcadia_ByteArray_getBytes(thread, fixed));
  writable[0] = 31;
  writable[3] = 73;
  Arcadia_Tests_assertEqualSize(thread, 31, Arcadia_ByteArray_getAt(thread, fixed, 0));
  Arcadia_Tests_assertEqualSize(thread, 73, Arcadia_ByteArray_getAt(thread, fixed, 3));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, fixed));

  Arcadia_ByteArray* grow = Arcadia_ByteArrayFactory_copyResizable(thread, fixed);
  Arcadia_Object_lock(thread, (Arcadia_Object*)grow);
  Arcadia_ByteArray_resize(thread, grow, 9);
  Arcadia_Natural8Value* newBorrow = Arcadia_ByteArray_getMutableBytes(thread, grow);
  Arcadia_Tests_assertEqualSize(thread, 31, newBorrow[0]);
  Arcadia_Tests_assertEqualSize(thread, 73, newBorrow[3]);
  for (Arcadia_SizeValue i = 4; i < 9; ++i) {
    Arcadia_Tests_assertEqualSize(thread, 0, newBorrow[i]);
  }
  newBorrow[8] = 201;
  Arcadia_Tests_assertEqualSize(thread, 201, Arcadia_ByteArray_getAt(thread, grow, 8));
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArray_getAt(thread, fixed, 2));

  Arcadia_ByteArray* snapshot = Arcadia_ByteArrayFactory_copyImmutable(thread, grow);
  Arcadia_Object_lock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, snapshot));
  expectMutableBorrowInvalid(thread, snapshot);
  Arcadia_ByteArray* slice = Arcadia_ByteArray_slice(thread, fixed, 1, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)slice);
  expectMutableBorrowInvalid(thread, slice);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)slice);
  Arcadia_ByteArray_setAt(thread, grow, 8, 0);
  Arcadia_Tests_assertEqualSize(thread, 201, Arcadia_ByteArray_getAt(thread, snapshot, 8));
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertEqualSize(thread, 201, Arcadia_ByteArray_getAt(thread, snapshot, 8));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)grow);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)fixed);
}

static void
testFactoryCopyViews
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ByteArray* original = Arcadia_ByteArrayFactory_createResizable(thread, 4);
  Arcadia_Object_lock(thread, (Arcadia_Object*)original);
  Arcadia_ByteArray_setAt(thread, original, 1, 17);
  Arcadia_ByteArray_setAt(thread, original, 2, 23);
  Arcadia_ByteArray* view = Arcadia_ByteArrayFactory_createSlice(thread, original, 1, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)view);

  Arcadia_ByteArray* fixed = Arcadia_ByteArrayFactory_copyMutable(thread, view);
  Arcadia_Object_lock(thread, (Arcadia_Object*)fixed);
  Arcadia_ByteArray* growable = Arcadia_ByteArrayFactory_copyResizable(thread, view);
  Arcadia_Object_lock(thread, (Arcadia_Object*)growable);
  Arcadia_ByteArray* snapshot = Arcadia_ByteArrayFactory_copyImmutable(thread, view);
  Arcadia_Object_lock(thread, (Arcadia_Object*)snapshot);

  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_ByteArray_getSize(thread, fixed));
  Arcadia_Tests_assertEqualSize(thread, 17, Arcadia_ByteArray_getAt(thread, fixed, 0));
  Arcadia_Tests_assertEqualSize(thread, 23, Arcadia_ByteArray_getAt(thread, growable, 1));
  Arcadia_Tests_assertEqualSize(thread, 17, Arcadia_ByteArray_getAt(thread, snapshot, 0));
  Arcadia_ByteArray_setAt(thread, original, 1, 99);
  Arcadia_Tests_assertEqualSize(thread, 17, Arcadia_ByteArray_getAt(thread, fixed, 0));
  Arcadia_Tests_assertEqualSize(thread, 17, Arcadia_ByteArray_getAt(thread, growable, 0));
  Arcadia_Tests_assertEqualSize(thread, 17, Arcadia_ByteArray_getAt(thread, snapshot, 0));

  Arcadia_ByteArray* empty = Arcadia_ByteArrayFactory_createResizable(thread, 0);
  Arcadia_Object_lock(thread, (Arcadia_Object*)empty);
  Arcadia_ByteArray* emptySnapshot = Arcadia_ByteArrayFactory_copyImmutable(thread, empty);
  Arcadia_Object_lock(thread, (Arcadia_Object*)emptySnapshot);
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArray_getSize(thread, emptySnapshot));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)emptySnapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)empty);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)growable);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)fixed);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)view);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)original);
}

static void
testNonContiguousCopies
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ByteArray* source = (Arcadia_ByteArray*)Arcadia_TestStridedByteArray_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)source);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isContiguous(thread, source));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, source));
  Arcadia_Tests_assertEqualSize(thread, 3, Arcadia_ByteArray_getSize(thread, source));

  Arcadia_ByteArray* view = Arcadia_ByteArrayFactory_createSlice(thread, source, 1, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)view);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isContiguous(thread, view));
  Arcadia_Tests_assertEqualSize(thread, 22, Arcadia_ByteArray_getAt(thread, view, 0));
  Arcadia_ByteReader* reader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, view);
  Arcadia_Object_lock(thread, (Arcadia_Object*)reader);
  Arcadia_Tests_assertEqualSize(thread, 22, Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteReader_nextValue(thread, reader);
  Arcadia_Tests_assertEqualSize(thread, 33, Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteReader_nextValue(thread, reader);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteReader_hasValue(thread, reader));

  Arcadia_ByteArray* fixed = Arcadia_ByteArrayFactory_copyMutable(thread, source);
  Arcadia_Object_lock(thread, (Arcadia_Object*)fixed);
  Arcadia_ByteArray* growable = Arcadia_ByteArrayFactory_copyResizable(thread, view);
  Arcadia_Object_lock(thread, (Arcadia_Object*)growable);
  Arcadia_ByteArray* immutable = Arcadia_ByteArrayFactory_copyImmutable(thread, source);
  Arcadia_Object_lock(thread, (Arcadia_Object*)immutable);

  Arcadia_Natural8Value const original[] = { 11, 22, 33 };
  Arcadia_Natural8Value const sliced[] = { 22, 33 };
  Arcadia_Tests_assertEqualBytes(thread, original, 3,
                                Arcadia_ByteArray_getBytes(thread, fixed),
                                Arcadia_ByteArray_getSize(thread, fixed));
  Arcadia_Tests_assertEqualBytes(thread, sliced, 2,
                                Arcadia_ByteArray_getBytes(thread, growable),
                                Arcadia_ByteArray_getSize(thread, growable));
  Arcadia_Tests_assertEqualBytes(thread, original, 3,
                                Arcadia_ByteArray_getBytes(thread, immutable),
                                Arcadia_ByteArray_getSize(thread, immutable));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isResizable(thread, growable));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, immutable));
  Arcadia_ByteArray_setAt(thread, fixed, 0, 99);
  Arcadia_Tests_assertEqualSize(thread, 11, Arcadia_ByteArray_getAt(thread, source, 0));
  Arcadia_Tests_assertEqualSize(thread, 11, Arcadia_ByteArray_getAt(thread, immutable, 0));
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertEqualSize(thread, 22, Arcadia_ByteArray_getAt(thread, growable, 0));
  Arcadia_Tests_assertEqualSize(thread, 33, Arcadia_ByteArray_getAt(thread, immutable, 2));

  Arcadia_Object_unlock(thread, (Arcadia_Object*)reader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)immutable);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)growable);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)fixed);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)view);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)source);
}

static void
expectCopyFailure
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* source,
    Arcadia_Natural8Value kind
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    if (kind == 0) {
      (void)Arcadia_ByteArrayFactory_copyMutable(thread, source);
    } else if (kind == 1) {
      (void)Arcadia_ByteArrayFactory_copyResizable(thread, source);
    } else {
      (void)Arcadia_ByteArrayFactory_copyImmutable(thread, source);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationInvalid, status);
    Arcadia_Tests_assertEqualSize(thread, stackSize, Arcadia_ValueStack_getSize(thread));
  }
}

static void
testNonContiguousCopyFailures
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TestStridedByteArray* object = Arcadia_TestStridedByteArray_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)object);
  object->failReads = Arcadia_BooleanValue_True;
  for (Arcadia_Natural8Value kind = 0; kind < 3; ++kind) {
    expectCopyFailure(thread, (Arcadia_ByteArray*)object, kind);
  }
  object->failReads = Arcadia_BooleanValue_False;
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_ByteArray* recovered = Arcadia_ByteArrayFactory_copyImmutable(thread, (Arcadia_ByteArray*)object);
  Arcadia_Object_lock(thread, (Arcadia_Object*)recovered);
  Arcadia_Tests_assertEqualSize(thread, 22, Arcadia_ByteArray_getAt(thread, recovered, 1));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)recovered);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)object);
}

static int
runNonContiguousCopyFailures
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testNonContiguousCopyFailures) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runNonContiguousCopies
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testNonContiguousCopies) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runFactoryCopyViews
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testFactoryCopyViews) ? EXIT_SUCCESS : EXIT_FAILURE; }

static void
expectReaderEnd
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* reader,
    Arcadia_BooleanValue advance
  )
{
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    if (advance) {
      Arcadia_ByteReader_nextValue(thread, reader);
    } else {
      (void)Arcadia_ByteReader_getValue(thread, reader);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationInvalid, status);
  }
}

static void
testReaderCompatibility
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value const bytes[] = { 12, 34 };
  Arcadia_RuntimeByteArray* runtime = Arcadia_RuntimeByteArray_create(thread, bytes, 2);
  Arcadia_ByteReader* runtimeReader = (Arcadia_ByteReader*)Arcadia_RuntimeByteArray_ByteReader_create(thread, runtime);
  Arcadia_Object_lock(thread, (Arcadia_Object*)runtimeReader);
  Arcadia_ByteArray* array = Arcadia_ByteArrayFactory_createResizable(thread, 2);
  Arcadia_Object_lock(thread, (Arcadia_Object*)array);
  Arcadia_ByteArray_setAt(thread, array, 0, 12);
  Arcadia_ByteArray_setAt(thread, array, 1, 34);
  Arcadia_ByteReader* arrayReader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, array);
  Arcadia_Object_lock(thread, (Arcadia_Object*)arrayReader);
  Arcadia_ByteArrayBuilder* builder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, bytes, 2);
  Arcadia_ByteReader* builderReader = (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, builder);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builderReader);
  Arcadia_ByteReader* readers[] = { runtimeReader, arrayReader, builderReader };
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Tests_assertEqualSize(thread, 12, Arcadia_ByteReader_getValue(thread, readers[i]));
    Arcadia_ByteReader_nextValue(thread, readers[i]);
    Arcadia_Tests_assertEqualSize(thread, 34, Arcadia_ByteReader_getValue(thread, readers[i]));
    Arcadia_ByteReader_nextValue(thread, readers[i]);
    Arcadia_Tests_assertFalse(thread, Arcadia_ByteReader_hasValue(thread, readers[i]));
    Arcadia_Tests_assertFalse(thread, Arcadia_ByteReader_hasError(thread, readers[i]));
    expectReaderEnd(thread, readers[i], Arcadia_BooleanValue_False);
    expectReaderEnd(thread, readers[i], Arcadia_BooleanValue_True);
  }
  Arcadia_ByteArray_resize(thread, array, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteReader_hasValue(thread, arrayReader));
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteReader_getValue(thread, arrayReader));
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, "Z", 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteReader_hasValue(thread, builderReader));
  Arcadia_Tests_assertEqualSize(thread, 'Z', Arcadia_ByteReader_getValue(thread, builderReader));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builderReader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)arrayReader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)array);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)runtimeReader);
}

static int
runReaderCompatibility
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testReaderCompatibility) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int runNativeMutableBorrow(void)
{ return Arcadia_Tests_safeExecute(&testNativeMutableBorrow) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int runFactory(void)
{ return Arcadia_Tests_safeExecute(&testFactory) ? EXIT_SUCCESS : EXIT_FAILURE; }
static int runImmutable(void)
{ return Arcadia_Tests_safeExecute(&testImmutable) ? EXIT_SUCCESS : EXIT_FAILURE; }
static int runReaders(void)
{ return Arcadia_Tests_safeExecute(&testReaders) ? EXIT_SUCCESS : EXIT_FAILURE; }
static int runSliceResize(void)
{ return Arcadia_Tests_safeExecute(&testSliceResize) ? EXIT_SUCCESS : EXIT_FAILURE; }
static int runReaderAndSliceGCLifetimes(void)
{ return Arcadia_Tests_safeExecute(&testReaderAndSliceGCLifetimes) ? EXIT_SUCCESS : EXIT_FAILURE; }

int main(int argc, char** argv)
{
  static const Arcadia_Testing_Case cases[] = {
    { "FactoryAndResize", &runFactory },
    { "NativeMutableBorrow", &runNativeMutableBorrow },
    { "ImmutableAndCopy", &runImmutable },
    { "FactoryCopyViews", &runFactoryCopyViews },
    { "NonContiguousCopies", &runNonContiguousCopies },
    { "NonContiguousCopyFailures", &runNonContiguousCopyFailures },
    { "ReaderMutationAndResize", &runReaders },
    { "ReaderCompatibility", &runReaderCompatibility },
    { "SliceBackingResize", &runSliceResize },
    { "ReaderAndSliceGCLifetimes", &runReaderAndSliceGCLifetimes },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
