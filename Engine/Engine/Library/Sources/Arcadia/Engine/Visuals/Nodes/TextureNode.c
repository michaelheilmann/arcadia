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
#include "Arcadia/Engine/Visuals/Nodes/TextureNode.h"

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Visuals/NodeFactory.h"

#define AddressModeUDirty (1)
#define AddressModeVDirty (2)
#define HeightDirty (4)
#define MagnificationFilterDirty (8)
#define MinificationFilterDirty (16)
#define WidthDirty (32)

#define DirtyBitsAll (AddressModeUDirty | AddressModeVDirty | HeightDirty | MagnificationFilterDirty | MinificationFilterDirty | WidthDirty)

static void
Arcadia_Engine_Visuals_TextureNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static void
Arcadia_Engine_Visuals_TextureNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static void
Arcadia_Engine_Visuals_TextureNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNodeDispatch* self
  );

static void
Arcadia_Engine_Visuals_TextureNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static void
Arcadia_Engine_Visuals_TextureNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_Visuals_TextureNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeUImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeVImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getHeightImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMagnificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMinificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getWidthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_TextureNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_TextureNode_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_TextureNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_TextureNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

static Arcadia_Media_PixelBuffer*
makePixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferDefinition* source
  )
{
  Arcadia_Media_PixelBuffer* target = Arcadia_Media_PixelBuffer_create(thread, 0, source->width, source->height, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source->operations); i < n; ++i) {
    Arcadia_ADL_Reference* e = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueAt(thread, source->operations, i);
    Arcadia_Media_PixelBufferOperation* operation;
    if (!e->definition) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)e->definition, _Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_getType(thread))) {
      operation = (Arcadia_Media_PixelBufferOperation*)Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, (Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition*)e->definition);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)e->definition, _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread))) {
      operation = (Arcadia_Media_PixelBufferOperation*)Arcadia_Media_PixelBufferOperations_Fill_create(thread, (Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*)e->definition);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Media_PixelBufferOperation_apply(thread, operation, target);
  }

  return target;
}

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.TextureNode", Arcadia_Engine_Visuals_TextureNode,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &_typeOperations);

#include <stdio.h>

static void
Arcadia_Engine_Visuals_TextureNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_TextureNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->source = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_TextureDefinition_getType(thread));
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)self->source);
  //
  self->addressModeU = Arcadia_Engine_Visuals_TextureAddressMode_Repeat;
  self->addressModeV = Arcadia_Engine_Visuals_TextureAddressMode_Repeat;
  self->height = 240;
  self->magnificationFilter = Arcadia_Engine_Visuals_TextureFilter_Linear;
  self->minificationFilter = Arcadia_Engine_Visuals_TextureFilter_Linear;
  self->width = 320;
  //
  if (Arcadia_ADL_TextureFilter_None != self->source->magnificationFilter) {
    self->magnificationFilter = (Arcadia_Engine_Visuals_TextureFilter)self->source->magnificationFilter;
  }
  if (Arcadia_ADL_TextureFilter_None != self->source->minificationFilter) {
    self->minificationFilter = (Arcadia_Engine_Visuals_TextureFilter)self->source->minificationFilter;
  }
  if (Arcadia_ADL_TextureAddressMode_None != self->source->addressModeU) {
    self->addressModeU = (Arcadia_Engine_Visuals_TextureAddressMode)self->source->addressModeU;
  }
  if (Arcadia_ADL_TextureAddressMode_None != self->source->addressModeV) {
    self->addressModeV = (Arcadia_Engine_Visuals_TextureAddressMode)self->source->addressModeV;
  }
  //
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  self->pixelBuffer = Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL,
                                                                               (Arcadia_ADL_PixelBufferDefinition*)self->source->pixelBuffer->definition);
  //
  self->textureResource = NULL;
  self->dirtyBits = DirtyBitsAll;
  //
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_TextureNode);
}

static void
Arcadia_Engine_Visuals_TextureNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{
  if (self->backendContext) {
    if (self->textureResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->textureResource);
      self->textureResource = NULL;
    }
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
    self->dirtyBits = DirtyBitsAll;
  }
}

static void
Arcadia_Engine_Visuals_TextureNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNodeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Node*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_Visuals_TextureNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_Visuals_TextureNode_setVisualsBackendContextImpl;
  self->getAddressModeU = (Arcadia_Engine_Visuals_TextureAddressMode(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*)) & Arcadia_Engine_Visuals_TextureNode_getAddressModeUImpl;
  self->getAddressModeV = (Arcadia_Engine_Visuals_TextureAddressMode(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*)) & Arcadia_Engine_Visuals_TextureNode_getAddressModeVImpl;
  self->getHeight = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*)) & Arcadia_Engine_Visuals_TextureNode_getHeightImpl;
  self->getMagnificationFilter = (Arcadia_Engine_Visuals_TextureFilter(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*)) & Arcadia_Engine_Visuals_TextureNode_getMagnificationFilterImpl;
  self->getMinificationFilter = (Arcadia_Engine_Visuals_TextureFilter(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*)) & Arcadia_Engine_Visuals_TextureNode_getMinificationFilterImpl;
  self->getWidth = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureNode*)) & Arcadia_Engine_Visuals_TextureNode_getWidthImpl;
}

