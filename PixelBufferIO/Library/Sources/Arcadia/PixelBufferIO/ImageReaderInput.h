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

#if !defined(ARCADIA_PIXELBUFFERIO_IMAGEREADERINPUT_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_IMAGEREADERINPUT_H_INCLUDED

#include "Arcadia/PixelBufferIO/ImageReaderParameters.h"
#include "Arcadia/Media/Include.h"

// Return a contiguous, independently owned byte array. The caller must root
// the result while any borrowed native pointer into its storage is in use.
Arcadia_ByteArray*
Arcadia_Imaging_ImageReaderInput_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* source
  );

// Checked factory used by PNG and ICO readers. RGBA8, zero padding, top row first.
Arcadia_Media_PixelBuffer*
Arcadia_Imaging_ImageReaderInput_createPixels
  (
    Arcadia_Thread* thread,
    size_t width,
    size_t height,
    uint8_t const* rgbaBytes
  );

#endif // ARCADIA_PIXELBUFFERIO_IMAGEREADERINPUT_H_INCLUDED
