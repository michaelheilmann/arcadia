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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_FILLOPERATIONDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_FILLOPERATIONDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
typedef struct Arcadia_ADL_ColorDefinition Arcadia_ADL_ColorDefinition;
typedef struct Arcadia_ADL_Reference Arcadia_ADL_Reference;

// A definition of a fill operation on a pixel buffer.
//
// @code
// {
//   type  : "PixelBufferOperations.FillOperation",
//   name  : "MyGame.RedFillOperation",
//   color : "MyGame.Colors.Red",
// }
// @endcode
//
// @code
// class Arcadia.ADL.PixelBufferOperations.FillOperationDefinition extends Arcadia.ADL.Definition {
//
//   field color : Arcadia.ADL.ColorDefinition
//   field colorName : Arcadia.String
//
//   constructor(definitions : Arcadia.ADL.Definitions, qualifiedName : Arcadia.String, colorName : Arcadia.String)
//
//   method getColor() : Arcadia.String
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.PixelBufferOperations.FillOperationDefinition", Arcadia_ADL_PixelBufferOperations_FillOperationDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_PixelBufferOperations_FillOperationDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_PixelBufferOperations_FillOperationDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_ADL_Reference* color;
};

Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*
Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_String* colorName
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_FILLOPERATIONDEFINITION_H_INCLUDED
