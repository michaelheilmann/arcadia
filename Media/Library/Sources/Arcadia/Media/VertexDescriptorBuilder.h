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

#if !defined(ARCADIA_MEDIA_VERTEXDESCRIPTORBUILDER_H_INCLUDED)
#define ARCADIA_MEDIA_VERTEXDESCRIPTORBUILDER_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Media/VertexElementDescriptor.h"
#include "Arcadia/Media/VertexDescriptor.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Media.VertexDescriptorBuilder", Arcadia_Media_VertexDescriptorBuilder,
                          Arcadia_Object);

struct Arcadia_Media_VertexDescriptorBuilderDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_VertexDescriptorBuilder {
  Arcadia_Object parent;
  /// The flags.
  Arcadia_Natural8Value flags;
  /// The stride, that is, the distance, in Bytes, from the start of one vertex to the start of the next vertex.
  /// The stride is computed from the vertex element descriptors.
  Arcadia_SizeValue stride;
  /// The list of vertex element descriptors.
  Arcadia_List* vertexElementDescriptors;
};

Arcadia_Media_VertexDescriptorBuilder*
Arcadia_Media_VertexDescriptorBuilder_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexDescriptorBuilder_build
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  );

#endif // ARCADIA_MEDIA_VERTEXDESCRIPTORBUILDER_H_INCLUDED
