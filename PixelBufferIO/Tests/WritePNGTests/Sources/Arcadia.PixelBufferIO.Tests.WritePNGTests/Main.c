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

#include "Arcadia/PixelBufferIO/Include.h"

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_String* extension = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"png", sizeof(u8"png") - 1)));
  Arcadia_Imaging_ImageWriterParameters* parameters = NULL;
  Arcadia_String* path = NULL;
  Arcadia_List* pixelBufferList = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Media_PixelBuffer* pixelBuffer = NULL;
  Arcadia_Imaging_ImageManager* imageManager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, imageManager, extension);
  Arcadia_Tests_assertTrue(thread, 0 != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers));
  Arcadia_Object* object = Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, object, _Arcadia_Imaging_ImageWriter_getType(thread)));
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)object;

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_RedGreenBlueNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"rn8gn8bn8.png", sizeof(u8"rn8gn8bn8.png") - 1)));
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"rn8gn8bn8an8.png", sizeof(u8"rn8gn8bn8an8.png") - 1)));
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"an8rn8gn8bn8.png", sizeof(u8"an8rn8gn8bn8.png") - 1)));
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_BlueGreenRedNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"bn8gn8rn8.png", sizeof(u8"bn8gn8rn8.png") - 1)));
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"bn8gn8rn8an8.png", sizeof(u8"bn8gn8rn8an8.png") - 1)));
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"an8bn8gn8rn8.png", sizeof(u8"an8bn8gn8rn8.png") - 1)));
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);
}

// Exercise the real writer against ByteArray-owned pixel storage (including
// padded lines), while forcing a collection before the writer borrows bytes.
static void
testManagedPixelStorage(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_String* extension = Arcadia_String_createFromCxxString(thread, u8"png");
  Arcadia_Object_lock(thread, (Arcadia_Object*)extension);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  // Filtering allocates a list; keep the weakly cached manager alive while
  // obtaining and using its registered writer instances.
  Arcadia_Object_lock(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, extension);
  Arcadia_Object_lock(thread, (Arcadia_Object*)writers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
    Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_Object_lock(thread, (Arcadia_Object*)writer);
  // Check the writer's own supportedTypes reference independently of WIC.
  // Without an object visitor this list is reclaimed by the first GC pass.
  for (Arcadia_SizeValue gcPass = 0; gcPass < 3; ++gcPass) {
    Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
    Arcadia_ImmutableList* supported = Arcadia_Imaging_ImageWriter_getSupportedTypes(thread, writer);
    Arcadia_Tests_assertTrue(thread, Arcadia_List_contains(thread, (Arcadia_List*)supported,
      Arcadia_Value_makeObjectReferenceValue(extension)));
  }
  Arcadia_List* sources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sources);

  const Arcadia_Media_PixelFormat formats[] = {
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,
    Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8,
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    Arcadia_Media_PixelBuffer* pixels =
      Arcadia_Media_PixelBuffer_create(thread, 3, 8, 5, formats[i]);
    Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 2, 3, 11, 22, 33, 44);
    Arcadia_ByteArray* storage = Arcadia_Media_PixelBuffer_getPixelBytes(thread, pixels);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isMutable(thread, storage));
    Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, storage));
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, storage));
    Arcadia_Tests_assertEqualSize(thread, (8 *
      Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, pixels) + 3) * 5,
      Arcadia_ByteArray_getSize(thread, storage));
    Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
    Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getPixelBytes(thread, pixels) == storage);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, storage));
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)sources);
    Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
    Arcadia_ByteArrayBuilder* bytes = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)bytes);
    Arcadia_Imaging_ImageWriterParameters* parameters =
      Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, bytes, extension);
    Arcadia_Object_lock(thread, (Arcadia_Object*)parameters);
    Arcadia_Imaging_ImageWriter_write(thread, writer, sources, parameters);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_getSize(thread, bytes) > 16);
    Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getPixelBytes(thread, pixels) == storage);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArray_isContiguous(thread, storage));
    Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
    Arcadia_Object_unlock(thread, (Arcadia_Object*)parameters);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)bytes);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sources);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)writer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)writers);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)manager);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)extension);
}

