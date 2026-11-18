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

#if !defined(ARCADIA_PIXELBUFFERIO_ICOIMAGEREADER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_ICOIMAGEREADER_H_INCLUDED

#include "Arcadia/PixelBufferIO/ImageReader.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.ICOImageReader", Arcadia_Imaging_ICOImageReader,
                          u8"Arcadia.Imaging.ImageReader");

struct Arcadia_Imaging_ICOImageReaderDispatch {
  Arcadia_Imaging_ImageReaderDispatch parent;
};

struct Arcadia_Imaging_ICOImageReader {
  Arcadia_Imaging_ImageReader _parent;
  Arcadia_ImmutableList* supportedTypes;
};

Arcadia_Imaging_ICOImageReader*
Arcadia_Imaging_ICOImageReader_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PIXELBUFFERIO_ICOIMAGEREADER_H_INCLUDED
