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

#include "Arcadia/Engine/Demo/Scenes/MainMenuScene.h"

#include "Arcadia/Engine/Demo/AssetUtilities.h"
#include "Arcadia/Starship/OrientationChangeInputEvent.h"
#include "Arcadia/Starship/PositionChangeInputEvent.h"

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
doYawPitchRoll
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
  Arcadia_Real32Value yaw, pitch, roll;

  // Pitch:
  // If the mouse moves up (down), then the delta is negative (positive).
  // If the mouse moves up, the delta is negative and the nose of the plane is lowered.
  // If the mouse moves down, the delta is positive and the nose of the plane is raised.
  // We swap the mouse's y-axis:
  // If the mouse moves up, the delta is positive and the nose of the plane is raised.
  // If the mouse moves down, the delta is negative and the nose of the plane is lowered.
  pitch = self->mouse.delta.y;
  pitch = -pitch;

  // Yaw:
  // If the mouse moves right (left), then the delta is positive (negative).
  // If the mouse moves right, the delta is positive and the nose of the plane is turned left.
  // If the mouse moves left, the delta is negative and the nose of the plane is turned right.
  // We swap the mouse's x-axis:
  // If the mouse moves right, the delta is negative and the nose of the plane is turned right.
  // If the mouse moves left, the delta is positive and the nose of the plane is turned left.
  yaw = self->mouse.delta.x;
  yaw = -yaw;

  // Roll:
  roll = 0.f;
  if (self->latches[4] != self->latches[5]) {
    static Arcadia_Real32Value scale = 32.f;
    roll = self->latches[4] ? -1.f / scale : +1.f / scale;
  }

  Arcadia_Math_Vector3Real32* forward = Arcadia_Starship_Viewer3D_getForward(thread, self->viewer3D);
  Arcadia_Math_Vector3Real32* upward = Arcadia_Starship_Viewer3D_getUpward(thread, self->viewer3D);
  Arcadia_Math_Vector3Real32* rightward = Arcadia_Starship_Viewer3D_getRightward(thread, self->viewer3D);

  Arcadia_Math_QuaternionReal32* yawQuaternion = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, yawQuaternion, upward, yaw);
  Arcadia_Math_QuaternionReal32* pitchQuaternion = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, pitchQuaternion, rightward, pitch);
  Arcadia_Math_QuaternionReal32* rollQuaternion = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, rollQuaternion, forward, roll);

  // yaw * pitch * roll
  Arcadia_Math_QuaternionReal32* rotation = yawQuaternion;
  Arcadia_Math_QuaternionReal32_multiply(thread, rotation, pitchQuaternion);
  Arcadia_Math_QuaternionReal32_multiply(thread, rotation, rollQuaternion);
  Arcadia_Math_QuaternionReal32_normalize(thread, rotation);
  Arcadia_Starship_Viewer3D_onOrientationChangeInputEvent(thread, self->viewer3D, Arcadia_Starship_OrientationChangeInputEvent_create(thread, Arcadia_getTickCount(thread), rotation));
}

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
  self->viewer3D = Arcadia_Starship_Viewer3D_create(thread);
  //
  self->soundSourceNode = NULL;
  //
  self->uiCanvasNode = NULL;
  //
  self->latches[0] = Arcadia_BooleanValue_False;
  self->latches[1] = Arcadia_BooleanValue_False;
  self->latches[2] = Arcadia_BooleanValue_False;
  self->latches[3] = Arcadia_BooleanValue_False;
  self->latches[4] = Arcadia_BooleanValue_False;
  self->latches[5] = Arcadia_BooleanValue_False;
  self->mouse.oldPosition.x = 0.f;
  self->mouse.oldPosition.y = 0.f;
  self->mouse.delta.x = 0.f;
  self->mouse.delta.y = 0.f;
  self->mouse.inWindow = Arcadia_BooleanValue_False;
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

  if (self->enterPassNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enterPassNode);
  }

  if (self->cameraNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  }

  if (self->viewer3D) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewer3D);
  }

  if (self->modelNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->modelNode);
  }
  if (self->viewportNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  }

  if (self->soundSourceNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->soundSourceNode);
  }

  if (self->uiCanvasNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->uiCanvasNode);
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
  if (!self->soundSourceNode) {
    Arcadia_ADL_SampleBufferDefinition* SAMPLEBUFFERS[] =
    {
      getSampleBufferDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, u8"Assets/MainMenuScene/AmbienceSampleBuffer.adl"),
                                                           Arcadia_String_createFromCxxString(thread, u8"MainMenuScene.AmbienceSampleBuffer")),
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

  if (!self->uiCanvasNode) {
    self->uiCanvasNode = Arcadia_Engine_UI_CanvasNode_create(thread);
    Arcadia_ADL_ModelDefinition* modelDefinition =
      getModelDefinition(thread, self->definitions, Arcadia_String_createFromCxxString(thread, "Assets/MainMenuScene/NewGameButton/NewGameButtonModel.adl"),
                                                    Arcadia_String_createFromCxxString(thread, "MainMenuScene.NewGameButtonModel"));
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)modelDefinition);
    Arcadia_Engine_UI_PanelNode* panelNode =
      (Arcadia_Engine_UI_PanelNode*)
      Arcadia_Engine_UI_PanelNode_create
        (
          thread,
          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
          modelDefinition
        );
    Arcadia_List_insertBackObjectReferenceValue(thread, self->uiCanvasNode->rectangles, (Arcadia_Object*)panelNode);
  }

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
  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;
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
Arcadia_Engine_Demo_MainMenuScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick
  )
{
  Arcadia_Math_Vector3Real32* v = Arcadia_Math_Vector3Real32_create(thread);

  if (self->latches[0]) {
    // 'W' / forward
    Arcadia_Math_Vector3Real32* forward = Arcadia_Starship_Viewer3D_getForward(thread, self->viewer3D);
    Arcadia_Math_Vector3Real32_add(thread, v, forward);
  }
  if (self->latches[1]) {
    // 'A' / leftward
    Arcadia_Math_Vector3Real32* leftward = Arcadia_Starship_Viewer3D_getLeftward(thread, self->viewer3D);
    Arcadia_Math_Vector3Real32_add(thread, v, leftward);
  }
  if (self->latches[2]) {
    // 'S' / backward
    Arcadia_Math_Vector3Real32* backward = Arcadia_Starship_Viewer3D_getBackward(thread, self->viewer3D);
    Arcadia_Math_Vector3Real32_add(thread, v, backward);
  }
  if (self->latches[3]) {
    // 'D' / rightward
    Arcadia_Math_Vector3Real32* rightward = Arcadia_Starship_Viewer3D_getRightward(thread, self->viewer3D);
    Arcadia_Math_Vector3Real32_add(thread, v, rightward);
  }
  
  doYawPitchRoll(thread, self);

  if (Arcadia_Math_Vector3Real32_getSquaredLength(thread, v)) {
    Arcadia_Math_Vector3Real32_normalize(thread, v);
  }
  Arcadia_Math_Vector3Real32_multiplyScalar(thread, v, tick/1000.f*4.f); // TOOD: Remove magic constants. This is effectively 4 meters per second.
  Arcadia_Starship_Viewer3D_onPositionChangeInputEvent(thread, self->viewer3D, Arcadia_Starship_PositionChangeInputEvent_create(thread, Arcadia_getTickCount(thread), v));

  self->mouse.delta.x = 0.f;
  self->mouse.delta.y = 0.f;
}

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
  Arcadia_Engine* engine = ((Arcadia_Engine_Demo_Scene*)self)->engine;

  // We use a right-handed coordinate system.
  // -negative z-axis forward, positive z-axis backward
  // -negative y-axis down, positive y-axis up
  // -negative x-axis left, positive x-axis right
  // Viewer located at (0,0,+1).
  // Variant #1: Perspective projection looking down the negative z-axis.
  // Variant #2: Orthographic projection looking down the negative z-axis.
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setPerspectiveProjection(thread, viewToProjectionMatrix, 60.f, 4.f/3.f, 0.1f, 100.f);
  //Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, -1, +1, -1, +1, -1, +1);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  // Set the world space to view space transformation matrix.
  Arcadia_Math_Matrix4Real32* worldToViewMatrix = Arcadia_Starship_Viewer3D_getWorldToViewMatrix(thread, self->viewer3D);
  Arcadia_Engine_Visuals_CameraNode_setWorldToViewMatrix(thread, self->cameraNode, worldToViewMatrix);

  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);
  // Render the models.
  Arcadia_Engine_Visuals_renderScene(thread, self->enterPassNode, self->modelNode, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);

  // Tell the UI canvas the size of the visuals canvas.
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, self->uiCanvasNode, width, height);
  // The position and the size of the UI canvas.
  Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)self->uiCanvasNode, 0, 0);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)self->uiCanvasNode, width, height);
  Arcadia_Engine_UI_CanvasNode_updateVisuals(thread, self->uiCanvasNode, self->enterPassNode);
}

