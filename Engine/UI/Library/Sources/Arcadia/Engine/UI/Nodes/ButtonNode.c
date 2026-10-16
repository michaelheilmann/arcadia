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
#include "Arcadia/Engine/UI/Nodes/ButtonNode.h"

#include "Arcadia/Engine/UI/Activatable.h"
#include "Arcadia/Engine/UI/Focusable.h"
#include "Arcadia/Engine/UI/HitTestable.h"
#include "Arcadia/Engine/UI/KeyboardInput.h"
#include "Arcadia/Engine/UI/PointerInput.h"
#include "Arcadia/Engine/UI/Renderable.h"
#include "Arcadia/Engine/UI/TextUtilities.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Text/Include.h"
#include "Arcadia/FontIO/Include.h"

#define VerticesDirty (1)

static void
updateLineMetrics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_FontIO_AtlasBitmapFont* font
  )
{
  Arcadia_FontIO_AtlasGlyphInformation upper;
  Arcadia_FontIO_AtlasGlyphInformation lower;
  Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, 'H', &upper);
  Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, 'g', &lower);
  Arcadia_Integer32Value upperBottom = upper.glyphInformation.bearingY - upper.glyphInformation.height;
  Arcadia_Integer32Value lowerBottom = lower.glyphInformation.bearingY - lower.glyphInformation.height;
  self->labelLineTop = upper.glyphInformation.bearingY > lower.glyphInformation.bearingY ? upper.glyphInformation.bearingY : lower.glyphInformation.bearingY;
  self->labelLineBottom = upperBottom < lowerBottom ? upperBottom : lowerBottom;
}

static void
Arcadia_Engine_UI_ButtonNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  );

static void
Arcadia_Engine_UI_ButtonNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNodeDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializeActivatableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ActivatableDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  );

static void
Arcadia_Engine_UI_ButtonNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  );

static void
Arcadia_Engine_UI_ButtonNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_ButtonNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_ButtonNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static void
Arcadia_Engine_UI_ButtonNode_activateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
Arcadia_Engine_UI_ButtonNode_focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
Arcadia_Engine_UI_ButtonNode_blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.ButtonNode",
    Arcadia_Engine_UI_ButtonNode,
    u8"Arcadia.Engine.UI.WidgetNode",
    Arcadia_Engine_UI_WidgetNode,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializeRenderableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_Activatable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_ActivatableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializeActivatableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializeFocusableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializeHitTestableDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializeKeyboardInputDispatchImpl
    },
    {
      .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_ButtonNode_initializePointerInputDispatchImpl
    }
  )

static void
getReferenceColor
  (
    Arcadia_ADL_Reference* reference,
    Arcadia_Natural8Value* red,
    Arcadia_Natural8Value* green,
    Arcadia_Natural8Value* blue
  )
{
  if (!reference || !reference->definition) return;
  Arcadia_ADL_ColorDefinition* color = (Arcadia_ADL_ColorDefinition*)reference->definition;
  *red = color->red;
  *green = color->green;
  *blue = color->blue;
}

static void
computePlacement
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Real32Value canvasWidth,
    Arcadia_Real32Value canvasHeight,
    Arcadia_Real32Value* left,
    Arcadia_Real32Value* bottom
  )
{
  (void)thread;
  (void)canvasWidth;
  (void)canvasHeight;
  *left = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->left;
  *bottom = (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->bottom;
}

static void
writeQuad
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue vertexIndex,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top,
    Arcadia_Real32Value red,
    Arcadia_Real32Value green,
    Arcadia_Real32Value blue
  );

