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

#if !defined(ARCADIA_ADL_DEFINITIONS_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_ADL_Definition Arcadia_ADL_Definition;

// @brief A set of definitions.
//
// @code
// class Arcadia.ADL.Definitions extends Arcadia.Object {
//
//   field definitions : Arcadia.Map;
//
//   constructor();
//
//   method getQualifiedName(name : Arcadia.String) : Arcadia.ADL.Definition
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.Definitions", Arcadia_ADL_Definitions,
                          u8"Arcadia.Object");

struct Arcadia_ADL_DefinitionsDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_Definitions {
  Arcadia_Object _parent;
  Arcadia_Map* definitions;
};

Arcadia_ADL_Definitions*
Arcadia_ADL_Definitions_create
  (
    Arcadia_Thread* thread
  );

Arcadia_ADL_Definition*
Arcadia_ADL_Definitions_getDefinitionOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self,
    Arcadia_String* name
  );

void
Arcadia_ADL_Definitions_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self
  );

#endif  // ARCADIA_ADL_DEFINITIONS_H_INCLUDED
