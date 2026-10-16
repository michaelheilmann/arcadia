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

#if !defined(ARCADIA_PDLC_AST_COMPILATIONUNITNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_COMPILATIONUNITNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/Node.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_PDLC_AST_DefinitionNode Arcadia_PDLC_AST_DefinitionNode;
typedef struct Arcadia_PDLC_AST_ModuleNode Arcadia_PDLC_AST_ModuleNode;

/// @code
/// class Arcadia.PDLC.AST.CompilationUnitNode extends Arcadia.PDLC.AST.Node
/// @endcode
/// @brief A compilation unit is a single `.mil` file.
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.CompilationUnitNode", Arcadia_PDLC_AST_CompilationUnitNode,
                          u8"Arcadia.PDLC.AST.Node");

struct Arcadia_PDLC_AST_CompilationUnitNodeDispatch {
  Arcadia_PDLC_AST_NodeDispatch _parent;
};

struct Arcadia_PDLC_AST_CompilationUnitNode {
  Arcadia_PDLC_AST_Node _parent;
  /// The module in which this compilation unit is located in (`Arcadia.PDLC.AST.ModuleNode`).
  Arcadia_PDLC_AST_ModuleNode* moduleNode;
  /// The file path of the compilation unit (e.g., `Module.mil` or `Source/MyClass.mil`).
  Arcadia_FilePath* filePath;
  /// The definitions in this compilation unit.
  Arcadia_List* definitions;
};

Arcadia_PDLC_AST_CompilationUnitNode*
Arcadia_PDLC_AST_CompilationUnitNode_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_PDLC_AST_CompilationUnitNode_appendDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_CompilationUnitNode* self,
    Arcadia_PDLC_AST_DefinitionNode* definition
  );

Arcadia_SizeValue
Arcadia_PDLC_AST_CompilationUnitNode_getNumberOfDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_CompilationUnitNode* self
  );

Arcadia_PDLC_AST_DefinitionNode*
Arcadia_PDLC_AST_CompilationUnitNode_getDefinitionAt
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_CompilationUnitNode* self,
    Arcadia_SizeValue index
  );

#endif // ARCADIA_PDLC_AST_COMPILATIONUNITNODE_H_INCLUDED
