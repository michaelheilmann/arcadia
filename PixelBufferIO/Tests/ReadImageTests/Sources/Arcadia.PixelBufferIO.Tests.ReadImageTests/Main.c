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
#include <string.h>
#include <stdlib.h>

// Independently prepared known-answer PNG: 2x2 RGBA8 with explicit PNG
// chunks and CRCs, compressed outside the Arcadia writer under test.
// Row 0: opaque red, opaque green.
// Row 1: transparent blue, half-transparent white.
static uint8_t const tinyPng[] = {
  137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82,
  0, 0, 0, 2, 0, 0, 0, 2, 8, 6, 0, 0, 0, 114, 182, 13,
  36, 0, 0, 0, 19, 73, 68, 65, 84, 120, 218, 99, 248, 207, 192, 240,
  31, 12, 193, 244, 255, 255, 13, 0, 68, 78, 8, 121, 164, 63, 25, 9,
  0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130,
};


// Independently prepared 256x256 grayscale PNG (all black, fully opaque).
static uint8_t const largePng[] = {
  137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82,
  0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 116, 9, 149,
  203, 0, 0, 0, 31, 73, 68, 65, 84, 120, 218, 237, 193, 1, 13, 0,
  0, 0, 194, 160, 247, 79, 109, 14, 55, 160, 0, 0, 0, 0, 0, 0,
  0, 0, 190, 13, 33, 0, 0, 1, 96, 228, 157, 151, 0, 0, 0, 0,
  73, 69, 78, 68, 174, 66, 96, 130,
};

static void
put16
  (
    uint8_t* p,
    uint16_t x
  )
{
  p[0] = (uint8_t)x;
  p[1] = (uint8_t)(x >> 8);
}

static void
put32
  (
    uint8_t* p,
    uint32_t x
  )
{
  p[0] = (uint8_t)x;
  p[1] = (uint8_t)(x >> 8);
  p[2] = (uint8_t)(x >> 16);
  p[3] = (uint8_t)(x >> 24);
}

static Arcadia_List*
decode
  (
    Arcadia_Thread* thread,
    char const* format,
    uint8_t const* bytes,
    size_t size
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
  Arcadia_String* extension = Arcadia_String_createFromCxxString(thread, format);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)extension);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* readers = Arcadia_Imaging_ImageManager_getReaders(thread, manager, extension);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)readers);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)readers));
  Arcadia_Imaging_ImageReader* reader = (Arcadia_Imaging_ImageReader*)
    Arcadia_List_getObjectReferenceValueAt(thread, readers, 0);
  Arcadia_ByteArray* input = Arcadia_ByteArrayFactory_borrowReadOnly(thread, bytes, size);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)input);
  Arcadia_Imaging_ImageReaderParameters* parameters =
    Arcadia_Imaging_ImageReaderParameters_createByteArray(thread, input, extension);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)parameters);
  Arcadia_List* result = Arcadia_Imaging_ImageReader_read(thread, reader, parameters);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
assertPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* pixels,
    int column,
    int row,
    uint8_t r,
    uint8_t g,
    uint8_t b,
    uint8_t a
  )
{
  Arcadia_Natural8Value red, green, blue, alpha;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, pixels,
    column, row, &red, &green, &blue, &alpha);
  Arcadia_Tests_assertTrue(thread,
    red == r && green == g && blue == b && alpha == a);
}

static void
testKnownAnswerPNG
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* list = decode(thread, "png", tinyPng, sizeof(tinyPng));
  Arcadia_Object_lock(thread, (Arcadia_Object*)list);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list));
  Arcadia_Media_PixelBuffer* p = (Arcadia_Media_PixelBuffer*)
    Arcadia_List_getObjectReferenceValueAt(thread, list, 0);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Media_PixelBuffer_getPixelFormat(thread, p) ==
      Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Media_PixelBuffer_getWidth(thread, p) == 2 &&
    Arcadia_Media_PixelBuffer_getHeight(thread, p) == 2);
  assertPixel(thread, p, 0, 0, 255, 0, 0, 255);
  assertPixel(thread, p, 1, 0, 0, 255, 0, 255);
  assertPixel(thread, p, 0, 1, 0, 0, 255, 0);
  assertPixel(thread, p, 1, 1, 255, 255, 255, 128);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)list);
}

static void
makeICO
  (
    uint8_t* result
  )
{
  memset(result, 0, 6 + 32 + sizeof(tinyPng) * 2);
  put16(result + 2, 1);
  put16(result + 4, 2);
  for (size_t i = 0; i < 2; ++i) {
    size_t at = 6 + 16 * i;
    result[at] = 2;
    result[at + 1] = 2;
    put16(result + at + 4, 1);
    put16(result + at + 6, 32);
    put32(result + at + 8, sizeof(tinyPng));
    put32(result + at + 12, (uint32_t)(6 + 32 + sizeof(tinyPng) * i));
    memcpy(result + 6 + 32 + sizeof(tinyPng) * i, tinyPng, sizeof(tinyPng));
  }
}

static void
testMultiImageICO
  (
    Arcadia_Thread* thread
  )
{
  uint8_t bytes[6 + 32 + sizeof(tinyPng) * 2];
  makeICO(bytes);
  Arcadia_List* list = decode(thread, "ico", bytes, sizeof(bytes));
  Arcadia_Object_lock(thread, (Arcadia_Object*)list);
  Arcadia_Tests_assertEqualSize(thread, 2,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list));
  for (size_t i = 0; i < 2; ++i) {
    Arcadia_Media_PixelBuffer* p = (Arcadia_Media_PixelBuffer*)
      Arcadia_List_getObjectReferenceValueAt(thread, list, i);
    assertPixel(thread, p, 0, 0, 255, 0, 0, 255);
    assertPixel(thread, p, 1, 1, 255, 255, 255, 128);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)list);
}

