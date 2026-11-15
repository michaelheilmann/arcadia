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

#include "Arcadia/Engine/UI/Tests/EventBases/CanvasPopup.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Testing/Include.h"

// A popup is drawn in canvas coordinates even when its trigger lives in a
// scroll panel's content coordinate system. Test two nested panels with
// independent scroll offsets, and preserve flat-canvas behavior above.
static void
testNestedScrollAnchor
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  // Each safeExecute callback owns a fresh process. ScrollPanelNode needs
  // an initialized visuals node factory even when no backend is attached.
  Arcadia_Engine* engine = Arcadia_Engine_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)engine);
  if (!engine->visualsNodeFactory)
    engine->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_Engine_UI_CanvasNode* canvas = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)canvas);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 400, 300);
  Arcadia_Engine_UI_LayoutPanelNode* root = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)root);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)root, 0, 0, 400, 300);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)canvas, (Arcadia_Object*)root);
  Arcadia_Engine_UI_ScrollPanelNode* outer = Arcadia_Engine_UI_ScrollPanelNode_create(thread, 30, 40, 160, 90);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)outer);
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, outer, 300, 240);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)root, (Arcadia_Object*)outer);
  Arcadia_Engine_UI_LayoutPanelNode* anchor = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)anchor, 90, 100, 30, 20);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)outer, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_LayoutPanelNode* popup = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)popup);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)anchor,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 45);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 120 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 95);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)outer, 10, 25);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)anchor,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 45);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 110 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 70);
  // A second scroll viewport contributes its own origin and scroll offset.
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)outer, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_ScrollPanelNode* inner = Arcadia_Engine_UI_ScrollPanelNode_create(thread, 12, 15, 100, 60);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)inner);
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, inner, 120, 100);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)outer, (Arcadia_Object*)inner);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)anchor, 20, 40, 30, 20);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)inner, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)inner, 4, 6);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)anchor,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 45);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 48 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 19);
  // The same nested coordinates must route pointer presses to the trigger.
  root->lastCanvasHeight = 300.f;
  outer->lastCanvasHeight = 300.f;
  inner->lastCanvasHeight = 300.f;
  Arcadia_Engine_Input_MouseButtonEvent* nestedPress = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    1, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, 50, 231);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)nestedPress);
  Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, nestedPress);
  Arcadia_Tests_assertTrue(thread, outer->focusedChild == (Arcadia_Object*)inner &&
    inner->focusedChild == (Arcadia_Object*)anchor);
  // A child spanning the full content width must not receive hover from the
  // scrollbar gutter or a pointer outside the visible scroll viewport.
  Arcadia_Engine_UI_LayoutPanelNode* oversized = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)oversized);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)oversized, 0, 0, 300, 240);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)outer, (Arcadia_Object*)oversized);
  Arcadia_Engine_Input_MousePointerEvent* overGutter = Arcadia_Engine_Input_MousePointerEvent_create(thread,
    2, Arcadia_Engine_Input_MousePointerAction_Moved, 185, 225);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)overGutter);
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)outer, overGutter);
  Arcadia_Tests_assertTrue(thread, outer->hoveredChild == NULL);
  overGutter->x = 200;
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)outer, overGutter);
  Arcadia_Tests_assertTrue(thread, outer->hoveredChild == NULL);
  overGutter->x = 100;
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)outer, overGutter);
  Arcadia_Tests_assertTrue(thread, outer->hoveredChild == (Arcadia_Object*)oversized);
  // Resize the canvas and retain normal edge-clamping behavior.
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 100, 90);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)anchor,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 45);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 20 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 19);
  // A visible popup follows its anchor when either nested scroll offset changes.
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 400, 300);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)anchor,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 45);
  Arcadia_Engine_UI_CanvasNode_showPopup(thread, canvas, (Arcadia_Object*)popup);
  Arcadia_Tests_assertTrue(thread, canvas->popupPlacementAnchor == (Arcadia_Engine_UI_WidgetNode*)anchor);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)inner, 4, 16);
  Arcadia_Engine_UI_CanvasNode_refreshPopupPlacement(thread, canvas);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 48 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 9);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)outer, 25, 25);
  Arcadia_Engine_UI_CanvasNode_refreshPopupPlacement(thread, canvas);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 33 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 9);
  // Losing the trigger dismisses the popup and releases anchor tracking.
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)inner, (Arcadia_Object*)anchor);
  Arcadia_Engine_UI_CanvasNode_refreshPopupPlacement(thread, canvas);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_CanvasNode_getPopup(thread, canvas) == NULL &&
    canvas->popupPlacementAnchor == NULL && canvas->popupPlacementPopup == NULL);
  // When only horizontal scrolling remains, the vertical gutter disappears
  // and child input must use the entire viewport width.
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, outer, 300, 90);
  overGutter->x = 185;
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)outer, overGutter);
  Arcadia_Tests_assertTrue(thread, outer->hoveredChild == (Arcadia_Object*)oversized);
  // Vertical content overflow restores the clipping/gutter invariant.
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, outer, 300, 240);
  Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)outer, overGutter);
  Arcadia_Tests_assertTrue(thread, outer->hoveredChild == NULL);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)outer, 9999, 9999);
  Arcadia_Integer32Value scrollX = -1;
  Arcadia_Integer32Value scrollY = -1;
  Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset(thread, outer, &scrollX, &scrollY);
  Arcadia_Tests_assertTrue(thread, scrollX == 152 && scrollY == 150);
  // A declaration that matches the outer width must not create an artificial
  // twelve-pixel horizontal scroll range while the vertical bar is visible.
  Arcadia_Engine_UI_ScrollPanelNode_setContentSize(thread, outer, 160, 240);
  Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)outer, 9999, 9999);
  Arcadia_Engine_UI_ScrollPanelNode_getScrollOffset(thread, outer, &scrollX, &scrollY);
  Arcadia_Tests_assertTrue(thread, outer->contentWidth == 148 && scrollX == 0 && scrollY == 150);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
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
  Arcadia_Engine_UI_CanvasNode* canvas = Arcadia_Engine_UI_CanvasNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)canvas);
  Arcadia_Engine_UI_CanvasNode_setVisualsCanvasSize(thread, canvas, 200, 120);
  Arcadia_Engine_UI_LayoutPanelNode* behind = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)behind);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)behind, 0, 0, 200, 120);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)canvas, (Arcadia_Object*)behind);
  Arcadia_Engine_Input_MouseButtonEvent* press = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    1, Arcadia_Engine_Input_MouseButtonAction_Pressed, 0, 10, 10);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)press);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, press));
  Arcadia_Tests_assertTrue(thread, canvas->focusedChild == (Arcadia_Object*)behind && behind->focused);
  Arcadia_Engine_UI_LayoutPanelNode* popup = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)popup);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)popup, 0, 0, 60, 40);
  Arcadia_Engine_UI_CanvasNode_showPopup(thread, canvas, (Arcadia_Object*)popup);
  Arcadia_Tests_assertTrue(thread, canvas->focusedChild == (Arcadia_Object*)popup && popup->focused && !behind->focused);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_CanvasNode_getPopup(thread, canvas) == (Arcadia_Object*)popup);
  // The canvas retains its popup across managed collections.
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_CanvasNode_getPopup(thread, canvas) == (Arcadia_Object*)popup);
  Arcadia_Engine_Input_MousePointerEvent* wheel = Arcadia_Engine_Input_MousePointerEvent_create(thread,
    2, Arcadia_Engine_Input_MousePointerAction_Scrolled, 170, 10);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)wheel);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMousePointerEvent(thread, (Arcadia_Object*)canvas, wheel));
  // Outside click dismisses, but cannot press the underlying panel.
  press->x = 170;
  press->y = 10;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, press));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_UI_CanvasNode_getPopup(thread, canvas) &&
    canvas->focusedChild == (Arcadia_Object*)behind && canvas->pressedChild == NULL);
  Arcadia_Engine_Input_MouseButtonEvent* release = Arcadia_Engine_Input_MouseButtonEvent_create(thread,
    3, Arcadia_Engine_Input_MouseButtonAction_Released, 0, 170, 10);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)release);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent(thread, (Arcadia_Object*)canvas, release));
  Arcadia_Tests_assertTrue(thread, !canvas->consumePopupRelease && canvas->pressedChild == NULL);
  Arcadia_Engine_UI_CanvasNode_showPopup(thread, canvas, (Arcadia_Object*)popup);
  Arcadia_Engine_Input_KeyboardKeyEvent* escape = Arcadia_Engine_Input_KeyboardKeyEvent_create(thread,
    4, Arcadia_Engine_Input_KeyboardKeyAction_Released, Arcadia_Engine_Input_KeyboardKey_Escape);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)escape);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent(thread, (Arcadia_Object*)canvas, escape));
  Arcadia_Tests_assertTrue(thread, !canvas->popup && canvas->focusedChild == (Arcadia_Object*)behind);
  // Position below if it fits, above otherwise; always within canvas bounds.
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)behind, 180, 5, 20, 20);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)behind,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 60);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 120 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 25);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)behind, 10, 100, 20, 20);
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, canvas, (Arcadia_Engine_UI_WidgetNode*)behind,
    (Arcadia_Engine_UI_WidgetNode*)popup, 80, 60);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)popup)->left == 10 &&
    ((Arcadia_Engine_UI_WidgetNode*)popup)->bottom == 40);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
Arcadia_Engine_UI_Tests_canvasPopup
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test)) return EXIT_FAILURE;
  return Arcadia_Tests_safeExecute(&testNestedScrollAnchor) ? EXIT_SUCCESS : EXIT_FAILURE;
}
