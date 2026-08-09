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

#if !defined(ARCADIA_ADL_DEFINITIONS_DSP_CONSTANTDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_DSP_CONSTANTDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"

// A DSP generating a constant signal.
//
// @code
// {
//   type  : "DSP.Constant",
//   name  : "MyGame.MyDSPs.MyConstant",
//   value : 1, /* Numeric value. */ 
// }
// @endcode
//
// @code
// class Arcadia.ADL.ConstantDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, name : Arcadia.String)
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.ConstantDefinition", Arcadia_ADL_ConstantDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_ConstantDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_ConstantDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_Real32Value value;
};

Arcadia_ADL_ConstantDefinition*
Arcadia_ADL_ConstantDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Real32Value value
  );

#endif  // ARCADIA_ADL_DEFINITIONS_DSP_CONSTANTDEFINITION_H_INCLUDED
