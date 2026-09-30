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

#if !defined(ARCADIA_ENGINE_EXAMPLES_TEXTRENDERING_TEXTRUN_H_INCLUDED)
#define ARCADIA_ENGINE_EXAMPLES_TEXTRENDERING_TEXTRUN_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Text/Include.h"
#include "Arcadia/FontIO/AtlasGlyphInformation.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Examples.TextRendering.TextRun", Arcadia_Engine_Examples_TextRendering_TextRun,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Examples_TextRendering_TextRunDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Engine_Examples_TextRendering_TextRun {
  Arcadia_Object parent;

  // The model node rendering the glyph mesh of this run.
  Arcadia_Engine_Visuals_ModelNode* modelNode;

  // The dimensions, in pixels, of the installed font atlas.
  Arcadia_Integer32Value fontAtlasWidth;
  Arcadia_Integer32Value fontAtlasHeight;

  // Whether this run is placed at a fixed position instead of participating in
  // the centered text stack.
  Arcadia_BooleanValue fixedPlacement;
  Arcadia_Engine_Text_TextAlignment alignment;
  Arcadia_BooleanValue hasRectangle;
  Arcadia_Integer32Value rectangleLeft;
  Arcadia_Integer32Value rectangleBottom;
  Arcadia_Integer32Value rectangleWidth;
  Arcadia_Integer32Value rectangleHeight;

  // Glyph-source code points and glyph information. This unmanaged memory is
  // owned by the run and freed when the run is destroyed.
  Arcadia_Natural32Value* allCodePoints;
  Arcadia_FontIO_AtlasGlyphInformation* allGlyphInformation;
  Arcadia_SizeValue allCodePointCount;

  // Active glyphs. This unmanaged memory is owned by the run and freed when the
  // run is destroyed.
  Arcadia_FontIO_AtlasGlyphInformation* glyphs;
  Arcadia_SizeValue maximumNumberOfGlyphs;
  Arcadia_SizeValue numberOfGlyphs;

  // The color of the run, in RGBA.
  Arcadia_Real32Value red;
  Arcadia_Real32Value green;
  Arcadia_Real32Value blue;
  Arcadia_Real32Value alpha;
};

Arcadia_Engine_Examples_TextRendering_TextRun*
Arcadia_Engine_Examples_TextRendering_TextRun_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ENGINE_EXAMPLES_TEXTRENDERING_TEXTRUN_H_INCLUDED
