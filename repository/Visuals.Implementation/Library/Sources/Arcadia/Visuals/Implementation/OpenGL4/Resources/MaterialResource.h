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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_MATERIALRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_MATERIALRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext;

typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource;
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource;
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource;
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource;

Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.MaterialResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource,
                          u8"Arcadia.Visuals.MaterialResource");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResourceDispatch {
  Arcadia_Engine_Visuals_MaterialResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource {
  Arcadia_Engine_Visuals_MaterialResource _parent;
  Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* ambientColorTexture;
  Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* program;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext,
    Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* ambientColorTexture,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* program
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_MATERIALRESOURCE_H_INCLUDED
