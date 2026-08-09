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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_OBJECTTYPENODE_MODULE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_OBJECTTYPENODE_MODULE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#if defined(ARCADIA_RING1_EXPORT)
  #error("this file is not supposed to be exported")
#endif
#include "Arcadia/Ring1/Implementation/TypeSystem/TypeNode.module.h"

#define ObjectTypeNodeName u8"Arcadia.DynamicTypeSystem.ObjectTypeNode"
typedef struct ObjectTypeNode ObjectTypeNode;

struct ObjectTypeNode {
  TypeNode _parent;

  TypeNode* parentObjectType;
  /// The size, in Bytes, of a value of this type.
  Arcadia_SizeValue valueSize;


  /// The pointer to the dispatch or null.
  Arcadia_ObjectDispatch* dispatch;

  /// The size, in Bytes, of the dispatch.
  Arcadia_SizeValue dispatchSize;

  /// A pointer to the dispatch initializer or null.
  Arcadia_ObjectDispatch_InitializeCallbackFunction* initializeDispatch;

};

/// @brief Allocate an "object" type node with all members set to defaults.
ObjectTypeNode*
ObjectTypeNode_allocate
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_OBJECTTYPENODE_MODULE_H_INCLUDED
