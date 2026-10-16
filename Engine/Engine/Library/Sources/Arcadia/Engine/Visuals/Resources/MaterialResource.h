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

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/Resources/MaterialResource.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Visuals/BlendFunction.h"
#include "Arcadia/Engine/Visuals/ColorSource.h"
#include "Arcadia/Engine/Visuals/Resource.h"

typedef struct Arcadia_Engine_Visuals_MaterialResource Arcadia_Engine_Visuals_MaterialResource;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.MaterialResource", Arcadia_Engine_Visuals_MaterialResource,
                          u8"Arcadia.Engine.Visuals.Resource");

struct Arcadia_Engine_Visuals_MaterialResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_MaterialResource {
  Arcadia_Engine_Visuals_Resource _parent;
  Arcadia_BooleanValue blendEnabled;
  Arcadia_Engine_Visuals_BlendFunction blendSourceFunction;
  Arcadia_Engine_Visuals_BlendFunction blendDestinationFunction;
  Arcadia_Engine_Visuals_ColorSource ambientColorSource;
};

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_MATERIALRESOURCE_H_INCLUDED
