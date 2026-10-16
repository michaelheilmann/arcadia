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
#include "Arcadia/Engine/UI/Nodes/SegmentedChoiceNode.h"

#include "Arcadia/Engine/UI/Focusable.h"
#include "Arcadia/Engine/UI/HitTestable.h"
#include "Arcadia/Engine/UI/KeyboardInput.h"
#include "Arcadia/Engine/UI/Nodes/LabelNode.h"
#include "Arcadia/Engine/UI/PointerInput.h"
#include "Arcadia/Engine/UI/Renderable.h"
#include "Arcadia/Engine/UI/TextUtilities.h"
#include "Arcadia/Engine/UI/ValueControl.h"
#include "Arcadia/Engine/Include.h"

#define VerticesDirty (1)

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNodeDispatch* self
  );

static void
initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
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
initializeValueControlDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ValueControlDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
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
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
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

static Arcadia_Value
getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
setValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};
static const Arcadia_Type_Operations _typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &_objectTypeOperations };

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.SegmentedChoiceNode", Arcadia_Engine_UI_SegmentedChoiceNode,
    u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode, &_typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeRenderableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeFocusableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeHitTestableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeKeyboardInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializePointerInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_ValueControl_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ValueControlDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeValueControlDispatchImpl }
  )

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
    Arcadia_Real32Value blue
  )
{
  Arcadia_Real32Value const positions[6][3] = {
    { left, bottom, 0.f }, { right, bottom, 0.f }, { left, top, 0.f },
    { right, bottom, 0.f }, { right, top, 0.f }, { left, top, 0.f },
  };
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    Arcadia_SizeValue j = (index + i) * 9;
    vertices[j + 0] = positions[i][0];
    vertices[j + 1] = positions[i][1];
    vertices[j + 2] = positions[i][2];
    vertices[j + 3] = red;
    vertices[j + 4] = green;
    vertices[j + 5] = blue;
    vertices[j + 6] = 1.f;
    vertices[j + 7] = 0.f;
    vertices[j + 8] = 0.f;
  }
}

static Arcadia_Natural32Value
segmentAt
  (
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Integer32Value x
  )
{
  if (!self->numberOfSegments) {
    return 0;
  }
  Arcadia_Integer32Value relative = x - ((Arcadia_Engine_UI_WidgetNode*)self)->left;
  if (relative < 0) {
    return 0;
  }
  Arcadia_Natural32Value i = (Arcadia_Natural32Value)(((Arcadia_Natural64Value)relative * self->numberOfSegments) / ((Arcadia_Engine_UI_WidgetNode*)self)->width);
  return i >= self->numberOfSegments ? self->numberOfSegments - 1 : i;
}

