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
#include "Arcadia/Engine/UI/Nodes/ScrollPanelNode.h"

#include "Arcadia/Engine/UI/DefinitionFactory.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/UI/TextUtilities.h"

#define VerticesDirty (1)

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNodeDispatch* self
  );

static void
initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  );

static void
initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  );

static void
initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  );

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  );

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  );

static void
initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  );

static void
initializeScrollableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollableDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  );

static Arcadia_Natural64Value
getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_Object*
getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  );

static void
addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

static void
removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static Arcadia_BooleanValue
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
scrollByImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value deltaX,
    Arcadia_Integer32Value deltaY
  );

static void
scrollToImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );

static Arcadia_Integer32Value
getMaxScrollY
  (
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  Arcadia_Integer32Value height = ((Arcadia_Engine_UI_WidgetNode*)self)->height;
  return self->contentHeight > (Arcadia_Natural32Value)height ? (Arcadia_Integer32Value)(self->contentHeight - (Arcadia_Natural32Value)height) : 0;
}

static Arcadia_Real32Value
getScrollbarThumbHeight
  (
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  Arcadia_Integer32Value height = ((Arcadia_Engine_UI_WidgetNode*)self)->height;
  if (self->contentHeight <= (Arcadia_Natural32Value)height) return (Arcadia_Real32Value)height;
  Arcadia_Real32Value thumbHeight = (Arcadia_Real32Value)height * (Arcadia_Real32Value)height / (Arcadia_Real32Value)self->contentHeight;
  return thumbHeight < 24.f ? 24.f : thumbHeight;
}

static Arcadia_Real32Value
getScrollbarThumbOffset
  (
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  Arcadia_Integer32Value maxScrollY = getMaxScrollY(self);
  if (!maxScrollY) return 0.f;
  Arcadia_Real32Value thumbHeight = getScrollbarThumbHeight(self);
  return ((Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height - thumbHeight) * ((Arcadia_Real32Value)self->scrollY / (Arcadia_Real32Value)maxScrollY);
}

static Arcadia_BooleanValue
hitTestScrollbar
  (
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  return getMaxScrollY(self) > 0 &&
         x >= ((Arcadia_Engine_UI_WidgetNode*)self)->left + ((Arcadia_Engine_UI_WidgetNode*)self)->width - 12 && x < ((Arcadia_Engine_UI_WidgetNode*)self)->left + ((Arcadia_Engine_UI_WidgetNode*)self)->width &&
         y >= ((Arcadia_Engine_UI_WidgetNode*)self)->bottom && y < ((Arcadia_Engine_UI_WidgetNode*)self)->bottom + ((Arcadia_Engine_UI_WidgetNode*)self)->height;
}

static void
scrollToScrollbarThumbOffset
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Integer32Value thumbOffset
  )
{
  Arcadia_Integer32Value maxScrollY = getMaxScrollY(self);
  if (!maxScrollY) return;
  Arcadia_Real32Value thumbHeight = getScrollbarThumbHeight(self);
  Arcadia_Real32Value trackRange = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height - thumbHeight;
  if (trackRange <= 0.f) return;
  Arcadia_Integer32Value y = (Arcadia_Integer32Value)(((Arcadia_Real32Value)thumbOffset / trackRange) * (Arcadia_Real32Value)maxScrollY + 0.5f);
  scrollToImpl(thread, (Arcadia_Object*)self, self->scrollX, y);
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.ScrollPanelNode", Arcadia_Engine_UI_ScrollPanelNode,
    u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode, &_typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Container_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeContainerDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeFocusableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeHitTestableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeKeyboardInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializePointerInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeRenderableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Scrollable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ScrollableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeScrollableDispatchImpl }
  )

static Arcadia_BooleanValue
hasInterface
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_InterfaceType* interfaceType
  )
{
  return NULL != Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)Arcadia_Object_getType(thread, object), interfaceType);
}

static void
setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Object* child,
    Arcadia_BooleanValue hovered
  )
{
  if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ButtonNode_getType(thread))) {
    Arcadia_Engine_UI_ButtonNode_setHovered(thread, (Arcadia_Engine_UI_ButtonNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread))) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setHovered(thread, (Arcadia_Engine_UI_SegmentedChoiceNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SliderNode_getType(thread))) {
    Arcadia_Engine_UI_SliderNode_setHovered(thread, (Arcadia_Engine_UI_SliderNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ToggleNode_getType(thread))) {
    Arcadia_Engine_UI_ToggleNode_setHovered(thread, (Arcadia_Engine_UI_ToggleNode*)child, hovered);
  }
}

