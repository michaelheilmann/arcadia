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
#include "Arcadia/Engine/UI/Nodes/TabBarNode.h"
#include "Arcadia/Engine/UI/Include.h"

static Arcadia_Engine_UI_ContainerDispatch*
parentContainer
  (
    Arcadia_Thread* thread
  )
{
  return (Arcadia_Engine_UI_ContainerDispatch*)Arcadia_ObjectType_getInterfaceDispatch
    (
      thread,
      (Arcadia_ObjectType*)_Arcadia_Engine_UI_LayoutPanelNode_getType(thread),
      (Arcadia_InterfaceType*)_Arcadia_Engine_UI_Container_getType(thread)
    );
}

static Arcadia_Engine_UI_PointerInputDispatch*
parentPointer
  (
    Arcadia_Thread* thread
  )
{
  return (Arcadia_Engine_UI_PointerInputDispatch*)Arcadia_ObjectType_getInterfaceDispatch
    (
      thread,
      (Arcadia_ObjectType*)_Arcadia_Engine_UI_LayoutPanelNode_getType(thread),
      (Arcadia_InterfaceType*)_Arcadia_Engine_UI_PointerInput_getType(thread)
    );
}

static Arcadia_Engine_UI_KeyboardInputDispatch*
parentKeyboard
  (
    Arcadia_Thread* thread
  )
{
  return (Arcadia_Engine_UI_KeyboardInputDispatch*)Arcadia_ObjectType_getInterfaceDispatch
    (
      thread,
      (Arcadia_ObjectType*)_Arcadia_Engine_UI_LayoutPanelNode_getType(thread),
      (Arcadia_InterfaceType*)_Arcadia_Engine_UI_KeyboardInput_getType(thread)
    );
}

static Arcadia_SizeValue
count
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  )
{
  return Arcadia_Collection_getSize(thread,
    (Arcadia_Collection*)((Arcadia_Engine_UI_LayoutPanelNode*)self)->children);
}

static Arcadia_Engine_UI_ButtonNode*
at
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_SizeValue index
  )
{
  return (Arcadia_Engine_UI_ButtonNode*)Arcadia_List_getObjectReferenceValueAt(thread,
    ((Arcadia_Engine_UI_LayoutPanelNode*)self)->children, index);
}

static Arcadia_SizeValue
indexOf
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_Engine_UI_ButtonNode* button
  )
{
  Arcadia_SizeValue n = count(thread, self);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    if (at(thread, self, i) == button) return i;
  }
  return n;
}

// Search one whole cycle, excluding start when it is an existing tab.
static Arcadia_Engine_UI_ButtonNode*
nextEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_SizeValue start,
    Arcadia_BooleanValue forward
  )
{
  Arcadia_SizeValue n = count(thread, self);
  if (!n) return NULL;
  Arcadia_SizeValue i = start == n ? (forward ? n - 1 : 0) : start;
  for (Arcadia_SizeValue step = 0; step < n; ++step) {
    i = forward ? (i + 1 == n ? 0 : i + 1) : (i ? i - 1 : n - 1);
    Arcadia_Engine_UI_ButtonNode* button = at(thread, self, i);
    if (button->enabled) return button;
  }
  return NULL;
}

static void
focusButton
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_Engine_UI_ButtonNode* button
  )
{
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
  if (panel->focusedChild == (Arcadia_Object*)button) return;
  if (panel->focusedChild)
    Arcadia_Engine_UI_Focusable_blur(thread, panel->focusedChild);
  panel->focusedChild = (Arcadia_Object*)button;
  if (button) Arcadia_Engine_UI_Focusable_focus(thread, (Arcadia_Object*)button);
}

static void
changeSelection
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_Engine_UI_ButtonNode* button
  )
{
  if (self->selected == button) return;
  if (self->selected) Arcadia_Engine_UI_ButtonNode_setSelected(thread, self->selected, Arcadia_BooleanValue_False);
  self->selected = button;
  if (button) Arcadia_Engine_UI_ButtonNode_setSelected(thread, button, Arcadia_BooleanValue_True);
  // State is final before notification. Never overwrite it after callbacks,
  // because a callback may remove a tab or select another tab re-entrantly.
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, self->selectionChanged, (Arcadia_Object*)self);
  Arcadia_ValueStack_popValues(thread, 1);
}