static void
updateVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Real32Value canvasWidth,
    Arcadia_Real32Value canvasHeight
  )
{
  self->lastCanvasWidth = canvasWidth;
  self->lastCanvasHeight = canvasHeight;
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->mesh));
  Arcadia_Real32Value segmentWidth = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width / (Arcadia_Real32Value)self->numberOfSegments;
  for (Arcadia_Natural32Value i = 0; i < self->numberOfSegments; ++i) {
    Arcadia_Real32Value red = i == self->selectedIndex ? 0.24f : 0.18f;
    Arcadia_Real32Value green = i == self->selectedIndex ? 0.48f : 0.20f;
    Arcadia_Real32Value blue = i == self->selectedIndex ? 0.82f : (self->focused ? 0.34f : 0.26f);
    if (i == self->hoveredIndex) {
      red += 0.08f;
      green += 0.08f;
      blue += 0.08f;
    }
    Arcadia_Real32Value left = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->left + segmentWidth * (Arcadia_Real32Value)i;
    Arcadia_Real32Value bottom = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->bottom;
    Arcadia_Real32Value top = (Arcadia_Real32Value)(((Arcadia_Engine_UI_WidgetNode*)self)->bottom + ((Arcadia_Engine_UI_WidgetNode*)self)->height);
    Arcadia_Real32Value borderWidth = (Arcadia_Real32Value)self->borderWidth;
    Arcadia_Real32Value maxBorderWidth = (segmentWidth < top - bottom ? segmentWidth : top - bottom) * 0.5f;
    if (borderWidth > maxBorderWidth) {
      borderWidth = maxBorderWidth;
    }
    Arcadia_SizeValue vertexIndex = i * 30;
    Arcadia_Real32Value fillLeftInset = borderWidth > 0.f ? (0 == i ? borderWidth : 0.f) : 1.f;
    Arcadia_Real32Value fillRightInset = borderWidth > 0.f ? borderWidth : 1.f;
    writeQuad(vertices, vertexIndex, left + fillLeftInset, bottom + (borderWidth > 0.f ? borderWidth : 0.f),
      left + segmentWidth - fillRightInset, top - (borderWidth > 0.f ? borderWidth : 0.f), red, green, blue);
    if (borderWidth > 0.f) {
      Arcadia_Real32Value borderRed = self->borderRed / 255.f;
      Arcadia_Real32Value borderGreen = self->borderGreen / 255.f;
      Arcadia_Real32Value borderBlue = self->borderBlue / 255.f;
      writeQuad(vertices, vertexIndex + 6, left, bottom, left + segmentWidth, bottom + borderWidth, borderRed, borderGreen, borderBlue);
      writeQuad(vertices, vertexIndex + 12, left, top - borderWidth, left + segmentWidth, top, borderRed, borderGreen, borderBlue);
      if (0 == i) {
        writeQuad(vertices, vertexIndex + 18, left, bottom + borderWidth, left + borderWidth, top - borderWidth, borderRed, borderGreen, borderBlue);
      }
      writeQuad(vertices, vertexIndex + 24, left + segmentWidth - borderWidth, bottom + borderWidth, left + segmentWidth, top - borderWidth, borderRed, borderGreen, borderBlue);
    }
  }
  self->mesh->dirtyFlags |= VerticesDirty;
}

