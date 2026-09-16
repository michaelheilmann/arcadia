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

#include "Arcadia/Engine/Examples/CheckerboardTexture/Scenes/CheckerboardTextureScene.h"

#include "Arcadia/Engine/Examples/CheckerboardTexture/AssetUtilities.h"

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
  // Variant #1: Perspective projection looking down the negative z-axis.
  // Variant #2: Orthographic projection looking down the negative z-axis.
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setPerspectiveProjection(thread, viewToProjectionMatrix, 60.f, 4.f/3.f, 0.1f, 100.f);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, -1, +1, -1, +1, -1, +1);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);

  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);

  // Render the enter pass node.
  // Pass mesh nodes to the enter pass node.
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->enterPassNode, (Arcadia_Engine_Visuals_EnterPassNode*)self->enterPassNode);
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
