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

#define ARCADIA_MEDIA_MODULE (1)
#include "Arcadia/Media/PixelBuffer.h"

#include "Arcadia/Ring2/Include.h"

typedef struct PIXEL {
  Arcadia_Natural8Value r, g, b, a;
} PIXEL;

static void inline
DECODE_ABGR
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  pixel->a = *(p + 0);
  pixel->b = *(p + 1);
  pixel->g = *(p + 2);
  pixel->r = *(p + 3);
}

static void inline
DECODE_ARGB
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  pixel->a = *(p + 0);
  pixel->r = *(p + 1);
  pixel->g = *(p + 2);
  pixel->b = *(p + 3);
}

static void inline
DECODE_BGR
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  pixel->b = *(p + 0);
  pixel->g = *(p + 1);
  pixel->r = *(p + 2);
  pixel->a = 255;
}

static void inline
DECODE_BGRA
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  pixel->b = *(p + 0);
  pixel->g = *(p + 1);
  pixel->r = *(p + 2);
  pixel->a = *(p + 3);
}

static void inline
DECODE_RGB
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  pixel->r = *(p + 0);
  pixel->g = *(p + 1);
  pixel->b = *(p + 2);
  pixel->a = 255;
}

static void inline
DECODE_RGBA
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  pixel->r = *(p + 0);
  pixel->g = *(p + 1);
  pixel->b = *(p + 2);
  pixel->a = *(p + 3);
}

static void inline
ENCODE_ABGR
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  *(p + 0) = pixel->a;
  *(p + 1) = pixel->b;
  *(p + 2) = pixel->g;
  *(p + 3) = pixel->r;
}

static void inline
ENCODE_ARGB
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  *(p + 0)=  pixel->a;
  *(p + 1) = pixel->r;
  *(p + 2) = pixel->g;
  *(p + 3) = pixel->b;
}

static void inline
ENCODE_BGR
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  *(p + 0) = pixel->b;
  *(p + 1) = pixel->g;
  *(p + 2) = pixel->r;
}

static void inline
ENCODE_BGRA
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  *(p + 0) = pixel->b;
  *(p + 1) = pixel->g;
  *(p + 2) = pixel->r;
  *(p + 3) = pixel->a;
}

static void inline
ENCODE_RGB
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  *(p + 0) = pixel->r;
  *(p + 1) = pixel->g;
  *(p + 2) = pixel->b;
}

static void inline
ENCODE_RGBA
  (
    Arcadia_Natural8Value* p,
    PIXEL* pixel
  )
{
  *(p + 0) = pixel->r;
  *(p + 1) = pixel->g;
  *(p + 2) = pixel->b;
  *(p + 3) = pixel->a;
}

typedef void (TranscodeCallback)(Arcadia_Natural8Value*, PIXEL*);

static void
getTranscodeCallbacks
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value source,
    size_t* sourceNumberOfBytesPerPixel,
    TranscodeCallback** decode,
    Arcadia_Natural8Value target,
    size_t* targetNumberOfBytesPerPixel,
    TranscodeCallback** encode
  )
{
  typedef struct Entry {
    Arcadia_Natural8Value pixelFormat;
    size_t numberOfBytesPerPixel;
    TranscodeCallback* decodeCallback;
    TranscodeCallback* encodeCallback;
  } Entry;

  static const Entry g_entries[] = {
    { Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8, 4, &DECODE_ABGR, &ENCODE_ABGR },
    { Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8, 4, &DECODE_ARGB, &ENCODE_ARGB },
    { Arcadia_Media_PixelFormat_BlueGreenRedNatural8,      3, &DECODE_BGR,  &ENCODE_BGR  },
    { Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8, 4, &DECODE_BGRA, &ENCODE_BGRA },
    { Arcadia_Media_PixelFormat_RedGreenBlueNatural8,      3, &DECODE_RGB,  &ENCODE_RGB  },
    { Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8, 4, &DECODE_RGBA, &ENCODE_RGBA },
  };

  static const size_t g_numberOfEntries = sizeof(g_entries) / sizeof(Entry);

  Entry const* sourceEntry = NULL;
  for (size_t i = 0, n = g_numberOfEntries; i < n; ++i) {
    if (g_entries[i].pixelFormat == source) {
      sourceEntry = &g_entries[i];
      break;
    }
  }

  Entry const* targetEntry = NULL;
  for (size_t i = 0, n = g_numberOfEntries; i < n; ++i) {
    if (g_entries[i].pixelFormat == target) {
      targetEntry = &g_entries[i];
      break;
    }
  }

  if (!sourceEntry || !targetEntry) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  *decode = sourceEntry->decodeCallback;
  *sourceNumberOfBytesPerPixel = sourceEntry->numberOfBytesPerPixel;
  *encode = targetEntry->encodeCallback;
  *targetNumberOfBytesPerPixel = targetEntry->numberOfBytesPerPixel;
}

