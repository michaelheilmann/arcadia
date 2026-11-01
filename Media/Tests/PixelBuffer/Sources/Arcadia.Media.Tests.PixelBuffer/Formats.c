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

#include "Formats.h"
#include "Arcadia/Media/Include.h"
#include <stdio.h>
#include <string.h>

_Static_assert(Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8 == 0, "ABGR value");
_Static_assert(Arcadia_Media_PixelFormat_AlphaGrayNatural8 == 1, "AG value");
_Static_assert(Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8 == 2, "ARGB value");
_Static_assert(Arcadia_Media_PixelFormat_BlueGreenRedNatural8 == 3, "BGR value");
_Static_assert(Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8 == 4, "BGRA value");
_Static_assert(Arcadia_Media_PixelFormat_GrayNatural8 == 5, "G value");
_Static_assert(Arcadia_Media_PixelFormat_GrayAlphaNatural8 == 6, "GA value");
_Static_assert(Arcadia_Media_PixelFormat_RedGreenBlueNatural8 == 7, "RGB value");
_Static_assert(Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8 == 8, "RGBA value");
_Static_assert(sizeof(Arcadia_Media_PixelFormat) == sizeof(Arcadia_Integer32Value), "enum size");
_Static_assert(_Alignof(Arcadia_Media_PixelFormat) == _Alignof(Arcadia_Integer32Value), "enum alignment");
_Static_assert(_Generic(&Arcadia_Media_PixelFormat_getNumberOfBytes, Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_PixelFormat): 1, default: 0), "byte query signature");
_Static_assert(_Generic(&Arcadia_Media_PixelFormat_getNumberOfComponents, Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_PixelFormat): 1, default: 0), "component query signature");
_Static_assert(_Generic(&_Arcadia_Media_PixelFormat_getType, Arcadia_TypeValue (*)(Arcadia_Thread*): 1, default: 0), "descriptor signature");

static const Arcadia_SizeValue byteCounts[] = { 4, 2, 4, 3, 4, 1, 2, 3, 4 };
static const Arcadia_SizeValue componentCounts[] = { 4, 2, 4, 3, 4, 1, 2, 3, 4 };

static void
testMetadata
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_TypeValue type = _Arcadia_Media_PixelFormat_getType(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Enumeration);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  const char expected[] = "Arcadia.Media.PixelFormat";
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == sizeof(expected) - 1 && !memcmp(Arcadia_Name_getBytes(thread, name), expected, sizeof(expected) - 1));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  for (Arcadia_Integer32Value i = 0; i < 9; ++i) {
    Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, i));
    Arcadia_ValueStack_pushValue(thread, &value);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, _Arcadia_Media_PixelFormat_getType(thread) == type);
  for (Arcadia_Integer32Value i = 0; i < 9; ++i) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, 8 - i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isEnumerationValue(&value));
    Arcadia_EnumerationValue enumeration = Arcadia_Value_getEnumerationValue(&value);
    Arcadia_Tests_assertTrue(thread, enumeration.type == type && enumeration.value == i);
    Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelFormat_getNumberOfBytes(thread, (Arcadia_Media_PixelFormat)enumeration.value) == byteCounts[i]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelFormat_getNumberOfComponents(thread, (Arcadia_Media_PixelFormat)enumeration.value) == componentCounts[i]);
  }
  Arcadia_ValueStack_popValues(thread, 9);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
expectUnsupported
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value format,
    Arcadia_SizeValue (*query)(Arcadia_Thread*, Arcadia_Media_PixelFormat)
  )
{
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    query(thread, (Arcadia_Media_PixelFormat)format);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
  }
}

