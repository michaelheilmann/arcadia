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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Arcadia/Media/Include.h"
#include "Composite.h"
#include "Operations.h"
#include "FillModels.h"
#include "Formats.h"
#include "Values.h"
#include "NamedColors.h"

static void
testByteArrayStorage(Arcadia_Thread* thread)
{
  // Verify that the native compatibility pointer always aliases managed
  // storage through changes which replace allocations.
  Arcadia_Media_PixelBuffer* a = Arcadia_Media_PixelBuffer_create(thread, 0, 2, 2,
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)a);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ByteArray* storage = Arcadia_Media_PixelBuffer_getPixelBytes(thread, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isMutable(thread, storage));
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, storage));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, storage));
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread, storage) == a->bytes);
  Arcadia_Tests_assertEqualSize(thread, Arcadia_ByteArray_getSize(thread, storage), 12);

  Arcadia_Media_PixelBuffer_setLinePadding(thread, a, 2);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  storage = Arcadia_Media_PixelBuffer_getPixelBytes(thread, a);
  Arcadia_Tests_assertEqualSize(thread, Arcadia_ByteArray_getSize(thread, storage), 16);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread, storage) == a->bytes);

  Arcadia_Media_PixelBuffer_setPixelFormat(thread, a,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  storage = Arcadia_Media_PixelBuffer_getPixelBytes(thread, a);
  Arcadia_Tests_assertEqualSize(thread, Arcadia_ByteArray_getSize(thread, storage), 20);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread, storage) == a->bytes);

  Arcadia_Media_PixelBuffer_setWidth(thread, a, 3);
  storage = Arcadia_Media_PixelBuffer_getPixelBytes(thread, a);
  Arcadia_Tests_assertEqualSize(thread, Arcadia_ByteArray_getSize(thread, storage), 28);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread, storage) == a->bytes);

  Arcadia_Media_PixelBuffer* b = Arcadia_Media_PixelBuffer_createClone(thread, a);
  Arcadia_Object_lock(thread, (Arcadia_Object*)b);
  Arcadia_Tests_assertTrue(thread, b->bytes != a->bytes);
  Arcadia_Tests_assertEqualSize(thread,
    Arcadia_ByteArray_getSize(thread, Arcadia_Media_PixelBuffer_getPixelBytes(thread, b)), 28);

  Arcadia_Media_PixelBuffer_swap(thread, a, b);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, a)) == a->bytes);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, b)) == b->bytes);

  Arcadia_Media_PixelBuffer_assign(thread, a, b);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, a->bytes != b->bytes);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, a)) == a->bytes);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)b);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)a);
}

static int
runByteArrayStorage(void)
{ return Arcadia_Tests_safeExecute(&testByteArrayStorage) ? EXIT_SUCCESS : EXIT_FAILURE; }

static void
expectBadPixelDimensions(Arcadia_Thread* thread, Arcadia_Integer32Value padding,
                         Arcadia_Integer32Value width, Arcadia_Integer32Value height)
{
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    (void)Arcadia_Media_PixelBuffer_create(thread, padding, width, height,
      Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
  }
}

static void
testPixelStorageBounds(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  expectBadPixelDimensions(thread, 0, -1, 2);
  expectBadPixelDimensions(thread, 0, 2, -1);
  expectBadPixelDimensions(thread, -1, 2, 2);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));

  // Existing padding/format conversions continue to retain the payload.
  Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 3, 2, 2,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, buffer, 1, 1, 19, 29, 39, 49);
  Arcadia_Media_PixelBuffer_setLinePadding(thread, buffer, 7);
  Arcadia_Media_PixelBuffer_setPixelFormat(thread, buffer,
    Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8);
  Arcadia_Natural8Value r, g, b, a;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, 1, 1, &r, &g, &b, &a);
  Arcadia_Tests_assertTrue(thread, r == 19 && g == 29 && b == 39 && a == 49);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, buffer)) == buffer->bytes);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, buffer)) == buffer->bytes);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
}

static void
expectPixelReadOutOfBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* buffer,
    Arcadia_Integer32Value column,
    Arcadia_Integer32Value row
  )
{
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Natural8Value r, g, b, a;
    Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, column, row, &r, &g, &b, &a);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
  }
}

static void
testPixelReadBounds
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 3, 2, 2,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, buffer, 1, 1, 11, 22, 33, 44);

  Arcadia_Natural8Value r, g, b, a;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, 1, 1, &r, &g, &b, &a);
  Arcadia_Tests_assertTrue(thread, r == 11 && g == 22 && b == 33 && a == 44);
  expectPixelReadOutOfBounds(thread, buffer, 2, 0);
  expectPixelReadOutOfBounds(thread, buffer, 0, 2);
  expectPixelReadOutOfBounds(thread, buffer, -1, 0);
  expectPixelReadOutOfBounds(thread, buffer, 0, -1);

  Arcadia_Media_PixelBuffer* empty = Arcadia_Media_PixelBuffer_create(thread, 0, 0, 0,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)empty);
  expectPixelReadOutOfBounds(thread, empty, 0, 0);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)empty);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
}

