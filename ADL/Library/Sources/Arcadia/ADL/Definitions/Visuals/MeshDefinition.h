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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_MESHDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_MESHDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"

// The definition of a color.
//
// @code
// {
//   type  : "Mesh",
//   name  : "MyGame.Meshes.MyMesh",
// }
// @endcode
//
// @code
// class Arcadia.ADL.MeshDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, name : Arcadia.String)
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.MeshDefinition", Arcadia_ADL_MeshDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_MeshDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_MeshDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_SizeValue numberOfVertices;
  Arcadia_RuntimeByteArray* vertexPositions;
  Arcadia_RuntimeByteArray* vertexAmbientColors;
  Arcadia_RuntimeByteArray* vertexAmbientTextureCoordinates;
  Arcadia_ADL_Reference* ambientColor;
};

Arcadia_ADL_MeshDefinition*
Arcadia_ADL_MeshDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_SizeValue numberOfVertices,
    Arcadia_RuntimeByteArray* vertexPositions,
    Arcadia_RuntimeByteArray* vertexAmbientColors,
    Arcadia_RuntimeByteArray* vertexAmbientTextureCoordinates,
    Arcadia_String* ambientColorName
  );

Arcadia_SizeValue
Arcadia_ADL_MeshDefinition_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

Arcadia_RuntimeByteArray*
Arcadia_ADL_MeshDefinition_getVertexPositions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

Arcadia_RuntimeByteArray*
Arcadia_ADL_MeshDefinition_getVertexAmbientColors
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

Arcadia_RuntimeByteArray*
Arcadia_ADL_MeshDefinition_getVertexAmbientTextureCoordinates
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_MESHDEFINITION_H_INCLUDED
