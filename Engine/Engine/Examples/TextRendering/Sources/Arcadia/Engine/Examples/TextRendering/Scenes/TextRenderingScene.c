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

#include "Arcadia/Engine/Examples/TextRendering/Scenes/TextRenderingScene.h"

#include "Arcadia/Engine/Examples/TextRendering/AssetUtilities.h"

#include "Arcadia/FontIO/Include.h"

static void
Arcadia_Engine_Demo_TextRenderingScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingSceneDispatch* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static void
setGlyphQuads
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
setFontTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  );

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_TextRenderingScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_TextRenderingScene_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_TextRenderingScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_TextRenderingScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.TextRenderingScene", Arcadia_Engine_Demo_TextRenderingScene,
                         u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                         &_Arcadia_Engine_Demo_TextRenderingScene_typeOperations);

static void
Arcadia_Engine_Demo_TextRenderingScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_TextRenderingScene);
  {
    Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 1),
                  sceneManager = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &engine);
    Arcadia_ValueStack_pushValue(thread, &sceneManager);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->definitions = NULL;
  //
  self->cameraNode = NULL;
  self->enterPassNode = NULL;
  self->modelNode = NULL;
  self->viewportNode = NULL;
  //
  self->text = NULL;
  //
  self->fontAtlasWidth = 0;
  self->fontAtlasHeight = 0;
  //
  self->glyphs = NULL;
  self->numberOfGlyphs = 0;
  //
  self->textCanvasWidth = -1;
  self->textCanvasHeight = -1;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_TextRenderingScene);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  if (self->glyphs) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->glyphs);
    self->glyphs = NULL;
    self->numberOfGlyphs = 0;
  }
}

static void
Arcadia_Engine_Demo_TextRenderingScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingSceneDispatch* self
  )
{
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_TextRenderingScene_updateAudialsImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Demo_TextRenderingScene_updateLogicsImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_TextRenderingScene_updateVisualsImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*)) & Arcadia_Engine_Demo_TextRenderingScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MouseButtonEvent*)) & Arcadia_Engine_Demo_TextRenderingScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MousePointerEvent*)) & Arcadia_Engine_Demo_TextRenderingScene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_Demo_TextRenderingScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }

  if (self->cameraNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  }
  if (self->enterPassNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enterPassNode);
  }
  if (self->modelNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->modelNode);
  }
  if (self->viewportNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  }
  if (self->text) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  }
}

