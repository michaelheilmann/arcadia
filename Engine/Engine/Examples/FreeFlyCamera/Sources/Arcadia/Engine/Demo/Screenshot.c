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

#include "Arcadia/Engine/Demo/Screenshot.h"

#include "Arcadia/PixelBufferIO/Include.h"

static Arcadia_String*
makePath
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value index
  )
{
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Screenshot-");
  Arcadia_StringBuilder_insertBackCxxInt(thread, stringBuilder, (int)index);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".png");
  return Arcadia_String_create
    (
      thread,
      Arcadia_Value_makeRuntimeUTF8StringValue
        (
          Arcadia_RuntimeUTF8String_create
            (
              thread,
              Arcadia_StringBuilder_getBytes(thread, stringBuilder),
              Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuilder)
            )
        )
    );
}

void
Arcadia_Engine_Demo_writeScreenshot
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Window* window,
    Arcadia_Natural32Value index
  )
{
  Arcadia_Media_PixelBuffer* pixelBuffer = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {

    pixelBuffer = Arcadia_Engine_Visuals_Window_capturePixels(thread, window);

    // Writing the image raises on failure, hence the pixel buffer is locked such that it
    // survives until the write has concluded.
    Arcadia_Object_lock(thread, (Arcadia_Object*)pixelBuffer);
    {
      Arcadia_String* extension = Arcadia_String_create
        (
          thread,
          Arcadia_Value_makeRuntimeUTF8StringValue
            (
              Arcadia_RuntimeUTF8String_create(thread, u8"png", sizeof(u8"png") - 1)
            )
        );
      Arcadia_String* path = makePath(thread, index);
      Arcadia_Imaging_ImageWriterParameters* parameters =
        Arcadia_Imaging_ImageWriterParameters_createFile(thread, path, extension);
      // The image writer consumes a list of pixel buffers. A list is used rather than the pixel
      // buffer directly as that is what the writer expects, also for multi-image formats.
      Arcadia_List* pixelBuffers = (Arcadia_List*)Arcadia_ArrayList_create(thread);
      Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffers, pixelBuffer);

      Arcadia_Imaging_ImageManager* imageManager = Arcadia_Imaging_ImageManager_getOrCreate(thread);
      Arcadia_List* writers = Arcadia_Imaging_ImageManager_getWriters(thread, imageManager, extension);
      if (0 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)writers)) {
        // No writer for the "png" extension is registered on this platform.
        Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_Imaging_ImageWriter* writer = (Arcadia_Imaging_ImageWriter*)
        Arcadia_List_getObjectReferenceValueAt(thread, writers, 0);
      Arcadia_Imaging_ImageWriter_write(thread, writer, pixelBuffers, parameters);
    }

    Arcadia_Thread_popJumpTarget(thread);
    if (pixelBuffer) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)pixelBuffer);
      pixelBuffer = NULL;
    }

  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (pixelBuffer) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)pixelBuffer);
      pixelBuffer = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
}