static void
writeQuad
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue index,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top,
    Arcadia_Real32Value red,
    Arcadia_Real32Value green,
    Arcadia_Real32Value blue,
    Arcadia_Real32Value alpha
  )
{
  Arcadia_Real32Value p[6][3] = {{left,bottom,0.f},{right,bottom,0.f},{left,top,0.f},{right,bottom,0.f},{right,top,0.f},{left,top,0.f}};
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    Arcadia_SizeValue j = (index + i) * 9;
    vertices[j] = p[i][0]; vertices[j + 1] = p[i][1]; vertices[j + 2] = p[i][2];
    vertices[j + 3] = red; vertices[j + 4] = green; vertices[j + 5] = blue; vertices[j + 6] = alpha;
    vertices[j + 7] = 0.f; vertices[j + 8] = 0.f;
  }
}

static void
updateScrollbarVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->scrollbarMesh);
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer);
  Arcadia_Real32Value left = (Arcadia_Real32Value)(self->scrollX + ((Arcadia_Engine_UI_WidgetNode*)self)->width - 10);
  Arcadia_Real32Value right = (Arcadia_Real32Value)(self->scrollX + ((Arcadia_Engine_UI_WidgetNode*)self)->width - 3);
  Arcadia_Real32Value bottom = (Arcadia_Real32Value)self->scrollY;
  Arcadia_Real32Value top = (Arcadia_Real32Value)(self->scrollY + ((Arcadia_Engine_UI_WidgetNode*)self)->height);
  writeQuad(vertices, 0, left, bottom, right, top, 0.18f, 0.20f, 0.27f, 1.f);
  if (self->contentHeight <= (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height) {
    writeQuad(vertices, 6, left + 1.f, bottom + 1.f, right - 1.f, top - 1.f, 0.50f, 0.55f, 0.68f, 1.f);
  } else {
    Arcadia_Real32Value thumbHeight = getScrollbarThumbHeight(self);
    Arcadia_Real32Value thumbBottom = bottom + getScrollbarThumbOffset(self);
    writeQuad(vertices, 6, left + 1.f, thumbBottom, right - 1.f, thumbBottom + thumbHeight, 0.66f, 0.72f, 0.86f, 1.f);
  }
  self->scrollbarMesh->dirtyFlags |= VerticesDirty;
}

static Arcadia_Object*
hitTestChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Integer32Value childX = x - ((Arcadia_Engine_UI_WidgetNode*)self)->left + self->scrollX;
  Arcadia_Integer32Value childY = y - ((Arcadia_Engine_UI_WidgetNode*)self)->bottom + self->scrollY;
  for (Arcadia_SizeValue i = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i > 0; --i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i - 1);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_HitTestable_getType(thread)) &&
        Arcadia_Engine_UI_HitTestable_hitTest(thread, child, childX, childY)) {
      return child;
    }
  }
  return NULL;
}

