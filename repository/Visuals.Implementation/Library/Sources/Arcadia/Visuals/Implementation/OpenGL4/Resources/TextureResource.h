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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_TEXTURERESOURCE_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_TEXTURERESOURCE_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
typedef struct Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext;

Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.TextureResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource,
                          u8"Arcadia.Visuals.TextureResource");

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResourceDispatch {
  Arcadia_Engine_Visuals_TextureResourceDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource {
  Arcadia_Engine_Visuals_TextureResource _parent;

  // The address mode of this texture along the u-axis.
  // @default Arcadia_Visuals_TextureAddressMode_Repeat.
  Arcadia_Engine_Visuals_TextureAddressMode addressModeU;

  // The address mode of this texture along the v-axis.
  // @default Arcadia_Visuals_TextureAddressMode_Repeat.
  Arcadia_Engine_Visuals_TextureAddressMode addressModeV;

  // Bitmask indicating what aspects of the texture are dirty.
  Arcadia_Natural8Value dirtyBits;

  // The OpenGL ID of this texture.
  GLuint id;

  // The texture magnification filter.
  // @default Arcadia_Visuals_TextureFilter_Linear
  Arcadia_Engine_Visuals_TextureFilter magnificationFilter;
  // The texture minification filter.
  // @default Arcadia_Visuals_TextureFilter_Linear
  Arcadia_Engine_Visuals_TextureFilter minificationFilter;

  /// @brief The pixel buffer storing the texture data.
  Arcadia_Media_PixelBuffer* pixelBuffer;
};

Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_OPENGL4_RESOURCES_TEXTURERESOURCE_H_INCLUDED
