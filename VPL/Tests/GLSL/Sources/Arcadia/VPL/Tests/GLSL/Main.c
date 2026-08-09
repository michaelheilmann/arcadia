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

#include "Arcadia/VPL/Include.h"

#include "Arcadia/FileSystem/Include.h"

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Symbols_Program* program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor);
  Arcadia_VPL_Backends_GLSL_Transpiler* transpiler = Arcadia_VPL_Backends_GLSL_Transpiler_create(thread);
  Arcadia_Map* constantMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* vertexShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* fragmentShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_String* fragmentColorOutput = NULL;

  Arcadia_ByteArrayBuilder* vertexShaderCode = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultVertexShader(thread, transpiler, program, constantMapping, vertexShaderVariableScalarMapping, vertexShaderCode);

  Arcadia_ByteArrayBuilder* fragmentShaderCode = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultFragmentShader(thread, transpiler, program, constantMapping, fragmentShaderVariableScalarMapping , &fragmentColorOutput, fragmentShaderCode);

  Arcadia_FilePath* vertexShaderPath = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FilePath_append(thread, vertexShaderPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"vertex-shader-mesh-ambient-color.txt")));
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), vertexShaderPath, vertexShaderCode);

  Arcadia_FilePath* fragmentShaderPath = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FilePath_append(thread, fragmentShaderPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"fragment-shader-mesh-ambient-color.txt")));
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), fragmentShaderPath, fragmentShaderCode);
}

static void
test2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Symbols_Program* program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_VertexAmbientColor);
  Arcadia_VPL_Backends_GLSL_Transpiler* transpiler = Arcadia_VPL_Backends_GLSL_Transpiler_create(thread);
  Arcadia_Map* constantMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* vertexShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* fragmentShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_String* fragmentColorOutput = NULL;

  Arcadia_ByteArrayBuilder* vertexShaderCode = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultVertexShader(thread, transpiler, program, constantMapping, vertexShaderVariableScalarMapping, vertexShaderCode);

  Arcadia_ByteArrayBuilder* fragmentShaderCode = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultFragmentShader(thread, transpiler, program, constantMapping, fragmentShaderVariableScalarMapping , &fragmentColorOutput, fragmentShaderCode);

  Arcadia_FilePath* vertexShaderPath = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FilePath_append(thread, vertexShaderPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"vertex-shader-vertex-ambient-color.txt")));
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), vertexShaderPath, vertexShaderCode);

  Arcadia_FilePath* fragmentShaderPath = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FilePath_append(thread, fragmentShaderPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"fragment-shader-vertex-ambient-color.txt")));
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), fragmentShaderPath, fragmentShaderCode);
}

static void
test3
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Symbols_Program* program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_TextureAmbientColor);
  Arcadia_VPL_Backends_GLSL_Transpiler* transpiler = Arcadia_VPL_Backends_GLSL_Transpiler_create(thread);
  Arcadia_Map* constantMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* vertexShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Map* fragmentShaderVariableScalarMapping = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_String* fragmentColorOutput = NULL;

  Arcadia_ByteArrayBuilder* vertexShaderCode = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultVertexShader(thread, transpiler, program, constantMapping, vertexShaderVariableScalarMapping, vertexShaderCode);

  Arcadia_ByteArrayBuilder* fragmentShaderCode = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultFragmentShader(thread, transpiler, program, constantMapping, fragmentShaderVariableScalarMapping , &fragmentColorOutput, fragmentShaderCode);

  Arcadia_FilePath* vertexShaderPath = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FilePath_append(thread, vertexShaderPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"vertex-shader-texture-ambient-color.txt")));
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), vertexShaderPath, vertexShaderCode);

  Arcadia_FilePath* fragmentShaderPath = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FilePath_append(thread, fragmentShaderPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"fragment-shader-texture-ambient-color.txt")));
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), fragmentShaderPath, fragmentShaderCode);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&test3)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
