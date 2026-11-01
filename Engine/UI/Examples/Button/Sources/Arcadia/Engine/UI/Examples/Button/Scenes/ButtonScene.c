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

#include "Arcadia/Engine/UI/Examples/Button/Scenes/ButtonScene.h"

#include "Arcadia/Engine/UI/Examples/Button/AssetUtilities.h"

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonSceneDispatch* self
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Real64Value tick
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static void
onButtonActivated
  (
    Arcadia_Thread* thread
  );

static void
onToggleChanged
  (
    Arcadia_Thread* thread
  );

static void
onSliderChanged
  (
    Arcadia_Thread* thread
  );

static void
onSegmentedChoiceChanged
  (
    Arcadia_Thread* thread
  );

static void
layoutControls
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );


static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_UI_Examples_Button_ButtonScene_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_UI_Examples_Button_ButtonScene_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_UI_Examples_Button_ButtonScene_visit,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.Examples.Button.ButtonScene", Arcadia_Engine_UI_Examples_Button_ButtonScene,
                         u8"Arcadia.Engine.Scene", Arcadia_Engine_Scene,
                         &_typeOperations);

static void
layoutControls
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Integer32Value const designWidth = 800;
  Arcadia_Integer32Value const designHeight = 336;
  Arcadia_Integer32Value const left = (width - designWidth) / 2;
  Arcadia_Integer32Value const bottom = (height - designHeight) / 2;

  if (self->helpLabelNode) Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)self->helpLabelNode, left, bottom + 296, 760, 40);
  if (self->rootPanelNode) Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)self->rootPanelNode, left, bottom, designWidth, designHeight);
  if (self->scrollPanelNode) {
    Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, self->scrollPanelNode, 240, 140);
  }
}

static void
onButtonActivated
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_Examples_Button_ButtonScene* self =
    (Arcadia_Engine_UI_Examples_Button_ButtonScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_Examples_Button_ButtonScene_getType(thread));
  Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_ButtonNode_getType(thread));
  if (self->helpLabelNode) {
    Arcadia_Engine_UI_LabelNode_setText(thread, self->helpLabelNode, Arcadia_String_createFromCxxString(thread, u8"Button activated"));
  }
}

static void
onToggleChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_Examples_Button_ButtonScene* self =
    (Arcadia_Engine_UI_Examples_Button_ButtonScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_Examples_Button_ButtonScene_getType(thread));
  Arcadia_Engine_UI_ToggleNode* toggle = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_ToggleNode_getType(thread));
  if (self->helpLabelNode) {
    Arcadia_Engine_UI_LabelNode_setText(thread, self->helpLabelNode,
                                        Arcadia_String_createFromCxxString(thread, Arcadia_Engine_UI_ToggleNode_getChecked(thread, toggle) ? u8"Toggle switched on" : u8"Toggle switched off"));
  }
}

static void
onSliderChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_Examples_Button_ButtonScene* self =
    (Arcadia_Engine_UI_Examples_Button_ButtonScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_Examples_Button_ButtonScene_getType(thread));
  Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_SliderNode_getType(thread));
  if (self->helpLabelNode) {
    Arcadia_Engine_UI_LabelNode_setText(thread, self->helpLabelNode, Arcadia_String_createFromCxxString(thread, u8"Slider value changed"));
  }
}

