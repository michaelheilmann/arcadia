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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TYPES_MODULE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TYPES_MODULE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#if defined(ARCADIA_RING1_EXPORT)
  #error("this file is not supposed to be exported")
#endif
#include "Arcadia/Ring1/Implementation/Module.h"
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/TypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/EnumerationTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/InterfaceTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/InternalTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/ObjectTypeNode.module.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/ScalarTypeNode.module.h"

typedef struct TypeNodes TypeNodes;

struct TypeNodes {
  TypeNode** buckets;
  size_t size;
  size_t capacity;
};

Arcadia_DeclareModule("Arcadia.Types", Arcadia_Types);

#endif // ARCADIA_RING1_IMPLEMENTATION_TYPES_MODULE_H_INCLUDED
