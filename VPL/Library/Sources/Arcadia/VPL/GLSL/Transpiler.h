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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_VPL_GLSL_TRANSPILER_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_VPL_GLSL_TRANSPILER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_VPL_Symbols_Program Arcadia_VPL_Symbols_Program;

typedef enum Context{
  Context_VertexShader,
  Context_FragmentShader,
} Context;


// Programs are managed by the program manager.
// Each program has a unique numeric ID.
// Using this ID, backends can determine if they already have a uploaded a variant of that program.
Arcadia_declareObjectType(u8"Arcadia.Visuals.VPL.Backends.GLSL.Transpiler", Arcadia_VPL_Backends_GLSL_Transpiler,
                          u8"Arcadia.Object");

struct Arcadia_VPL_Backends_GLSL_TranspilerDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_Backends_GLSL_Transpiler {
  Arcadia_Object _parent;

  /// @brief The number of constant blocks emitted so far.
  Arcadia_Natural32Value numberOfConstantBlocks;

  /// @brief A mapping from symbols to names.
  /// For example, we map the builtin procedure symbol "texture2D" to the name "texture2D" in the target language.
  /// For example, we map the name "fragmentProgram_inputs_vertex_ambientColor" to some computed name which does not cause conflicts in the target language.
  /// This mapping must be computed before emitting the target language code (obviously).
  Arcadia_Map* symbolNameMapping;
};

Arcadia_VPL_Backends_GLSL_Transpiler*
Arcadia_VPL_Backends_GLSL_Transpiler_create
  (
    Arcadia_Thread* thread
  );

/// @param constantMapping maps "VPL constant record names" / "VPL constant scalar names" to "OpenGL/GLSL uniform block names" and "OpenGL/GLSL uniform names".
void
Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultVertexShader
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Map* constantMapping,
    Arcadia_Map* vertexShaderVariableScalarMapping,
    Arcadia_ByteArrayBuilder* target
  );

/// @param constantMapping maps "VPL constant record names" / "VPL constant scalar names" to "OpenGL/GLSL uniform block names" and "OpenGL/GLSL uniform names".
void
Arcadia_VPL_Backends_GLSL_Transpiler_writeDefaultFragmentShader
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Transpiler* self,
    Arcadia_VPL_Symbols_Program* program,
    Arcadia_Map* constantMapping,
    Arcadia_Map* fragmentShaderVariableScalarMapping,
    Arcadia_String** fragmentColorOutput,
    Arcadia_ByteArrayBuilder* target
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_VPL_GLSL_TRANSPILER_H_INCLUDED
