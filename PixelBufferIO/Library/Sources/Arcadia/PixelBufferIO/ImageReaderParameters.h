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

#if !defined(ARCADIA_PIXELBUFFERIO_IMAGEREADERPARAMETERS_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_IMAGEREADERPARAMETERS_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// @brief An image reader input and explicitly selected format.
/// @remarks An input is either a file path (String) or an Arcadia.ByteArray.
///          The byte array is retained as a managed reference by the parameters.
Arcadia_declareObjectType(u8"Arcadia.Imaging.ImageReaderParameters", Arcadia_Imaging_ImageReaderParameters,
                          u8"Arcadia.Object");

Arcadia_Imaging_ImageReaderParameters*
Arcadia_Imaging_ImageReaderParameters_createFile
  (
    Arcadia_Thread* thread,
    Arcadia_String* path,
    Arcadia_String* format
  );

Arcadia_Imaging_ImageReaderParameters*
Arcadia_Imaging_ImageReaderParameters_createByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* bytes,
    Arcadia_String* format
  );

Arcadia_BooleanValue
Arcadia_Imaging_ImageReaderParameters_hasPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  );

Arcadia_String*
Arcadia_Imaging_ImageReaderParameters_getPath
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  );

Arcadia_BooleanValue
Arcadia_Imaging_ImageReaderParameters_hasByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  );

Arcadia_ByteArray*
Arcadia_Imaging_ImageReaderParameters_getByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  );

Arcadia_String*
Arcadia_Imaging_ImageReaderParameters_getFormat
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderParameters* self
  );

#endif // ARCADIA_PIXELBUFFERIO_IMAGEREADERPARAMETERS_H_INCLUDED