static void
updateVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Real32Value canvasWidth,
    Arcadia_Real32Value canvasHeight
  )
{
  self->lastCanvasWidth = canvasWidth;
  self->lastCanvasHeight = canvasHeight;
  Arcadia_Real32Value left, bottom;
  computePlacement(thread, self, canvasWidth, canvasHeight, &left, &bottom);
  Arcadia_Real32Value const right = left + (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->width;
  Arcadia_Real32Value const top = bottom + (Arcadia_Real32Value)((Arcadia_Engine_UI_WidgetNode*)self)->height;
  Arcadia_Natural8Value red = self->source->backgroundRed;
  Arcadia_Natural8Value green = self->source->backgroundGreen;
  Arcadia_Natural8Value blue = self->source->backgroundBlue;
  getReferenceColor(self->source->backgroundColor, &red, &green, &blue);
  if (!self->enabled) {
    getReferenceColor(self->source->disabledBackgroundColor, &red, &green, &blue);
  } else if (self->pressed) {
    getReferenceColor(self->source->pressedBackgroundColor, &red, &green, &blue);
  } else if (self->hovered) {
    getReferenceColor(self->source->hoveredBackgroundColor, &red, &green, &blue);
  } else if (self->focused) {
    getReferenceColor(self->source->focusedBackgroundColor, &red, &green, &blue);
  }
  Arcadia_Real32Value const r = red / 255.f;
  Arcadia_Real32Value const g = green / 255.f;
  Arcadia_Real32Value const b = blue / 255.f;
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices
    (
      thread,
      Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->mesh)
    );
  Arcadia_Real32Value borderWidth = (Arcadia_Real32Value)self->source->borderWidth;
  Arcadia_Real32Value maxBorderWidth = (Arcadia_Real32Value)
    (((Arcadia_Engine_UI_WidgetNode*)self)->width < ((Arcadia_Engine_UI_WidgetNode*)self)->height
      ? ((Arcadia_Engine_UI_WidgetNode*)self)->width
      : ((Arcadia_Engine_UI_WidgetNode*)self)->height) * 0.5f;
  if (borderWidth > maxBorderWidth) {
    borderWidth = maxBorderWidth;
  }
  writeQuad(vertices, 0, left + borderWidth, bottom + borderWidth, right - borderWidth, top - borderWidth, r, g, b);
  if (borderWidth > 0.f) {
    Arcadia_Real32Value const borderRed = self->source->borderRed / 255.f;
    Arcadia_Real32Value const borderGreen = self->source->borderGreen / 255.f;
    Arcadia_Real32Value const borderBlue = self->source->borderBlue / 255.f;
    writeQuad(vertices, 6, left, bottom, right, bottom + borderWidth, borderRed, borderGreen, borderBlue);
    writeQuad(vertices, 12, left, top - borderWidth, right, top, borderRed, borderGreen, borderBlue);
    writeQuad(vertices, 18, left, bottom + borderWidth, left + borderWidth, top - borderWidth, borderRed, borderGreen, borderBlue);
    writeQuad(vertices, 24, right - borderWidth, bottom + borderWidth, right, top - borderWidth, borderRed, borderGreen, borderBlue);
  }
  self->mesh->dirtyFlags |= VerticesDirty;
}

static void
writeQuad
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue vertexIndex,
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
    Arcadia_SizeValue j = (vertexIndex + i) * 9;
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
updateLabelVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Real32Value canvasWidth,
    Arcadia_Real32Value canvasHeight
  )
{
  if (!self->labelModelNode || !self->numberOfLabelGlyphs) {
    return;
  }
  Arcadia_Real32Value left, bottom;
  computePlacement(thread, self, canvasWidth, canvasHeight, &left, &bottom);
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->labelModelNode->mesh);
  Arcadia_String* center = Arcadia_String_createFromCxxString(thread, u8"Center");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)center);
  Arcadia_Engine_UI_TextUtilities_writeMultilineGlyphQuads(thread, self->labelGlyphs, self->numberOfLabelGlyphs, self->labelLines, self->numberOfLabelLines,
                                                           self->labelLineTop, self->labelLineBottom,
                                                           (Arcadia_Integer32Value)left, (Arcadia_Integer32Value)bottom, ((Arcadia_Engine_UI_WidgetNode*)self)->width, ((Arcadia_Engine_UI_WidgetNode*)self)->height,
                                                           center, center, Arcadia_BooleanValue_True,
                                                           (Arcadia_Real32Value)self->labelFontAtlasWidth, (Arcadia_Real32Value)self->labelFontAtlasHeight,
                                                           1.f, 1.f, 1.f, 1.f,
                                                           (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer),
                                                           Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 9);
  Arcadia_ValueStack_popValues(thread, 1);
  self->labelModelNode->mesh->dirtyFlags |= VerticesDirty;
}

