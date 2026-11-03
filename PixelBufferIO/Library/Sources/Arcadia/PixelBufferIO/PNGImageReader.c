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

#include "Arcadia/PixelBufferIO/PNGImageReader.h"
#include "Arcadia/PixelBufferIO/ImageReaderInput.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include <png.h>
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <wincodec.h>
  #pragma comment(lib, "Windowscodecs.lib")
  #pragma comment(lib, "Ole32.lib")
#else
  #error("unsupported image reader platform")
#endif

// Strictly bound chunk extents before passing input to either codec.
// libpng and WIC may otherwise tolerate a missing final IEND chunk.
static uint32_t
pngCrc32
  (
    uint8_t const* data,
    size_t size
  )
{
  uint32_t crc = UINT32_MAX;
  for (size_t i = 0; i < size; ++i) {
    crc ^= data[i];
    for (unsigned bit = 0; bit < 8; ++bit) {
      crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
  }
  return ~crc;
}

static Arcadia_BooleanValue
validPngContainer
  (
    uint8_t const* bytes,
    size_t size
  )
{
  size_t offset = 8;
  size_t chunks = 0;
  Arcadia_BooleanValue idat = Arcadia_BooleanValue_False;
  while (offset <= size && size - offset >= 12) {
    uint32_t n = ((uint32_t)bytes[offset] << 24) |
      ((uint32_t)bytes[offset + 1] << 16) |
      ((uint32_t)bytes[offset + 2] << 8) | bytes[offset + 3];
    if (n > size - offset - 12) return Arcadia_BooleanValue_False;
    uint8_t const* type = bytes + offset + 4;
    if (chunks == 0 && (memcmp(type, "IHDR", 4) || n != 13)) {
      return Arcadia_BooleanValue_False;
    }
    if (!memcmp(type, "IDAT", 4)) idat = Arcadia_BooleanValue_True;
    // Verify each chunk's CRC independently of backend policy.
    size_t end = offset + 8 + (size_t)n;
    uint32_t stored = ((uint32_t)bytes[end] << 24) |
      ((uint32_t)bytes[end + 1] << 16) |
      ((uint32_t)bytes[end + 2] << 8) | bytes[end + 3];
    if (pngCrc32(type, (size_t)n + 4) != stored) {
      return Arcadia_BooleanValue_False;
    }
    offset += (size_t)n + 12;
    ++chunks;
    if (!memcmp(type, "IEND", 4)) {
      return n == 0 && idat && offset == size;
    }
  }
  return Arcadia_BooleanValue_False;
}

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

// Decode externally prepared PNG only. libpng's simplified interface avoids
// libpng callbacks and longjmp across Arcadia frames; all libpng-owned state
// is released before raising an Arcadia error.
Arcadia_Media_PixelBuffer*
Arcadia_Imaging_PNGImageReader_decode
  (
    Arcadia_Thread* thread,
    uint8_t const* bytes,
    size_t size
  )
{
  static const uint8_t signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
  if (!bytes || size < sizeof(signature) || memcmp(bytes, signature, sizeof(signature)) ||
      !validPngContainer(bytes, size)) {
    failRead(thread, Arcadia_Status_EncodingInvalid);
  }
#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  png_image image;
  memset(&image, 0, sizeof(image));
  image.version = PNG_IMAGE_VERSION;
  if (!png_image_begin_read_from_memory(&image, bytes, size)) {
    png_image_free(&image);
    failRead(thread, Arcadia_Status_EncodingInvalid);
  }
  if (!image.width || !image.height || image.width > INT32_MAX ||
      image.height > INT32_MAX || image.width > INT32_MAX / 4 ||
      (size_t)image.height > SIZE_MAX / ((size_t)image.width * 4)) {
    png_image_free(&image);
    failRead(thread, Arcadia_Status_ArgumentValueInvalid);
  }
  image.format = PNG_FORMAT_RGBA;
  size_t width = image.width, height = image.height;
  size_t count = width * height * 4;
  uint8_t* rgba = malloc(count);
  if (!rgba) {
    png_image_free(&image);
    failRead(thread, Arcadia_Status_AllocationFailed);
  }
  if (!png_image_finish_read(&image, NULL, rgba, 0, NULL)) {
    png_image_free(&image);
    free(rgba);
    failRead(thread, Arcadia_Status_EncodingInvalid);
  }
  png_image_free(&image);
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  // WIC's InitializeFromMemory uses a 32-bit size and borrows the buffer.
  if (size > UINT_MAX) {
    failRead(thread, Arcadia_Status_ArgumentValueInvalid);
  }
  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr)) {
    failRead(thread, Arcadia_Status_EnvironmentFailed);
  }
  IWICImagingFactory* factory = NULL;
  IWICStream* stream = NULL;
  IWICBitmapDecoder* decoder = NULL;
  IWICBitmapFrameDecode* frame = NULL;
  IWICFormatConverter* converter = NULL;
  uint8_t* rgba = NULL;
  size_t width = 0, height = 0;
  Arcadia_Status failure = Arcadia_Status_EnvironmentFailed;
  do {
    hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                          &IID_IWICImagingFactory, (void**)&factory);
    if (FAILED(hr)) break;
    hr = factory->lpVtbl->CreateStream(factory, &stream);
    if (FAILED(hr)) break;
    hr = stream->lpVtbl->InitializeFromMemory(stream, (BYTE*)bytes, (DWORD)size);
    if (FAILED(hr)) break;
    hr = factory->lpVtbl->CreateDecoderFromStream(factory, (IStream*)stream,
      NULL, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) {
      failure = Arcadia_Status_EncodingInvalid;
      break;
    }
    GUID container;
    hr = decoder->lpVtbl->GetContainerFormat(decoder, &container);
    if (FAILED(hr) || !IsEqualGUID(&container, &GUID_ContainerFormatPng)) {
      failure = Arcadia_Status_EncodingInvalid;
      break;
    }
    UINT frames = 0;
    hr = decoder->lpVtbl->GetFrameCount(decoder, &frames);
    if (FAILED(hr) || frames != 1) {
      failure = Arcadia_Status_EncodingInvalid;
      break;
    }
    hr = decoder->lpVtbl->GetFrame(decoder, 0, &frame);
    if (FAILED(hr)) {
      failure = Arcadia_Status_EncodingInvalid;
      break;
    }
    UINT w, h;
    hr = frame->lpVtbl->GetSize(frame, &w, &h);
    if (FAILED(hr) || !w || !h || w > INT32_MAX || h > INT32_MAX ||
        w > INT32_MAX / 4 || h > SIZE_MAX / ((size_t)w * 4) ||
        h > UINT_MAX / (w * 4)) {
      failure = Arcadia_Status_ArgumentValueInvalid;
      break;
    }
    width = w;
    height = h;
    hr = factory->lpVtbl->CreateFormatConverter(factory, &converter);
    if (FAILED(hr)) break;
    hr = converter->lpVtbl->Initialize(converter, (IWICBitmapSource*)frame,
      &GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone,
      NULL, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) {
      failure = Arcadia_Status_EncodingInvalid;
      break;
    }
    rgba = malloc(width * height * 4);
    if (!rgba) {
      failure = Arcadia_Status_AllocationFailed;
      break;
    }
    hr = converter->lpVtbl->CopyPixels(converter, NULL,
      (UINT)(width * 4), (UINT)(width * height * 4), rgba);
    if (FAILED(hr)) {
      failure = Arcadia_Status_EncodingInvalid;
      break;
    }
    failure = Arcadia_Status_Success;
  } while (0);
  if (converter) converter->lpVtbl->Release(converter);
  if (frame) frame->lpVtbl->Release(frame);
  if (decoder) decoder->lpVtbl->Release(decoder);
  if (stream) stream->lpVtbl->Release(stream);
  if (factory) factory->lpVtbl->Release(factory);
  CoUninitialize();
  if (failure != Arcadia_Status_Success) {
    if (hr == E_OUTOFMEMORY) {
      failure = Arcadia_Status_AllocationFailed;
    }
    free(rgba);
    failRead(thread, failure);
  }
