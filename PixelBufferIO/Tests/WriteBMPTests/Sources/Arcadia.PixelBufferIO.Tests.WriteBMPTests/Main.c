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

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
