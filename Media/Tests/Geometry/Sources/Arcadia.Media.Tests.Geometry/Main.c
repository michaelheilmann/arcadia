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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arcadia/Media/Include.h"

// // `Arcadia.ADL.MeshGenerator` itself is abstract in ADL but has a constructor, hence a base mesh generator can be created here.
// A base mesh generator is an `Arcadia.ADL.MeshGenerator` which is none of the supported mesh generator types.
static Arcadia_ADL_MeshGenerator*
createBaseMeshGenerator
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_String* ambientColorName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_MeshGenerator);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)ambientColorName);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_ADL_MeshGenerator);
}
// The layout of the vertex buffer produced by `Arcadia_Media_Geometry_createFromMeshGenerator`.
#define componentsInVertex (9)
#define componentsInPosition (3)
#define componentsInAmbientUV (2)

static void
testFailure
  (
    Arcadia_Thread* thread,
    const char* message
  )
{
  printf("FAIL: %s", message);
  printf("\n");
  fflush(stdout);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  Arcadia_Thread_jump(thread);
}

static void
expectFloat
  (
    Arcadia_Thread* thread,
    const char* message,
    Arcadia_Real32Value actual,
    Arcadia_Real32Value expected
  )
{
  Arcadia_Real32Value d = actual - expected;
  if (d < 0.f) d = -d;
  if (d > 0.0001f) {
    testFailure(thread, message);
  }
}

static void
expectSize
  (
    Arcadia_Thread* thread,
    const char* message,
    Arcadia_SizeValue actual,
    Arcadia_SizeValue expected
  )
{
  if (actual != expected) {
    testFailure(thread, message);
  }
}

static void
expectComponents
  (
    Arcadia_Thread* thread,
    const char* message,
    Arcadia_Math_Color4Real32* actual,
    Arcadia_Real32Value expectedRed,
    Arcadia_Real32Value expectedGreen,
    Arcadia_Real32Value expectedBlue,
    Arcadia_Real32Value expectedAlpha
  )
{
  expectFloat(thread, message, actual->components[0], expectedRed);
  expectFloat(thread, message, actual->components[1], expectedGreen);
  expectFloat(thread, message, actual->components[2], expectedBlue);
  expectFloat(thread, message, actual->components[3], expectedAlpha);
}

static void
readDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    const char* ddl
  )
{
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, ddl), Arcadia_BooleanValue_False);
}

static void
defineCommonDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions
  )
{
  readDefinition(thread, context, definitions,
    "{\n"
    "  type : \"Color\",\n"
    "  name : \"MyGame.Colors.MyWhite\",\n"
    "  red : 255,\n"
    "  green : 255,\n"
    "  blue : 255,\n"
    "}\n");
  readDefinition(thread, context, definitions,
    "{\n"
    "  type : \"Color\",\n"
    "  name : \"MyGame.Colors.MyRed\",\n"
    "  red : 255,\n"
    "  green : 0,\n"
    "  blue : 0,\n"
    "}\n");
}

static Arcadia_ADL_MeshGenerator*
getMeshGenerator
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    const char* name
  )
{
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, name));
  if (NULL == definition) {
    testFailure(thread, "the mesh generator definition was not read properly.");
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_MeshGenerator_getType(thread))) {
    testFailure(thread, "the mesh generator definition is not an `Arcadia.ADL.MeshGenerator`.");
  }
  return (Arcadia_ADL_MeshGenerator*)definition;
}

