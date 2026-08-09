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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_CHECKERBOARDFILLOPERATIONDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_CHECKERBOARDFILLOPERATIONDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"
typedef struct Arcadia_ADL_ColorDefinition Arcadia_ADL_ColorDefinition;

// A definition of a fill operation on a pixel buffer.
//
// @code
// {
//   type  : "PixelBufferOperations.CheckerboardFillOperation",
//   name  : "MyGame.RedCheckerboardFillOperation",
//   firstCheckerColor : "MyGame.Colors.Black",
//   secondCheckerColor : "MyGame.Colors.White",
//   checkerboardWidth : 8,
//   checkerboardHeight: 8,
// }
// @endcode
//
// @code
// class Arcadia.ADL.PixelBufferOperations.CheckerboardFillOperationDefinition extends Arcadia.ADL.Definition {
//
//   field firstCheckerColor : Arcadia.ADL.Reference
//
//   field secondCheckerColor : Arcadia.ADL.Reference
//
//   field checkerWidth : Arcadia.Integer32
//   field checkerHeight : Arcadia.Integer32
//
//   constructor(definitions : Arcadia.ADL.Definitions,
//               qualifiedName : Arcadia.String,
//               checkerWidth, checkerHeight : Arcadia.Integer32,
//               firstCheckerColorName, secondCheckerColorName : Arcadia.String)
//
//   method getColor() : Arcadia.String
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.PixelBufferOperations.CheckerboardFillOperationDefinition", Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition {
  Arcadia_ADL_Definition _parent;

  Arcadia_Integer32Value checkerWidth;
  Arcadia_Integer32Value checkerHeight;

  Arcadia_ADL_Reference* firstCheckerColor;
  Arcadia_ADL_Reference* secondCheckerColor;
};

Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value checkerWidth,
    Arcadia_Integer32Value checkerHeight,
    Arcadia_String* firstCheckerColorName,
    Arcadia_String* secondCheckerColorName
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_CHECKERBOARDFILLOPERATIONDEFINITION_H_INCLUDED
