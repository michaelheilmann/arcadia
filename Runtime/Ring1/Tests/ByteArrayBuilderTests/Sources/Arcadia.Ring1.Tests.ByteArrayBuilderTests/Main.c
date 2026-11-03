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
testInsertAtAllPositions(Arcadia_Thread* thread)
{
  Arcadia_ByteArrayBuilder* builder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);

  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, "ace", 3);
  Arcadia_ByteArrayBuilder_insertBytes(thread, builder, 1, "b", 1);
  Arcadia_ByteArrayBuilder_insertBytes(thread, builder, 3, "d", 1);
  Arcadia_ByteArrayBuilder_insertFrontBytes(thread, builder, "^", 1);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, "$", 1);
  Arcadia_Tests_assertEqualBytes(thread, "^abcde$", 7,
                                 Arcadia_ByteArrayBuilder_getBytes(thread, builder),
                                 Arcadia_ByteArrayBuilder_getSize(thread, builder));

  Arcadia_ByteArray* snapshot = Arcadia_ByteArrayBuilder_toByteArray(thread, builder);
  Arcadia_Object_lock(thread, (Arcadia_Object*)snapshot);
  Arcadia_ByteArrayBuilder_clear(thread, builder);
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_ByteArrayBuilder_getSize(thread, builder));
  Arcadia_Tests_assertEqualBytes(thread, "^abcde$", 7,
                                 Arcadia_ByteArray_getBytes(thread, snapshot),
                                 Arcadia_ByteArray_getSize(thread, snapshot));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testStorageAndSwap(Arcadia_Thread* thread)
{
  Arcadia_ByteArrayBuilder* a = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)a);
  Arcadia_ByteArrayBuilder* b = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)b);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, a, "abc", 3);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, "XY", 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isResizable(thread, a->storage));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, a->storage));
  Arcadia_Tests_assertTrue(thread, a->p == (char*)Arcadia_ByteArray_getBytes(thread, a->storage));
  Arcadia_Tests_assertEqualSize(thread, a->sz, Arcadia_ByteArray_getSize(thread, a->storage));
  Arcadia_ByteArrayBuilder_swap(thread, a, b);
  Arcadia_Tests_assertEqualBytes(thread, "XY", 2, a->p, a->sz);
  Arcadia_Tests_assertEqualBytes(thread, "abc", 3, b->p, b->sz);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertEqualBytes(thread, "XY", 2, a->p, a->sz);
  Arcadia_Tests_assertEqualBytes(thread, "abc", 3, b->p, b->sz);
  Arcadia_ByteArrayBuilder_clear(thread, b);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, "q", 1);
  Arcadia_Tests_assertEqualBytes(thread, "q", 1, b->p, b->sz);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)b);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)a);
}

static void
testReaderAfterMutations(Arcadia_Thread* thread)
{
  Arcadia_ByteArrayBuilder* builder = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)builder);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, "ab", 2);
  Arcadia_ByteReader* reader = (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, builder);
  Arcadia_Object_lock(thread, (Arcadia_Object*)reader);
  Arcadia_Tests_assertEqualSize(thread, 'a', Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteReader_nextValue(thread, reader);
  Arcadia_ByteArrayBuilder_insertBytes(thread, builder, 1, "Z", 1);
  Arcadia_Tests_assertEqualSize(thread, 'Z', Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteReader_nextValue(thread, reader);
  Arcadia_Tests_assertEqualSize(thread, 'b', Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_ByteArrayBuilder_clear(thread, builder);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteReader_hasValue(thread, reader));
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, builder, "xyz", 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteReader_hasValue(thread, reader));
  Arcadia_Tests_assertEqualSize(thread, 'z', Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertEqualSize(thread, 'z', Arcadia_ByteReader_getValue(thread, reader));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)reader);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)builder);
}

