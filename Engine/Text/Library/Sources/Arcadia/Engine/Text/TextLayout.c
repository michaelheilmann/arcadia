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

#define ARCADIA_ENGINE_TEXT_PRIVATE (1)
#include "Arcadia/Engine/Text/TextLayout.h"

void
Arcadia_Engine_Text_computeExtents
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_GlyphPlacement const* glyphs,
    Arcadia_SizeValue numberOfGlyphs,
    Arcadia_Engine_Text_Extents* extents
  )
{
  extents->top = 0;
  extents->bottom = 0;
  extents->firstBearingX = 0;
  extents->totalAdvanceX = 0;
  for (Arcadia_SizeValue i = 0; i < numberOfGlyphs; ++i) {
    if (0 == i) {
      extents->firstBearingX = glyphs[i].bearingX;
    }
    extents->totalAdvanceX += glyphs[i].advanceX;
    if (glyphs[i].bearingY > extents->top) {
      extents->top = glyphs[i].bearingY;
    }
    Arcadia_Integer32Value const bottomEdge = glyphs[i].bearingY - glyphs[i].height;
    if (bottomEdge < extents->bottom) {
      extents->bottom = bottomEdge;
    }
  }
}

Arcadia_Integer32Value
Arcadia_Engine_Text_computeAlignedPenX
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value rectangleLeft,
    Arcadia_Integer32Value rectangleWidth,
    Arcadia_Integer32Value textWidth,
    Arcadia_Engine_Text_TextAlignment alignment
  )
{
  switch (alignment) {
    case Arcadia_Engine_Text_TextAlignment_Left:
      return rectangleLeft;
    case Arcadia_Engine_Text_TextAlignment_Right:
      return rectangleLeft + rectangleWidth - textWidth;
    case Arcadia_Engine_Text_TextAlignment_Center:
    default:
      return rectangleLeft + (rectangleWidth - textWidth) / 2;
  }
}

