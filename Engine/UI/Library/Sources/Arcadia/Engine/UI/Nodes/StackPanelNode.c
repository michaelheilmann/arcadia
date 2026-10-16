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

#define ARCADIA_ENGINE_UI_PRIVATE (1)
#include "Arcadia/Engine/UI/Nodes/StackPanelNode.h"

#include "Arcadia/Engine/UI/DefinitionFactory.h"
#include "Arcadia/Engine/UI/Include.h"
#include "Arcadia/Engine/UI/TextUtilities.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  );

static void
initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNodeDispatch* self
  );

static void
initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  );

static void
initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  );

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  );

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  );

static void
initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  );

static Arcadia_Natural64Value
getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_Object*
getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  );

static void
addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

static void
removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  );

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  );

static Arcadia_BooleanValue
hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  );

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  );

static Arcadia_BooleanValue
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  );

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  );

static void
renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  );

static void
updateBackgroundPanelVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  );

static void
writeQuadPositions
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue vertexIndex,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  );

static void
validateAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNodeAlignment alignment
  )
{
  if (alignment < Arcadia_Engine_UI_StackPanelNodeAlignment_Start || alignment > Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_Engine_UI_StackPanelNodeAlignment
parseAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_String* alignment
  )
{
  if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, alignment, u8"Start")) {
    return Arcadia_Engine_UI_StackPanelNodeAlignment_Start;
  }
  if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, alignment, u8"Center")) {
    return Arcadia_Engine_UI_StackPanelNodeAlignment_Center;
  }
  if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, alignment, u8"End")) {
    return Arcadia_Engine_UI_StackPanelNodeAlignment_End;
  }
  if (Arcadia_Engine_UI_TextUtilities_stringEqualsCxx(thread, alignment, u8"Stretch")) {
    return Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch;
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_Integer32Value
clampAvailable
  (
    Arcadia_Integer32Value value
  )
{
  return value < 0 ? 0 : value;
}

static Arcadia_Integer32Value
alignedOffset
  (
    Arcadia_Integer32Value available,
    Arcadia_Integer32Value used,
    Arcadia_Engine_UI_StackPanelNodeAlignment alignment
  )
{
  Arcadia_Integer32Value extra = available - used;
  if (extra < 0) extra = 0;
  if (alignment == Arcadia_Engine_UI_StackPanelNodeAlignment_End) return extra;
  if (alignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Center || alignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch) return extra / 2;
  return 0;
}

static Arcadia_Engine_UI_StackPanelNodeSlot
makeDefaultSlot
  (
    void
  )
{
  Arcadia_Engine_UI_StackPanelNodeSlot slot;
  slot.marginLeft = 0;
  slot.marginBottom = 0;
  slot.marginRight = 0;
  slot.marginTop = 0;
  slot.hasAlignmentOverride = Arcadia_BooleanValue_False;
  slot.crossAlignment = Arcadia_Engine_UI_StackPanelNodeAlignment_Center;
  return slot;
}

static void
ensureSlotCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_SizeValue requiredCapacity
  )
{
  if (self->slotCapacity >= requiredCapacity) return;
  Arcadia_SizeValue oldCapacity = self->slotCapacity;
  Arcadia_SizeValue newCapacity = oldCapacity ? oldCapacity * 2 : 8;
  while (newCapacity < requiredCapacity) newCapacity *= 2;
  Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->slots, sizeof(Arcadia_Engine_UI_StackPanelNodeSlot) * newCapacity);
  for (Arcadia_SizeValue i = oldCapacity; i < newCapacity; ++i) {
    self->slots[i] = makeDefaultSlot();
  }
  self->slotCapacity = newCapacity;
}

static Arcadia_SizeValue
findChildIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child
  )
{
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, self->children, i) == child) return i;
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
  return 0;
}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.StackPanelNode", Arcadia_Engine_UI_StackPanelNode,
    u8"Arcadia.Engine.UI.WidgetNode", Arcadia_Engine_UI_WidgetNode, &_typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Container_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeContainerDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Focusable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_FocusableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeFocusableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_HitTestable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_HitTestableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeHitTestableDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeKeyboardInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializePointerInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeRenderableDispatchImpl }
  )

