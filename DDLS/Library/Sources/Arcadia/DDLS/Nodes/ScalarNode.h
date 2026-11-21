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

#if !defined(ARCADIA_DDLS_NODES_SCALARNODE_H_INCLUDED)
#define ARCADIA_DDLS_NODES_SCALARNODE_H_INCLUDED

#if !defined(ARCADIA_DDLS_NODES_PRIVATE) || 1 != ARCADIA_DDLS_NODES_PRIVATE
  #error("do not include directly, include `Arcadia/DDLS/Nodes/Include.h` instead")
#endif
#include "Arcadia/DDLS/Nodes/Node.h"
#include "Arcadia/Collections/Include.h"

/// A scalar node.
/// A name of a built-in scalar type.
Arcadia_declareObjectType(u8"Arcadia.DDLS.ScalarNode", Arcadia_DDLS_ScalarNode,
                          u8"Arcadia.DDLS.Node");

struct Arcadia_DDLS_ScalarNodeDispatch {
  Arcadia_DDLS_NodeDispatch parent;
};

struct Arcadia_DDLS_ScalarNode {
  Arcadia_DDLS_Node parent;
  Arcadia_String* name;
  // NULL means unrestricted; a non-NULL list contains the allowed decoded String values.
  Arcadia_List* enumeration;
  // NULL denotes an absent Number bound. Bound text preserves DDL spelling.
  Arcadia_String* minimum;
  Arcadia_String* exclusiveMinimum;
  Arcadia_String* maximum;
  Arcadia_String* exclusiveMaximum;
};

/// @brief Create a scalar type.
/// @param thread This thread.
Arcadia_DDLS_ScalarNode*
Arcadia_DDLS_ScalarNode_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_DDLS_NODES_SCALARNODE_H_INCLUDED
