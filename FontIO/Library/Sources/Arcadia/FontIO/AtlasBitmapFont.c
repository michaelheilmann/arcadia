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

#include "Arcadia/FontIO/AtlasBitmapFont.h"

#define FT2_BUILD_LIBRARY
#include <ft2build.h>
#include FT_FREETYPE_H

/// @code
/// class AtlasBitmapFont {
///   construct(path:String,pixelSize:Integer32)
/// }
/// @endcode
struct Arcadia_FontIO_AtlasBitmapFontDispatch {
  Arcadia_ObjectDispatch parent;
};

/// An entry of the glyph cache.
typedef struct CacheEntry {

  /// The unicode code point of the glyph.
  Arcadia_Natural32Value codePoint;

  /// The information about the glyph image within the atlas.
  Arcadia_FontIO_AtlasGlyphInformation information;

} CacheEntry;

struct Arcadia_FontIO_AtlasBitmapFont {
  Arcadia_Object _parent;
  /// A pointer to the bitmap font used to render the glyph images.
  Arcadia_FontIO_BitmapFont* bitmapFont;
  /// A pointer to the atlas. The format of the atlas is RGBA, its line padding is 0.
  Arcadia_Media_PixelBuffer* atlas;
  /// The shelf packer state: x-position of the next glyph image, in pixels.
  Arcadia_Integer32Value cursorX;
  /// The shelf packer state: y-position of the current shelf, in pixels.
  Arcadia_Integer32Value cursorY;
  /// The shelf packer state: height of the current shelf, in pixels.
  Arcadia_Integer32Value shelfHeight;
  /// The glyph cache.
  CacheEntry* cacheEntries;
  /// The number of elements in the glyph cache.
  Arcadia_SizeValue numberOfCacheEntries;
  /// The capacity of the glyph cache.
  Arcadia_SizeValue capacityOfCacheEntries;
};

static void
Arcadia_FontIO_AtlasBitmapFont_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  );

static void
Arcadia_FontIO_AtlasBitmapFont_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFontDispatch* self
  );

static void
Arcadia_FontIO_AtlasBitmapFont_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  );

static void
Arcadia_FontIO_AtlasBitmapFont_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_FontIO_AtlasBitmapFont_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_FontIO_AtlasBitmapFont_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_FontIO_AtlasBitmapFont_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_FontIO_AtlasBitmapFont_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.FontIO.AtlasBitmapFont", Arcadia_FontIO_AtlasBitmapFont,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_FontIO_AtlasBitmapFont_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  )
{
  Arcadia_EnterConstructor(Arcadia_FontIO_AtlasBitmapFont);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->bitmapFont = NULL;
  self->atlas = NULL;
  self->cursorX = 0;
  self->cursorY = 0;
  self->shelfHeight = 0;
  self->cacheEntries = NULL;
  self->numberOfCacheEntries = 0;
  self->capacityOfCacheEntries = 0;
  Arcadia_String* path = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  Arcadia_Integer32Value pixelSize = Arcadia_ValueStack_getInteger32Value(thread, 1);

  // Create the bitmap font backing this atlas bitmap font.
  self->bitmapFont = Arcadia_FontIO_BitmapFont_create(thread, path, pixelSize);
  // Create the atlas.
  self->atlas = Arcadia_Media_PixelBuffer_create(thread, 0, 128, 128, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_LeaveConstructor(Arcadia_FontIO_AtlasBitmapFont);
}

static void
Arcadia_FontIO_AtlasBitmapFont_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFontDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FontIO_AtlasBitmapFont_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  )
{
  if (self->bitmapFont) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->bitmapFont);
  }
  if (self->atlas) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->atlas);
  }
}

static void
Arcadia_FontIO_AtlasBitmapFont_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  )
{
  if (self->cacheEntries) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->cacheEntries);
    self->cacheEntries = NULL;
  }
  self->numberOfCacheEntries = 0;
  self->capacityOfCacheEntries = 0;
}

Arcadia_FontIO_AtlasBitmapFont*
Arcadia_FontIO_AtlasBitmapFont_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_Integer32Value pixelSize
  )
{
  _Arcadia_BeginCreate(Arcadia_FontIO_AtlasBitmapFont);
  if (path) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, pixelSize);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_FontIO_AtlasBitmapFont);
}

Arcadia_Media_PixelBuffer*
Arcadia_FontIO_AtlasBitmapFont_getAtlas
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  )
{
  return self->atlas;
}

/// @brief Grow the atlas so that it is at least @a width x @a height pixels large.
/// The contents of the atlas are preserved. Existing glyph positions remain valid as
/// glyphs are packed from the top-left corner and the atlas only grows to the right/bottom.
static void
growAtlas
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  static const Arcadia_Natural8Value g_pixelFormat = Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8;

  Arcadia_Integer32Value oldWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self->atlas);
  Arcadia_Integer32Value oldHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, self->atlas);
  Arcadia_Integer32Value newWidth = oldWidth, newHeight = oldHeight;
  while (newWidth < width) {
    newWidth *= 2;
  }
  while (newHeight < height) {
    newHeight *= 2;
  }
  Arcadia_Media_PixelBuffer* newAtlas = Arcadia_Media_PixelBuffer_create(thread, 0, newWidth, newHeight, g_pixelFormat);
  // Copy the rows of the old atlas into the new atlas.
  Arcadia_SizeValue oldLineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, self->atlas);
  Arcadia_SizeValue newLineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, newAtlas);
  for (Arcadia_Integer32Value y = 0; y < oldHeight; ++y) {
    Arcadia_Memory_copy(thread,
                        newAtlas->bytes + (size_t)y * newLineStride,
                        self->atlas->bytes + (size_t)y * oldLineStride,
                        oldLineStride);
  }
  self->atlas = newAtlas;
}