/// @code
/// construct(linePadding:Integer32,width:Integer32,height:Integer32,pixelFormat:Natural8)
/// @endcode
///
/// @code
/// construct(pixelBuffer:Arcadia.Media.PixelBuffer)
/// @endcode
static void
Arcadia_Media_PixelBuffer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

static void
Arcadia_Media_PixelBuffer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferDispatch* self
  );

static void
Arcadia_Media_PixelBuffer_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_PixelBuffer_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Media_PixelBuffer_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_PixelBuffer_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.PixelBuffer", Arcadia_Media_PixelBuffer,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Media_PixelBuffer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_PixelBuffer);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 == _numberOfArguments) {
    Arcadia_Media_PixelBuffer* other = (Arcadia_Media_PixelBuffer*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Media_PixelBuffer_getType(thread));
    self->width = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, other);
    self->height = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, other);
    self->linePadding = Arcadia_Media_PixelBuffer_getLinePadding(thread, other);
    self->pixelFormat = Arcadia_Media_PixelBuffer_getPixelFormat(thread, other);
    Arcadia_SizeValue bytesPerPixel = Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, other);
    Arcadia_SizeValue bytes = (bytesPerPixel * self->width + self->linePadding) * self->height;
    self->bytes = Arcadia_Memory_allocateUnmanaged(thread, bytes);
    Arcadia_Memory_copy(thread, self->bytes, other->bytes, bytes);
  } else if (4 == _numberOfArguments) {

    self->bytes = NULL;
    self->linePadding = Arcadia_ValueStack_getInteger32Value(thread, 4);
    self->width = Arcadia_ValueStack_getInteger32Value(thread, 3);
    self->height = Arcadia_ValueStack_getInteger32Value(thread, 2);
    self->pixelFormat = Arcadia_ValueStack_getNatural32Value(thread, 1);

    Arcadia_SizeValue bytesPerPixel = Arcadia_Media_PixelFormat_getNumberOfBytes(thread, self->pixelFormat);
    self->bytes = Arcadia_Memory_allocateUnmanaged(thread, (self->width * bytesPerPixel + self->linePadding) * self->height);
    switch (self->pixelFormat) {
      case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
        Arcadia_SizeValue lineStride = self->width * bytesPerPixel + self->linePadding;
        for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
          Arcadia_Natural8Value* p = self->bytes + (y * lineStride);
          for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
            PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
            ENCODE_ABGR(p + x * bytesPerPixel, &pixel);
          }
        }
      } break;
      case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
        Arcadia_SizeValue lineStride = self->width * bytesPerPixel + self->linePadding;
        for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
          Arcadia_Natural8Value* p = self->bytes + (y * lineStride);
          for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
            PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
            ENCODE_ARGB(p + x * bytesPerPixel, &pixel);
          }
        }
      } break;
      case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
        Arcadia_SizeValue lineStride = self->width * bytesPerPixel + self->linePadding;
        for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
          Arcadia_Natural8Value* p = self->bytes + (y * lineStride);
          for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
            PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
            ENCODE_BGR(p + x * bytesPerPixel, &pixel);
          }
        }
      } break;
      case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
        Arcadia_SizeValue lineStride = self->width * bytesPerPixel + self->linePadding;
        for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
          Arcadia_Natural8Value* p = self->bytes + (y * lineStride);
          for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
            PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
            ENCODE_BGRA(p + x * bytesPerPixel, &pixel);
          }
        }
      } break;
      case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
        Arcadia_SizeValue lineStride = self->width * bytesPerPixel + self->linePadding;
        for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
          Arcadia_Natural8Value* p = self->bytes + (y * lineStride);
          for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
            PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
            ENCODE_RGB(p + x * bytesPerPixel, &pixel);
          }
        }
      } break;
      case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
        Arcadia_SizeValue lineStride = self->width * bytesPerPixel + self->linePadding;
        for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
          Arcadia_Natural8Value* p = self->bytes + (y * lineStride);
          for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
            PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
            ENCODE_RGBA(p + x * bytesPerPixel, &pixel);
          }
        }
      } break;
      default: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      } break;
    };
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Media_PixelBuffer);
}

