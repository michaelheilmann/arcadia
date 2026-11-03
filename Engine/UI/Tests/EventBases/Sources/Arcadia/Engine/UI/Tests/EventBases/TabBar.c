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

#include "Arcadia/Engine/UI/Tests/EventBases/TabBar.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Testing/Include.h"

static Arcadia_SizeValue notifications;
static Arcadia_Engine_UI_ButtonNode* reentrantTarget;

static void
onSelectionChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 2);
  Arcadia_Engine_UI_TabBarNode* bar = (Arcadia_Engine_UI_TabBarNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2,
    _Arcadia_Engine_UI_TabBarNode_getType(thread));
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1,
    _Arcadia_Engine_UI_TabBarNode_getType(thread)) == (Arcadia_Object*)bar);
  notifications++;
  if (reentrantTarget) {
    Arcadia_Engine_UI_ButtonNode* target = reentrantTarget;
    reentrantTarget = NULL;
    Arcadia_Engine_UI_TabBarNode_select(thread, bar, target);
  }
}

static Arcadia_Engine_UI_ButtonNode*
createButton
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    char const* name
  )
{
  Arcadia_ADL_UI_ButtonDefinition* source = Arcadia_ADL_UI_ButtonDefinition_create(thread, definitions,
    Arcadia_String_createFromCxxString(thread, name), 0, 0, 90, 32,
    Arcadia_String_createFromCxxString(thread, ""), Arcadia_String_createFromCxxString(thread, "Left"),
    Arcadia_String_createFromCxxString(thread, "Bottom"),
    Arcadia_String_createFromCxxString(thread, "Tests.TabBar.Background"), NULL,
    Arcadia_String_createFromCxxString(thread, "Tests.TabBar.Selected"), NULL, NULL, 16, 0, 0, 0, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Engine_UI_ButtonNode* button = Arcadia_Engine_UI_ButtonNode_create(thread, NULL, source);
  Arcadia_ValueStack_popValues(thread, 1);
  return button;
}

static void
expectInvalidSelection
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* bar,
    Arcadia_Engine_UI_ButtonNode* button
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_TabBarNode_select(thread, bar, button);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid &&
      Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
expectInvalidChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* bar,
    Arcadia_Object* child,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)bar, child);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status &&
      Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static Arcadia_BooleanValue
