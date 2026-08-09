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

#include "Arcadia/Engine/Demo/Scenes/MainScene.h"

#include "Arcadia/Engine/Demo/AssetUtilities.h"

static void
Arcadia_Engine_Demo_MainScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self
  );

static void
Arcadia_Engine_Demo_MainScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainSceneDispatch* self
  );

static void
Arcadia_Engine_Demo_MainScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self
  );

static void
Arcadia_Engine_Demo_MainScene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_MainScene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Demo_MainScene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Demo_MainScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_Demo_MainScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_Demo_MainScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_MainScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_MainScene_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_MainScene_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_MainScene_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_MainScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_MainScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.MainScene", Arcadia_Engine_Demo_MainScene,
                         u8"Arcadia.Engine.Demo.Scene", Arcadia_Engine_Demo_Scene,
                         &_Arcadia_Engine_Demo_MainScene_typeOperations);

static void
Arcadia_Engine_Demo_MainScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Demo_MainScene);
  {
    Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 1),
                  sceneManager = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &engine);
    Arcadia_ValueStack_pushValue(thread, &sceneManager);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (Arcadia_ValueStack_getSize(thread) < 1 || 2 != Arcadia_ValueStack_getNatural8Value(thread, 0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->definitions = NULL;
  //
  self->cameraNode = NULL;
  self->enterPassNode = NULL;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->modelNode[i] = NULL;
    self->viewportNodes[i] = NULL;
  }
  //
  self->soundSourceNode = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_MainScene);
}

static void
Arcadia_Engine_Demo_MainScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainSceneDispatch* self
  )
{
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_MainScene_updateAudials;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Demo_MainScene_updateLogics;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_MainScene_updateVisuals;

  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*)) & Arcadia_Engine_Demo_MainScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MouseButtonEvent*)) & Arcadia_Engine_Demo_MainScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_Demo_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Demo_Scene*, Arcadia_Engine_Input_MousePointerEvent*)) & Arcadia_Engine_Demo_MainScene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_Demo_MainScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self
  )
{
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }

  if (self->enterPassNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enterPassNode);
  }

  if (self->cameraNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  }

  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    if (self->modelNode[i]) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->modelNode[i]);
    }
    if (self->viewportNodes[i]) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNodes[i]);
    }
  }

  if (self->soundSourceNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->soundSourceNode);
  }
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self
  )
{ 
  if (!self->definitions) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/MainScene")), files);
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
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/DefaultWall")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }

    self->definitions = definitions;
  }
}

static void
Arcadia_Engine_Demo_MainScene_updateAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;
  if (!self->soundSourceNode) {
    Arcadia_ADL_SampleBufferDefinition* SAMPLEBUFFERS[] =
    {
      getSampleBufferDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, u8"Assets/MainScene/AmbienceSampleBuffer.adl"),
                                                           Arcadia_String_createFromCxxString(thread, u8"MainScene.AmbienceSampleBuffer")),
    };
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)SAMPLEBUFFERS[0]);
    self->soundSourceNode =
      Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode
        (
          thread,
          (Arcadia_Engine_Audials_NodeFactory*)engine->audialsNodeFactory,
          (Arcadia_Engine_Audials_BackendContext*)engine->audialsBackendContext,
          SAMPLEBUFFERS[0]
        );
  }
  Arcadia_Engine_Node_setAudialsBackendContext(thread, (Arcadia_Engine_Node*)self->soundSourceNode, (Arcadia_Engine_Audials_BackendContext*)engine->audialsBackendContext);
  Arcadia_Engine_Audials_Node_render(thread, (Arcadia_Engine_Audials_Node*)self->soundSourceNode);
  Arcadia_Engine_Audials_SoundSourceNode_setVolume(thread, self->soundSourceNode, 0.125f);
  Arcadia_Engine_Audials_SoundSourceNode_setIsLooping(thread, self->soundSourceNode, Arcadia_BooleanValue_True);
  if (!Arcadia_Engine_Audials_SoundSourceNode_isPlaying(thread, self->soundSourceNode)) {
    Arcadia_Engine_Audials_SoundSourceNode_stop(thread, self->soundSourceNode);
    Arcadia_Engine_Audials_SoundSourceNode_play(thread, self->soundSourceNode);
  }
}

static void
Arcadia_Engine_Demo_MainScene_updateLogics
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Demo_MainScene_updateVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
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

  Arcadia_ADL_ColorDefinition* CLEARCOLORS[] =
    {
      getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Red.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Colors.Red")),
      getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Green.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Colors.Green")),
      getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/Blue.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Colors.Blue")),
    };

  Arcadia_ADL_ModelDefinition* MODELS[] =
    {
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/DefaultWall/Model.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Assets.DefaultWall.Model")),
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/DefaultWall/Model.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Assets.DefaultWall.Model")),
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/DefaultWall/Model.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Assets.DefaultWall.Model")),
    };

  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    if (!self->viewportNodes[i]) {
      Arcadia_ADL_ColorDefinition* d = CLEARCOLORS[i];
      self->viewportNodes[i] =
        (Arcadia_Engine_Visuals_ViewportNode*)
        Arcadia_Engine_Visuals_NodeFactory_createViewportNode
          (
            thread,
            (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
            (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext
          );
      Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNodes[i], Arcadia_Math_Color4Real32_create4(thread, d->red / 255.f, d->green / 255.f, d->blue / 255.f, 1.f));
      Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNodes[i], (i + 0) * 1.f / 3.f, 0.f, (i + 1) * 1.f / 3.f, 1.f);
    }

    if (!self->modelNode[i]) {
      Arcadia_ADL_ModelDefinition* modelDefinition = MODELS[i];
      if (NULL == modelDefinition) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)modelDefinition);
      self->modelNode[i] =
        (Arcadia_Engine_Visuals_ModelNode*)
        Arcadia_Engine_Visuals_NodeFactory_createModelNode
          (
            thread,
            (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
            (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
            modelDefinition
          );
    }

    Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNodes[i], width, height);
    // Assign the "viewport" node and "camera" node to the "enter pass" node.
    Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNodes[i]);
    Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);
    // Render the scene.
    Arcadia_Engine_Visuals_renderScene(thread, self->enterPassNode, self->modelNode[i], (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);

  }
}

static void
Arcadia_Engine_Demo_MainScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Escape) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* e = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, Arcadia_getTickCount(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)e);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Demo_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
    Arcadia_ValueStack_popValues(thread, 2);
  } else if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
    Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_R) {
    Arcadia_logf(Arcadia_LogFlags_Info, "re-initializing backends\n");
  }
}

static void
Arcadia_Engine_Demo_MainScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{ }

static void
Arcadia_Engine_Demo_MainScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{ }

Arcadia_Engine_Demo_MainScene*
Arcadia_Engine_Demo_MainScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_Demo_SceneManager* sceneManager
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Demo_MainScene);
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
  _Arcadia_EndCreate(Arcadia_Engine_Demo_MainScene);
}
