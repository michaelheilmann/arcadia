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

#if !defined(ARCADIA_FONTIO_BITMAPFONT_H_INCLUDED)
#define ARCADIA_FONTIO_BITMAPFONT_H_INCLUDED

#include "Arcadia/Media/Include.h"
#include "Arcadia/FontIO/GlyphInformation.h"

/// @code
/// class BitmapFont {
///   construct(path:String,pixelSize:Integer32)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.FontIO.BitmapFont", Arcadia_FontIO_BitmapFont,
                          u8"Arcadia.Object");

/// @brief Create a bitmap font.
/// @param thread A pointer to the thread.
/// @param path The path, in UTF-8, of the TrueType/OpenType font file backing the bitmap font.
/// @param pixelSize The size, in pixels, of the glyphs of the bitmap font. Must be positive.
/// @return A pointer to the bitmap font.
/// @error Arcadia_Status_ArgumentValueInvalid @a pixelSize is not positive.
/// @error Arcadia_Status_OperationInvalid The font file could not be loaded.
Arcadia_FontIO_BitmapFont*
Arcadia_FontIO_BitmapFont_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_Integer32Value pixelSize
  );

/// @brief Get the information of the glyph of a unicode code point.
/// @param thread A pointer to the thread.
/// @param self A pointer to this bitmap font.
/// @param codePoint The unicode code point.
/// @param target A pointer to a <code>Arcadia_FontIO_GlyphInformation</code> variable receiving the glyph information.
/// @error Arcadia_Status_ArgumentValueInvalid No glyph exists for @a codePoint.
/// @error Arcadia_Status_OperationInvalid The glyph could not be loaded.
void
Arcadia_FontIO_BitmapFont_getGlyphInformation
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self,
    Arcadia_Natural32Value codePoint,
    Arcadia_FontIO_GlyphInformation* target
  );

/// @brief Get the bitmap of the glyph of a unicode code point.
/// @param thread A pointer to the thread.
/// @param self A pointer to this bitmap font.
/// @param codePoint The unicode code point.
/// @return A pointer to a pixel buffer of the format RGBA storing the glyph bitmap.
/// The glyph coverage is stored in the alpha component of the pixels,
/// the red, green, and blue component values are 255.
/// @error Arcadia_Status_ArgumentValueInvalid No glyph exists for @a codePoint.
/// @error Arcadia_Status_OperationInvalid The glyph could not be rendered.
Arcadia_Media_PixelBuffer*
Arcadia_FontIO_BitmapFont_getGlyphBitmap
  (
    Arcadia_Thread* thread,
    Arcadia_FontIO_BitmapFont* self,
    Arcadia_Natural32Value codePoint
  );

#endif // ARCADIA_FONTIO_BITMAPFONT_H_INCLUDED
