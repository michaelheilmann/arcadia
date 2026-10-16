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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/TextUtilities.h"

void
Arcadia_Engine_UI_TextUtilities_insertDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_ADL_Definition* definition
  )
{
  Arcadia_Map_set(thread, definitions->definitions, Arcadia_Value_makeObjectReferenceValue(definition->name), Arcadia_Value_makeObjectReferenceValue(definition), NULL, NULL);
}

char const*
Arcadia_Engine_UI_TextUtilities_findFontPath
  (
    Arcadia_Thread* thread
  )
{
  static char const* fallbackFontPaths[] = {
    "C:/Windows/Fonts/arial.ttf",
    "C:/Windows/Fonts/segoeui.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
  };
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  for (Arcadia_SizeValue i = 0; i < sizeof(fallbackFontPaths) / sizeof(fallbackFontPaths[0]); ++i) {
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, fallbackFontPaths[i]));
    if (Arcadia_FileSystem_regularFileExists(thread, fileSystem, filePath)) {
      return fallbackFontPaths[i];
    }
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
  return NULL;
}

Arcadia_SizeValue
Arcadia_Engine_UI_TextUtilities_countCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  Arcadia_SizeValue numberOfCodePoints = 0;
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    ++numberOfCodePoints;
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  }
  return numberOfCodePoints;
}

void
Arcadia_Engine_UI_TextUtilities_writeCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    Arcadia_Natural32Value* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
    codePoints[i] = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    if (i + 1 < numberOfCodePoints) {
      Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    }
  }
}

void
Arcadia_Engine_UI_TextUtilities_copyGlyphPlacements
  (
    Arcadia_Engine_Text_GlyphPlacement* target,
    Arcadia_FontIO_AtlasGlyphInformation const* source,
    Arcadia_SizeValue numberOfGlyphs
  )
{
  for (Arcadia_SizeValue i = 0; i < numberOfGlyphs; ++i) {
    target[i].width = source[i].glyphInformation.width;
    target[i].height = source[i].glyphInformation.height;
    target[i].bearingX = source[i].glyphInformation.bearingX;
    target[i].bearingY = source[i].glyphInformation.bearingY;
    target[i].advanceX = source[i].glyphInformation.advanceX;
    target[i].atlasLeft = source[i].x;
    target[i].atlasTop = source[i].y;
  }
}

void
Arcadia_Engine_UI_TextUtilities_countGlyphsAndLines
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    Arcadia_SizeValue* numberOfGlyphs,
    Arcadia_SizeValue* numberOfLines
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  *numberOfGlyphs = 0;
  *numberOfLines = 1;
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    if ('\r' == codePoint || '\n' == codePoint) {
      if ('\r' == codePoint && Arcadia_UnicodeCodePointReader_hasValue(thread, reader) && '\n' == Arcadia_UnicodeCodePointReader_getValue(thread, reader)) {
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
      }
      ++(*numberOfLines);
    } else {
      ++(*numberOfGlyphs);
    }
  }
}

void
Arcadia_Engine_UI_TextUtilities_loadGlyphsAndLines
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* font,
    Arcadia_String* string,
    Arcadia_FontIO_AtlasGlyphInformation* glyphs,
    Arcadia_SizeValue numberOfGlyphs,
    Arcadia_Engine_UI_TextLine* lines,
    Arcadia_SizeValue numberOfLines
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  Arcadia_SizeValue glyphIndex = 0;
  Arcadia_SizeValue lineIndex = 0;
  if (numberOfLines) {
    lines[0].firstGlyph = 0;
    lines[0].numberOfGlyphs = 0;
  }
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    if ('\r' == codePoint || '\n' == codePoint) {
      if ('\r' == codePoint && Arcadia_UnicodeCodePointReader_hasValue(thread, reader) && '\n' == Arcadia_UnicodeCodePointReader_getValue(thread, reader)) {
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
      }
      ++lineIndex;
      if (lineIndex < numberOfLines) {
        lines[lineIndex].firstGlyph = glyphIndex;
        lines[lineIndex].numberOfGlyphs = 0;
      }
    } else {
      if (glyphIndex < numberOfGlyphs) {
        Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, codePoint, &glyphs[glyphIndex]);
        ++glyphIndex;
        if (lineIndex < numberOfLines) {
          ++lines[lineIndex].numberOfGlyphs;
        }
      }
    }
  }
}

static Arcadia_Engine_Text_TextAlignment
getHorizontalTextAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_String* horizontalAlignment
  )
{
  if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, horizontalAlignment, u8"Right")) {
    return Arcadia_Engine_Text_TextAlignment_Right;
  } else if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, horizontalAlignment, u8"Center")) {
    return Arcadia_Engine_Text_TextAlignment_Center;
  } else {
    return Arcadia_Engine_Text_TextAlignment_Left;
  }
}

