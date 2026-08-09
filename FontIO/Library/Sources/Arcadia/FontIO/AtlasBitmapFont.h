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

#if !defined(ARCADIA_FONTIO_ATLASBITMAPFONT_H_INCLUDED)
#define ARCADIA_FONTIO_ATLASBITMAPFONT_H_INCLUDED

#include "Arcadia/Media/Include.h"
#include "Arcadia/FontIO/GlyphInformation.h"
#include "Arcadia/FontIO/AtlasGlyphInformation.h"
#include "Arcadia/FontIO/BitmapFont.h"

/// @code
/// class AtlasBitmapFont {
///   construct(path:String,pixelSize:Integer32)
/// }
/// @endcode
///
/// A bitmap font which packs the glyph images it renders into a single, growing atlas.
Arcadia_declareObjectType(u8"Arcadia.FontIO.AtlasBitmapFont", Arcadia_FontIO_AtlasBitmapFont,
                          u8"Arcadia.Object");

/// @brief Create an atlas bitmap font.
/// @param thread A pointer to the thread.
/// @param path The path, in UTF-8, of the TrueType/OpenType font file backing the atlas bitmap font.
/// @param pixelSize The size, in pixels, of the glyphs of the atlas bitmap font. Must be positive.
/// @return A pointer to the atlas bitmap font. Its atlas is empty.
/// @error Arcadia_Status_ArgumentValueInvalid @a pixelSize is not positive.
/// @error Arcadia_Status_OperationInvalid The font file could not be loaded.
Arcadia_FontIO_AtlasBitmapFont*
Arcadia_FontIO_AtlasBitmapFont_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_Integer32Value pixelSize
  );

/// @brief Get the information of the glyph of a unicode code point.
/// If the glyph image is not part of the atlas yet, it is rendered and added to the atlas.
/// @param thread A pointer to the thread.
/// @param self A pointer to this atlas bitmap font.
/// @param codePoint The unicode code point.
/// @param target A pointer to a <code>Arcadia_FontIO_AtlasGlyphInformation</code> variable receiving the glyph information.
/// @error Arcadia_Status_ArgumentValueInvalid No glyph exists for @a codePoint.
/// @error Arcadia_Status_OperationInvalid The glyph could not be loaded or added to the atlas.
void
Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self,
    Arcadia_Natural32Value codePoint,
    Arcadia_FontIO_AtlasGlyphInformation* target
  );

/// @brief Get the atlas of this atlas bitmap font.
/// @param thread A pointer to the thread.
/// @param self A pointer to this atlas bitmap font.
/// @return A pointer to the atlas as a pixel buffer of the format RGBA.
/// The glyph coverage is stored in the alpha component of the pixels,
/// the red, green, and blue component values are 255.
/// @warning The returned pixel buffer may be replaced by a larger pixel buffer whenever glyphs are added to the atlas.
Arcadia_Media_PixelBuffer*
Arcadia_FontIO_AtlasBitmapFont_getAtlas
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_AtlasBitmapFont* self
  );

#endif // ARCADIA_FONTIO_ATLASBITMAPFONT_H_INCLUDED
