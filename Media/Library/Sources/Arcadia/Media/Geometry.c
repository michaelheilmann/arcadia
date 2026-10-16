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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/Geometry.h"

#include "Arcadia/Media/Include.h"

// A vertex consists of a position, a per-vertex ambient color, and UV coordinates.
#define Arcadia_Media_Geometry_componentsInPosition (3)
#define Arcadia_Media_Geometry_componentsInAmbientColor (4)
#define Arcadia_Media_Geometry_componentsInAmbientUV (2)
#define Arcadia_Media_Geometry_componentsInVertex \
  (Arcadia_Media_Geometry_componentsInPosition \
   + Arcadia_Media_Geometry_componentsInAmbientColor \
   + Arcadia_Media_Geometry_componentsInAmbientUV)

// A quad is emitted as two triangles sharing the diagonal between its first and third corner, hence six vertices.
#define Arcadia_Media_Geometry_verticesInQuad (6)

// A box consists of six quads, one per face.
#define Arcadia_Media_Geometry_verticesInBox (6 * Arcadia_Media_Geometry_verticesInQuad)

// A plane consists of one quad.
#define Arcadia_Media_Geometry_verticesInPlane (Arcadia_Media_Geometry_verticesInQuad)

// A sphere consists of one quad per slice per stack.
#define Arcadia_Media_Geometry_verticesInSphereQuad (Arcadia_Media_Geometry_verticesInQuad)

static void
Arcadia_Media_Geometry_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  );

static void
Arcadia_Media_Geometry_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  );

static void
Arcadia_Media_Geometry_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_GeometryDispatch* self
  );

static void
Arcadia_Media_Geometry_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_Geometry_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Media_Geometry_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_Geometry_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_Geometry_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.Geometry", Arcadia_Media_Geometry,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Media_Geometry_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_Geometry);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->vertexBuffer = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Media_VertexBuffer_getType(thread));
  self->ambientColor = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Math_Color4Real32_getType(thread));
  //
  Arcadia_LeaveConstructor(Arcadia_Media_Geometry);
}

static void
Arcadia_Media_Geometry_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_Geometry_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_GeometryDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_Geometry_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  )
{
  if (self->vertexBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexBuffer);
  }
  if (self->ambientColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ambientColor);
  }
}

Arcadia_Media_Geometry*
Arcadia_Media_Geometry_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* vertexBuffer,
    Arcadia_Math_Color4Real32* ambientColor
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_Geometry);
  if (vertexBuffer) Arcadia_ValueStack_pushObjectReferenceValue(thread, vertexBuffer); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (ambientColor) Arcadia_ValueStack_pushObjectReferenceValue(thread, ambientColor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_Geometry);
}

Arcadia_Math_Color4Real32*
Arcadia_Media_Geometry_getAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  )
{ return self->ambientColor; }

Arcadia_Media_VertexBuffer*
Arcadia_Media_Geometry_getVertexBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self
  )
{
  // The vertex buffer of a geometry is mutable, and whoever renders a geometry does not necessarily render the vertices
  // unchanged; a text rendering node, for example, replaces the glyph quads of a run. The geometry therefore keeps its
  // vertex buffer to itself and hands out an independent copy, which the caller owns. The vertex descriptor is shared,
  // because it is not mutated.
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Media_VertexBuffer_create
    (
      thread,
      self->vertexBuffer->vertexDescriptor,
      self->vertexBuffer->numberOfVertices
    );
  Arcadia_Memory_copy
    (
      thread,
      vertexBuffer->vertices,
      self->vertexBuffer->vertices,
      self->vertexBuffer->vertexDescriptor->stride * self->vertexBuffer->numberOfVertices
    );
  return vertexBuffer;
}

typedef enum Arcadia_Media_Geometry_GeneratorKind {
  Arcadia_Media_Geometry_GeneratorKind_Unknown,
  Arcadia_Media_Geometry_GeneratorKind_BakedMesh,
  Arcadia_Media_Geometry_GeneratorKind_Box,
  Arcadia_Media_Geometry_GeneratorKind_Sphere,
  Arcadia_Media_Geometry_GeneratorKind_Plane,
} Arcadia_Media_Geometry_GeneratorKind;

typedef struct Arcadia_Media_Geometry_Vertex {
  Arcadia_Real32Value x;
  Arcadia_Real32Value y;
  Arcadia_Real32Value z;
  Arcadia_Real32Value u;
  Arcadia_Real32Value v;
} Arcadia_Media_Geometry_Vertex;

