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

#if !defined(ARCADIA_VPL_SYMBOLS_PROGRAM_H_INCLUDED)
#define ARCADIA_VPL_SYMBOLS_PROGRAM_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Symbols/Symbol.h"
#include "Arcadia/VPL/Tree/Include.h"
typedef struct Arcadia_VPL_Symbols_Stage Arcadia_VPL_Symbols_Stage;
typedef struct Arcadia_VPL_Symbols_ConstantRecord Arcadia_VPL_Symbols_ConstantRecord;
typedef struct Arcadia_VPL_Symbols_BuiltinType Arcadia_VPL_Symbols_BuiltinType;
typedef struct Arcadia_VPL_Symbols_Procedure Arcadia_VPL_Symbols_Procedure;

// Mutually exclusive to Arcadia_VPL_Symbols_ProgramFlags_VertexAmbientColor and Arcdia_Visuals_VPL_ProgramFlags_TextureAmbientColor.
#define Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor (1)

// Mutually exclusive to Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor and Arcadia_VPL_Symbols_ProgramFlags_TextureAmbientColor.
#define Arcadia_VPL_Symbols_ProgramFlags_VertexAmbientColor (2)

// Mutually exclusive to Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor and Arcadia_VPL_Symbols_ProgramFlags_VertexAmbientColor.
#define Arcadia_VPL_Symbols_ProgramFlags_TextureAmbientColor (3)

// Programs are managed by the program manager.
// Each program has a unique numeric ID.
// Using this ID, backends can determine if they already have a uploaded a variant of that program.
Arcadia_declareObjectType(u8"Arcadia.VPL.Symbols.Program", Arcadia_VPL_Symbols_Program,
                          u8"Arcadia.VPL.Symbols.Symbol");

struct Arcadia_VPL_Symbols_ProgramDispatch {
  Arcadia_VPL_Symbols_SymbolDispatch _parent;
};

struct Arcadia_VPL_Symbols_Program {
  Arcadia_VPL_Symbols_Symbol _parent;
  /// The flags for this program.
  /// Must be one of Arcadia_VPL_Symbols_ProgramFlags_PerMeshColor or Arcadia_VPL_Symbols_ProgramFlags_PerVertexColor.
  Arcadia_Natural8Value flags;

  /// The vertex stage.
  Arcadia_VPL_Symbols_Stage* vertexStage;
  /// The fragment stage.
  Arcadia_VPL_Symbols_Stage* fragmentStage;

  // `mat4` as used by the VPL.
  // Happens that this currently coincides with `mat4` in GLSL.
  Arcadia_VPL_Symbols_BuiltinType* MAT4;

  // `sampler2D` as used by the VPL.
  // Happens that this currently coincides with `sampler2D` in GLSL.
  Arcadia_VPL_Symbols_BuiltinType* SAMPLER2D;

  // `vec2` as used by the VPL.
  // Happens that this currently coincides with `vec2` in GLSL.
  Arcadia_VPL_Symbols_BuiltinType* VEC2;

  // `vec3` as used by the VPL.
  // Happens that this currently coincides with `vec3` in GLSL.
  Arcadia_VPL_Symbols_BuiltinType* VEC3;

  // `vec4` as used by the VPL.
  // Happens that this currently coincides with `vec4` in GLSL.
  Arcadia_VPL_Symbols_BuiltinType* VEC4;

  // `void` as used by the VPL.
  // Happens that this currently coincides with `void` in GLSL.
  Arcadia_VPL_Symbols_BuiltinType* VOID;

  /// The program scope.
  /// The following symbols are added:
  /// the "mat4", "sampler2D", "vec2", "vec3", "vec4" built-in types
  /// all "constant records" and "constant scalars"
  Arcadia_Languages_Scope* scope;

  // The list of constants (e.g., constant records, constant scalars, etc.) of this program.
  // Note that constants are "program scoped".
  Arcadia_List* constants;

  // The program definition node.
  Arcadia_VPL_Tree_ProgramDefnNode* programDefnNode;
};

Arcadia_VPL_Symbols_Program*
Arcadia_VPL_Symbols_Program_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags
  );

Arcadia_List*
Arcadia_VPL_Symbols_Program_getConstants
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* self
  );

Arcadia_VPL_Symbols_Program*
Arcadia_VPL_Symbols_Program_createProgram
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags
  );

Arcadia_VPL_Tree_ProcedureDefnNode*
Arcadia_VPL_Symbols_Program_createVertexShaderMainProcedureTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* self
  );

Arcadia_VPL_Tree_ProcedureDefnNode*
Arcadia_VPL_Symbols_Program_createFragmentShaderMainProcedureTree
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* self
  );

#endif // ARCADIA_VPL_SYMBOLS_PROGRAM_H_INCLUDED
