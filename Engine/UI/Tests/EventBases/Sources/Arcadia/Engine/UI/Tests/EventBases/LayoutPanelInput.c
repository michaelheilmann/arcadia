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

#include "Arcadia/Engine/UI/Tests/EventBases/LayoutPanelInput.h"
#include "Arcadia/Engine/UI/Include.h"

typedef struct InputProbe InputProbe;
typedef struct InputProbeDispatch InputProbeDispatch;

struct InputProbe {
  Arcadia_Engine_UI_WidgetNode _parent;
  Arcadia_BooleanValue focused;
  Arcadia_Natural32Value pointerCalls, buttonCalls, keyboardCalls;
};

struct InputProbeDispatch { Arcadia_Engine_UI_WidgetNodeDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.UILayoutInputProbe", InputProbe,
                          u8"Arcadia.Engine.UI.WidgetNode")

static void
constructImpl
  (
    Arcadia_Thread* thread,
    InputProbe* self
  )
{
  Arcadia_EnterConstructor(InputProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->focused = Arcadia_BooleanValue_False;
  self->pointerCalls = self->buttonCalls = self->keyboardCalls = 0;
  Arcadia_LeaveConstructor(InputProbe);
}

static Arcadia_BooleanValue
hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)object;
  return x >= widget->left && x < widget->left + widget->width &&
         y >= widget->bottom && y < widget->bottom + widget->height;
}

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{ ((InputProbe*)object)->focused = Arcadia_BooleanValue_True; }

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{ ((InputProbe*)object)->focused = Arcadia_BooleanValue_False; }

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{ return ((InputProbe*)object)->focused; }

static Arcadia_BooleanValue
handleKeyboardImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  ((InputProbe*)object)->keyboardCalls++;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
handlePointerImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  ((InputProbe*)object)->pointerCalls++;
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
handleButtonImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  ((InputProbe*)object)->buttonCalls++;
  return Arcadia_BooleanValue_True;
}

static void
initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{
  self->focus = &focusImpl;
  self->blur = &blurImpl;
  self->isFocused = &isFocusedImpl;
}

static void
initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{ self->hitTest = &hitTestImpl; }

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &handleKeyboardImpl; }

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &handlePointerImpl;
  self->handleMouseButtonEvent = &handleButtonImpl;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Tests.UILayoutInputProbe", InputProbe,
    u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode, &typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeFocusableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeHitTestableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeKeyboardInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializePointerInputDispatchImpl }
  )

static InputProbe*
createProbe
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(InputProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(InputProbe);
}

static void
testInput
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Engine_UI_LayoutPanelNode* panel = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)panel);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 0, 0, 100, 100);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, Arcadia_BooleanValue_True, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)layout);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout,
    Arcadia_Engine_UI_StackPanelNodeAlignment_Start, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)layout);
  Arcadia_ValueStack_popValues(thread, 1);
  InputProbe* first = createProbe(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)first);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)first, 20, 20);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)first);
  Arcadia_ValueStack_popValues(thread, 1);
  InputProbe* second = createProbe(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)second);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)second, 20, 20);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)second);
  Arcadia_ValueStack_popValues(thread, 1);

  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, !panel->layoutDirty && panel->hasArranged);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)first)->left == 40 &&
    ((Arcadia_Engine_UI_WidgetNode*)first)->bottom == 80);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)second)->left == 40 &&
    ((Arcadia_Engine_UI_WidgetNode*)second)->bottom == 60);
  panel->lastCanvasHeight = 100.f;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_HitTestable_hitTest(thread, (Arcadia_Object*)panel, 50, 90));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_HitTestable_hitTest(thread, (Arcadia_Object*)panel, 120, 90));

  Arcadia_Engine_Input_MousePointerEvent* motion = Arcadia_Engine_Input_MousePointerEvent_create(thread,
    1, Arcadia_Engine_Input_MousePointerAction_Moved, 50, 10);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)motion);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)panel, motion));
  Arcadia_Tests_assertTrue(thread, panel->hoveredChild == (Arcadia_Object*)first && first->pointerCalls == 1);
  Arcadia_Engine_Input_MouseButtonEvent* press = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    2, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, 50, 10);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)press);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)panel, press));
  Arcadia_Tests_assertTrue(thread, panel->pressedChild == (Arcadia_Object*)first && first->buttonCalls == 1 && first->focused);

  Arcadia_Engine_Input_KeyboardKeyEvent* key = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread,
    3, Arcadia_Engine_Input_KeyboardKeyAction_Pressed, Arcadia_Engine_Input_KeyboardKey_A);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)key);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)panel, key));
  Arcadia_Tests_assertTrue(thread, first->keyboardCalls == 1);

  Arcadia_Engine_Input_MouseButtonEvent* release = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    4, Arcadia_Engine_Input_MouseButtonAction_Released, 0, 10, 10);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)release);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)panel, release));
  Arcadia_Tests_assertTrue(thread, panel->pressedChild == NULL && first->buttonCalls == 2 && second->buttonCalls == 0);

  Arcadia_Engine_Input_KeyboardKeyEvent* tab = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread,
    5, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Tabulator);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)tab);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)panel, tab));
  Arcadia_Tests_assertTrue(thread, second->focused && !first->focused && panel->focusedChild == (Arcadia_Object*)second);

  // Removal disconnects the geometry observer and clears retained interaction targets.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)second);
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)second);
  Arcadia_Tests_assertTrue(thread, !second->focused && panel->focusedChild == NULL && panel->layoutDirty);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, !panel->layoutDirty);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)second, 60, 25);
  Arcadia_Tests_assertTrue(thread, !panel->layoutDirty);
  Arcadia_ValueStack_popValues(thread, 1);

  Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel) == 1);
  Arcadia_ValueStack_popValues(thread, 306);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_UI_Tests_layoutPanelInput
  (
    void
  )
{ return Arcadia_Tests_safeExecute(testInput) ? EXIT_SUCCESS : EXIT_FAILURE; }
