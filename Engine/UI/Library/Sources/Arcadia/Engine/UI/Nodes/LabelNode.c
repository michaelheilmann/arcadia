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
#include "Arcadia/Engine/UI/Nodes/LabelNode.h"

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
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_FontIO_AtlasBitmapFont* font
  )
{
  Arcadia_FontIO_AtlasGlyphInformation upper;
  Arcadia_FontIO_AtlasGlyphInformation lower;
  Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, 'H', &upper);
  Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, 'g', &lower);
  Arcadia_Integer32Value upperBottom = upper.glyphInformation.bearingY - upper.glyphInformation.height;
  Arcadia_Integer32Value lowerBottom = lower.glyphInformation.bearingY - lower.glyphInformation.height;
  self->lineTop = upper.glyphInformation.bearingY > lower.glyphInformation.bearingY ? upper.glyphInformation.bearingY : lower.glyphInformation.bearingY;
  self->lineBottom = upperBottom < lowerBottom ? upperBottom : lowerBottom;
}

static void
Arcadia_Engine_UI_LabelNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  );

static void
Arcadia_Engine_UI_LabelNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  );

static void
Arcadia_Engine_UI_LabelNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNodeDispatch* self
  );

static void
Arcadia_Engine_UI_LabelNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  );

static void
Arcadia_Engine_UI_LabelNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  );

static void
Arcadia_Engine_UI_LabelNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_LabelNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
Arcadia_Engine_UI_LabelNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_LabelNode_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_UI_LabelNode_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_LabelNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_LabelNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.LabelNode",
    Arcadia_Engine_UI_LabelNode,
    u8"Arcadia.Engine.UI.WidgetNode",
    Arcadia_Engine_UI_WidgetNode,
    &_typeOperations,
    {
      .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_LabelNode_initializeRenderableDispatchImpl
    }
  )

static void
computePlacement
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
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
updateVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Real32Value canvasWidth,
    Arcadia_Real32Value canvasHeight
  )
{
  if (!self->modelNode || !self->numberOfGlyphs) {
    return;
  }
  Arcadia_Real32Value left, bottom;
  computePlacement(thread, self, canvasWidth, canvasHeight, &left, &bottom);
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->modelNode->mesh);
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread, vertexBuffer);
  Arcadia_Engine_UI_TextUtilities_writeMultilineGlyphQuads(thread, self->glyphs, self->numberOfGlyphs, self->lines, self->numberOfLines,
                                                           self->lineTop, self->lineBottom,
                                                           (Arcadia_Integer32Value)left, (Arcadia_Integer32Value)bottom, ((Arcadia_Engine_UI_WidgetNode*)self)->width, ((Arcadia_Engine_UI_WidgetNode*)self)->height,
                                                           self->horizontalAlignment, self->verticalAlignment, Arcadia_BooleanValue_True,
                                                           (Arcadia_Real32Value)self->fontAtlasWidth, (Arcadia_Real32Value)self->fontAtlasHeight,
                                                           self->red / 255.f, self->green / 255.f, self->blue / 255.f, self->alpha / 255.f,
                                                           vertices, Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer), 9);
  self->modelNode->mesh->dirtyFlags |= VerticesDirty;
}

static void
createModel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  )
{
  Arcadia_SizeValue numberOfGlyphs = 0;
  Arcadia_SizeValue numberOfLines = 0;
  Arcadia_Engine_UI_TextUtilities_countGlyphsAndLines(thread, self->text, &numberOfGlyphs, &numberOfLines);
  if (!numberOfGlyphs) {
    return;
  }
  Arcadia_FontIO_AtlasBitmapFont* font = Arcadia_FontIO_AtlasBitmapFont_create(thread, Arcadia_String_createFromCxxString(thread, Arcadia_Engine_UI_TextUtilities_findFontPath(thread)), self->pixelSize);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)font);
  updateLineMetrics(thread, self, font);
  self->glyphs = Arcadia_Memory_allocateUnmanaged(thread, numberOfGlyphs * sizeof(Arcadia_FontIO_AtlasGlyphInformation));
  self->lines = Arcadia_Memory_allocateUnmanaged(thread, numberOfLines * sizeof(Arcadia_Engine_UI_TextLine));
  Arcadia_Engine_UI_TextUtilities_loadGlyphsAndLines(thread, font, self->text, self->glyphs, numberOfGlyphs, self->lines, numberOfLines);
  self->numberOfGlyphs = numberOfGlyphs;
  self->numberOfLines = numberOfLines;
  Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, font);
  self->fontAtlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
  self->fontAtlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);

  Arcadia_String* colorName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.Color");
  Arcadia_String* fillName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.Fill");
  Arcadia_String* pixelBufferName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.PixelBuffer");
  Arcadia_String* textureName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.Texture");
  Arcadia_String* meshName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.Mesh");
  Arcadia_String* materialName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.Material");
  Arcadia_String* modelName = Arcadia_String_createFromCxxString(thread, u8"UI.Label.Generated.Model");
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
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  self->modelNode = Arcadia_Engine_Visuals_NodeFactory_createModelNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, self->backendContext, model);
  Arcadia_Engine_Visuals_TextureNode* textureNode = self->modelNode->material->ambientColorTexture;
  textureNode->pixelBuffer->pixelBuffer = atlas;
  textureNode->width = self->fontAtlasWidth;
  textureNode->height = self->fontAtlasHeight;
  textureNode->dirtyBits = 0xff;
  Arcadia_Memory_deallocateUnmanaged(thread, positions);
  Arcadia_Memory_deallocateUnmanaged(thread, colors);
  Arcadia_Memory_deallocateUnmanaged(thread, textureCoordinates);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
