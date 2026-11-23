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
#include "Arcadia/PixelBufferIO/Include.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// The directory is checked without using the production ICO parser.
// The embedded images are decoded through the production ImageReader.
static uint16_t
read16
  (
    uint8_t const* bytes
  )
{
  return (uint16_t)((uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8));
}

static uint32_t
read32
  (
    uint8_t const* bytes
  )
{
  return (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) |
    ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
}

static void
assertPixel
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

static void
testEncodedICO
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

  Arcadia_String* format = Arcadia_String_createFromCxxString(thread, u8"ico");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)format);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)writers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
    Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_List* sources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sources);

  // Only ARGB8 is supported by the current ICO writer. Distinct pixels
  // on both rows detect channel/position swaps and row inversion.
  for (Arcadia_SizeValue i = 0; i < 2; ++i) {
    Arcadia_Integer32Value dimension = i == 0 ? 16 : 32;
    Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread,
      3, dimension, dimension, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
    Arcadia_Media_PixelBuffer_fill(thread, pixels, 0, 0, 0, 0);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 0, 255, 0, 0, 255);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 0, 0, 255, 0, 128);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 1, 0, 0, 255, 64);
    Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 1, 255, 255, 255, 0);
    Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
  }

  Arcadia_ByteArrayBuilder* output = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)output);
  Arcadia_Imaging_ImageWriterParameters* target =
    Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, output, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Imaging_ImageWriter_write(thread, writer, sources, target);

  Arcadia_SizeValue size = Arcadia_ByteArrayBuilder_getSize(thread, output);
  Arcadia_Tests_assertTrue(thread, size > 6 + 16 * 2);
  uint8_t const* bytes = (uint8_t const*)Arcadia_ByteArrayBuilder_getBytes(thread, output);
  Arcadia_Tests_assertTrue(thread, bytes != NULL);
  Arcadia_Tests_assertTrue(thread,
    read16(bytes) == 0 && read16(bytes + 2) == 1 && read16(bytes + 4) == 2);

  uint32_t previousEnd = 6 + 16 * 2;
  uint8_t const pngSignature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };
  for (Arcadia_SizeValue i = 0; i < 2; ++i) {
    uint8_t const* entry = bytes + 6 + 16 * i;
    uint32_t length = read32(entry + 8);
    uint32_t offset = read32(entry + 12);
    Arcadia_Tests_assertTrue(thread, entry[0] == (i == 0 ? 16 : 32));
    Arcadia_Tests_assertTrue(thread, entry[1] == (i == 0 ? 16 : 32));
    Arcadia_Tests_assertTrue(thread, entry[2] == 0 && entry[3] == 0);
    Arcadia_Tests_assertTrue(thread, read16(entry + 4) == 1);
    Arcadia_Tests_assertTrue(thread, read16(entry + 6) == 32);
    Arcadia_Tests_assertTrue(thread, length >= sizeof(pngSignature));
    Arcadia_Tests_assertTrue(thread, offset == previousEnd);
    Arcadia_Tests_assertTrue(thread, (Arcadia_SizeValue)offset <= size &&
      (Arcadia_SizeValue)length <= size - (Arcadia_SizeValue)offset);
    Arcadia_Tests_assertTrue(thread,
      memcmp(bytes + offset, pngSignature, sizeof(pngSignature)) == 0);
    previousEnd = offset + length;
  }
  Arcadia_Tests_assertEqualSize(thread, size, (Arcadia_SizeValue)previousEnd);

  Arcadia_List* readers = Arcadia_Imaging_ImageManager_getReaders(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)readers);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)readers));
  Arcadia_Imaging_ImageReader* reader = (Arcadia_Imaging_ImageReader*)
    Arcadia_List_getObjectReferenceValueAt(thread, readers, 0);
  Arcadia_ByteArray* encoded = Arcadia_ByteArrayFactory_borrowReadOnly(thread, bytes, size);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)encoded);
  Arcadia_Imaging_ImageReaderParameters* input =
    Arcadia_Imaging_ImageReaderParameters_createByteArray(thread, encoded, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
  Arcadia_List* decoded = Arcadia_Imaging_ImageReader_read(thread, reader, input);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)decoded);
  Arcadia_Tests_assertEqualSize(thread, 2,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)decoded));
  for (Arcadia_SizeValue i = 0; i < 2; ++i) {
    Arcadia_Media_PixelBuffer* image = (Arcadia_Media_PixelBuffer*)
      Arcadia_List_getObjectReferenceValueAt(thread, decoded, i);
    Arcadia_Integer32Value dimension = i == 0 ? 16 : 32;
    Arcadia_Tests_assertTrue(thread,
      Arcadia_Media_PixelBuffer_getWidth(thread, image) == dimension &&
      Arcadia_Media_PixelBuffer_getHeight(thread, image) == dimension);
    assertPixel(thread, image, 0, 0, 255, 0, 0, 255);
    assertPixel(thread, image, 1, 0, 0, 255, 0, 128);
    assertPixel(thread, image, 0, 1, 0, 0, 255, 64);
    assertPixel(thread, image, 1, 1, 255, 255, 255, 0);
    assertPixel(thread, image, dimension - 1, dimension - 1, 0, 0, 0, 0);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runEncodedICO
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testEncodedICO) ? EXIT_SUCCESS : EXIT_FAILURE;
}


