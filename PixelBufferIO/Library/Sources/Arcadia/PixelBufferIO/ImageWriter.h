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

#if !defined(ARCADIA_PIXELBUFFERIO_IMAGEWRITER_H_INCLUDED)
#define ARCADIA_PIXELBUFFERIO_IMAGEWRITER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/PixelBufferIO/ImageWriterParameters.h"

Arcadia_declareObjectType(u8"Arcadia.Imaging.ImageWriter", Arcadia_Imaging_ImageWriter, u8"Arcadia.Object");

struct Arcadia_Imaging_ImageWriterDispatch {
  Arcadia_ObjectDispatch parent;

  Arcadia_ImmutableList* (*getSupportedTypes)(Arcadia_Thread* thread, Arcadia_Imaging_ImageWriter* self);
  void (*write)(Arcadia_Thread* thread, Arcadia_Imaging_ImageWriter* self, Arcadia_List* source, Arcadia_Imaging_ImageWriterParameters* target);
};

struct Arcadia_Imaging_ImageWriter {
  Arcadia_Object _parent;
};

/// @brief A set of strings denoting the extensions supported by this image writer.
/// @param thread A pointer to this thread.
/// @param self A pointer to this image writer.
/// @return A set of strings denoting the extensions supported by this image writer.
/// @remarks Canonical strings are "bmp", "ico", and "png".
Arcadia_ImmutableList*
Arcadia_Imaging_ImageWriter_getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* self
  );

void
Arcadia_Imaging_ImageWriter_write
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  );

#endif // ARCADIA_PIXELBUFFERIO_IMAGEWRITER_H_INCLUDED
