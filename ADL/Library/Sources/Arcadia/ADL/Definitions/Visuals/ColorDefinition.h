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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_COLORDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_COLORDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"

// The definition of a color.
//
// @code
// {
//   type  : "Color",
//   name  : "MyGame.Colors.Red",
//   red   : 255,
//   green : 0,
//   blue  : 0,
// }
// @endcode
//
// @code
// class Arcadia.ADL.ColorDefinition extends Arcadia.ADL.Definition {
//
//   field red, green, blue : Arcadia.Natural8
//
//   constructor(definitions : Arcadia.ADL.Definitions, name : Arcadia.String, red, green, blue : Arcadia.Natural8)
//
//   method getRed() : Arcadia.Natural8
//   method getGreen() : Arcadia.Natural8
//   method getBlue() : Arcadia.Natural8
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.ColorDefinition", Arcadia_ADL_ColorDefinition,
                          u8"Arcadia.Object");

struct Arcadia_ADL_ColorDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_ColorDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_Natural8Value red;
  Arcadia_Natural8Value green;
  Arcadia_Natural8Value blue;
};

Arcadia_ADL_ColorDefinition*
Arcadia_ADL_ColorDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Natural8Value red,
    Arcadia_Natural8Value green,
    Arcadia_Natural8Value blue
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_COLORDEFINITION_H_INCLUDED