Arcadia_Engine_UI_LabelNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_LabelNode);
  if (12 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->backendContext = Arcadia_ValueStack_isVoidValue(thread, 12) ? NULL : Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 12, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  if (self->backendContext) {
    Arcadia_Object_lock(thread, (Arcadia_Object*)self->backendContext);
  }
  self->text = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 11, _Arcadia_String_getType(thread));
  self->left = Arcadia_ValueStack_getInteger32Value(thread, 10);
  self->bottom = Arcadia_ValueStack_getInteger32Value(thread, 9);
  self->width = Arcadia_ValueStack_getNatural32Value(thread, 8);
  self->height = Arcadia_ValueStack_getNatural32Value(thread, 7);
  ((Arcadia_Engine_UI_WidgetNode*)self)->left = self->left;
  ((Arcadia_Engine_UI_WidgetNode*)self)->bottom = self->bottom;
  ((Arcadia_Engine_UI_WidgetNode*)self)->width = (Arcadia_Integer32Value)self->width;
  ((Arcadia_Engine_UI_WidgetNode*)self)->height = (Arcadia_Integer32Value)self->height;
  self->horizontalAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 6, _Arcadia_String_getType(thread));
  self->verticalAlignment = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 5, _Arcadia_String_getType(thread));
  self->red = Arcadia_ValueStack_getNatural8Value(thread, 4);
  self->green = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->blue = Arcadia_ValueStack_getNatural8Value(thread, 2);
  self->alpha = Arcadia_ValueStack_getNatural8Value(thread, 1);
  self->pixelSize = 24;
  self->generatedDefinitions = Arcadia_ADL_Definitions_create(thread);
  self->modelNode = NULL;
  self->glyphs = NULL;
  self->numberOfGlyphs = 0;
  self->lines = NULL;
  self->numberOfLines = 0;
  self->fontAtlasWidth = 0;
  self->fontAtlasHeight = 0;
  self->lineTop = 0;
  self->lineBottom = 0;
  createModel(thread, self);
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_LabelNode);
}

static void
Arcadia_Engine_UI_LabelNode_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  )
{
  if (self->glyphs) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->glyphs);
    self->glyphs = NULL;
  }
  if (self->lines) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->lines);
    self->lines = NULL;
  }
  if (self->backendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
    self->backendContext = NULL;
  }
}

static void
Arcadia_Engine_UI_LabelNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNodeDispatch* self
  )
{
  ((Arcadia_Engine_UI_WidgetNodeDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_UI_WidgetNode*, Arcadia_Engine_Visuals_EnterPassNode*))&Arcadia_Engine_UI_LabelNode_renderImpl;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = (void (*)(Arcadia_Thread*, Arcadia_Engine_Node*, Arcadia_Engine_Visuals_BackendContext*))&Arcadia_Engine_UI_LabelNode_setVisualsBackendContextImpl;
}

static void
Arcadia_Engine_UI_LabelNode_initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{ self->render = &Arcadia_Engine_UI_LabelNode_renderInterfaceImpl; }

static void
Arcadia_Engine_UI_LabelNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self
  )
{
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  if (self->horizontalAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->horizontalAlignment);
  if (self->verticalAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->verticalAlignment);
  if (self->generatedDefinitions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->generatedDefinitions);
  if (self->modelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->modelNode);
}

static void
Arcadia_Engine_UI_LabelNode_renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{ Arcadia_Engine_UI_LabelNode_renderImpl(thread, (Arcadia_Engine_UI_LabelNode*)self, enterPassNode); }

static void
Arcadia_Engine_UI_LabelNode_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  if (!self->modelNode) return;
  Arcadia_Real32Value canvasWidth, canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_getCanvasSize(thread, enterPassNode->viewportNode, &canvasWidth, &canvasHeight);
  updateVertices(thread, self, canvasWidth, canvasHeight);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->modelNode, enterPassNode);
}

static void
Arcadia_Engine_UI_LabelNode_setVisualsBackendContextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext
  )
{
  if (backendContext == self->backendContext) return;
  if (backendContext) Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
  if (self->backendContext) Arcadia_Object_unlock(thread, (Arcadia_Object*)self->backendContext);
  self->backendContext = backendContext;
  if (self->modelNode) {
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self->modelNode, backendContext);
  }
}

