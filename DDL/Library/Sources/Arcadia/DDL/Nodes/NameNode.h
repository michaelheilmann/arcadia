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

#if !defined(ARCADIA_DDL_NODES_NAMENODE_H_INCLUDED)
#define ARCADIA_DDL_NODES_NAMENODE_H_INCLUDED

#if !defined(ARCADIA_DDL_NODES_PRIVATE) || 1 != ARCADIA_DDL_NODES_PRIVATE
  #error("do not include directly, include `Arcadia/DDL/Nodes/Include.h` instead")
#endif
#include "Arcadia/DDL/Nodes/Node.h"

Arcadia_declareObjectType(u8"Arcadia.DDL.NameNode", Arcadia_DDL_NameNode,
                          u8"Arcadia.DDL.Node");

struct Arcadia_DDL_NameNodeDispatch {
  Arcadia_DDL_NodeDispatch _parent;
};

struct Arcadia_DDL_NameNode {
  Arcadia_DDL_Node parent;
  Arcadia_String* value;
};

Arcadia_DDL_NameNode*
Arcadia_DDL_NameNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  );

#endif // ARCADIA_DDL_NODES_NAMENODE_H_INCLUDED
