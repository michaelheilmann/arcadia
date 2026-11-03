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
toggleShowcase
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  );

static void
Arcadia_Engine_Demo_MainMenuScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  );
static void
Arcadia_Engine_Demo_MainMenuScene_destructImpl
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
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Real64Value tick
  )
{
  static Arcadia_Real32Value const maximumPitch = 89.f;
  static Arcadia_Real32Value const rollRate = 90.f; // Degrees per second.

  Arcadia_Real32Value deltaYaw, deltaPitch, deltaRoll, seconds;
  Arcadia_Real32Value upwardValues[] = { 0.f, +1.f, 0.f };
  Arcadia_Real32Value rightwardValues[] = { +1.f, 0.f, 0.f };
  Arcadia_Real32Value forwardValues[] = { 0.f, 0.f, -1.f };
  Arcadia_Math_Vector3Real32 *worldUpward, *worldRightward, *worldForward;
  Arcadia_Math_QuaternionReal32 *yawQuaternion, *pitchQuaternion, *rollQuaternion, *rotation;

  // Pitch:
  // If the mouse moves up (down), then the delta is negative (positive).
  // We swap the mouse's y-axis so that moving the mouse up raises the nose.
  deltaPitch = -self->mouse.delta.y;

  // Yaw:
  // We swap the mouse's x-axis so that moving the mouse right turns the nose right.
  deltaYaw = -self->mouse.delta.x;

  // Roll:
  // Only the 'Q' and 'E' keys roll the viewer. Scale by the elapsed time so that the
  // roll rate does not depend on the frame rate.
  deltaRoll = 0.f;
  if (self->latches[4] != self->latches[5]) {
    seconds = (Arcadia_Real32Value)(tick / 1000.0);
    deltaRoll = (self->latches[4] ? -rollRate : +rollRate) * seconds;
  }

  if (0.f == deltaYaw && 0.f == deltaPitch && 0.f == deltaRoll) {
    // Nothing changed; keep the current orientation and do not build a new quaternion.
    return;
  }

  self->orientation.yaw += deltaYaw;
  self->orientation.pitch += deltaPitch;
  self->orientation.roll += deltaRoll;

  // Never turn past vertical; doing so would turn the viewer upside down.
  if (self->orientation.pitch > maximumPitch) {
    self->orientation.pitch = maximumPitch;
  } else if (self->orientation.pitch < -maximumPitch) {
    self->orientation.pitch = -maximumPitch;
  }

  // Rebuild the orientation from the Euler angles about the world axes:
  //   rotation = Ry(yaw) * Rx(pitch) * Rforward(roll)
  // Because the angles are absolute rather than accumulated as incremental rotations
  // about the viewer's own axes, combined yaw and pitch cannot introduce a roll.
  worldUpward = Arcadia_Math_Vector3Real32_createWithValues(thread, &upwardValues[0]);
  worldRightward = Arcadia_Math_Vector3Real32_createWithValues(thread, &rightwardValues[0]);
  worldForward = Arcadia_Math_Vector3Real32_createWithValues(thread, &forwardValues[0]);

  yawQuaternion = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, yawQuaternion, worldUpward, self->orientation.yaw);
  pitchQuaternion = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, pitchQuaternion, worldRightward, self->orientation.pitch);
  rollQuaternion = Arcadia_Math_QuaternionReal32_create(thread, 0, 0, 0, 0);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, rollQuaternion, worldForward, self->orientation.roll);

  // Ry(yaw) * Rx(pitch) * Rforward(roll)
  rotation = yawQuaternion;
  Arcadia_Math_QuaternionReal32_multiply(thread, rotation, pitchQuaternion);
  Arcadia_Math_QuaternionReal32_multiply(thread, rotation, rollQuaternion);
  Arcadia_Math_QuaternionReal32_normalize(thread, rotation);
  Arcadia_Starship_Viewer3D_onOrientationChangeInputEvent(thread, self->viewer3D, Arcadia_Starship_OrientationChangeInputEvent_create(thread, Arcadia_getTickCount(thread), rotation));
}

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Demo_MainMenuScene_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Demo_MainMenuScene_visit,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Demo_MainMenuScene_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Demo_MainMenuScene_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Demo.MainMenuScene", Arcadia_Engine_Demo_MainMenuScene,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
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
  self->modelNode = NULL;
  self->viewportNode = NULL;
  //
  self->showcaseCanvasNode = NULL;
  self->showcaseChoiceNode = NULL;
  self->showcaseSizeNode = NULL;
  self->showcaseSegmentsNode = NULL;
  self->showcaseFpsLabel = NULL;
  self->showcaseIndex = 0;
  self->showcaseMode = Arcadia_BooleanValue_False;
  self->showcaseLooking = Arcadia_BooleanValue_False;
  self->showcaseLastFps = -1;
  self->showcaseFpsCanvasWidth = -1;
  self->showcaseFpsCanvasHeight = -1;
  for (Arcadia_Natural32Value i = 0; i < 3; ++i) {
    self->showcaseModels[i] = NULL;
    self->showcaseDirty[i] = Arcadia_BooleanValue_True;
  }
  Arcadia_Engine_FPSCounter_initialize(thread, &self->fpsCounter);
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
  self->orientation.yaw = 0.f;
  self->orientation.pitch = 0.f;
  self->orientation.roll = 0.f;
  // Open with the geometry controls visible; G still returns to the original scene.
  toggleShowcase(thread, self);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Demo_MainMenuScene);
}

