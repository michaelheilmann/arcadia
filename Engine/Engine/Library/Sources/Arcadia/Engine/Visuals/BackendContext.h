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

#if !defined(ARCADIA_ENGINE_VISUALS_BACKENDCONTEXT_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_BACKENDCONTEXT_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Engine/BackendContext.h"
#include "Arcadia/PixelBufferIO/Include.h"

#include "Arcadia/Engine/Visuals/Resources/MaterialResource.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/VPL/Include.h"

typedef struct Arcadia_Engine_Visuals_Icon Arcadia_Engine_Visuals_Icon;
typedef struct Arcadia_Engine_Visuals_Window Arcadia_Engine_Visuals_Window;

typedef struct Arcadia_Engine_Visuals_ConstantBufferResource Arcadia_Engine_Visuals_ConstantBufferResource;
typedef struct Arcadia_Engine_Visuals_EnterPassResource Arcadia_Engine_Visuals_EnterPassResource;
typedef struct Arcadia_Engine_Visuals_FrameBufferResource Arcadia_Engine_Visuals_FrameBufferResource;
typedef struct Arcadia_Engine_Visuals_MaterialResource Arcadia_Engine_Visuals_MaterialResource;
typedef struct Arcadia_Engine_Visuals_ModelResource Arcadia_Engine_Visuals_ModelResource;
typedef struct Arcadia_Engine_Visuals_ProgramResource Arcadia_Engine_Visuals_ProgramResource;
typedef struct Arcadia_Engine_Visuals_TextureResource Arcadia_Engine_Visuals_TextureResource;
typedef struct Arcadia_Engine_Visuals_VertexBufferResource Arcadia_Engine_Visuals_VertexBufferResource;

/// @code
/// class Arcadia.Engine.Visuals.BackendContextBase extends Arcadia.Engine.Visuals.BackendContext {
///
///   constructor
///     (
///     )
///
///   method
///   createIcon
///     (
///       width : Arcadia.Integer32,
///       height : Arcadia.Integer32,
///       red : Arcadia.Natural8,
///       green : Arcadia.Natural8,
///       blue : Arcadia.Natural8,
///       alpha : Arcadia.Natural8
///     )
///     : Arcadia.Engine,Visuals.Icon
///
///   method
///   createWindow
///     (
///     )
///     : Arcadia.Engine.Visuals.Window
///
///   method
///   getDisplayDevices
///     (
///     )
///     : Arcadia.List
///
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.BackendContext", Arcadia_Engine_Visuals_BackendContext,
                          u8"Arcadia.Engine.BackendContext");

struct Arcadia_Engine_Visuals_BackendContextDispatch {
  Arcadia_Engine_BackendContextDispatch _parent;

  Arcadia_Engine_Visuals_Icon*
  (*createIcon)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self,
      Arcadia_Media_PixelBuffer* pixelBuffer
    );
  
  Arcadia_Engine_Visuals_Window*
  (*createWindow)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );
  
  Arcadia_List*
  (*getDisplayDevices)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );

  Arcadia_Engine_Visuals_ConstantBufferResource*
  (*createConstantBufferResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );

  Arcadia_Engine_Visuals_FrameBufferResource*
  (*createFrameBufferResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );

  Arcadia_Engine_Visuals_EnterPassResource*
  (*createEnterPassResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );

  Arcadia_Engine_Visuals_MaterialResource*
  (*createMaterialResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self,
      Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource,
      Arcadia_Engine_Visuals_TextureResource* ambientTexture,
      Arcadia_Engine_Visuals_ProgramResource* program
   );

  Arcadia_Engine_Visuals_ModelResource*
  (*createModelResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self,
      Arcadia_Engine_Visuals_ConstantBufferResource* meshConstantBufferResource,
      Arcadia_Engine_Visuals_VertexBufferResource* vertexBufferResource,
      Arcadia_Engine_Visuals_MaterialResource* material
    );

  Arcadia_Engine_Visuals_ProgramResource*
  (*createProgramResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self,
      Arcadia_VPL_Symbols_Program* program
    );

  Arcadia_Engine_Visuals_TextureResource*
  (*createTextureResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );

  Arcadia_Engine_Visuals_VertexBufferResource*
  (*createVertexBufferResource)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_BackendContext* self
    );
};

struct Arcadia_Engine_Visuals_BackendContext {
  Arcadia_Engine_BackendContext _parent;

  /// @brief List of windows.
  Arcadia_List* windows;
};

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_BackendContext_createIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  );

Arcadia_Engine_Visuals_Window*
Arcadia_Engine_Visuals_BackendContext_createWindow
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

/// @brief Enumerate all display devices.
/// @param thread A pointer to this thread.
/// @param self A pointer to this backend context.
Arcadia_List*
Arcadia_Engine_Visuals_BackendContext_getDisplayDevices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

// Create a constant buffer resource.
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_ConstantBufferResource*
Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

// Create a frame buffer resource.
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_FrameBufferResource*
Arcadia_Engine_Visuals_BackendContext_createFrameBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

// Create a rendering context resource.
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_EnterPassResource*
Arcadia_Engine_Visuals_BackendContext_createEnterPassResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

// Create a material resource.
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_MaterialResource*
Arcadia_Engine_Visuals_BackendContext_createMaterialResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource,
    Arcadia_Engine_Visuals_TextureResource* ambientTexture,
    Arcadia_Engine_Visuals_ProgramResource* program
  );

// Create a model resource.
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_ModelResource*
Arcadia_Engine_Visuals_BackendContext_createModelResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Engine_Visuals_ConstantBufferResource* meshConstantBufferResource,
    Arcadia_Engine_Visuals_VertexBufferResource* vertexBufferResource,
    Arcadia_Engine_Visuals_MaterialResource* material
  );

// Create a program resource
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_ProgramResource*
Arcadia_Engine_Visuals_BackendContext_createProgramResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_VPL_Symbols_Program* program
  );

// Create a texture resource
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_TextureResource*
Arcadia_Engine_Visuals_BackendContext_createTextureResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

// Create a vertex buffer resource.
// The initial reference count of the created resource is @a 0, hence it would be destroyed at the next update of the backend.
Arcadia_Engine_Visuals_VertexBufferResource*
Arcadia_Engine_Visuals_BackendContext_createVertexBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

#endif // ARCADIA_ENGINE_VISUALS_BACKENDCONTEXT_H_INCLUDED