void
Arcadia_Engine_UI_TabBarNode_select
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self,
    Arcadia_Engine_UI_ButtonNode* button
  )
{
  if (button && (indexOf(thread, self, button) == count(thread, self) || !button->enabled)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  changeSelection(thread, self, button);
}

static void
onActivated
  (
    Arcadia_Thread* thread
  )
{
  if (Arcadia_ValueStack_getNatural8Value(thread, 0) != 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_TabBarNode* self = (Arcadia_Engine_UI_TabBarNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2,
    _Arcadia_Engine_UI_TabBarNode_getType(thread));
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1,
    _Arcadia_Engine_UI_ButtonNode_getType(thread));
  // A queued/old activation from a removed or now-disabled button is ignored.
  if (button->enabled && indexOf(thread, self, button) != count(thread, self))
    Arcadia_Engine_UI_TabBarNode_select(thread, self, button);
}

static void
onEnabledChanged
  (
    Arcadia_Thread* thread
  )
{
  if (Arcadia_ValueStack_getNatural8Value(thread, 0) != 2) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_TabBarNode* self = (Arcadia_Engine_UI_TabBarNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2,
    _Arcadia_Engine_UI_TabBarNode_getType(thread));
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1,
    _Arcadia_Engine_UI_ButtonNode_getType(thread));
  if (button->enabled) return;
  Arcadia_SizeValue index = indexOf(thread, self, button);
  if (index == count(thread, self)) return;
  Arcadia_Engine_UI_ButtonNode* replacement = nextEnabled(thread, self, index, Arcadia_BooleanValue_True);
  if (((Arcadia_Engine_UI_LayoutPanelNode*)self)->focusedChild == (Arcadia_Object*)button)
    focusButton(thread, self, replacement);
  if (self->selected == button) changeSelection(thread, self, replacement);
}

static Arcadia_Natural64Value
getChildCountImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{ return parentContainer(thread)->getChildCount(thread, object); }

static Arcadia_Object*
getChildAtImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Natural64Value index
  )
{ return parentContainer(thread)->getChildAt(thread, object, index); }

static void
addChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_TabBarNode* self = (Arcadia_Engine_UI_TabBarNode*)object;
  if (!child || !Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ButtonNode_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)child;
  if (indexOf(thread, self, button) != count(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Connect only after the parent accepts the child. Keep both slot objects
  // on the value stack until the two managed connection lists retain them.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_BooleanValue added = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue storedActivation = Arcadia_BooleanValue_False;
  Arcadia_Slot* activatedSlot = NULL;
  Arcadia_Slot* enabledSlot = NULL;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    parentContainer(thread)->addChild(thread, object, child);
    added = Arcadia_BooleanValue_True;
    activatedSlot = Arcadia_Signal_connect(thread,
      Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, button), object, &onActivated);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)activatedSlot);
    enabledSlot = Arcadia_Signal_connect(thread,
      Arcadia_Engine_UI_ButtonNode_getEnabledChangedSignal(thread, button), object, &onEnabledChanged);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)enabledSlot);
    Arcadia_List_insertBackObjectReferenceValue(thread, self->activationConnections, (Arcadia_Object*)activatedSlot);
    storedActivation = Arcadia_BooleanValue_True;
    Arcadia_List_insertBackObjectReferenceValue(thread, self->enabledConnections, (Arcadia_Object*)enabledSlot);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (storedActivation) Arcadia_List_removeAt(thread, self->activationConnections,
      Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->activationConnections) - 1, 1);
    if (activatedSlot) Arcadia_Slot_disconnect(thread, activatedSlot);
    if (enabledSlot) Arcadia_Slot_disconnect(thread, enabledSlot);
    if (added) parentContainer(thread)->removeChild(thread, object, child);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
removeChildImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Object* child
  )
{
  Arcadia_Engine_UI_TabBarNode* self = (Arcadia_Engine_UI_TabBarNode*)object;
  if (!child || !Arcadia_Object_isInstanceOf(thread, child, _Arcadia_Engine_UI_ButtonNode_getType(thread))) return;
  Arcadia_SizeValue i = indexOf(thread, self, (Arcadia_Engine_UI_ButtonNode*)child);
  if (i == count(thread, self)) return;
  // Keep a removed selected button alive until its selected styling is cleared.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
  Arcadia_Slot* activatedSlot = (Arcadia_Slot*)Arcadia_List_getObjectReferenceValueAt(thread, self->activationConnections, i);
  Arcadia_Slot* enabledSlot = (Arcadia_Slot*)Arcadia_List_getObjectReferenceValueAt(thread, self->enabledConnections, i);
  Arcadia_Slot_disconnect(thread, activatedSlot);
  Arcadia_Slot_disconnect(thread, enabledSlot);
  Arcadia_List_removeAt(thread, self->activationConnections, i, 1);
  Arcadia_List_removeAt(thread, self->enabledConnections, i, 1);
  parentContainer(thread)->removeChild(thread, object, child);
  if (self->selected == (Arcadia_Engine_UI_ButtonNode*)child) {
    Arcadia_SizeValue remaining = count(thread, self);
    // The child after the removed slot moved to i; begin just before it.
    Arcadia_SizeValue start = remaining ? (i < remaining ? (i ? i - 1 : remaining - 1) : remaining - 1) : 0;
    Arcadia_Engine_UI_ButtonNode* next = nextEnabled(thread, self, start, Arcadia_BooleanValue_True);
    changeSelection(thread, self, next);
  }
  Arcadia_ValueStack_popValues(thread, 1);
}

static Arcadia_BooleanValue
handleKeyboardKeyEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_TabBarNode* self = (Arcadia_Engine_UI_TabBarNode*)object;
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) == Arcadia_Engine_Input_KeyboardKeyAction_Released) {
    Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
    if (key == Arcadia_Engine_Input_KeyboardKey_Tabulator) {
      Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
      Arcadia_Engine_UI_ButtonNode* focused = (Arcadia_Engine_UI_ButtonNode*)panel->focusedChild;
      Arcadia_Engine_UI_ButtonNode* next = nextEnabled(thread, self, indexOf(thread, self, focused), Arcadia_BooleanValue_True);
      if (next) { focusButton(thread, self, next); return Arcadia_BooleanValue_True; }
      return Arcadia_BooleanValue_False;
    }
    Arcadia_BooleanValue forward = Arcadia_BooleanValue_False;
    Arcadia_BooleanValue backward = Arcadia_BooleanValue_False;
    if (self->vertical) {
      forward = key == Arcadia_Engine_Input_KeyboardKey_DownArrow;
      backward = key == Arcadia_Engine_Input_KeyboardKey_UpArrow;
    } else {
      forward = key == Arcadia_Engine_Input_KeyboardKey_RightArrow;
      backward = key == Arcadia_Engine_Input_KeyboardKey_LeftArrow;
    }
    if (forward || backward) {
      Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
      Arcadia_Engine_UI_ButtonNode* current = (Arcadia_Engine_UI_ButtonNode*)panel->focusedChild;
      if (!current) current = self->selected;
      Arcadia_Engine_UI_ButtonNode* next = nextEnabled(thread, self, indexOf(thread, self, current), forward);
      if (next) {
        focusButton(thread, self, next);
        changeSelection(thread, self, next);
        return Arcadia_BooleanValue_True;
      }
      return Arcadia_BooleanValue_False;
    }
  }
  return parentKeyboard(thread)->handleKeyboardKeyEvent(thread, object, event);
}