static void
testQueries
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (Arcadia_Integer32Value i = -16; i <= 24; ++i) {
    if (i >= 0 && i < 9) {
      Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelFormat_getNumberOfBytes(thread, (Arcadia_Media_PixelFormat)i) == byteCounts[i]);
      Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelFormat_getNumberOfComponents(thread, (Arcadia_Media_PixelFormat)i) == componentCounts[i]);
    } else {
      expectUnsupported(thread, i, &Arcadia_Media_PixelFormat_getNumberOfBytes);
      expectUnsupported(thread, i, &Arcadia_Media_PixelFormat_getNumberOfComponents);
    }
  }
  const Arcadia_Integer32Value extremes[] = { INT32_MIN, INT32_MAX, 255, 256, 65536 };
  for (unsigned i = 0; i < sizeof(extremes) / sizeof(extremes[0]); ++i) {
    expectUnsupported(thread, extremes[i], &Arcadia_Media_PixelFormat_getNumberOfBytes);
    expectUnsupported(thread, extremes[i], &Arcadia_Media_PixelFormat_getNumberOfComponents);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  for (unsigned i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, i) == 77);
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testConsumers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Media_PixelFormat formats[] = {
    Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
  };
  for (unsigned i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 5, 3, 2, formats[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
    Arcadia_SizeValue bytes = (byteCounts[formats[i]] * 3 + 5) * 2;
    Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, buffer) == byteCounts[formats[i]]);
    memset(buffer->bytes, 0xa7, bytes);
    for (Arcadia_Integer32Value y = 0; y < 2; ++y) {
      for (Arcadia_Integer32Value x = 0; x < 3; ++x) Arcadia_Media_PixelBuffer_setPixelRGBA(thread, buffer, x, y, 17 + x, 53 + y, 101 + x + y, 149);
    }
    // Include 3-to-4-byte expansion: the old native conversion allocated
    // source-sized storage and overran it on the final target row.
    for (unsigned j = 0; j < sizeof(formats) / sizeof(formats[0]); ++j) {
      Arcadia_Media_PixelBuffer* clone = Arcadia_Media_PixelBuffer_createClone(thread, buffer);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, clone);
      Arcadia_Tests_assertTrue(thread, clone != buffer && clone->bytes != buffer->bytes && !memcmp(buffer->bytes, clone->bytes, bytes));
      Arcadia_Media_PixelBuffer_setPixelFormat(thread, clone, formats[j]);
      Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, clone) == byteCounts[formats[j]]);
      Arcadia_Tests_assertTrue(thread, clone->linePadding == 5 && clone->width == 3 && clone->height == 2 && clone->pixelFormat == formats[j]);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      for (Arcadia_Integer32Value y = 0; y < 2; ++y) {
        for (Arcadia_Integer32Value x = 0; x < 3; ++x) {
          Arcadia_Natural8Value r, g, b, a;
          Arcadia_Media_PixelBuffer_getPixelRGBA(thread, clone, x, y, &r, &g, &b, &a);
          Arcadia_Tests_assertTrue(thread, r == 17 + x && g == 53 + y && b == 101 + x + y);
          Arcadia_Tests_assertTrue(thread, a == (byteCounts[formats[i]] == 3 || byteCounts[formats[j]] == 3 ? 255 : 149));
        }
      }
      Arcadia_ValueStack_popValues(thread, 1);
    }
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testResize
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  const Arcadia_Media_PixelFormat formats[] = {
    Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
  };
  for (unsigned i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 5, 3, 2, formats[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
    Arcadia_SizeValue oldStride = 3 * byteCounts[formats[i]] + 5;
    // A guard tail makes an old out-of-bounds fill fail an assertion without
    // corrupting the allocator before the test can report the failure.
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&buffer->bytes, oldStride * 2 + 16);
    memset(buffer->bytes, 0xa7, oldStride * 2 + 16);
    Arcadia_Media_PixelBuffer_fill(thread, buffer, 17, 53, 101, 149);
    // In particular, BGRA must advance each pixel once, not twice.
    for (Arcadia_SizeValue y = 0; y < 2; ++y) {
      for (Arcadia_SizeValue x = 3 * byteCounts[formats[i]]; x < oldStride; ++x) {
        Arcadia_Tests_assertTrue(thread, buffer->bytes[y * oldStride + x] == 0xa7);
      }
    }
    for (Arcadia_SizeValue x = oldStride * 2; x < oldStride * 2 + 16; ++x) {
      Arcadia_Tests_assertTrue(thread, buffer->bytes[x] == 0xa7);
    }
    Arcadia_Media_PixelBuffer_setWidth(thread, buffer, 7);
    Arcadia_Media_PixelBuffer_setHeight(thread, buffer, 5);
    Arcadia_Tests_assertTrue(thread, buffer->width == 7 && buffer->height == 5 && buffer->linePadding == 5 && buffer->pixelFormat == formats[i]);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    for (Arcadia_Integer32Value y = 0; y < 5; ++y) {
      for (Arcadia_Integer32Value x = 0; x < 7; ++x) {
        Arcadia_Natural8Value r, g, b, a;
        Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, x, y, &r, &g, &b, &a);
        Arcadia_BooleanValue original = x < 3 && y < 2;
        Arcadia_Tests_assertTrue(thread, r == (original ? 17 : 0) && g == (original ? 53 : 0) && b == (original ? 101 : 0));
        Arcadia_Tests_assertTrue(thread, a == (original && byteCounts[formats[i]] == 4 ? 149 : 255));
      }
    }
    Arcadia_Media_PixelBuffer_setWidth(thread, buffer, 2);
    Arcadia_Media_PixelBuffer_setHeight(thread, buffer, 1);
    Arcadia_Natural8Value* bytes = buffer->bytes;
    Arcadia_Media_PixelBuffer_setWidth(thread, buffer, 2);
    Arcadia_Media_PixelBuffer_setHeight(thread, buffer, 1);
    Arcadia_Tests_assertTrue(thread, buffer->bytes == bytes && buffer->width == 2 && buffer->height == 1 && buffer->linePadding == 5 && buffer->pixelFormat == formats[i]);
    for (Arcadia_Integer32Value x = 0; x < 2; ++x) {
      Arcadia_Natural8Value r, g, b, a;
      Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, x, 0, &r, &g, &b, &a);
      Arcadia_Tests_assertTrue(thread, r == 17 && g == 53 && b == 101 && a == (byteCounts[formats[i]] == 4 ? 149 : 255));
    }
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testBulkResize
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 5, 256, 128, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
  Arcadia_Media_PixelBuffer_fill(thread, buffer, 17, 53, 101, 149);
  Arcadia_Media_PixelBuffer_setWidth(thread, buffer, 512);
  Arcadia_Media_PixelBuffer_setHeight(thread, buffer, 256);
  Arcadia_SizeValue stride = 512 * 4 + 5;
  for (Arcadia_SizeValue y = 0; y < 256; ++y) {
    for (Arcadia_SizeValue x = 0; x < 512; ++x) {
      Arcadia_Natural8Value* pixel = buffer->bytes + y * stride + x * 4;
      Arcadia_BooleanValue original = x < 256 && y < 128;
      Arcadia_Tests_assertTrue(thread, pixel[0] == (original ? 17 : 0) && pixel[1] == (original ? 53 : 0) && pixel[2] == (original ? 101 : 0) && pixel[3] == (original ? 149 : 255));
    }
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Media_Tests_PixelBuffer_formatMetadata
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testMetadata)) { fprintf(stderr, "pixel format metadata failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}

int
Arcadia_Media_Tests_PixelBuffer_formatQueries
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testQueries)) { fprintf(stderr, "pixel format queries failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}

int
Arcadia_Media_Tests_PixelBuffer_formatConsumers
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testConsumers)) { fprintf(stderr, "pixel format consumers failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}

int
Arcadia_Media_Tests_PixelBuffer_resize
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testResize)) { fprintf(stderr, "pixel buffer resize failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}

int
Arcadia_Media_Tests_PixelBuffer_bulkResize
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testBulkResize)) { fprintf(stderr, "pixel buffer bulk resize failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
