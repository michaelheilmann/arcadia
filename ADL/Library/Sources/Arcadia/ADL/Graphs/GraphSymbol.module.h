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

#if !defined(ARCADIA_ADL_GRAPHS_INTERNAL_MODULE_H_INCLUDED)
#define ARCADIA_ADL_GRAPHS_INTERNAL_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"

// @brief An internal graph-local symbol used during validation and expansion.
//
// This type is an implementation detail of the graph front end and is never
// exposed through the public ADL headers.
//
// @code
// class Arcadia.ADL.GraphSymbol extends Arcadia.Object {
//
//   field typeName : Arcadia.String;
//
//   field name : Arcadia.String;
//
//   field referenceType : Arcadia.String;
//
//   field valueType : Arcadia.String;
//
//   field node : Arcadia.DDL.MapNode;
//
//   field referencedNames : Arcadia.Map;
//
//   constructor();
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.GraphSymbol", Arcadia_ADL_GraphSymbol,
                          u8"Arcadia.Object");

struct Arcadia_ADL_GraphSymbolDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_GraphSymbol {
  Arcadia_Object _parent;
  /** @brief The type name of the node. */
  Arcadia_String* typeName;
  /** @brief The local name of the node. */
  Arcadia_String* name;
  /** @brief For a `Graph.Parameter`, the declared reference type, else the null pointer. */
  Arcadia_String* referenceType;
  /** @brief For a `Graph.Parameter`, the declared value type, else the null pointer. */
  Arcadia_String* valueType;
  /** @brief The DDL map node. */
  Arcadia_DDL_MapNode* node;
  /** @brief Map from referenced local name to Arcadia_VoidValue_Void. */
  Arcadia_Map* referencedNames;
};

Arcadia_ADL_GraphSymbol*
Arcadia_ADL_GraphSymbol_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ADL_GRAPHS_INTERNAL_MODULE_H_INCLUDED
