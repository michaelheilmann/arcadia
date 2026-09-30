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

#if !defined(ARCADIA_MILC_AST_DEFINITIONS_CONSTRUCTORDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_DEFINITIONS_CONSTRUCTORDEFINITIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/DefinitionNode.h"
#include "Arcadia/Collections/Include.h"

/// @code
/// class Arcadia.MILC.AST.ConstructorDefinitionNode extends Arcadia.MILC.AST.DefinitionNode
/// @endcode
/// See https://michaelheilmann.com/specifications/mil/#constructor-definition for more information.
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.ConstructorDefinitionNode", Arcadia_MILC_AST_ConstructorDefinitionNode,
                          u8"Arcadia.MILC.AST.DefinitionNode");

struct Arcadia_MILC_AST_ConstructorDefinitionNodeDispatch {
  Arcadia_MILC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_MILC_AST_ConstructorDefinitionNode {
  Arcadia_MILC_AST_DefinitionNode _parent;
  Arcadia_String* nativeName;
  Arcadia_List* parameters;
  Arcadia_List* body;
};

Arcadia_MILC_AST_ConstructorDefinitionNode*
Arcadia_MILC_AST_ConstructorDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* nativeName,
    Arcadia_List* parameters,
    Arcadia_List* body
  );

#endif // ARCADIA_MILC_AST_DEFINITIONS_CONSTRUCTORDEFINITIONNODE_H_INCLUDED
