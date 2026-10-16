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

#if !defined(ARCADIA_MEDIA_PIXELBUFFEROPERATIONS_FILL_H_INCLUDED)
#define ARCADIA_MEDIA_PIXELBUFFEROPERATIONS_FILL_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/PixelBufferOperations/Fill.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Media/PixelBufferOperation.h"

typedef struct Arcadia_ADL_PixelBufferOperations_FillOperationDefinition Arcadia_ADL_PixelBufferOperations_FillOperationDefinition;
typedef struct Arcadia_Media_PixelBuffer Arcadia_Media_PixelBuffer;
typedef struct Arcadia_Media_PixelBufferOperations_Fill Arcadia_Media_PixelBufferOperations_Fill;

Arcadia_declareObjectType(u8"Arcadia.Media.PixelBufferOperations.Fill", Arcadia_Media_PixelBufferOperations_Fill,
                          u8"Arcadia.Media.PixelBufferOperation");

struct Arcadia_Media_PixelBufferOperations_FillDispatch {
  Arcadia_Media_PixelBufferOperationDispatch _parent;
};

struct Arcadia_Media_PixelBufferOperations_Fill {
  Arcadia_Media_PixelBufferOperation _parent;
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* definition;
};

void
Arcadia_Media_PixelBufferOperations_Fill_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_Media_PixelBuffer* target
  );

Arcadia_Media_PixelBufferOperations_Fill*
Arcadia_Media_PixelBufferOperations_Fill_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* definition
  );

#endif // ARCADIA_MEDIA_PIXELBUFFEROPERATIONS_FILL_H_INCLUDED
