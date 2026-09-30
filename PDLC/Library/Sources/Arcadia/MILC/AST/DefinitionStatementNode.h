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

#if !defined(ARCADIA_MILC_AST_DEFINITIONSTATEMENTNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_DEFINITIONSTATEMENTNODE_H_INCLUDED

#include "Arcadia/MILC/AST/StatementNode.h"

/// @code
/// class Arcadia.MILC.AST.DefinitionStatementNode extends Arcadia.MILC.AST.StatementNode {
///   ...
/// }
/// @endcode
/// represents
/// @code
/// definitionStatement : labelDefinitionStatement
///                     | variableDefinitionStatement
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.DefinitionStatementNode", Arcadia_MILC_AST_DefinitionStatementNode,
                          u8"Arcadia.MILC.AST.StatementNode");

struct Arcadia_MILC_AST_DefinitionStatementNodeDispatch {
  Arcadia_MILC_AST_StatementNodeDispatch _parent;
};

struct Arcadia_MILC_AST_DefinitionStatementNode {
  Arcadia_MILC_AST_StatementNode _parent;
};

#endif // ARCADIA_MILC_AST_DEFINITIONSTATEMENTNODE_H_INCLUDED