static int
runManagedPixelStorage(void)
{ return Arcadia_Tests_safeExecute(&testManagedPixelStorage) ? EXIT_SUCCESS : EXIT_FAILURE; }

// BMP is intentionally not a supported image output format on any platform.
static void
testBMPUnsupported(Arcadia_Thread* thread)
{
  Arcadia_String* extension = Arcadia_String_createFromCxxString(thread, u8"bmp");
  Arcadia_Object_lock(thread, (Arcadia_Object*)extension);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, extension);
  Arcadia_Object_lock(thread, (Arcadia_Object*)writers);
  Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)writers);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)manager);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)extension);
}

static int
runBMPUnsupported(void)
{ return Arcadia_Tests_safeExecute(&testBMPUnsupported) ? EXIT_SUCCESS : EXIT_FAILURE; }


static uint32_t
pngRead32
  (
    uint8_t const* bytes
  )
{
  return ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
    ((uint32_t)bytes[2] << 8) | (uint32_t)bytes[3];
}

static void
assertExpectedPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* image,
    int x,
    int y,
    uint8_t r,
    uint8_t g,
    uint8_t b,
    uint8_t a
  )
{
  Arcadia_Natural8Value actualR, actualG, actualB, actualA;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, image, x, y,
    &actualR, &actualG, &actualB, &actualA);
  Arcadia_Tests_assertTrue(thread,
    actualR == r && actualG == g && actualB == b && actualA == a);
}

static Arcadia_BooleanValue
matchesKnownPixels
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* image
  );

// Check independently specified encoded pixels for every supported source
// byte ordering. All images have nonzero row padding and adjacent contrasts.
static void
testEncodedPixels
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_String* format = Arcadia_String_createFromCxxString(thread, u8"png");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)format);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)writers);
  Arcadia_List* readers = Arcadia_Imaging_ImageManager_getReaders(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)readers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)readers));
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
    Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_Imaging_ImageReader* reader = (Arcadia_Imaging_ImageReader*)
    Arcadia_List_getObjectReferenceValueAt(thread, readers, 0);

  Arcadia_List* sources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sources);
  Arcadia_ByteArrayBuilder* output = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)output);
  Arcadia_Imaging_ImageWriterParameters* target =
    Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, output, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);

  Arcadia_Media_PixelFormat const formats[] = {
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,
    Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8,
  };
  uint8_t const signature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };
  for (Arcadia_SizeValue i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    Arcadia_ByteArrayBuilder_clear(thread, output);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)sources);
    Arcadia_Media_PixelBuffer* pixels =
      Arcadia_Media_PixelBuffer_create(thread, 5, 3, 2, formats[i]);
    Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 0, 255, 0, 0, 255);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 0, 0, 255, 0, 128);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 2, 0, 0, 0, 255, 64);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 1, 10, 20, 30, 0);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 1, 240, 120, 60, 192);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 2, 1, 255, 255, 255, 255);
    Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
    Arcadia_Imaging_ImageWriter_write(thread, writer, sources, target);
    Arcadia_SizeValue size = Arcadia_ByteArrayBuilder_getSize(thread, output);
    Arcadia_Tests_assertTrue(thread, size >= 45);
    uint8_t const* bytes = (uint8_t const*)Arcadia_ByteArrayBuilder_getBytes(thread, output);
    Arcadia_Tests_assertTrue(thread, bytes != NULL);
    Arcadia_Tests_assertTrue(thread, memcmp(bytes, signature, sizeof(signature)) == 0);
    Arcadia_Tests_assertTrue(thread, pngRead32(bytes + 8) == 13);
    Arcadia_Tests_assertTrue(thread, memcmp(bytes + 12, "IHDR", 4) == 0);
    Arcadia_Tests_assertTrue(thread,
      pngRead32(bytes + 16) == 3 && pngRead32(bytes + 20) == 2);
    Arcadia_Tests_assertTrue(thread, bytes[24] == 8);
    Arcadia_Tests_assertTrue(thread, bytes[25] == (i == 0 || i == 2 ? 2 : 6));

    Arcadia_ByteArray* encoded = Arcadia_ByteArrayFactory_borrowReadOnly(thread, bytes, size);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)encoded);
    Arcadia_Imaging_ImageReaderParameters* input =
      Arcadia_Imaging_ImageReaderParameters_createByteArray(thread, encoded, format);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
    Arcadia_List* images = Arcadia_Imaging_ImageReader_read(thread, reader, input);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)images);
    Arcadia_Tests_assertEqualSize(thread, 1,
      Arcadia_Collection_getSize(thread, (Arcadia_Collection*)images));
    Arcadia_Media_PixelBuffer* decoded = (Arcadia_Media_PixelBuffer*)
      Arcadia_List_getObjectReferenceValueAt(thread, images, 0);
    Arcadia_Tests_assertTrue(thread,
      Arcadia_Media_PixelBuffer_getWidth(thread, decoded) == 3 &&
      Arcadia_Media_PixelBuffer_getHeight(thread, decoded) == 2);
    Arcadia_Tests_assertTrue(thread,
      Arcadia_Media_PixelBuffer_getPixelFormat(thread, decoded) ==
        Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
    uint8_t const alpha0 = i == 0 || i == 2 ? 255 : 128;
    uint8_t const alpha1 = i == 0 || i == 2 ? 255 : 64;
    uint8_t const alpha2 = i == 0 || i == 2 ? 255 : 0;
    uint8_t const alpha3 = i == 0 || i == 2 ? 255 : 192;
    assertExpectedPixel(thread, decoded, 0, 0, 255, 0, 0, 255);
    assertExpectedPixel(thread, decoded, 1, 0, 0, 255, 0, alpha0);
    assertExpectedPixel(thread, decoded, 2, 0, 0, 0, 255, alpha1);
    assertExpectedPixel(thread, decoded, 0, 1, 10, 20, 30, alpha2);
    assertExpectedPixel(thread, decoded, 1, 1, 240, 120, 60, alpha3);
    assertExpectedPixel(thread, decoded, 2, 1, 255, 255, 255, 255);
    if (i != 0 && i != 2) {
      Arcadia_Tests_assertTrue(thread, matchesKnownPixels(thread, decoded));
    }
    Arcadia_ValueStack_popValues(thread, 3);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runEncodedPixels
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testEncodedPixels) ? EXIT_SUCCESS : EXIT_FAILURE;
}