static void
Arcadia_Media_PixelBuffer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_PixelBuffer_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{
  if (self->bytes) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->bytes);
    self->bytes = NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_Media_PixelFormat
Arcadia_Media_PixelBuffer_getPixelFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return self->pixelFormat; }

void
Arcadia_Media_PixelBuffer_setPixelFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Natural8Value pixelFormat
  )
{
  if (pixelFormat == self->pixelFormat) {
    return;
  }
  TranscodeCallback* decodeCallback = NULL;
  size_t sourceNumberOfBytesPerPixel = 0;
  TranscodeCallback* encodeCallback = NULL;
  size_t targetNumberOfBytesPerPixel = 0;
  getTranscodeCallbacks(thread, self->pixelFormat, &sourceNumberOfBytesPerPixel, &decodeCallback, pixelFormat, &targetNumberOfBytesPerPixel, &encodeCallback);

  Arcadia_Natural8Value* sourceBytes = self->bytes;
  Arcadia_Natural8Value* targetBytes = NULL;
  if (sourceNumberOfBytesPerPixel != targetNumberOfBytesPerPixel) {
    targetBytes = Arcadia_Memory_allocateUnmanaged(thread, (self->width * sourceNumberOfBytesPerPixel + self->linePadding) * self->height);
  } else {
    targetBytes = self->bytes;
  }

  Arcadia_SizeValue sourceLineStride = self->width * sourceNumberOfBytesPerPixel + self->linePadding;
  Arcadia_SizeValue targetLineStride = self->width * targetNumberOfBytesPerPixel + self->linePadding;
  for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
    Arcadia_Natural8Value* p = sourceBytes + (y * sourceLineStride);
    Arcadia_Natural8Value* q = targetBytes + (y * targetLineStride);
    for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
      PIXEL pixel;
      (*decodeCallback)(p + x * sourceNumberOfBytesPerPixel, &pixel);
      (*encodeCallback)(q + x * targetNumberOfBytesPerPixel, &pixel);
    }
  }
  if (sourceNumberOfBytesPerPixel != targetNumberOfBytesPerPixel) {
    Arcadia_Memory_deallocateUnmanaged(thread, sourceBytes);
    self->bytes = targetBytes;
  }
  self->pixelFormat = pixelFormat;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getLinePadding
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return self->linePadding; }

