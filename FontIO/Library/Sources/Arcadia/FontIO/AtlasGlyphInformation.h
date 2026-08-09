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

#if !defined(ARCADIA_FONTIO_ATLASGLYPHINFORMATION_H_INCLUDED)
#define ARCADIA_FONTIO_ATLASGLYPHINFORMATION_H_INCLUDED

#include "Arcadia/Media/Include.h"
#include "Arcadia/FontIO/GlyphInformation.h"

/// @brief Information about a glyph image stored within the atlas of an atlas bitmap font.
typedef struct Arcadia_FontIO_AtlasGlyphInformation {

  /// The information about the glyph image.
  Arcadia_FontIO_GlyphInformation glyphInformation;

  /// The position, in pixels, of the left edge of the glyph image within the atlas.
  Arcadia_Integer32Value x;

  /// The position, in pixels, of the top edge of the glyph image within the atlas.
  Arcadia_Integer32Value y;

} Arcadia_FontIO_AtlasGlyphInformation;

#endif // ARCADIA_FONTIO_ATLASGLYPHINFORMATION_H_INCLUDED