static void
focusChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Object* child
  )
{
  if (self->focusedChild == child) return;
  if (self->focusedChild && hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
    Arcadia_Engine_UI_Focusable_blur(thread, self->focusedChild);
  }
  self->focusedChild = child;
  if (self->focusedChild && hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
    Arcadia_Engine_UI_Focusable_focus(thread, self->focusedChild);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_ScrollPanelNode);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 3);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 1);
  ((Arcadia_Engine_UI_WidgetNode*)self)->left = self->left;
  ((Arcadia_Engine_UI_WidgetNode*)self)->bottom = self->bottom;
  ((Arcadia_Engine_UI_WidgetNode*)self)->width = (Arcadia_Integer32Value)self->width;
  ((Arcadia_Engine_UI_WidgetNode*)self)->height = (Arcadia_Integer32Value)self->height;
  self->contentWidth = (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width;
  self->contentHeight = (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height;
  self->scrollX = 0;
  self->scrollY = 0;
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  self->viewportNode = Arcadia_Engine_Visuals_NodeFactory_createViewportNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL);
  self->cameraNode = Arcadia_Engine_Visuals_NodeFactory_createCameraNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL);
  self->generatedDefinitions = Arcadia_ADL_Definitions_create(thread);
  self->scrollbarModelResource = NULL;
  self->lastCanvasHeight = 0.f;

  Arcadia_String* colorName = Arcadia_String_createFromCxxString(thread, u8"UI.ScrollPanel.Scrollbar.Generated.Color");
  Arcadia_String* meshName = Arcadia_String_createFromCxxString(thread, u8"UI.ScrollPanel.Scrollbar.Generated.Mesh");
  Arcadia_String* materialName = Arcadia_String_createFromCxxString(thread, u8"UI.ScrollPanel.Scrollbar.Generated.Material");
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, u8"UI.ScrollPanel.Scrollbar.Generated.Texture");
  Arcadia_String* pixelBufferName = Arcadia_String_createFromCxxString(thread, u8"UI.ScrollPanel.Scrollbar.Generated.PixelBuffer");
  Arcadia_String* fillName = Arcadia_String_createFromCxxString(thread, u8"UI.ScrollPanel.Scrollbar.Generated.Fill");
  Arcadia_ADL_ColorDefinition* color = Arcadia_ADL_ColorDefinition_create(thread, self->generatedDefinitions, colorName, 255, 255, 255);
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fill = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, self->generatedDefinitions, fillName, colorName);
  Arcadia_ADL_PixelBufferDefinition* pixelBuffer = Arcadia_ADL_PixelBufferDefinition_create(thread, self->generatedDefinitions, pixelBufferName, 1, 1);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffer->operations, (Arcadia_Object*)Arcadia_ADL_Reference_create(thread, self->generatedDefinitions, fillName));
  Arcadia_ADL_TextureDefinition* texture = Arcadia_ADL_TextureDefinition_create(thread, self->generatedDefinitions, textureName, pixelBufferName, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureAddressMode_None, Arcadia_ADL_TextureAddressMode_None);
  Arcadia_SizeValue n = 12;
  Arcadia_Real32Value* positions = Arcadia_Memory_allocateUnmanaged(thread, 3 * n * sizeof(Arcadia_Real32Value));
  Arcadia_Real32Value* colors = Arcadia_Memory_allocateUnmanaged(thread, 4 * n * sizeof(Arcadia_Real32Value));
  Arcadia_Real32Value* uvs = Arcadia_Memory_allocateUnmanaged(thread, 2 * n * sizeof(Arcadia_Real32Value));
  for (Arcadia_SizeValue i = 0; i < 3 * n; ++i) positions[i] = 0.f;
  for (Arcadia_SizeValue i = 0; i < 4 * n; ++i) colors[i] = 1.f;
  for (Arcadia_SizeValue i = 0; i < 2 * n; ++i) uvs[i] = 0.f;
  Arcadia_ADL_MeshDefinition* mesh = Arcadia_ADL_MeshDefinition_create(thread, self->generatedDefinitions, meshName, n,
                                                                      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)positions, 3 * n * sizeof(Arcadia_Real32Value)),
                                                                      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)colors, 4 * n * sizeof(Arcadia_Real32Value)),
                                                                      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)uvs, 2 * n * sizeof(Arcadia_Real32Value)),
                                                                      colorName);
  Arcadia_ADL_MaterialDefinition* material = Arcadia_ADL_MaterialDefinition_create(thread, self->generatedDefinitions, materialName, Arcadia_ADL_MaterialType_BlinnPhong, Arcadia_ADL_AmbientColorSource_Vertex, textureName, Arcadia_ADL_BlendFunction_None, Arcadia_ADL_BlendFunction_None);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)color);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)fill);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)pixelBuffer);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)texture);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)mesh);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)material);
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, (Arcadia_ADL_MeshGenerator*)mesh);
  self->scrollbarMesh = Arcadia_Engine_Visuals_NodeFactory_createMeshNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, geometry);
  self->scrollbarMaterial = Arcadia_Engine_Visuals_NodeFactory_createMaterialNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, material);
  updateScrollbarVertices(thread, self);
  Arcadia_Memory_deallocateUnmanaged(thread, positions);
  Arcadia_Memory_deallocateUnmanaged(thread, colors);
  Arcadia_Memory_deallocateUnmanaged(thread, uvs);
  self->children = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->hoveredChild = NULL;
  self->pressedChild = NULL;
  self->focusedChild = NULL;
  self->focused = Arcadia_BooleanValue_False;
  self->draggingScrollbar = Arcadia_BooleanValue_False;
  self->scrollbarDragOffset = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_ScrollPanelNode);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  if (self->scrollbarModelResource) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->scrollbarModelResource);
    self->scrollbarModelResource = NULL;
  }
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_Visuals_EnterPassNode*))&renderImpl;
}

