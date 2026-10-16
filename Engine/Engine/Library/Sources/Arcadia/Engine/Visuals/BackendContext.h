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
  #error("do not include `Arcadia/Engine/Visuals/BackendContext.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/BackendContext.h"
#include "Arcadia/Engine/Visuals/BlendFunction.h"
#include "Arcadia/Engine/Visuals/ColorSource.h"

typedef struct Arcadia_Engine_Visuals_BackendContext Arcadia_Engine_Visuals_BackendContext;
typedef struct Arcadia_Engine_Visuals_ConstantBufferResource Arcadia_Engine_Visuals_ConstantBufferResource;
typedef struct Arcadia_Engine_Visuals_EnterPassResource Arcadia_Engine_Visuals_EnterPassResource;
typedef struct Arcadia_Engine_Visuals_FrameBufferResource Arcadia_Engine_Visuals_FrameBufferResource;
typedef struct Arcadia_Engine_Visuals_Icon Arcadia_Engine_Visuals_Icon;
typedef struct Arcadia_Engine_Visuals_MaterialResource Arcadia_Engine_Visuals_MaterialResource;
typedef struct Arcadia_Engine_Visuals_ModelResource Arcadia_Engine_Visuals_ModelResource;
typedef struct Arcadia_Engine_Visuals_ProgramResource Arcadia_Engine_Visuals_ProgramResource;
typedef struct Arcadia_Engine_Visuals_TextureResource Arcadia_Engine_Visuals_TextureResource;
typedef struct Arcadia_Engine_Visuals_VertexBufferResource Arcadia_Engine_Visuals_VertexBufferResource;
typedef struct Arcadia_Engine_Visuals_Window Arcadia_Engine_Visuals_Window;
typedef struct Arcadia_List Arcadia_List;
typedef struct Arcadia_Media_PixelBuffer Arcadia_Media_PixelBuffer;
typedef struct Arcadia_VPL_VerifiedModule Arcadia_VPL_VerifiedModule;

Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.BackendContext", Arcadia_Engine_Visuals_BackendContext,
                          u8"Arcadia.Engine.BackendContext");

struct Arcadia_Engine_Visuals_BackendContextDispatch {
  Arcadia_Engine_BackendContextDispatch _parent;

  Arcadia_Engine_Visuals_Icon* (*createIcon)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_Media_PixelBuffer*);

  Arcadia_Engine_Visuals_Window* (*createWindow)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);

  Arcadia_List* (*getDisplayDevices)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);

  Arcadia_Engine_Visuals_ConstantBufferResource* (*createConstantBufferResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);

  Arcadia_Engine_Visuals_FrameBufferResource* (*createFrameBufferResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);

  Arcadia_Engine_Visuals_EnterPassResource* (*createEnterPassResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);

  Arcadia_Engine_Visuals_MaterialResource* (*createMaterialResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_BooleanValue, Arcadia_Engine_Visuals_BlendFunction, Arcadia_Engine_Visuals_BlendFunction, Arcadia_Engine_Visuals_ColorSource, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_ProgramResource*);

  Arcadia_Engine_Visuals_ModelResource* (*createModelResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_Engine_Visuals_ConstantBufferResource*, Arcadia_Engine_Visuals_VertexBufferResource*, Arcadia_Engine_Visuals_MaterialResource*);

  Arcadia_Engine_Visuals_ProgramResource* (*createProgramResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_VPL_VerifiedModule*);

  Arcadia_Engine_Visuals_TextureResource* (*createTextureResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);

  Arcadia_Engine_Visuals_VertexBufferResource* (*createVertexBufferResource)(Arcadia_Thread*, Arcadia_Engine_Visuals_BackendContext*);
};

struct Arcadia_Engine_Visuals_BackendContext {
  Arcadia_Engine_BackendContext _parent;
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

Arcadia_List*
Arcadia_Engine_Visuals_BackendContext_getDisplayDevices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

Arcadia_Engine_Visuals_ConstantBufferResource*
Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

Arcadia_Engine_Visuals_FrameBufferResource*
Arcadia_Engine_Visuals_BackendContext_createFrameBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

Arcadia_Engine_Visuals_EnterPassResource*
Arcadia_Engine_Visuals_BackendContext_createEnterPassResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

Arcadia_Engine_Visuals_MaterialResource*
Arcadia_Engine_Visuals_BackendContext_createMaterialResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_BooleanValue blendEnabled,
    Arcadia_Engine_Visuals_BlendFunction blendSourceFunction,
    Arcadia_Engine_Visuals_BlendFunction blendDestinationFunction,
    Arcadia_Engine_Visuals_ColorSource ambientColorSource,
    Arcadia_Engine_Visuals_TextureResource* ambientTexture,
    Arcadia_Engine_Visuals_ProgramResource* program
  );

Arcadia_Engine_Visuals_ModelResource*
Arcadia_Engine_Visuals_BackendContext_createModelResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Engine_Visuals_ConstantBufferResource* meshConstantBufferResource,
    Arcadia_Engine_Visuals_VertexBufferResource* vertexBufferResource,
    Arcadia_Engine_Visuals_MaterialResource* material
  );

Arcadia_Engine_Visuals_ProgramResource*
Arcadia_Engine_Visuals_BackendContext_createProgramResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_VPL_VerifiedModule* program
  );

Arcadia_Engine_Visuals_TextureResource*
Arcadia_Engine_Visuals_BackendContext_createTextureResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

Arcadia_Engine_Visuals_VertexBufferResource*
Arcadia_Engine_Visuals_BackendContext_createVertexBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

#endif // ARCADIA_ENGINE_VISUALS_BACKENDCONTEXT_H_INCLUDED
