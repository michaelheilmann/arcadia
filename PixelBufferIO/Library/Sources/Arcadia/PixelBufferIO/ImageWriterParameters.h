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

#if !defined(ARCADIA_PIXELBUFFERIO_IMAGEWRITERPARAMETERS_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_IMAGEWRITERPARAMETERS_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// @code
/// class ImageWriterParameters {
///   construct(path:String,format:String)
///   construct(byteBuffer:ByteBuffer,format:String)
/// }
/// @endcode
/// where @ format is one of "bmp", "ico", "png", "tiff".
Arcadia_declareObjectType(u8"Arcadia.Imagging.ImageWriterParameters", Arcadia_Imaging_ImageWriterParameters,
                          u8"Arcadia.Object");

Arcadia_Imaging_ImageWriterParameters*
Arcadia_Imaging_ImageWriterParameters_createFile
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_String* format
  );

Arcadia_Imaging_ImageWriterParameters*
Arcadia_Imaging_ImageWriterParameters_createByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* byteBuffer,
    Arcadia_String* format
  );

Arcadia_BooleanValue
Arcadia_Imaging_ImageWriterParameters_hasPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

Arcadia_String*
Arcadia_Imaging_ImageWriterParameters_getPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

Arcadia_BooleanValue
Arcadia_Imaging_ImageWriterParameters_hasByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

Arcadia_ByteArrayBuilder*
Arcadia_Imaging_ImageWriterParameters_getByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

Arcadia_String*
Arcadia_Imaging_ImageWriterParameters_getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterParameters* self
  );

#endif // ARCADIA_PIXELBUFFERIO_IMAGEWRITERPARAMETERS_H_INCLUDED
