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

#include "Arcadia/FontIO/BitmapFont.h"

#define FT2_BUILD_LIBRARY
#include <ft2build.h>
#include FT_FREETYPE_H

/// @code
/// class BitmapFont {
///   construct(path:String,pixelSize:Integer32)
/// }
/// @endcode
struct Arcadia_FontIO_BitmapFontDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_FontIO_BitmapFont {
  Arcadia_Object _parent;
  /// A pointer to the FreeType library handle.
  FT_Library freeTypeLibrary;
  /// A pointer to the FreeType face handle.
  FT_Face freeTypeFace;
};

static void
Arcadia_FontIO_BitmapFont_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self
  );

static void
Arcadia_FontIO_BitmapFont_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFontDispatch* self
  );

static void
Arcadia_FontIO_BitmapFont_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_FontIO_BitmapFont_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_FontIO_BitmapFont_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_FontIO_BitmapFont_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.FontIO.BitmapFont", Arcadia_FontIO_BitmapFont,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_FontIO_BitmapFont_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self
  )
{
  Arcadia_EnterConstructor(Arcadia_FontIO_BitmapFont);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->freeTypeLibrary = NULL;
  self->freeTypeFace = NULL;
  Arcadia_String* path = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  Arcadia_Integer32Value pixelSize = Arcadia_ValueStack_getInteger32Value(thread, 1);
  if (pixelSize <= 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  // Copy the Bytes of the path and terminate them by a zero Byte.
  // This is required as FreeType expects zero-terminated strings but the Bytes of an Arcadia.String are not necessarily zero-terminated.
  char* p = NULL;
  {
    Arcadia_SizeValue numberOfBytes = Arcadia_String_getNumberOfBytes(thread, path);
    Arcadia_Natural8Value const* bytes = Arcadia_String_getBytes(thread, path);
    p = (char*)Arcadia_Memory_allocateUnmanaged(thread, numberOfBytes + 1);
    for (Arcadia_SizeValue i = 0; i < numberOfBytes; ++i) {
      p[i] = (char)(bytes[i]);
    }
    p[numberOfBytes] = '\0';
  }

  // (1) Create the FreeType library handle.
  if (FT_Init_FreeType(&self->freeTypeLibrary)) {
    Arcadia_Memory_deallocateUnmanaged(thread, p);
    p = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  // (2) Create the FreeType face handle.
  if (FT_New_Face(self->freeTypeLibrary, p, 0, &self->freeTypeFace)) {
    self->freeTypeFace = NULL;
    FT_Done_FreeType(self->freeTypeLibrary);
    self->freeTypeLibrary = NULL;
    Arcadia_Memory_deallocateUnmanaged(thread, p);
    p = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  // (3) Set the size of the glyphs, in pixels.
  if (FT_Set_Pixel_Sizes(self->freeTypeFace, 0, (FT_UInt)pixelSize)) {
    FT_Done_Face(self->freeTypeFace);
    self->freeTypeFace = NULL;
    FT_Done_FreeType(self->freeTypeLibrary);
    self->freeTypeLibrary = NULL;
    Arcadia_Memory_deallocateUnmanaged(thread, p);
    p = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Memory_deallocateUnmanaged(thread, p);
  p = NULL;
  Arcadia_LeaveConstructor(Arcadia_FontIO_BitmapFont);
}

static void
Arcadia_FontIO_BitmapFont_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFontDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FontIO_BitmapFont_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self
  )
{
  if (self->freeTypeFace) {
    FT_Done_Face(self->freeTypeFace);
    self->freeTypeFace = NULL;
  }
  if (self->freeTypeLibrary) {
    FT_Done_FreeType(self->freeTypeLibrary);
    self->freeTypeLibrary = NULL;
  }
}

Arcadia_FontIO_BitmapFont*
Arcadia_FontIO_BitmapFont_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_Integer32Value pixelSize
  )
{
  _Arcadia_BeginCreate(Arcadia_FontIO_BitmapFont);
  if (path) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, pixelSize);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_FontIO_BitmapFont);
}

static FT_GlyphSlot
loadGlyph
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self,
    Arcadia_Natural32Value codePoint,
    FT_Int32 loadFlags
  )
{
  FT_UInt glyphIndex = FT_Get_Char_Index((FT_Face)self->freeTypeFace, (FT_ULong)codePoint);
  if (!glyphIndex) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (FT_Load_Glyph((FT_Face)self->freeTypeFace, glyphIndex, loadFlags)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return ((FT_Face)self->freeTypeFace)->glyph;
}

void
Arcadia_FontIO_BitmapFont_getGlyphInformation
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self,
    Arcadia_Natural32Value codePoint,
    Arcadia_FontIO_GlyphInformation* target
  )
{
  if (!target) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  FT_GlyphSlot glyph = loadGlyph(thread, self, codePoint, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);
  // The metrics are in 26.6 fixed point format. Convert them to pixels by rounding.
  FT_Glyph_Metrics const* metrics = &glyph->metrics;
  target->width = (Arcadia_Integer32Value)((metrics->width + 32) >> 6);
  target->height = (Arcadia_Integer32Value)((metrics->height + 32) >> 6);
  target->bearingX = (Arcadia_Integer32Value)((metrics->horiBearingX + 32) >> 6);
  target->bearingY = (Arcadia_Integer32Value)((metrics->horiBearingY + 32) >> 6);
  target->advanceX = (Arcadia_Integer32Value)((metrics->horiAdvance + 32) >> 6);
  target->advanceY = (Arcadia_Integer32Value)((metrics->vertAdvance + 32) >> 6);
}

Arcadia_Media_PixelBuffer*
Arcadia_FontIO_BitmapFont_getGlyphBitmap
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self,
    Arcadia_Natural32Value codePoint
  )
{
  static const Arcadia_Natural8Value g_pixelFormat = Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8;

  FT_GlyphSlot glyph = loadGlyph(thread, self, codePoint, FT_LOAD_DEFAULT);
  if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  FT_Bitmap const* bitmap = &glyph->bitmap;
  if (bitmap->pixel_mode != FT_PIXEL_MODE_GRAY) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Integer32Value bitmapWidth = bitmap->width > 0 ? (Arcadia_Integer32Value)(bitmap->width) : 1;
  Arcadia_Integer32Value bitmapHeight = bitmap->rows > 0 ? (Arcadia_Integer32Value)(bitmap->rows) : 1;
  Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, bitmapWidth, bitmapHeight, g_pixelFormat);
  Arcadia_SizeValue lineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, pixelBuffer);
  Arcadia_Natural8Value* bytes = pixelBuffer->bytes;
  for (Arcadia_Integer32Value y = 0; y < bitmapHeight; ++y) {
    Arcadia_Natural8Value const* sourceRow = bitmap->buffer ? (bitmap->buffer + (size_t)y * (size_t)bitmap->pitch) : NULL;
    Arcadia_Natural8Value* targetRow = bytes + (size_t)y * lineStride;
    for (Arcadia_Integer32Value x = 0; x < bitmapWidth; ++x) {
      Arcadia_Natural8Value coverage = (sourceRow && x < (Arcadia_Integer32Value)bitmap->width)
                                       ? sourceRow[x]
                                       : (Arcadia_Natural8Value)0;
      Arcadia_Natural8Value* targetPixel = targetRow + (size_t)x * 4;
      targetPixel[0] = 255; // r
      targetPixel[1] = 255; // g
      targetPixel[2] = 255; // b
      targetPixel[3] = coverage; // a
    }
  }
  return pixelBuffer;
}