void
Arcadia_Media_PixelBuffer_setLinePadding
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value linePadding
  )
{
  if (self->linePadding != linePadding) {
    if (linePadding < 0) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Integer32Value bytesPerPixel = Arcadia_Media_PixelFormat_getNumberOfBytes(thread, self->pixelFormat);
    Arcadia_Integer32Value oldLinePadding = self->linePadding;
    Arcadia_Integer32Value newLinePadding = linePadding;
    Arcadia_Natural8Value* oldBytes = self->bytes;
    Arcadia_Natural8Value* newBytes = NULL;
    newBytes = Arcadia_Memory_allocateUnmanaged(thread, (self->width * bytesPerPixel + newLinePadding) * self->height);
    for (Arcadia_SizeValue rowIndex = 0; rowIndex < self->height; ++rowIndex) {
      Arcadia_Natural8Value* oldLine = oldBytes + rowIndex * (self->width * bytesPerPixel + oldLinePadding);
      Arcadia_Natural8Value* newLine = newBytes + rowIndex * (self->width * bytesPerPixel + newLinePadding);
      Arcadia_Memory_copy(thread, newLine, oldLine, self->width * bytesPerPixel);
    }
    Arcadia_Memory_deallocateUnmanaged(thread, oldBytes);
    self->bytes = newBytes;
    self->linePadding = linePadding;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Arcadia_Media_PixelBuffer_getPixelRGBA
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value column,
    Arcadia_Integer32Value row,
    Arcadia_Natural8Value* r,
    Arcadia_Natural8Value* g,
    Arcadia_Natural8Value* b,
    Arcadia_Natural8Value* a
  )
{
  if (column < 0 || column > Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (row < 0 || row > Arcadia_Media_PixelBuffer_getNumberOfRows(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue offset = row * Arcadia_Media_PixelBuffer_getLineStride(thread, self) + column * Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self);
  Arcadia_Natural8Value* bytes = self->bytes + offset;
  PIXEL pixel = { .r = 0, .g = 0, .b = 0, .a = 255 };
  switch (self->pixelFormat) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
      DECODE_ABGR(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
      DECODE_ARGB(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
      DECODE_BGR(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
      DECODE_BGRA(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      DECODE_RGB(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      DECODE_RGBA(bytes, &pixel);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  *r = pixel.r;
  *g = pixel.g;
  *b = pixel.b;
  *a = pixel.a;
}

void
Arcadia_Media_PixelBuffer_setPixelRGBA
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value column,
    Arcadia_Integer32Value row,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  )
{
  if (column < 0 || column >= Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (row < 0 || row >= Arcadia_Media_PixelBuffer_getNumberOfRows(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue offset = row * Arcadia_Media_PixelBuffer_getLineStride(thread, self) + column * Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self);
  Arcadia_Natural8Value* bytes = self->bytes + offset;
  PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
  switch (self->pixelFormat) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
      ENCODE_ABGR(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
      ENCODE_ARGB(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
      ENCODE_BGR(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
      ENCODE_BGRA(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      ENCODE_RGB(bytes, &pixel);
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      ENCODE_RGBA(bytes, &pixel);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Arcadia_Media_PixelBuffer_fillRectangle
(
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value top,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  )
{
  if (width < 0 || height < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Integer32Value right, bottom;
  if (!Arcadia_safeAddInteger32Value(left, width, &right) || !Arcadia_safeAddInteger32Value(top, height, &bottom)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // We have the rectangle (left, top, right, bottom); let's clip it.
  if (left < 0) left = 0;
  if (top < 0) top = 0;
  if (right > self->width) right = self->width;
  if (bottom > self->height) bottom = self->height;

  switch (self->pixelFormat) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      Arcadia_SizeValue stride = (self->width * bytesPerPixel) + self->linePadding;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      p = p + top * stride; // "p" points to (top, 0)
      for (Arcadia_SizeValue y = top; y < bottom; ++y) {
        Arcadia_Natural8Value* q = p + left * bytesPerPixel; // "q" points to(y, left)
        for (Arcadia_SizeValue x = left; x < right; ++x) {
          ENCODE_ABGR(q, &pixel);
          q += bytesPerPixel;
        }
        p += stride;
      }
    } break;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      Arcadia_SizeValue stride = (self->width * bytesPerPixel) + self->linePadding;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      p = p + top * stride; // "p" points to (top, 0)
      for (Arcadia_SizeValue y = top; y < bottom; ++y) {
        Arcadia_Natural8Value* q = p + left * bytesPerPixel; // "q" points to(y, left)
        for (Arcadia_SizeValue x = left; x < right; ++x) {
          ENCODE_ARGB(q, &pixel);
          q += bytesPerPixel;
        }
        p += stride;
      }
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
      Arcadia_SizeValue bytesPerPixel = 3;
      Arcadia_SizeValue stride = (self->width * bytesPerPixel) + self->linePadding;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      p = p + top * stride; // "p" points to (top, 0)
      for (Arcadia_SizeValue y = top; y < bottom; ++y) {
        Arcadia_Natural8Value* column = p + left * bytesPerPixel; // "q" points to(y, left)
        for (Arcadia_SizeValue x = left; x < right; ++x) {
          ENCODE_BGR(column, &pixel);
          column += bytesPerPixel;
        }
        p += stride;
      }
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      Arcadia_SizeValue stride = (self->width * bytesPerPixel) + self->linePadding;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      p = p + top * stride; // "p" points to (top, 0)
      for (Arcadia_SizeValue y = top; y < bottom; ++y) {
        Arcadia_Natural8Value* q = p + left * bytesPerPixel; // "q" points to (y, left)
        for (Arcadia_SizeValue x = left; x < right; ++x) {
          ENCODE_BGRA(q, &pixel);
          q += bytesPerPixel;
        }
        p += stride;
      }
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      Arcadia_SizeValue bytesPerPixel = 3;
      Arcadia_SizeValue stride = (self->width * bytesPerPixel) + self->linePadding;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      p = p + top * stride; // "p" points to (top, 0)
      for (Arcadia_SizeValue y = top; y < bottom; ++y) {
        Arcadia_Natural8Value *q = p + left * bytesPerPixel; // "q" points to (y, left)
        for (Arcadia_SizeValue x = left; x < right; ++x) {
          ENCODE_RGB(q, &pixel);
          q += bytesPerPixel;
        }
        p += stride;
      }
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      Arcadia_SizeValue stride = (self->width * bytesPerPixel) + self->linePadding;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      p = p + top * stride; // "p" points to (top, 0)
      for (Arcadia_SizeValue y = top; y < bottom; ++y) {
        Arcadia_Natural8Value* q = p + left * bytesPerPixel; // "q" points to (y, left)
        for (Arcadia_SizeValue x = left; x < right; ++x) {
          ENCODE_RGBA(q, &pixel);
          q += bytesPerPixel;
        }
        p += stride;
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

void
Arcadia_Media_PixelBuffer_fill
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  )
{
  switch (self->pixelFormat) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
        for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
          ENCODE_ABGR(p, &pixel);
          p += bytesPerPixel;
        }
        p += self->linePadding;
      }
    } break;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
        for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
          ENCODE_ARGB(p, &pixel);
          p += bytesPerPixel;
        }
        p += self->linePadding;
      }
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8: {
      Arcadia_SizeValue bytesPerPixel = 3;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
        for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
          ENCODE_BGR(p, &pixel);
          p += bytesPerPixel;
        }
        p += self->linePadding;
      }
    } break;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
        for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
          ENCODE_BGRA(p + x * bytesPerPixel, &pixel);
          p += bytesPerPixel;
        }
        p += self->linePadding;
      }
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8: {
      Arcadia_SizeValue bytesPerPixel = 3;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
        for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
          ENCODE_RGB(p, &pixel);
          p += bytesPerPixel;
        }
        p += self->linePadding;
      }
    } break;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8: {
      Arcadia_SizeValue bytesPerPixel = 4;
      PIXEL pixel = { .r = r, .g = g, .b = b, .a = a };
      Arcadia_Natural8Value* p = self->bytes;
      for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
        for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
          ENCODE_RGBA(p, &pixel);
          p += bytesPerPixel;
        }
        p += self->linePadding;
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Arcadia_Media_PixelBuffer_swapColumns
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value column1,
    Arcadia_Integer32Value column2
  )
{
  if (column1 < 0 || column1 >= self->width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (column2 < 0 || column2 >= self->width) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (column1 == column2) {
    return;
  }
  Arcadia_Integer32Value bytesPerPixel = Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self);
  Arcadia_Integer32Value lineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, self);
  Arcadia_Natural8Value* source = self->bytes + column1 * Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self);
  Arcadia_Natural8Value* target = self->bytes + column2 * Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self);
  for (Arcadia_SizeValue row = 0; row < self->height; ++row) {
    Arcadia_Memory_swap(thread, source, target, bytesPerPixel);
    source += lineStride;
    target += lineStride;
  }
}

void
Arcadia_Media_PixelBuffer_swapRows
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value row1,
    Arcadia_Integer32Value row2
  )
{
  if (row1 < 0 || row1 >= self->height) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (row2 < 0 || row2 >= self->height) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (row1 == row2) {
    return;
  }
  Arcadia_Integer32Value bytesPerPixel = Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self);
  Arcadia_Natural8Value* source = self->bytes + Arcadia_Media_PixelBuffer_getLineStride(thread, self) * row1;
  Arcadia_Natural8Value* target = self->bytes + Arcadia_Media_PixelBuffer_getLineStride(thread, self) * row2;
  Arcadia_Memory_swap(thread, source, target, self->width * bytesPerPixel);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void
Arcadia_Media_PixelBuffer_reflectVertically
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{
  for (Arcadia_Integer32Value column = 0, w = self->width / 2; column < w; ++column) {
    Arcadia_Media_PixelBuffer_swapColumns(thread, self, column, self->width - 1 - column);
  }
}

void
Arcadia_Media_PixelBuffer_reflectHorizontally
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{
  for (Arcadia_Integer32Value row = 0, h = self->height / 2; row < h; ++row) {
    Arcadia_Media_PixelBuffer_swapRows(thread, self, row, self->height - 1 - row);
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getNumberOfColumns
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return Arcadia_Media_PixelBuffer_getWidth(thread, self); }

void
Arcadia_Media_PixelBuffer_setNumberOfColumns
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value numberOfColumns
  )
{ Arcadia_Media_PixelBuffer_setWidth(thread, self, numberOfColumns); }

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getNumberOfRows
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return Arcadia_Media_PixelBuffer_getHeight(thread, self); }

