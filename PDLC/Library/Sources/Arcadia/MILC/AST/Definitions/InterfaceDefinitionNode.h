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

#if !defined(ARCADIA_MILC_AST_DEFINITIONS_INTERFACEDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_MILC_AST_DEFINITIONS_INTERFACEDEFINITIONNODE_H_INCLUDED

#include "Arcadia/MILC/AST/DefinitionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_MILC_AST_IdentifierNode Arcadia_MILC_AST_IdentifierNode;

/// @code
/// class Arcadia.MILC.AST.InterfaceDefinitionNode extends Arcadia.MILC.AST.DefinitionNode {
///   constructor(interfaceName:Arcadia.String, extendedInterfaceNames:Arcadia.List, operations:Arcadia.List)
/// }
/// @endcode
/// Represents
/// @code
/// interfaceDefinition : 'interface' identifier ('extends' interfaceName (',' interfaceName)*)? interfaceBody
/// interfaceBody : '{' operationDefinition* '}'
/// @endcode
/// A <code>null</code> @c extendedInterfaceNames indicates that the interface type extends no
/// interface type.
Arcadia_declareObjectType(u8"Arcadia.MILC.AST.InterfaceDefinitionNode", Arcadia_MILC_AST_InterfaceDefinitionNode,
                          u8"Arcadia.MILC.AST.DefinitionNode");

struct Arcadia_MILC_AST_InterfaceDefinitionNodeDispatch {
  Arcadia_MILC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_MILC_AST_InterfaceDefinitionNode {
  Arcadia_MILC_AST_DefinitionNode _parent;
  Arcadia_MILC_AST_IdentifierNode* interfaceName;
  /// The identifiers of the extended interface types in order of appearance.
  /// A null pointer if the interface type extends no interface type.
  Arcadia_List* extendedInterfaceNames;
  /// The operation definition nodes of this interface type in order of appearance.
  Arcadia_List* operations;
};

/// @brief Create a MIL interface definition AST node.
/// @return A pointer to this MIL interface definition AST node.
Arcadia_MILC_AST_InterfaceDefinitionNode*
Arcadia_MILC_AST_InterfaceDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_IdentifierNode* interfaceName,
    Arcadia_List* extendedInterfaceNames,
    Arcadia_List* operations
  );

#endif // ARCADIA_MILC_AST_DEFINITIONS_INTERFACEDEFINITIONNODE_H_INCLUDED