static Arcadia_Media_Geometry_GeneratorKind
Arcadia_Media_Geometry_getGeneratorKind
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* meshGenerator
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)meshGenerator, _Arcadia_ADL_MeshDefinition_getType(thread))) {
    return Arcadia_Media_Geometry_GeneratorKind_BakedMesh;
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)meshGenerator, _Arcadia_ADL_BoxMeshGenerator_getType(thread))) {
    return Arcadia_Media_Geometry_GeneratorKind_Box;
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)meshGenerator, _Arcadia_ADL_SphereMeshGenerator_getType(thread))) {
    return Arcadia_Media_Geometry_GeneratorKind_Sphere;
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)meshGenerator, _Arcadia_ADL_PlaneMeshGenerator_getType(thread))) {
    return Arcadia_Media_Geometry_GeneratorKind_Plane;
  }
  return Arcadia_Media_Geometry_GeneratorKind_Unknown;
}

static Arcadia_Media_VertexDescriptor*
Arcadia_Media_Geometry_createVertexDescriptor
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_VertexDescriptorBuilder* vertexDescriptorBuilder = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  Arcadia_List_insertBackObjectReferenceValue
    (
      thread,
      vertexDescriptorBuilder->vertexElementDescriptors,
      (Arcadia_ObjectReferenceValue)
      Arcadia_Media_VertexElementDescriptor_create
        (
          thread,
          sizeof(Arcadia_Real32Value) * 0,
          Arcadia_Media_VertexElementSemantics_PositionXYZ,
          Arcadia_Media_VertexElementSyntactics_Real32Real32Real32
        )
    );
  Arcadia_List_insertBackObjectReferenceValue
    (
      thread,
      vertexDescriptorBuilder->vertexElementDescriptors,
      (Arcadia_ObjectReferenceValue)
      Arcadia_Media_VertexElementDescriptor_create
        (
          thread,
          sizeof(Arcadia_Real32Value) * 3,
          Arcadia_Media_VertexElementSemantics_AmbientRGBA,
          Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32
        )
    );
  Arcadia_List_insertBackObjectReferenceValue
    (
      thread,
      vertexDescriptorBuilder->vertexElementDescriptors,
      (Arcadia_ObjectReferenceValue)
      Arcadia_Media_VertexElementDescriptor_create
        (
          thread,
          sizeof(Arcadia_Real32Value) * 7,
          Arcadia_Media_VertexElementSemantics_AmbientUV,
          Arcadia_Media_VertexElementSyntactics_Real32Real32
        )
    );
  return Arcadia_Media_VertexDescriptorBuilder_build(thread, vertexDescriptorBuilder);
}

static void
Arcadia_Media_Geometry_writeVertex
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue index,
    const Arcadia_Media_Geometry_Vertex* vertex
  )
{
  Arcadia_Real32Value* p = vertices + index * Arcadia_Media_Geometry_componentsInVertex;
  p[0] = vertex->x;
  p[1] = vertex->y;
  p[2] = vertex->z;
  // The per-vertex ambient color is white. The mesh-wide ambient color is carried by the geometry itself.
  p[3] = 1.f;
  p[4] = 1.f;
  p[5] = 1.f;
  p[6] = 1.f;
  p[7] = vertex->u;
  p[8] = vertex->v;
}

static void
Arcadia_Media_Geometry_writeQuad
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue index,
    const Arcadia_Media_Geometry_Vertex* corners
  )
{
  Arcadia_Media_Geometry_writeVertex(vertices, index + 0, corners + 0);
  Arcadia_Media_Geometry_writeVertex(vertices, index + 1, corners + 1);
  Arcadia_Media_Geometry_writeVertex(vertices, index + 2, corners + 2);
  Arcadia_Media_Geometry_writeVertex(vertices, index + 3, corners + 3);
  Arcadia_Media_Geometry_writeVertex(vertices, index + 4, corners + 0);
  Arcadia_Media_Geometry_writeVertex(vertices, index + 5, corners + 2);
}