static void
onSegmentedChoiceChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArgumentValues = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArgumentValues) {
    Arcadia_ValueStack_popValues(thread, numberOfArgumentValues + 1);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_Examples_Button_ButtonScene* self =
    (Arcadia_Engine_UI_Examples_Button_ButtonScene*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_Examples_Button_ButtonScene_getType(thread));
  Arcadia_Engine_UI_SegmentedChoiceNode* segmentedChoice = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread));
  if (self->helpLabelNode) {
    Arcadia_Natural32Value selectedIndex = Arcadia_Engine_UI_SegmentedChoiceNode_getSelectedIndex(thread, segmentedChoice);
    Arcadia_Engine_UI_LabelNode_setText(thread, self->helpLabelNode,
                                        Arcadia_String_createFromCxxString(thread, 0 == selectedIndex ? u8"Choice: Low" : 1 == selectedIndex ? u8"Choice: Medium" : u8"Choice: High"));
  }
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_Examples_Button_ButtonScene);
  {
    Arcadia_Value engine = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_Value sceneManager = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &engine);
    Arcadia_ValueStack_pushValue(thread, &sceneManager);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->definitions = NULL;
  self->cameraNode = NULL;
  self->enterPassNode = NULL;
  self->viewportNode = NULL;
  self->canvasNode = NULL;
  self->rootPanelNode = NULL;
  self->bodyPanelNode = NULL;
  self->leftColumnPanelNode = NULL;
  self->buttonRowPanelNode = NULL;
  self->toggleRowPanelNode = NULL;
  self->sliderRowPanelNode = NULL;
  self->choiceRowPanelNode = NULL;
  self->scrollColumnPanelNode = NULL;
  self->helpLabelNode = NULL;
  self->buttonNode = NULL;
  self->disabledButtonNode = NULL;
  self->toggleLabelNode = NULL;
  self->toggleNode = NULL;
  self->sliderLabelNode = NULL;
  self->sliderNode = NULL;
  self->segmentedChoiceLabelNode = NULL;
  self->segmentedChoiceNode = NULL;
  self->scrollPanelLabelNode = NULL;
  self->scrollPanelNode = NULL;
  self->scrollPanelFirstLabelNode = NULL;
  self->scrollPanelSecondLabelNode = NULL;
  self->scrollPanelThirdLabelNode = NULL;
  self->scrollPanelFourthLabelNode = NULL;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_Examples_Button_ButtonScene);
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonSceneDispatch* self
  )
{
  ((Arcadia_Engine_SceneDispatch*)self)->updateAudials = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value))&Arcadia_Engine_UI_Examples_Button_ButtonScene_updateAudialsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateLogics = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value))&Arcadia_Engine_UI_Examples_Button_ButtonScene_updateLogicsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->updateVisuals = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Real64Value, Arcadia_Integer32Value, Arcadia_Integer32Value))&Arcadia_Engine_UI_Examples_Button_ButtonScene_updateVisualsImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleKeyboardKeyEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_KeyboardKeyEvent*))&Arcadia_Engine_UI_Examples_Button_ButtonScene_handleKeyboardKeyEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMouseButtonEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MouseButtonEvent*))&Arcadia_Engine_UI_Examples_Button_ButtonScene_handleMouseButtonEventImpl;
  ((Arcadia_Engine_SceneDispatch*)self)->handleMousePointerEvent = (void (*)(Arcadia_Thread*, Arcadia_Engine_Scene*, Arcadia_Engine_Input_MousePointerEvent*))&Arcadia_Engine_UI_Examples_Button_ButtonScene_handleMousePointerEventImpl;
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self
  )
{
  if (self->definitions) Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  if (self->cameraNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->cameraNode);
  if (self->enterPassNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->enterPassNode);
  if (self->viewportNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->viewportNode);
  if (self->canvasNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->canvasNode);
  if (self->rootPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->rootPanelNode);
  if (self->bodyPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->bodyPanelNode);
  if (self->leftColumnPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->leftColumnPanelNode);
  if (self->buttonRowPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->buttonRowPanelNode);
  if (self->toggleRowPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->toggleRowPanelNode);
  if (self->sliderRowPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sliderRowPanelNode);
  if (self->choiceRowPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->choiceRowPanelNode);
  if (self->scrollColumnPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollColumnPanelNode);
  if (self->helpLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->helpLabelNode);
  if (self->buttonNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->buttonNode);
  if (self->disabledButtonNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->disabledButtonNode);
  if (self->toggleLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->toggleLabelNode);
  if (self->toggleNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->toggleNode);
  if (self->sliderLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sliderLabelNode);
  if (self->sliderNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->sliderNode);
  if (self->segmentedChoiceLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->segmentedChoiceLabelNode);
  if (self->segmentedChoiceNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->segmentedChoiceNode);
  if (self->scrollPanelLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollPanelLabelNode);
  if (self->scrollPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollPanelNode);
  if (self->scrollPanelFirstLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollPanelFirstLabelNode);
  if (self->scrollPanelSecondLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollPanelSecondLabelNode);
  if (self->scrollPanelThirdLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollPanelThirdLabelNode);
  if (self->scrollPanelFourthLabelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scrollPanelFourthLabelNode);
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self
  )
{
  if (!self->definitions) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Engine_UI_Examples_Button_AssetUtilities_enumerateFiles(thread, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Assets/Button")), files);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); i < n; ++i) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, files, i, _Arcadia_FilePath_getType(thread));
      Arcadia_ByteArrayBuilder* fileBytes = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
      Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(fileBytes)), Arcadia_BooleanValue_True);
    }
    self->definitions = definitions;
  }

  Arcadia_Engine* engine = ((Arcadia_Engine_Scene*)self)->engine;
  if (!self->enterPassNode) {
    self->enterPassNode = Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
  }
  if (!self->viewportNode) {
    self->viewportNode = (Arcadia_Engine_Visuals_ViewportNode*)Arcadia_Engine_Visuals_NodeFactory_createViewportNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
    Arcadia_Engine_Visuals_ViewportNode_setClearColor(thread, self->viewportNode, Arcadia_Math_Color4Real32_create4(thread, 0.08f, 0.09f, 0.11f, 1.f));
    Arcadia_Engine_Visuals_ViewportNode_setRelativeViewportRectangle(thread, self->viewportNode, 0.f, 0.f, 1.f, 1.f);
  }
  if (!self->cameraNode) {
    self->cameraNode = (Arcadia_Engine_Visuals_CameraNode*)Arcadia_Engine_Visuals_NodeFactory_createCameraNode(thread, (Arcadia_Engine_Visuals_NodeFactory*)engine->visualsNodeFactory, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext);
  }
  if (!self->canvasNode) {
    self->canvasNode = Arcadia_Engine_UI_CanvasNode_create(thread);
  }
  if (!self->rootPanelNode) {
    self->rootPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 800, 336, Arcadia_BooleanValue_True, 16);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->rootPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Center, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->canvasNode, (Arcadia_Object*)self->rootPanelNode);
  }
  if (!self->bodyPanelNode) {
    self->bodyPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 800, 260, Arcadia_BooleanValue_False, 20);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->bodyPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Center, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  }
  if (!self->leftColumnPanelNode) {
    self->leftColumnPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 500, 260, Arcadia_BooleanValue_True, 20);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->leftColumnPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Center, Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->bodyPanelNode, (Arcadia_Object*)self->leftColumnPanelNode);
  }
  if (!self->buttonRowPanelNode) {
    self->buttonRowPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 500, 64, Arcadia_BooleanValue_False, 20);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->buttonRowPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Start, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->leftColumnPanelNode, (Arcadia_Object*)self->buttonRowPanelNode);
  }
  if (!self->toggleRowPanelNode) {
    self->toggleRowPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 500, 48, Arcadia_BooleanValue_False, 0);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->toggleRowPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Start, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->leftColumnPanelNode, (Arcadia_Object*)self->toggleRowPanelNode);
  }
  if (!self->sliderRowPanelNode) {
    self->sliderRowPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 500, 40, Arcadia_BooleanValue_False, 0);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->sliderRowPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Start, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->leftColumnPanelNode, (Arcadia_Object*)self->sliderRowPanelNode);
  }
  if (!self->choiceRowPanelNode) {
    self->choiceRowPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 500, 40, Arcadia_BooleanValue_False, 0);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->choiceRowPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Start, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->leftColumnPanelNode, (Arcadia_Object*)self->choiceRowPanelNode);
  }
  if (!self->scrollColumnPanelNode) {
    self->scrollColumnPanelNode = Arcadia_Engine_UI_StackPanelNode_create(thread, 0, 0, 280, 196, Arcadia_BooleanValue_True, 24);
    Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, self->scrollColumnPanelNode, Arcadia_Engine_UI_StackPanelNodeAlignment_Center, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->bodyPanelNode, (Arcadia_Object*)self->scrollColumnPanelNode);
  }
  if (!self->buttonNode) {
    Arcadia_ADL_UI_ButtonDefinition* buttonDefinition = Arcadia_Engine_UI_Examples_Button_getButtonDefinition(thread, self->definitions,
                                                                                                              Arcadia_String_createFromCxxString(thread, "Assets/Button/Button.adl"),
                                                                                                              Arcadia_String_createFromCxxString(thread, "Button.Primary"));
    self->buttonNode = Arcadia_Engine_UI_ButtonNode_create(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext, buttonDefinition);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->buttonRowPanelNode, (Arcadia_Object*)self->buttonNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->buttonRowPanelNode, (Arcadia_Object*)self->buttonNode, 0, 0, 0, 0);
  }
  if (!self->disabledButtonNode) {
    Arcadia_ADL_UI_ButtonDefinition* disabledButtonDefinition =
      Arcadia_ADL_UI_ButtonDefinition_create(thread,
                                             self->definitions,
                                             Arcadia_String_createFromCxxString(thread, u8"Button.Disabled"),
                                             420,
                                             240,
                                             240,
                                             64,
                                             Arcadia_String_createFromCxxString(thread, u8"Disabled"),
                                             Arcadia_String_createFromCxxString(thread, u8"Left"),
                                             Arcadia_String_createFromCxxString(thread, u8"Bottom"),
                                             Arcadia_String_createFromCxxString(thread, u8"Button.Primary.Color.Normal"),
                                             Arcadia_String_createFromCxxString(thread, u8"Button.Primary.Color.Hovered"),
                                             Arcadia_String_createFromCxxString(thread, u8"Button.Primary.Color.Pressed"),
                                             Arcadia_String_createFromCxxString(thread, u8"Button.Primary.Color.Disabled"),
                                             Arcadia_String_createFromCxxString(thread, u8"Button.Primary.Color.Focused"),
                                              56,
                                              120,
                                              240,
                                              24,
                                              0,
                                              87,
                                              104,
                                              118);
    self->disabledButtonNode = Arcadia_Engine_UI_ButtonNode_create(thread, (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext, disabledButtonDefinition);
    Arcadia_Engine_UI_ButtonNode_setEnabled(thread, self->disabledButtonNode, Arcadia_BooleanValue_False);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->buttonRowPanelNode, (Arcadia_Object*)self->disabledButtonNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->buttonRowPanelNode, (Arcadia_Object*)self->disabledButtonNode, 0, 0, 0, 0);
  }
  if (!self->toggleLabelNode) {
    self->toggleLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                              (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                              Arcadia_String_createFromCxxString(thread, u8"Toggle"),
                                                              160,
                                                              148,
                                                              160,
                                                              40,
                                                              Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                              Arcadia_String_createFromCxxString(thread, u8"Bottom"),
                                                              220,
                                                              228,
                                                              240,
                                                              255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->toggleRowPanelNode, (Arcadia_Object*)self->toggleLabelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->toggleRowPanelNode, (Arcadia_Object*)self->toggleLabelNode, 0, 0, 0, 0);
  }
  if (!self->toggleNode) {
    self->toggleNode = Arcadia_Engine_UI_ToggleNode_create(thread,
                                                          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                          320,
                                                          144,
                                                          96,
                                                          48,
                                                          Arcadia_BooleanValue_False);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->toggleRowPanelNode, (Arcadia_Object*)self->toggleNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->toggleRowPanelNode, (Arcadia_Object*)self->toggleNode, 0, 0, 0, 0);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ToggleNode_getChangedSignal(thread, self->toggleNode), (Arcadia_Object*)self, &onToggleChanged);
  }
  if (!self->sliderLabelNode) {
    self->sliderLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                              (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                              Arcadia_String_createFromCxxString(thread, u8"Slider"),
                                                              160,
                                                              80,
                                                              160,
                                                              40,
                                                              Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                              Arcadia_String_createFromCxxString(thread, u8"Bottom"),
                                                              220,
                                                              228,
                                                              240,
                                                              255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->sliderRowPanelNode, (Arcadia_Object*)self->sliderLabelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->sliderRowPanelNode, (Arcadia_Object*)self->sliderLabelNode, 0, 0, 0, 0);
  }
  if (!self->sliderNode) {
    self->sliderNode = Arcadia_Engine_UI_SliderNode_create(thread,
                                                          (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                          320,
                                                          80,
                                                          240,
                                                          36,
                                                          0.0,
                                                          1.0,
                                                          0.35,
                                                          0.05);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->sliderRowPanelNode, (Arcadia_Object*)self->sliderNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->sliderRowPanelNode, (Arcadia_Object*)self->sliderNode, 0, 0, 0, 0);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SliderNode_getChangedSignal(thread, self->sliderNode), (Arcadia_Object*)self, &onSliderChanged);
  }
  if (!self->segmentedChoiceLabelNode) {
    self->segmentedChoiceLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                       (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                       Arcadia_String_createFromCxxString(thread, u8"Choice"),
                                                                       160,
                                                                       24,
                                                                       160,
                                                                       40,
                                                                       Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                                       Arcadia_String_createFromCxxString(thread, u8"Bottom"),
                                                                       220,
                                                                       228,
                                                                       240,
                                                                       255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->choiceRowPanelNode, (Arcadia_Object*)self->segmentedChoiceLabelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->choiceRowPanelNode, (Arcadia_Object*)self->segmentedChoiceLabelNode, 0, 0, 0, 0);
  }
  if (!self->segmentedChoiceNode) {
    self->segmentedChoiceNode = Arcadia_Engine_UI_SegmentedChoiceNode_create(thread,
                                                                           (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                            320,
                                                                            24,
                                                                            300,
                                                                            40,
                                                                           3,
                                                                           1);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->choiceRowPanelNode, (Arcadia_Object*)self->segmentedChoiceNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->choiceRowPanelNode, (Arcadia_Object*)self->segmentedChoiceNode, 0, 0, 0, 0);
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, self->segmentedChoiceNode, 0, Arcadia_String_createFromCxxString(thread, u8"Low"));
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, self->segmentedChoiceNode, 1, Arcadia_String_createFromCxxString(thread, u8"Medium"));
    Arcadia_Engine_UI_SegmentedChoiceNode_setSegmentLabel(thread, self->segmentedChoiceNode, 2, Arcadia_String_createFromCxxString(thread, u8"High"));
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_SegmentedChoiceNode_getChangedSignal(thread, self->segmentedChoiceNode), (Arcadia_Object*)self, &onSegmentedChoiceChanged);
  }
  if (!self->scrollPanelLabelNode) {
    self->scrollPanelLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                   (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                   Arcadia_String_createFromCxxString(thread, u8"Scroll panel (Up/Down)"),
                                                                   680,
                                                                   188,
                                                                   280,
                                                                   32,
                                                                   Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                                   Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                   220,
                                                                   228,
                                                                   240,
                                                                   255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->scrollColumnPanelNode, (Arcadia_Object*)self->scrollPanelLabelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->scrollColumnPanelNode, (Arcadia_Object*)self->scrollPanelLabelNode, 0, 0, 0, 0);
  }
  if (!self->scrollPanelNode) {
    self->scrollPanelNode = Arcadia_Engine_UI_ScrollPanelNode_create(thread, 680, 24, 240, 96);
    Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, self->scrollPanelNode, 240, 140);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->scrollColumnPanelNode, (Arcadia_Object*)self->scrollPanelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->scrollColumnPanelNode, (Arcadia_Object*)self->scrollPanelNode, 0, 0, 0, 0);
  }
  if (!self->scrollPanelFirstLabelNode) {
    self->scrollPanelFirstLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                        (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                        Arcadia_String_createFromCxxString(thread, u8"Display"),
                                                                        0,
                                                                        0,
                                                                        220,
                                                                        32,
                                                                        Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                                        Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                        245,
                                                                        248,
                                                                        252,
                                                                        255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->scrollPanelNode, (Arcadia_Object*)self->scrollPanelFirstLabelNode);
  }
  if (!self->scrollPanelSecondLabelNode) {
    self->scrollPanelSecondLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                         (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                         Arcadia_String_createFromCxxString(thread, u8"Audio"),
                                                                         0,
                                                                         36,
                                                                         220,
                                                                         32,
                                                                         Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                                         Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                         245,
                                                                         248,
                                                                         252,
                                                                         255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->scrollPanelNode, (Arcadia_Object*)self->scrollPanelSecondLabelNode);
  }
  if (!self->scrollPanelThirdLabelNode) {
    self->scrollPanelThirdLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                        (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                        Arcadia_String_createFromCxxString(thread, u8"Controls"),
                                                                        0,
                                                                        72,
                                                                        220,
                                                                        32,
                                                                        Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                                        Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                        245,
                                                                        248,
                                                                        252,
                                                                        255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->scrollPanelNode, (Arcadia_Object*)self->scrollPanelThirdLabelNode);
  }
  if (!self->scrollPanelFourthLabelNode) {
    self->scrollPanelFourthLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                                         (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                                         Arcadia_String_createFromCxxString(thread, u8"Accessibility"),
                                                                         0,
                                                                         108,
                                                                         220,
                                                                         32,
                                                                         Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                                         Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                                         245,
                                                                         248,
                                                                         252,
                                                                         255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->scrollPanelNode, (Arcadia_Object*)self->scrollPanelFourthLabelNode);
  }
  if (!self->helpLabelNode) {
    self->helpLabelNode = Arcadia_Engine_UI_LabelNode_create(thread,
                                                             (Arcadia_Engine_Visuals_BackendContext*)engine->visualsBackendContext,
                                                             Arcadia_String_createFromCxxString(thread, u8"Hover, click, or press Tab then Space/Return"),
                                                             160,
                                                             320,
                                                             760,
                                                             40,
                                                             Arcadia_String_createFromCxxString(thread, u8"Left"),
                                                             Arcadia_String_createFromCxxString(thread, u8"Center"),
                                                            220,
                                                            228,
                                                            240,
                                                            255);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->rootPanelNode, (Arcadia_Object*)self->helpLabelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->rootPanelNode, (Arcadia_Object*)self->helpLabelNode, 0, 0, 0, 0);
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)self->rootPanelNode, (Arcadia_Object*)self->bodyPanelNode);
    Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, self->rootPanelNode, (Arcadia_Object*)self->bodyPanelNode, 0, 0, 0, 0);
    Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, self->buttonNode), (Arcadia_Object*)self, &onButtonActivated);
  }
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_updateAudialsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ load(thread, self); }

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_updateLogicsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Real64Value tick
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_updateVisualsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Real64Value tick,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  load(thread, self);
  layoutControls(thread, self, width, height);

  Arcadia_Math_Matrix4Real32* viewToProjectionMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setOrthographicProjection(thread, viewToProjectionMatrix, 0.f, (Arcadia_Real32Value)width, 0.f, (Arcadia_Real32Value)height, -1.f, 1.f);
  Arcadia_Engine_Visuals_CameraNode_setViewToProjectionMatrix(thread, self->cameraNode, viewToProjectionMatrix);
  Arcadia_Engine_Visuals_ViewportNode_setCanvasSize(thread, self->viewportNode, width, height);
  Arcadia_Engine_Visuals_EnterPassNode_setViewportNode(thread, self->enterPassNode, self->viewportNode);
  Arcadia_Engine_Visuals_EnterPassNode_setCameraNode(thread, self->enterPassNode, self->cameraNode);
  Arcadia_Engine_Visuals_EnterPassNode_setFrameBufferNode(thread, self->enterPassNode, NULL);
  Arcadia_Engine_Visuals_Node_render(thread, (Arcadia_Engine_Visuals_Node*)self->enterPassNode, self->enterPassNode);

  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, self->canvasNode, width, height);
  Arcadia_Engine_UI_CanvasNode_updateVisuals(thread, self->canvasNode, self->enterPassNode);
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released &&
      Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event) == Arcadia_Engine_Input_KeyboardKey_Escape) {
    Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent* e = Arcadia_Engine_Visuals_ApplicationQuitRequestedEvent_create(thread, Arcadia_getTickCount(thread));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)e);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Signal_emit(thread, ((Arcadia_Engine_Scene*)self)->applicationQuitRequestSignal, (Arcadia_Object*)self);
    Arcadia_ValueStack_popValues(thread, 2);
  } else if (self->canvasNode) {
    Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)self->canvasNode, event);
  }
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  load(thread, self);
  Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)self->canvasNode, event);
}

static void
Arcadia_Engine_UI_Examples_Button_ButtonScene_handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_Examples_Button_ButtonScene* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  load(thread, self);
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)self->canvasNode, event);
}

Arcadia_Engine_UI_Examples_Button_ButtonScene*
Arcadia_Engine_UI_Examples_Button_ButtonScene_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine,
    Arcadia_Engine_SceneManager* sceneManager
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_Examples_Button_ButtonScene);
  if (engine) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (sceneManager) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sceneManager); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_UI_Examples_Button_ButtonScene);
}