static void
Arcadia_Engine_Demo_MainMenuScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  // (1) Handle movement latches.
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Pressed) {
    switch (Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event)) {
      case Arcadia_Engine_Input_KeyboardKey_W: {
        self->latches[0] = Arcadia_BooleanValue_True;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_A: {
        self->latches[1] = Arcadia_BooleanValue_True;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_S: {
        self->latches[2] = Arcadia_BooleanValue_True;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_D: {
        self->latches[3] = Arcadia_BooleanValue_True;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_Q: {
        self->latches[4] = Arcadia_BooleanValue_True;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_E: {
        self->latches[5] = Arcadia_BooleanValue_True;
      } break;
    };
  };

  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released) {
    switch (Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event)) {
      case Arcadia_Engine_Input_KeyboardKey_W: {
        self->latches[0] = Arcadia_BooleanValue_False;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_A: {
        self->latches[1] = Arcadia_BooleanValue_False;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_S: {
        self->latches[2] = Arcadia_BooleanValue_False;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_D: {
        self->latches[3] = Arcadia_BooleanValue_False;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_Q: {
        self->latches[4] = Arcadia_BooleanValue_False;
      } break;
      case Arcadia_Engine_Input_KeyboardKey_E: {
        self->latches[5] = Arcadia_BooleanValue_False;
      } break;
    };
  };

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
{ }

static void
Arcadia_Engine_Demo_MainMenuScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  switch (event->action) {
    case Arcadia_Engine_Input_MousePointerAction_Entered: {
      self->mouse.inWindow = Arcadia_BooleanValue_True;
      self->mouse.oldPosition.x = (Arcadia_Real32Value)event->x;
      self->mouse.oldPosition.y = (Arcadia_Real32Value)event->y;
      self->mouse.delta.x = 0.f;
      self->mouse.delta.y = 0.f;
    } break;
    case Arcadia_Engine_Input_MousePointerAction_Exited: {
      self->mouse.inWindow =  Arcadia_BooleanValue_False;
      self->mouse.delta.x = 0.f;
      self->mouse.delta.y = 0.f;
    } break;
    case Arcadia_Engine_Input_MousePointerAction_Moved: {
      // @todo
      // "roll" is currently not taken into account when turning left / right / up / down.
      // If roll should be taken into account, then we should use the following technique for turning left / right:
      // - Get the up vector.
      // - Create an axis-angle rotation with the desired angle around the up vector.
      // - Add this rotation to the rotation quaternion of the viewer.
      // If roll should be taken into account, then we should use the following technique for turning up / down:
      // - Get the right vector.
      // - Create an axis-angle rotation with the resired angle around the right vector.
      // - Add this rotation to the rotation quaternion of the viewer.
      if (!self->mouse.inWindow) {
        self->mouse.inWindow = Arcadia_BooleanValue_True;
        self->mouse.delta.x = 0.f;
        self->mouse.delta.y = 0.f;
      } else {
        self->mouse.delta.x = (Arcadia_Real32Value)event->x - self->mouse.oldPosition.x;
        self->mouse.delta.y = (Arcadia_Real32Value)event->y - self->mouse.oldPosition.y;
      }
      self->mouse.oldPosition.x = (Arcadia_Real32Value)event->x;
      self->mouse.oldPosition.y = (Arcadia_Real32Value)event->y;
    } break;
  };
}

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