static void
createLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Engine* engine
  )
{
  Arcadia_SizeValue numberOfGlyphs = 0;
  Arcadia_SizeValue numberOfLines = 0;
  Arcadia_Engine_UI_TextUtilities_countGlyphsAndLines(thread, self->source->text, &numberOfGlyphs, &numberOfLines);
  if (!numberOfGlyphs) {
    return;
  }
  Arcadia_FontIO_AtlasBitmapFont* font = Arcadia_FontIO_AtlasBitmapFont_create(thread, Arcadia_String_createFromCxxString(thread, Arcadia_Engine_UI_TextUtilities_findFontPath(thread)), self->source->pixelSize);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)font);
  updateLineMetrics(thread, self, font);
  self->labelGlyphs = Arcadia_Memory_allocateUnmanaged(thread, numberOfGlyphs * sizeof(Arcadia_FontIO_AtlasGlyphInformation));
  self->labelLines = Arcadia_Memory_allocateUnmanaged(thread, numberOfLines * sizeof(Arcadia_Engine_UI_TextLine));
  Arcadia_Engine_UI_TextUtilities_loadGlyphsAndLines(thread, font, self->source->text, self->labelGlyphs, numberOfGlyphs, self->labelLines, numberOfLines);
  self->numberOfLabelGlyphs = numberOfGlyphs;
  self->numberOfLabelLines = numberOfLines;
  Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, font);
  self->labelFontAtlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
  self->labelFontAtlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);

  Arcadia_String* colorName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.Color");
  Arcadia_String* fillName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.Fill");
  Arcadia_String* pixelBufferName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.PixelBuffer");
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.Texture");
  Arcadia_String* meshName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.Mesh");
  Arcadia_String* materialName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.Material");
  Arcadia_String* modelName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Label.Model");
  Arcadia_ADL_ColorDefinition* color = Arcadia_ADL_ColorDefinition_create(thread, self->generatedDefinitions, colorName, 255, 255, 255);
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fill = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, self->generatedDefinitions, fillName, colorName);
  Arcadia_ADL_PixelBufferDefinition* pixelBuffer = Arcadia_ADL_PixelBufferDefinition_create(thread, self->generatedDefinitions, pixelBufferName, 1, 1);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffer->operations, (Arcadia_Object*)Arcadia_ADL_Reference_create(thread, self->generatedDefinitions, fillName));
  Arcadia_ADL_TextureDefinition* texture = Arcadia_ADL_TextureDefinition_create(thread, self->generatedDefinitions, textureName, pixelBufferName, Arcadia_ADL_TextureFilter_Nearest, Arcadia_ADL_TextureFilter_Nearest, Arcadia_ADL_TextureAddressMode_None, Arcadia_ADL_TextureAddressMode_None);

  Arcadia_SizeValue const numberOfVertices = 6 * numberOfGlyphs;
  Arcadia_SizeValue const numberOfPositionComponents = 3 * numberOfVertices;
  Arcadia_SizeValue const numberOfColorComponents = 4 * numberOfVertices;
  Arcadia_SizeValue const numberOfTextureCoordinatesComponents = 2 * numberOfVertices;
  Arcadia_Real32Value* positions = Arcadia_Memory_allocateUnmanaged(thread, numberOfPositionComponents * sizeof(Arcadia_Real32Value));
  Arcadia_Real32Value* colors = Arcadia_Memory_allocateUnmanaged(thread, numberOfColorComponents * sizeof(Arcadia_Real32Value));
  Arcadia_Real32Value* textureCoordinates = Arcadia_Memory_allocateUnmanaged(thread, numberOfTextureCoordinatesComponents * sizeof(Arcadia_Real32Value));
  for (Arcadia_SizeValue i = 0; i < numberOfPositionComponents; ++i) positions[i] = 0.f;
  for (Arcadia_SizeValue i = 0; i < numberOfColorComponents; ++i) colors[i] = 1.f;
  for (Arcadia_SizeValue i = 0; i < numberOfTextureCoordinatesComponents; ++i) textureCoordinates[i] = 0.f;
  Arcadia_ADL_MeshDefinition* mesh = Arcadia_ADL_MeshDefinition_create(thread, self->generatedDefinitions, meshName, numberOfVertices,
                                                                      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)positions, numberOfPositionComponents * sizeof(Arcadia_Real32Value)),
                                                                      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)colors, numberOfColorComponents * sizeof(Arcadia_Real32Value)),
                                                                      Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)textureCoordinates, numberOfTextureCoordinatesComponents * sizeof(Arcadia_Real32Value)),
                                                                      colorName);
  Arcadia_ADL_MaterialDefinition* material = Arcadia_ADL_MaterialDefinition_create(thread, self->generatedDefinitions, materialName, Arcadia_ADL_MaterialType_BlinnPhong, Arcadia_ADL_AmbientColorSource_TextureVertex, textureName, Arcadia_ADL_BlendFunction_SourceAlpha, Arcadia_ADL_BlendFunction_OneMinusSourceAlpha);
  Arcadia_ADL_ModelDefinition* model = Arcadia_ADL_ModelDefinition_create(thread, self->generatedDefinitions, modelName, meshName, materialName);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)color);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)fill);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)pixelBuffer);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)texture);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)mesh);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)material);
  Arcadia_Engine_UI_TextUtilities_insertDefinition(thread, self->generatedDefinitions, (Arcadia_ADL_Definition*)model);
  Arcadia_ADL_Definitions_link(thread, self->generatedDefinitions);
  self->labelModelNode = Arcadia_Engine_Visuals_NodeFactory_createModelNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, self->backendContext, model);
  Arcadia_Engine_Visuals_TextureNode* textureNode = self->labelModelNode->material->ambientColorTexture;
  textureNode->pixelBuffer->pixelBuffer = atlas;
  textureNode->width = self->labelFontAtlasWidth;
  textureNode->height = self->labelFontAtlasHeight;
  textureNode->dirtyBits = 0xff;
  Arcadia_Memory_deallocateUnmanaged(thread, positions);
  Arcadia_Memory_deallocateUnmanaged(thread, colors);
  Arcadia_Memory_deallocateUnmanaged(thread, textureCoordinates);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
