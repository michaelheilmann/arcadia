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
#include "Arcadia/Engine/UI/Nodes/ToggleNode.h"

#include "Arcadia/Engine/UI/Activatable.h"
#include "Arcadia/Engine/UI/Focusable.h"
#include "Arcadia/Engine/UI/HitTestable.h"
#include "Arcadia/Engine/UI/KeyboardInput.h"
#include "Arcadia/Engine/UI/PointerInput.h"
#include "Arcadia/Engine/UI/Renderable.h"
#include "Arcadia/Engine/UI/TextUtilities.h"
#include "Arcadia/Engine/UI/ValueControl.h"
#include "Arcadia/Engine/Include.h"

#define VerticesDirty (1)

static void
Arcadia_Engine_UI_ToggleNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  );

static void
Arcadia_Engine_UI_ToggleNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNodeDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeActivatableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ActivatableDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_initializeValueControlDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ValueControlDispatch* self
  );

static void
Arcadia_Engine_UI_ToggleNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  );

static void
Arcadia_Engine_UI_ToggleNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_ToggleNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_ToggleNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static void
Arcadia_Engine_UI_ToggleNode_activateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
Arcadia_Engine_UI_ToggleNode_focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
Arcadia_Engine_UI_ToggleNode_blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static Arcadia_Value
Arcadia_Engine_UI_ToggleNode_getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
Arcadia_Engine_UI_ToggleNode_setValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.ToggleNode",
    Arcadia_Engine_UI_ToggleNode,
    u8"Arcadia.Engine.UI.WidgetNode",
    Arcadia_Engine_UI_WidgetNode,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeRenderableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_Activatable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_ActivatableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeActivatableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeFocusableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeHitTestableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeKeyboardInputDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializePointerInputDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_ValueControl_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_ValueControlDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ToggleNode_initializeValueControlDispatchImpl
    }
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

static void
updateVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_Real32Value canvasWidth,
    Arcadia_Real32Value canvasHeight
  )
{
  self->lastCanvasWidth = canvasWidth;
  self->lastCanvasHeight = canvasHeight;
  Arcadia_Real32Value left = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->left;
  Arcadia_Real32Value bottom = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->bottom;
  Arcadia_Real32Value right = left + (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width;
  Arcadia_Real32Value top = bottom + (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height;
  Arcadia_Real32Value trackRed = self->checked ? 0.18f : 0.22f;
  Arcadia_Real32Value trackGreen = self->checked ? 0.58f : 0.24f;
  Arcadia_Real32Value trackBlue = self->checked ? 0.32f : 0.28f;
  if (self->hovered) {
    trackRed += 0.08f;
    trackGreen += 0.08f;
    trackBlue += 0.08f;
  }
  if (self->pressed) {
    trackRed *= 0.75f;
    trackGreen *= 0.75f;
    trackBlue *= 0.75f;
  }
  if (self->focused) {
    trackBlue += 0.12f;
  }
  Arcadia_Real32Value knobSize = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height - 12.f;
  Arcadia_Real32Value knobLeft = self->checked ? right - knobSize - 6.f : left + 6.f;
  Arcadia_Real32Value knobBottom = bottom + 6.f;
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->mesh));
  writeQuad(vertices, 0, left, bottom, right, top, trackRed, trackGreen, trackBlue);
  writeQuad(vertices, 6, knobLeft, knobBottom, knobLeft + knobSize, knobBottom + knobSize, 0.92f, 0.94f, 0.96f);
  self->mesh->dirtyFlags |= VerticesDirty;
}