static Arcadia_BooleanValue
handleMouseButtonEventImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_TabBarNode* self = (Arcadia_Engine_UI_TabBarNode*)object;
  Arcadia_BooleanValue handled = parentPointer(thread)->handleMouseButtonEvent(thread, object, event);
  // The generic panel can focus any hit-testable child, including disabled
  // buttons; tab-bar focus traversal must skip those.
  Arcadia_Engine_UI_LayoutPanelNode* panel = (Arcadia_Engine_UI_LayoutPanelNode*)self;
  Arcadia_Engine_UI_ButtonNode* focused = (Arcadia_Engine_UI_ButtonNode*)panel->focusedChild;
  if (focused && !focused->enabled) {
    Arcadia_Engine_UI_ButtonNode* replacement = self->selected;
    if (!replacement || !replacement->enabled)
      replacement = nextEnabled(thread, self, indexOf(thread, self, focused), Arcadia_BooleanValue_True);
    focusButton(thread, self, replacement);
  }
  return handled;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_TabBarNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue vertical = Arcadia_ValueStack_getBooleanValue(thread, 2);
  Arcadia_Integer32Value gap = Arcadia_ValueStack_getInteger32Value(thread, 1);
  if (gap < 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->vertical = vertical;
  self->selected = NULL;
  self->activationConnections = NULL;
  self->enabledConnections = NULL;
  self->selectionChanged = NULL;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    self->activationConnections = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self->activationConnections);
    self->enabledConnections = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self->enabledConnections);
    self->selectionChanged = Arcadia_Signal_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self->selectionChanged);
    Arcadia_Engine_UI_StackLayout* layout = Arcadia_Engine_UI_StackLayout_create(thread, vertical, gap);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)layout);
    Arcadia_Engine_UI_LayoutPanelNode_setLayout(thread, (Arcadia_Engine_UI_LayoutPanelNode*)self, (Arcadia_Object*)layout);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_TabBarNode);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  )
{
  if (self->activationConnections) Arcadia_Object_visit(thread, (Arcadia_Object*)self->activationConnections);
  if (self->enabledConnections) Arcadia_Object_visit(thread, (Arcadia_Object*)self->enabledConnections);
  if (self->selectionChanged) Arcadia_Object_visit(thread, (Arcadia_Object*)self->selectionChanged);
  if (self->selected) Arcadia_Object_visit(thread, (Arcadia_Object*)self->selected);
}

static void
initializeContainerDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_ContainerDispatch* self
  )
{
  self->getChildCount = &getChildCountImpl;
  self->getChildAt = &getChildAtImpl;
  self->addChild = &addChildImpl;
  self->removeChild = &removeChildImpl;
}

static void
initializeKeyboardInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_KeyboardInputDispatch* self
  )
{ self->handleKeyboardKeyEvent = &handleKeyboardKeyEventImpl; }

static void
initializePointerInputDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_PointerInputDispatch* self
  )
{
  self->handleMousePointerEvent = parentPointer(thread)->handleMousePointerEvent;
  self->handleMouseButtonEvent = &handleMouseButtonEventImpl;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.TabBarNode", Arcadia_Engine_UI_TabBarNode,
    u8"Arcadia.Engine.UI.LayoutPanelNode", Arcadia_Engine_UI_LayoutPanelNode, &typeOperations,
    { .interfaceType = _Arcadia_Engine_UI_Container_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_ContainerDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeContainerDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_KeyboardInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_KeyboardInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializeKeyboardInputDispatchImpl },
    { .interfaceType = _Arcadia_Engine_UI_PointerInput_getType(thread), .dispatchSize = sizeof(Arcadia_Engine_UI_PointerInputDispatch), .initializeDispatch = (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&initializePointerInputDispatchImpl }
  )

Arcadia_Engine_UI_TabBarNode*
Arcadia_Engine_UI_TabBarNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue vertical,
    Arcadia_Integer32Value gap
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_TabBarNode);
  Arcadia_ValueStack_pushBooleanValue(thread, vertical);
  Arcadia_ValueStack_pushInteger32Value(thread, gap);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Engine_UI_TabBarNode);
}

Arcadia_Engine_UI_ButtonNode*
Arcadia_Engine_UI_TabBarNode_getSelected
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  )
{ return self->selected; }

Arcadia_Signal*
Arcadia_Engine_UI_TabBarNode_getSelectionChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_TabBarNode* self
  )
{ return self->selectionChanged; }