static void
Arcadia_Engine_Demo_MainMenuScene_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
  Arcadia_Engine_FPSCounter_uninitialize(thread, &self->fpsCounter);
}


static void
Arcadia_Engine_Demo_MainMenuScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuSceneDispatch* self
  )
{
  ((Arcadia_Engine_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_MainMenuScene_updateAudialsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value)) & Arcadia_Engine_Demo_MainMenuScene_updateLogicsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Demo_MainMenuScene_updateVisualsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*)) & Arcadia_Engine_Demo_MainMenuScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MouseButtonEvent*)) & Arcadia_Engine_Demo_MainMenuScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MousePointerEvent*)) & Arcadia_Engine_Demo_MainMenuScene_handleMousePointerEventImpl;
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
  if (self->showcaseCanvasNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->showcaseCanvasNode);
  }
  for (Arcadia_Natural32Value i = 0; i < 3; ++i) {
    if (self->showcaseModels[i]) {
      Arcadia_Object_visit(thread, (Arcadia_Object*)self->showcaseModels[i]);
    }
  }
}

static void
onShowcaseChoiceChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value count = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != count) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Demo_MainMenuScene* self =
    (Arcadia_Engine_Demo_MainMenuScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Demo_MainMenuScene_getType(thread));
  Arcadia_Engine_UI_SegmentedChoiceNode* choice =
    (Arcadia_Engine_UI_SegmentedChoiceNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread));
  self->showcaseIndex = Arcadia_Engine_UI_SegmentedChoiceNode_getSelectedIndex(thread, choice);
}

static void
onShowcaseSizeChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value count = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != count) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Demo_MainMenuScene* self =
    (Arcadia_Engine_Demo_MainMenuScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Demo_MainMenuScene_getType(thread));
  Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_SliderNode_getType(thread));
  for (Arcadia_Natural32Value i = 0; i < 3; ++i) {
    self->showcaseDirty[i] = Arcadia_BooleanValue_True;
  }
}

static void
onShowcaseSegmentsChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value count = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != count) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Demo_MainMenuScene* self =
    (Arcadia_Engine_Demo_MainMenuScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Demo_MainMenuScene_getType(thread));
  Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_SliderNode_getType(thread));
  self->showcaseDirty[1] = Arcadia_BooleanValue_True;
}

static void
addShowcaseLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    char const* text,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Natural32Value width
  )
{
  Arcadia_Engine_UI_LabelNode* label = Arcadia_Engine_UI_LabelNode_create
    (
      thread,
      backendContext,
      Arcadia_String_createFromCxxString(thread, text),
      left,
      bottom,
      width,
      40,
      Arcadia_String_createFromCxxString(thread, u8"Left"),
      Arcadia_String_createFromCxxString(thread, u8"Center"),
      225,
      230,
      240,
      255
    );
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->showcaseCanvasNode, (Arcadia_Object*)label);
}