// A `Arcadia.ADL.PlaneMeshGenerator` with a width of 2 and a height of 4 evaluates to 6 vertices lying in the XZ plane.
static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  readDefinition(thread, context, definitions,
    "{\n"
    "  type : \"PlaneMeshGenerator\",\n"
    "  name : \"MyGame.Meshes.MyPlane\",\n"
    "  width : 2.0,\n"
    "  height : 4.0,\n"
    "  ambientColor : \"MyGame.Colors.MyRed\",\n"
    "}\n");
  Arcadia_ADL_Definitions_link(thread, definitions);
  //
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, getMeshGenerator(thread, definitions, "MyGame.Meshes.MyPlane"));
  //
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  expectSize(thread, "the plane does not have 6 vertices.", Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 6);
  // The mesh-wide ambient color is the red of the referenced `Arcadia.ADL.Color` definition, with an alpha of one.
  expectComponents(thread, "the mesh-wide ambient color of the plane is wrong.", Arcadia_Media_Geometry_getAmbientColor(thread, geometry), 1.f, 0.f, 0.f, 1.f);
  //
  Arcadia_Real32Value const* vertices = (Arcadia_Real32Value const*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer);
  // All vertices of the plane lie in the XZ plane, hence their y coordinate is zero.
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    expectFloat(thread, "a vertex of the plane does not lie in the XZ plane.", vertices[i * componentsInVertex + 1], 0.f);
    // The per-vertex ambient color is white.
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      expectFloat(thread, "the per-vertex ambient color of the plane is not white.", vertices[i * componentsInVertex + componentsInPosition + j], 1.f);
    }
  }
  // The plane spans `width` along x and `height` along z, centered at the origin.
  {
    Arcadia_Real32Value minimumX = vertices[0], maximumX = vertices[0];
    Arcadia_Real32Value minimumZ = vertices[2], maximumZ = vertices[2];
    for (Arcadia_SizeValue i = 0; i < 6; ++i) {
      Arcadia_Real32Value x = vertices[i * componentsInVertex + 0];
      Arcadia_Real32Value z = vertices[i * componentsInVertex + 2];
      if (x < minimumX) minimumX = x;
      if (x > maximumX) maximumX = x;
      if (z < minimumZ) minimumZ = z;
      if (z > maximumZ) maximumZ = z;
    }
    expectFloat(thread, "the plane does not span the given width along x.", maximumX - minimumX, 2.f);
    expectFloat(thread, "the plane does not span the given height along z.", maximumZ - minimumZ, 4.f);
    expectFloat(thread, "the plane is not centered at the origin along x.", (minimumX + maximumX) / 2.f, 0.f);
    expectFloat(thread, "the plane is not centered at the origin along z.", (minimumZ + maximumZ) / 2.f, 0.f);
  }
  // The UV coordinates of the plane cover the unit square.
  {
    Arcadia_Real32Value minimumU = 1.f, maximumU = 0.f, minimumV = 1.f, maximumV = 0.f;
    for (Arcadia_SizeValue i = 0; i < 6; ++i) {
      Arcadia_Real32Value u = vertices[i * componentsInVertex + componentsInPosition + 4 + 0];
      Arcadia_Real32Value v = vertices[i * componentsInVertex + componentsInPosition + 4 + 1];
      if (u < minimumU) minimumU = u;
      if (u > maximumU) maximumU = u;
      if (v < minimumV) minimumV = v;
      if (v > maximumV) maximumV = v;
    }
    expectFloat(thread, "the UV coordinates of the plane do not cover the unit square.", minimumU, 0.f);
    expectFloat(thread, "the UV coordinates of the plane do not cover the unit square.", maximumU, 1.f);
    expectFloat(thread, "the UV coordinates of the plane do not cover the unit square.", minimumV, 0.f);
    expectFloat(thread, "the UV coordinates of the plane do not cover the unit square.", maximumV, 1.f);
  }
}

