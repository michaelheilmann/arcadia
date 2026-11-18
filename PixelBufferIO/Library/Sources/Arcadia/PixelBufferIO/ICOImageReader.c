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

#include "Arcadia/PixelBufferIO/ICOImageReader.h"
#include "Arcadia/PixelBufferIO/PNGImageReader.h"
#include "Arcadia/PixelBufferIO/ImageReaderInput.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

static void
failRead
  (
    Arcadia_Thread* thread,
    Arcadia_Status status
  )
{
  Arcadia_Thread_setStatus(thread, status);
  Arcadia_Thread_jump(thread);
}

static uint16_t
read16
  (
    uint8_t const* p
  )
{
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t
read32
  (
    uint8_t const* p
  )
{
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
    ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static Arcadia_ImmutableList*
getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageReader* self
  )
{
  return self->supportedTypes;
}

static Arcadia_List*
readImages
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageReader* self,
    Arcadia_Imaging_ImageReaderParameters* source
  )
{
  Arcadia_Value requested = Arcadia_Value_makeObjectReferenceValue(
    Arcadia_Imaging_ImageReaderParameters_getFormat(thread, source));
  if (!Arcadia_List_contains(thread, (Arcadia_List*)self->supportedTypes, requested)) {
    failRead(thread, Arcadia_Status_ArgumentValueInvalid);
  }

  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ByteArray* data = Arcadia_Imaging_ImageReaderInput_getBytes(thread, source);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)data);
  size_t size = Arcadia_ByteArray_getSize(thread, data);
  uint8_t const* bytes = Arcadia_ByteArray_getBytes(thread, data);
  if (size < 6 || read16(bytes) != 0 || read16(bytes + 2) != 1) {
    failRead(thread, Arcadia_Status_EncodingInvalid);
  }
  size_t count = read16(bytes + 4);
  // Bound the quadratic overlap check and total decoded image resources.
  // Normal icon containers have substantially fewer than 1024 variants.
  if (count == 0 || count > 1024) {
    failRead(thread, Arcadia_Status_ArgumentValueInvalid);
  }
  if (count > (size - 6) / 16) {
    failRead(thread, Arcadia_Status_EncodingInvalid);
  }
  size_t directoryEnd = 6 + count * 16;
  static const uint8_t pngSignature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

  // Validate *all* directory ranges before any decoder work. This avoids
  // returning partial results for a malformed multi-image container.
  for (size_t i = 0; i < count; ++i) {
    uint8_t const* entry = bytes + 6 + i * 16;
    size_t length = read32(entry + 8);
    size_t offset = read32(entry + 12);
    if (entry[3] != 0 || length < 8 || offset < directoryEnd ||
        offset > size || length > size - offset) {
      failRead(thread, Arcadia_Status_EncodingInvalid);
    }
    for (size_t j = 0; j < i; ++j) {
      uint8_t const* previous = bytes + 6 + j * 16;
      size_t oldOffset = read32(previous + 12);
      size_t oldLength = read32(previous + 8);
      if (offset < oldOffset + oldLength && oldOffset < offset + length) {
        failRead(thread, Arcadia_Status_EncodingInvalid);
      }
    }
    // A DIB-backed ICO is well-formed ICO, but unsupported by this reader.
    if (memcmp(bytes + offset, pngSignature, sizeof(pngSignature))) {
      failRead(thread, Arcadia_Status_NotImplemented);
    }
  }

  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)list);
  for (size_t i = 0; i < count; ++i) {
    uint8_t const* entry = bytes + 6 + i * 16;
    size_t width = entry[0] ? entry[0] : 256;
    size_t height = entry[1] ? entry[1] : 256;
    size_t length = read32(entry + 8);
    size_t offset = read32(entry + 12);
    Arcadia_Media_PixelBuffer* pixels =
      Arcadia_Imaging_PNGImageReader_decode(thread, bytes + offset, length);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)pixels);
    if (Arcadia_Media_PixelBuffer_getWidth(thread, pixels) != width ||
        Arcadia_Media_PixelBuffer_getHeight(thread, pixels) != height) {
      failRead(thread, Arcadia_Status_EncodingInvalid);
    }
    Arcadia_List_insertBackObjectReferenceValue(thread, list, pixels);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return list;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ICOImageReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (_numberOfArguments != 0) {
    failRead(thread, Arcadia_Status_NumberOfArgumentsInvalid);
  }
  self->supportedTypes = NULL;
  Arcadia_List* types = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)types);
  Arcadia_List_insertBackObjectReferenceValue(thread, types,
    Arcadia_String_createFromCxxString(thread, u8"ico"));
  self->supportedTypes = Arcadia_ImmutableList_create(thread,
    Arcadia_Value_makeObjectReferenceValue(types));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Imaging_ICOImageReader);
}

static void
visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageReader* self
  )
{
  if (self->supportedTypes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->supportedTypes);
  }
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ICOImageReaderDispatch* self
  )
{
  ((Arcadia_Imaging_ImageReaderDispatch*)self)->getSupportedTypes =
    (Arcadia_ImmutableList* (*)(Arcadia_Thread*, Arcadia_Imaging_ImageReader*))&getSupportedTypes;
  ((Arcadia_Imaging_ImageReaderDispatch*)self)->read =
    (Arcadia_List* (*)(Arcadia_Thread*, Arcadia_Imaging_ImageReader*, Arcadia_Imaging_ImageReaderParameters*))&readImages;
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_TypeDescriptor_Operations _typeOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ICOImageReader", Arcadia_Imaging_ICOImageReader,
                         u8"Arcadia.Imaging.ImageReader", Arcadia_Imaging_ImageReader,
                         &_typeOperations);

Arcadia_Imaging_ICOImageReader*
Arcadia_Imaging_ICOImageReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_ICOImageReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_ICOImageReader);
}