#endif
  // In case the managed factory jumps, release the temporary allocation.
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Media_PixelBuffer* result =
      Arcadia_Imaging_ImageReaderInput_createPixels(thread, width, height, rgba);
    Arcadia_Thread_popJumpTarget(thread);
    free(rgba);
    return result;
  }
  Arcadia_Thread_popJumpTarget(thread);
  free(rgba);
  Arcadia_Thread_jump(thread);
}

static Arcadia_ImmutableList*
getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_PNGImageReader* self
  )
{
  return self->supportedTypes;
}

static Arcadia_List*
readImages
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_PNGImageReader* self,
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
  Arcadia_ByteArray* bytes = Arcadia_Imaging_ImageReaderInput_getBytes(thread, source);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)bytes);
  Arcadia_Media_PixelBuffer* pixels = Arcadia_Imaging_PNGImageReader_decode(thread,
    Arcadia_ByteArray_getBytes(thread, bytes), Arcadia_ByteArray_getSize(thread, bytes));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)pixels);
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)list);
  Arcadia_List_insertBackObjectReferenceValue(thread, list, pixels);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return list;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_PNGImageReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_PNGImageReader);
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
    Arcadia_String_createFromCxxString(thread, u8"png"));
  self->supportedTypes = Arcadia_ImmutableList_create(thread,
    Arcadia_Value_makeObjectReferenceValue(types));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Imaging_PNGImageReader);
}

static void
visit
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_PNGImageReader* self
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
    Arcadia_Imaging_PNGImageReaderDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Imaging.PNGImageReader", Arcadia_Imaging_PNGImageReader,
                         u8"Arcadia.Imaging.ImageReader", Arcadia_Imaging_ImageReader,
                         &_typeOperations);

Arcadia_Imaging_PNGImageReader*
Arcadia_Imaging_PNGImageReader_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Imaging_PNGImageReader);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Imaging_PNGImageReader);
}