// A `Arcadia.ADL.BoxMeshGenerator` evaluates to 36 vertices, six per face.
static void
test2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  readDefinition(thread, context, definitions,
    "{\n"
    "  type : \"BoxMeshGenerator\",\n"
    "  name : \"MyGame.Meshes.MyBox\",\n"
    "  width : 2.0,\n"
    "  height : 4.0,\n"
    "  depth : 6.0,\n"
    "  ambientColor : \"MyGame.Colors.MyWhite\",\n"
    "}\n");
  Arcadia_ADL_Definitions_link(thread, definitions);
  //
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, getMeshGenerator(thread, definitions, "MyGame.Meshes.MyBox"));
  //
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  expectSize(thread, "the box does not have 36 vertices.", Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 36);
  expectComponents(thread, "the mesh-wide ambient color of the box is wrong.", Arcadia_Media_Geometry_getAmbientColor(thread, geometry), 1.f, 1.f, 1.f, 1.f);
  //
  Arcadia_Real32Value const* vertices = (Arcadia_Real32Value const*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer);
  // The box spans the given dimensions, centered at the origin.
  {
    Arcadia_Real32Value minimumX = vertices[0], maximumX = vertices[0];
    Arcadia_Real32Value minimumY = vertices[1], maximumY = vertices[1];
    Arcadia_Real32Value minimumZ = vertices[2], maximumZ = vertices[2];
    for (Arcadia_SizeValue i = 0; i < 36; ++i) {
      Arcadia_Real32Value x = vertices[i * componentsInVertex + 0];
      Arcadia_Real32Value y = vertices[i * componentsInVertex + 1];
      Arcadia_Real32Value z = vertices[i * componentsInVertex + 2];
      if (x < minimumX) minimumX = x;
      if (x > maximumX) maximumX = x;
      if (y < minimumY) minimumY = y;
      if (y > maximumY) maximumY = y;
      if (z < minimumZ) minimumZ = z;
      if (z > maximumZ) maximumZ = z;
    }
    expectFloat(thread, "the box does not span the given width along x.", maximumX - minimumX, 2.f);
    expectFloat(thread, "the box does not span the given height along y.", maximumY - minimumY, 4.f);
    expectFloat(thread, "the box does not span the given depth along z.", maximumZ - minimumZ, 6.f);
    expectFloat(thread, "the box is not centered at the origin along x.", (minimumX + maximumX) / 2.f, 0.f);
    expectFloat(thread, "the box is not centered at the origin along y.", (minimumY + maximumY) / 2.f, 0.f);
    expectFloat(thread, "the box is not centered at the origin along z.", (minimumZ + maximumZ) / 2.f, 0.f);
  }
  // Each face contributes six vertices which are two triangles sharing the diagonal between the first and third corner.
  {
    static const Arcadia_SizeValue cornerOfVertex[6] = { 0, 1, 2, 3, 0, 2 };
    for (Arcadia_SizeValue f = 0; f < 6; ++f) {
      Arcadia_SizeValue i = f * 6;
      for (Arcadia_SizeValue t = 0; t < 6; ++t) {
        Arcadia_Real32Value const* expected = vertices + (i + cornerOfVertex[t]) * componentsInVertex;
        Arcadia_Real32Value const* actual = vertices + (i + t) * componentsInVertex;
        for (Arcadia_SizeValue c = 0; c < componentsInVertex; ++c) {
          expectFloat(thread, "a vertex of a box face is repeated incorrectly.", actual[c], expected[c]);
        }
      }
    }
  }
}

// A `Arcadia.ADL.SphereMeshGenerator` evaluates to `segments * (segments / 2) * 6` vertices, all of them on the sphere.
static void
test3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  readDefinition(thread, context, definitions,
    "{\n"
    "  type : \"SphereMeshGenerator\",\n"
    "  name : \"MyGame.Meshes.MySphere\",\n"
    "  radius : 2.0,\n"
    "  segments : 8,\n"
    "  ambientColor : \"MyGame.Colors.MyWhite\",\n"
    "}\n");
  Arcadia_ADL_Definitions_link(thread, definitions);
  //
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, getMeshGenerator(thread, definitions, "MyGame.Meshes.MySphere"));
  //
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  expectSize(thread, "the sphere does not have the expected number of vertices.", Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 8 * 4 * 6);
  expectComponents(thread, "the mesh-wide ambient color of the sphere is wrong.", Arcadia_Media_Geometry_getAmbientColor(thread, geometry), 1.f, 1.f, 1.f, 1.f);
  //
  Arcadia_Real32Value const* vertices = (Arcadia_Real32Value const*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer);
  Arcadia_SizeValue numberOfVertices = Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer);
  for (Arcadia_SizeValue i = 0; i < numberOfVertices; ++i) {
    Arcadia_Real32Value x = vertices[i * componentsInVertex + 0];
    Arcadia_Real32Value y = vertices[i * componentsInVertex + 1];
    Arcadia_Real32Value z = vertices[i * componentsInVertex + 2];
    // Every vertex of the sphere lies on its surface.
    expectFloat(thread, "a vertex of the sphere does not lie on its surface.", Arcadia_sqrtReal32Value(thread, x * x + y * y + z * z), 2.f);
  }
  // The sphere touches both poles of the y axis.
  {
    Arcadia_Real32Value minimumY = vertices[1], maximumY = vertices[1];
    for (Arcadia_SizeValue i = 0; i < numberOfVertices; ++i) {
      Arcadia_Real32Value y = vertices[i * componentsInVertex + 1];
      if (y < minimumY) minimumY = y;
      if (y > maximumY) maximumY = y;
    }
    expectFloat(thread, "the sphere does not reach the positive pole.", maximumY, 2.f);
    expectFloat(thread, "the sphere does not reach the negative pole.", minimumY, -2.f);
  }
}