static void
Arcadia_Media_Geometry_fillBakedMesh
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value* vertices,
    Arcadia_ADL_MeshDefinition* meshDefinition
  )
{
  Arcadia_SizeValue numberOfVertices = Arcadia_ADL_MeshDefinition_getNumberOfVertices(thread, meshDefinition);
  Arcadia_Real32Value const* vertexPositions = (Arcadia_Real32Value const*)Arcadia_RuntimeByteArray_getBytes(thread, Arcadia_ADL_MeshDefinition_getVertexPositions(thread, meshDefinition));
  Arcadia_Real32Value const* vertexAmbientColors = (Arcadia_Real32Value const*)Arcadia_RuntimeByteArray_getBytes(thread, Arcadia_ADL_MeshDefinition_getVertexAmbientColors(thread, meshDefinition));
  Arcadia_Real32Value const* vertexAmbientUVs = (Arcadia_Real32Value const*)Arcadia_RuntimeByteArray_getBytes(thread, Arcadia_ADL_MeshDefinition_getVertexAmbientTextureCoordinates(thread, meshDefinition));
  for (Arcadia_SizeValue i = 0; i < numberOfVertices; ++i) {
    Arcadia_Real32Value* p = vertices + i * Arcadia_Media_Geometry_componentsInVertex;
    p[0] = vertexPositions[i * 3 + 0];
    p[1] = vertexPositions[i * 3 + 1];
    p[2] = vertexPositions[i * 3 + 2];
    p[3] = vertexAmbientColors[i * 4 + 0];
    p[4] = vertexAmbientColors[i * 4 + 1];
    p[5] = vertexAmbientColors[i * 4 + 2];
    p[6] = vertexAmbientColors[i * 4 + 3];
    p[7] = vertexAmbientUVs[i * 2 + 0];
    p[8] = vertexAmbientUVs[i * 2 + 1];
  }
}

static void
Arcadia_Media_Geometry_fillBox
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value* vertices,
    Arcadia_ADL_BoxMeshGenerator* boxMeshGenerator
  )
{
  Arcadia_Real32Value hx = Arcadia_ADL_BoxMeshGenerator_getWidth(thread, boxMeshGenerator) / 2.f;
  Arcadia_Real32Value hy = Arcadia_ADL_BoxMeshGenerator_getHeight(thread, boxMeshGenerator) / 2.f;
  Arcadia_Real32Value hz = Arcadia_ADL_BoxMeshGenerator_getDepth(thread, boxMeshGenerator) / 2.f;
  Arcadia_SizeValue index = 0;
  // The corners of each face are ordered counter-clockwise as seen from outside the box.
  {
    const Arcadia_Media_Geometry_Vertex corners[4] = { { hx, -hy, -hz, 0.f, 0.f }, { hx, hy, -hz, 1.f, 0.f }, { hx, hy, hz, 1.f, 1.f }, { hx, -hy, hz, 0.f, 1.f } };
    Arcadia_Media_Geometry_writeQuad(vertices, index, corners); index += Arcadia_Media_Geometry_verticesInQuad;
  }
  {
    const Arcadia_Media_Geometry_Vertex corners[4] = { { -hx, -hy, hz, 0.f, 0.f }, { -hx, hy, hz, 1.f, 0.f }, { -hx, hy, -hz, 1.f, 1.f }, { -hx, -hy, -hz, 0.f, 1.f } };
    Arcadia_Media_Geometry_writeQuad(vertices, index, corners); index += Arcadia_Media_Geometry_verticesInQuad;
  }
  {
    const Arcadia_Media_Geometry_Vertex corners[4] = { { -hx, hy, -hz, 0.f, 0.f }, { -hx, hy, hz, 1.f, 0.f }, { hx, hy, hz, 1.f, 1.f }, { hx, hy, -hz, 0.f, 1.f } };
    Arcadia_Media_Geometry_writeQuad(vertices, index, corners); index += Arcadia_Media_Geometry_verticesInQuad;
  }
  {
    const Arcadia_Media_Geometry_Vertex corners[4] = { { hx, -hy, -hz, 0.f, 0.f }, { hx, -hy, hz, 1.f, 0.f }, { -hx, -hy, hz, 1.f, 1.f }, { -hx, -hy, -hz, 0.f, 1.f } };
    Arcadia_Media_Geometry_writeQuad(vertices, index, corners); index += Arcadia_Media_Geometry_verticesInQuad;
  }
  {
    const Arcadia_Media_Geometry_Vertex corners[4] = { { -hx, -hy, hz, 0.f, 0.f }, { hx, -hy, hz, 1.f, 0.f }, { hx, hy, hz, 1.f, 1.f }, { -hx, hy, hz, 0.f, 1.f } };
    Arcadia_Media_Geometry_writeQuad(vertices, index, corners); index += Arcadia_Media_Geometry_verticesInQuad;
  }
  {
    const Arcadia_Media_Geometry_Vertex corners[4] = { { hx, -hy, -hz, 0.f, 0.f }, { -hx, -hy, -hz, 1.f, 0.f }, { -hx, hy, -hz, 1.f, 1.f }, { hx, hy, -hz, 0.f, 1.f } };
    Arcadia_Media_Geometry_writeQuad(vertices, index, corners); index += Arcadia_Media_Geometry_verticesInQuad;
  }
}

