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

#if !defined(ARCADIA_ADL_GRAPH_TEMPLATE_H_INCLUDED)
#define ARCADIA_ADL_GRAPH_TEMPLATE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/ADL/Graphs/GraphSymbol.module.h"

// @brief A graph template collected from a `GraphDefinition` node.
//
// The symbol table is built once during validation and reused during expansion.
//
// @code
// class Arcadia.ADL.GraphTemplate extends Arcadia.Object {
//
//   field name : Arcadia.String;
//
//   field nodes : Arcadia.List;
//
//   field symbols : Arcadia.List;
//
//   field symbolTable : Arcadia.Map;
//
//   field outputSymbol : Arcadia.ADL.GraphSymbol;
//
//   constructor(name : Arcadia.String, nodes : Arcadia.List);
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.GraphTemplate", Arcadia_ADL_GraphTemplate,
                          u8"Arcadia.Object");

struct Arcadia_ADL_GraphTemplateDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_GraphTemplate {
  Arcadia_Object _parent;
  /** @brief The globally unique template name. */
  Arcadia_String* name;
  /** @brief The ordered list of graph-local DDL map nodes (Arcadia_DDL_MapNode*). */
  Arcadia_List* nodes;
  /** @brief The graph-local symbols in node order (Arcadia_ADL_GraphSymbol*); set during validation. */
  Arcadia_List* symbols;
  /** @brief Map from local name (Arcadia.String) to Arcadia_ADL_GraphSymbol; set during validation. */
  Arcadia_Map* symbolTable;
  /** @brief Map from parameter local name (Arcadia.String) to Arcadia_ADL_GraphSymbol; set during validation. */
  Arcadia_Map* parameterByName;
  /** @brief The unique `Graph.Output` symbol, or the null pointer; set during validation. */
  Arcadia_ADL_GraphSymbol* outputSymbol;
};

Arcadia_ADL_GraphTemplate*
Arcadia_ADL_GraphTemplate_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_List* nodes
  );

#endif // ARCADIA_ADL_GRAPH_TEMPLATE_H_INCLUDED