static void
setGlyphQuads
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Real32Value const atlasWidth = (Arcadia_Real32Value)self->fontAtlasWidth;
  Arcadia_Real32Value const atlasHeight = (Arcadia_Real32Value)self->fontAtlasHeight;

  // Compute the bounding box of the text relative to the baseline.
  // The top-most edge has the largest bearingY, the bottom-most edge has the smallest (bearingY - height).
  Arcadia_Integer32Value top = 0;
  Arcadia_Integer32Value bottom = 0;
  Arcadia_Integer32Value firstBearingX = 0;
  Arcadia_Integer32Value totalAdvanceX = 0;
  for (Arcadia_SizeValue i = 0; i < self->numberOfGlyphs; ++i) {
    Arcadia_FontIO_GlyphInformation const* glyphInformation = &self->glyphs[i].glyphInformation;
    if (0 == i) {
      firstBearingX = glyphInformation->bearingX;
    }
    totalAdvanceX += glyphInformation->advanceX;
    if (glyphInformation->bearingY > top) {
      top = glyphInformation->bearingY;
    }
    Arcadia_Integer32Value const bottomEdge = glyphInformation->bearingY - glyphInformation->height;
    if (bottomEdge < bottom) {
      bottom = bottomEdge;
    }
  }

  // Center the text horizontally and vertically.
  // The canvas is a right-handed coordinate system with origin at the lower-left corner:
  // x grows to the right, y grows upwards.
  // The projection is a pixel-aligned orthographic projection (one world unit is one canvas pixel).
  // All glyph edges are therefore aligned to integer canvas coordinates so that, with the
  // Nearest-filtered atlas, every glyph texel maps exactly onto one canvas pixel.
  Arcadia_Integer32Value const penX = (width - (firstBearingX + totalAdvanceX)) / 2;
  Arcadia_Integer32Value const baselineY = (height - (top + bottom)) / 2;

  Arcadia_Media_VertexBuffer* vertexBuffer = self->modelNode->mesh->vertexBuffer;
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)vertexBuffer->vertices;
  Arcadia_Integer32Value penXCurrent = penX;
  for (Arcadia_SizeValue i = 0; i < self->numberOfGlyphs; ++i) {
    Arcadia_FontIO_GlyphInformation const* glyphInformation = &self->glyphs[i].glyphInformation;
    Arcadia_Integer32Value const width0 = glyphInformation->width;
    Arcadia_Integer32Value const height0 = glyphInformation->height;
    Arcadia_Integer32Value const bearingX = glyphInformation->bearingX;
    Arcadia_Integer32Value const bearingY = glyphInformation->bearingY;

    Arcadia_Real32Value const x0 = (Arcadia_Real32Value)(penXCurrent + bearingX);
    Arcadia_Real32Value const x1 = x0 + (Arcadia_Real32Value)width0;
    Arcadia_Real32Value const yTop = (Arcadia_Real32Value)(baselineY + bearingY);
    Arcadia_Real32Value const yBottom = yTop - (Arcadia_Real32Value)height0;

    Arcadia_Real32Value const u0 = ((Arcadia_Real32Value)self->glyphs[i].x + 0.5f) / atlasWidth;
    Arcadia_Real32Value const u1 = ((Arcadia_Real32Value)self->glyphs[i].x + (Arcadia_Real32Value)width0 - 0.5f) / atlasWidth;
    Arcadia_Real32Value const v0 = ((Arcadia_Real32Value)self->glyphs[i].y + 0.5f) / atlasHeight;
    Arcadia_Real32Value const v1 = ((Arcadia_Real32Value)self->glyphs[i].y + (Arcadia_Real32Value)height0 - 0.5f) / atlasHeight;

    Arcadia_Real32Value const vertexData[6][5] = {
      { x0, yBottom, 0.f, u0, v1 },
      { x1, yBottom, 0.f, u1, v1 },
      { x0, yTop,    0.f, u0, v0 },
      { x1, yBottom, 0.f, u1, v1 },
      { x1, yTop,    0.f, u1, v0 },
      { x0, yTop,    0.f, u0, v0 },
    };
    for (Arcadia_SizeValue j = 0; j < 6; ++j) {
      Arcadia_SizeValue const base = (i * 6 + j) * 9;
      vertices[base + 0] = vertexData[j][0];
      vertices[base + 1] = vertexData[j][1];
      vertices[base + 2] = vertexData[j][2];
      vertices[base + 7] = vertexData[j][3];
      vertices[base + 8] = vertexData[j][4];
    }

    penXCurrent += glyphInformation->advanceX;
  }
  self->modelNode->mesh->dirtyFlags |= 1;
}

static void
setFontTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  static char const* fallbackFontPaths[] = {
    "C:/Windows/Fonts/arial.ttf",
    "C:/Windows/Fonts/segoeui.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
  };
  char const* fontPath = NULL;
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  for (Arcadia_SizeValue i = 0; i < sizeof(fallbackFontPaths) / sizeof(fallbackFontPaths[0]); ++i) {
    if (Arcadia_FileSystem_regularFileExists(thread, fileSystem,
                                             Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, fallbackFontPaths[i])))) {
      fontPath = fallbackFontPaths[i];
      break;
    }
  }
  if (!fontPath) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_FontIO_AtlasBitmapFont* font = NULL;
  Arcadia_BooleanValue fontLocked = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    font = Arcadia_FontIO_AtlasBitmapFont_create(thread, Arcadia_String_createFromCxxString(thread, fontPath), 32);
    Arcadia_Object_lock(thread, (Arcadia_Object*)font);
    fontLocked = Arcadia_BooleanValue_True;
    // Warm all glyphs of the text. Only after warming all glyphs the atlas positions are stable.
    for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
      Arcadia_FontIO_AtlasGlyphInformation glyphInformation;
      Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, codePoints[i], &glyphInformation);
    }
    // Install the atlas as the ambient color texture of the model.
    Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, font);
    Arcadia_Engine_Visuals_TextureNode* textureNode = self->modelNode->material->ambientColorTexture;
    textureNode->pixelBuffer->pixelBuffer = atlas;
    textureNode->width = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
    textureNode->height = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);
    textureNode->dirtyBits = 0xff;
    self->fontAtlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
    self->fontAtlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);
    // Fetch the glyph information of all code points of the text.
    Arcadia_FontIO_AtlasGlyphInformation* glyphs = Arcadia_Memory_allocateUnmanaged(thread, numberOfCodePoints * sizeof(Arcadia_FontIO_AtlasGlyphInformation));
    for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
      Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, codePoints[i], &glyphs[i]);
    }
    self->numberOfGlyphs = numberOfCodePoints;
    self->glyphs = glyphs;

    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)font);
    font = NULL;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (fontLocked) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)font);
      font = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_SizeValue
countCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  Arcadia_SizeValue numberOfCodePoints = 0;
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    ++numberOfCodePoints;
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  }
  return numberOfCodePoints;
}

static void
writeCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String* string,
    Arcadia_Natural32Value* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  for (Arcadia_SizeValue i = 0; i < numberOfCodePoints; ++i) {
    codePoints[i] = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    if (i + 1 < numberOfCodePoints) {
      Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    }
  }
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self
  )
{
  if (!self->definitions) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/TextRenderingScene")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/Colors")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }

    self->definitions = definitions;
  }

  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;

  if (!self->enterPassNode) {
    self->enterPassNode =
      Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }

  if (!self->viewportNode) {
    Arcadia_ADL_ColorDefinition* CLEARCOLORS[] =
    {
      getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Red.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Colors.Red")),
    };

    Arcadia_ADL_ColorDefinition* d = CLEARCOLORS[0];
    self->viewportNode =
      (Arcadia_Engine_Visuals_ViewportNode*)
      Arcadia_Engine_Visuals_NodeFactory_createViewportNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
    Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNode, Arcadia_Math_Color4Real32_create4(thread, d->red / 255.f, d->green / 255.f, d->blue / 255.f, 1.f));
    Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNode, 0.f, 0.f, 1.f, 1.f);
  }

  if (!self->cameraNode) {
    self->cameraNode =
      (Arcadia_Engine_Visuals_CameraNode*)
      Arcadia_Engine_Visuals_NodeFactory_createCameraNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }

  if (!self->modelNode) {
    // (1) The text.
    self->text = Arcadia_String_createFromCxxString(thread, "Hello, World!");
    // (2) Parse the text into an array of code points.
    Arcadia_SizeValue const numberOfCodePoints = countCodePoints(thread, self->text);
    Arcadia_Natural32Value* codePoints = Arcadia_Memory_allocateUnmanaged(thread, numberOfCodePoints * sizeof(Arcadia_Natural32Value));
    writeCodePoints(thread, self->text, codePoints, numberOfCodePoints);

    // (3) Build the mesh definition.
    Arcadia_SizeValue const numberOfVertices = 6 * numberOfCodePoints;
    Arcadia_SizeValue const numberOfPositionComponents = 3 * numberOfVertices;
    Arcadia_SizeValue const numberOfColorComponents = 4 * numberOfVertices;
    Arcadia_SizeValue const numberOfTextureCoordinatesComponents = 2 * numberOfVertices;
    Arcadia_Real32Value* positions = Arcadia_Memory_allocateUnmanaged(thread, numberOfPositionComponents * sizeof(Arcadia_Real32Value));
    Arcadia_Real32Value* colors = Arcadia_Memory_allocateUnmanaged(thread, numberOfColorComponents * sizeof(Arcadia_Real32Value));
    Arcadia_Real32Value* textureCoordinates = Arcadia_Memory_allocateUnmanaged(thread, numberOfTextureCoordinatesComponents * sizeof(Arcadia_Real32Value));
    for (Arcadia_SizeValue i = 0; i < numberOfPositionComponents; ++i) {
      positions[i] = 0.f;
    }
    for (Arcadia_SizeValue i = 0; i < numberOfColorComponents; ++i) {
      colors[i] = 1.f;
    }
    for (Arcadia_SizeValue i = 0; i < numberOfTextureCoordinatesComponents; ++i) {
      textureCoordinates[i] = 0.f;
    }
    Arcadia_RuntimeByteArray* positionArray = Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)positions, numberOfPositionComponents * sizeof(Arcadia_Real32Value));
    Arcadia_RuntimeByteArray* colorArray = Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)colors, numberOfColorComponents * sizeof(Arcadia_Real32Value));
    Arcadia_RuntimeByteArray* textureCoordinatesArray = Arcadia_RuntimeByteArray_create(thread, (Arcadia_Natural8Value const*)textureCoordinates, numberOfTextureCoordinatesComponents * sizeof(Arcadia_Real32Value));
    Arcadia_Memory_deallocateUnmanaged(thread, positions);
    Arcadia_Memory_deallocateUnmanaged(thread, colors);
    Arcadia_Memory_deallocateUnmanaged(thread, textureCoordinates);

    Arcadia_ADL_MeshDefinition* meshDefinition = Arcadia_ADL_MeshDefinition_create(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "TextRenderingScene.Mesh"),
                                                                                   numberOfVertices, positionArray, colorArray, textureCoordinatesArray,
                                                                                   Arcadia_String_createFromCxxString(thread, "Colors.White"));
    // (4) Register the mesh into the definitions.
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)meshDefinition);
    Arcadia_Map_set(thread, self->definitions->definitions,
                    Arcadia_Value_makeObjectReferenceValue(((Arcadia_ADL_Definition*)meshDefinition)->name),
                    Arcadia_Value_makeObjectReferenceValue(meshDefinition), NULL, NULL);
    Arcadia_ValueStack_popValues(thread, 1);

    // (5) Create the model node.
    Arcadia_ADL_ModelDefinition* MODELS[] =
    {
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/TextRenderingScene/TextureColorModel.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "TextRenderingScene.TextureColorModel")),
    };
    Arcadia_ADL_ModelDefinition* modelDefinition = MODELS[0];
    if (NULL == modelDefinition) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)modelDefinition);
    self->modelNode =
      (Arcadia_Engine_Visuals_ModelNode*)
      Arcadia_Engine_Visuals_NodeFactory_createModelNode
      (
        thread,
        (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
        (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
        modelDefinition
      );

    // (6) Install the font atlas and fetch the glyph information.
    setFontTexture(thread, self, codePoints, numberOfCodePoints);
    Arcadia_Memory_deallocateUnmanaged(thread, codePoints);
  }
}

