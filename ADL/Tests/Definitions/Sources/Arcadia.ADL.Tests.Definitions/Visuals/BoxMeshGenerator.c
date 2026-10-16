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

#include "Arcadia.ADL.Tests.Definitions/Visuals/BoxMeshGenerator.h"

#include "Arcadia/ADL/Include.h"

void
Arcadia_ADL_Tests_Visuals_BoxMeshGeneratorTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
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
    "  type : \"BoxMeshGenerator\",\n"
    "  name : \"MyGame.Meshes.MyBox\",\n"
    "  width : 1.0,\n"
    "  height : 2.0,\n"
    "  depth : 3.0,\n"
    "  ambientColor : \"MyGame.Colors.MyWhite\",\n"
    "}\n"
    ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"MyGame.Meshes.MyBox"));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_BoxMeshGenerator_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_MeshGenerator_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_BoxMeshGenerator* box = (Arcadia_ADL_BoxMeshGenerator*)definition;
  if (1.0 != Arcadia_ADL_BoxMeshGenerator_getWidth(thread, box)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (2.0 != Arcadia_ADL_BoxMeshGenerator_getHeight(thread, box)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (3.0 != Arcadia_ADL_BoxMeshGenerator_getDepth(thread, box)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_ColorDefinition* ambientColor = Arcadia_ADL_MeshGenerator_getAmbientColor(thread, (Arcadia_ADL_MeshGenerator*)box);
  if (NULL == ambientColor) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)ambientColor, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_ADL_Tests_Visuals_BoxMeshGeneratorTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    static const char* DDL =
      "{\n"
      "  type : \"BoxMeshGenerator\",\n"
      "  name : \"MyGame.Meshes.MyBox\",\n"
      "  width : 1.0,\n"
      "  height : 2.0,\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_SemanticalError) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

void
Arcadia_ADL_Tests_Visuals_BoxMeshGeneratorTest3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    static const char* DDL =
      "{\n"
      "  type : \"BoxMeshGenerator\",\n"
      "  name : \"MyGame.Meshes.MyBox\",\n"
      "  width : 1.0,\n"
      "  height : 2.0,\n"
      "  depth : 3.0,\n"
      "  ambientColor : \"MyGame.Meshes.MyBox\",\n"
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