static Arcadia_BooleanValue
matchesKnownPixels
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* image
  )
{
  static uint8_t const expected[2][3][4] = {
    { {255, 0, 0, 255}, {0, 255, 0, 128}, {0, 0, 255, 64} },
    { {10, 20, 30, 0}, {240, 120, 60, 192}, {255, 255, 255, 255} }
  };
  if (Arcadia_Media_PixelBuffer_getWidth(thread, image) != 3 ||
      Arcadia_Media_PixelBuffer_getHeight(thread, image) != 2) {
    return Arcadia_BooleanValue_False;
  }
  for (int y = 0; y < 2; ++y) {
    for (int x = 0; x < 3; ++x) {
      Arcadia_Natural8Value r, g, b, a;
      Arcadia_Media_PixelBuffer_getPixelRGBA(thread, image, x, y, &r, &g, &b, &a);
      if (r != expected[y][x][0] || g != expected[y][x][1] ||
          b != expected[y][x][2] || a != expected[y][x][3]) {
        return Arcadia_BooleanValue_False;
      }
    }
  }
  return Arcadia_BooleanValue_True;
}

static void
testMutationDetection
  (
    Arcadia_Thread* thread
  )
{
  static uint8_t const expected[2][3][4] = {
    { {255, 0, 0, 255}, {0, 255, 0, 128}, {0, 0, 255, 64} },
    { {10, 20, 30, 0}, {240, 120, 60, 192}, {255, 255, 255, 255} }
  };
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread,
    5, 3, 2, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
  for (int y = 0; y < 2; ++y) {
    for (int x = 0; x < 3; ++x) {
      Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, x, y,
        expected[y][x][0], expected[y][x][1],
        expected[y][x][2], expected[y][x][3]);
    }
  }
  Arcadia_Tests_assertTrue(thread, matchesKnownPixels(thread, pixels));
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 0, 0, 0, 255, 255);
  Arcadia_Tests_assertFalse(thread, matchesKnownPixels(thread, pixels));
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 0, 255, 0, 0, 255);
  Arcadia_Media_PixelBuffer_swapRows(thread, pixels, 0, 1);
  Arcadia_Tests_assertFalse(thread, matchesKnownPixels(thread, pixels));
  Arcadia_Media_PixelBuffer_swapRows(thread, pixels, 0, 1);
  Arcadia_Media_PixelBuffer_swapColumns(thread, pixels, 0, 1);
  Arcadia_Tests_assertFalse(thread, matchesKnownPixels(thread, pixels));
  Arcadia_Media_PixelBuffer_swapColumns(thread, pixels, 0, 1);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 0, 0, 255, 0, 255);
  Arcadia_Tests_assertFalse(thread, matchesKnownPixels(thread, pixels));
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 0, 0, 255, 0, 128);
  Arcadia_Tests_assertTrue(thread, matchesKnownPixels(thread, pixels));
  Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
}