// An `Arcadia.ADL.MeshDefinition` is evaluated by interleaving its vertex data.
// `Arcadia.ADL.MeshDefinition` has no reader, hence the baked mesh is created through the `Arcadia.ADL` API.
static void
test4
  (
    Arcadia_Thread* thread
  )
{
  static Arcadia_Real32Value const vertexPositions[] = {
     0.f,  0.f, 0.f,
     1.f,  0.f, 0.f,
     0.f,  1.f, 0.f,
  };
  static Arcadia_Real32Value const vertexAmbientColors[] = {
    1.f, 1.f, 1.f, 1.f,
    0.f, 1.f, 0.f, 1.f,
    0.f, 0.f, 1.f, 1.f,
  };
  static Arcadia_Real32Value const vertexAmbientTextureCoordinates[] = {
    0.f, 0.f,
    1.f, 0.f,
    0.f, 1.f,
  };
  //
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  Arcadia_ADL_MeshDefinition* meshDefinition = Arcadia_ADL_MeshDefinition_create
    (
      thread,
      definitions,
      Arcadia_String_createFromCxxString(thread, "MyGame.Meshes.MyBakedMesh"),
      3,
      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)vertexPositions, sizeof(vertexPositions)),
      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)vertexAmbientColors, sizeof(vertexAmbientColors)),
      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)vertexAmbientTextureCoordinates, sizeof(vertexAmbientTextureCoordinates)),
      Arcadia_String_createFromCxxString(thread, "MyGame.Colors.MyRed")
    );
  //
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, (Arcadia_ADL_MeshGenerator*)meshDefinition);
  //
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  expectSize(thread, "the baked mesh does not have 3 vertices.", Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 3);
  expectComponents(thread, "the mesh-wide ambient color of the baked mesh is wrong.", Arcadia_Media_Geometry_getAmbientColor(thread, geometry), 1.f, 0.f, 0.f, 1.f);
  //
  Arcadia_Real32Value const* vertices = (Arcadia_Real32Value const*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer);
  // The positions of the baked mesh are copied as is.
  expectFloat(thread, "the first vertex of the baked mesh is wrong.", vertices[0 * componentsInVertex + 0], 0.f);
  expectFloat(thread, "the second vertex of the baked mesh is wrong.", vertices[1 * componentsInVertex + 0], 1.f);
  expectFloat(thread, "the second vertex of the baked mesh is wrong.", vertices[1 * componentsInVertex + 1], 0.f);
  expectFloat(thread, "the third vertex of the baked mesh is wrong.", vertices[2 * componentsInVertex + 1], 1.f);
  // The per-vertex ambient colors of the baked mesh are copied as is.
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    for (Arcadia_SizeValue j = 0; j < 4; ++j) {
      expectFloat(thread, "a per-vertex ambient color of the baked mesh is wrong.", vertices[i * componentsInVertex + componentsInPosition + j], vertexAmbientColors[i * 4 + j]);
    }
  }
  // The UV coordinates of the baked mesh are copied as is.
  expectFloat(thread, "the UV coordinates of the baked mesh are wrong.", vertices[1 * componentsInVertex + componentsInPosition + 4 + 0], 1.f);
  expectFloat(thread, "the UV coordinates of the baked mesh are wrong.", vertices[2 * componentsInVertex + componentsInPosition + 4 + 1], 1.f);
}

// A mesh generator whose ambient color reference does not resolve is rejected.
static void
test5
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    defineCommonDefinitions(thread, context, definitions);
    readDefinition(thread, context, definitions,
      "{\n"
      "  type : \"PlaneMeshGenerator\",\n"
      "  name : \"MyGame.Meshes.MyPlane\",\n"
      "  width : 2.0,\n"
      "  height : 4.0,\n"
      "  ambientColor : \"MyGame.Colors.DoesNotExist\",\n"
      "}\n");
    Arcadia_ADL_Definitions_link(thread, definitions);
    Arcadia_Media_Geometry_createFromMeshGenerator(thread, getMeshGenerator(thread, definitions, "MyGame.Meshes.MyPlane"));
    //
    Arcadia_Thread_popJumpTarget(thread);
    testFailure(thread, "the evaluation of a mesh generator with an unresolvable ambient color did not raise.");
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_NotFound) {
      testFailure(thread, "the evaluation of a mesh generator with an unresolvable ambient color did not raise `Arcadia.Status.NotFound`.");
    }
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