static Arcadia_BooleanValue
hasInterface
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_InterfaceType* interfaceType
  )
{
  return NULL != Arcadia_ObjectType_getInterfaceDispatch(thread, (Arcadia_ObjectType*)Arcadia_Object_getType(thread, object), interfaceType);
}

static void
setHovered
  (
    Arcadia_Thread* thread,
    Arcadia_Object* child,
    Arcadia_BooleanValue hovered
  )
{
  if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ButtonNode_getType(thread))) {
    Arcadia_Engine_UI_ButtonNode_setHovered(thread, (Arcadia_Engine_UI_ButtonNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SegmentedChoiceNode_getType(thread))) {
    Arcadia_Engine_UI_SegmentedChoiceNode_setHovered(thread, (Arcadia_Engine_UI_SegmentedChoiceNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_SliderNode_getType(thread))) {
    Arcadia_Engine_UI_SliderNode_setHovered(thread, (Arcadia_Engine_UI_SliderNode*)child, hovered);
  } else if (child && Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ToggleNode_getType(thread))) {
    Arcadia_Engine_UI_ToggleNode_setHovered(thread, (Arcadia_Engine_UI_ToggleNode*)child, hovered);
  }
}

static void
arrangeChildren
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  )
{
  Arcadia_Integer32Value left, bottom, width, height;
  Arcadia_Engine_UI_WidgetNode_getBounds(thread, (Arcadia_Engine_UI_WidgetNode*)self, &left, &bottom, &width, &height);
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children);
  if (!n) return;

  Arcadia_Integer32Value contentLeft = left + self->paddingLeft;
  Arcadia_Integer32Value contentBottom = bottom + self->paddingBottom;
  Arcadia_Integer32Value contentWidth = clampAvailable(width - self->paddingLeft - self->paddingRight);
  Arcadia_Integer32Value contentHeight = clampAvailable(height - self->paddingBottom - self->paddingTop);
  Arcadia_Integer32Value numberOfWidgetChildren = 0;
  Arcadia_Integer32Value total = 0;
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
    if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
    Arcadia_Engine_UI_StackPanelNodeSlot* slot = &self->slots[i];
    Arcadia_Integer32Value childWidth, childHeight;
    Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
    total += self->vertical ? childHeight + slot->marginTop + slot->marginBottom : childWidth + slot->marginLeft + slot->marginRight;
    ++numberOfWidgetChildren;
  }
  if (!numberOfWidgetChildren) return;
  total += self->gap * (numberOfWidgetChildren - 1);

  if (self->vertical) {
    Arcadia_Integer32Value y = contentBottom + contentHeight - alignedOffset(contentHeight, total, self->mainAlignment);
    for (Arcadia_SizeValue i = 0; i < n; ++i) {
      Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
      if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
      Arcadia_Engine_UI_StackPanelNodeSlot* slot = &self->slots[i];
      Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment = slot->hasAlignmentOverride ? slot->crossAlignment : self->crossAlignment;
      Arcadia_Integer32Value childWidth, childHeight;
      Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
      Arcadia_Integer32Value childAvailableWidth = clampAvailable(contentWidth - slot->marginLeft - slot->marginRight);
      y -= slot->marginTop;
      if (crossAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch) {
        childWidth = childAvailableWidth;
        Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, childWidth, childHeight);
      }
      y -= childHeight;
      Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)child, contentLeft + slot->marginLeft + alignedOffset(childAvailableWidth, childWidth, crossAlignment), y);
      y -= slot->marginBottom + self->gap;
    }
  } else {
    Arcadia_Integer32Value x = contentLeft + alignedOffset(contentWidth, total, self->mainAlignment);
    for (Arcadia_SizeValue i = 0; i < n; ++i) {
      Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
      if (!Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_WidgetNode_getType(thread))) continue;
      Arcadia_Engine_UI_StackPanelNodeSlot* slot = &self->slots[i];
      Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment = slot->hasAlignmentOverride ? slot->crossAlignment : self->crossAlignment;
      Arcadia_Integer32Value childWidth, childHeight;
      Arcadia_Engine_UI_WidgetNode_getSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, &childWidth, &childHeight);
      Arcadia_Integer32Value childAvailableHeight = clampAvailable(contentHeight - slot->marginBottom - slot->marginTop);
      x += slot->marginLeft;
      if (crossAlignment == Arcadia_Engine_UI_StackPanelNodeAlignment_Stretch) {
        childHeight = childAvailableHeight;
        Arcadia_Engine_UI_WidgetNode_setSize(thread, (Arcadia_Engine_UI_WidgetNode*)child, childWidth, childHeight);
      }
      Arcadia_Engine_UI_WidgetNode_setPosition(thread, (Arcadia_Engine_UI_WidgetNode*)child, x, contentBottom + slot->marginBottom + alignedOffset(childAvailableHeight, childHeight, crossAlignment));
      x += childWidth + slot->marginRight + self->gap;
    }
  }
}

