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

#if !defined(ARCADIA_PDLC_AST_DEFINITIONS_METHODDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_DEFINITIONS_METHODDEFINITIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/DefinitionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_PDLC_AST_IdentifierNode Arcadia_PDLC_AST_IdentifierNode;

/// @code
/// class Arcadia.PDLC.AST.MethodDefinitionNode extends Arcadia.PDLC.AST.DefinitionNode
/// @endcode
/// See https://michaelheilmann.com/specifications/mil/#method-definition for more information.
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.MethodDefinitionNode", Arcadia_PDLC_AST_MethodDefinitionNode,
                          u8"Arcadia.PDLC.AST.DefinitionNode");

struct Arcadia_PDLC_AST_MethodDefinitionNodeDispatch {
  Arcadia_PDLC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_PDLC_AST_MethodDefinitionNode {
  Arcadia_PDLC_AST_DefinitionNode _parent;
  Arcadia_String* nativeName;
  Arcadia_String* name;
  Arcadia_List* parameters;
  Arcadia_PDLC_AST_IdentifierNode* returnType;
  Arcadia_List* body;
  Arcadia_BooleanValue isNonvirtual;
  Arcadia_BooleanValue isAbstract;
};

Arcadia_PDLC_AST_MethodDefinitionNode*
Arcadia_PDLC_AST_MethodDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* nativeName,
    Arcadia_String* name,
    Arcadia_List* parameters,
    Arcadia_PDLC_AST_IdentifierNode* returnType,
    Arcadia_List* body
  );

#endif // ARCADIA_PDLC_AST_DEFINITIONS_METHODDEFINITIONNODE_H_INCLUDED
