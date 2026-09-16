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

#include "Arcadia/Engine/Examples/FontTexture/Scenes/FontTextureScene.h"

#include "Arcadia/Engine/Examples/FontTexture/AssetUtilities.h"

#if defined(ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE) && 1 == ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE
  #include "Arcadia/FontIO/Include.h"
#endif

static void
Arcadia_Engine_Demo_MainMenuScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  );

static void
Arcadia_Engine_Demo_MainMenuScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuSceneDispatch* self
  );

static void
Arcadia_Engine_Demo_MainMenuScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  );

static void
Arcadia_Engine_Demo_MainMenuScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_MainMenuScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Demo_MainMenuScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_MainMenuScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_Demo_MainMenuScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_Demo_MainMenuScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static void
setFontQuad
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );


static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_MainMenuScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_MainMenuScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_MainMenuScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.MainMenuScene", Arcadia_Engine_Demo_MainMenuScene,
                         u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                         &_Arcadia_Engine_Demo_MainMenuScene_typeOperations);

static void
Arcadia_Engine_Demo_MainMenuScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_MainMenuScene);
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
  self->fontAtlasWidth = 0;
  self->fontAtlasHeight = 0;
  self->fontQuadCanvasWidth = -1;
  self->fontQuadCanvasHeight = -1;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_MainMenuScene);
}

static void
Arcadia_Engine_Demo_MainMenuScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuSceneDispatch* self
  )
{
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_MainMenuScene_updateAudialsImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Demo_MainMenuScene_updateLogicsImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_MainMenuScene_updateVisualsImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*)) & Arcadia_Engine_Demo_MainMenuScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MouseButtonEvent*)) & Arcadia_Engine_Demo_MainMenuScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MousePointerEvent*)) & Arcadia_Engine_Demo_MainMenuScene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_Demo_MainMenuScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
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
}

static void
setFontQuad
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Integer32Value const atlasWidth = self->fontAtlasWidth;
  Arcadia_Integer32Value const atlasHeight = self->fontAtlasHeight;
  Arcadia_Integer32Value scaleX = width / atlasWidth;
  Arcadia_Integer32Value scaleY = height / atlasHeight;
  Arcadia_Integer32Value scale = scaleX < scaleY ? scaleX : scaleY;
  if (scale < 1) {
    scale = 1;
  }
  Arcadia_Integer32Value const quadWidth = scale * atlasWidth;
  Arcadia_Integer32Value const quadHeight = scale * atlasHeight;
  Arcadia_Integer32Value const x0 = (width - quadWidth) / 2;
  Arcadia_Integer32Value const y0 = (height - quadHeight) / 2;
  Arcadia_Integer32Value const x1 = x0 + quadWidth;
  Arcadia_Integer32Value const y1 = y0 + quadHeight;
  Arcadia_Real32Value const u0 = 0.5f / (Arcadia_Real32Value)atlasWidth;
  Arcadia_Real32Value const u1 = ((Arcadia_Real32Value)atlasWidth - 0.5f) / (Arcadia_Real32Value)atlasWidth;
  Arcadia_Real32Value const v0 = 0.5f / (Arcadia_Real32Value)atlasHeight;
  Arcadia_Real32Value const v1 = ((Arcadia_Real32Value)atlasHeight - 0.5f) / (Arcadia_Real32Value)atlasHeight;
  Arcadia_Media_VertexBuffer* vertexBuffer = self->modelNode->mesh->vertexBuffer;
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)vertexBuffer->vertices;
  Arcadia_Real32Value const vertexData[6][5] = {
    { (Arcadia_Real32Value)x0, (Arcadia_Real32Value)y0, 0.f, u0, v1 },
    { (Arcadia_Real32Value)x1, (Arcadia_Real32Value)y0, 0.f, u1, v1 },
    { (Arcadia_Real32Value)x0, (Arcadia_Real32Value)y1, 0.f, u0, v0 },
    { (Arcadia_Real32Value)x1, (Arcadia_Real32Value)y0, 0.f, u1, v1 },
    { (Arcadia_Real32Value)x1, (Arcadia_Real32Value)y1, 0.f, u1, v0 },
    { (Arcadia_Real32Value)x0, (Arcadia_Real32Value)y1, 0.f, u0, v0 },
  };
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    vertices[i * 9 + 0] = vertexData[i][0];
    vertices[i * 9 + 1] = vertexData[i][1];
    vertices[i * 9 + 2] = vertexData[i][2];
    vertices[i * 9 + 7] = vertexData[i][3];
    vertices[i * 9 + 8] = vertexData[i][4];
  }
  self->modelNode->mesh->dirtyFlags |= 1;
}

static void
setFontTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
#if defined(ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE) && 1 == ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE
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
    for (Arcadia_Natural32Value codePoint = 32; codePoint < 127; ++codePoint) {
      Arcadia_FontIO_AtlasGlyphInformation glyphInformation;
      Arcadia_FontIO_AtlasBitmapFont_getGlyphInformation(thread, font, codePoint, &glyphInformation);
    }

    Arcadia_Media_PixelBuffer* atlas = Arcadia_FontIO_AtlasBitmapFont_getAtlas(thread, font);
    Arcadia_Engine_Visuals_TextureNode* textureNode = self->modelNode->material->ambientColorTexture;
    textureNode->pixelBuffer->pixelBuffer = atlas;
    textureNode->width = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
    textureNode->height = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);
    textureNode->dirtyBits = 0xff;
    self->fontAtlasWidth = Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, atlas);
    self->fontAtlasHeight = Arcadia_Media_PixelBuffer_getNumberOfRows(thread, atlas);

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
#else
  (void)thread;
  (void)self;
#endif
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
  if (!self->definitions) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/MainMenuScene")), files);
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
    Arcadia_ADL_ModelDefinition* MODELS[] =
    {
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/MainMenuScene/TextureColorModel.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "MainMenuScene.TextureColorModel")),
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
    setFontTexture(thread, self);
  }
}

static void
Arcadia_Engine_Demo_MainMenuScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
}

static void
Arcadia_Engine_Demo_MainMenuScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_MainMenuScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
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
  if (0 != self->fontAtlasWidth && 0 != self->fontAtlasHeight &&
      (self->fontQuadCanvasWidth != width || self->fontQuadCanvasHeight != height)) {
    setFontQuad(thread, self, width, height);
    self->fontQuadCanvasWidth = width;
    self->fontQuadCanvasHeight = height;
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
Arcadia_Engine_Demo_MainMenuScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
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
  } else if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
    Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_R) {
    Arcadia_logf(Arcadia_LogFlags_Info, "re-initializing backends\n");
  }
}

static void
Arcadia_Engine_Demo_MainMenuScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_MainMenuScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Demo_MainMenuScene*
Arcadia_Engine_Demo_MainMenuScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_MainMenuScene);
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
  _Arcadia_EndCreate(Arcadia_Engine_Demo_MainMenuScene);
}
