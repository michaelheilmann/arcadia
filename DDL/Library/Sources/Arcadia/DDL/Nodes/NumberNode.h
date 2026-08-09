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

#if !defined(ARCADIA_DDL_NODES_NUMBERNODE_H_INCLUDED)
#define ARCADIA_DDL_NODES_NUMBERNODE_H_INCLUDED

#if !defined(ARCADIA_DDL_NODES_PRIVATE) || 1 != ARCADIA_DDL_NODES_PRIVATE
  #error("do not include directly, include `Arcadia/DDL/Nodes/Include.h` instead")
#endif
#include "Arcadia/DDL/Nodes/Node.h"

Arcadia_declareObjectType(u8"Arcadia.DDL.NumberNode", Arcadia_DDL_NumberNode,
                          u8"Arcadia.DDL.Node");

struct Arcadia_DDL_NumberNodeDispatch {
  Arcadia_DDL_NodeDispatch _parent;
};

struct Arcadia_DDL_NumberNode {
  Arcadia_DDL_Node parent;
  Arcadia_String* value;
};

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createReal32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value real32Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createReal64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value real64Value
  );

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createString
  (
    Arcadia_Thread* thread,
    Arcadia_String* stringValue
  );

#endif // ARCADIA_DDL_NODES_NUMBERNODE_H_INCLUDED
