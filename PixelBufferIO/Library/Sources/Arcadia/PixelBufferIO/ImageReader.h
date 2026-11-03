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

#if !defined(ARCADIA_PIXELBUFFERIO_IMAGEREADER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_IMAGEREADER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/PixelBufferIO/ImageReaderParameters.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.ImageReader", Arcadia_Imaging_ImageReader,
                          u8"Arcadia.Object");

struct Arcadia_Imaging_ImageReaderDispatch {
  Arcadia_ObjectDispatch parent;

  Arcadia_ImmutableList* (*getSupportedTypes)(Arcadia_Thread* thread, Arcadia_Imaging_ImageReader* self);
  // The caller owns the returned managed list, containing one or more PixelBuffer objects.
  Arcadia_List* (*read)(Arcadia_Thread* thread, Arcadia_Imaging_ImageReader* self,
                        Arcadia_Imaging_ImageReaderParameters* source);
};

struct Arcadia_Imaging_ImageReader {
  Arcadia_Object _parent;
};

/// @brief Get the file extensions supported by this reader (currently "png" or "ico").
Arcadia_ImmutableList*
Arcadia_Imaging_ImageReader_getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReader* self
  );

/// @brief Decode images from a byte array or file.
/// @return A managed list of PixelBuffer objects, in source order.
/// @remarks The returned images use RGBA8 (straight alpha), top-to-bottom rows,
///          and zero padding. PNG returns one image; ICO can return multiple images.
Arcadia_List*
Arcadia_Imaging_ImageReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReader* self,
    Arcadia_Imaging_ImageReaderParameters* source
  );

#endif // ARCADIA_PIXELBUFFERIO_IMAGEREADER_H_INCLUDED
