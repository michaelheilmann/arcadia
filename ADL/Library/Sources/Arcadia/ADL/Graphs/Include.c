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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Graphs/Include.h"

void
Arcadia_ADL_Graphs_readFromNodes
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_List* nodes
  )
{
  Arcadia_ADL_GraphCollector* collector = Arcadia_ADL_GraphCollector_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)nodes); i < n; ++i) {
    Arcadia_Value element = Arcadia_List_getAt(thread, nodes, i);
    Arcadia_DDL_Node* node = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValue(&element);
    Arcadia_ADL_GraphCollector_addNode(thread, collector, node);
  }
  Arcadia_ADL_GraphCollector_validate(thread, collector);

  // Ordinary nodes are read first so that instance bindings can refer to them.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)collector->ordinary); i < n; ++i) {
    Arcadia_DDL_MapNode* node =
      (Arcadia_DDL_MapNode*)
      Arcadia_List_getObjectReferenceValueCheckedAt(thread, collector->ordinary, i, _Arcadia_DDL_MapNode_getType(thread));
    Arcadia_ADL_Context_readFromNode(thread, context, definitions, (Arcadia_DDL_Node*)node, Arcadia_BooleanValue_False);
  }

  Arcadia_ADL_GraphExpander* expander = Arcadia_ADL_GraphExpander_create(thread, collector);
  Arcadia_ADL_GraphExpander_expandInto(thread, expander, definitions);
}