static void
Arcadia_Engine_Visuals_TextureNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{
  if (self->pixelBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->pixelBuffer);
  }
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
}

static void
Arcadia_Engine_Visuals_TextureNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)((Arcadia_Engine_Visuals_TextureNode*)self)->pixelBuffer, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, (Arcadia_Engine_Visuals_BackendContext*)enterPassNode->backendContext);
}

static void
Arcadia_Engine_Visuals_TextureNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  Arcadia_BooleanValue backendContextChanged = backendContext != self->backendContext;
  if (backendContextChanged) {
    if (backendContext) {
      Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
    }
    if (self->backendContext) {
      if (self->textureResource) {
        Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->textureResource);
        self->textureResource = NULL;
      }
      Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    }
    self->backendContext = backendContext;
    self->dirtyBits = DirtyBitsAll;
  }
  if (!self->textureResource && self->backendContext) {
    self->textureResource = Arcadia_Engine_Visuals_BackendContext_createTextureResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self->backendContext);
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->textureResource);
  }
  if ((DirtyBitsAll & self->dirtyBits) && self->textureResource) {
    if (AddressModeUDirty & self->dirtyBits) {
      Arcadia_Engine_Visuals_TextureResource_setAddressModeU(thread, self->textureResource, Arcadia_Engine_Visuals_TextureNode_getAddressModeU(thread, (Arcadia_Engine_Visuals_TextureNode*)self));
      self->dirtyBits &= ~AddressModeUDirty;
    }
    if (AddressModeVDirty & self->dirtyBits) {
      Arcadia_Engine_Visuals_TextureResource_setAddressModeV(thread, self->textureResource, Arcadia_Engine_Visuals_TextureNode_getAddressModeU(thread, (Arcadia_Engine_Visuals_TextureNode*)self));
      self->dirtyBits &= ~AddressModeVDirty;
    }
    if (HeightDirty & self->dirtyBits) {
      Arcadia_Engine_Visuals_TextureResource_setHeight(thread, self->textureResource, Arcadia_Engine_Visuals_TextureNode_getHeight(thread, (Arcadia_Engine_Visuals_TextureNode*)self));
      self->dirtyBits &= ~HeightDirty;
    }
    if (MagnificationFilterDirty & self->dirtyBits) {
      Arcadia_Engine_Visuals_TextureResource_setMagnificationFilter(thread, self->textureResource, Arcadia_Engine_Visuals_TextureNode_getMagnificationFilter(thread, (Arcadia_Engine_Visuals_TextureNode*)self));
      self->dirtyBits &= ~MagnificationFilterDirty;
    }
    if (MinificationFilterDirty & self->dirtyBits) {
      Arcadia_Engine_Visuals_TextureResource_setMinificationFilter(thread, self->textureResource, Arcadia_Engine_Visuals_TextureNode_getMinificationFilter(thread, (Arcadia_Engine_Visuals_TextureNode*)self));
      self->dirtyBits &= ~MinificationFilterDirty;
    }
    if (WidthDirty & self->dirtyBits) {
      Arcadia_Engine_Visuals_TextureResource_setWidth(thread, self->textureResource, Arcadia_Engine_Visuals_TextureNode_getWidth(thread, (Arcadia_Engine_Visuals_TextureNode*)self));
      self->dirtyBits &= ~WidthDirty;
    }
    Arcadia_Engine_Visuals_PixelBufferNode* pixelBuffer = ((Arcadia_Engine_Visuals_TextureNode*)self)->pixelBuffer;
    Arcadia_Engine_Visuals_TextureResource_setPixelBuffer(thread, self->textureResource, pixelBuffer->pixelBuffer);
  }
}


static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeUImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ return self->addressModeU; }

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeVImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ return self->addressModeV; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getHeightImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ return self->height; }

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMagnificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ return self->magnificationFilter; }

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMinificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ return self->minificationFilter; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getWidthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ return self->width; }

Arcadia_Engine_Visuals_TextureNode*
Arcadia_Engine_Visuals_TextureNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_TextureDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_TextureNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_TextureNode);
}

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureNode, getAddressModeU, self); }

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureNode_getAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureNode, getAddressModeV, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureNode, getHeight, self); }

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureNode, getMagnificationFilter, self); }

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureNode_getMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureNode, getMinificationFilter, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureNode_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureNode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureNode, getWidth, self); }
