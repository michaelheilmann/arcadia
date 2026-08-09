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

#if !defined(ARCADIA_FONTIO_GLYPHINFORMATION_H_INCLUDED)
#define ARCADIA_FONTIO_GLYPHINFORMATION_H_INCLUDED

#include "Arcadia/Media/Include.h"

/// @brief Information about a glyph.
/// All values are in pixels.
typedef struct Arcadia_FontIO_GlyphInformation {

  /// The width of the glyph image.
  Arcadia_Integer32Value width;

  /// The height of the glyph image.
  Arcadia_Integer32Value height;

  /// The distance in left direction from the pen position to the left edge of the glyph image.
  /// A positive value indicates that the glyph image extends to the right of the pen position,
  /// a negative value indicates that it extends to the left of the pen position.
  Arcadia_Integer32Value bearingX;

  /// The distance in upwards direction from the pen position on the baseline to the top edge of the glyph image.
  /// Usually positive.
  Arcadia_Integer32Value bearingY;

  /// The distance the pen position is advanced in right direction to render the next glyph.
  Arcadia_Integer32Value advanceX;

  /// The distance the pen position is advanced in downwards direction to render the next glyph.
  /// Usually zero for horizontal layouts.
  Arcadia_Integer32Value advanceY;

} Arcadia_FontIO_GlyphInformation;

#endif // ARCADIA_FONTIO_GLYPHINFORMATION_H_INCLUDED