static Arcadia_Object*
hitTestChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  arrangeChildren(thread, self);
  for (Arcadia_SizeValue i = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i > 0; --i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i - 1);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_HitTestable_getType(thread)) &&
        Arcadia_Engine_UI_HitTestable_hitTest(thread, child, x, y)) {
      return child;
    }
  }
  return NULL;
}

static void
focusChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child
  )
{
  if (self->focusedChild == child) return;
  if (self->focusedChild && hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
    Arcadia_Engine_UI_Focusable_blur(thread, self->focusedChild);
  }
  self->focusedChild = child;
  if (self->focusedChild && hasInterface(thread, self->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Focusable_getType(thread))) {
    Arcadia_Engine_UI_Focusable_focus(thread, self->focusedChild);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_StackPanelNode);
  if (6 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, (Arcadia_Engine_UI_WidgetNode*)self,
                                         Arcadia_ValueStack_getInteger32Value(thread, 6),
                                         Arcadia_ValueStack_getInteger32Value(thread, 5),
                                         Arcadia_ValueStack_getInteger32Value(thread, 4),
                                         Arcadia_ValueStack_getInteger32Value(thread, 3));
  self->vertical = Arcadia_ValueStack_getBooleanValue(thread, 2);
  self->gap = Arcadia_ValueStack_getInteger32Value(thread, 1);
  self->paddingLeft = 0;
  self->paddingBottom = 0;
  self->paddingRight = 0;
  self->paddingTop = 0;
  self->mainAlignment = Arcadia_Engine_UI_StackPanelNodeAlignment_Center;
  self->crossAlignment = Arcadia_Engine_UI_StackPanelNodeAlignment_Center;
  self->children = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->slots = NULL;
  self->slotCapacity = 0;
  self->hoveredChild = NULL;
  self->pressedChild = NULL;
  self->focusedChild = NULL;
  self->backgroundPanelNode = NULL;
  self->backgroundBorderWidth = 0;
  self->lastCanvasHeight = 480.f;
  self->focused = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_StackPanelNode);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  )
{
  if (self->slots) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->slots);
    self->slots = NULL;
    self->slotCapacity = 0;
  }
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNodeDispatch* self
  )
{
  (void)thread; (void)self;
}

static void
initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &getChildCountImpl; self->getChildAt = &getChildAtImpl; self->addChild = &addChildImpl; self->removeChild = &removeChildImpl;
}

static void
initializeFocusableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_FocusableDispatch* self
  )
{
  self->focus = &focusImpl; self->blur = &blurImpl; self->isFocused = &isFocusedImpl;
}

static void
initializeHitTestableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_HitTestableDispatch* self
  )
{
  self->hitTest = &hitTestImpl;
}

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{
  self->handleKeyboardKeyEvent = &handleKeyboardKeyEventImpl;
}

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = &handleMousePointerEventImpl; self->handleMouseButtonEvent = &handleMouseButtonEventImpl;
}

static void
initializeRenderableDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_RenderableDispatch* self
  )
{
  self->render = &renderInterfaceImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  )
{
  if (self->children) Arcadia_Object_visit(thread, (Arcadia_Object*)self->children);
  if (self->hoveredChild) Arcadia_Object_visit(thread, self->hoveredChild);
  if (self->pressedChild) Arcadia_Object_visit(thread, self->pressedChild);
  if (self->focusedChild) Arcadia_Object_visit(thread, self->focusedChild);
  if (self->backgroundPanelNode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backgroundPanelNode);
}

