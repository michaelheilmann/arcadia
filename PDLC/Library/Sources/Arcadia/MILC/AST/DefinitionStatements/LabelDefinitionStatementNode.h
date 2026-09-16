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

#if !defined(ARCADIA_MILC_AST_LABELDEFINITIONSTATEMENTNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_LABELDEFINITIONSTATEMENTNODE_H_INCLUDED

#include "Arcadia/MILC/AST/DefinitionStatementNode.h"

/// @code
/// class Arcadia.MILC.AST.LabelDefintionStatementNode extends Arcadia.MILC.AST.DefinitionStatement {
///   ...
/// }
/// @endcode
/// Represents
/// @code
/// name ':'
/// @endcode
///
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.LabelDefinitionStatementNode", Arcadia_MILC_AST_LabelDefinitionStatementNode,
                          u8"Arcadia.MILC.AST.DefinitionStatementNode");

struct Arcadia_MILC_AST_LabelDefinitionStatementNodeDispatch {
  Arcadia_MILC_AST_DefinitionStatementNodeDispatch parent;
};

struct Arcadia_MILC_AST_LabelDefinitionStatementNode {
  Arcadia_MILC_AST_DefinitionStatementNode parent;
  Arcadia_String* labelName;
};

/// @brief Create a MIL label instruction AST node.
/// @return A pointer to this MIL label instruction AST node.
Arcadia_MILC_AST_LabelDefinitionStatementNode*
Arcadia_MILC_AST_LabelDefinitionStatementNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* labelName
  );

#endif // ARCADIA_MILC_AST_LABELDEFINITIONSTATEMENTNODE_H_INCLUDED
