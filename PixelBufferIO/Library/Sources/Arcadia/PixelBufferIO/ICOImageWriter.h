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

#if !defined(ARCADIA_PIXELBUFFERIO_ICOIMAGEWRITER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_ICOIMAGEWRITER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/PixelBufferIO/ImageWriter.h"

// a) "Linux" does not provide an encoder for the "ICO" file format.
// b) "Windows" / the "Windows Imaging Component" ("WIC") does not provide an encoder for the "ICO" file format.
// See https://learn.microsoft.com/en-us/windows/win32/wic/ICO-format-overview?redirectedfrom=MSDN for more information.
//
// This is hand-coded "ICO" file format writer which utilizes any available "png" file format writer.
//
// The "ICO" file format is described on Wikipedia (https://en.wikipedia.org/wiki/ICO_(file_format)).
//
// From the description of the file format:
// - The width and height of any image in an "ICO" files may not exceed 256.
// - all source pixel buffers must be of format PixelBuffer_An8Rn8Gn8Bn8.
Arcadia_declareObjectType(u8"Arcadia.Imaging.ICOImageWriter", Arcadia_Imaging_ICOImageWriter, u8"Arcadia.Imaging.ImageWriter");

struct Arcadia_Imaging_ICOImageWriterDispatch {
  Arcadia_Imaging_ImageWriterDispatch parent;
};

struct Arcadia_Imaging_ICOImageWriter {
  Arcadia_Imaging_ImageWriter _parent;
  Arcadia_ImmutableList* supportedTypes;
};

Arcadia_Imaging_ICOImageWriter*
Arcadia_Imaging_ICOImageWriter_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PIXELBUFFERIO_ICOIMAGEWRITER_H_INCLUDED
