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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_MESHGENERATOR_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_MESHGENERATOR_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"

typedef struct Arcadia_ADL_ColorDefinition Arcadia_ADL_ColorDefinition;

// The base class of the mesh generator definitions.
//
// A mesh generator describes a mesh procedurally. It carries the ambient color of the mesh it generates.
//
// @code
// class Arcadia.ADL.MeshGenerator extends Arcadia.ADL.Definition {
//
//   field ambientColor : Arcadia.ADL.Reference
//
//   constructor(definitions : Arcadia.ADL.Definitions, name : Arcadia.String, ambientColorName : Arcadia.String)
//
//   method getAmbientColor() : Arcadia.ADL.ColorDefinition
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.MeshGenerator", Arcadia_ADL_MeshGenerator,
                           u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_MeshGeneratorDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_MeshGenerator {
  Arcadia_ADL_Definition _parent;
  Arcadia_ADL_Reference* ambientColor;
};

/// @brief Get the ambient color of this mesh generator.
/// @param thread A pointer to this thread.
/// @param self A pointer to this mesh generator.
/// @return A pointer to the ambient color definition.
Arcadia_ADL_ColorDefinition*
Arcadia_ADL_MeshGenerator_getAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  );

#endif // ARCADIA_ADL_DEFINITIONS_VISUALS_MESHGENERATOR_H_INCLUDED