static void
loadShowcase
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;
  Arcadia_Engine_Visuals_BackendContext* backendContext = (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext;

  if (!self->showcaseCanvasNode) {
    self->showcaseCanvasNode = Arcadia_Engine_UI_CanvasNode_create(thread);

    addShowcaseLabel(thread, self, backendContext, u8"Primitive", 16, 160, 112);
    addShowcaseLabel(thread, self, backendContext, u8"Size", 16, 110, 112);
    addShowcaseLabel(thread, self, backendContext, u8"Segments", 16, 60, 112);
    addShowcaseLabel(thread, self, backendContext, u8"G / Esc: menu   Right-drag: look   WASD/QE: move/roll   F12: screenshot", 16, 8, 750);

    // Reserve space for the longest formatted FPS string ("FPS: 9999").
    // The label is moved to the top-right when the canvas size is known.
    self->showcaseFpsLabel = Arcadia_Engine_UI_LabelNode_create
      (
        thread, backendContext,
        Arcadia_String_createFromCxxString(thread, u8"FPS: 0000"),
        0, 0, 168, 40,
        Arcadia_String_createFromCxxString(thread, u8"Right"),
        Arcadia_String_createFromCxxString(thread, u8"Center"),
        225, 230, 240, 255
      );
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->showcaseCanvasNode,
                                         (Arcadia_Object*)self->showcaseFpsLabel);


    self->showcaseChoiceNode = Arcadia_Engine_UI_SegmentedChoiceNode_create
      (
        thread, backendContext, 136, 160, 318, 40, 3, 0
      );
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->showcaseCanvasNode, (Arcadia_Object*)self->showcaseChoiceNode);
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, self->showcaseChoiceNode, 0, Arcadia_String_createFromCxxString(thread, u8"Box"));
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, self->showcaseChoiceNode, 1, Arcadia_String_createFromCxxString(thread, u8"Sphere"));
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, self->showcaseChoiceNode, 2, Arcadia_String_createFromCxxString(thread, u8"Plane"));
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SegmentedChoiceNode_getChangedSignal(thread, self->showcaseChoiceNode),
                           (Arcadia_Object*)self, &onShowcaseChoiceChanged);

    self->showcaseSizeNode = Arcadia_Engine_UI_SliderNode_create
      (
        thread, backendContext, 136, 110, 318, 40, 0.5, 4.0, 2.0, 0.25
      );
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->showcaseCanvasNode, (Arcadia_Object*)self->showcaseSizeNode);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->showcaseSizeNode),
                           (Arcadia_Object*)self, &onShowcaseSizeChanged);

    self->showcaseSegmentsNode = Arcadia_Engine_UI_SliderNode_create
      (
        thread, backendContext, 136, 60, 318, 40, 8.0, 64.0, 32.0, 8.0
      );
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->showcaseCanvasNode, (Arcadia_Object*)self->showcaseSegmentsNode);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->showcaseSegmentsNode),
                           (Arcadia_Object*)self, &onShowcaseSegmentsChanged);
  }

  Arcadia_Natural32Value index = self->showcaseIndex;
  if (index >= 3) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!self->showcaseDirty[index] && self->showcaseModels[index]) {
    return;
  }

  char const* paths[3] =
  {
    "Assets/GeometryShowcase/BoxModel.adl",
    "Assets/GeometryShowcase/SphereModel.adl",
    "Assets/GeometryShowcase/PlaneModel.adl",
  };
  char const* names[3] =
  {
    "GeometryShowcase.BoxModel",
    "GeometryShowcase.SphereModel",
    "GeometryShowcase.PlaneModel",
  };
  Arcadia_ADL_ModelDefinition* model = getModelDefinition
    (
      thread, self->definitions,
      Arcadia_String_createFromCxxString(thread, paths[index]),
      Arcadia_String_createFromCxxString(thread, names[index])
    );
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)model);

  // The model definition resolves the generator; only the selected model is
  // rebuilt and the old model's GPU resources are retired through its destructor.
  Arcadia_Real32Value size = (Arcadia_Real32Value)Arcadia_Engine_UI_SliderNode_getValue(thread, self->showcaseSizeNode);
  Arcadia_ADL_MeshGenerator* generator = (Arcadia_ADL_MeshGenerator*)model->mesh->definition;
  switch (index) {
    case 0: {
      Arcadia_ADL_BoxMeshGenerator* box = (Arcadia_ADL_BoxMeshGenerator*)generator;
      box->width = size;
      box->height = size;
      box->depth = size;
    } break;
    case 1: {
      Arcadia_ADL_SphereMeshGenerator* sphere = (Arcadia_ADL_SphereMeshGenerator*)generator;
      sphere->radius = size * 0.5f;
      sphere->segments = (Arcadia_Natural32Value)Arcadia_Engine_UI_SliderNode_getValue(thread, self->showcaseSegmentsNode);
    } break;
    case 2: {
      Arcadia_ADL_PlaneMeshGenerator* plane = (Arcadia_ADL_PlaneMeshGenerator*)generator;
      plane->width = size;
      plane->height = size;
    } break;
  }

  // Publish only after successful construction, retaining the old node if
  // creation fails. Unselected models and their material state stay untouched.
  Arcadia_Engine_Visuals_ModelNode* replacement =
    Arcadia_Engine_Visuals_NodeFactory_createModelNode
      (
        thread,
        (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory,
        backendContext,
        model
      );
  self->showcaseModels[index] = replacement;
  self->showcaseDirty[index] = Arcadia_BooleanValue_False;
}

