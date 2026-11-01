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

#if !defined(ARCADIA_ADL_VALUE_FIELDS_MODULE_H_INCLUDED)
#define ARCADIA_ADL_VALUE_FIELDS_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Graphs/Graphs.module.h"

// @brief Describes one value-parameter-aware field of one ordinary node type.
//
// Unlike a reference field, whose value names another definition, a value field
// carries a graph-local reference to a value-typed `Graph.Parameter`. During
// expansion the graph front end replaces the reference string with the bound
// DDL number node, so that the ordinary reader observes a plain numeric literal.
// The value type name is the expected `valueType` of the parameter.
typedef struct Arcadia_ADL_ValueFieldDescriptor {
  /** @brief The ADL type name of the owning node type (static C string). */
  const char* typeName;
  /** @brief The field name (static C string). */
  const char* fieldName;
  /** @brief The value type name a bound parameter must declare (static C string). */
  const char* valueTypeName;
} Arcadia_ADL_ValueFieldDescriptor;

/**
 * @brief Find the value field descriptor for the specified owning type name and field name.
 * @param thread A pointer to this thread.
 * @param typeName The ADL type name of the owning node type.
 * @param fieldName The field name.
 * @return A pointer to the descriptor, or the null pointer if the field is not graph-value-aware.
 */
const Arcadia_ADL_ValueFieldDescriptor*
Arcadia_ADL_ValueFields_find
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName,
    Arcadia_String* fieldName
  );

#endif // ARCADIA_ADL_VALUE_FIELDS_MODULE_H_INCLUDED
