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

#if !defined(ARCADIA_MEDIA_VERTEXDESCRIPTOR_MODULE_H_INCLUDED)
#define ARCADIA_MEDIA_VERTEXDESCRIPTOR_MODULE_H_INCLUDED

#if !defined(ARCADIA_MEDIA_MODULE) || 1 != ARCADIA_MEDIA_MODULE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#if defined(ARCADIA_MEDIA_EXPORT) && 1 == ARCADIA_MEDIA_EXPORT
  #error("module internal contents exported")
#endif
#include "Arcadia/Media/VertexDescriptor.h"

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexDescriptor_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue stride,
    Arcadia_ImmutableList* vertexElementDescriptors
  );

#endif // ARCADIA_MEDIA_VERTEXDESCRIPTOR_MODULE_H_INCLUDED
