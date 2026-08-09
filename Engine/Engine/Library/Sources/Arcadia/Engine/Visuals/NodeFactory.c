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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Visuals/NodeFactory.h"

static void
Arcadia_Engine_Visuals_NodeFactory_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self
  );

static void
Arcadia_Engine_Visuals_NodeFactory_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self
  );

static void
Arcadia_Engine_Visuals_NodeFactory_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactoryDispatch* self
  );

static void
Arcadia_Engine_Visuals_NodeFactory_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self
  );

static Arcadia_Engine_Visuals_CameraNode*
Arcadia_Engine_Visuals_NodeFactory_createCameraNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_NodeFactory_createMaterialNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  );

static Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_NodeFactory_createMeshNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MeshDefinition* source
  );

static Arcadia_Engine_Visuals_ModelNode*
Arcadia_Engine_Visuals_NodeFactory_createModelNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  );

static Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  );

static Arcadia_Engine_Visuals_EnterPassNode*
Arcadia_Engine_Visuals_NodeFactory_createEnterPassNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_NodeFactory_createTextureNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  );

static Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_NodeFactory_createViewportNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Visuals_NodeFactory_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_NodeFactory_construct,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_NodeFactory_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_NodeFactory_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_NodeFactory_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Visuals_NodeFactory_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Visuals_NodeFactory_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.NodeFactory", Arcadia_Engine_Visuals_NodeFactory,
                         u8"Arcadia.Engine.NodeFactory", Arcadia_Engine_NodeFactory,
                         &_Arcadia_Engine_Visuals_NodeFactory_typeOperations);

static void
Arcadia_Engine_Visuals_NodeFactory_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_NodeFactory);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_NodeFactory);
}

static void
Arcadia_Engine_Visuals_NodeFactory_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_NodeFactory_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactoryDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createCameraNode = (Arcadia_Engine_Visuals_CameraNode *(*)(Arcadia_Thread * thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_NodeFactory_createCameraNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createFrameBufferNode = (Arcadia_Engine_Visuals_FrameBufferNode *(*)(Arcadia_Thread * thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createEnterPassNode = (Arcadia_Engine_Visuals_EnterPassNode * (*)(Arcadia_Thread * thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_NodeFactory_createEnterPassNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createMaterialNode = (Arcadia_Engine_Visuals_MaterialNode *(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_ADL_MaterialDefinition*)) & Arcadia_Engine_Visuals_NodeFactory_createMaterialNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createMeshNode = (Arcadia_Engine_Visuals_MeshNode *(*)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_ADL_MeshDefinition*)) & Arcadia_Engine_Visuals_NodeFactory_createMeshNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createModelNode = (Arcadia_Engine_Visuals_ModelNode * (*)(Arcadia_Thread * thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_ADL_ModelDefinition*)) & Arcadia_Engine_Visuals_NodeFactory_createModelNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createPixelBufferNode = (Arcadia_Engine_Visuals_PixelBufferNode * (*)(Arcadia_Thread * thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_ADL_PixelBufferDefinition*)) & Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createTextureNode = (Arcadia_Engine_Visuals_TextureNode *(*)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*, Arcadia_ADL_TextureDefinition*)) & Arcadia_Engine_Visuals_NodeFactory_createTextureNodeImpl;
  ((Arcadia_Engine_Visuals_NodeFactoryDispatch*)self)->createViewportNode = (Arcadia_Engine_Visuals_ViewportNode * (*)(Arcadia_Thread* thread, Arcadia_Engine_Visuals_NodeFactory*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_NodeFactory_createViewportNodeImpl;
}

static void
Arcadia_Engine_Visuals_NodeFactory_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self
  )
{/*Intentionally empty.*/}

static Arcadia_Engine_Visuals_CameraNode*
Arcadia_Engine_Visuals_NodeFactory_createCameraNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_CameraNode_create(thread, backendContext); }

static Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_FrameBufferNode_create(thread, backendContext); }

static Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_NodeFactory_createMaterialNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  )
{ return Arcadia_Engine_Visuals_MaterialNode_create(thread, backendContext, source); }

static Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_NodeFactory_createMeshNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MeshDefinition* source
  )
{ return Arcadia_Engine_Visuals_MeshNode_create(thread, backendContext, source); }

static Arcadia_Engine_Visuals_ModelNode*
Arcadia_Engine_Visuals_NodeFactory_createModelNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  )
{ return Arcadia_Engine_Visuals_ModelNode_create(thread, backendContext, source); }

static Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  )
{ return Arcadia_Engine_Visuals_PixelBufferNode_create(thread, backendContext, source); }

static Arcadia_Engine_Visuals_EnterPassNode*
Arcadia_Engine_Visuals_NodeFactory_createEnterPassNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_EnterPassNode_create(thread, backendContext); }

static Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_NodeFactory_createTextureNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  )
{ return Arcadia_Engine_Visuals_TextureNode_create(thread, backendContext, source); }

static Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_NodeFactory_createViewportNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ return Arcadia_Engine_Visuals_ViewportNode_create(thread, backendContext); }

Arcadia_Engine_Visuals_NodeFactory*
Arcadia_Engine_Visuals_NodeFactory_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_NodeFactory);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_NodeFactory);
}

Arcadia_Engine_Visuals_CameraNode*
Arcadia_Engine_Visuals_NodeFactory_createCameraNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createCameraNode, self, backendContext); }

Arcadia_Engine_Visuals_FrameBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createFrameBufferNode, self, backendContext); }

Arcadia_Engine_Visuals_EnterPassNode*
Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createEnterPassNode, self, backendContext); }

Arcadia_Engine_Visuals_MaterialNode*
Arcadia_Engine_Visuals_NodeFactory_createMaterialNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MaterialDefinition* source
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createMaterialNode, self, backendContext, source); }

Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_NodeFactory_createMeshNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MeshDefinition* source
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createMeshNode, self, backendContext, source); }

Arcadia_Engine_Visuals_ModelNode*
Arcadia_Engine_Visuals_NodeFactory_createModelNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createModelNode, self, backendContext, source); }

Arcadia_Engine_Visuals_PixelBufferNode*
Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_PixelBufferDefinition* source
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createPixelBufferNode, self, backendContext, source); }

Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_NodeFactory_createTextureNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createTextureNode, self, backendContext, source); }

Arcadia_Engine_Visuals_ViewportNode*
Arcadia_Engine_Visuals_NodeFactory_createViewportNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_NodeFactory* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_NodeFactory, createViewportNode, self, backendContext); }
