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
#include "Arcadia/Engine/Visuals/Nodes/MeshNode.h"

#include "Arcadia/Engine/Include.h"

#define VerticesDirty (1)
#define MeshAmbientColorDirty (2)

static void
Arcadia_Engine_Visuals_MeshNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  );

static void
Arcadia_Engine_Visuals_MeshNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  );

static void
Arcadia_Engine_Visuals_MeshNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNodeDispatch* self
  );

static void
Arcadia_Engine_Visuals_MeshNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  );

static void
Arcadia_Engine_Visuals_MeshNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_Visuals_MeshNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_MeshNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_MeshNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_MeshNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_MeshNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.MeshNode", Arcadia_Engine_Visuals_MeshNode,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_MeshNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_MeshNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }

  self->dirtyFlags = MeshAmbientColorDirty
                   | VerticesDirty;

  self->constantBufferResource = NULL;
  self->vertexBufferResource = NULL;
  self->vertexBuffer = NULL;
  
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
  
    Arcadia_Media_VertexDescriptorBuilder* vertexDescriptorBuilder = Arcadia_Media_VertexDescriptorBuilder_create(thread);

    Arcadia_List_insertBackObjectReferenceValue
      (
        thread,
        vertexDescriptorBuilder->vertexElementDescriptors,
        (Arcadia_ObjectReferenceValue)
        Arcadia_Media_VertexElementDescriptor_create
          (
            thread,
            sizeof(Arcadia_Real32Value) * 0,
            Arcadia_Media_VertexElementSemantics_PositionXYZ,
            Arcadia_Media_VertexElementSyntactics_Real32Real32Real32
          )
      );
    Arcadia_List_insertBackObjectReferenceValue
      (
        thread,
        vertexDescriptorBuilder->vertexElementDescriptors,
        Arcadia_Media_VertexElementDescriptor_create
          (
            thread,
            sizeof(Arcadia_Real32Value) * 3,
            Arcadia_Media_VertexElementSemantics_AmbientRGBA,
            Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32
          )
      );
    Arcadia_List_insertBackObjectReferenceValue
      (
        thread,
        vertexDescriptorBuilder->vertexElementDescriptors,
        (Arcadia_ObjectReferenceValue)
        Arcadia_Media_VertexElementDescriptor_create
          (
            thread,
            sizeof(Arcadia_Real32Value) * 7,
            Arcadia_Media_VertexElementSemantics_AmbientUV,
            Arcadia_Media_VertexElementSyntactics_Real32Real32
          )
      );

    Arcadia_ADL_MeshDefinition* source = (Arcadia_ADL_MeshDefinition*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_MeshDefinition_getType(thread));
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)source);
    Arcadia_ADL_ColorDefinition* ambientColorDefinition = (Arcadia_ADL_ColorDefinition*)source->ambientColor->definition;
    self->source = source;
    // TODO:
    // Each mesh creates its own material node for an ADL material definition.
    // This is wasteful.
    // Meshes using the same ADL material definition should use the material node.
    self->ambientColor = Arcadia_Math_Color4Real32_create4(thread, ambientColorDefinition->red / 255.f,
                                                                   ambientColorDefinition->green / 255.f,
                                                                   ambientColorDefinition->blue / 255.f,
                                                                   1.f);
    
    self->vertexBuffer = Arcadia_Media_VertexBuffer_create(thread, Arcadia_Media_VertexDescriptorBuilder_build(thread, vertexDescriptorBuilder), Arcadia_ADL_MeshDefinition_getNumberOfVertices(thread, source));

    Arcadia_RuntimeByteArray* vertexXZZ = Arcadia_ADL_MeshDefinition_getVertexPositions(thread, source);
    Arcadia_Real32Value const* vertexYZBytes = (Arcadia_Real32Value const*)Arcadia_RuntimeByteArray_getBytes(thread, vertexXZZ);

    Arcadia_RuntimeByteArray* vertexAmbientRGBA = Arcadia_ADL_MeshDefinition_getVertexAmbientColors(thread, source);
    Arcadia_Real32Value const* vertexAmbientRGBBytes = (Arcadia_Real32Value const*)Arcadia_RuntimeByteArray_getBytes(thread, vertexAmbientRGBA);

    Arcadia_RuntimeByteArray* vertexAmbientUV = Arcadia_ADL_MeshDefinition_getVertexAmbientTextureCoordinates(thread, source);
    Arcadia_Real32Value const* vertexAmbientUVBytes = (Arcadia_Real32Value const*)Arcadia_RuntimeByteArray_getBytes(thread, vertexAmbientUV);

    Arcadia_ByteArrayBuilder* temporary = Arcadia_ByteArrayBuilder_create(thread);
    for (Arcadia_SizeValue i = 0, n = self->vertexBuffer->numberOfVertices; i < n; ++i) {
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, temporary, &vertexYZBytes[i * 3], sizeof(Arcadia_Real32Value) * 3);
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, temporary, &vertexAmbientRGBBytes[i * 4], sizeof(Arcadia_Real32Value) * 4);
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, temporary, &vertexAmbientUVBytes[i * 2], sizeof(Arcadia_Real32Value) * 2);
    }
    Arcadia_Memory_copy(thread, self->vertexBuffer->vertices, Arcadia_ByteArrayBuilder_getBytes(thread, temporary),
                                                              Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, temporary));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    Arcadia_Thread_jump(thread);
  }
                                              
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_MeshNode);
}