static void
testImmutableSnapshot(Arcadia_Thread* thread)
{
  Arcadia_ByteArray* source = Arcadia_ByteArrayFactory_createResizable(thread, 3);
  Arcadia_Object_lock(thread, (Arcadia_Object*)source);
  Arcadia_ByteArray_setAt(thread, source, 0, 11);
  Arcadia_ByteArray_setAt(thread, source, 1, 22);
  Arcadia_ByteArray_setAt(thread, source, 2, 33);
  Arcadia_ByteArray* snapshot = Arcadia_ByteArrayFactory_copyImmutable(thread, source);
  Arcadia_Object_lock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isMutable(thread, snapshot));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, snapshot));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, snapshot));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getBytes(thread, snapshot) !=
                                  Arcadia_ByteArray_getBytes(thread, source));
  Arcadia_ByteArray_setAt(thread, source, 1, 99);
  Arcadia_ByteArray_resize(thread, source, 0);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Natural8Value const expected[] = { 11, 22, 33 };
  Arcadia_Tests_assertEqualBytes(thread, expected, 3,
                                 Arcadia_ByteArray_getBytes(thread, snapshot),
                                 Arcadia_ByteArray_getSize(thread, snapshot));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)snapshot);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)source);
}

// A self-referential insert must survive growth, including relocation of the
// backing buffer. Exercise source ranges before, after and across the gap.
static void
testSelfInsertion(Arcadia_Thread* thread)
{
  Arcadia_ByteArrayBuilder* b = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)b);

  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, "abcd", 4);
  Arcadia_ByteArrayBuilder_insertBytes(thread, b, 2,
                                      Arcadia_ByteArrayBuilder_getBytes(thread, b) + 1, 3);
  Arcadia_Tests_assertEqualBytes(thread, "abbcdcd", 7,
                                Arcadia_ByteArrayBuilder_getBytes(thread, b),
                                Arcadia_ByteArrayBuilder_getSize(thread, b));

  Arcadia_ByteArrayBuilder_clear(thread, b);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, "abcd", 4);
  Arcadia_ByteArrayBuilder_insertBytes(thread, b, 3,
                                      Arcadia_ByteArrayBuilder_getBytes(thread, b), 2);
  Arcadia_Tests_assertEqualBytes(thread, "abcabd", 6,
                                Arcadia_ByteArrayBuilder_getBytes(thread, b),
                                Arcadia_ByteArrayBuilder_getSize(thread, b));

  Arcadia_ByteArrayBuilder_clear(thread, b);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, "abcd", 4);
  Arcadia_ByteArrayBuilder_insertBytes(thread, b, 1,
                                      Arcadia_ByteArrayBuilder_getBytes(thread, b) + 2, 2);
  Arcadia_Tests_assertEqualBytes(thread, "acdbcd", 6,
                                Arcadia_ByteArrayBuilder_getBytes(thread, b),
                                Arcadia_ByteArrayBuilder_getSize(thread, b));

  Arcadia_ByteArrayBuilder_clear(thread, b);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, "abc", 3);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, b, Arcadia_ByteArrayBuilder_getBytes(thread, b), 3);
  Arcadia_Tests_assertEqualBytes(thread, "abcabc", 6,
                                Arcadia_ByteArrayBuilder_getBytes(thread, b),
                                Arcadia_ByteArrayBuilder_getSize(thread, b));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)b);
}

static int
runSelfInsertion(void)
{ return Arcadia_Tests_safeExecute(&testSelfInsertion) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runInsertAtAllPositions(void)
{ return Arcadia_Tests_safeExecute(&testInsertAtAllPositions) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runStorageAndSwap(void)
{ return Arcadia_Tests_safeExecute(&testStorageAndSwap) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runReaderAfterMutations(void)
{ return Arcadia_Tests_safeExecute(&testReaderAfterMutations) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runImmutableSnapshot(void)
{ return Arcadia_Tests_safeExecute(&testImmutableSnapshot) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
main(int argc, char** argv)
{
  static const Arcadia_Testing_Case cases[] = {
    { "InsertAtAllPositions", &runInsertAtAllPositions },
    { "SelfInsertion", &runSelfInsertion },
    { "StorageAndSwap", &runStorageAndSwap },
    { "ReaderAfterMutations", &runReaderAfterMutations },
    { "ImmutableSnapshot", &runImmutableSnapshot },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