static void
Arcadia_Media_Geometry_fillPlane
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value* vertices,
    Arcadia_ADL_PlaneMeshGenerator* planeMeshGenerator
  )
{
  Arcadia_Real32Value hx = Arcadia_ADL_PlaneMeshGenerator_getWidth(thread, planeMeshGenerator) / 2.f;
  Arcadia_Real32Value hz = Arcadia_ADL_PlaneMeshGenerator_getHeight(thread, planeMeshGenerator) / 2.f;
  // The plane lies in the XZ plane, hence its y coordinate is zero. The corners are ordered so that the front face points towards positive y.
  const Arcadia_Media_Geometry_Vertex corners[4] = { { -hx, 0.f, -hz, 0.f, 0.f }, { -hx, 0.f, hz, 0.f, 1.f }, { hx, 0.f, hz, 1.f, 1.f }, { hx, 0.f, -hz, 1.f, 0.f } };
  Arcadia_Media_Geometry_writeQuad(vertices, 0, corners);
}

static void
Arcadia_Media_Geometry_fillSphere
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value* vertices,
    Arcadia_ADL_SphereMeshGenerator* sphereMeshGenerator
  )
{
  Arcadia_Real32Value radius = Arcadia_ADL_SphereMeshGenerator_getRadius(thread, sphereMeshGenerator);
  Arcadia_SizeValue slices = Arcadia_ADL_SphereMeshGenerator_getSegments(thread, sphereMeshGenerator);
  Arcadia_SizeValue stacks = slices / 2;
  Arcadia_SizeValue index = 0;
  for (Arcadia_SizeValue i = 0; i < stacks; ++i) {
    Arcadia_Real32Value theta0 = (Arcadia_Real32Value)3.14159265358979323846f * (Arcadia_Real32Value)i / (Arcadia_Real32Value)stacks;
    Arcadia_Real32Value theta1 = (Arcadia_Real32Value)3.14159265358979323846f * (Arcadia_Real32Value)(i + 1) / (Arcadia_Real32Value)stacks;
    Arcadia_Real32Value v0 = (Arcadia_Real32Value)i / (Arcadia_Real32Value)stacks;
    Arcadia_Real32Value v1 = (Arcadia_Real32Value)(i + 1) / (Arcadia_Real32Value)stacks;
    for (Arcadia_SizeValue j = 0; j < slices; ++j) {
      Arcadia_Real32Value phi0 = (Arcadia_Real32Value)6.28318530717958647692f * (Arcadia_Real32Value)j / (Arcadia_Real32Value)slices;
      Arcadia_Real32Value phi1 = (Arcadia_Real32Value)6.28318530717958647692f * (Arcadia_Real32Value)(j + 1) / (Arcadia_Real32Value)slices;
      Arcadia_Real32Value u0 = (Arcadia_Real32Value)j / (Arcadia_Real32Value)slices;
      Arcadia_Real32Value u1 = (Arcadia_Real32Value)(j + 1) / (Arcadia_Real32Value)slices;
      Arcadia_Real32Value sinTheta0 = Arcadia_sinReal32Value(thread, theta0);
      Arcadia_Real32Value sinTheta1 = Arcadia_sinReal32Value(thread, theta1);
      // The corners are ordered so that the front face points away from the center of the sphere.
      Arcadia_Media_Geometry_Vertex corners[4];
      corners[0] = (Arcadia_Media_Geometry_Vertex) { radius * sinTheta0 * Arcadia_cosReal32Value(thread, phi0), radius * Arcadia_cosReal32Value(thread, theta0), radius * sinTheta0 * Arcadia_sinReal32Value(thread, phi0), u0, v0 };
      corners[1] = (Arcadia_Media_Geometry_Vertex) { radius * sinTheta0 * Arcadia_cosReal32Value(thread, phi1), radius * Arcadia_cosReal32Value(thread, theta0), radius * sinTheta0 * Arcadia_sinReal32Value(thread, phi1), u1, v0 };
      corners[2] = (Arcadia_Media_Geometry_Vertex) { radius * sinTheta1 * Arcadia_cosReal32Value(thread, phi1), radius * Arcadia_cosReal32Value(thread, theta1), radius * sinTheta1 * Arcadia_sinReal32Value(thread, phi1), u1, v1 };
      corners[3] = (Arcadia_Media_Geometry_Vertex) { radius * sinTheta1 * Arcadia_cosReal32Value(thread, phi0), radius * Arcadia_cosReal32Value(thread, theta1), radius * sinTheta1 * Arcadia_sinReal32Value(thread, phi0), u0, v1 };
      Arcadia_Media_Geometry_writeQuad(vertices, index, corners);
      index += Arcadia_Media_Geometry_verticesInSphereQuad;
    }
  }
}

