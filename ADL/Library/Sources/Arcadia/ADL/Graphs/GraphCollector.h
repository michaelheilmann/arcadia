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

#if !defined(ARCADIA_ADL_GRAPH_COLLECTOR_H_INCLUDED)
#define ARCADIA_ADL_GRAPH_COLLECTOR_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/ADL/Graphs/GraphTemplate.h"

// @brief Collects graph definitions, graph instances, and ordinary nodes from one batch of DDL nodes.
//
// The collector never modifies the definitions container; validation and expansion
// are separate phases so that a batch either exposes no graph-expanded definitions at all
// (if validation or expansion fails) or the complete set (if all phases succeed).
//
// @code
// class Arcadia.ADL.GraphCollector extends Arcadia.Object {
//
//   field templates : Arcadia.Map;
//
//   field templateList : Arcadia.List;
//
//   field instances : Arcadia.List;
//
//   field ordinary : Arcadia.List;
//
//   constructor();
//
//   method addNode(node : Arcadia.DDL.Node);
//
//   method validate();
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.GraphCollector", Arcadia_ADL_GraphCollector,
                          u8"Arcadia.Object");

struct Arcadia_ADL_GraphCollectorDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_GraphCollector {
  Arcadia_Object _parent;
  /** @brief Map from template name (Arcadia.String) to Arcadia_ADL_GraphTemplate. */
  Arcadia_Map* templates;
  /** @brief The templates in input order (Arcadia_ADL_GraphTemplate*). */
  Arcadia_List* templateList;
  /** @brief The GraphInstance nodes in input order (Arcadia_DDL_MapNode*). */
  Arcadia_List* instances;
  /** @brief The ordinary nodes in input order (Arcadia_DDL_MapNode*). */
  Arcadia_List* ordinary;
};

Arcadia_ADL_GraphCollector*
Arcadia_ADL_GraphCollector_create
  (
    Arcadia_Thread* thread
  );

/**
 * @brief Add one root node of the batch to this collector.
 * @param thread A pointer to this thread.
 * @param self A pointer to this graph collector.
 * @param node A pointer to the root DDL node; must be a DDL map node.
 * @error `Arcadia_Status_SemanticalError` the node is not a map, a required field is missing, a name is invalid, or a declaration is duplicated.
 */
void
Arcadia_ADL_GraphCollector_addNode
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self,
    Arcadia_DDL_Node* node
  );

/**
 * @brief Validate every collected graph template.
 *
 * Checks the graph-local structure: unique local names, exactly one Graph.Output,
 * supported parameter kinds, resolved and type-compatible local references, and
 * acyclic local reference edges.
 *
 * @param thread A pointer to this thread.
 * @param self A pointer to this graph collector.
 * @error `Arcadia_Status_SemanticalError` a collected graph template is invalid.
 */
void
Arcadia_ADL_GraphCollector_validate
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* self
  );

#endif // ARCADIA_ADL_GRAPH_COLLECTOR_H_INCLUDED