static void
initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &getChildCountImpl;
  self->getChildAt = &getChildAtImpl;
  self->addChild = &addChildImpl;
  self->removeChild = &removeChildImpl;
}

static void
initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{
  self->focus = &focusImpl;
  self->blur = &blurImpl;
  self->isFocused = &isFocusedImpl;
}

static void
initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{
  self->hitTest = &hitTestImpl;
}

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{
  self->handleKeyboardKeyEvent = &handleKeyboardKeyEventImpl;
}

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &handleMousePointerEventImpl; self->handleMouseButtonEvent = &handleMouseButtonEventImpl;
}

static void
initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{
  self->render = &renderInterfaceImpl;
}

static void
initializeScrollableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollableDispatch* self
  )
{
  self->scrollBy = &scrollByImpl; self->scrollTo = &scrollToImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self
  )
{
  if (self->viewportNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  if (self->cameraNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  if (self->generatedDefinitions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->generatedDefinitions);
  if (self->scrollbarMesh) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollbarMesh);
  if (self->scrollbarMaterial) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollbarMaterial);
  if (self->children) Arcadia_Object_visit(thread, (Arcadia_Object*)self->children);
  if (self->hoveredChild) Arcadia_Object_visit(thread, self->hoveredChild);
  if (self->pressedChild) Arcadia_Object_visit(thread, self->pressedChild);
  if (self->focusedChild) Arcadia_Object_visit(thread, self->focusedChild);
}

static Arcadia_Natural64Value
getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_UI_ScrollPanelNode*)self)->children);
}

static Arcadia_Object*
getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  )
{
  return Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_Engine_UI_ScrollPanelNode*)self)->children, index);
}

static void
addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_List_insertBackObjectReferenceValue(thread, ((Arcadia_Engine_UI_ScrollPanelNode*)self)->children, child);
}

static void
removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* panel = (Arcadia_Engine_UI_ScrollPanelNode*)self;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->children); i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, panel->children, i) == child) {
      Arcadia_List_removeAt(thread, panel->children, i, 1);
      if (panel->hoveredChild == child) panel->hoveredChild = NULL;
      if (panel->pressedChild == child) panel->pressedChild = NULL;
      if (panel->focusedChild == child) panel->focusedChild = NULL;
      return;
    }
  }
}

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ((Arcadia_Engine_UI_ScrollPanelNode*)self)->focused = Arcadia_BooleanValue_True;
}

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ((Arcadia_Engine_UI_ScrollPanelNode*)self)->focused = Arcadia_BooleanValue_False; focusChild(thread, (Arcadia_Engine_UI_ScrollPanelNode*)self, NULL);
}

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return ((Arcadia_Engine_UI_ScrollPanelNode*)self)->focused;
}

