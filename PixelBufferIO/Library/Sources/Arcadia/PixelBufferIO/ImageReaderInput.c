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

#include "Arcadia/PixelBufferIO/ImageReaderInput.h"

#include <limits.h>
#include <string.h>

Arcadia_ByteArray*
Arcadia_Imaging_ImageReaderInput_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* source
  )
{
  if (Arcadia_Imaging_ImageReaderParameters_hasByteArray(thread, source)) {
    // A slice or non-contiguous implementation is allowed as input.
    return Arcadia_ByteArrayFactory_copyImmutable(thread,
      Arcadia_Imaging_ImageReaderParameters_getByteArray(thread, source));
  }
  if (Arcadia_Imaging_ImageReaderParameters_hasPath(thread, source)) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (!Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* path = Arcadia_Imaging_ImageReaderParameters_getPath(thread, source);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path);
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseNative(thread, path);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)filePath);
    Arcadia_ByteArrayBuilder* builder = Arcadia_FileSystem_getFileContents(thread,
      Arcadia_FileSystem_getOrCreate(thread), filePath);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)builder);
    Arcadia_SizeValue n = Arcadia_ByteArrayBuilder_getSize(thread, builder);
    Arcadia_ByteArray* bytes = Arcadia_ByteArrayFactory_createMutable(thread, n);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)bytes);
    if (n) {
      memcpy(Arcadia_ByteArray_getMutableBytes(thread, bytes),
             Arcadia_ByteArrayBuilder_getBytes(thread, builder), n);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    return bytes;
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_Thread_jump(thread);
}

Arcadia_Media_PixelBuffer*
Arcadia_Imaging_ImageReaderInput_createPixels
  (
    Arcadia_Thread* thread,
    size_t width,
    size_t height,
    uint8_t const* rgbaBytes
  )
{
  if (!width || !height || width > INT32_MAX || height > INT32_MAX ||
      width > (size_t)INT32_MAX / 4 ||
      height > SIZE_MAX / (width * 4)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Media_PixelBuffer_create(thread, 0,
    (Arcadia_Integer32Value)width, (Arcadia_Integer32Value)height,
    Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ByteArray* bytes = Arcadia_Media_PixelBuffer_getPixelBytes(thread, pixels);
    if (Arcadia_ByteArray_getSize(thread, bytes) < width * height * 4) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    memcpy(Arcadia_ByteArray_getMutableBytes(thread, bytes), rgbaBytes, width * height * 4);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
    return pixels;
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
  Arcadia_Thread_jump(thread);
}