Arcadia_Engine_UI_ButtonNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_ButtonNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);

  self->source = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_UI_ButtonDefinition_getType(thread));
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)self->source);
  ((Arcadia_Engine_UI_WidgetNode*)self)->name = ((Arcadia_ADL_Definition*)self->source)->name;
  ((Arcadia_Engine_UI_WidgetNode*)self)->left = self->source->left;
  ((Arcadia_Engine_UI_WidgetNode*)self)->bottom = self->source->bottom;
  ((Arcadia_Engine_UI_WidgetNode*)self)->width = (Arcadia_Integer32Value)self->source->width;
  ((Arcadia_Engine_UI_WidgetNode*)self)->height = (Arcadia_Integer32Value)self->source->height;
  self->backendContext = Arcadia_ValueStack_isVoidValue(thread, 2) ? NULL : Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  if (self->backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->modelResource = NULL;
  self->labelModelNode = NULL;
  self->labelGlyphs = NULL;
  self->numberOfLabelGlyphs = 0;
  self->labelLines = NULL;
  self->numberOfLabelLines = 0;
  self->labelLineTop = 0;
  self->labelLineBottom = 0;
  self->labelFontAtlasWidth = 0;
  self->labelFontAtlasHeight = 0;
  self->lastCanvasWidth = 800.f;
  self->lastCanvasHeight = 480.f;
  self->generatedDefinitions = Arcadia_ADL_Definitions_create(thread);
  self->enabled = Arcadia_BooleanValue_True;
  self->hovered = Arcadia_BooleanValue_False;
  self->pressed = Arcadia_BooleanValue_False;
  self->focused = Arcadia_BooleanValue_False;
  self->activated = Arcadia_Signal_create(thread);

  Arcadia_String* colorName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Color");
  Arcadia_String* meshName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Mesh");
  Arcadia_String* materialName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Material");
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Texture");
  Arcadia_String* pixelBufferName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.PixelBuffer");
  Arcadia_String* fillName = Arcadia_String_createFromCxxString(thread, u8"UI.Button.Generated.Fill");

  Arcadia_ADL_ColorDefinition* color = Arcadia_ADL_ColorDefinition_create(thread, self->generatedDefinitions, colorName, self->source->backgroundRed, self->source->backgroundGreen, self->source->backgroundBlue);
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fill = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, self->generatedDefinitions, fillName, colorName);
  Arcadia_ADL_PixelBufferDefinition* pixelBuffer = Arcadia_ADL_PixelBufferDefinition_create(thread, self->generatedDefinitions, pixelBufferName, 1, 1);
  Arcadia_List_insertBackObjectReferenceValue(thread, pixelBuffer->operations, (Arcadia_Object*)Arcadia_ADL_Reference_create(thread, self->generatedDefinitions, fillName));
  Arcadia_ADL_TextureDefinition* texture = Arcadia_ADL_TextureDefinition_create(thread, self->generatedDefinitions, textureName, pixelBufferName, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureFilter_None, Arcadia_ADL_TextureAddressMode_None, Arcadia_ADL_TextureAddressMode_None);

  Arcadia_Real32Value positions[90] = { 0.f };
  Arcadia_Real32Value const r = self->source->backgroundRed / 255.f;
  Arcadia_Real32Value const g = self->source->backgroundGreen / 255.f;
  Arcadia_Real32Value const b = self->source->backgroundBlue / 255.f;
  Arcadia_Real32Value colors[120];
  Arcadia_Real32Value uvs[60] = { 0.f };
  for (Arcadia_SizeValue i = 0; i < 30; ++i) {
    colors[i * 4 + 0] = r;
    colors[i * 4 + 1] = g;
    colors[i * 4 + 2] = b;
    colors[i * 4 + 3] = 1.f;
  }
  Arcadia_ADL_MeshDefinition* mesh = Arcadia_ADL_MeshDefinition_create(thread, self->generatedDefinitions, meshName, 30,
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
  // `Arcadia_Media_Geometry_createFromMeshGenerator` links the mesh generator, hence the generated definitions need not be linked yet.
  Arcadia_Media_Geometry* geometry = Arcadia_Media_Geometry_createFromMeshGenerator(thread, (Arcadia_ADL_MeshGenerator*)mesh);
  self->mesh = Arcadia_Engine_Visuals_NodeFactory_createMeshNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, geometry);
  self->material = Arcadia_Engine_Visuals_NodeFactory_createMaterialNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, NULL, material);
  createLabel(thread, self, engine);
  if (!self->labelModelNode) {
    Arcadia_ADL_Definitions_link(thread, self->generatedDefinitions);
  }
  updateVertices(thread, self, 800.f, 480.f);
  updateLabelVertices(thread, self, 800.f, 480.f);
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_ButtonNode);
}

