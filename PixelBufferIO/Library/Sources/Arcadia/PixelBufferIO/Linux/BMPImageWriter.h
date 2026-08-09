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

#if !defined(ARCADIA_PIXELBUFFERIO_LINUX_BMPIMAGEWRITER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_LINUX_BMPIMAGEWRITER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/PixelBufferIO/ImageWriter.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.Linux.BMPImageWriter", Arcadia_Imaging_Linux_BMPImageWriter,
                          u8"Arcadia.Imaging.ImageWriter");

struct Arcadia_Imaging_Linux_BMPImageWriterDispatch {
  Arcadia_Imaging_ImageWriterDispatch _parent;
};

struct Arcadia_Imaging_Linux_BMPImageWriter {
  Arcadia_Imaging_ImageWriter _parent;
  Arcadia_ImmutableList* supportedTypes;
};

Arcadia_Imaging_Linux_BMPImageWriter*
Arcadia_Imaging_Linux_BMPImageWriter_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PIXELBUFFERIO_LINUX_BMPIMAGEWRITER_H_INCLUDED