static void
toggleShowcase
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self
  )
{
  self->showcaseMode = !self->showcaseMode;
  self->showcaseLooking = Arcadia_BooleanValue_False;
  self->mouse.delta.x = 0.f;
  self->mouse.delta.y = 0.f;
  for (Arcadia_Natural32Value i = 0; i < 6; ++i) {
    self->latches[i] = Arcadia_BooleanValue_False;
  }
  if (self->showcaseMode) {
    // An oblique view keeps the horizontal XZ plane visible alongside the box and sphere.
    Arcadia_Real32Value position[3] = { 0.f, 3.f, 5.f };
    Arcadia_Starship_Viewer3D_setPosition(thread, self->viewer3D, Arcadia_Math_Vector3Real32_createWithValues(thread, position));
    self->orientation.yaw = 0.f;
    self->orientation.pitch = -31.f;
    self->orientation.roll = 0.f;
    Arcadia_Real32Value right[3] = { 1.f, 0.f, 0.f };
    Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, self->viewer3D->rotation,
                                                   Arcadia_Math_Vector3Real32_createWithValues(thread, right), self->orientation.pitch);
  }
  if (self->viewportNode) {
    Arcadia_Engine_Visuals_ViewportNode_setClearColor
      (
        thread, self->viewportNode,
        self->showcaseMode
          ? Arcadia_Math_Color4Real32_create4(thread, 0.08f, 0.10f, 0.14f, 1.f)
          : Arcadia_Math_Color4Real32_create4(thread, 1.f, 0.f, 0.f, 1.f)
      );
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

    Arcadia_List* showcaseFiles = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_Demo_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/GeometryShowcase")), showcaseFiles);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)showcaseFiles); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, showcaseFiles, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }

    self->definitions = definitions;
  }

  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;
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
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->uiCanvasNode, (Arcadia_Object*)panelNode);
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
    Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNode,
      self->showcaseMode
        ? Arcadia_Math_Color4Real32_create4(thread, 0.08f, 0.10f, 0.14f, 1.f)
        : Arcadia_Math_Color4Real32_create4(thread, d->red / 255.f, d->green / 255.f, d->blue / 255.f, 1.f));
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
  if (self->showcaseMode) {
    loadShowcase(thread, self);
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
  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;
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

  doYawPitchRoll(thread, self, tick);

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
  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;

  // We use a right-handed coordinate system.
  // -negative z-axis forward, positive z-axis backward
  // -negative y-axis down, positive y-axis up
  // -negative x-axis left, positive x-axis right
  // Viewer located at (0,0,+1).
  // Variant #1: Perspective projection looking down the negative z-axis.
  // Variant #2: Orthographic projection looking down the negative z-axis.
  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Real32Value aspect = self->showcaseMode && height > 0
    ? (Arcadia_Real32Value)width / (Arcadia_Real32Value)height : 4.f / 3.f;
  Arcadia_Math_Matrix4x4Real32_setPerspectiveProjection(thread, viewToProjectionMatrix, 60.f, aspect, 0.1f, 100.f);
  //Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, -1, +1, -1, +1, -1, +1);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  // Set the world space to view space transformation matrix.
  Arcadia_Math_Matrix4Real32* worldToViewMatrix = Arcadia_Starship_Viewer3D_getWorldToViewMatrix(thread, self->viewer3D);
  Arcadia_Engine_Visuals_CameraNode_setWorldToViewMatrix(thread, self->cameraNode, worldToViewMatrix);

  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  // Assign the "viewport" node and "camera" node to the "enter pass" node.
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);
  // The showcase replaces only the model and UI, not the camera or render pass.
  Arcadia_Engine_Visuals_ModelNode* renderedModel = self->showcaseMode
    ? self->showcaseModels[self->showcaseIndex] : self->modelNode;
  Arcadia_Engine_Visuals_renderScene(thread, self->enterPassNode, renderedModel, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);

  if (self->showcaseMode && self->showcaseFpsLabel) {
    Arcadia_Engine_FPSCounter_update(&self->fpsCounter, tick);
    // Updating text rebuilds glyph information; do it only when FPS changes.
    if (self->showcaseLastFps != self->fpsCounter.currentFps) {
      char fpsText[Arcadia_Engine_FPSCounter_MaximumTextLength + 1];
      Arcadia_Engine_FPSCounter_format(&self->fpsCounter, fpsText);
      Arcadia_Engine_UI_LabelNode_setText(thread, self->showcaseFpsLabel,
                                         Arcadia_String_createFromCxxString(thread, fpsText));
      self->showcaseLastFps = self->fpsCounter.currentFps;
    }
    if (width != self->showcaseFpsCanvasWidth || height != self->showcaseFpsCanvasHeight) {
      Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)self->showcaseFpsLabel,
                                               width > 184 ? width - 184 : 0,
                                               height > 56 ? height - 56 : 0);
      self->showcaseFpsCanvasWidth = width;
      self->showcaseFpsCanvasHeight = height;
    }
  }

  Arcadia_Engine_UI_CanvasNode* canvas = self->showcaseMode ? self->showcaseCanvasNode : self->uiCanvasNode;
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, width, height);
  Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)canvas, 0, 0);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)canvas, width, height);
  Arcadia_Engine_UI_CanvasNode_updateVisuals(thread, canvas, self->enterPassNode);
}