static void
assertReject
  (
    Arcadia_Thread* thread,
    char const* format,
    uint8_t const* bytes,
    size_t size,
    Arcadia_Status expected
  )
{
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    (void)decode(thread, format, bytes, size);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertTrue(thread, actual == expected);
  }
}

static void
testRejectedFiles
  (
    Arcadia_Thread* thread
  )
{
  uint8_t ico[6 + 32 + sizeof(tinyPng) * 2];
  makeICO(ico);
  assertReject(thread, "png", tinyPng, 7, Arcadia_Status_EncodingInvalid);
  assertReject(thread, "png", tinyPng, sizeof(tinyPng) - 8, Arcadia_Status_EncodingInvalid);
  uint8_t corruptPng[sizeof(tinyPng)];
  memcpy(corruptPng, tinyPng, sizeof(tinyPng));
  corruptPng[45] ^= 0x40; // Corrupt IDAT without updating its CRC.
  assertReject(thread, "png", corruptPng, sizeof(corruptPng), Arcadia_Status_EncodingInvalid);
  assertReject(thread, "ico", ico, 5, Arcadia_Status_EncodingInvalid);
  // Offset into the directory.
  put32(ico + 6 + 12, 6);
  assertReject(thread, "ico", ico, sizeof(ico), Arcadia_Status_EncodingInvalid);
  makeICO(ico);
  // Two directory entries claiming the same slice.
  put32(ico + 6 + 16 + 12, 38);
  assertReject(thread, "ico", ico, sizeof(ico), Arcadia_Status_EncodingInvalid);
  makeICO(ico);
  // Well-formed directory, unsupported embedded non-PNG data.
  ico[38] = 40;
  assertReject(thread, "ico", ico, sizeof(ico), Arcadia_Status_NotImplemented);
  makeICO(ico);
  // Directory height zero means 256, inconsistent with 2x2 embedded PNG.
  ico[6 + 1] = 0;
  assertReject(thread, "ico", ico, sizeof(ico), Arcadia_Status_EncodingInvalid);
}


static void
test256ICO
  (
    Arcadia_Thread* thread
  )
{
  uint8_t ico[6 + 16 + sizeof(largePng)];
  memset(ico, 0, sizeof(ico));
  put16(ico + 2, 1);
  put16(ico + 4, 1);
  // Both zero directory dimension bytes represent 256.
  put16(ico + 6 + 4, 1);
  put16(ico + 6 + 6, 32);
  put32(ico + 6 + 8, sizeof(largePng));
  put32(ico + 6 + 12, 22);
  memcpy(ico + 22, largePng, sizeof(largePng));
  Arcadia_List* list = decode(thread, "ico", ico, sizeof(ico));
  Arcadia_Object_lock(thread, (Arcadia_Object*)list);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list));
  Arcadia_Media_PixelBuffer* p = (Arcadia_Media_PixelBuffer*)
    Arcadia_List_getObjectReferenceValueAt(thread, list, 0);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Media_PixelBuffer_getWidth(thread, p) == 256 &&
    Arcadia_Media_PixelBuffer_getHeight(thread, p) == 256);
  assertPixel(thread, p, 0, 0, 0, 0, 0, 255);
  assertPixel(thread, p, 255, 255, 0, 0, 0, 255);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)list);
}

static int
run256ICO
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&test256ICO) ? EXIT_SUCCESS : EXIT_FAILURE; }


static void
testPathInput
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
  Arcadia_String* path = Arcadia_String_createFromCxxString(thread, u8"reader-known-answer.png");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path);
  Arcadia_String* extension = Arcadia_String_createFromCxxString(thread, u8"png");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)extension);
  Arcadia_ByteArrayBuilder* file = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)file);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, file, tinyPng, sizeof(tinyPng));
  Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fs);
  Arcadia_FilePath* parsed = Arcadia_FilePath_parseNative(thread, path);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)parsed);
  Arcadia_FileSystem_setFileContents(thread, fs, parsed, file);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)manager);
  Arcadia_List* readers = Arcadia_Imaging_ImageManager_getReaders(thread, manager, extension);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)readers);
  Arcadia_Imaging_ImageReader* reader = (Arcadia_Imaging_ImageReader*)
    Arcadia_List_getObjectReferenceValueAt(thread, readers, 0);
  Arcadia_Imaging_ImageReaderParameters* params =
    Arcadia_Imaging_ImageReaderParameters_createFile(thread, path, extension);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)params);
  Arcadia_List* result = Arcadia_Imaging_ImageReader_read(thread, reader, params);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);
  Arcadia_Tests_assertEqualSize(thread, 1,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)result));
  Arcadia_Media_PixelBuffer* pixels = (Arcadia_Media_PixelBuffer*)
    Arcadia_List_getObjectReferenceValueAt(thread, result, 0);
  assertPixel(thread, pixels, 1, 0, 0, 255, 0, 255);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runPath
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testPathInput) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runPNG
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testKnownAnswerPNG) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runICO
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testMultiImageICO) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runInvalid
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testRejectedFiles) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "KnownAnswerPNG", &runPNG },
    { "PathInput", &runPath },
    { "MultiImageICO", &runICO },
    { "ICO256", &run256ICO },
    { "MalformedAndUnsupported", &runInvalid },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
