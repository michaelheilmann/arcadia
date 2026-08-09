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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_PIXELBUFFERDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_PIXELBUFFERDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/Collections/Include.h"

// A definition of a pixel buffer.
//
// @code
// {
//   type  : "PixelBuffer",
//   name  : "MyGame.CheckerboardPixelBuffer",
//   width : 320,
//   height : 240,
//   operations : [ ... ],
// }
// @endcode
//
// @code
// class Arcadia.ADL.PixelBufferDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, qualifiedName : Arcadia.String)
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.PixelBufferDefinition", Arcadia_ADL_PixelBufferDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_PixelBufferDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_PixelBufferDefinition {
  Arcadia_ADL_Definition _parent;
  /// The width, in pixels, of the pixel buffer.
  Arcadia_Natural32Value width;
  /// The height, in pixels, of the pixel bufer.
  Arcadia_Natural32Value height;
  /// The list of operations.
  Arcadia_List* operations;
};

Arcadia_ADL_PixelBufferDefinition*
Arcadia_ADL_PixelBufferDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_PIXELBUFFERDEFINITION_H_INCLUDED