static void
Arcadia_Engine_UI_ToggleNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_ToggleNode);
  if (6 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->backendContext = Arcadia_ValueStack_isVoidValue(thread, 6) ? NULL : Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 6, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  if (self->backendContext) Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 5);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 3);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 2);
  ((Arcadia_Engine_UI_WidgetNode*)self)->left = self->left;
  ((Arcadia_Engine_UI_WidgetNode*)self)->bottom = self->bottom;
  ((Arcadia_Engine_UI_WidgetNode*)self)->width = (Arcadia_Integer32Value)self->width;
  ((Arcadia_Engine_UI_WidgetNode*)self)->height = (Arcadia_Integer32Value)self->height;
  self->checked = Arcadia_ValueStack_getBooleanValue(thread, 1);
  self->enabled = Arcadia_BooleanValue_True;
  self->hovered = Arcadia_BooleanValue_False;
  self->pressed = Arcadia_BooleanValue_False;
  self->focused = Arcadia_BooleanValue_False;
  self->lastCanvasWidth = 800.f;
  self->lastCanvasHeight = 480.f;
  self->changed = Arcadia_Signal_create(thread);
  self->modelResource = NULL;
  self->generatedDefinitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_String* colorName = Arcadia_String_createFromCxxString(thread, u8"UI.Toggle.Generated.Color");
  Arcadia_String* meshName = Arcadia_String_createFromCxxString(thread, u8"UI.Toggle.Generated.Mesh");
  Arcadia_String* materialName = Arcadia_String_createFromCxxString(thread, u8"UI.Toggle.Generated.Material");
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, u8"UI.Toggle.Generated.Texture");
  Arcadia_String* pixelBufferName = Arcadia_String_createFromCxxString(thread, u8"UI.Toggle.Generated.PixelBuffer");
  Arcadia_String* fillName = Arcadia_String_createFromCxxString(thread, u8"UI.Toggle.Generated.Fill");
  Arcadia_ADL_ColorDefinition* color = Arcadia_ADL_ColorDefinition_create(thread, self->generatedDefinitions, colorName, 255, 255, 255);
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fill = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, self->generatedDefinitions, fillName, colorName);
  Arcadia_ADL_PixelBufferDefinition* pixelBuffer = Arcadia_ADL_PixelBufferDefinition_create(thread, self->generatedDefinitions, pixelBufferName, 1, 1);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffer->operations, (Arcadia_Object*)Arcadia_ADL_Reference_create(thread, self->generatedDefinitions, fillName));
  Arcadia_ADL_TextureDefinition* texture = Arcadia_ADL_TextureDefinition_create(thread, self->generatedDefinitions, textureName, pixelBufferName, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureAddressMode_None, Arcadia_ADL_TextureAddressMode_None);
  Arcadia_Real32Value positions[36] = { 0.f };
  Arcadia_Real32Value colors[48];
  Arcadia_Real32Value uvs[24] = { 0.f };
  for (Arcadia_SizeValue i = 0; i < 48; ++i) colors[i] = 1.f;
  Arcadia_ADL_MeshDefinition* mesh = Arcadia_ADL_MeshDefinition_create(thread, self->generatedDefinitions, meshName, 12,
                                                                       Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)positions, sizeof(positions)),
                                                                       Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)colors, sizeof(colors)),
                                                                       Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)uvs, sizeof(uvs)),
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
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_ToggleNode);
}

static void
Arcadia_Engine_UI_ToggleNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
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
Arcadia_Engine_UI_ToggleNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_Visuals_EnterPassNode*))&Arcadia_Engine_UI_ToggleNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*))&Arcadia_Engine_UI_ToggleNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_UI_ToggleNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{ self->render = &Arcadia_Engine_UI_ToggleNode_renderInterfaceImpl; }

static void
Arcadia_Engine_UI_ToggleNode_initializeActivatableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ActivatableDispatch* self
  )
{ self->activate = &Arcadia_Engine_UI_ToggleNode_activateImpl; }

static void
Arcadia_Engine_UI_ToggleNode_initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{
  self->focus = &Arcadia_Engine_UI_ToggleNode_focusImpl;
  self->blur = &Arcadia_Engine_UI_ToggleNode_blurImpl;
  self->isFocused = &Arcadia_Engine_UI_ToggleNode_isFocusedImpl;
}

static void
Arcadia_Engine_UI_ToggleNode_initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{ self->hitTest = &Arcadia_Engine_UI_ToggleNode_hitTestImpl; }

static void
Arcadia_Engine_UI_ToggleNode_initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &Arcadia_Engine_UI_ToggleNode_handleKeyboardKeyEventImpl; }

static void
Arcadia_Engine_UI_ToggleNode_initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &Arcadia_Engine_UI_ToggleNode_handleMousePointerEventImpl;
  self->handleMouseButtonEvent = &Arcadia_Engine_UI_ToggleNode_handleMouseButtonEventImpl;
}

static void
Arcadia_Engine_UI_ToggleNode_initializeValueControlDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ValueControlDispatch* self
  )
{
  self->getValue = &Arcadia_Engine_UI_ToggleNode_getValueImpl;
  self->setValue = &Arcadia_Engine_UI_ToggleNode_setValueImpl;
}

static void
Arcadia_Engine_UI_ToggleNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  )
{
  if (self->mesh) Arcadia_Object_visit(thread, (Arcadia_Object*)self->mesh);
  if (self->material) Arcadia_Object_visit(thread, (Arcadia_Object*)self->material);
  if (self->generatedDefinitions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->generatedDefinitions);
  if (self->changed) Arcadia_Object_visit(thread, (Arcadia_Object*)self->changed);
}

static void
Arcadia_Engine_UI_ToggleNode_activateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_Engine_UI_ToggleNode* toggle = (Arcadia_Engine_UI_ToggleNode*)self;
  if (!toggle->enabled) return;
  Arcadia_Engine_UI_ToggleNode_setChecked(thread, toggle, !toggle->checked);
}

static void
Arcadia_Engine_UI_ToggleNode_focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ ((Arcadia_Engine_UI_ToggleNode*)self)->focused = Arcadia_BooleanValue_True; }

