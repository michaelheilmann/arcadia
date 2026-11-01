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

#if !defined(ARCADIA_ADL_GRAPHS_INCLUDE_H_INCLUDED)
#define ARCADIA_ADL_GRAPHS_INCLUDE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Definitions.h"

#include "Arcadia/ADL/Graphs/GraphTemplate.h"
#include "Arcadia/ADL/Graphs/GraphCollector.h"
#include "Arcadia/ADL/Graphs/GraphExpander.h"

/**
 * @brief Read a batch of DDL root nodes, collecting graph definitions and instances, and expanding them.
 *
 * The batch may contain ordinary definitions, `GraphDefinition` nodes, and `GraphInstance` nodes.
 * The graph templates are validated, the instances expanded, and the resulting ordinary
 * definitions are read with the existing readers into the specified definitions container.
 *
 * Validation and expansion are performed before any definition is published, so a batch either
 * exposes no graph-expanded definitions at all (on failure) or the complete set (on success).
 *
 * @param thread A pointer to this thread.
 * @param context A pointer to the ADL context.
 * @param definitions A pointer to the definitions container to publish into. It must be a fresh container.
 * @param nodes A pointer to the ordered list of root DDL nodes (Arcadia_DDL_Node*).
 */
void
Arcadia_ADL_Graphs_readFromNodes
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_List* nodes
  );

#endif // ARCADIA_ADL_GRAPHS_INCLUDE_H_INCLUDED
