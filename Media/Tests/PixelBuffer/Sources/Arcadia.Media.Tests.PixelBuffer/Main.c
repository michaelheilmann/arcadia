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

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ByteArrayStorage", &runByteArrayStorage },
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
