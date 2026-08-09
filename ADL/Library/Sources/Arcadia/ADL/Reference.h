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

#if !defined(ARCADIA_ADL_REFERENCE_H_INCLUDED)
#define ARCADIA_ADL_REFERENCE_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_ADL_Definition Arcadia_ADL_Definition;
typedef struct Arcadia_ADL_Definitions Arcadia_ADL_Definitions;

// @brief A reference.
//
// @code
// class Arcadia.ADL.Reference extends Arcadia.Object {
//
//   field definitions : Arcadia.ADL.Definitions
//
//   field definition : Arcadia.ADL.Definition
//
//   field definitionName : Arcadia.String
//
//   constructor(definitions: Arcadia.ADL.Definitions, definitionName : Arcadia.String)
//
//   method getDefinitionName() : Arcadia.String
//
//   method getDefinitions() : Arcadia.ADL.Definitions
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.Reference", Arcadia_ADL_Reference,
                          u8"Arcadia.Object");

struct Arcadia_ADL_ReferenceDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_ADL_Reference {
  Arcadia_Object _parent;

  Arcadia_ADL_Definition* definition;
  Arcadia_String* definitionName;
  Arcadia_ADL_Definitions* definitions;
};

Arcadia_ADL_Reference*
Arcadia_ADL_Reference_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* definitionName
  );

Arcadia_ADL_Definitions*
Arcadia_ADL_References_getDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  );

Arcadia_String*
Arcadia_ADL_Reference_getDefinitionName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  );

void
Arcadia_ADL_Reference_resolve
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  );

#endif  // ARCADIA_ADL_REFERENCE_H_INCLUDED
