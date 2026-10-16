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

#if !defined(ARCADIA_PDLC_AST_ENUMERATIONDEFINITIONNODE_H_INCLUDED)
#define ARCADIA_PDLC_AST_ENUMERATIONDEFINITIONNODE_H_INCLUDED

#include "Arcadia/PDLC/AST/DefinitionNode.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_PDLC_AST_IdentifierNode Arcadia_PDLC_AST_IdentifierNode;

/// @code
/// class Arcadia.PDLC.AST.EnumerationDefinitionNode extends Arcadia.PDLC.AST.DefinitionNode {
///   construct(enumerationName:Arcadia.String, enumerationBody:Arcadia.List)
/// }
/// @endcode
/// Represents
/// @code
/// enumerationDefinition : 'enumeration' stringLiteral? identifier enumerationBody
///                       | 'native' stringLiteral? 'enumeration' identifier enumerationBody
/// enumerationBody : '{' enumerationMemberDefinition* '}'
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.AST.EnumerationDefinitionNode", Arcadia_PDLC_AST_EnumerationDefinitionNode,
                          u8"Arcadia.PDLC.AST.DefinitionNode");

struct Arcadia_PDLC_AST_EnumerationDefinitionNodeDispatch {
  Arcadia_PDLC_AST_DefinitionNodeDispatch _parent;
};

struct Arcadia_PDLC_AST_EnumerationDefinitionNode {
  Arcadia_PDLC_AST_DefinitionNode _parent;
  Arcadia_PDLC_AST_IdentifierNode* enumerationName;
  Arcadia_List* enumerationBody;
  Arcadia_BooleanValue isNative;
  // Optional mapped include/output path; ownership is determined by isNative.
  Arcadia_String* nativeHeaderPath;
};

/// @brief Create a MIL enumeration definition AST node.
/// @return A pointer to this MIL enumeration definition AST node.
Arcadia_PDLC_AST_EnumerationDefinitionNode*
Arcadia_PDLC_AST_EnumerationDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_IdentifierNode* enumerationName,
    Arcadia_List* enumerationBody
  );

#endif // ARCADIA_PDLC_AST_ENUMERATIONDEFINITIONNODE_H_INCLUDED
