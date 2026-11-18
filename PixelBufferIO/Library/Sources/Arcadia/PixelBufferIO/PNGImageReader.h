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

#if !defined(ARCADIA_PIXELBUFFERIO_PNGIMAGEREADER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_PNGIMAGEREADER_H_INCLUDED

#include "Arcadia/PixelBufferIO/ImageReader.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.PNGImageReader", Arcadia_Imaging_PNGImageReader,
                          u8"Arcadia.Imaging.ImageReader");

struct Arcadia_Imaging_PNGImageReaderDispatch {
  Arcadia_Imaging_ImageReaderDispatch parent;
};

struct Arcadia_Imaging_PNGImageReader {
  Arcadia_Imaging_ImageReader _parent;
  Arcadia_ImmutableList* supportedTypes;
};

Arcadia_Imaging_PNGImageReader*
Arcadia_Imaging_PNGImageReader_create
  (
    Arcadia_Thread* thread
  );

// Decode exactly one PNG image from a bounded byte span, with platform backend.
Arcadia_Media_PixelBuffer*
Arcadia_Imaging_PNGImageReader_decode
  (
    Arcadia_Thread* thread,
    uint8_t const* bytes,
    size_t size
  );

#endif // ARCADIA_PIXELBUFFERIO_PNGIMAGEREADER_H_INCLUDED