static Arcadia_BooleanValue
hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* panel = (Arcadia_Engine_UI_ScrollPanelNode*)self;
  return x >= ((Arcadia_Engine_UI_WidgetNode*)panel)->left && x < ((Arcadia_Engine_UI_WidgetNode*)panel)->left + ((Arcadia_Engine_UI_WidgetNode*)panel)->width &&
         y >= ((Arcadia_Engine_UI_WidgetNode*)panel)->bottom && y < ((Arcadia_Engine_UI_WidgetNode*)panel)->bottom + ((Arcadia_Engine_UI_WidgetNode*)panel)->height;
}

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* panel = (Arcadia_Engine_UI_ScrollPanelNode*)self;
  if (panel->focusedChild && hasInterface(thread, panel->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread)) &&
      Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, panel->focusedChild, event)) {
    return Arcadia_BooleanValue_True;
  }
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) != Arcadia_Engine_Input_KeyboardKeyAction_Released) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
  if (key == Arcadia_Engine_Input_KeyboardKey_UpArrow) { Arcadia_Engine_UI_Scrollable_scrollBy(thread, self, 0, 24); return Arcadia_BooleanValue_True; }
  if (key == Arcadia_Engine_Input_KeyboardKey_DownArrow) { Arcadia_Engine_UI_Scrollable_scrollBy(thread, self, 0, -24); return Arcadia_BooleanValue_True; }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* panel = (Arcadia_Engine_UI_ScrollPanelNode*)self;
  Arcadia_Integer32Value y = (Arcadia_Integer32Value)panel->lastCanvasHeight - event->y;
  if (panel->draggingScrollbar && event->action == Arcadia_Engine_Input_MousePointerAction_Moved) {
    scrollToScrollbarThumbOffset(thread, panel, y - ((Arcadia_Engine_UI_WidgetNode*)panel)->bottom - panel->scrollbarDragOffset);
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Object* child = hitTestChild(thread, panel, event->x, y);
  if (panel->hoveredChild != child) {
    setHovered(thread, panel->hoveredChild, Arcadia_BooleanValue_False);
    setHovered(thread, child, Arcadia_BooleanValue_True);
  }
  panel->hoveredChild = child;
  Arcadia_Object* receiver = panel->pressedChild ? panel->pressedChild : child;
  if (receiver && hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread))) {
    return
    Arcadia_Engine_UI_PointerInput_handleMousePointerEvent
      (
        thread,
        receiver,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* panel = (Arcadia_Engine_UI_ScrollPanelNode*)self;
  Arcadia_Integer32Value y = (Arcadia_Integer32Value)panel->lastCanvasHeight - event->y;
  if (hitTestScrollbar(panel, event->x, y) || panel->draggingScrollbar) {
    if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
      Arcadia_Real32Value thumbOffset = getScrollbarThumbOffset(panel);
      Arcadia_Real32Value thumbHeight = getScrollbarThumbHeight(panel);
      Arcadia_Integer32Value mouseOffset = y - ((Arcadia_Engine_UI_WidgetNode*)panel)->bottom;
      if ((Arcadia_Real32Value)mouseOffset >= thumbOffset && (Arcadia_Real32Value)mouseOffset <= thumbOffset + thumbHeight) {
        panel->scrollbarDragOffset = mouseOffset - (Arcadia_Integer32Value)thumbOffset;
      } else {
        panel->scrollbarDragOffset = (Arcadia_Integer32Value)(thumbHeight / 2.f);
        scrollToScrollbarThumbOffset(thread, panel, mouseOffset - panel->scrollbarDragOffset);
      }
      panel->draggingScrollbar = Arcadia_BooleanValue_True;
      return Arcadia_BooleanValue_True;
    }
    if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
      panel->draggingScrollbar = Arcadia_BooleanValue_False;
      return Arcadia_BooleanValue_True;
    }
  }
  Arcadia_Object* child = hitTestChild(thread, panel, event->x, y);
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    panel->pressedChild = child;
    if (child) focusChild(thread, panel, child);
  }
  Arcadia_Object* receiver = panel->pressedChild ? panel->pressedChild : child;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
    panel->pressedChild = NULL;
  }
  if (receiver && hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread))) {
    return
    Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent
      (
        thread,
        receiver,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static void
renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  renderImpl(thread, (Arcadia_Engine_UI_ScrollPanelNode*)self, enterPassNode);
}

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_Visuals_ViewportNode* previousViewportNode = enterPassNode->viewportNode;
  Arcadia_Engine_Visuals_CameraNode* previousCameraNode = enterPassNode->cameraNode;
  Arcadia_Real32Value canvasWidth;
  Arcadia_Real32Value canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_getCanvasSize(thread, enterPassNode->viewportNode, &canvasWidth, &canvasHeight);
  self->lastCanvasHeight = canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, canvasWidth, canvasHeight);
  Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNode,
                                                                   (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->left / canvasWidth,
                                                                   (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->bottom / canvasHeight,
                                                                   (Arcadia_Real32Value)(((Arcadia_Engine_UI_WidgetNode*)self)->left + ((Arcadia_Engine_UI_WidgetNode*)self)->width) / canvasWidth,
                                                                   (Arcadia_Real32Value)(((Arcadia_Engine_UI_WidgetNode*)self)->bottom + ((Arcadia_Engine_UI_WidgetNode*)self)->height) / canvasHeight);
  Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNode, Arcadia_Math_Color4Real32_create4(thread, self->focused ? 0.15f : 0.11f, self->focused ? 0.17f : 0.13f, self->focused ? 0.23f : 0.18f, 1.f));
  Arcadia_Engine_Visuals_ViewportNode_setClearColorBuffer(thread, self->viewportNode, Arcadia_BooleanValue_True);
  Arcadia_Engine_Visuals_ViewportNode_setClearDepthBuffer(thread, self->viewportNode, Arcadia_BooleanValue_False);

  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix,
                                                         (Arcadia_Real32Value)self->scrollX,
                                                         (Arcadia_Real32Value)(self->scrollX + ((Arcadia_Engine_UI_WidgetNode*)self)->width),
                                                         (Arcadia_Real32Value)self->scrollY,
                                                         (Arcadia_Real32Value)(self->scrollY + ((Arcadia_Engine_UI_WidgetNode*)self)->height),
                                                         -1.f,
                                                         +1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, enterPassNode, self->cameraNode);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, enterPassNode);

  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i < n; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Renderable_getType(thread))) {
      Arcadia_Engine_UI_Renderable_render(thread, child, enterPassNode);
    }
  }
  if (self->contentHeight > (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height || self->contentWidth > (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width) {
    updateScrollbarVertices(thread, self);
    Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->scrollbarMaterial, enterPassNode);
    Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->scrollbarMesh, enterPassNode);
    if (enterPassNode->backendContext && !self->scrollbarModelResource) {
      self->scrollbarModelResource = Arcadia_Engine_Visuals_BackendContext_createModelResource(thread, enterPassNode->backendContext, self->scrollbarMesh->constantBufferResource, self->scrollbarMesh->vertexBufferResource, self->scrollbarMaterial->materialResource);
      Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->scrollbarModelResource);
    }
    if (self->scrollbarModelResource) {
      Arcadia_Engine_Visuals_Resource_render(thread, (Arcadia_Engine_Visuals_Resource*)self->scrollbarModelResource, enterPassNode->enterPassResource);
    }
  }
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, enterPassNode, previousViewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, enterPassNode, previousCameraNode);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)enterPassNode, enterPassNode);
}