static void
Arcadia_Engine_Demo_TextRenderingScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_TextRenderingScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);

  // We use a right-handed coordinate system.
  // -negative z-axis forward, positive z-axis backward
  // -negative y-axis down, positive y-axis up
  // -negative x-axis left, positive x-axis right
  // Viewer located at (0,0,+1).
  // The projection is a pixel-aligned orthographic projection:
  // one world unit corresponds to one pixel of the canvas.
  if (NULL != self->glyphs &&
      0 != self->fontAtlasWidth && 0 != self->fontAtlasHeight &&
      (self->textCanvasWidth != width || self->textCanvasHeight != height)) {
    setGlyphQuads(thread, self, width, height);
    self->textCanvasWidth = width;
    self->textCanvasHeight = height;
  }
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, 0.f, (Arcadia_Real32Value)width, 0.f, (Arcadia_Real32Value)height, -1.f, 1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);

  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);

  Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode(thread, self->enterPassNode, NULL);
  Arcadia_Engine_Visuals_renderScene(thread, self->enterPassNode, self->modelNode, (Arcadia_Engine_Visuals_BackendContext*)((Arcadia_Engine_Demo_Scene*)self)->engine->visualsBackendContext);
}

static void
Arcadia_Engine_Demo_TextRenderingScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
    Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Escape) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* e = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, Arcadia_getTickCount(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread,  (Arcadia_Object*)e);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Demo_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
    Arcadia_ValueStack_popValues(thread, 2);
  }
}

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_TextRenderingScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_TextRenderingScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Demo_TextRenderingScene*
Arcadia_Engine_Demo_TextRenderingScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_TextRenderingScene);
  if (engine) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (sceneManager) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sceneManager);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_Demo_TextRenderingScene);
}