key
  (
    Arcadia_Thread* thread,
    Arcadia_Object* receiver,
    Arcadia_Engine_Input_KeyboardKeyEvent* event,
    Arcadia_Engine_Input_KeyboardKey value
  )
{
  event->key = value;
  event->action = Arcadia_Engine_Input_KeyboardKeyAction_Released;
  return Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, receiver, event);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  engine->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Context_readFromString(thread, Arcadia_ADL_Context_getOrCreate(thread), definitions,
    Arcadia_String_createFromCxxString(thread, "{ type: \"Color\", name: \"Tests.TabBar.Background\", red: 35, green: 40, blue: 45 }"), false);
  Arcadia_ADL_Context_readFromString(thread, Arcadia_ADL_Context_getOrCreate(thread), definitions,
    Arcadia_String_createFromCxxString(thread, "{ type: \"Color\", name: \"Tests.TabBar.Selected\", red: 80, green: 100, blue: 120 }"), false);

  Arcadia_Engine_UI_TabBarNode* bar = Arcadia_Engine_UI_TabBarNode_create(thread, Arcadia_BooleanValue_False, 8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)bar);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) &&
    Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)bar) == 0);
  Arcadia_Engine_Input_KeyboardKeyEvent* event = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread, 0,
    Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_RightArrow);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)event);
  Arcadia_Tests_assertTrue(thread, !key(thread, (Arcadia_Object*)bar, event, Arcadia_Engine_Input_KeyboardKey_RightArrow));
  Arcadia_Signal_connect(thread, Arcadia_Engine_UI_TabBarNode_getSelectionChangedSignal(thread, bar),
    (Arcadia_Object*)bar, &onSelectionChanged);

  Arcadia_Engine_UI_ButtonNode* a = createButton(thread, definitions, "Tests.TabBar.A");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Engine_UI_ButtonNode* b = createButton(thread, definitions, "Tests.TabBar.B");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  Arcadia_Engine_UI_ButtonNode* c = createButton(thread, definitions, "Tests.TabBar.C");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)c);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)bar, (Arcadia_Object*)a);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)bar, (Arcadia_Object*)b);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)bar, (Arcadia_Object*)c);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)bar) == 3 &&
    Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)bar, 1) == (Arcadia_Object*)b);
  expectInvalidChild(thread, bar, (Arcadia_Object*)b, Arcadia_Status_ArgumentValueInvalid);
  expectInvalidChild(thread, bar, (Arcadia_Object*)bar, Arcadia_Status_ArgumentTypeInvalid);
  expectInvalidChild(thread, bar, NULL, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)bar) == 3);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));

  notifications = 0;
  Arcadia_Engine_UI_TabBarNode_select(thread, bar, a);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == a &&
    Arcadia_Engine_UI_ButtonNode_getSelected(thread, a) && notifications == 1);
  Arcadia_Engine_UI_TabBarNode_select(thread, bar, a);
  Arcadia_Tests_assertTrue(thread, notifications == 1);
  Arcadia_Engine_UI_Focusable_focus(thread, (Arcadia_Object*)bar);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_Focusable_isFocused(thread, (Arcadia_Object*)a));
  Arcadia_Tests_assertTrue(thread, key(thread, (Arcadia_Object*)bar, event, Arcadia_Engine_Input_KeyboardKey_Tabulator));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Focusable_isFocused(thread, (Arcadia_Object*)a) && notifications == 1);
  Arcadia_Tests_assertTrue(thread, key(thread, (Arcadia_Object*)bar, event, Arcadia_Engine_Input_KeyboardKey_RightArrow));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == b &&
    !Arcadia_Engine_UI_ButtonNode_getSelected(thread, a) && Arcadia_Engine_UI_ButtonNode_getSelected(thread, b) &&
    notifications == 2);

  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, b, Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == c &&
    Arcadia_Engine_UI_ButtonNode_getSelected(thread, c) && !Arcadia_Engine_UI_ButtonNode_getSelected(thread, b) &&
    Arcadia_Engine_UI_Focusable_isFocused(thread, (Arcadia_Object*)c) && notifications == 3);
  expectInvalidSelection(thread, bar, b);
  Arcadia_Tests_assertTrue(thread, key(thread, (Arcadia_Object*)bar, event, Arcadia_Engine_Input_KeyboardKey_LeftArrow));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == a && notifications == 4);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, b, Arcadia_BooleanValue_True);
  reentrantTarget = b;
  Arcadia_Engine_UI_TabBarNode_select(thread, bar, c);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == b &&
    !Arcadia_Engine_UI_ButtonNode_getSelected(thread, c) && notifications == 6);

  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)bar, (Arcadia_Object*)b);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == c && notifications == 7 &&
    !Arcadia_Engine_UI_ButtonNode_getSelected(thread, b));
  Arcadia_Engine_UI_Activatable_activate(thread, (Arcadia_Object*)b);
  Arcadia_Tests_assertTrue(thread, notifications == 7); // A removed button is disconnected.
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, c, Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) == a && notifications == 8);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, a, Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar) && notifications == 9);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, a, Arcadia_BooleanValue_True);
  Arcadia_Engine_UI_TabBarNode_select(thread, bar, a);
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)bar, (Arcadia_Object*)a);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_TabBarNode_getSelected(thread, bar));
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)bar, (Arcadia_Object*)c);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)bar) == 0);

  // Real pointer routing through CanvasNode -> TabBarNode -> ButtonNode.
  Arcadia_Engine_UI_TabBarNode* nested = Arcadia_Engine_UI_TabBarNode_create(thread, Arcadia_BooleanValue_False, 8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)nested);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)nested, (Arcadia_Object*)a);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)nested, (Arcadia_Object*)b);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)nested, 0, 0, 250, 40);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, (Arcadia_Engine_UI_LayoutPanelNode*)nested);
  ((Arcadia_Engine_UI_LayoutPanelNode*)nested)->lastCanvasHeight = 200.f;
  a->lastCanvasHeight = b->lastCanvasHeight = 200.f;
  Arcadia_Engine_UI_CanvasNode* canvas = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)canvas);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 400, 200);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)canvas, (Arcadia_Object*)nested);
  Arcadia_Integer32Value x, y;
  Arcadia_Engine_UI_WidgetNode_getPosition(thread, (Arcadia_Engine_UI_WidgetNode*)a, &x, &y);
  Arcadia_Engine_Input_MouseButtonEvent* click = Arcadia_Engine_Input_MouseButtonEvent_create(thread, 0,
    Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, x + 5, 200 - (y + 5));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)click);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, click));
  click->action = Arcadia_Engine_Input_MouseButtonAction_Released;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, click));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, nested) == a &&
    Arcadia_Engine_UI_Focusable_isFocused(thread, (Arcadia_Object*)a));
  Arcadia_Tests_assertTrue(thread, key(thread, (Arcadia_Object*)canvas, event, Arcadia_Engine_Input_KeyboardKey_RightArrow));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, nested) == b &&
    Arcadia_Engine_UI_Focusable_isFocused(thread, (Arcadia_Object*)b));
  Arcadia_Tests_assertTrue(thread, key(thread, (Arcadia_Object*)canvas, event, Arcadia_Engine_Input_KeyboardKey_LeftArrow));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, nested) == a);

  // The same behavior works with a vertical StackLayout and Up/Down keys.
  Arcadia_Engine_UI_TabBarNode* vertical = Arcadia_Engine_UI_TabBarNode_create(thread, Arcadia_BooleanValue_True, 4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vertical);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)vertical, (Arcadia_Object*)c);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, c, Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, key(thread, (Arcadia_Object*)vertical, event, Arcadia_Engine_Input_KeyboardKey_DownArrow));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_TabBarNode_getSelected(thread, vertical) == c);
  Arcadia_Tests_assertTrue(thread, !key(thread, (Arcadia_Object*)vertical, event, Arcadia_Engine_Input_KeyboardKey_RightArrow));

  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Engine_UI_Tests_tabBar
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE; }
