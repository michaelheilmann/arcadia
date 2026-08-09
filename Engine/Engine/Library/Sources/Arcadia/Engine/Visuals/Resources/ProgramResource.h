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

#if !defined(ARCADIA_ENGINE_VISUALS_RESOURCES_PROGRAMRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_RESOURCES_PROGRAMRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Visuals/Resource.h"

typedef struct Arcadia_Engine_Visuals_VertexProgramResource Arcadia_Engine_Visuals_VertexProgramResource;
typedef struct Arcadia_Engine_Visuals_FragmentProgramResource Arcadia_Engine_Visuals_FragmentProgramResource;

Arcadia_declareObjectType(u8"Arcadia.Visuals.ProgramResource", Arcadia_Engine_Visuals_ProgramResource,
                          u8"Arcadia.Visuals.Resource");

struct Arcadia_Engine_Visuals_ProgramResourceDispatch {
  Arcadia_Engine_Visuals_ResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_ProgramResource {
  Arcadia_Engine_Visuals_Resource _parent;
};

#endif // ARCADIA_ENGINE_VISUALS_RESOURCES_PROGRAMRESOURCE_H_INCLUDED
