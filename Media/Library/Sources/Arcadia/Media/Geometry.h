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

#if !defined(ARCADIA_MEDIA_GEOMETRY_H_INCLUDED)
#define ARCADIA_MEDIA_GEOMETRY_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Math/Include.h"
#include "Arcadia/Media/VertexBuffer.h"

// Geometry is the procedural evaluation of an `Arcadia.ADL.MeshGenerator`.
//
// A mesh generator definition describes a mesh declaratively: it either carries precomputed vertex data (`Arcadia.ADL.MeshDefinition`)
// or a set of parameters for a procedural shape (`Arcadia.ADL.BoxMeshGenerator`, `Arcadia.ADL.SphereMeshGenerator`,
// `Arcadia.ADL.PlaneMeshGenerator`). A geometry is the result: a vertex buffer holding position, per-vertex ambient color, and UV
// coordinates for unindexed triangles, plus the mesh-wide ambient color of the evaluated mesh generator.
//
// The vertex layout of the vertex buffer is:
//
// - `Arcadia.Media.VertexElementSemantics.PositionXYZ` of `Arcadia.Media.VertexElementSyntactics.Real32Real32Real32` at byte offset 0.
// - `Arcadia.Media.VertexElementSemantics.AmbientRGBA` of `Arcadia.Media.VertexElementSyntactics.Real32Real32Real32Real32` at byte
//   offset `3 * sizeof(Arcadia_Real32Value)`.
// - `Arcadia.Media.VertexElementSemantics.AmbientUV` of `Arcadia.Media.VertexElementSyntactics.Real32Real32` at byte offset
//   `7 * sizeof(Arcadia_Real32Value)`.
//
// The vertex buffer holds unindexed triangles and is to be rendered with `glDrawArrays(GL_TRIANGLES, 0, numberOfVertices)`.
Arcadia_declareObjectType(u8"Arcadia.Media.Geometry", Arcadia_Media_Geometry,
                          u8"Arcadia.Object")

struct Arcadia_Media_GeometryDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Media_Geometry {
  Arcadia_Object _parent;

  /// A pointer to the vertex buffer holding the vertices of this geometry.
  Arcadia_Media_VertexBuffer* vertexBuffer;
  /// A pointer to the ambient color of this geometry.
  Arcadia_Math_Color4Real32* ambientColor;
};

Arcadia_Media_Geometry*
Arcadia_Media_Geometry_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* vertexBuffer,
    Arcadia_Math_Color4Real32* ambientColor
  );

/// @brief Evaluate a mesh generator definition, producing the geometry it describes.
/// @details This links @a meshGenerator if it is not linked yet, hence it is safe to pass an unlinked definition.
/// @param thread A pointer to this thread.
/// @param meshGenerator A pointer to the mesh generator to evaluate.
/// @return A pointer to the geometry.
/// @exception Arcadia.Status.ArgumentTypeInvalid @a meshGenerator is not a supported mesh generator type.
/// @exception Arcadia.Status.NotFound the ambient color of @a meshGenerator does not resolve to a definition.
/// @exception Arcadia.Status.ArgumentTypeInvalid the ambient color of @a meshGenerator does not resolve to an `Arcadia.ADL.ColorDefinition`.
Arcadia_Media_Geometry*
Arcadia_Media_Geometry_createFromMeshGenerator
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* meshGenerator
  );

/// @brief Get the ambient color of this geometry.
/// @param thread A pointer to this thread.
/// @param self A pointer to this geometry.
/// @return A pointer to the ambient color.
Arcadia_Math_Color4Real32*
Arcadia_Media_Geometry_getAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  );

/// @brief Get a vertex buffer holding a copy of the vertices of this geometry.
/// @details The vertices of a geometry are immutable, but the vertex buffer that carries them is not, hence this does not
///          return the vertex buffer of this geometry. It returns a new vertex buffer with a copy of the vertices, and the
///          caller owns that vertex buffer. Two calls therefore return two vertex buffers that can be modified
///          independently of each other and of this geometry.
/// @param thread A pointer to this thread.
/// @param self A pointer to this geometry.
/// @return A pointer to a new vertex buffer holding a copy of the vertices of this geometry.
Arcadia_Media_VertexBuffer*
Arcadia_Media_Geometry_getVertexBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  );

#endif // ARCADIA_MEDIA_GEOMETRY_H_INCLUDED