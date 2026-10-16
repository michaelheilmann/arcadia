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

#include "Arcadia.ADL.Tests.Definitions/Visuals/Model.h"

#include "Arcadia/ADL/Include.h"

static void
makeMaterial
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions
  )
{
  {
    static const char* DDL =
      "{\n"
      "  type : \"Color\",\n"
      "  name : \"MyGame.Colors.MyBlack\",\n"
      "  red : 0,\n"
      "  green : 0,\n"
      "  blue : 0,\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Color\",\n"
      "  name : \"MyGame.Colors.MyWhite\",\n"
      "  red : 255,\n"
      "  green : 255,\n"
      "  blue : 255,\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type  : \"PixelBufferOperations.CheckerboardFillOperation\",\n"
      "  name  : \"MyGame.PixelBufferOperations.MyPixelBufferFillOperation\",\n"
      "  checkerWidth : 32,\n"
      "  checkerHeight : 32,\n"
      "  firstCheckerColor : \"MyGame.Colors.MyWhite\",\n"
      "  secondCheckerColor : \"MyGame.Colors.MyBlack\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type  : \"PixelBuffer\",\n"
      "  name  : \"MyGame.PixelBuffers.MyPixelBuffer\",\n"
      "  width: 256,\n"
      "  height: 256,\n"
      "  operations : [\n"
      "    \"MyGame.PixelBufferOperations.MyPixelBufferFillOperation\",\n"
      "  ],\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type  : \"Texture\",\n"
      "  name  : \"MyGame.Textures.MyTexture\",\n"
      "  pixelBuffer : \"MyGame.PixelBuffers.MyPixelBuffer\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  {
    static const char* DDL =
      "{\n"
      "  type : \"Material\",\n"
      "  name : \"MyGame.Materials.MyMaterial\",\n"
      "  materialType : \"Blinn-Phong\",/* Allowed Values: 'Blinn-Phong'. */\n"
      "  ambientColorSource : \"Texture\",/* Allowed values: 'Mesh', 'Vertex', or 'Texture'. */\n"
      "  ambientColorTexture : \"MyGame.Textures.MyTexture\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
}

static void
makeMesh
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions
  )
{
  {
    static const char *DDL =
      "{\n"
      "  type : \"Mesh\",\n"
      "  name : \"MyGame.Meshes.MyMesh\",\n"
      "  ambientColor : \"MyGame.Colors.MyWhite\",\n"
      "  vertexPositions : [],\n"
      "  vertexAmbientColors : [],\n"
      "  vertexAmbientColorTextureCoordinates: [],\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
}

void
Arcadia_ADL_Tests_Visuals_ModelDefinitionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  makeMaterial(thread, context, definitions);
  makeMesh(thread, context, definitions);
  static const char *DDL =
    "{\n"
    "  type : \"Model\",\n"
    "  name : \"Models.MyModel\",\n"
    "  mesh : \"MyGame.Meshes.MyMesh\",\n"
    "  material : \"MyGame.Materials.MyMaterial\",\n"
    "}\n"
    ;
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  Arcadia_ADL_Definitions_link(thread, definitions);
}

static void
makePlaneMeshGenerator
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions
  )
{
  {
    static const char *DDL =
      "{\n"
      "  type : \"PlaneMeshGenerator\",\n"
      "  name : \"MyGame.Meshes.MyPlane\",\n"
      "  width : 1.0,\n"
      "  height : 1.0,\n"
      "  ambientColor : \"MyGame.Colors.MyWhite\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
}

void
Arcadia_ADL_Tests_Visuals_ModelDefinitionTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  makeMaterial(thread, context, definitions);
  makePlaneMeshGenerator(thread, context, definitions);
  {
    static const char *DDL =
      "{\n"
      "  type : \"Model\",\n"
      "  name : \"Models.MyModel\",\n"
      "  mesh : \"MyGame.Meshes.MyPlane\",\n"
      "  material : \"MyGame.Materials.MyMaterial\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Models.MyModel"));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_ModelDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_Tests_Visuals_ModelDefinitionTest3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    makeMaterial(thread, context, definitions);
    static const char *DDL =
      "{\n"
      "  type : \"Model\",\n"
      "  name : \"Models.MyModel\",\n"
      "  mesh : \"MyGame.Colors.MyWhite\",\n"
      "  material : \"MyGame.Materials.MyMaterial\",\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
    Arcadia_ADL_Definitions_link(thread, definitions);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_ArgumentTypeInvalid) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}
