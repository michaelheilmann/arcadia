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

#include "Arcadia.Media.Tests.PixelBuffer/Composite.h"

#include <stdlib.h>

static void
expectPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* buffer,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y,
    Arcadia_Natural8Value er,
    Arcadia_Natural8Value eg,
    Arcadia_Natural8Value eb,
    Arcadia_Natural8Value ea
  )
{
  Arcadia_Natural8Value r, g, b, a;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, x, y, &r, &g, &b, &a);
  Arcadia_Tests_assertTrue(thread, r == er && g == eg && b == eb && a == ea);
}

static Arcadia_BooleanValue
hasAlpha
  (
    Arcadia_Media_PixelFormat format
  )
{
  return format != Arcadia_Media_PixelFormat_BlueGreenRedNatural8 &&
         format != Arcadia_Media_PixelFormat_RedGreenBlueNatural8;
}

static void
testAllFormats
  (
    Arcadia_Thread* thread
  )
{
  static Arcadia_Media_PixelFormat const formats[] = {
    Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    for (Arcadia_SizeValue j = 0; j < sizeof(formats) / sizeof(formats[0]); ++j) {
      Arcadia_Media_PixelBuffer* destination = Arcadia_Media_PixelBuffer_create(thread, 3, 1, 1, formats[i]);
      Arcadia_Object_lock(thread, (Arcadia_Object*)destination);
      Arcadia_Media_PixelBuffer* source = Arcadia_Media_PixelBuffer_create(thread, 5, 1, 1, formats[j]);
      Arcadia_Object_lock(thread, (Arcadia_Object*)source);
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, destination, 0, 0, 10, 20, 30, 64);
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, source, 0, 0, 200, 100, 50, 128);

      Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 0, 0, source, 0, 0, 1, 1, Arcadia_Media_PixelBufferCompositeMode_Copy);
      expectPixel(thread, destination, 0, 0, 200, 100, 50,
                  hasAlpha(formats[i]) ? (hasAlpha(formats[j]) ? 128 : 255) : 255);

      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, destination, 0, 0, 10, 20, 30, 64);
      Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 0, 0, source, 0, 0, 1, 1, Arcadia_Media_PixelBufferCompositeMode_SourceOver);
      if (!hasAlpha(formats[j])) {
        expectPixel(thread, destination, 0, 0, 200, 100, 50, 255);
      } else if (!hasAlpha(formats[i])) {
        expectPixel(thread, destination, 0, 0, 105, 60, 40, 255);
      } else {
        expectPixel(thread, destination, 0, 0, 162, 84, 46, 160);
      }
      Arcadia_Object_unlock(thread, (Arcadia_Object*)source);
      Arcadia_Object_unlock(thread, (Arcadia_Object*)destination);
    }
  }
}

static void
testOverlap
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread, 2, 4, 4, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
  for (Arcadia_Integer32Value y = 0; y < 4; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 4; ++x) {
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, x, y, (Arcadia_Natural8Value)(10 * y + x), 0, 0, 255);
    }
  }
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, pixels, 0, 1, pixels, 0, 0, 4, 3, Arcadia_Media_PixelBufferCompositeMode_Copy);
  for (Arcadia_Integer32Value y = 1; y < 4; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 4; ++x) {
      expectPixel(thread, pixels, x, y, (Arcadia_Natural8Value)(10 * (y - 1) + x), 0, 0, 255);
    }
  }
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, pixels, 1, 1, pixels, 0, 1, 3, 1, Arcadia_Media_PixelBufferCompositeMode_Copy);
  expectPixel(thread, pixels, 0, 1, 0, 0, 0, 255);
  expectPixel(thread, pixels, 1, 1, 0, 0, 0, 255);
  expectPixel(thread, pixels, 2, 1, 1, 0, 0, 255);
  expectPixel(thread, pixels, 3, 1, 2, 0, 0, 255);

  for (Arcadia_Integer32Value x = 0; x < 4; ++x) {
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, x, 0, (Arcadia_Natural8Value)(x * 40), 0, 0, 128);
  }
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, pixels, 1, 0, pixels, 0, 0, 3, 1, Arcadia_Media_PixelBufferCompositeMode_SourceOver);
  // x=2 must use original source x=1 (red=40), not modified source x=1.
  expectPixel(thread, pixels, 2, 0, 53, 0, 0, 192);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
}

