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

#if !defined(ARCADIA_DDL_NODES_INCLUDE_H_INCLUDED)
#define ARCADIA_DDL_NODES_INCLUDE_H_INCLUDED

#pragma push_macro("ARCADIA_DDL_NODES_PRIVATE")
#define ARCADIA_DDL_NODES_PRIVATE (1)
#include "Arcadia/DDL/Nodes/BooleanNode.h"
#include "Arcadia/DDL/Nodes/ListNode.h"
#include "Arcadia/DDL/Nodes/MapEntryNode.h"
#include "Arcadia/DDL/Nodes/MapNode.h"
#include "Arcadia/DDL/Nodes/NameNode.h"
#include "Arcadia/DDL/Nodes/Node.h"
#include "Arcadia/DDL/Nodes/NodeType.h"
#include "Arcadia/DDL/Nodes/NumberNode.h"
#include "Arcadia/DDL/Nodes/StringNode.h"
#include "Arcadia/DDL/Nodes/VoidNode.h"
#undef ARCADIA_DDL_NODES_PRIVATE
#pragma pop_macro("ARCADIA_DDL_NODES_PRIVATE")

#endif // ARCADIA_DDL_NODES_INCLUDE_H_INCLUDED
