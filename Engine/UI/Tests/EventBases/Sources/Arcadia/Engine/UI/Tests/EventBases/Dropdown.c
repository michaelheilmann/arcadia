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

#include "Arcadia/Engine/UI/Tests/EventBases/Dropdown.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Testing/Include.h"

static Arcadia_Natural32Value changeCount;

static void
onChanged
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 2);
  Arcadia_Engine_UI_DropdownController* self = (Arcadia_Engine_UI_DropdownController*)
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_DropdownController_getType(thread));
  Arcadia_Tests_assertTrue(thread, self != NULL);
  changeCount++;
}

static Arcadia_Engine_UI_ButtonNode*
createButton
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    char const* name,
    Arcadia_Integer32Value bottom
  )
{
  Arcadia_ADL_UI_ButtonDefinition* source = Arcadia_ADL_UI_ButtonDefinition_create(thread, definitions,
    Arcadia_String_createFromCxxString(thread, name), 0, bottom, 100, 32,
    Arcadia_String_createFromCxxString(thread, ""), Arcadia_String_createFromCxxString(thread, "Left"),
    Arcadia_String_createFromCxxString(thread, "Bottom"),
    Arcadia_String_createFromCxxString(thread, "Test.Dropdown.Background"), NULL,
    Arcadia_String_createFromCxxString(thread, "Test.Dropdown.Selected"), NULL, NULL, 16, 0, 0, 0, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Engine_UI_ButtonNode* button = Arcadia_Engine_UI_ButtonNode_create(thread, NULL, source);
  Arcadia_ValueStack_popValues(thread, 1);
  return button;
}

static void
expectInvalid
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Natural64Value index
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_DropdownController_select(thread, self, index);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid &&
      Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  }
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
  if (!engine->visualsNodeFactory)
    engine->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  // Button sources resolve both color references during construction.
  // Register these shared test fixtures before creating the anchor/options.
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Context_readFromString(thread, context, definitions,
    Arcadia_String_createFromCxxString(thread, "{ type: \"Color\", name: \"Test.Dropdown.Background\", red: 35, green: 40, blue: 45 }"), false);
  Arcadia_ADL_Context_readFromString(thread, context, definitions,
    Arcadia_String_createFromCxxString(thread, "{ type: \"Color\", name: \"Test.Dropdown.Selected\", red: 80, green: 100, blue: 120 }"), false);
  Arcadia_Engine_UI_CanvasNode* canvas = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)canvas);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 200, 120);
  Arcadia_Engine_UI_ButtonNode* anchor = createButton(thread, definitions, "Test.Dropdown.Trigger", 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)anchor, 30, 80, 100, 32);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)canvas, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_ScrollPanelNode* popup = Arcadia_Engine_UI_ScrollPanelNode_create(thread, 0, 0, 110, 60);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)popup);
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, popup, 110, 120);
  Arcadia_Engine_UI_ButtonNode* a = createButton(thread, definitions, "Test.Dropdown.Low", 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)popup, (Arcadia_Object*)a);
  Arcadia_Engine_UI_ButtonNode* b = createButton(thread, definitions, "Test.Dropdown.Medium", 40);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)popup, (Arcadia_Object*)b);
  Arcadia_Engine_UI_ButtonNode* c = createButton(thread, definitions, "Test.Dropdown.High", 80);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)c);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)popup, (Arcadia_Object*)c);
  Arcadia_Engine_UI_DropdownController* dropdown = Arcadia_Engine_UI_DropdownController_create(thread, canvas, anchor, popup);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)dropdown);
  changeCount = 0;
  Arcadia_Slot* listener = Arcadia_Signal_connect(thread,
    Arcadia_Engine_UI_DropdownController_getSelectionChangedSignal(thread, dropdown),
    (Arcadia_Object*)dropdown, &onChanged);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)listener);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_DropdownController_getSelectedIndex(thread, dropdown) == Arcadia_Natural64Value_Maximum &&
    Arcadia_Engine_UI_DropdownController_getSelectedValue(thread, dropdown) == NULL);
  Arcadia_Engine_UI_DropdownController_select(thread, dropdown, 0);
  Arcadia_Engine_UI_DropdownController_select(thread, dropdown, 0);
  Arcadia_Tests_assertTrue(thread, changeCount == 1 && Arcadia_Engine_UI_ButtonNode_getSelected(thread, a) && !Arcadia_Engine_UI_ButtonNode_getSelected(thread, b));
  Arcadia_Engine_UI_DropdownController_selectValue(thread, dropdown, Arcadia_String_createFromCxxString(thread, "Test.Dropdown.Medium"));
  Arcadia_Tests_assertTrue(thread, changeCount == 2 && Arcadia_Engine_UI_DropdownController_getSelectedIndex(thread, dropdown) == 1 &&
    Arcadia_String_isEqualTo_pn(thread, Arcadia_Engine_UI_DropdownController_getSelectedValue(thread, dropdown), "Test.Dropdown.Medium", 20));
  expectInvalid(thread, dropdown, 3);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, c, Arcadia_BooleanValue_False);
  expectInvalid(thread, dropdown, 2);
  Arcadia_Engine_UI_DropdownController_open(thread, dropdown);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_DropdownController_isOpen(thread, dropdown) &&
    Arcadia_Engine_UI_CanvasNode_getPopup(thread, canvas) == (Arcadia_Object*)popup);
  // Highlight is separate from the selected value; arrows do not commit.
  Arcadia_Engine_Input_KeyboardKeyEvent* down = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread,
    1, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_DownArrow);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)down);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)canvas, down) && changeCount == 2);
  Arcadia_Engine_Input_KeyboardKeyEvent* enter = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread,
    2, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Return);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)enter);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)canvas, enter) &&
    !Arcadia_Engine_UI_DropdownController_isOpen(thread, dropdown) &&
    Arcadia_Engine_UI_DropdownController_getSelectedIndex(thread, dropdown) == 0 && changeCount == 3);
  // A real frame renders these nodes before input, setting their canvas height.
  // Press/release in the popup uses scroll-local coordinates, not canvas-global.
  popup->lastCanvasHeight = 120.f;
  b->lastCanvasHeight = 120.f;
  Arcadia_Engine_UI_DropdownController_open(thread, dropdown);
  Arcadia_Engine_Input_MouseButtonEvent* optionPress = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    3, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, 40, 52);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)optionPress);
  Arcadia_Engine_Input_MouseButtonEvent* optionRelease = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    4, Arcadia_Engine_Input_MouseButtonAction_Released, 0, 40, 52);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)optionRelease);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, optionPress));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, optionRelease));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_DropdownController_isOpen(thread, dropdown) &&
    Arcadia_Engine_UI_DropdownController_getSelectedIndex(thread, dropdown) == 1 && changeCount == 4);
  Arcadia_Engine_UI_DropdownController_open(thread, dropdown);
  Arcadia_Engine_Input_MousePointerEvent* wheel = Arcadia_Engine_Input_MousePointerEvent_createScrolled(thread, 5, 40, 90, 0, 120);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)wheel);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)canvas, wheel));
  Arcadia_Integer32Value offsetY = -1;
  Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset(thread, popup, NULL, &offsetY);
  Arcadia_Tests_assertTrue(thread, offsetY == 24);
  Arcadia_Engine_Input_MouseButtonEvent* press = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    4, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, 190, 8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)press);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, press) &&
    !Arcadia_Engine_UI_DropdownController_isOpen(thread, dropdown));
  Arcadia_Engine_Input_MouseButtonEvent* release = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    5, Arcadia_Engine_Input_MouseButtonAction_Released, 0, 190, 8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)release);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, release) &&
    !canvas->consumePopupRelease);
  Arcadia_Engine_UI_DropdownController_setEnabled(thread, dropdown, Arcadia_BooleanValue_False);
  Arcadia_Engine_UI_DropdownController_open(thread, dropdown);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_DropdownController_isOpen(thread, dropdown));
  Arcadia_Engine_UI_DropdownController_setEnabled(thread, dropdown, Arcadia_BooleanValue_True);
  Arcadia_Engine_UI_DropdownController_open(thread, dropdown);
  Arcadia_Engine_Input_KeyboardKeyEvent* escape = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread,
    6, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Escape);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)escape);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)canvas, escape));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_DropdownController_isOpen(thread, dropdown) && changeCount >= 2);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_UI_Tests_dropdown
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE; }