static int
runPixelReadBounds
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testPixelReadBounds) ? EXIT_SUCCESS : EXIT_FAILURE; }

static void
expectUnrepresentableLineStride
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value padding,
    Arcadia_Integer32Value width
  )
{
  // Zero height avoids allocating a large pixel image while testing the
  // distinct signed 32-bit line-stride API boundary.
  Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, padding, width, 0,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    (void)Arcadia_Media_PixelBuffer_getLineStride(thread, buffer);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
}

static void
testLineStrideBounds
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  expectUnrepresentableLineStride(thread, INT32_MAX, 1);
  expectUnrepresentableLineStride(thread, 0, INT32_MAX / 4 + 1);
  Arcadia_Tests_assertEqualSize(thread, stackSize, Arcadia_ValueStack_getSize(thread));

  Arcadia_Media_PixelBuffer* normal = Arcadia_Media_PixelBuffer_create(thread, 3, 2, 1,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)normal);
  Arcadia_Tests_assertEqualSize(thread, 11, Arcadia_Media_PixelBuffer_getLineStride(thread, normal));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)normal);
}

static int
runLineStrideBounds
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testLineStrideBounds) ? EXIT_SUCCESS : EXIT_FAILURE; }



static int
runPixelStorageBounds(void)
{ return Arcadia_Tests_safeExecute(&testPixelStorageBounds) ? EXIT_SUCCESS : EXIT_FAILURE; }

static void
pixelStorageReplacementStress(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Media_PixelBuffer* a = Arcadia_Media_PixelBuffer_create(thread, 3, 3, 2,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)a);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, a, 2, 1, 19, 29, 39, 49);
  for (Arcadia_SizeValue iteration = 0; iteration < 12; ++iteration) {
    Arcadia_Media_PixelBuffer_setPixelFormat(thread, a,
      iteration % 2 == 0 ? Arcadia_Media_PixelFormat_BlueGreenRedNatural8 :
                           Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
    Arcadia_Media_PixelBuffer_setLinePadding(thread, a, (Arcadia_Integer32Value)(iteration % 5));
    Arcadia_Natural8Value r, g, b, alpha;
    Arcadia_Media_PixelBuffer_getPixelRGBA(thread, a, 2, 1, &r, &g, &b, &alpha);
    Arcadia_Tests_assertTrue(thread, r == 19 && g == 29 && b == 39);
    Arcadia_Tests_assertTrue(thread, alpha == 255);
    Arcadia_ByteArray* storage = Arcadia_Media_PixelBuffer_getPixelBytes(thread, a);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread, storage) == a->bytes);
    Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
      Arcadia_Media_PixelBuffer_getPixelBytes(thread, a)) == a->bytes);
  }
  Arcadia_Media_PixelBuffer* b = Arcadia_Media_PixelBuffer_createClone(thread, a);
  Arcadia_Object_lock(thread, (Arcadia_Object*)b);
  Arcadia_Tests_assertTrue(thread, a->bytes != b->bytes);
  Arcadia_Media_PixelBuffer_assign(thread, b, a);
  Arcadia_Tests_assertTrue(thread, a->bytes != b->bytes);
  Arcadia_Media_PixelBuffer_swap(thread, a, b);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, a)) == a->bytes);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_PixelBuffer_getPixelBytes(thread, b)) == b->bytes);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)b);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)a);
}

static int
runPixelStorageReplacementStress(void)
{ return Arcadia_Tests_safeExecute(&pixelStorageReplacementStress) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ByteArrayStorage", &runByteArrayStorage },
    { "PixelStorageBounds", &runPixelStorageBounds },
    { "PixelReadBounds", &runPixelReadBounds },
    { "LineStrideBounds", &runLineStrideBounds },
    { "PixelStorageReplacementStress", &runPixelStorageReplacementStress },
    { "Operations", &Arcadia_Media_Tests_PixelBuffer_operations },
    { "Fills", &Arcadia_Media_Tests_PixelBuffer_fills },
    { "FormatMetadata", &Arcadia_Media_Tests_PixelBuffer_formatMetadata },
    { "FormatQueries", &Arcadia_Media_Tests_PixelBuffer_formatQueries },
    { "FormatConsumers", &Arcadia_Media_Tests_PixelBuffer_formatConsumers },
    { "Resize", &Arcadia_Media_Tests_PixelBuffer_resize },
    { "BulkResize", &Arcadia_Media_Tests_PixelBuffer_bulkResize },
    { "Composite", &Arcadia_Media_Tests_PixelBuffer_composite },
    { "PixelValues", &Arcadia_Media_Tests_PixelBuffer_pixelValues },
    { "PixelValueInterpolation", &Arcadia_Media_Tests_PixelBuffer_pixelValueInterpolation },
    { "PixelValueColorConversions", &Arcadia_Media_Tests_PixelBuffer_pixelValueColorConversions },
    { "NamedColors", &Arcadia_Media_Tests_PixelBuffer_namedColors },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