void
Arcadia_Media_PixelBuffer_setNumberOfRows
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value numberOfRows
  )
{ Arcadia_Media_PixelBuffer_setHeight(thread, self, numberOfRows); }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getLineStride
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self)
       * Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, self)
       + Arcadia_Media_PixelBuffer_getLinePadding(thread, self);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getBytesPerPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return Arcadia_Media_PixelFormat_getNumberOfBytes(thread, self->pixelFormat); }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Arcadia_Media_PixelBuffer*
Arcadia_Media_PixelBuffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value linePadding,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Natural8Value pixelFormat
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_PixelBuffer);
  Arcadia_ValueStack_pushInteger32Value(thread, linePadding);
  Arcadia_ValueStack_pushInteger32Value(thread, width);
  Arcadia_ValueStack_pushInteger32Value(thread, height);
  Arcadia_ValueStack_pushNatural32Value(thread, pixelFormat);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Media_PixelBuffer);
}

Arcadia_Media_PixelBuffer*
Arcadia_Media_PixelBuffer_createClone
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* other
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_PixelBuffer);
  if (other) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, other);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Media_PixelBuffer);
}

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return self->width; }

void
Arcadia_Media_PixelBuffer_setWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value width
  )
{
  if (self->width < width) {
    Arcadia_Media_PixelBuffer* temporary = Arcadia_Media_PixelBuffer_create(thread, self->linePadding, width, self->height, self->pixelFormat);
    for (size_t y = 0; y < self->height; ++y) {
      for (size_t x = 0; x < self->width; ++x) {
        Arcadia_Natural8Value r, g, b, a;
        Arcadia_Media_PixelBuffer_getPixelRGBA(thread, self, x, y, &r, &g, &b, &a);
        Arcadia_Media_PixelBuffer_setPixelRGBA(thread, temporary, x, y, r, g, b, a);
      }
    }
    Arcadia_Media_PixelBuffer_swap(thread, self, temporary);
  } else if (self->width > width) {
    Arcadia_Media_PixelBuffer* temporary = Arcadia_Media_PixelBuffer_create(thread, self->linePadding, width, self->height, self->pixelFormat);
    for (size_t y = 0; y < self->height; ++y) {
      for (size_t x = 0; x < width; ++x) {
        Arcadia_Natural8Value r, g, b, a;
        Arcadia_Media_PixelBuffer_getPixelRGBA(thread, self, x, y, &r, &g, &b, &a);
        Arcadia_Media_PixelBuffer_setPixelRGBA(thread, temporary, x, y, r, g, b, a);
      }
    }
    Arcadia_Media_PixelBuffer_swap(thread, self, temporary);
  }
}

