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

#if !defined(ARCADIA_MILC_AST_MODULENODE_H_INCLUDED)
#define ARCADIA_MILC_AST_MODULENODE_H_INCLUDED

#include "Arcadia/MILC/AST/Node.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_MILC_AST_CompilationUnitNode Arcadia_MILC_AST_CompilationUnitNode;
typedef struct Arcadia_MILC_ModuleSymbol Arcadia_MILC_ModuleSymbol;

/// @code
/// class Arcadia.MILC.AST.ModuleNode extends Arcadia.MILC.AST.Node
/// @endcode
/// @code
/// module : definition*
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.ModuleNode", Arcadia_MILC_AST_ModuleNode,
                          u8"Arcadia.MILC.AST.Node");

struct Arcadia_MILC_AST_ModuleNodeDispatch {
  Arcadia_MILC_AST_NodeDispatch _parent;
};

struct Arcadia_MILC_AST_ModuleNode {
  Arcadia_MILC_AST_Node _parent;

  /// The module symbol of this module.
  Arcadia_MILC_ModuleSymbol* moduleSymbol;

  /// The  path of the module directory or null.
  Arcadia_FilePath* moduleDirectoryPath;

  /// The compilation units (Arcadia.MILC.AST.CompilationUnit) discovered.
  /// These comprise any `.mil` file in the `Source` directory and its subdirectories and the `Module.mil` file.
  Arcadia_List* compilationUnits;
};

Arcadia_MILC_AST_ModuleNode*
Arcadia_MILC_AST_ModuleNode_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_MILC_AST_ModuleNode_appendCompilationUnit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self,
    Arcadia_MILC_AST_CompilationUnitNode* compilationUnit
  );

Arcadia_SizeValue
Arcadia_MILC_AST_ModuleNode_getNumberOfCompilationUnits
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self
  );

Arcadia_MILC_AST_CompilationUnitNode*
Arcadia_MILC_AST_ModuleNode_getCompilationUnitAt
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self,
    Arcadia_SizeValue index
  );

#endif // ARCADIA_MILC_AST_MODULENODE_H_INCLUDED