static void
updateLabelBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  )
{
  Arcadia_Real32Value segmentWidth = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width / (Arcadia_Real32Value)self->numberOfSegments;
  for (Arcadia_Natural32Value i = 0; i < self->numberOfSegments; ++i) {
    Arcadia_Engine_UI_LabelNode* label = (Arcadia_Engine_UI_LabelNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->labelNodes, i);
    Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)label,
                                           ((Arcadia_Engine_UI_WidgetNode*)self)->left + (Arcadia_Integer32Value)(segmentWidth * (Arcadia_Real32Value)i),
                                           ((Arcadia_Engine_UI_WidgetNode*)self)->bottom,
                                           (Arcadia_Integer32Value)segmentWidth,
                                           ((Arcadia_Engine_UI_WidgetNode*)self)->height);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_SegmentedChoiceNode);
  if (7 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);

  self->backendContext = Arcadia_ValueStack_isVoidValue(thread, 7) ? NULL : Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 7, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  if (self->backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 6);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 5);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 4);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 3);
  self->pixelSize = 24;
  self->borderWidth = 0;
  self->borderRed = 87;
  self->borderGreen = 104;
  self->borderBlue = 118;
  self->numberOfSegments = Arcadia_ValueStack_getNatural32Value(thread, 2);
  self->selectedIndex = Arcadia_ValueStack_getNatural32Value(thread, 1);
  ((Arcadia_Engine_UI_WidgetNode*)self)->left = self->left;
  ((Arcadia_Engine_UI_WidgetNode*)self)->bottom = self->bottom;
  ((Arcadia_Engine_UI_WidgetNode*)self)->width = (Arcadia_Integer32Value)self->width;
  ((Arcadia_Engine_UI_WidgetNode*)self)->height = (Arcadia_Integer32Value)self->height;
  if (!self->numberOfSegments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->selectedIndex >= self->numberOfSegments) {
    self->selectedIndex = self->numberOfSegments - 1;
  }
  self->hoveredIndex = self->numberOfSegments;
  self->enabled = Arcadia_BooleanValue_True;
  self->pressed = Arcadia_BooleanValue_False;
  self->focused = Arcadia_BooleanValue_False;
  self->changed = Arcadia_Signal_create(thread);
  self->modelResource = NULL;
  self->generatedDefinitions = Arcadia_ADL_Definitions_create(thread);
  self->labelNodes = (Arcadia_List*)Arcadia_ArrayList_create(thread);

  Arcadia_String* colorName = Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice.Generated.Color");
  Arcadia_String* meshName = Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice.Generated.Mesh");
  Arcadia_String* materialName = Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice.Generated.Material");
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice.Generated.Texture");
  Arcadia_String* pixelBufferName = Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice.Generated.PixelBuffer");
  Arcadia_String* fillName = Arcadia_String_createFromCxxString(thread, u8"UI.SegmentedChoice.Generated.Fill");
  Arcadia_ADL_ColorDefinition* color = Arcadia_ADL_ColorDefinition_create(thread, self->generatedDefinitions, colorName, 255, 255, 255);
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fill = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, self->generatedDefinitions, fillName, colorName);
  Arcadia_ADL_PixelBufferDefinition* pixelBuffer = Arcadia_ADL_PixelBufferDefinition_create(thread, self->generatedDefinitions, pixelBufferName, 1, 1);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffer->operations, (Arcadia_Object*)Arcadia_ADL_Reference_create(thread, self->generatedDefinitions, fillName));
  Arcadia_ADL_TextureDefinition* texture = Arcadia_ADL_TextureDefinition_create(thread, self->generatedDefinitions, textureName, pixelBufferName, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureAddressMode_None, Arcadia_ADL_TextureAddressMode_None);

  Arcadia_SizeValue n = 30 * self->numberOfSegments;
  Arcadia_Real32Value* positions = Arcadia_Memory_allocateUnmanaged(thread, 3 * n * sizeof(Arcadia_Real32Value));
  Arcadia_Real32Value* colors = Arcadia_Memory_allocateUnmanaged(thread, 4 * n * sizeof(Arcadia_Real32Value));
  Arcadia_Real32Value* uvs = Arcadia_Memory_allocateUnmanaged(thread, 2 * n * sizeof(Arcadia_Real32Value));
  for (Arcadia_SizeValue i = 0; i < 3 * n; ++i) {
    positions[i] = 0.f;
  }
  for (Arcadia_SizeValue i = 0; i < 4 * n; ++i) {
    colors[i] = 1.f;
  }
  for (Arcadia_SizeValue i = 0; i < 2 * n; ++i) {
    uvs[i] = 0.f;
  }
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

  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, (Arcadia_ADL_MeshGenerator*)mesh);
  self->mesh = Arcadia_Engine_Visuals_NodeFactory_createMeshNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, geometry);
  self->material = Arcadia_Engine_Visuals_NodeFactory_createMaterialNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, material);
  updateVertices(thread, self, 800.f, 480.f);
  for (Arcadia_Natural32Value i = 0; i < self->numberOfSegments; ++i) {
    Arcadia_Engine_UI_LabelNode* label = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                            self->backendContext,
                                                                            Arcadia_String_createFromCxxString(thread, u8""),
                                                                            0,
                                                                            0,
                                                                            1,
                                                                            1,
                                                                            Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                            Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                            245,
                                                                            248,
                                                                            252,
                                                                            255);
    Arcadia_List_insertBackObjectReferenceValue(thread, self->labelNodes, (Arcadia_Object*)label);
  }
  Arcadia_Memory_deallocateUnmanaged(thread, positions);
  Arcadia_Memory_deallocateUnmanaged(thread, colors);
  Arcadia_Memory_deallocateUnmanaged(thread, uvs);
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_SegmentedChoiceNode);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  )
{
  if (self->backendContext) {
    if (self->modelResource) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
    }
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  }
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_Visuals_EnterPassNode*))&renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*))&setVisualsBackendContextImpl;
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
  self->handleMousePointerEvent = &handleMousePointerEventImpl;
  self->handleMouseButtonEvent = &handleMouseButtonEventImpl;
}

