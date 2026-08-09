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

#include "Arcadia/Engine/Demo/Scenes/ArcadiaLogoScene.h"

#include "Arcadia/Engine/Demo/SceneManager.h"
#include "Arcadia/Engine/Demo/Scenes/MainScene.h"
#include "Arcadia/Engine/Demo/Scenes/MainMenuScene.h"
#include "Arcadia/Engine/Demo/AssetUtilities.h"

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self
  );

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoSceneDispatch* self
  );

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self
  );

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_ArcadiaLogoScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_ArcadiaLogoScene_construct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_ArcadiaLogoScene_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_ArcadiaLogoScene_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_ArcadiaLogoScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_ArcadiaLogoScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.ArcadiaLogoScene", Arcadia_Engine_Demo_ArcadiaLogoScene,
                         u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                         &_Arcadia_Engine_Demo_ArcadiaLogoScene_typeOperations);

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_ArcadiaLogoScene);
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
  self->duration = 0.;
  //
  self->frameBufferNode = NULL;
  //
  self->viewportNode = NULL;
  self->cameraNode = NULL;
  self->enterPassNode = NULL;
  //
  self->modelNode = NULL;
  //
  self->definitions = Arcadia_ADL_Definitions_create(thread);
  //
  self->toLoad = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  //
  Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS")), self->toLoad);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_ArcadiaLogoScene);
}

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoSceneDispatch* self
  )
{
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_ArcadiaLogoScene_updateAudials;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Demo_ArcadiaLogoScene_updateLogics;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_ArcadiaLogoScene_updateVisuals;
}

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self
  )
{
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }

  if (self->frameBufferNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->frameBufferNode);
  }

  if (self->viewportNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
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

  if (self->toLoad) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->toLoad);
  }
}

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick
  )
{
  self->duration += tick;
  /* The loading screen fades in and fades out over a duration of 3 secods. After 3.25 seconds we move to the next scene. */
  if (self->duration > 3250 && 0 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->toLoad)) {
    //Arcadia_Engine_Demo_Scene* nextScene = (Arcadia_Engine_Demo_Scene*)Arcadia_Engine_Demo_MainScene_create(thread, ((Arcadia_Engine_Demo_Scene*)self)->engine, ((Arcadia_Engine_Demo_Scene*)self)->sceneManager);
    Arcadia_Engine_Demo_Scene* nextScene = (Arcadia_Engine_Demo_Scene*)Arcadia_Engine_Demo_MainMenuScene_create(thread, ((Arcadia_Engine_Demo_Scene*)self)->engine, ((Arcadia_Engine_Demo_Scene*)self)->sceneManager);
    Arcadia_Engine_Demo_SceneManager_setScene(thread, ((Arcadia_Engine_Demo_Scene*)self)->sceneManager, nextScene);
  }
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->toLoad);
  if (n) {
    Arcadia_SizeValue i = n - 1;
    Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->toLoad, i, _Arcadia_FilePath_getType(thread));
    Arcadia_List_removeAt(thread, self->toLoad, i, 1);
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, self->definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_String* temporary = Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True);
      Arcadia_logf(Arcadia_LogFlags_Debug, "problem loading asset `%s`\n", Arcadia_String_getBytes(thread, temporary));
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
updateVisuals0
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;

  // Ensure the "frame buffer" node exist.
  if (!self->frameBufferNode) {
    self->frameBufferNode =
      Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }
  // Ensure the "rendering context" node exists.
  if (!self->enterPassNode) {
    self->enterPassNode =
      Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }
  // Ensure the "viewport" node exists.
  // Set its clear color and relative viewport rectangle.
  if (!self->viewportNode) {
    Arcadia_ADL_ColorDefinition* d = getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Green.adl"),
                                                                                   Arcadia_String_createFromCxxString(thread, "Assets/Colors.Green"));
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
  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Ensure the "camera" node exists.
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
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, -1, +1, -1, +1, -1, +1);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  if (!self->modelNode) {
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/LogoScene")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, self->definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }
    Arcadia_ADL_ModelDefinition* modelDefinition =
      (Arcadia_ADL_ModelDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "LogoScene.MeshColorModel"));
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
  }
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);
}

static void
updateVisualsFrameBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  updateVisuals0(thread, self, tick, width, height);
  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;
  // Ensure the "frame buffer" node exist.
  if (!self->frameBufferNode) {
    self->frameBufferNode =
      Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
        );
  }
  // Assign the "frame buffer" node to the "rendering context" node.
  Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode(thread, self->enterPassNode, self->frameBufferNode);
  // Render the scene.
  Arcadia_Engine_Visuals_renderScene(thread, self->enterPassNode, self->modelNode, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
}

static void
updateVisualsDefaultFrameBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  updateVisuals0(thread, self, tick, width, height);
  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;
  // Assign the "frame buffer" node to the "rendering context" node.
  Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode(thread, self->enterPassNode, NULL);
  // Render the scene.
  Arcadia_Engine_Visuals_renderScene(thread, self->enterPassNode, self->modelNode, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
}

static void
Arcadia_Engine_Demo_ArcadiaLogoScene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_ArcadiaLogoScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  updateVisualsFrameBuffer(thread, self, tick, width, height);
  updateVisualsDefaultFrameBuffer(thread, self, tick, width, height);
}

Arcadia_Engine_Demo_ArcadiaLogoScene*
Arcadia_Engine_Demo_ArcadiaLogoScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_ArcadiaLogoScene);
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
  _Arcadia_EndCreate(Arcadia_Engine_Demo_ArcadiaLogoScene);
}
