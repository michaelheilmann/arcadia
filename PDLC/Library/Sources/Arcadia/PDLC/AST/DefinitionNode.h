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

#if !defined(ARCADIA_PDLC_AST_DEFINITIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_DEFINITIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/Node.h"
typedef struct Arcadia_PDLC_AST_CompilationUnitNode Arcadia_PDLC_AST_CompilationUnitNode;

/// @code
/// class Arcadia.PDLC.AST.DefinitionNode extends Arcadia.PDLC.AST.Node
/// @endcode
/// @code
/// module : definition*
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.DefinitionNode", Arcadia_PDLC_AST_DefinitionNode,
                          u8"Arcadia.PDLC.AST.Node");

struct Arcadia_PDLC_AST_DefinitionNodeDispatch {
  Arcadia_PDLC_AST_NodeDispatch _parent;
};

struct Arcadia_PDLC_AST_DefinitionNode {
  Arcadia_PDLC_AST_Node _parent;
  /// @brief The start offset.
  Arcadia_SizeValue startOffset;
  /// @brief The compilation unit in which this definition is located in.
  Arcadia_PDLC_AST_CompilationUnitNode* compilationUnitNode;
};

#endif // ARCADIA_PDLC_AST_DEFINITIONNODE_H_INCLUDED