static void
Arcadia_Engine_UI_ButtonNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  )
{
  if (self->labelGlyphs) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->labelGlyphs);
    self->labelGlyphs = NULL;
  }
  if (self->labelLines) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->labelLines);
    self->labelLines = NULL;
  }
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
Arcadia_Engine_UI_ButtonNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_Visuals_EnterPassNode*))&Arcadia_Engine_UI_ButtonNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*))&Arcadia_Engine_UI_ButtonNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_UI_ButtonNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{
  self->render = &Arcadia_Engine_UI_ButtonNode_renderInterfaceImpl;
}

static void
Arcadia_Engine_UI_ButtonNode_initializeActivatableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ActivatableDispatch* self
  )
{ self->activate = &Arcadia_Engine_UI_ButtonNode_activateImpl; }

static void
Arcadia_Engine_UI_ButtonNode_initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{
  self->focus = &Arcadia_Engine_UI_ButtonNode_focusImpl;
  self->blur = &Arcadia_Engine_UI_ButtonNode_blurImpl;
  self->isFocused = &Arcadia_Engine_UI_ButtonNode_isFocusedImpl;
}

static void
Arcadia_Engine_UI_ButtonNode_initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{ self->hitTest = &Arcadia_Engine_UI_ButtonNode_hitTestImpl; }

static void
Arcadia_Engine_UI_ButtonNode_initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &Arcadia_Engine_UI_ButtonNode_handleKeyboardKeyEventImpl; }

static void
Arcadia_Engine_UI_ButtonNode_initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &Arcadia_Engine_UI_ButtonNode_handleMousePointerEventImpl;
  self->handleMouseButtonEvent = &Arcadia_Engine_UI_ButtonNode_handleMouseButtonEventImpl;
}