static void
Arcadia_Media_Geometry_fill
  (
    Arcadia_Thread* thread,
    Arcadia_Media_Geometry* self,
    Arcadia_ADL_MeshGenerator* meshGenerator,
    Arcadia_Media_Geometry_GeneratorKind generatorKind
  )
{
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)self->vertexBuffer->vertices;
  switch (generatorKind) {
    case Arcadia_Media_Geometry_GeneratorKind_BakedMesh: {
      Arcadia_Media_Geometry_fillBakedMesh(thread, vertices, (Arcadia_ADL_MeshDefinition*)meshGenerator);
    } break;
    case Arcadia_Media_Geometry_GeneratorKind_Box: {
      Arcadia_Media_Geometry_fillBox(thread, vertices, (Arcadia_ADL_BoxMeshGenerator*)meshGenerator);
    } break;
    case Arcadia_Media_Geometry_GeneratorKind_Sphere: {
      Arcadia_Media_Geometry_fillSphere(thread, vertices, (Arcadia_ADL_SphereMeshGenerator*)meshGenerator);
    } break;
    case Arcadia_Media_Geometry_GeneratorKind_Plane: {
      Arcadia_Media_Geometry_fillPlane(thread, vertices, (Arcadia_ADL_PlaneMeshGenerator*)meshGenerator);
    } break;
    default: {/*Intentionally empty.*/}
  }
}

static Arcadia_SizeValue
Arcadia_Media_Geometry_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* meshGenerator,
    Arcadia_Media_Geometry_GeneratorKind generatorKind
  )
{
  switch (generatorKind) {
    case Arcadia_Media_Geometry_GeneratorKind_BakedMesh: {
      return Arcadia_ADL_MeshDefinition_getNumberOfVertices(thread, (Arcadia_ADL_MeshDefinition*)meshGenerator);
    }
    case Arcadia_Media_Geometry_GeneratorKind_Box: {
      return Arcadia_Media_Geometry_verticesInBox;
    }
    case Arcadia_Media_Geometry_GeneratorKind_Sphere: {
      Arcadia_SizeValue segments = Arcadia_ADL_SphereMeshGenerator_getSegments(thread, (Arcadia_ADL_SphereMeshGenerator*)meshGenerator);
      return segments * (segments / 2) * Arcadia_Media_Geometry_verticesInSphereQuad;
    }
    case Arcadia_Media_Geometry_GeneratorKind_Plane: {
      return Arcadia_Media_Geometry_verticesInPlane;
    }
    default: {
      return 0;
    }
  }
}

Arcadia_Media_Geometry*
Arcadia_Media_Geometry_createFromMeshGenerator
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* meshGenerator
  )
{
  // Linking is idempotent, hence this is safe even if the caller has already linked the definition.
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)meshGenerator);
  Arcadia_ADL_ColorDefinition* ambientColorDefinition = Arcadia_ADL_MeshGenerator_getAmbientColor(thread, meshGenerator);
  Arcadia_Media_Geometry_GeneratorKind generatorKind = Arcadia_Media_Geometry_getGeneratorKind(thread, meshGenerator);
  if (Arcadia_Media_Geometry_GeneratorKind_Unknown == generatorKind) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_Media_VertexBuffer* vertexBuffer = NULL;
  Arcadia_Math_Color4Real32* ambientColor = NULL;
  //
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    vertexBuffer = Arcadia_Media_VertexBuffer_create(thread, Arcadia_Media_Geometry_createVertexDescriptor(thread),
                                                    Arcadia_Media_Geometry_getNumberOfVertices(thread, meshGenerator, generatorKind));
    Arcadia_Object_lock(thread, (Arcadia_Object*)vertexBuffer);
    ambientColor = Arcadia_Math_Color4Real32_create4(thread, ambientColorDefinition->red / 255.f,
                                                          ambientColorDefinition->green / 255.f,
                                                          ambientColorDefinition->blue / 255.f,
                                                          1.f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)ambientColor);
    Arcadia_Media_Geometry* self = Arcadia_Media_Geometry_create(thread, vertexBuffer, ambientColor);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)ambientColor);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)vertexBuffer);
    Arcadia_Thread_popJumpTarget(thread);
    // The geometry is rooted on the value stack, and no allocation occurs below, hence the vertex data is filled safely.
    Arcadia_Media_Geometry_fill(thread, self, meshGenerator, generatorKind);
    return self;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (ambientColor) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)ambientColor);
    }
    if (vertexBuffer) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)vertexBuffer);
    }
    Arcadia_Thread_jump(thread);
  }
}