void
Arcadia_Engine_UI_TextUtilities_writeMultilineGlyphQuads
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasGlyphInformation const* glyphs,
    Arcadia_SizeValue numberOfGlyphs,
    Arcadia_Engine_UI_TextLine const* lines,
    Arcadia_SizeValue numberOfLines,
    Arcadia_Integer32Value lineTop,
    Arcadia_Integer32Value lineBottom,
    Arcadia_Integer32Value rectangleLeft,
    Arcadia_Integer32Value rectangleBottom,
    Arcadia_Integer32Value rectangleWidth,
    Arcadia_Integer32Value rectangleHeight,
    Arcadia_String* horizontalAlignment,
    Arcadia_String* verticalAlignment,
    Arcadia_BooleanValue hasClipRectangle,
    Arcadia_Real32Value atlasWidth,
    Arcadia_Real32Value atlasHeight,
    Arcadia_Real32Value red,
    Arcadia_Real32Value green,
    Arcadia_Real32Value blue,
    Arcadia_Real32Value alpha,
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue numberOfVertices,
    Arcadia_SizeValue numberOfVertexComponents
  )
{
  for (Arcadia_SizeValue i = 0; i < numberOfVertices * numberOfVertexComponents; ++i) {
    vertices[i] = 0.f;
  }
  if (!numberOfGlyphs || !numberOfLines) {
    return;
  }
  Arcadia_Engine_Text_GlyphPlacement* placements = Arcadia_Memory_allocateUnmanaged(thread, numberOfGlyphs * sizeof(Arcadia_Engine_Text_GlyphPlacement));
  Arcadia_Engine_UI_TextUtilities_copyGlyphPlacements(placements, glyphs, numberOfGlyphs);

  Arcadia_Integer32Value const lineHeight = lineTop - lineBottom;
  Arcadia_Integer32Value const lineGap = lineHeight / 4;
  Arcadia_Integer32Value const totalHeight = (Arcadia_Integer32Value)numberOfLines * lineHeight + (Arcadia_Integer32Value)(numberOfLines - 1) * lineGap;
  Arcadia_Integer32Value topY;
  if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, verticalAlignment, u8"Top")) {
    topY = rectangleBottom + rectangleHeight;
  } else if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, verticalAlignment, u8"Bottom")) {
    topY = rectangleBottom + totalHeight;
  } else {
    topY = rectangleBottom + (rectangleHeight + totalHeight) / 2;
  }

  Arcadia_Engine_Text_TextAlignment const alignment = getHorizontalTextAlignment(thread, horizontalAlignment);
  for (Arcadia_SizeValue i = 0; i < numberOfLines; ++i) {
    if (!lines[i].numberOfGlyphs) {
      continue;
    }
    Arcadia_Engine_Text_Extents extents;
    Arcadia_Engine_Text_computeExtents(thread, placements + lines[i].firstGlyph, lines[i].numberOfGlyphs, &extents);
    Arcadia_Integer32Value const textWidth = extents.firstBearingX + extents.totalAdvanceX;
    Arcadia_Integer32Value const penX = Arcadia_Engine_Text_computeAlignedPenX(thread, rectangleLeft, rectangleWidth, textWidth, alignment);
    Arcadia_Integer32Value const baselineY = topY - (Arcadia_Integer32Value)i * (lineHeight + lineGap) - lineTop;
    Arcadia_Engine_Text_writeGlyphQuads(thread, placements + lines[i].firstGlyph, lines[i].numberOfGlyphs, penX, baselineY,
                                        hasClipRectangle, rectangleLeft, rectangleBottom, rectangleWidth, rectangleHeight,
                                        atlasWidth, atlasHeight, red, green, blue, alpha,
                                        vertices + lines[i].firstGlyph * 6 * numberOfVertexComponents,
                                        numberOfVertices > lines[i].firstGlyph * 6 ? numberOfVertices - lines[i].firstGlyph * 6 : 0,
                                        numberOfVertexComponents);
  }
  Arcadia_Memory_deallocateUnmanaged(thread, placements);
}

Arcadia_BooleanValue
Arcadia_Engine_UI_TextUtilities_stringEqualsCxx
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    char const* cxxString
  )
{
  Arcadia_String* other = Arcadia_String_createFromCxxString(thread, cxxString);
  Arcadia_Value otherValue = Arcadia_Value_makeObjectReferenceValue(other);
  return Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)string, &otherValue);
}
