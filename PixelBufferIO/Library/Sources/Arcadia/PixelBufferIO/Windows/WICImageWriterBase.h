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

#if !defined(ARCADIA_PIXELBUFFERIO_WINDOWS_WICIMAGEWRITERBASE_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_WINDOWS_WICIMAGEWRITERBASE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/PixelBufferIO/ImageWriter.h"
#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"

// WIC header
#define COBJMACROS (1)
#include <wincodec.h>

Arcadia_declareObjectType(u8"Arcadia.Imaging.Windows.WICImageWriterBase", Arcadia_Imaging_Windows_WICImageWriterBase,
                          u8"Arcadia.Imaging.ImageWriter");

struct Arcadia_Imaging_Windows_WICImageWriterBaseDispatch {
  Arcadia_Imaging_ImageWriterDispatch _parent;

  void (*getFormat)(Arcadia_Thread* thread, Arcadia_Imaging_Windows_WICImageWriterBase* self, Arcadia_Media_PixelBuffer* sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters* parameters, GUID const** guid);
  void (*configure)(Arcadia_Thread* thread, Arcadia_Imaging_Windows_WICImageWriterBase* self, Arcadia_Media_PixelBuffer* sourcePixelBuffer, Arcadia_Imaging_ImageWriterParameters* parameters);
};

struct Arcadia_Imaging_Windows_WICImageWriterBase {
  Arcadia_Imaging_ImageWriter _parent;

  IWICImagingFactory* piFactory;
  HGLOBAL hMemory;
  IStream* piMemoryStream; // NULL if there is no memory stream
  IWICStream* piStream;
  IWICBitmapEncoder* piEncoder;

  IWICBitmapFrameEncode* piBitmapFrame;
  IPropertyBag2* pPropertyBag;
};

void
Arcadia_Imaging_Windows_WICImageWriterBase_doWrite
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  );

void
Arcadia_Imaging_Windows_WICImageWriterBase_getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters,
    GUID const** guid
  );

void
Arcadia_Imaging_Windows_WICImageWriterBase_configure
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_Windows_WICImageWriterBase* self,
    Arcadia_Media_PixelBuffer* sourcePixelBuffer,
    Arcadia_Imaging_ImageWriterParameters* parameters
  );

#endif // ARCADIA_PIXELBUFFERIO_WINDOWS_WICIMAGEWRITERBASE_H_INCLUDED