static void
Arcadia_Engine_Demo_MainMenuScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Demo_MainMenuScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_Input_KeyboardKeyAction action = Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event);
  if (action == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_G) {
    toggleShowcase(thread, self);
    return;
  }
  if (self->showcaseMode) {
    if (action == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
        Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Escape) {
      toggleShowcase(thread, self);
      return;
    }
    if (self->showcaseCanvasNode &&
        Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)self->showcaseCanvasNode, event)) {
      return;
    }
  }
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
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
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
{
  if (!self->showcaseMode) {
    return;
  }
  // The OpenGL WGL/GLX input adapters both use 2 for the right button.
  // Left-button UI interactions never enable camera look.
  if (2 == event->button) {
    self->showcaseLooking = event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed;
    self->mouse.delta.x = 0.f;
    self->mouse.delta.y = 0.f;
  } else if (0 == event->button && self->showcaseCanvasNode) {
    Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)self->showcaseCanvasNode, event);
  }
}

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
      if (!self->mouse.inWindow) {
        self->mouse.inWindow = Arcadia_BooleanValue_True;
        self->mouse.delta.x = 0.f;
        self->mouse.delta.y = 0.f;
      } else if (!self->showcaseMode || self->showcaseLooking) {
        // In showcase mode, only right-drag can rotate the camera.
        // A left-drag on a slider cannot accidentally move the viewer.
        self->mouse.delta.x += (Arcadia_Real32Value)event->x - self->mouse.oldPosition.x;
        self->mouse.delta.y += (Arcadia_Real32Value)event->y - self->mouse.oldPosition.y;
      }
      self->mouse.oldPosition.x = (Arcadia_Real32Value)event->x;
      self->mouse.oldPosition.y = (Arcadia_Real32Value)event->y;
    } break;
  };
  if (self->showcaseMode && self->showcaseCanvasNode) {
    Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)self->showcaseCanvasNode, event);
  }
}

Arcadia_Engine_Demo_MainMenuScene*
Arcadia_Engine_Demo_MainMenuScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager
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
