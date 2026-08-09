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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext;
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource;

Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.FrameBufferResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource,
                          u8"Arcadia.Visuals.FrameBufferResource")

#define Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_Default (1)

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResourceDispatch {
  Arcadia_Engine_Visuals_FrameBufferResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource {
  Arcadia_Engine_Visuals_FrameBufferResource _parent;
  // A pointer to the backing texture.
  Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* texture;
  Arcadia_BooleanValue dirty;
  // @brief The width, in pixels, of the frame buffer.
  // @default 320.
  Arcadia_Integer32Value width;
  // @brief The height, in pixels of the frame buffer.
  // @default 240.
  Arcadia_Integer32Value height;
  // The OpenGL ID of the frame buffer.
  GLuint frameBufferID;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_FRAMEBUFFERRESOURCE_H_INCLUDED