static Arcadia_Natural64Value
getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_UI_StackPanelNode*)self)->children);
}

static Arcadia_Object*
getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Natural64Value index
  )
{
  return Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_Engine_UI_StackPanelNode*)self)->children, index);
}

static void
addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_StackPanelNode* panel = (Arcadia_Engine_UI_StackPanelNode*)self; Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->children); ensureSlotCapacity(thread, panel, n + 1); panel->slots[n] = makeDefaultSlot(); Arcadia_List_insertBackObjectReferenceValue(thread, panel->children, child);
}

static void
removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_StackPanelNode* panel = (Arcadia_Engine_UI_StackPanelNode*)self;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)panel->children); i < n; ++i) {
    if (Arcadia_List_getObjectReferenceValueAt(thread, panel->children, i) == child) {
      Arcadia_List_removeAt(thread, panel->children, i, 1);
      for (Arcadia_SizeValue j = i + 1; j < n; ++j) {
        panel->slots[j - 1] = panel->slots[j];
      }
      if (n) panel->slots[n - 1] = makeDefaultSlot();
      if (panel->hoveredChild == child) panel->hoveredChild = NULL;
      if (panel->pressedChild == child) panel->pressedChild = NULL;
      if (panel->focusedChild == child) panel->focusedChild = NULL;
      return;
    }
  }
}

static void
focusImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  ((Arcadia_Engine_UI_StackPanelNode*)self)->focused = Arcadia_BooleanValue_True;
}

static void
blurImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  Arcadia_Engine_UI_StackPanelNode* panel = (Arcadia_Engine_UI_StackPanelNode*)self; panel->focused = Arcadia_BooleanValue_False; focusChild(thread, panel, NULL);
}

static Arcadia_BooleanValue
isFocusedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  return ((Arcadia_Engine_UI_StackPanelNode*)self)->focused;
}

static Arcadia_BooleanValue
hitTestImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y
  )
{
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)self;
  return x >= widget->left && x < widget->left + widget->width && y >= widget->bottom && y < widget->bottom + widget->height;
}

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_StackPanelNode* panel = (Arcadia_Engine_UI_StackPanelNode*)self;
  if (panel->focusedChild && hasInterface(thread, panel->focusedChild, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread))) {
    return
    Arcadia_Engine_UI_KeyboardInput_handleKeyboardKeyEvent
      (
        thread,
        panel->focusedChild,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
handleMousePointerEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MousePointerEvent* event
  )
{
  Arcadia_Engine_UI_StackPanelNode* panel = (Arcadia_Engine_UI_StackPanelNode*)self;
  Arcadia_Object* child = hitTestChild(thread, panel, event->x, (Arcadia_Integer32Value)panel->lastCanvasHeight - event->y);
  if (panel->hoveredChild != child) {
    setHovered(thread, panel->hoveredChild, Arcadia_BooleanValue_False);
    setHovered(thread, child, Arcadia_BooleanValue_True);
  }
  panel->hoveredChild = child;
  Arcadia_Object* receiver = panel->pressedChild ? panel->pressedChild : child;
  if (receiver && hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread))) {
    return
    Arcadia_Engine_UI_PointerInput_handleMousePointerEvent
      (
        thread,
        receiver,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_StackPanelNode* panel = (Arcadia_Engine_UI_StackPanelNode*)self;
  Arcadia_Object* child = hitTestChild(thread, panel, event->x, (Arcadia_Integer32Value)panel->lastCanvasHeight - event->y);
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Pressed) {
    panel->pressedChild = child;
    if (child) focusChild(thread, panel, child);
  }
  Arcadia_Object* receiver = panel->pressedChild ? panel->pressedChild : child;
  if (event->action == Arcadia_Engine_Input_MouseButtonAction_Released) {
    panel->pressedChild = NULL;
  }
  if (receiver && hasInterface(thread, receiver, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread))) {
    return
    Arcadia_Engine_UI_PointerInput_handleMouseButtonEvent
      (
        thread,
        receiver,
        event
      );
  }
  return Arcadia_BooleanValue_False;
}

static void
renderInterfaceImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  renderImpl(thread, (Arcadia_Engine_UI_StackPanelNode*)self, enterPassNode);
}

