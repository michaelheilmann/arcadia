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

#if !defined(ARCADIA_ADL_GRAPH_EXPANDER_H_INCLUDED)
#define ARCADIA_ADL_GRAPH_EXPANDER_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/ADL/Definitions.h"
#include "Arcadia/ADL/Graphs/GraphCollector.h"

// @brief Expands the graph instances collected by a graph collector into ordinary ADL definitions.
//
// Expansion is a pure function of the collected templates, instance names, bindings,
// and the supported field descriptor table. It never mutates the collected templates.
//
// @code
// class Arcadia.ADL.GraphExpander extends Arcadia.Object {
//
//   field collector : Arcadia.ADL.GraphCollector;
//
//   constructor(collector : Arcadia.ADL.GraphCollector);
//
//   method expandInto(definitions : Arcadia.ADL.Definitions);
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.GraphExpander", Arcadia_ADL_GraphExpander,
                          u8"Arcadia.Object");

struct Arcadia_ADL_GraphExpanderDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_GraphExpander {
  Arcadia_Object _parent;
  /** @brief The collector providing the templates and instances. */
  Arcadia_ADL_GraphCollector* collector;
};

Arcadia_ADL_GraphExpander*
Arcadia_ADL_GraphExpander_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphCollector* collector
  );

/**
 * @brief Expand every graph instance into the specified definitions container.
 *
 * The caller must have validated the collector first. Each instance produces the
 * generated internal definitions plus the published output definition. Binding
 * validation and expansion are performed before any definition is published, so
 * that a failure exposes no partial expansion.
 *
 * @param thread A pointer to this thread.
 * @param self A pointer to this graph expander.
 * @param definitions A pointer to the definitions container to publish into.
 * @error `Arcadia_Status_SemanticalError` a binding is invalid.
 */
void
Arcadia_ADL_GraphExpander_expandInto
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphExpander* self,
    Arcadia_ADL_Definitions* definitions
  );

#endif // ARCADIA_ADL_GRAPH_EXPANDER_H_INCLUDED