static void
initializeValueControlDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ValueControlDispatch* self
  )
{
  self->getValue = &getValueImpl;
  self->setValue = &setValueImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  )
{
  if (self->mesh) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->mesh);
  }
  if (self->material) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->material);
  }
  if (self->generatedDefinitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->generatedDefinitions);
  }
  if (self->labelNodes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->labelNodes);
  }
  if (self->changed) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->changed);
  }
}

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ((Arcadia_Engine_UI_SegmentedChoiceNode*)self)->focused = Arcadia_BooleanValue_True;
}

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ((Arcadia_Engine_UI_SegmentedChoiceNode*)self)->focused = Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return ((Arcadia_Engine_UI_SegmentedChoiceNode*)self)->focused;
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
  Arcadia_Engine_UI_WidgetNode* choice = (Arcadia_Engine_UI_WidgetNode*)self;
  return x >= choice->left && x < choice->left + choice->width && y >= choice->bottom && y < choice->bottom + choice->height;
}

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_SegmentedChoiceNode* choice = (Arcadia_Engine_UI_SegmentedChoiceNode*)self;
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) != Arcadia_Engine_Input_KeyboardKeyAction_Released) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
  if (key == Arcadia_Engine_Input_KeyboardKey_LeftArrow && choice->selectedIndex > 0) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setSelectedIndex(thread, choice, choice->selectedIndex - 1);
    return Arcadia_BooleanValue_True;
  }
  if (key == Arcadia_Engine_Input_KeyboardKey_RightArrow && choice->selectedIndex + 1 < choice->numberOfSegments) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setSelectedIndex(thread, choice, choice->selectedIndex + 1);
    return Arcadia_BooleanValue_True;
  }
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
  Arcadia_Engine_UI_SegmentedChoiceNode* choice = (Arcadia_Engine_UI_SegmentedChoiceNode*)self;
  choice->hoveredIndex = segmentAt(choice, event->x);
  return choice->enabled;
}

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_SegmentedChoiceNode* choice = (Arcadia_Engine_UI_SegmentedChoiceNode*)self;
  if (!choice->enabled || event->button != 0) {
    return Arcadia_BooleanValue_False;
  }
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    choice->pressed = Arcadia_BooleanValue_True;
    return Arcadia_BooleanValue_True;
  }
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
    choice->pressed = Arcadia_BooleanValue_False;
    Arcadia_Engine_UI_SegmentedChoiceNode_setSelectedIndex(thread, choice, segmentAt(choice, event->x));
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_Value
getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return Arcadia_Value_makeNatural32Value(((Arcadia_Engine_UI_SegmentedChoiceNode*)self)->selectedIndex);
}

static void
setValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  )
{
  if (!Arcadia_Value_isNatural32Value(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_SegmentedChoiceNode_setSelectedIndex(thread, (Arcadia_Engine_UI_SegmentedChoiceNode*)self, Arcadia_Value_getNatural32Value(&value));
}

static void
renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  renderImpl(thread, (Arcadia_Engine_UI_SegmentedChoiceNode*)self, enterPassNode);
}

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Real32Value canvasWidth, canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_getCanvasSize(thread, enterPassNode->viewportNode, &canvasWidth, &canvasHeight);
  updateVertices(thread, self, canvasWidth, canvasHeight);
  updateLabelBounds(thread, self);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->material, enterPassNode);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->mesh, enterPassNode);
  if (self->backendContext && !self->modelResource) {
    self->modelResource = Arcadia_Engine_Visuals_BackendContext_createModelResource(thread, self->backendContext, self->mesh->constantBufferResource, self->mesh->vertexBufferResource, self->material->materialResource);
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
  }
  if (self->backendContext) {
    Arcadia_Engine_Visuals_Resource_render(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource, enterPassNode->enterPassResource);
  }
  for (Arcadia_Natural32Value i = 0; i < self->numberOfSegments; ++i) {
    Arcadia_Engine_UI_LabelNode* label = (Arcadia_Engine_UI_LabelNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->labelNodes, i);
    Arcadia_Engine_UI_Renderable_render(thread, (Arcadia_Object*)label, enterPassNode);
  }
}

