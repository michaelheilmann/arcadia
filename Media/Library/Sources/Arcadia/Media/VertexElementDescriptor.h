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

#if !defined(ARCADIA_MEDIA_VERTEXELEMENTDESCRIPTOR_H_INCLUDED)
#define ARCADIA_MEDIA_VERTEXELEMENTDESCRIPTOR_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/VertexElementSemantics.h"
#include "Arcadia/Media/VertexElementSyntactics.h"

Arcadia_declareObjectType(u8"Arcadia.Media.VertexElementDescriptor", Arcadia_Media_VertexElementDescriptor,
                          Arcadia_Object);

struct Arcadia_Media_VertexElementDescriptorDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_VertexElementDescriptor {
  Arcadia_Object parent;
  /// The source of the vertex element.
  /// This is the index of a vertex buffer in a vertex buffer binding.
  /// @todo This is currently unused and is initialized to @a 0.
  Arcadia_Natural8Value source;
  /// The offset of the vertex element, in Bytes, from the beginning of the vertex.
  Arcadia_Natural32Value offset;
  /// The semantics of the vertex element.
  Arcadia_Media_VertexElementSemantics semantics;
  /// The syntax of the vertex element.
  Arcadia_Media_VertexElementSyntactics syntactics;
};

Arcadia_Media_VertexElementDescriptor*
Arcadia_Media_VertexElementDescriptor_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value offset,
    Arcadia_Media_VertexElementSemantics semantics,
    Arcadia_Media_VertexElementSyntactics syntactics
  );

/// @brief Get the source of the vertex element.
/// @param thread A pointer to this thread.
/// @param self A pointer to this vertex element descriptor.
/// @return The source of the vertex element.
Arcadia_Natural8Value
Arcadia_Media_VertexElementDescriptor_getSource
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexElementDescriptor* self
  );

/// @brief Get the offset of the vertex element.
/// @param thread A pointer to this thread.
/// @param self A pointer to this vertex element descriptor.
/// @return The offset of the vertex element.
Arcadia_Natural32Value
Arcadia_Media_VertexElementDescriptor_getOffset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexElementDescriptor* self
  );

/// @brief Get the semantics of the vertex element.
/// @param thread A pointer to this thread.
/// @param self A pointer to this vertex element descriptor.
/// @return The semantics of the vertex element.
Arcadia_Media_VertexElementSemantics
Arcadia_Media_VertexElementDescriptor_getSemantics
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexElementDescriptor* self
  );

/// @brief Get the syntactics of the vertex element.
/// @param thread A pointer to this thread.
/// @param self A pointer to this vertex element descriptor.
/// @return The syntactics of the vertex element.
Arcadia_Media_VertexElementSyntactics
Arcadia_Media_VertexElementDescriptor_getSyntactics
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexElementDescriptor* self
  );

/// @brief Get the size, in Bytes, of the vertex element.
/// @param thread A pointer to this thread.
/// @param self A pointer to this vertex element descriptor.
/// @return The size, in Bytes, of the vertex element.
Arcadia_Natural32Value
Arcadia_Media_VertexElementDescriptor_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexElementDescriptor* self
  );

#endif // ARCADIA_MEDIA_VERTEXELEMENTDESCRIPTOR_H_INCLUDED
