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

#if !defined(ARCADIA_PIXELBUFFERIO_WINDOWS_TIFIMAGEWRITER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_WINDOWS_TIFIMAGEWRITER_H_INCLUDED

#include "Arcadia/PixelBufferIO/Windows/WicImageWriterBase.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.Windows.TIFImageWriter", Arcadia_Imaging_Windows_TIFImageWriter,
                          u8"Arcadia.Imaging.Windows.WICImageWriterBase");

struct Arcadia_Imaging_Windows_TIFImageWriterDispatch {
  Arcadia_Imaging_Windows_WICImageWriterBaseDispatch parent;
};

struct Arcadia_Imaging_Windows_TIFImageWriter {
  Arcadia_Imaging_Windows_WICImageWriterBase _parent;
  Arcadia_ImmutableList* supportedTypes;
};

Arcadia_Imaging_Windows_TIFImageWriter*
Arcadia_Imaging_Windows_TIFImageWriter_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PIXELBUFFERIO_WINDOWS_TIFIMAGEWRITER_H_INCLUDED
