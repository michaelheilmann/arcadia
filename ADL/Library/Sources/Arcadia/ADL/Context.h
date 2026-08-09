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

#if !defined(ARCADIA_ADL_CONTEXT_H_INCLUDED)
#define ARCADIA_ADL_CONTEXT_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/DDL/Include.h"
typedef struct Arcadia_ADL_Definition Arcadia_ADL_Definition;
typedef struct Arcadia_ADL_Definitions Arcadia_ADL_Definitions;

/**
 * The ADL context is a singleton providing access to ADL functionality including but not restricted to
 * a) adding/removing readers and writer
 * b) reading/writing definitions
 */
Arcadia_declareObjectType(u8"Arcadia.ADL.Context", Arcadia_ADL_Context,
                          u8"Arcadia.Object");

struct Arcadia_ADL_ContextDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_Context {
  Arcadia_Object _parent;
  /** @brief Map of ADL types to readers. */
  Arcadia_Map* readers;
};

/**
 * @brief Get or create the ADL context.
 * @param thread A pointer to this thread.
 * @return A pointer to the ADL context.
 */
Arcadia_ADL_Context*
Arcadia_ADL_Context_getOrCreate
  (
    Arcadia_Thread* thread
  );

/**
 * @brief Read a definition.
 * @param thread A pointer to this thread.
 * @param self A pointer to this ADL context.
 * @param definitions A pointer to the ADL definitions to add the definition to.
 * @param input A pointer to the input DDL node.
 * @param skipExisting If an ADL definition of the same name is already loaded, ignore newly loaded ADL definition(s) of that name.
 * @return A pointer to the ADL definition.
 */
Arcadia_ADL_Definition*
Arcadia_ADL_Context_readFromNode
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_DDL_Node* input,
    Arcadia_BooleanValue skipExisting
  );

/**
 * @brief Read a definition.
 * @param thread A pointer to this thread.
 * @param self A pointer to this ADL context.
 * @param definitions A pointer to the ADL definitions to add the definition to.
 * @param input A pointer to the input string.
 * @param skipExisting If an ADL definition of the same name is already loaded, ignore newly loaded ADL definition(s) of that name.
 * @return A pointer to the ADL definition.
 */
Arcadia_ADL_Definition*
Arcadia_ADL_Context_readFromString
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* self,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* input,
    Arcadia_BooleanValue skipExisting
  );

#endif  // ARCADIA_ADL_CONTEXT_H_INCLUDED
