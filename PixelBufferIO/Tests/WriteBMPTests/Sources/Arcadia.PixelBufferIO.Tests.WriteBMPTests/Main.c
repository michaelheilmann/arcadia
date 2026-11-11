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

#include "Arcadia/PixelBufferIO/Include.h"

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_String* extension = Arcadia_String_createFromCxxString(thread, u8"bmp");
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
  path = Arcadia_String_createFromCxxString(thread, u8"rn8gn8bn8.bmp");
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_createFromCxxString(thread, u8"rn8gn8bn8an8.bmp");
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_createFromCxxString(thread, u8"an8rn8gn8bn8.bmp");
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_BlueGreenRedNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_createFromCxxString(thread, u8"bn8gn8rn8.bmp");
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_createFromCxxString(thread, u8"bn8gn8rn8an8.bmp");
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);

  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pixelBufferList);
  pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 256, 256, Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBufferList, pixelBuffer);
  path = Arcadia_String_createFromCxxString(thread, u8"an8bn8gn8rn8.bmp");
  parameters = Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
  Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBufferList, parameters);
}

// Exercise the real writer against ByteArray-owned pixel storage (including
// padded lines), while forcing a collection before the writer borrows bytes.
static void
testManagedPixelStorage(Arcadia_Thread* thread)
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_String* extension = Arcadia_String_createFromCxxString(thread, u8"bmp");
  Arcadia_Object_lock(thread, (Arcadia_Object*)extension);
  Arcadia_Imaging_ImageManager* manager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
  Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, manager, extension);
  Arcadia_Object_lock(thread, (Arcadia_Object*)writers);
  Arcadia_Tests_assertTrue(thread,
    Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers) != 0);
  Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
    Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
  Arcadia_Object_lock(thread, (Arcadia_Object*)writer);
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
    Arcadia_Tests_assertTrue(thread, pixels->bytes ==
      Arcadia_ByteArray_getMutableBytes(thread,
        Arcadia_Media_PixelBuffer_getPixelBytes(thread, pixels)));
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)sources);
    Arcadia_List_insertBackObjectReferenceValue(thread, sources, pixels);
    Arcadia_ByteArrayBuilder* bytes = Arcadia_ByteArrayBuilder_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)bytes);
    Arcadia_Imaging_ImageWriterParameters* parameters =
      Arcadia_Imaging_ImageWriterParameters_createByteBuffer(thread, bytes, extension);
    Arcadia_Object_lock(thread, (Arcadia_Object*)parameters);
    Arcadia_Imaging_ImageWriter_write(thread, writer, sources, parameters);
    Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_getSize(thread, bytes) > 16);
    Arcadia_Tests_assertTrue(thread, pixels->bytes ==
      Arcadia_ByteArray_getMutableBytes(thread,
        Arcadia_Media_PixelBuffer_getPixelBytes(thread, pixels)));
    Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
    Arcadia_Object_unlock(thread, (Arcadia_Object*)parameters);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)bytes);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sources);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)writer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)writers);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)extension);
}

static int
runManagedPixelStorage(void)
{ return Arcadia_Tests_safeExecute(&testManagedPixelStorage) ? EXIT_SUCCESS : EXIT_FAILURE; }

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
    { "WriteBMPTests", &runTests },
    { "ManagedPixelStorage", &runManagedPixelStorage },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