void
Arcadia_Engine_Text_layoutLineStack
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_LineLayoutElement* elements,
    Arcadia_SizeValue numberOfElements,
    Arcadia_Integer32Value canvasHeight,
    Arcadia_Integer32Value lineGap
  )
{
  Arcadia_SizeValue numberOfActiveElements = 0;
  Arcadia_Integer32Value totalHeight = 0;
  for (Arcadia_SizeValue i = 0; i < numberOfElements; ++i) {
    if (!elements[i].active) {
      continue;
    }
    totalHeight += elements[i].textTop - elements[i].textBottom;
    ++numberOfActiveElements;
  }
  if (numberOfActiveElements > 1) {
    totalHeight += (Arcadia_Integer32Value)(numberOfActiveElements - 1) * lineGap;
  }

  Arcadia_Integer32Value y = (canvasHeight + totalHeight) / 2;
  for (Arcadia_SizeValue i = 0; i < numberOfElements; ++i) {
    if (!elements[i].active) {
      continue;
    }
    elements[i].penX = Arcadia_Engine_Text_computeAlignedPenX(thread, elements[i].rectangleLeft, elements[i].rectangleWidth, elements[i].textWidth, elements[i].alignment);
    elements[i].baselineY = y - elements[i].textTop;
    y = elements[i].baselineY + elements[i].textBottom - lineGap;
  }
}

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
  )
{
  for (Arcadia_SizeValue i = 0; i < numberOfVertices * numberOfVertexComponents; ++i) {
    vertices[i] = 0.f;
  }
  Arcadia_Real32Value const clipX0 = (Arcadia_Real32Value)clipLeft;
  Arcadia_Real32Value const clipX1 = (Arcadia_Real32Value)(clipLeft + clipWidth);
  Arcadia_Real32Value const clipY0 = (Arcadia_Real32Value)clipBottom;
  Arcadia_Real32Value const clipY1 = (Arcadia_Real32Value)(clipBottom + clipHeight);
  Arcadia_Integer32Value penXCurrent = penX;
  for (Arcadia_SizeValue i = 0; i < numberOfGlyphs; ++i) {
    Arcadia_Engine_Text_GlyphPlacement const* glyph = &glyphs[i];
    if (glyph->width <= 0 || glyph->height <= 0) {
      penXCurrent += glyph->advanceX;
      continue;
    }
    Arcadia_Real32Value const x0 = (Arcadia_Real32Value)(penXCurrent + glyph->bearingX);
    Arcadia_Real32Value const x1 = x0 + (Arcadia_Real32Value)glyph->width;
    Arcadia_Real32Value const yTop = (Arcadia_Real32Value)(baselineY + glyph->bearingY);
    Arcadia_Real32Value const yBottom = yTop - (Arcadia_Real32Value)glyph->height;
    Arcadia_Real32Value clippedX0 = x0;
    Arcadia_Real32Value clippedX1 = x1;
    Arcadia_Real32Value clippedYTop = yTop;
    Arcadia_Real32Value clippedYBottom = yBottom;
    if (hasClipRectangle) {
      if (clippedX0 < clipX0) clippedX0 = clipX0;
      if (clippedX1 > clipX1) clippedX1 = clipX1;
      if (clippedYBottom < clipY0) clippedYBottom = clipY0;
      if (clippedYTop > clipY1) clippedYTop = clipY1;
      if (clippedX0 >= clippedX1 || clippedYBottom >= clippedYTop) {
        penXCurrent += glyph->advanceX;
        continue;
      }
    }
    Arcadia_Real32Value const sourceX0 = (Arcadia_Real32Value)glyph->atlasLeft + 0.5f;
    Arcadia_Real32Value const sourceX1 = (Arcadia_Real32Value)glyph->atlasLeft + (Arcadia_Real32Value)glyph->width - 0.5f;
    Arcadia_Real32Value const sourceY0 = (Arcadia_Real32Value)glyph->atlasTop + 0.5f;
    Arcadia_Real32Value const sourceY1 = (Arcadia_Real32Value)glyph->atlasTop + (Arcadia_Real32Value)glyph->height - 0.5f;
    Arcadia_Real32Value const u0 = (sourceX0 + (sourceX1 - sourceX0) * ((clippedX0 - x0) / (x1 - x0))) / atlasWidth;
    Arcadia_Real32Value const u1 = (sourceX0 + (sourceX1 - sourceX0) * ((clippedX1 - x0) / (x1 - x0))) / atlasWidth;
    Arcadia_Real32Value const v0 = (sourceY0 + (sourceY1 - sourceY0) * ((yTop - clippedYTop) / (yTop - yBottom))) / atlasHeight;
    Arcadia_Real32Value const v1 = (sourceY0 + (sourceY1 - sourceY0) * ((yTop - clippedYBottom) / (yTop - yBottom))) / atlasHeight;
    Arcadia_Real32Value const vertexData[6][9] = {
      { clippedX0, clippedYBottom, 0.f, red, green, blue, alpha, u0, v1 },
      { clippedX1, clippedYBottom, 0.f, red, green, blue, alpha, u1, v1 },
      { clippedX0, clippedYTop,    0.f, red, green, blue, alpha, u0, v0 },
      { clippedX1, clippedYBottom, 0.f, red, green, blue, alpha, u1, v1 },
      { clippedX1, clippedYTop,    0.f, red, green, blue, alpha, u1, v0 },
      { clippedX0, clippedYTop,    0.f, red, green, blue, alpha, u0, v0 },
    };
    for (Arcadia_SizeValue j = 0; j < 6 && i * 6 + j < numberOfVertices; ++j) {
      Arcadia_SizeValue const base = (i * 6 + j) * numberOfVertexComponents;
      for (Arcadia_SizeValue k = 0; k < 9 && k < numberOfVertexComponents; ++k) {
        vertices[base + k] = vertexData[j][k];
      }
    }
    penXCurrent += glyph->advanceX;
  }
}
