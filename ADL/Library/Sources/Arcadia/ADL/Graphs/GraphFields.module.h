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

#if !defined(ARCADIA_ADL_GRAPH_FIELDS_MODULE_H_INCLUDED)
#define ARCADIA_ADL_GRAPH_FIELDS_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"

// Field accessors shared by the graph collector and the graph expander.

/// @brief Get the map entry of the specified map node identified by the specified key, or the null pointer.
Arcadia_DDL_MapEntryNode*
Arcadia_ADL_GraphFields_getEntryOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    Arcadia_String* key
  );

/**
 * @brief Get the string value of the required string field identified by the specified key.
 * @error `Arcadia_Status_SemanticalError` the field is missing or is not a string.
 */
Arcadia_String*
Arcadia_ADL_GraphFields_getStringField
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    const char* cxxKey
  );

/**
 * @brief Get the string value of the required non-empty string field identified by the specified key.
 * @error `Arcadia_Status_SemanticalError` the field is missing, is not a string, or is empty.
 */
Arcadia_String*
Arcadia_ADL_GraphFields_getNonEmptyStringField
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* mapNode,
    const char* cxxKey
  );

#endif // ARCADIA_ADL_GRAPH_FIELDS_MODULE_H_INCLUDED