static void
testReverseOverlap
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread, 0, 4, 4, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
  for (Arcadia_Integer32Value y = 0; y < 4; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 4; ++x) {
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, x, y, (Arcadia_Natural8Value)(10 * y + x), 0, 0, 255);
    }
  }
  // Moving upward must not overwrite later source rows.
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, pixels, 0, 0, pixels, 0, 1, 4, 3, Arcadia_Media_PixelBufferCompositeMode_Copy);
  for (Arcadia_Integer32Value y = 0; y < 3; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 4; ++x) {
      expectPixel(thread, pixels, x, y, (Arcadia_Natural8Value)(10 * (y + 1) + x), 0, 0, 255);
    }
  }
  // Moving left must visit each row from left to right.
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, pixels, 0, 0, pixels, 1, 0, 3, 1, Arcadia_Media_PixelBufferCompositeMode_Copy);
  expectPixel(thread, pixels, 0, 0, 11, 0, 0, 255);
  expectPixel(thread, pixels, 1, 0, 12, 0, 0, 255);
  expectPixel(thread, pixels, 2, 0, 13, 0, 0, 255);
  // Diagonal down-right moves must also preserve source pixels.
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, pixels, 1, 1, pixels, 0, 0, 3, 3, Arcadia_Media_PixelBufferCompositeMode_Copy);
  expectPixel(thread, pixels, 1, 1, 11, 0, 0, 255);
  expectPixel(thread, pixels, 3, 3, 32, 0, 0, 255);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
}

static void
testClipAndPadding
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBuffer* destination = Arcadia_Media_PixelBuffer_create(thread, 3, 3, 3, Arcadia_Media_PixelFormat_RedGreenBlueNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)destination);
  Arcadia_Media_PixelBuffer* source = Arcadia_Media_PixelBuffer_create(thread, 1, 3, 3, Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)source);
  for (Arcadia_Integer32Value y = 0; y < 3; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 3; ++x) {
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, destination, x, y, 0, 0, 0, 255);
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, source, x, y, 255, 0, 0, 255);
    }
  }
  Arcadia_Integer32Value stride = Arcadia_Media_PixelBuffer_getLineStride(thread, destination);
  for (Arcadia_Integer32Value y = 0; y < 3; ++y) {
    for (Arcadia_Integer32Value p = 0; p < 3; ++p) {
      destination->bytes[(Arcadia_SizeValue)y * stride + 9 + p] = 0xA5;
    }
  }
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, -1, 1, source, 0, 0, 3, 3, Arcadia_Media_PixelBufferCompositeMode_Copy);
  expectPixel(thread, destination, 0, 0, 0, 0, 0, 255);
  expectPixel(thread, destination, 0, 1, 255, 0, 0, 255);
  expectPixel(thread, destination, 1, 1, 255, 0, 0, 255);
  expectPixel(thread, destination, 2, 1, 0, 0, 0, 255);
  expectPixel(thread, destination, 0, 2, 255, 0, 0, 255);
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 0, 0, source, 2, 2, 3, 3, Arcadia_Media_PixelBufferCompositeMode_Copy);
  expectPixel(thread, destination, 0, 0, 255, 0, 0, 255);
  expectPixel(thread, destination, 1, 0, 0, 0, 0, 255);
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 0, 0, source, 0, 0, 0, 2, Arcadia_Media_PixelBufferCompositeMode_Copy);
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 99, 99, source, 0, 0, 3, 3, Arcadia_Media_PixelBufferCompositeMode_Copy);
  for (Arcadia_Integer32Value y = 0; y < 3; ++y) {
    for (Arcadia_Integer32Value p = 0; p < 3; ++p) {
      Arcadia_Tests_assertTrue(thread, destination->bytes[(Arcadia_SizeValue)y * stride + 9 + p] == 0xA5);
    }
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)source);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)destination);
}

static void
testTransparent
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBuffer* destination = Arcadia_Media_PixelBuffer_create(thread, 0, 1, 1, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)destination);
  Arcadia_Media_PixelBuffer* source = Arcadia_Media_PixelBuffer_create(thread, 0, 1, 1, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)source);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, destination, 0, 0, 100, 50, 25, 0);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, source, 0, 0, 200, 80, 40, 0);
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 0, 0, source, 0, 0, 1, 1, Arcadia_Media_PixelBufferCompositeMode_SourceOver);
  expectPixel(thread, destination, 0, 0, 0, 0, 0, 0);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, source, 0, 0, 200, 80, 40, 255);
  Arcadia_Media_PixelBuffer_compositeRectangle(thread, destination, 0, 0, source, 0, 0, 1, 1, Arcadia_Media_PixelBufferCompositeMode_SourceOver);
  expectPixel(thread, destination, 0, 0, 200, 80, 40, 255);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)source);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)destination);
}

void
Arcadia_Media_Tests_PixelBuffer_testComposite
  (
    Arcadia_Thread* thread
  )
{
  testAllFormats(thread);
  testOverlap(thread);
  testReverseOverlap(thread);
  testClipAndPadding(thread);
  testTransparent(thread);
}

int
Arcadia_Media_Tests_PixelBuffer_composite
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Media_Tests_PixelBuffer_testComposite) ? EXIT_SUCCESS : EXIT_FAILURE;
}