static void
scrollByImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value deltaX,
    Arcadia_Integer32Value deltaY
  )
{
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, self, ((Arcadia_Engine_UI_ScrollPanelNode*)self)->scrollX + deltaX, ((Arcadia_Engine_UI_ScrollPanelNode*)self)->scrollY + deltaY);
}

static void
scrollToImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* panel = (Arcadia_Engine_UI_ScrollPanelNode*)self;
  Arcadia_Integer32Value maxX = panel->contentWidth > (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)panel)->width ? (Arcadia_Integer32Value)(panel->contentWidth - (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)panel)->width) : 0;
  Arcadia_Integer32Value maxY = getMaxScrollY(panel);
  panel->scrollX = x < 0 ? 0 : x > maxX ? maxX : x;
  panel->scrollY = y < 0 ? 0 : y > maxY ? maxY : y;
}

Arcadia_Engine_UI_ScrollPanelNode*
Arcadia_Engine_UI_ScrollPanelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_ScrollPanelNode);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_Engine_UI_ScrollPanelNode);
}

Arcadia_Engine_UI_ScrollPanelNode*
Arcadia_Engine_UI_ScrollPanelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_ScrollPanelDefinition* source
  )
{
  Arcadia_Engine_UI_ScrollPanelNode* node = Arcadia_Engine_UI_ScrollPanelNode_create(thread, source->left, source->bottom,
                                                                                     source->width, source->height);
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, node, source->contentWidth, source->contentHeight);
  Arcadia_Engine_UI_WidgetNode_setName(thread, (Arcadia_Engine_UI_WidgetNode*)node, ((Arcadia_ADL_Definition*)source)->name);
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, backendContext, (Arcadia_Object*)node, (Arcadia_Object*)source);
  return node;
}

void
Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Integer32Value* x,
    Arcadia_Integer32Value* y
  )
{
  if (x) *x = self->scrollX;
  if (y) *y = self->scrollY;
}

void
Arcadia_Engine_UI_ScrollPanelNode_setContentSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ScrollPanelNode* self,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height
  )
{
  self->contentWidth = width < (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width ? (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width : width;
  self->contentHeight = height < (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height ? (Arcadia_Natural32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height : height;
  scrollToImpl(thread, (Arcadia_Object*)self, self->scrollX, self->scrollY);
}

#undef VerticesDirty
