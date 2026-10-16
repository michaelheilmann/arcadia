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

#include "Arcadia/Engine/Examples/ColorSources/Scenes/ColorSourcesScene.h"

#include "Arcadia/Engine/Examples/ColorSources/AssetUtilities.h"

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesSceneDispatch* self
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Examples.ColorSources.ColorSourcesScene", Arcadia_Engine_Examples_ColorSources_ColorSourcesScene,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                         &_Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_typeOperations);

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Examples_ColorSources_ColorSourcesScene);
  {
    Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 2),
                  sceneManager = Arcadia_ValueStack_getValue(thread, 1);
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
  for (Arcadia_SizeValue i = 0; i < sizeof(self->modelNodes) / sizeof(self->modelNodes[0]); ++i) {
    self->modelNodes[i] = NULL;
  }
  self->viewportNode = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Examples_ColorSources_ColorSourcesScene);
}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesSceneDispatch* self
  )
{
  ((Arcadia_Engine_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateAudialsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateLogicsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateVisualsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*)) & Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MouseButtonEvent*)) & Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MousePointerEvent*)) & Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self
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
  for (Arcadia_SizeValue i = 0; i < sizeof(self->modelNodes) / sizeof(self->modelNodes[0]); ++i) {
    if (self->modelNodes[i]) Arcadia_Object_visit(thread, (Arcadia_Object*)self->modelNodes[i]);
  }
  if (self->viewportNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  }
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  if (!self->definitions) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, fileSystem);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
    const char* directories[] = { "Assets/ColorSourcesScene", "Assets/Colors" };
    for (Arcadia_SizeValue d = 0; d < sizeof(directories) / sizeof(directories[0]); ++d) {
      Arcadia_SizeValue directoryBase = Arcadia_ValueStack_getSize(thread);
      Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, files);
      Arcadia_FilePath* directory = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, directories[d]));
      Arcadia_ValueStack_pushObjectReferenceValue(thread, directory);
      Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, directory, files);
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
        Arcadia_FilePath* filePath = Arcadia_List_getObjectReferenceValueAt(thread, files, i);
        Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, fileBytes);
        Arcadia_String* text = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes));
        Arcadia_ValueStack_pushObjectReferenceValue(thread, text);
        Arcadia_ADL_Context_readFromString(thread, context, definitions, text, Arcadia_BooleanValue_True);
        Arcadia_ValueStack_popValues(thread, 2);
      }
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - directoryBase);
    }

    self->definitions = definitions;
  }

  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;

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
      getColorDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/Colors/CSS/DimGray.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "Colors.DimGray")),
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
  static const struct { const char* file; const char* name; } models[] = {
    { "Assets/ColorSourcesScene/TextureColorModel.adl", "ColorSourcesScene.TextureColorModel" },
    { "Assets/ColorSourcesScene/VertexColorModel.adl", "ColorSourcesScene.VertexColorModel" },
    { "Assets/ColorSourcesScene/MeshColorModel.adl", "ColorSourcesScene.MeshColorModel" },
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(models) / sizeof(models[0]); ++i) {
    if (self->modelNodes[i]) continue;
    Arcadia_SizeValue modelBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_String* file = Arcadia_String_createFromCxxString(thread, models[i].file);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, file);
    Arcadia_String* name = Arcadia_String_createFromCxxString(thread, models[i].name);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
    Arcadia_ADL_ModelDefinition* modelDefinition = getModelDefinition(thread, self->definitions, file, name);
    if (NULL == modelDefinition) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)modelDefinition);
    self->modelNodes[i] =
      (Arcadia_Engine_Visuals_ModelNode*)
      Arcadia_Engine_Visuals_NodeFactory_createModelNode
        (
          thread,
          (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
          modelDefinition
        );
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - modelBase);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  if (width <= 0 || height <= 0) return;
  load(thread, self);

  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  // Keep the three ADL-defined unit quads square and visible when resized.
  Arcadia_Real32Value aspect = (Arcadia_Real32Value)width / (Arcadia_Real32Value)height;
  Arcadia_Real32Value halfWidth = 2.f;
  Arcadia_Real32Value halfHeight = halfWidth / aspect;
  if (halfHeight < 0.75f) {
    halfHeight = 0.75f;
    halfWidth = halfHeight * aspect;
  }
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, viewToProjectionMatrix);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, -halfWidth, halfWidth, -halfHeight, halfHeight, -1.f, +1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  Arcadia_ValueStack_popValues(thread, 1);

  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);

  // A single pass clears once, then draws texture, vertex and mesh colors.
  Arcadia_Engine_Visuals_renderSceneWithModelNodes(thread, self->enterPassNode, self->modelNodes,
    sizeof(self->modelNodes) / sizeof(self->modelNodes[0]),
    (Arcadia_Engine_Visuals_BackendContext*)((Arcadia_Engine_Scene*)self)->engine->visualsBackendContext);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
    Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Escape) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* e = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, Arcadia_getTickCount(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread,  (Arcadia_Object*)e);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
    Arcadia_ValueStack_popValues(thread, 2);
  } else if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
    Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_R) {
    Arcadia_logf(Arcadia_LogFlags_Info, "re-initializing backends\n");
  }
}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Examples_ColorSources_ColorSourcesScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Examples_ColorSources_ColorSourcesScene*
Arcadia_Engine_Examples_ColorSources_ColorSourcesScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Examples_ColorSources_ColorSourcesScene);
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
  _Arcadia_EndCreate(Arcadia_Engine_Examples_ColorSources_ColorSourcesScene);
}
