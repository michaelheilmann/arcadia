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

#include "Arcadia/Engine/UI/Tests/EventBases/LayoutPanel.h"
#include "Arcadia/Engine/UI/Include.h"

typedef struct LayoutProbe LayoutProbe;
typedef struct LayoutProbeDispatch LayoutProbeDispatch;

struct LayoutProbe { Arcadia_Object _parent; };
struct LayoutProbeDispatch { Arcadia_ObjectDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.UILayoutProbe", LayoutProbe,
                          u8"Arcadia.Object")

static Arcadia_SizeValue measureCount, arrangeCount;
static Arcadia_BooleanValue invalidMeasurement;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    LayoutProbe* self
  )
{
  Arcadia_EnterConstructor(LayoutProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(LayoutProbe);
}

static void
measureImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value availableWidth,
    Arcadia_Integer32Value availableHeight,
    Arcadia_Integer32Value* desiredWidth,
    Arcadia_Integer32Value* desiredHeight
  )
{
  measureCount++;
  Arcadia_Natural64Value count = Arcadia_Engine_UI_Container_getChildCount(thread, container);
  Arcadia_Integer32Value width = 0, height = 0;
  for (Arcadia_Natural64Value i = 0; i < count; ++i) {
    Arcadia_Engine_UI_WidgetNode* child = (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, container, i);
    Arcadia_Engine_UI_WidgetNode_measure(thread, child, availableWidth, availableHeight);
    Arcadia_Integer32Value childWidth, childHeight;
    Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, child, &childWidth, &childHeight);
    if (childWidth > width) width = childWidth;
    height += childHeight + (i ? 7 : 0);
  }
  *desiredWidth = invalidMeasurement ? -1 : width + 8;
  *desiredHeight = height + 8;
}

static void
arrangeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* container,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  arrangeCount++;
  Arcadia_Integer32Value y = bottom + 4;
  Arcadia_Natural64Value count = Arcadia_Engine_UI_Container_getChildCount(thread, container);
  for (Arcadia_Natural64Value i = 0; i < count; ++i) {
    Arcadia_Engine_UI_WidgetNode* child = (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, container, i);
    Arcadia_Integer32Value childWidth, childHeight;
    Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, child, &childWidth, &childHeight);
    Arcadia_Engine_UI_WidgetNode_arrange(thread, child, left + 4, y, childWidth, childHeight);
    y += childHeight + 7;
  }
}

static void
initializeLayoutDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_LayoutDispatch* self
  )
{
  self->measure = &measureImpl;
  self->arrange = &arrangeImpl;
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
    u8"Tests.UILayoutProbe", LayoutProbe,
    u8"Arcadia.Object", Arcadia_Object, &typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Layout_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_LayoutDispatch),
      .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeLayoutDispatchImpl }
  )

static LayoutProbe*
createProbe
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(LayoutProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(LayoutProbe);
}

