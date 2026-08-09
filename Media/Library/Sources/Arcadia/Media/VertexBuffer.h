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

#if !defined(ARCADIA_MEDIA_VERTEXBUFFER_H_INCLUDED)
#define ARCADIA_MEDIA_VERTEXBUFFER_H_INCLUDED

#if !defined(ARCADIA_MEDIA_MODULE) || 1 != ARCADIA_MEDIA_MODULE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/VertexDescriptor.h"

// A vertex buffer consists of a vertex descriptor, the number of vertices, and the vertices."mesh node" consists of a
// - a vertex descriptor: Must be of the form [xyz/position, rgba/ambient, uv/ambient]. Describes the data in the vertex buffer.
// - a vertex buffer: The vertex data. Either of the form [xyz/position] or [xyz/position, rgba/ambient].
Arcadia_declareObjectType(u8"Arcadia.Media.VertexBuffer", Arcadia_Media_VertexBuffer,
                          u8"Arcadia.Object")

struct Arcadia_Media_VertexBufferDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_VertexBuffer {
  Arcadia_Object _parent;

  /// A pointer to the vertex descriptor of the vertices of this vertex buffer.
  Arcadia_Media_VertexDescriptor* vertexDescriptor;
  /// The number of vertices in this vertex buffer.
  Arcadia_SizeValue numberOfVertices;
  /// Pointers to the data of the vertices of this vertex buffer.
  void* vertices;
};

/// @brief Create a vertex buffer.
/// @param thread A pointer to this thread.
/// @param vertexDescriptor A pointer to the vertex descriptor.
/// @param numberOfVertices The number of vertices.
/// @return A pointer to the vertex buffer.
Arcadia_Media_VertexBuffer*
Arcadia_Media_VertexBuffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* vertexDescriptor,
    Arcadia_SizeValue numberOfVertices
  );

Arcadia_SizeValue
Arcadia_Media_VertexBuffer_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  );

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexBuffer_getVertexDescriptor  
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  );

#endif // ARCADIA_MEDIA_VERTEXBUFFER_H_INCLUDED
