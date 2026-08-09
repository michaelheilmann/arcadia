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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPENODE_MODULE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPENODE_MODULE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#if defined(ARCADIA_RING1_EXPORT)
  #error("this file is not supposed to be exported")
#endif
#include "Arcadia/Ring1/Implementation/TypeSystem/TypeKind.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/Names.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/Types.h"

#define TypeNodeName u8"Arcadia.DynamicTypeSystem.TypeNode"
typedef struct TypeNode TypeNode;

struct TypeNode {
  /// Pointer to the successor in the bucket list or the null pointer.
  TypeNode* next;
  /// The kind of this type.
  Arcadia_TypeKind kind;
  /// The name of this type.
  Arcadia_Name* name;


  Arcadia_Type_Operations const* typeOperations;
  Arcadia_Type_TypeDestructingCallbackFunction* typeDestructing;

};

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPESYSTEM_TYPENODE_MODULE_H_INCLUDED
