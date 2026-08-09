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

#if !defined(ARCADIA_DDL_NODES_BOOLEANNODE_H_INCLUDED)
#define ARCADIA_DDL_NODES_BOOLEANNODE_H_INCLUDED

#if !defined(ARCADIA_DDL_NODES_PRIVATE) || 1 != ARCADIA_DDL_NODES_PRIVATE
  #error("do not include directly, include `Arcadia/DDL/Nodes/Include.h` instead")
#endif
#include "Arcadia/DDL/Nodes/Node.h"

Arcadia_declareObjectType(u8"Arcadia.DDL.BooleanNode", Arcadia_DDL_BooleanNode,
                          u8"Arcadia.DDL.Node");

struct Arcadia_DDL_BooleanNodeDispatch {
  Arcadia_DDL_NodeDispatch _parent;
};

struct Arcadia_DDL_BooleanNode {
  Arcadia_DDL_Node _parent;
  Arcadia_String* value;
};

Arcadia_DDL_BooleanNode*
Arcadia_DDL_BooleanNode_createBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue
  );

// A writer which consumes a boolean node always checks if the string is a valid boolean literal string.
// If a reader produces a boolean node, its string is valid boolean literal string.
Arcadia_DDL_BooleanNode*
Arcadia_DDL_BooleanNode_createString
  (
    Arcadia_Thread* thread,
    Arcadia_String* stringValue
  );

#endif // ARCADIA_DDL_NODES_BOOLEANNODE_H_INCLUDED
