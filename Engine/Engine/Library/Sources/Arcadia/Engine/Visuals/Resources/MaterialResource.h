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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_MATERIALRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_MATERIALRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Resource.h"
#include "Arcadia/Math/Include.h"

typedef enum Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource {
  Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Mesh,
  Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Vertex,
  Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource_Texture,
} Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource;

Arcadia_declareObjectType(u8"Arcadia.Visuals.MaterialResource", Arcadia_Engine_Visuals_MaterialResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_MaterialResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_MaterialResource {
  Arcadia_Engine_Visuals_Resource _parent;
};

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_MATERIALRESOURCE_H_INCLUDED
