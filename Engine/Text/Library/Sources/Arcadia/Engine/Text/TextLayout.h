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

#if !defined(ARCADIA_ENGINE_TEXT_TEXTLAYOUT_H_INCLUDED)
#define ARCADIA_ENGINE_TEXT_TEXTLAYOUT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_TEXT_PRIVATE) || 1 != ARCADIA_ENGINE_TEXT_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Text/Include.h` instead")
#endif

#include "Arcadia/Engine/Text/TextAlignment.h"

typedef struct Arcadia_Engine_Text_GlyphPlacement {
  Arcadia_Integer32Value width;
  Arcadia_Integer32Value height;
  Arcadia_Integer32Value bearingX;
  Arcadia_Integer32Value bearingY;
  Arcadia_Integer32Value advanceX;
  Arcadia_Integer32Value atlasLeft;
  Arcadia_Integer32Value atlasTop;
} Arcadia_Engine_Text_GlyphPlacement;

typedef struct Arcadia_Engine_Text_Extents {
  Arcadia_Integer32Value top;
  Arcadia_Integer32Value bottom;
  Arcadia_Integer32Value firstBearingX;
  Arcadia_Integer32Value totalAdvanceX;
} Arcadia_Engine_Text_Extents;

typedef struct Arcadia_Engine_Text_LineLayoutElement {
  Arcadia_BooleanValue active;
  Arcadia_Engine_Text_TextAlignment alignment;
  Arcadia_Integer32Value rectangleLeft;
  Arcadia_Integer32Value rectangleBottom;
  Arcadia_Integer32Value rectangleWidth;
  Arcadia_Integer32Value rectangleHeight;
  Arcadia_Integer32Value textTop;
  Arcadia_Integer32Value textBottom;
  Arcadia_Integer32Value textWidth;
  Arcadia_Integer32Value penX;
  Arcadia_Integer32Value baselineY;
} Arcadia_Engine_Text_LineLayoutElement;

void
Arcadia_Engine_Text_computeExtents
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_GlyphPlacement const* glyphs,
    Arcadia_SizeValue numberOfGlyphs,
    Arcadia_Engine_Text_Extents* extents
  );

Arcadia_Integer32Value
Arcadia_Engine_Text_computeAlignedPenX
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value rectangleLeft,
    Arcadia_Integer32Value rectangleWidth,
    Arcadia_Integer32Value textWidth,
    Arcadia_Engine_Text_TextAlignment alignment
  );

void
Arcadia_Engine_Text_layoutLineStack
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_LineLayoutElement* elements,
    Arcadia_SizeValue numberOfElements,
    Arcadia_Integer32Value canvasHeight,
    Arcadia_Integer32Value lineGap
  );

void
Arcadia_Engine_Text_writeGlyphQuads
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_GlyphPlacement const* glyphs,
    Arcadia_SizeValue numberOfGlyphs,
    Arcadia_Integer32Value penX,
    Arcadia_Integer32Value baselineY,
    Arcadia_BooleanValue hasClipRectangle,
    Arcadia_Integer32Value clipLeft,
    Arcadia_Integer32Value clipBottom,
    Arcadia_Integer32Value clipWidth,
    Arcadia_Integer32Value clipHeight,
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

#endif // ARCADIA_ENGINE_TEXT_TEXTLAYOUT_H_INCLUDED