static void
Arcadia_Engine_UI_ButtonNode_activateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)self;
  if (!button->enabled) return;
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, button->activated, self);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
Arcadia_Engine_UI_ButtonNode_focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ ((Arcadia_Engine_UI_ButtonNode*)self)->focused = Arcadia_BooleanValue_True; }

static void
Arcadia_Engine_UI_ButtonNode_blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ ((Arcadia_Engine_UI_ButtonNode*)self)->focused = Arcadia_BooleanValue_False; }

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ return ((Arcadia_Engine_UI_ButtonNode*)self)->focused; }

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)self;
  Arcadia_Real32Value left, bottom;
  computePlacement(thread, button, button->lastCanvasWidth, button->lastCanvasHeight, &left, &bottom);
  return x >= (Arcadia_Integer32Value)left && x < (Arcadia_Integer32Value)(left + ((Arcadia_Engine_UI_WidgetNode*)button)->width) &&
         y >= (Arcadia_Integer32Value)bottom && y < (Arcadia_Integer32Value)(bottom + ((Arcadia_Engine_UI_WidgetNode*)button)->height);
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      (Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Return ||
       Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Space)) {
    Arcadia_Engine_UI_ButtonNode_activateImpl(thread, self);
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)self;
  button->hovered = button->enabled;
  return button->enabled;
}

static Arcadia_BooleanValue
Arcadia_Engine_UI_ButtonNode_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)self;
  if (!button->enabled || event->button != 0) return Arcadia_BooleanValue_False;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    button->pressed = Arcadia_BooleanValue_True;
    return Arcadia_BooleanValue_True;
  }
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
    Arcadia_BooleanValue wasPressed = button->pressed;
    button->pressed = Arcadia_BooleanValue_False;
    if (wasPressed && Arcadia_Engine_UI_ButtonNode_hitTestImpl(thread, self, event->x, (Arcadia_Integer32Value)button->lastCanvasHeight - event->y)) {
      Arcadia_Engine_UI_ButtonNode_activateImpl(thread, self);
    }
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static void
Arcadia_Engine_UI_ButtonNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Engine_UI_ButtonNode_renderImpl(thread, (Arcadia_Engine_UI_ButtonNode*)self, enterPassNode);
}

static void
Arcadia_Engine_UI_ButtonNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  )
{
  if (self->source) Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  if (self->generatedDefinitions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->generatedDefinitions);
  if (self->mesh) Arcadia_Object_visit(thread, (Arcadia_Object*)self->mesh);
  if (self->material) Arcadia_Object_visit(thread, (Arcadia_Object*)self->material);
  if (self->labelModelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->labelModelNode);
  if (self->activated) Arcadia_Object_visit(thread, (Arcadia_Object*)self->activated);
}

static void
Arcadia_Engine_UI_ButtonNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
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
  if (self->labelModelNode) {
    updateLabelVertices(thread, self, canvasWidth, canvasHeight);
    Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->labelModelNode, enterPassNode);
  }
}

static void
Arcadia_Engine_UI_ButtonNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) return;
  if (backendContext) Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  if (self->backendContext) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  if (self->modelResource) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->modelResource);
    self->modelResource = NULL;
  }
  self->backendContext = backendContext;
  if (self->labelModelNode) {
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self->labelModelNode, backendContext);
  }
}

Arcadia_Engine_UI_ButtonNode*
Arcadia_Engine_UI_ButtonNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_ButtonDefinition* source
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_ButtonNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_UI_ButtonNode);
}

void
Arcadia_Engine_UI_ButtonNode_setEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_BooleanValue enabled
  )
{
  self->enabled = enabled;
  if (!enabled) {
    self->hovered = Arcadia_BooleanValue_False;
    self->pressed = Arcadia_BooleanValue_False;
    self->focused = Arcadia_BooleanValue_False;
  }
}

void
Arcadia_Engine_UI_ButtonNode_setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self,
    Arcadia_BooleanValue hovered
  )
{ self->hovered = self->enabled && hovered; }

Arcadia_Signal*
Arcadia_Engine_UI_ButtonNode_getActivatedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ButtonNode* self
  )
{ return self->activated; }

#undef VerticesDirty