/// @brief Find a free rectangle for a glyph image of the specified size.
/// Uses simple shelf packing with one pixel of padding between glyph images.
static void
packGlyph
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_FontIO_AtlasGlyphInformation* target
  )
{
  static const Arcadia_Integer32Value g_padding = 1;

  Arcadia_Integer32Value atlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self->atlas);
  Arcadia_Integer32Value atlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, self->atlas);

  // Ensure that the glyph image fits into the width of the atlas.
  while (width + g_padding > atlasWidth) {
    growAtlas(thread, self, atlasWidth * 2, atlasHeight);
    atlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self->atlas);
    atlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, self->atlas);
  }

  // If the glyph image does not fit into the remaining space of the current shelf, advance to the next shelf.
  if (self->cursorX + width + g_padding > atlasWidth) {
    self->cursorY += self->shelfHeight + g_padding;
    self->cursorX = g_padding;
    self->shelfHeight = 0;
  }

  // Ensure that the glyph image fits into the height of the atlas.
  while (self->cursorY + height + g_padding > atlasHeight) {
    growAtlas(thread, self, atlasWidth, atlasHeight * 2);
    atlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, self->atlas);
    atlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, self->atlas);
  }

  target->x = self->cursorX;
  target->y = self->cursorY;
  self->cursorX += width + g_padding;
  if (height > self->shelfHeight) {
    self->shelfHeight = height;
  }
}

static CacheEntry*
findCacheEntry
  (
    Arcadia_FontIO_AtlasBitmapFont* self,
    Arcadia_Natural32Value codePoint
  )
{
  for (Arcadia_SizeValue i = 0; i < self->numberOfCacheEntries; ++i) {
    if (self->cacheEntries[i].codePoint == codePoint) {
      return &self->cacheEntries[i];
    }
  }
  return NULL;
}

void
Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self,
    Arcadia_Natural32Value codePoint,
    Arcadia_FontIO_AtlasGlyphInformation* target
  )
{
  if (!target) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  // If the glyph is cached, return its information.
  CacheEntry* cacheEntry = findCacheEntry(self, codePoint);
  if (cacheEntry) {
    *target = cacheEntry->information;
    return;
  }

  // Otherwise render the glyph image, pack it into the atlas, and cache its information.
  Arcadia_FontIO_GlyphInformation glyphInformation;
  Arcadia_FontIO_BitmapFont_getGlyphInformation(thread, self->bitmapFont, codePoint, &glyphInformation);
  Arcadia_Media_PixelBuffer* glyphImage = Arcadia_FontIO_BitmapFont_getGlyphBitmap(thread, self->bitmapFont, codePoint);

  Arcadia_FontIO_AtlasGlyphInformation atlasGlyphInformation;
  packGlyph(thread, self,
            Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, glyphImage),
            Arcadia_Media_PixelBuffer_getNumberOfRows(thread, glyphImage),
            &atlasGlyphInformation);
  atlasGlyphInformation.glyphInformation = glyphInformation;

  // Blit the glyph image into the atlas.
  Arcadia_SizeValue sourceLineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, glyphImage);
  Arcadia_SizeValue targetLineStride = Arcadia_Media_PixelBuffer_getLineStride(thread, self->atlas);
  Arcadia_Integer32Value glyphImageHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, glyphImage);
  Arcadia_Integer32Value glyphImageWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, glyphImage);
  for (Arcadia_Integer32Value y = 0; y < glyphImageHeight; ++y) {
    Arcadia_Memory_copy(thread,
                        self->atlas->bytes + (size_t)(atlasGlyphInformation.y + y) * targetLineStride
                                           + (size_t)atlasGlyphInformation.x * 4,
                        glyphImage->bytes + (size_t)y * sourceLineStride,
                        (size_t)glyphImageWidth * 4);
  }

  // Add the glyph information to the cache.
  if (self->numberOfCacheEntries == self->capacityOfCacheEntries) {
    Arcadia_SizeValue newCapacity = self->capacityOfCacheEntries > 0 ? self->capacityOfCacheEntries * 2 : 64;
    CacheEntry* newCacheEntries = (CacheEntry*)Arcadia_Memory_allocateUnmanaged(thread, newCapacity * sizeof(CacheEntry));
    if (self->numberOfCacheEntries > 0) {
      Arcadia_Memory_copy(thread, newCacheEntries, self->cacheEntries, self->numberOfCacheEntries * sizeof(CacheEntry));
      Arcadia_Memory_deallocateUnmanaged(thread, self->cacheEntries);
    }
    self->cacheEntries = newCacheEntries;
    self->capacityOfCacheEntries = newCapacity;
  }
  self->cacheEntries[self->numberOfCacheEntries].codePoint = codePoint;
  self->cacheEntries[self->numberOfCacheEntries].information = atlasGlyphInformation;
  self->numberOfCacheEntries++;

  *target = atlasGlyphInformation;
}