// An unsupported mesh generator can be constructed, and it is a `Arcadia.ADL.MeshGenerator`.
static void
test6
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  Arcadia_ADL_MeshGenerator* unknownMeshGenerator = createBaseMeshGenerator
    (
      thread,
      definitions,
      Arcadia_String_createFromCxxString(thread, "MyGame.Meshes.MyUnknown"),
      Arcadia_String_createFromCxxString(thread, "MyGame.Colors.MyWhite")
    );
  if (NULL == unknownMeshGenerator) {
    testFailure(thread, "the unsupported mesh generator was not created.");
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)unknownMeshGenerator, _Arcadia_ADL_MeshGenerator_getType(thread))) {
    testFailure(thread, "the unsupported mesh generator is not an `Arcadia.ADL.MeshGenerator`.");
  }
}

// A mesh generator that is none of the supported mesh generator types is rejected.
static void
test7
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  Arcadia_ADL_MeshGenerator* unknownMeshGenerator = createBaseMeshGenerator
    (
      thread,
      definitions,
      Arcadia_String_createFromCxxString(thread, "MyGame.Meshes.MyUnknown"),
      Arcadia_String_createFromCxxString(thread, "MyGame.Colors.MyWhite")
    );
  //
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Media_Geometry_createFromMeshGenerator(thread, unknownMeshGenerator);
    //
    Arcadia_Thread_popJumpTarget(thread);
    testFailure(thread, "the evaluation of an unsupported mesh generator did not raise.");
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_ArgumentTypeInvalid) {
      testFailure(thread, "the evaluation of an unsupported mesh generator did not raise `Arcadia.Status.ArgumentTypeInvalid`.");
    }
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
}

// `Arcadia.Media.Geometry` hands out a copy of its vertex buffer, hence two such copies can be modified independently of each
// other and of the geometry.
static void
test8
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  defineCommonDefinitions(thread, context, definitions);
  readDefinition(thread, context, definitions,
    "{\n"
    "  type : \"PlaneMeshGenerator\",\n"
    "  name : \"MyGame.Meshes.MyPlane\",\n"
    "  width : 2.0,\n"
    "  height : 4.0,\n"
    "  ambientColor : \"MyGame.Colors.MyWhite\",\n"
    "}\n");
  Arcadia_ADL_Definitions_link(thread, definitions);
  //
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, getMeshGenerator(thread, definitions, "MyGame.Meshes.MyPlane"));
  //
  Arcadia_Media_VertexBuffer* vertexBuffer1 = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  Arcadia_Media_VertexBuffer* vertexBuffer2 = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  // Both copies describe the same geometry.
  expectSize(thread, "the first vertex buffer of the plane does not have 6 vertices.", Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer1), 6);
  expectSize(thread, "the second vertex buffer of the plane does not have 6 vertices.", Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer2), 6);
  expectSize(thread, "the vertex descriptor of the first vertex buffer of the plane has an unexpected stride.",
             Arcadia_Media_VertexDescriptor_getStride(thread, Arcadia_Media_VertexBuffer_getVertexDescriptor(thread, vertexBuffer1)),
             componentsInVertex * sizeof(Arcadia_Real32Value));
  // The two copies are neither the same vertex buffer nor do they share their vertex data.
  if (vertexBuffer1 == vertexBuffer2) {
    testFailure(thread, "the geometry returned the same vertex buffer twice.");
  }
  Arcadia_Real32Value* vertices1 = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer1);
  Arcadia_Real32Value* vertices2 = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer2);
  if (vertices1 == vertices2) {
    testFailure(thread, "both vertex buffers of the plane share their vertex data.");
  }
  //
  Arcadia_Real32Value const reference = vertices2[0];
  vertices1[0] = vertices1[0] + 1.f;
  // Changing the first copy changes neither the second copy ...
  expectFloat(thread, "changing the vertex data of the first vertex buffer of the plane changed the vertex data of the second vertex buffer of the plane.",
              vertices2[0], reference);
  // ... nor the geometry.
  Arcadia_Media_VertexBuffer* vertexBuffer3 = Arcadia_Media_Geometry_getVertexBuffer(thread, geometry);
  Arcadia_Real32Value const* vertices3 = (Arcadia_Real32Value const*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer3);
  expectFloat(thread, "changing the vertex data of a vertex buffer of the plane changed the vertex data of the plane.",
              vertices3[0], reference);
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test2)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test3)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test4)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test5)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test6)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test7)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&test8)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}