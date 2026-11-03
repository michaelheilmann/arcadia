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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/Nodes/PanelNode.h"

#include "Arcadia/Engine/UI/Renderable.h"
#include "Arcadia/Engine/Include.h"

static void
Arcadia_Engine_UI_PanelNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self
  );

static void
Arcadia_Engine_UI_PanelNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self
  );

static void
Arcadia_Engine_UI_PanelNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNodeDispatch* self
  );

static void
Arcadia_Engine_UI_PanelNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  );

static void
Arcadia_Engine_UI_PanelNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self
  );

static void
Arcadia_Engine_UI_PanelNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_PanelNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_PanelNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_PanelNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_UI_PanelNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_PanelNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_PanelNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.PanelNode",
    Arcadia_Engine_UI_PanelNode,
    u8"Arcadia.Engine.UI.WidgetNode",
    Arcadia_Engine_UI_WidgetNode,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_PanelNode_initializeRenderableDispatchImpl
    }
  )

static void
Arcadia_Engine_UI_PanelNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_PanelNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }

  // (1) get the node factory and the model definition
  Arcadia_ADL_ModelDefinition* source = (Arcadia_ADL_ModelDefinition*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_ModelDefinition_getType(thread));

  // (2) link the model definition
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)source);
  self->source = source;

  // (3) create the geometry, the mesh node and the material node
  // `source->mesh->definition` is an `Arcadia.ADL.MeshGenerator` because `Arcadia_ADL_ModelDefinition_linkImpl` has verified it.
  Arcadia_ADL_MeshGenerator* meshGenerator = (Arcadia_ADL_MeshGenerator*)source->mesh->definition;
  Arcadia_ADL_MaterialDefinition* materialDefinition = (Arcadia_ADL_MaterialDefinition*)source->material->definition;

  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, meshGenerator);
  self->mesh = Arcadia_Engine_Visuals_NodeFactory_createMeshNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, geometry);
  self->material = Arcadia_Engine_Visuals_NodeFactory_createMaterialNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, materialDefinition);

  //
  if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->backendContext = NULL;
  } else {
    self->backendContext = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  //
  self->modelResource = NULL;
  self->backgroundGeometryValid = Arcadia_BooleanValue_False;
  self->lastBackgroundBorderWidth = 0;
  //

  Arcadia_LeaveConstructor(Arcadia_Engine_UI_PanelNode);
}

static void
Arcadia_Engine_UI_PanelNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self
  )
{
  if (self->backendContext) {
    if (self->modelResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
      self->modelResource = NULL;
    }
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_UI_PanelNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_Visuals_EnterPassNode*)) & Arcadia_Engine_UI_PanelNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*)) & Arcadia_Engine_UI_PanelNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_UI_PanelNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{
  self->render = &Arcadia_Engine_UI_PanelNode_renderInterfaceImpl;
}

static void
Arcadia_Engine_UI_PanelNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_UI_PanelNode_renderImpl(thread, (Arcadia_Engine_UI_PanelNode*)self, enterPassNode);
}

static void
Arcadia_Engine_UI_PanelNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self
  )
{
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
  if (self->mesh) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->mesh);
  }
  if (self->material) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->material);
  }
}

static void
Arcadia_Engine_UI_PanelNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  // (1) Render the material dependency.
  Arcadia_Engine_Visuals_MaterialNode* materialNode = (Arcadia_Engine_Visuals_MaterialNode*)((Arcadia_Engine_UI_PanelNode*)self)->material;
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)materialNode, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  // (2) Render the mesh dependency.
  Arcadia_Engine_Visuals_MeshNode* meshNode = (Arcadia_Engine_Visuals_MeshNode*)((Arcadia_Engine_UI_PanelNode*)self)->mesh;
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)meshNode, (Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode);
  // (3) Create the model resource.
  if (self->backendContext) {
    if (!self->modelResource) {
      self->modelResource = Arcadia_Engine_Visuals_BackendContext_createModelResource(thread, (Arcadia_Engine_Visuals_BackendContext*)self->backendContext,
                                                                                              meshNode->constantBufferResource,
                                                                                              meshNode->vertexBufferResource,
                                                                                              materialNode->materialResource);
      Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
    }
  }
  // (4) Perform actual render to frame buffer.
  if (self->backendContext) {
    // (4.2) Render the mesh resource.
    Arcadia_Engine_Visuals_Resource_render(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource,
                                                   ((Arcadia_Engine_Visuals_EnterPassNode*)enterPassNode)->enterPassResource);
  }
}

static void
Arcadia_Engine_UI_PanelNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self,
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
  if (self->modelResource) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
    self->modelResource = NULL;
  }
  self->backendContext = backendContext;
}

static void
writeBackgroundQuadPositions
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue vertexIndex,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  )
{
  Arcadia_Real32Value const positions[6][3] = {
    { left, bottom, 0.f }, { right, bottom, 0.f }, { left, top, 0.f },
    { right, bottom, 0.f }, { right, top, 0.f }, { left, top, 0.f },
  };
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    Arcadia_SizeValue j = (vertexIndex + i) * 9;
    vertices[j + 0] = positions[i][0];
    vertices[j + 1] = positions[i][1];
    vertices[j + 2] = positions[i][2];
  }
}

void
Arcadia_Engine_UI_PanelNode_updateBackgroundGeometry
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PanelNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_Natural32Value borderWidth
  )
{
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)self;
  if (self->backgroundGeometryValid && self->lastBackgroundBorderWidth == borderWidth &&
      widget->left == left && widget->bottom == bottom &&
      widget->width == width && widget->height == height) return;
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, widget, left, bottom, width, height);
  Arcadia_Real32Value x0 = (Arcadia_Real32Value)left;
  Arcadia_Real32Value y0 = (Arcadia_Real32Value)bottom;
  Arcadia_Real32Value x1 = x0 + (Arcadia_Real32Value)width;
  Arcadia_Real32Value y1 = y0 + (Arcadia_Real32Value)height;
  Arcadia_Real32Value border = (Arcadia_Real32Value)borderWidth;
  Arcadia_Real32Value maxBorder = (width < height ? (Arcadia_Real32Value)width : (Arcadia_Real32Value)height) * 0.5f;
  if (border > maxBorder) border = maxBorder;
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_ByteArray_getMutableBytes(thread,
    Arcadia_Media_VertexBuffer_getVertexBytes(thread,
      Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->mesh)));
  writeBackgroundQuadPositions(vertices, 0, x0 + border, y0 + border, x1 - border, y1 - border);
  if (border > 0.f) {
    writeBackgroundQuadPositions(vertices, 6, x0, y0, x1, y0 + border);
    writeBackgroundQuadPositions(vertices, 12, x0, y1 - border, x1, y1);
    writeBackgroundQuadPositions(vertices, 18, x0, y0 + border, x0 + border, y1 - border);
    writeBackgroundQuadPositions(vertices, 24, x1 - border, y0 + border, x1, y1 - border);
  }
  Arcadia_Engine_Visuals_MeshNode_markVerticesDirty(thread, self->mesh);
  self->lastBackgroundBorderWidth = borderWidth;
  self->backgroundGeometryValid = Arcadia_BooleanValue_True;
}

Arcadia_Engine_UI_PanelNode*
Arcadia_Engine_UI_PanelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_ModelDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_PanelNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_UI_PanelNode);
}
