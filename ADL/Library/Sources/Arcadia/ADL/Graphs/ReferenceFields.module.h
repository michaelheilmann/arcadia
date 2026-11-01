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

#if !defined(ARCADIA_ADL_REFERENCE_FIELDS_MODULE_H_INCLUDED)
#define ARCADIA_ADL_REFERENCE_FIELDS_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Graphs/Graphs.module.h"

typedef enum Arcadia_ADL_ReferenceFieldKind {
  Arcadia_ADL_ReferenceFieldKind_Single,
  Arcadia_ADL_ReferenceFieldKind_List,
} Arcadia_ADL_ReferenceFieldKind;

// @brief Describes one graph-reference-aware field of one ordinary node type.
//
// The graph front end resolves the string value(s) of such a field against the
// graph-local symbol table during validation and rewrites them to generated or
// externally bound names during expansion. All other fields are passed through unchanged.
typedef struct Arcadia_ADL_ReferenceFieldDescriptor {
  /** @brief The ADL type name of the owning node type (static C string). */
  const char* typeName;
  /** @brief The field name (static C string). */
  const char* fieldName;
  /** @brief The kind of the field. */
  Arcadia_ADL_ReferenceFieldKind kind;
  /** @brief Exact target type, "DSP" for the supported DSP family, or null for the pixel buffer operation family. */
  const char* targetTypeName;
} Arcadia_ADL_ReferenceFieldDescriptor;

/**
 * @brief Find the reference field descriptor for the specified owning type name and field name.
 * @param thread A pointer to this thread.
 * @param typeName The ADL type name of the owning node type.
 * @param fieldName The field name.
 * @return A pointer to the descriptor, or the null pointer if the field is not graph-reference-aware.
 */
const Arcadia_ADL_ReferenceFieldDescriptor*
Arcadia_ADL_ReferenceFields_find
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName,
    Arcadia_String* fieldName
  );

/**
 * @brief Get if the specified type name names a registered pixel buffer operation type.
 * @param thread A pointer to this thread.
 * @param typeName The ADL type name.
 * @return `Arcadia_BooleanValue_True` if the type is a pixel buffer operation type, `Arcadia_BooleanValue_False` otherwise.
 */
Arcadia_BooleanValue
Arcadia_ADL_ReferenceFields_isPixelBufferOperationType
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName
  );

/** @brief Get if the type names a graph-local DSP definition (SineWave or Mix). */
Arcadia_BooleanValue
Arcadia_ADL_ReferenceFields_isDSPDefinitionType
  (
    Arcadia_Thread* thread,
    Arcadia_String* typeName
  );

#endif // ARCADIA_ADL_REFERENCE_FIELDS_MODULE_H_INCLUDED