static void
setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) {
    return;
  }
  if (backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  }
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->backendContext = backendContext;
  if (self->mesh) {
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self->mesh, backendContext);
  }
  if (self->material) {
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self->material, backendContext);
  }
  if (self->labelNodes) {
    Arcadia_Natural32Value n = (Arcadia_Natural32Value)Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->labelNodes);
    for (Arcadia_Natural32Value i = 0; i < n; ++i) {
      Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)Arcadia_List_getObjectReferenceValueAt(thread, self->labelNodes, i), backendContext);
    }
  }
}

Arcadia_Engine_UI_SegmentedChoiceNode*
Arcadia_Engine_UI_SegmentedChoiceNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_Natural32Value numberOfSegments,
    Arcadia_Natural32Value selectedIndex
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_SegmentedChoiceNode);
  if (backendContext) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushNatural32Value(thread, numberOfSegments);
  Arcadia_ValueStack_pushNatural32Value(thread, selectedIndex);
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  _Arcadia_EndCreate(Arcadia_Engine_UI_SegmentedChoiceNode);
}
Arcadia_Engine_UI_SegmentedChoiceNode*
Arcadia_Engine_UI_SegmentedChoiceNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_SegmentedChoiceDefinition* source
  )
{
  Arcadia_SizeValue numberOfSegments = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)source->segmentLabels);
  if (!numberOfSegments || numberOfSegments > Arcadia_Natural32Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Engine_UI_SegmentedChoiceNode* node = Arcadia_Engine_UI_SegmentedChoiceNode_create(thread, backendContext, source->left, source->bottom,
                                                                                               source->width, source->height,
                                                                                               (Arcadia_Natural32Value)numberOfSegments,
                                                                                               source->selectedIndex);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
  node->pixelSize = source->pixelSize;
  node->borderWidth = source->borderWidth;
  node->borderRed = source->borderRed;
  node->borderGreen = source->borderGreen;
  node->borderBlue = source->borderBlue;
  for (Arcadia_SizeValue i = 0; i < numberOfSegments; ++i) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, node, (Arcadia_Natural32Value)i,
                                                          (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, source->segmentLabels, i, _Arcadia_String_getType(thread)));
    Arcadia_Engine_UI_LabelNode_setPixelSize(thread,
      (Arcadia_Engine_UI_LabelNode*)Arcadia_List_getObjectReferenceValueAt(thread, node->labelNodes, i), node->pixelSize);
  }
  updateVertices(thread, node, node->lastCanvasWidth, node->lastCanvasHeight);
  Arcadia_ValueStack_popValues(thread, 2);
  return node;
}
void
Arcadia_Engine_UI_SegmentedChoiceNode_setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_BooleanValue hovered
  )
{
  if (!hovered) {
    self->hoveredIndex = self->numberOfSegments;
  }
}

void
Arcadia_Engine_UI_SegmentedChoiceNode_setSelectedIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Natural32Value selectedIndex
  )
{
  if (selectedIndex >= self->numberOfSegments) {
    selectedIndex = self->numberOfSegments - 1;
  }
  if (self->selectedIndex == selectedIndex) {
    return;
  }
  self->selectedIndex = selectedIndex;
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, self->changed, (Arcadia_Object*)self);
  Arcadia_ValueStack_popValues(thread, 1);
}

Arcadia_Natural32Value
Arcadia_Engine_UI_SegmentedChoiceNode_getSelectedIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  )
{
  return self->selectedIndex;
}

Arcadia_Signal*
Arcadia_Engine_UI_SegmentedChoiceNode_getChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self
  )
{
  return self->changed;
}

void
Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_SegmentedChoiceNode* self,
    Arcadia_Natural32Value index,
    Arcadia_String* label
  )
{
  if (index >= self->numberOfSegments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_LabelNode_setText(thread, (Arcadia_Engine_UI_LabelNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->labelNodes, index), label);
}

#undef VerticesDirty