static void
testLayoutPanel
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Engine_UI_LayoutPanelNode* panel = Arcadia_Engine_UI_LayoutPanelNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, panel);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 200, 100);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)panel)->desiredWidth == 200);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_UI_WidgetNode*)panel)->desiredHeight == 100);

  LayoutProbe* probe = createProbe(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)probe);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getLayout(thread, panel) == (Arcadia_Object*)probe);

  Arcadia_Engine_UI_WidgetNode* first = Arcadia_Engine_UI_WidgetNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, first);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, first, 60, 20);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)first);
  Arcadia_ValueStack_popValues(thread, 1);

  Arcadia_Engine_UI_WidgetNode* second = Arcadia_Engine_UI_WidgetNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, second);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, second, 80, 30);
  Arcadia_Engine_UI_Container_addChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)second);
  Arcadia_ValueStack_popValues(thread, 1);

  Arcadia_SizeValue invalidLayoutDepth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget invalidLayoutJump;
  Arcadia_Thread_pushJumpTarget(thread, &invalidLayoutJump);
  if (Arcadia_JumpTarget_save(&invalidLayoutJump)) {
    Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)first);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == invalidLayoutDepth);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getLayout(thread, panel) == (Arcadia_Object*)probe);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel) == 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, 0) == (Arcadia_Object*)first);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, 1) == (Arcadia_Object*)second);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getLayout(thread, panel) == (Arcadia_Object*)probe);

  measureCount = arrangeCount = 0;
  invalidMeasurement = Arcadia_BooleanValue_False;
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
  Arcadia_Integer32Value width, height;
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, measureCount == 1 && width == 88 && height == 65);
  Arcadia_Engine_UI_WidgetNode_arrange(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 5, 10, 250, 120);
  Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, arrangeCount == 1 && width == 250 && height == 120);
  Arcadia_Engine_UI_WidgetNode_getRequestedSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 200 && height == 100);
  Arcadia_Tests_assertTrue(thread, first->left == 9 && first->bottom == 14 && first->width == 60 && first->height == 20);
  Arcadia_Tests_assertTrue(thread, second->left == 9 && second->bottom == 41 && second->width == 80 && second->height == 30);

  // The new explicit sizing policy must preserve the parent's size without
  // skipping measurement of its descendants.
  Arcadia_Engine_UI_LayoutPanelNode_setSizePolicy(thread, panel, Arcadia_Engine_UI_WidgetNodeSizePolicy_Fixed);
  Arcadia_Tests_assertTrue(thread, panel->layoutDirty);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 200 && height == 100 && measureCount == 2);
  Arcadia_Engine_UI_LayoutPanelNode_setSizePolicy(thread, panel, Arcadia_Engine_UI_WidgetNodeSizePolicy_Content);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 88 && height == 65);

  invalidMeasurement = Arcadia_BooleanValue_True;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
    Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
    Arcadia_Tests_assertTrue(thread, width == 88 && height == 65);
  }
  invalidMeasurement = Arcadia_BooleanValue_False;

  Arcadia_Engine_UI_Container_removeChild(thread, (Arcadia_Object*)panel, (Arcadia_Object*)first);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)panel) == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)panel, 0) == (Arcadia_Object*)second);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 88 && height == 38);
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, NULL);
  Arcadia_Engine_UI_WidgetNode_measure(thread, (Arcadia_Engine_UI_WidgetNode*)panel, 300, 200);
  Arcadia_Engine_UI_WidgetNode_getDesiredSize(thread, (Arcadia_Engine_UI_WidgetNode*)panel, &width, &height);
  Arcadia_Tests_assertTrue(thread, width == 200 && height == 100);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_UI_LayoutPanelNode_getLayout(thread, panel) == NULL);

  // Strategy changes and direct child geometry changes invalidate exactly one pass.
  Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, panel, (Arcadia_Object*)probe);
  measureCount = arrangeCount = 0;
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, measureCount == 1 && arrangeCount == 1 && !panel->layoutDirty);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, measureCount == 1 && arrangeCount == 1);
  Arcadia_Engine_UI_WidgetNode_setSize(thread, second, 94, 39);
  Arcadia_Tests_assertTrue(thread, panel->layoutDirty);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, measureCount == 2 && arrangeCount == 2 && !panel->layoutDirty);
  Arcadia_Tests_assertTrue(thread, second->left == 9 && second->bottom == 14 &&
    second->width == 94 && second->height == 39);
  Arcadia_Engine_UI_LayoutPanelNode_invalidateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, panel->layoutDirty);
  Arcadia_Engine_UI_LayoutPanelNode_updateLayout(thread, panel);
  Arcadia_Tests_assertTrue(thread, measureCount == 3 && arrangeCount == 3);
  Arcadia_ValueStack_popValues(thread, 301);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_UI_Tests_layoutPanel
  (
    void
  )
{ return Arcadia_Tests_safeExecute(testLayoutPanel) ? EXIT_SUCCESS : EXIT_FAILURE; }
