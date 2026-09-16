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

#if !defined(ARCADIA_MILC_AST_VARIABLEDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_VARIABLEDEFINITIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/DefinitionNode.h"
typedef struct Arcadia_MILC_AST_IdentifierNode Arcadia_MILC_AST_IdentifierNode;

/// The definition of a parameter variable, local variable, or member variable.
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.FieldDefinitionNode", Arcadia_MILC_AST_FieldDefinitionNode,
                          u8"Arcadia.MILC.AST.DefinitionNode");

struct Arcadia_MILC_AST_FieldDefinitionNodeDispatch {
  Arcadia_MILC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_MILC_AST_FieldDefinitionNode {
  Arcadia_MILC_AST_DefinitionNode _parent;
  Arcadia_String* name;
  Arcadia_MILC_AST_IdentifierNode* type;
};

Arcadia_MILC_AST_FieldDefinitionNode*
Arcadia_MILC_AST_FieldDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* name,
    Arcadia_MILC_AST_IdentifierNode* type
  );

#endif // ARCADIA_MILC_AST_VARIABLEDEFINITIONNODE_H_INCLUDED
