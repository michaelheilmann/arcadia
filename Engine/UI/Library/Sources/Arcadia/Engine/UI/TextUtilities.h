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

#if !defined(ARCADIA_ENGINE_UI_TEXTUTILITIES_H_INCLUDED)
#define ARCADIA_ENGINE_UI_TEXTUTILITIES_H_INCLUDED

#if !defined(ARCADIA_ENGINE_UI_PRIVATE) || 1 != ARCADIA_ENGINE_UI_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/UI/Include.h` instead")
#endif
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Text/Include.h"
#include "Arcadia/FontIO/Include.h"

typedef struct Arcadia_Engine_UI_TextLine {
  Arcadia_SizeValue firstGlyph;
  Arcadia_SizeValue numberOfGlyphs;
} Arcadia_Engine_UI_TextLine;

void
Arcadia_Engine_UI_TextUtilities_insertDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_ADL_Definition* definition
  );

char const*
Arcadia_Engine_UI_TextUtilities_findFontPath
  (
    Arcadia_Thread* thread
  );

Arcadia_SizeValue
Arcadia_Engine_UI_TextUtilities_countCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  );

void
Arcadia_Engine_UI_TextUtilities_writeCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    Arcadia_Natural32Value* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

void
Arcadia_Engine_UI_TextUtilities_copyGlyphPlacements
  (
    Arcadia_Engine_Text_GlyphPlacement* target,
    Arcadia_FontIO_AtlasGlyphInformation const* source,
    Arcadia_SizeValue numberOfGlyphs
  );

void
Arcadia_Engine_UI_TextUtilities_countGlyphsAndLines
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    Arcadia_SizeValue* numberOfGlyphs,
    Arcadia_SizeValue* numberOfLines
  );

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
  );

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
  );

Arcadia_BooleanValue
Arcadia_Engine_UI_TextUtilities_stringEqualsCxx
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    char const* cxxString
  );

#endif // ARCADIA_ENGINE_UI_TEXTUTILITIES_H_INCLUDED
