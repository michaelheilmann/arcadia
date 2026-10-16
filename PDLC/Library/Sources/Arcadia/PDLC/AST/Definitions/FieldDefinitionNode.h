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

#if !defined(ARCADIA_PDLC_AST_VARIABLEDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_VARIABLEDEFINITIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/DefinitionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_PDLC_AST_IdentifierNode Arcadia_PDLC_AST_IdentifierNode;
typedef struct Arcadia_PDLC_AST_IntegerLiteralNode Arcadia_PDLC_AST_IntegerLiteralNode;

#define Arcadia_PDLC_Ownership_Managed (0)
#define Arcadia_PDLC_Ownership_Borrowed (1)
#define Arcadia_PDLC_Ownership_Owned (2)
#define Arcadia_PDLC_Ownership_Retained (3)

/// The definition of a parameter variable, local variable, or member variable.
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.FieldDefinitionNode", Arcadia_PDLC_AST_FieldDefinitionNode,
                          u8"Arcadia.PDLC.AST.DefinitionNode");

struct Arcadia_PDLC_AST_FieldDefinitionNodeDispatch {
  Arcadia_PDLC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_PDLC_AST_FieldDefinitionNode {
  Arcadia_PDLC_AST_DefinitionNode _parent;
  Arcadia_String* name;
  Arcadia_PDLC_AST_IdentifierNode* type;
  Arcadia_BooleanValue isReference;
  Arcadia_BooleanValue isReadOnly;
  Arcadia_BooleanValue isOutput;
  Arcadia_Natural8Value ownership;
  Arcadia_List* recordFields;
  Arcadia_PDLC_AST_IntegerLiteralNode* initializer;
};

Arcadia_PDLC_AST_FieldDefinitionNode*
Arcadia_PDLC_AST_FieldDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* name,
    Arcadia_PDLC_AST_IdentifierNode* type
  );

#endif // ARCADIA_PDLC_AST_VARIABLEDEFINITIONNODE_H_INCLUDED
