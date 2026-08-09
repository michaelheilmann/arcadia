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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_MATERIALDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_MATERIALDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Definitions/Visuals/AmbientColorSource.h"
#include "Arcadia/ADL/Definitions/Visuals/MaterialType.h"
#include "Arcadia/ADL/Reference.h"

// The definition of a color.
//
// @code
// {
//   type  : "Material",
//   name  : "MyGame.Materials.MyMaterial",
//   materialType : "Blinn-Phong",
//   ambientSource : "Mesh",
// }
// @endcode
//
// @code
//
// enumeration Arcadia.ADL.MaterialType {
//   BlinnPhong,
// }
//
// enumeration Arcadia.ADL.AmbientSource {
//   Mesh,
//   Vertex,
//   Texture,
// }
//
// class Arcadia.ADL.MaterialDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, name : Arcadia.String,
//               materialType : Arcadial.ADL.MaterialType, ambientColorSource : Arcadia.ADL.AmbientColorSource)
//
// };
//
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.MaterialDefinition", Arcadia_ADL_MaterialDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_MaterialDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_MaterialDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_ADL_MaterialType materialType;
  Arcadia_ADL_AmbientColorSource ambientColorSource;
  Arcadia_ADL_Reference* ambientColorTexture;
};

Arcadia_ADL_MaterialDefinition*
Arcadia_ADL_MaterialDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_ADL_MaterialType materialType,
    Arcadia_ADL_AmbientColorSource ambientColorSource,
    Arcadia_String* ambientColorTextureName
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_MATERIALDEFINITION_H_INCLUDED
