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

#include "Arcadia/Engine/Include.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/Include.h"

Arcadia_Engine_Visuals_CameraNode*
Arcadia_Engine_Visuals_NodeFactory_createCameraNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_CameraNode_create(thread, backendContext); }

Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_FrameBufferNode_create(thread, backendContext); }

Arcadia_Engine_Visuals_EnterPassNode*
Arcadia_Engine_Visuals_NodeFactory_createEnterPassNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_EnterPassNode_create(thread, backendContext); }

Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_NodeFactory_createMaterialNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  )
{ return Arcadia_Engine_Visuals_MaterialNode_create(thread, backendContext, source); }

Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_NodeFactory_createMeshNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_Media_Geometry* geometry
  )
{ return Arcadia_Engine_Visuals_MeshNode_create(thread, backendContext, geometry); }

Arcadia_Engine_Visuals_ModelNode*
Arcadia_Engine_Visuals_NodeFactory_createModelNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  )
{ return Arcadia_Engine_Visuals_ModelNode_create(thread, backendContext, source); }

Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  )
{ return Arcadia_Engine_Visuals_PixelBufferNode_create(thread, backendContext, source); }

Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_NodeFactory_createTextureNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  )
{ return Arcadia_Engine_Visuals_TextureNode_create(thread, backendContext, source); }

Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_NodeFactory_createViewportNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_ViewportNode_create(thread, backendContext); }