static int
runMutationDetection
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMutationDetection) ? EXIT_SUCCESS : EXIT_FAILURE;
}


static void
testCorruptedEncodedPNG
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget outer;
  Arcadia_Thread_pushJumpTarget(thread, &outer);
  if (!Arcadia_JumpTarget_save(&outer)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* format = Arcadia_String_createFromCxxString(thread, u8"png");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)format);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)writers);
  Arcadia_List* readers = Arcadia_Imaging_ImageManager_getReaders(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)readers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)readers));
  Arcadia_List* sources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sources);
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread,
    3, 2, 2, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
  Arcadia_Media_PixelBuffer_fill(thread, pixels, 25, 50, 75, 128);
  Arcadia_ByteArrayBuilder* output = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)output);
  Arcadia_Imaging_ImageWriterParameters* target =
    Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, output, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  Arcadia_Imaging_ImageWriter_write(thread,
    (Arcadia_Imaging_ImageWriter*)Arcadia_List_getObjectReferenceValueAt(thread, writers, 0),
    sources, target);
  Arcadia_SizeValue size = Arcadia_ByteArrayBuilder_getSize(thread, output);
  Arcadia_Tests_assertTrue(thread, size > 41);
  uint8_t const* bytes = (uint8_t const*)Arcadia_ByteArrayBuilder_getBytes(thread, output);
  Arcadia_Tests_assertTrue(thread, pngRead32(bytes + 8) == 13 &&
    memcmp(bytes + 12, "IHDR", 4) == 0);
  Arcadia_ByteArray* copy = Arcadia_ByteArrayFactory_createResizable(thread, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)copy);
  Arcadia_ByteArray_appendBytes(thread, copy, bytes, size);
  // A changed IHDR width with the old CRC is invalid on both platforms.
  Arcadia_ByteArray_setAt(thread, copy, 19,
    (uint8_t)(Arcadia_ByteArray_getAt(thread, copy, 19) ^ 1));
  Arcadia_Imaging_ImageReaderParameters* input =
    Arcadia_Imaging_ImageReaderParameters_createByteArray(thread, copy, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
  Arcadia_JumpTarget inner;
  Arcadia_Thread_pushJumpTarget(thread, &inner);
  if (Arcadia_JumpTarget_save(&inner)) {
    (void)Arcadia_Imaging_ImageReader_read(thread,
      (Arcadia_Imaging_ImageReader*)Arcadia_List_getObjectReferenceValueAt(thread, readers, 0),
      input);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertTrue(thread, actual == Arcadia_Status_EncodingInvalid);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runCorruptedEncodedPNG
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testCorruptedEncodedPNG) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "WritePNGTests", &runTests },
    { "ManagedPixelStorage", &runManagedPixelStorage },
    { "BMPUnsupported", &runBMPUnsupported },
    { "EncodedPixels", &runEncodedPixels },
    { "MutationDetection", &runMutationDetection },
    { "CorruptedEncodedPNG", &runCorruptedEncodedPNG },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