static void
renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Engine_Visuals_EnterPassNode* enterPassNode
  )
{
  Arcadia_Real32Value canvasWidth;
  Arcadia_Real32Value canvasHeight;
  Arcadia_Engine_Visuals_ViewportNode_getCanvasSize(thread, enterPassNode->viewportNode, &canvasWidth, &canvasHeight);
  self->lastCanvasHeight = canvasHeight;
  arrangeChildren(thread, self);
  if (self->backgroundPanelNode) {
    updateBackgroundPanelVertices(thread, self);
    Arcadia_Engine_UI_Renderable_render(thread, (Arcadia_Object*)self->backgroundPanelNode, enterPassNode);
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->children); i < n; ++i) {
    Arcadia_Object* child = Arcadia_List_getObjectReferenceValueAt(thread, self->children, i);
    if (hasInterface(thread, child, (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Renderable_getType(thread))) {
      Arcadia_Engine_UI_Renderable_render(thread, child, enterPassNode);
    }
  }
}

static void
updateBackgroundPanelVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self
  )
{
  Arcadia_Engine_UI_WidgetNode* widget = (Arcadia_Engine_UI_WidgetNode*)self;
  Arcadia_Engine_UI_WidgetNode* backgroundWidget = (Arcadia_Engine_UI_WidgetNode*)self->backgroundPanelNode;
  if (widget->left == backgroundWidget->left && widget->bottom == backgroundWidget->bottom &&
      widget->width == backgroundWidget->width && widget->height == backgroundWidget->height) {
    return;
  }
  Arcadia_Engine_UI_WidgetNode_setBounds(thread, backgroundWidget, widget->left, widget->bottom, widget->width, widget->height);
  Arcadia_Real32Value left = (Arcadia_Real32Value)widget->left;
  Arcadia_Real32Value bottom = (Arcadia_Real32Value)widget->bottom;
  Arcadia_Real32Value right = left + (Arcadia_Real32Value)widget->width;
  Arcadia_Real32Value top = bottom + (Arcadia_Real32Value)widget->height;
  Arcadia_Real32Value borderWidth = (Arcadia_Real32Value)self->backgroundBorderWidth;
  Arcadia_Real32Value maxBorderWidth = (widget->width < widget->height
    ? (Arcadia_Real32Value)widget->width : (Arcadia_Real32Value)widget->height) * 0.5f;
  if (borderWidth > maxBorderWidth) {
    borderWidth = maxBorderWidth;
  }
  Arcadia_Real32Value* vertices = (Arcadia_Real32Value*)Arcadia_Media_VertexBuffer_getVertices(thread,
    Arcadia_Engine_Visuals_MeshNode_getVertexBuffer(thread, self->backgroundPanelNode->mesh));
  writeQuadPositions(vertices, 0, left + borderWidth, bottom + borderWidth, right - borderWidth, top - borderWidth);
  if (borderWidth > 0.f) {
    writeQuadPositions(vertices, 6, left, bottom, right, bottom + borderWidth);
    writeQuadPositions(vertices, 12, left, top - borderWidth, right, top);
    writeQuadPositions(vertices, 18, left, bottom + borderWidth, left + borderWidth, top - borderWidth);
    writeQuadPositions(vertices, 24, right - borderWidth, bottom + borderWidth, right, top - borderWidth);
  }
  Arcadia_Engine_Visuals_MeshNode_markVerticesDirty(thread, self->backgroundPanelNode->mesh);
}

static void
writeQuadPositions
  (
    Arcadia_Real32Value* vertices,
    Arcadia_SizeValue vertexIndex,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  )
{
  Arcadia_Real32Value const positions[6][3] = {
    { left, bottom, 0.f }, { right, bottom, 0.f }, { left, top, 0.f },
    { right, bottom, 0.f }, { right, top, 0.f }, { left, top, 0.f },
  };
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    Arcadia_SizeValue j = (vertexIndex + i) * 9;
    vertices[j + 0] = positions[i][0];
    vertices[j + 1] = positions[i][1];
    vertices[j + 2] = positions[i][2];
  }
}