static void
Arcadia_Engine_UI_ToggleNode_blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ ((Arcadia_Engine_UI_ToggleNode*)self)->focused = Arcadia_BooleanValue_False; }

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ return ((Arcadia_Engine_UI_ToggleNode*)self)->focused; }

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_ToggleNode* toggle = (Arcadia_Engine_UI_ToggleNode*)self;
  return x >= ((Arcadia_Engine_UI_WidgetNode*)toggle)->left && x < ((Arcadia_Engine_UI_WidgetNode*)toggle)->left + ((Arcadia_Engine_UI_WidgetNode*)toggle)->width &&
         y >= ((Arcadia_Engine_UI_WidgetNode*)toggle)->bottom && y < ((Arcadia_Engine_UI_WidgetNode*)toggle)->bottom + ((Arcadia_Engine_UI_WidgetNode*)toggle)->height;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      (Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Return ||
       Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Space)) {
    Arcadia_Engine_UI_ToggleNode_activateImpl(thread, self);
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  Arcadia_Engine_UI_ToggleNode* toggle = (Arcadia_Engine_UI_ToggleNode*)self;
  toggle->hovered = toggle->enabled;
  return toggle->enabled;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_ToggleNode* toggle = (Arcadia_Engine_UI_ToggleNode*)self;
  if (!toggle->enabled || event->button != 0) return Arcadia_BooleanValue_False;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    toggle->pressed = Arcadia_BooleanValue_True;
    return Arcadia_BooleanValue_True;
  }
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
    Arcadia_BooleanValue wasPressed = toggle->pressed;
    toggle->pressed = Arcadia_BooleanValue_False;
    if (wasPressed && Arcadia_Engine_UI_ToggleNode_hitTestImpl(thread, self, event->x, (Arcadia_Integer32Value)toggle->lastCanvasHeight - event->y)) {
      Arcadia_Engine_UI_ToggleNode_activateImpl(thread, self);
    }
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_Value
Arcadia_Engine_UI_ToggleNode_getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ return Arcadia_Value_makeBooleanValue(((Arcadia_Engine_UI_ToggleNode*)self)->checked); }

static void
Arcadia_Engine_UI_ToggleNode_setValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Value value
  )
{
  if (!Arcadia_Value_isBooleanValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_ToggleNode_setChecked(thread, (Arcadia_Engine_UI_ToggleNode*)self, Arcadia_Value_getBooleanValue(&value));
}

static void
Arcadia_Engine_UI_ToggleNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{ Arcadia_Engine_UI_ToggleNode_renderImpl(thread, (Arcadia_Engine_UI_ToggleNode*)self, enterPassNode); }

static void
Arcadia_Engine_UI_ToggleNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Real32Value canvasWidth, canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_getCanvasSize(thread, enterPassNode->viewportNode, &canvasWidth, &canvasHeight);
  updateVertices(thread, self, canvasWidth, canvasHeight);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->material, enterPassNode);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->mesh, enterPassNode);
  if (self->backendContext && !self->modelResource) {
    self->modelResource = Arcadia_Engine_Visuals_BackendContext_createModelResource(thread, self->backendContext, self->mesh->constantBufferResource, self->mesh->vertexBufferResource, self->material->materialResource);
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
  }
  if (self->backendContext) {
    Arcadia_Engine_Visuals_Resource_render(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource, enterPassNode->enterPassResource);
  }
}

static void
Arcadia_Engine_UI_ToggleNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) return;
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
}

Arcadia_Engine_UI_ToggleNode*
Arcadia_Engine_UI_ToggleNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_BooleanValue checked
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_ToggleNode);
  if (backendContext) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushBooleanValue(thread, checked);
  Arcadia_ValueStack_pushNatural8Value(thread, 6);
  _Arcadia_EndCreate(Arcadia_Engine_UI_ToggleNode);
}

Arcadia_Engine_UI_ToggleNode*
Arcadia_Engine_UI_ToggleNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_ToggleDefinition* source
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Engine_UI_ToggleNode* node = Arcadia_Engine_UI_ToggleNode_create(thread, backendContext, source->left, source->bottom, source->width, source->height, source->checked);
  Arcadia_Engine_UI_WidgetNode_setName(thread, (Arcadia_Engine_UI_WidgetNode*)node, ((Arcadia_ADL_Definition*)source)->name);
  Arcadia_ValueStack_popValues(thread, 1);
  return node;
}

void
Arcadia_Engine_UI_ToggleNode_setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_BooleanValue hovered
  )
{ self->hovered = self->enabled && hovered; }

void
Arcadia_Engine_UI_ToggleNode_setChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self,
    Arcadia_BooleanValue checked
  )
{
  if (self->checked == checked) return;
  self->checked = checked;
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, self->changed, (Arcadia_Object*)self);
  Arcadia_ValueStack_popValues(thread, 1);
}

Arcadia_BooleanValue
Arcadia_Engine_UI_ToggleNode_getChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  )
{ return self->checked; }

Arcadia_Signal*
Arcadia_Engine_UI_ToggleNode_getChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ToggleNode* self
  )
{ return self->changed; }

#undef VerticesDirty