Arcadia_Integer32Value
Arcadia_Media_PixelBuffer_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self
  )
{ return self->height; }

void
Arcadia_Media_PixelBuffer_setHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Integer32Value height
  )
{
  if (self->height < height) {
    Arcadia_Media_PixelBuffer* temporary = Arcadia_Media_PixelBuffer_create(thread, self->linePadding, self->width, height, self->pixelFormat);
    for (size_t y = 0; y < self->height; ++y) {
      for (size_t x = 0; x < self->width; ++x) {
        Arcadia_Natural8Value r, g, b, a;
        Arcadia_Media_PixelBuffer_getPixelRGBA(thread, self, x, y, &r, &g, &b, &a);
        Arcadia_Media_PixelBuffer_setPixelRGBA(thread, temporary, x, y, r, g, b, a);
      }
    }
    Arcadia_Media_PixelBuffer_swap(thread, self, temporary);
  } else if (self->height > height) {
    Arcadia_Media_PixelBuffer* temporary = Arcadia_Media_PixelBuffer_create(thread, self->linePadding, self->width, height, self->pixelFormat);
    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < self->width; ++x) {
        Arcadia_Natural8Value r, g, b, a;
        Arcadia_Media_PixelBuffer_getPixelRGBA(thread, self, x, y, &r, &g, &b, &a);
        Arcadia_Media_PixelBuffer_setPixelRGBA(thread, temporary, x, y, r, g, b, a);
      }
    }
    Arcadia_Media_PixelBuffer_swap(thread, self, temporary);
  }
}

