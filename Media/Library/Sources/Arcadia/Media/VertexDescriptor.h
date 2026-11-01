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

#if !defined(ARCADIA_MEDIA_VERTEXDESCRIPTOR_H_INCLUDED)
#define ARCADIA_MEDIA_VERTEXDESCRIPTOR_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/VertexDescriptor.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_ImmutableList Arcadia_ImmutableList;
typedef struct Arcadia_Media_VertexDescriptor Arcadia_Media_VertexDescriptor;

Arcadia_declareObjectType(u8"Arcadia.Media.VertexDescriptor", Arcadia_Media_VertexDescriptor,
                          u8"Arcadia.Object");

struct Arcadia_Media_VertexDescriptorDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Media_VertexDescriptor {
  Arcadia_Object _parent;
  Arcadia_SizeValue stride;
  Arcadia_ImmutableList* vertexElementDescriptors;
  Arcadia_Natural32Value vertexSize;
};

Arcadia_Natural32Value
Arcadia_Media_VertexDescriptor_getVertexSize
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  );

Arcadia_SizeValue
Arcadia_Media_VertexDescriptor_getStride
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  );

Arcadia_ImmutableList*
Arcadia_Media_VertexDescriptor_getVertexElementDescriptors
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  );

#endif // ARCADIA_MEDIA_VERTEXDESCRIPTOR_H_INCLUDED
