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
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/VertexElementDescriptor.h"
#include "Arcadia/Collections/Include.h"

/// @brief Immutable descriptor of a vertex.
Arcadia_declareObjectType(u8"Arcadia.Media.VertexDescriptor", Arcadia_Media_VertexDescriptor,
                          Arcadia_Object);

struct Arcadia_Media_VertexDescriptorDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_VertexDescriptor {
  Arcadia_Object parent;
  /// The stride, that is, the distance, in Bytes, from the start of one vertex to the start of the next vertex.
  /// @todo Store padding, which is the number of Bytes between vertices, instead of the stride.
  Arcadia_SizeValue stride;
  /// The list of vertex element descriptors.
  Arcadia_ImmutableList* vertexElementDescriptors;
  /// The size, in Bytes, of the vertex.
  Arcadia_Natural32Value vertexSize;
};

Arcadia_Natural32Value
Arcadia_Media_VertexDescriptor_getVertexSize
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  );

#endif // ARCADIA_MEDIA_VERTEXDESCRIPTOR_H_INCLUDED