Arcadia_Engine_UI_LabelNode*
Arcadia_Engine_UI_LabelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_String* text,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width,
    Arcadia_Natural32Value height,
    Arcadia_String* horizontalAlignment,
    Arcadia_String* verticalAlignment,
    Arcadia_Natural8Value red,
    Arcadia_Natural8Value green,
    Arcadia_Natural8Value blue,
    Arcadia_Natural8Value alpha
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_LabelNode);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushNatural32Value(thread, width);
  Arcadia_ValueStack_pushNatural32Value(thread, height);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)horizontalAlignment);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)verticalAlignment);
  Arcadia_ValueStack_pushNatural8Value(thread, red);
  Arcadia_ValueStack_pushNatural8Value(thread, green);
  Arcadia_ValueStack_pushNatural8Value(thread, blue);
  Arcadia_ValueStack_pushNatural8Value(thread, alpha);
  Arcadia_ValueStack_pushNatural8Value(thread, 12);
  _Arcadia_EndCreate(Arcadia_Engine_UI_LabelNode);
}

Arcadia_Engine_UI_LabelNode*
Arcadia_Engine_UI_LabelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_LabelDefinition* source
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Engine_UI_LabelNode* node = Arcadia_Engine_UI_LabelNode_create(thread, backendContext, source->text, source->left, source->bottom,
                                                                         source->width, source->height, source->horizontalAlignment,
                                                                         source->verticalAlignment, source->red, source->green,
                                                                         source->blue, source->alpha);
  Arcadia_Engine_UI_LabelNode_setPixelSize(thread, node, source->pixelSize);
  Arcadia_ValueStack_popValues(thread, 1);
  return node;
}

void
Arcadia_Engine_UI_LabelNode_setText
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_String* text
  )
{
  if (!self->modelNode) {
    self->text = text;
    createModel(thread, self);
    return;
  }
  Arcadia_Media_VertexBuffer* vertexBuffer = Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->modelNode->mesh);
  Arcadia_SizeValue const maximumNumberOfGlyphs = Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, vertexBuffer) / 6;
  Arcadia_SizeValue numberOfGlyphs = 0;
  Arcadia_SizeValue numberOfLines = 0;
  Arcadia_Engine_UI_TextUtilities_countGlyphsAndLines(thread, text, &numberOfGlyphs, &numberOfLines);
  if (numberOfGlyphs > maximumNumberOfGlyphs) {
    numberOfGlyphs = maximumNumberOfGlyphs;
  }
  Arcadia_FontIO_AtlasGlyphInformation* glyphs = NULL;
  Arcadia_Engine_UI_TextLine* lines = NULL;
  Arcadia_FontIO_AtlasBitmapFont* font = Arcadia_FontIO_AtlasBitmapFont_create(thread, Arcadia_String_createFromCxxString(thread, Arcadia_Engine_UI_TextUtilities_findFontPath(thread)), self->pixelSize);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)font);
  updateLineMetrics(thread, self, font);
  if (numberOfGlyphs) {
    glyphs = Arcadia_Memory_allocateUnmanaged(thread, numberOfGlyphs * sizeof(Arcadia_FontIO_AtlasGlyphInformation));
    lines = Arcadia_Memory_allocateUnmanaged(thread, numberOfLines * sizeof(Arcadia_Engine_UI_TextLine));
    Arcadia_Engine_UI_TextUtilities_loadGlyphsAndLines(thread, font, text, glyphs, numberOfGlyphs, lines, numberOfLines);
  }
  if (self->glyphs) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->glyphs);
  }
  if (self->lines) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->lines);
  }
  self->text = text;
  self->glyphs = glyphs;
  self->numberOfGlyphs = numberOfGlyphs;
  self->lines = lines;
  self->numberOfLines = numberOfLines;
  Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, font);
  self->fontAtlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
  self->fontAtlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);
  Arcadia_Engine_Visuals_TextureNode* textureNode = self->modelNode->material->ambientColorTexture;
  textureNode->pixelBuffer->pixelBuffer = atlas;
  textureNode->width = self->fontAtlasWidth;
  textureNode->height = self->fontAtlasHeight;
  textureNode->dirtyBits = 0xff;
  Arcadia_ValueStack_popValues(thread, 1);
}

void
Arcadia_Engine_UI_LabelNode_setPixelSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LabelNode* self,
    Arcadia_Natural32Value pixelSize
  )
{
  if (!pixelSize) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->pixelSize == pixelSize) {
    return;
  }
  self->pixelSize = pixelSize;
  if (self->glyphs) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->glyphs);
    self->glyphs = NULL;
  }
  if (self->lines) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->lines);
    self->lines = NULL;
  }
  self->numberOfGlyphs = 0;
  self->numberOfLines = 0;
  self->generatedDefinitions = Arcadia_ADL_Definitions_create(thread);
  self->modelNode = NULL;
  createModel(thread, self);
}

#undef VerticesDirty
