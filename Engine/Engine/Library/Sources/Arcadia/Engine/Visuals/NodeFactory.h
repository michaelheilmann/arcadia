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

#if !defined(ARCADIA_ENGINE_VISUALS_NODEFACTORY_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_NODEFACTORY_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Math/Include.h"
#include "Arcadia/Engine//NodeFactory.h"
#include "Arcadia/Engine/Visuals/Nodes/CameraNode.h"
#include "Arcadia/Engine/Visuals/Nodes/FrameBufferNode.h"
#include "Arcadia/Engine/Visuals/Nodes/EnterPassNode.h"
#include "Arcadia/Engine/Visuals/Nodes/MaterialNode.h"
#include "Arcadia/Engine/Visuals/Nodes/MeshNode.h"
#include "Arcadia/Engine/Visuals/Nodes/ModelNode.h"
#include "Arcadia/Engine/Visuals/Nodes/PixelBufferNode.h"
#include "Arcadia/Engine/Visuals/Nodes/TextureNode.h"
#include "Arcadia/Engine/Visuals/Nodes/ViewportNode.h"

/// The implementation of a node factory for visuals nodes.
Arcadia_declareObjectType(u8"Arcadia.Engine.Visuals.NodeFactory", Arcadia_Engine_Visuals_NodeFactory,
                          Arcadia_Engine_NodeFactory);

struct Arcadia_Engine_Visuals_NodeFactoryDispatch {
  Arcadia_Engine_NodeFactoryDispatch parent;

  Arcadia_Engine_Visuals_CameraNode*
  (*createCameraNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext
    );

  Arcadia_Engine_Visuals_FrameBufferNode*
  (*createFrameBufferNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext
    );

  Arcadia_Engine_Visuals_EnterPassNode*
  (*createEnterPassNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext
    );

  Arcadia_Engine_Visuals_MaterialNode*
  (*createMaterialNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext,
      Arcadia_ADL_MaterialDefinition* source
    );

  Arcadia_Engine_Visuals_MeshNode*
  (*createMeshNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext,
      Arcadia_ADL_MeshDefinition* source
    );

  Arcadia_Engine_Visuals_ModelNode*
  (*createModelNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext,
      Arcadia_ADL_ModelDefinition* source
    );

  Arcadia_Engine_Visuals_PixelBufferNode*
  (*createPixelBufferNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext,
      Arcadia_ADL_PixelBufferDefinition* source
    );


  Arcadia_Engine_Visuals_TextureNode*
  (*createTextureNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext,
      Arcadia_ADL_TextureDefinition* source
    );

  Arcadia_Engine_Visuals_ViewportNode*
  (*createViewportNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Visuals_NodeFactory* self,
      Arcadia_Engine_Visuals_BackendContext* backendContext
    );
};

struct Arcadia_Engine_Visuals_NodeFactory {
  Arcadia_Engine_NodeFactory parent;
};

Arcadia_Engine_Visuals_NodeFactory*
Arcadia_Engine_Visuals_NodeFactory_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Engine_Visuals_CameraNode*
Arcadia_Engine_Visuals_NodeFactory_createCameraNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

Arcadia_Engine_Visuals_EnterPassNode*
Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_NodeFactory_createMaterialNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  );

Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_NodeFactory_createMeshNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MeshDefinition* source
  );

Arcadia_Engine_Visuals_ModelNode*
Arcadia_Engine_Visuals_NodeFactory_createModelNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  );

Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  );

Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_NodeFactory_createTextureNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  );

Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_NodeFactory_createViewportNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

#endif // ARCADIA_ENGINE_VISUALS_NODEFACTORY_H_INCLUDED