static void
assertRejectedWrite
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* writer,
    Arcadia_List* sources,
    Arcadia_Imaging_ImageWriterParameters* parameters,
    Arcadia_ByteArrayBuilder* output
  )
{
  Arcadia_ByteArrayBuilder_clear(thread, output);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Imaging_ImageWriter_write(thread, writer, sources, parameters);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_ArgumentValueInvalid);
    // The writer may have produced directory bytes before rejecting a source.
    // In particular, nonempty output is never counted as successful encoding.
  }
}

static void
testWriterInvalidSources
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
  Arcadia_String* format = Arcadia_String_createFromCxxString(thread, u8"ico");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)format);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)writers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
    Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_List* sources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sources);
  Arcadia_ByteArrayBuilder* output = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)output);
  Arcadia_Imaging_ImageWriterParameters* target =
    Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, output, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);

  Arcadia_Media_PixelBuffer* wrongFormat = Arcadia_Media_PixelBuffer_create(thread,
    3, 16, 16, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, sources, wrongFormat);
  assertRejectedWrite(thread, writer, sources, target, output);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)sources);
  Arcadia_Media_PixelBuffer* tooWide = Arcadia_Media_PixelBuffer_create(thread,
    0, 257, 1, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, sources, tooWide);
  assertRejectedWrite(thread, writer, sources, target, output);

  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runInvalidSources
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testWriterInvalidSources) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testICO256
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
  Arcadia_String* format = Arcadia_String_createFromCxxString(thread, u8"ico");
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
    1, 256, 256, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
  Arcadia_Media_PixelBuffer_fill(thread, pixels, 0, 0, 0, 255);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 255, 255, 255, 0, 0, 128);
  Arcadia_ByteArrayBuilder* output = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)output);
  Arcadia_Imaging_ImageWriterParameters* target =
    Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, output, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  Arcadia_Imaging_ImageWriter_write(thread,
    (Arcadia_Imaging_ImageWriter*)Arcadia_List_getObjectReferenceValueAt(thread, writers, 0),
    sources, target);
  Arcadia_SizeValue size = Arcadia_ByteArrayBuilder_getSize(thread, output);
  Arcadia_Tests_assertTrue(thread, size > 22);
  uint8_t const* bytes = (uint8_t const*)Arcadia_ByteArrayBuilder_getBytes(thread, output);
  Arcadia_Tests_assertTrue(thread, bytes[6] == 0 && bytes[7] == 0);
  Arcadia_Tests_assertTrue(thread,
    read32(bytes + 6 + 12) == 22 && read32(bytes + 6 + 8) == size - 22);
  Arcadia_ByteArray* encoded = Arcadia_ByteArrayFactory_borrowReadOnly(thread, bytes, size);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)encoded);
  Arcadia_Imaging_ImageReaderParameters* input =
    Arcadia_Imaging_ImageReaderParameters_createByteArray(thread, encoded, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
  Arcadia_List* decoded = Arcadia_Imaging_ImageReader_read(thread,
    (Arcadia_Imaging_ImageReader*)Arcadia_List_getObjectReferenceValueAt(thread, readers, 0),
    input);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)decoded);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)decoded));
  Arcadia_Media_PixelBuffer* image = (Arcadia_Media_PixelBuffer*)
    Arcadia_List_getObjectReferenceValueAt(thread, decoded, 0);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Media_PixelBuffer_getWidth(thread, image) == 256 &&
    Arcadia_Media_PixelBuffer_getHeight(thread, image) == 256);
  assertPixel(thread, image, 0, 0, 0, 0, 0, 255);
  assertPixel(thread, image, 255, 255, 255, 0, 0, 128);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runICO256
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testICO256) ? EXIT_SUCCESS : EXIT_FAILURE;
}


static void
testBuilderWriteFailure
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

  Arcadia_String* format = Arcadia_String_createFromCxxString(thread, u8"ico");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)format);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)writers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
    Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_List* sources = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sources);
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread,
    1, 16, 16, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
  Arcadia_Media_PixelBuffer_fill(thread, pixels, 12, 34, 56, 255);

  Arcadia_ByteArrayBuilder* output = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)output);
  // A non-resizable backing ByteArray is a deterministic failing output sink.
  // It avoids process-wide OOM injection and raises OperationInvalid on append.
  Arcadia_ByteArray* immutable = Arcadia_ByteArrayFactory_copyImmutable(thread, output->storage);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)immutable);
  output->storage = immutable;
  Arcadia_Tests_assertFalse(thread, Arcadia_ByteArray_isResizable(thread, output->storage));
  Arcadia_Imaging_ImageWriterParameters* target =
    Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, output, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  Arcadia_JumpTarget inner;
  Arcadia_Thread_pushJumpTarget(thread, &inner);
  if (Arcadia_JumpTarget_save(&inner)) {
    Arcadia_Imaging_ImageWriter_write(thread, writer, sources, target);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertTrue(thread, actual == Arcadia_Status_OperationInvalid);
    Arcadia_Tests_assertEqualSize(thread, 0,
      Arcadia_ByteArrayBuilder_getSize(thread, output));
  }

  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runBuilderWriteFailure
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testBuilderWriteFailure) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "EncodedICO", &runEncodedICO },
    { "ICO256", &runICO256 },
    { "InvalidSources", &runInvalidSources },
    { "BuilderWriteFailure", &runBuilderWriteFailure },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
