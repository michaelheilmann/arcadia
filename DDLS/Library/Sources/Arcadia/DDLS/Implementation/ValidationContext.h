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

#if !defined(ARCADIA_DDLS_IMPLEMENTATION_VALIDATIONCONTEXT_H_INCLUDED)
#define ARCADIA_DDLS_IMPLEMENTATION_VALIDATIONCONTEXT_H_INCLUDED

#include "Arcadia/DDLS/Nodes/Include.h"
#include "Arcadia/DDL/Include.h"
#include "Arcadia/DDLS/Implementation/Diagnostics.h"
#include "Arcadia/Languages/Include.h"

// Internal type of a validation context.
Arcadia_declareObjectType(u8"Arcadia.DDLS.ValidationContext", Arcadia_DDLS_ValidationContext,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_ValidationContextDispatch {
  Arcadia_ObjectDispatch parent;
  void (*run)(Arcadia_Thread* thread, Arcadia_DDLS_ValidationContext*, Arcadia_String* name, Arcadia_DDL_Node*);
};

struct Arcadia_DDLS_ValidationContext {
  Arcadia_Object parent;
  /// @brief The schemata used by this validation context.
  Arcadia_Map* schemata;
  Arcadia_DDLS_Diagnostics* diagnostics;
  Arcadia_StringBuilder* temporary1;
  Arcadia_Languages_StringTable* stringTable;
};

Arcadia_DDLS_ValidationContext*
Arcadia_DDLS_ValidationContext_create
  (
    Arcadia_Thread* thread
  );

/// @brief Add a schema node to this validation context.
/// @param thread A pointer to this thread.
/// @param self A pointer to this validation context.
/// @param schemaNode A pointer the schema node.
/// @error A schema of the same name already eixsts in this validation context.
void
Arcadia_DDLS_ValidationContext_addSchema
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_DDLS_SchemaNode* schemaNode
  );

/// @brief Get a schema node from this validation context.
/// @param thread A pointer to this thread.
/// @param self A pointer to this validation context.
/// @param name The name of the schema node.
/// @return A pointer to the schema node if it was found. The null pointer otherwise.
Arcadia_DDLS_SchemaNode*
Arcadia_DDLS_ValidationContext_getSchema
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name
  );

/// @brief Remove a schema node from this validation context.
/// @param thread A pointer to this thread.
/// @param self A pointer to this validation context.
/// @param name The name of the schema node.
/// @return A pointer to the schema node if it was found. The null pointer otherwise.
Arcadia_DDLS_SchemaNode*
Arcadia_DDLS_ValidationContext_removeSchema
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name
  );

void
Arcadia_DDLS_ValidationContext_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ValidationContext* self,
    Arcadia_String* name,
    Arcadia_DDL_Node* node
  );

#endif // ARCADIA_DDLS_IMPLEMENTATION_VALIDATIONCONTEXT_H_INCLUDED