Arcadia_Engine_UI_StackPanelNode*
Arcadia_Engine_UI_StackPanelNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_StackPanelNode);
  Arcadia_ValueStack_pushInteger32Value(thread, left);
  Arcadia_ValueStack_pushInteger32Value(thread, bottom);
  Arcadia_ValueStack_pushInteger32Value(thread, width);
  Arcadia_ValueStack_pushInteger32Value(thread, height);
  Arcadia_ValueStack_pushBooleanValue(thread, vertical);
  Arcadia_ValueStack_pushInteger32Value(thread, gap);
  Arcadia_ValueStack_pushNatural8Value(thread, 6);
  _Arcadia_EndCreate(Arcadia_Engine_UI_StackPanelNode);
}

Arcadia_Engine_UI_StackPanelNode*
Arcadia_Engine_UI_StackPanelNode_createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* backendContext,
    Arcadia_ADL_UI_StackPanelDefinition* source
  )
{
  Arcadia_Engine_UI_StackPanelNode* node = Arcadia_Engine_UI_StackPanelNode_create(thread, source->left, source->bottom,
                                                                                   (Arcadia_Integer32Value)source->width,
                                                                                   (Arcadia_Integer32Value)source->height,
                                                                                   source->vertical, source->gap);
  Arcadia_Engine_UI_StackPanelNode_setPadding(thread, node, source->paddingLeft, source->paddingBottom,
                                              source->paddingRight, source->paddingTop);
  Arcadia_Engine_UI_StackPanelNode_setAlignment(thread, node, parseAlignment(thread, source->mainAlignment),
                                                parseAlignment(thread, source->crossAlignment));
  node->backgroundBorderWidth = source->backgroundBorderWidth;
  if (source->backgroundModelName) {
    Arcadia_ADL_Definition* backgroundModel = Arcadia_ADL_Definitions_getDefinitionOrNull(thread,
      ((Arcadia_ADL_Definition*)source)->definitions, source->backgroundModelName);
    if (!backgroundModel || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)backgroundModel, _Arcadia_ADL_ModelDefinition_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    node->backgroundPanelNode = Arcadia_Engine_UI_PanelNode_create(thread, backendContext, (Arcadia_ADL_ModelDefinition*)backgroundModel);
  }
  Arcadia_Engine_UI_populateContainerFromDefinition(thread, backendContext, (Arcadia_Object*)node, (Arcadia_Object*)source);
  return node;
}

void
Arcadia_Engine_UI_StackPanelNode_setPadding
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  )
{
  if (left < 0 || bottom < 0 || right < 0 || top < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->paddingLeft = left;
  self->paddingBottom = bottom;
  self->paddingRight = right;
  self->paddingTop = top;
}

void
Arcadia_Engine_UI_StackPanelNode_setAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Engine_UI_StackPanelNodeAlignment mainAlignment,
    Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment
  )
{
  validateAlignment(thread, mainAlignment);
  validateAlignment(thread, crossAlignment);
  self->mainAlignment = mainAlignment;
  self->crossAlignment = crossAlignment;
}

void
Arcadia_Engine_UI_StackPanelNode_setChildMargin
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Integer32Value left,
    Arcadia_Integer32Value bottom,
    Arcadia_Integer32Value right,
    Arcadia_Integer32Value top
  )
{
  if (left < 0 || bottom < 0 || right < 0 || top < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_StackPanelNodeSlot* slot = &self->slots[findChildIndex(thread, self, child)];
  slot->marginLeft = left;
  slot->marginBottom = bottom;
  slot->marginRight = right;
  slot->marginTop = top;
}

void
Arcadia_Engine_UI_StackPanelNode_setChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child,
    Arcadia_Engine_UI_StackPanelNodeAlignment crossAlignment
  )
{
  validateAlignment(thread, crossAlignment);
  Arcadia_Engine_UI_StackPanelNodeSlot* slot = &self->slots[findChildIndex(thread, self, child)];
  slot->hasAlignmentOverride = Arcadia_BooleanValue_True;
  slot->crossAlignment = crossAlignment;
}

void
Arcadia_Engine_UI_StackPanelNode_clearChildCrossAlignment
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_StackPanelNode* self,
    Arcadia_Object* child
  )
{
  self->slots[findChildIndex(thread, self, child)].hasAlignmentOverride = Arcadia_BooleanValue_False;
}