static void
Arcadia_Engine_Visuals_MeshNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  )
{
#if 0
  if (self->meshSource.vertices) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->meshSource.vertices);
    self->meshSource.vertices = NULL;
  }
#endif
  if (self->backendContext) {
    if (self->vertexBufferResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->vertexBufferResource);
      self->vertexBufferResource = NULL;
    }
    if (self->constantBufferResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->constantBufferResource);
      self->constantBufferResource = NULL;
    }

    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_Visuals_MeshNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNodeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_Visuals_MeshNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_MeshNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_Visuals_MeshNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  )
{
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
  if (self->vertexBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexBuffer);
  }
  if (self->ambientColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ambientColor);
  }
#if 0
  if (self->meshSource.vertexDescriptor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->meshSource.vertexDescriptor);
  }
#endif
}

static void
Arcadia_Engine_Visuals_MeshNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  // (1) Set the backend context.
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, (Arcadia_Engine_Visuals_BackendContext*)enterPassNode->backendContext);
  // (2) Setup resources.
  if (self->backendContext) {
    if (!self->vertexBufferResource) {
      Arcadia_Engine_Visuals_BackendContext* backendContext = self->backendContext;
      // (2.1) Create the vertex buffer resource.
      self->vertexBufferResource = Arcadia_Engine_Visuals_BackendContext_createVertexBufferResource(thread, backendContext);
      Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->vertexBufferResource);
    }
    if (self->dirtyFlags & VerticesDirty) {
      Arcadia_Engine_Visuals_VertexBufferResource_setData
        (
          thread,
          self->vertexBufferResource,
          ((Arcadia_Engine_Visuals_MeshNode*)self)->vertexBuffer->vertexDescriptor,
          ((Arcadia_Engine_Visuals_MeshNode*)self)->vertexBuffer->numberOfVertices,
          (void*)((Arcadia_Engine_Visuals_MeshNode*)self)->vertexBuffer->vertices,
          ((Arcadia_Engine_Visuals_MeshNode*)self)->vertexBuffer->numberOfVertices * ((Arcadia_Engine_Visuals_MeshNode*)self)->vertexBuffer->vertexDescriptor->stride
        );
      self->dirtyFlags &= ~VerticesDirty;
    }
    if (!self->constantBufferResource) {
      Arcadia_Engine_Visuals_BackendContext* backendContext = self->backendContext;
      // (2.2) Create the constant buffer resource.
      self->constantBufferResource = Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource(thread, backendContext);
      Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->constantBufferResource);
    }
    if (self->dirtyFlags & MeshAmbientColorDirty) {
      Arcadia_Engine_Visuals_ConstantBufferResource_clear(thread, self->constantBufferResource);
      // @todo We need to ability to do sub-range updates: There is no need to rewrite every value if only one value is dirty.
      Arcadia_Engine_Visuals_ConstantBufferResource_writeColor4Real32(thread, self->constantBufferResource, self->ambientColor);
      self->dirtyFlags &= ~MeshAmbientColorDirty;
    }
  }
}

static void
Arcadia_Engine_Visuals_MeshNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) {
    // Only change something if the backend context changes.
    return;
  }
  if (backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  }
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  }
  if (self->constantBufferResource) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->constantBufferResource);
    self->constantBufferResource = NULL;
  }
  if (self->vertexBufferResource) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->vertexBufferResource);
    self->vertexBufferResource = NULL;
  }
  self->backendContext = backendContext;
}

Arcadia_Engine_Visuals_MeshNode*
Arcadia_Engine_Visuals_MeshNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_MeshDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_MeshNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_MeshNode);
}

void
Arcadia_Engine_Visuals_MeshNode_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_SizeValue* numberOfVertices
  )
{ *numberOfVertices = self->vertexBuffer->numberOfVertices; }

void
Arcadia_Engine_Visuals_MeshNode_setAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self,
    Arcadia_Math_Color4Real32* ambientColor
  )
{
  Arcadia_Math_Color4Real32_assign(thread, self->ambientColor, ambientColor);
}

Arcadia_Math_Color4Real32*
Arcadia_Engine_Visuals_MeshNode_getAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MeshNode* self
  )
{
  return Arcadia_Math_Color4Real32_create4v(thread, self->ambientColor->components);
}
