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

#if !defined(ARCADIA_ADL_DEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_ADL_Definitions Arcadia_ADL_Definitions;

// @brief A definition.
//
// @code
// class Arcadia.ADL.Definition extends Arcadia.Object {
//
//   field definitions : Arcadia.ADL.Definitions
//
//   field name : Arcadia.String
//
//   constructor(definitions: Arcadia.ADL.Definitions, name : Arcadia.String)
//
//   method getDefinitions() : Arcadia.ADL.Definitions
//
//   method getName() : Arcadia.String
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                          u8"Arcadia.Object");

struct Arcadia_ADL_DefinitionDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*link)(Arcadia_Thread*, Arcadia_ADL_Definition*);
};

struct Arcadia_ADL_Definition {
  Arcadia_Object _parent;
  Arcadia_ADL_Definitions* definitions;
  Arcadia_String* name;
};

Arcadia_ADL_Definitions*
Arcadia_ADL_Definitions_getDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  );

Arcadia_String*
Arcadia_ADL_Definition_getName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  );

/// @brief Resolve references of this definition.
/// @param thread A pointer to this thread.
/// @param self A pointer to this definition.
/// @see Arcadia_ADL_Reference_resolve
/// @todo We need a post link step which is executed after successful linking.
void
Arcadia_ADL_Definition_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  );

#endif  // ARCADIA_ADL_DEFINITION_H_INCLUDED
