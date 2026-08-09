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

#if !defined(ARCADIA_MILC_AST_DEFINITIONS_CLASSDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_DEFINITIONS_CLASSDEFINITIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/DefinitionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_MILC_AST_IdentifierNode Arcadia_MILC_AST_IdentifierNode;

/// @code
/// class Arcadia.MILC.AST.ClassDefinitionNode extends Arcadia.MILC.AST.DefinitionNode {
///   constructor(className:Arcadia.String, extendedClassName:Arcadia.String, classBody:Arcadia.List)
/// }
/// @endcode
/// See https://mmichaelheilmann.com/specifications/mil/#class-definition for more information.
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.ClassDefinitionNode", Arcadia_MILC_AST_ClassDefinitionNode,
                          u8"Arcadia.MILC.AST.DefinitionNode");

struct Arcadia_MILC_AST_ClassDefinitionNodeDispatch {
  Arcadia_MILC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_MILC_AST_ClassDefinitionNode {
  Arcadia_MILC_AST_DefinitionNode _parent;
  Arcadia_MILC_AST_IdentifierNode* className;
  Arcadia_MILC_AST_IdentifierNode* extendedClassName;
  Arcadia_List* classBody;
};

/// @brief Create a MIL class definition AST node.
/// @return A pointer to this MIL class definition AST node.
Arcadia_MILC_AST_ClassDefinitionNode*
Arcadia_MILC_AST_ClassDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_IdentifierNode* className,
    Arcadia_MILC_AST_IdentifierNode* extendedClassName,
    Arcadia_List* classBody
  );

#endif // ARCADIA_MILC_AST_DEFINITIONS_CLASSDEFINITIONNODE_H_INCLUDED