void
Arcadia_Media_PixelBuffer_swap
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Media_PixelBuffer* other
  )
{
  if (self != other) {
    Arcadia_swapInteger32(thread, (Arcadia_Integer32Value*) & self->pixelFormat, (Arcadia_Integer32Value*) & other->pixelFormat);
    Arcadia_swapInteger32(thread, &self->width, &other->width);
    Arcadia_swapInteger32(thread, &self->height, &other->height);
    Arcadia_swapInteger32(thread, &self->linePadding, &other->linePadding);
    Arcadia_swapPointer(thread, (void**)&self->bytes, (void**)&other->bytes);
  }
}

void
Arcadia_Media_PixelBuffer_assign
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* self,
    Arcadia_Media_PixelBuffer* other
  )
{
  if (self == other) {
    return;
  } else {
    Arcadia_SizeValue bytesPerPixel = Arcadia_Media_PixelBuffer_getBytesPerPixel(thread, other);
    Arcadia_SizeValue numberOfBytes = (other->width * bytesPerPixel + other->linePadding) * other->height;
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->bytes, numberOfBytes);
    Arcadia_Memory_copy(thread, self->bytes, other->bytes, numberOfBytes);
    self->width = other->width;
    self->height = other->height;
    self->linePadding = other->linePadding;
    self->pixelFormat = other->pixelFormat;
  }
}
