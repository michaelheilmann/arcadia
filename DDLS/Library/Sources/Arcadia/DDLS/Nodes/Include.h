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

#if !defined(ARCADIA_DDLS_NODES_INCLUDE_H_INCLUDED)
#define ARCADIA_DDLS_NODES_INCLUDE_H_INCLUDED

#pragma push_macro("ARCADIA_DDLS_NODES_PRIVATE")
#define ARCADIA_DDLS_NODES_PRIVATE (1)

#include "Arcadia/DDLS/Nodes/AnyNode.h"
#include "Arcadia/DDLS/Nodes/ChoiceNode.h"
#include "Arcadia/DDLS/Nodes/Node.h"
#include "Arcadia/DDLS/Nodes/ListNode.h"
#include "Arcadia/DDLS/Nodes/MapNode.h"
#include "Arcadia/DDLS/Nodes/ScalarNode.h"
#include "Arcadia/DDLS/Nodes/SchemaNode.h"
#include "Arcadia/DDLS/Nodes/SchemaReferenceNode.h"

#undef ARCADIA_DDLS_NODES_PRIVATE
#pragma pop_macro("ARCADIA_DDLS_NODES_PRIVATE")

#endif // ARCADIA_DDLS_NODES_INCLUDE_H_INCLUDED
