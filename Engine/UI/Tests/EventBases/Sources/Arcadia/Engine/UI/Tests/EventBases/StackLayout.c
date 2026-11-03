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

#include "Arcadia/Engine/UI/Tests/EventBases/StackLayout.h"
#include "Arcadia/Engine/UI/Include.h"

static Arcadia_Engine_UI_WidgetNode*
addWidget
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutPanelNode* parent,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  Arcadia_Engine_UI_WidgetNode* child = Arcadia_Engine_UI_WidgetNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
  // Emulate the initialization used by the concrete ADL-backed widgets.
  Arcadia_Engine_UI_WidgetNode_setInitialBounds(thread, child, 0, 0, width, height);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)parent, (Arcadia_Object*)child);
  Arcadia_ValueStack_popValues(thread, 1);
  return child;
}

static void
testVertical
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Engine_UI_LayoutPanelNode* panel = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, panel);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, Arcadia_BooleanValue_True, 7);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, layout);
  Arcadia_Engine_UI_StackLayout_setPadding(thread, layout, 10, 5, 4, 6);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout, Arcadia_Engine_UI_StackPanelNodeAlignment_Center,
                                            Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)layout);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_UI_WidgetNode* first = addWidget(thread, panel, 40, 20);
  Arcadia_Engine_UI_WidgetNode* second = addWidget(thread, panel, 80, 30);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel) == 2);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getLayout(thread, panel) == (Arcadia_Object*)layout);

  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 200, 140);
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 94 && height == 68);
  Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 5, 10, 200, 140);
  Arcadia_Tests_assertTrue(thread, first->left == 88 && first->bottom == 88 && first->width == 40 && first->height == 20);
  Arcadia_Tests_assertTrue(thread, second->left == 68 && second->bottom == 51 && second->width == 80 && second->height == 30);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout, Arcadia_Engine_UI_StackPanelNodeAlignment_Center,
                                            Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch);
  Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 5, 10, 200, 140);
  Arcadia_Tests_assertTrue(thread, first->left == 15 && first->bottom == 88 && first->width == 186);
  Arcadia_Tests_assertTrue(thread, second->left == 15 && second->bottom == 51 && second->width == 186);
  Arcadia_Engine_UI_WidgetNode_getRequestedSize(thread, first, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 40 && height == 20);
  Arcadia_Engine_UI_WidgetNode_getRequestedSize(thread, second, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 80 && height == 30);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 200, 140);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 94 && height == 68);
  Arcadia_ValueStack_popValues(thread, 301);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testHorizontal
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_LayoutPanelNode* panel = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, panel);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, Arcadia_BooleanValue_False, 4);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, layout);
  Arcadia_Engine_UI_StackLayout_setPadding(thread, layout, 1, 2, 3, 4);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout, Arcadia_Engine_UI_StackPanelNodeAlignment_End,
                                            Arcadia_Engine_UI_StackPanelNodeAlignment_End);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)layout);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_UI_WidgetNode* first = addWidget(thread, panel, 20, 10);
  Arcadia_Engine_UI_WidgetNode* second = addWidget(thread, panel, 30, 15);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 100, 80);
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 58 && height == 21);
  Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 10, 20, 100, 80);
  Arcadia_Tests_assertTrue(thread, first->left == 53 && first->bottom == 86 && first->width == 20 && first->height == 10);
  Arcadia_Tests_assertTrue(thread, second->left == 77 && second->bottom == 81 && second->width == 30 && second->height == 15);
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)first);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 100, 80);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 34 && height == 21);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testAnimaStartMenuGeometry
  (
    Arcadia_Thread* thread
  )
{
  // Same geometry as the legacy Anima StartRoot, now through LayoutPanelNode.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_LayoutPanelNode* panel = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)panel);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, Arcadia_BooleanValue_True, 14);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)layout);
  Arcadia_Engine_UI_StackLayout_setPadding(thread, layout, 24, 24, 24, 24);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout,
    Arcadia_Engine_UI_StackPanelNodeAlignment_Center,
    Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)layout);
  Arcadia_ValueStack_popValues(thread, 1);

  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 430, 145, 420, 430);
  addWidget(thread, panel, 260, 28);
  addWidget(thread, panel, 260, 72);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) addWidget(thread, panel, 260, 48);

  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 308 && height == 410);

  static const Arcadia_Integer32Value expectedBottoms[] = { 513, 427, 365, 303, 241, 179 };
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    Arcadia_Engine_UI_WidgetNode* child = (Arcadia_Engine_UI_WidgetNode*)
      Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, i);
    Arcadia_Tests_assertTrue(thread, child->left == 510 && child->bottom == expectedBottoms[i]);
    Arcadia_Tests_assertTrue(thread, child->width == 260);
  }

  // Resizing/repositioning the root through MenuScene's centering helper must
  // invalidate the cached arrangement even if its requested size is unchanged.
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 20, 30, 420, 430);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Engine_UI_WidgetNode* first = (Arcadia_Engine_UI_WidgetNode*)
    Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, 0);
  Arcadia_Engine_UI_WidgetNode* last = (Arcadia_Engine_UI_WidgetNode*)
    Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, 5);
  Arcadia_Tests_assertTrue(thread, first->left == 100 && first->bottom == 398);
  Arcadia_Tests_assertTrue(thread, last->left == 100 && last->bottom == 64);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testLayoutPanelChildSlots
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_LayoutPanelNode* panel = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)panel);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, Arcadia_BooleanValue_True, 7);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)layout);
  Arcadia_Engine_UI_StackLayout_setPadding(thread, layout, 10, 5, 4, 6);
  Arcadia_Engine_UI_StackLayout_setAlignment(thread, layout,
    Arcadia_Engine_UI_StackPanelNodeAlignment_Center, Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)layout);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_UI_WidgetNode* first = addWidget(thread, panel, 40, 20);
  // Keep the removed child alive for the subsequent missing-child regression.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)first);
  Arcadia_Engine_UI_WidgetNode* second = addWidget(thread, panel, 80, 30);
  Arcadia_Engine_UI_LayoutPanelNode_setChildMargin(thread, panel, (Arcadia_Object*)first, 3, 2, 1, 4);
  Arcadia_Engine_UI_LayoutPanelNode_setChildMargin(thread, panel, (Arcadia_Object*)second, 11, 1, 5, 3);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 5, 10, 200, 150);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 110 && height == 78);
  Arcadia_Tests_assertTrue(thread, first->left == 89 && first->bottom == 94);
  Arcadia_Tests_assertTrue(thread, second->left == 71 && second->bottom == 52);
  // A slot override must not alter the sibling's settings.
  Arcadia_Engine_UI_LayoutPanelNode_setChildCrossAlignment(thread, panel, (Arcadia_Object*)second,
    Arcadia_Engine_UI_StackPanelNodeAlignment_End);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, first->left == 89 && second->left == 116);
  Arcadia_Engine_UI_LayoutPanelNode_clearChildCrossAlignment(thread, panel, (Arcadia_Object*)second);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, second->left == 71);
  // Removing a child must shift the corresponding numeric slot with its sibling.
  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)first);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, second->left == 71 && second->bottom == 69);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 110 && height == 45);
  // Newly appended children receive fresh default slots, not a removed margin.
  Arcadia_Engine_UI_WidgetNode* third = addWidget(thread, panel, 20, 10);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getChildLayoutSlots(thread, panel)[1].marginLeft == 0);
  Arcadia_Engine_UI_LayoutPanelNode_setChildMargin(thread, panel, (Arcadia_Object*)third, 2, 0, 0, 0);
  Arcadia_Tests_assertTrue(thread, panel->layoutDirty);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, !panel->layoutDirty);
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_LayoutPanelNode_setChildMargin(thread, panel, (Arcadia_Object*)third, 0, -1, 0, 0);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    Arcadia_Tests_assertTrue(thread, !panel->layoutDirty);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getChildLayoutSlots(thread, panel)[1].marginLeft == 2);
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_LayoutPanelNode_setChildMargin(thread, panel, (Arcadia_Object*)first, 0, 0, 0, 0);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
  }
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testLegacySlots
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_StackPanelNode* panel = Arcadia_Engine_UI_StackPanelNode_create(thread, 5, 10, 200, 140, Arcadia_BooleanValue_True, 7);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, panel);
  Arcadia_Engine_UI_WidgetNode* first = Arcadia_Engine_UI_WidgetNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, first);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, first, 40, 20);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)first);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_UI_WidgetNode* second = Arcadia_Engine_UI_WidgetNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, second);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, second, 80, 30);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)second);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_UI_StackPanelNode_setPadding(thread, panel, 10, 5, 4, 6);
  Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, panel, Arcadia_Engine_UI_StackPanelNodeAlignment_Center,
                                               Arcadia_Engine_UI_StackPanelNodeAlignment_Center);
  Arcadia_Engine_UI_StackPanelNode_setChildMargin(thread, panel, (Arcadia_Object*)first, 3, 0, 1, 0);
  Arcadia_Engine_UI_StackLayoutSettings settings = {
    .vertical = panel->vertical, .gap = panel->gap,
    .paddingLeft = panel->paddingLeft, .paddingBottom = panel->paddingBottom,
    .paddingRight = panel->paddingRight, .paddingTop = panel->paddingTop,
    .mainAlignment = panel->mainAlignment, .crossAlignment = panel->crossAlignment
  };
  Arcadia_Engine_UI_StackLayout_arrangeLegacy(thread, (Arcadia_Object*)panel, &settings, panel->slots,
                                              Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel), 5, 10, 200, 140);
  Arcadia_Tests_assertTrue(thread, first->left == 89 && first->bottom == 88 && first->width == 40 && first->height == 20);
  Arcadia_Tests_assertTrue(thread, second->left == 68 && second->bottom == 51 && second->width == 80 && second->height == 30);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testInvalidConfiguration
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, Arcadia_BooleanValue_True, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, layout);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_StackLayout_setPadding(thread, layout, 0, -1, 0, 0);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, layout->settings.paddingBottom == 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
  }
  Arcadia_ValueStack_popValues(thread, 1);
}

int
Arcadia_Engine_UI_Tests_stackLayout
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testVertical)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testHorizontal)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testAnimaStartMenuGeometry)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testLayoutPanelChildSlots)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(testLegacySlots)) return EXIT_FAILURE;
  return Arcadia_Tests_safeExecute(testInvalidConfiguration) ? EXIT_SUCCESS : EXIT_FAILURE;
}
