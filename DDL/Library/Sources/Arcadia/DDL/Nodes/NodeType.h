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

#if !defined(ARCADIA_DDL_NODES_NODETYPE_H_INCLUDED)
#define ARCADIA_DDL_NODES_NODETYPE_H_INCLUDED

#if !defined(ARCADIA_DDL_NODES_PRIVATE) || 1 != ARCADIA_DDL_NODES_PRIVATE
  #error("do not include directly, include `Arcadia/DDL/Nodes/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType("Arcadia.DDL.NodeType", Arcadia_DDL_NodeType);

enum Arcadia_DDL_NodeType {

#define Define(Name, Description) \
  Arcadia_DDL_NodeType_##Name,

  #include "Arcadia/DDL/Nodes/NodeType.i"

#undef Define

};

#endif // ARCADIA_DDL_NODES_NODETYPE_H_INCLUDED
