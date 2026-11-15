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
#include "Arcadia/Engine/UI/DropdownController.h"
#include "Arcadia/Engine/UI/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_DropdownController);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->canvas = NULL;
  self->anchor = NULL;
  self->popup = NULL;
  self->selectionChanged = Arcadia_Signal_create(thread);
  self->connections = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->selectedIndex = Arcadia_Natural64Value_Maximum;
  self->highlightedIndex = Arcadia_Natural64Value_Maximum;
  self->requestedPopupWidth = 0;
  self->requestedPopupHeight = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_DropdownController);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  if (self->canvas) Arcadia_Object_visit(thread, (Arcadia_Object*)self->canvas);
  if (self->anchor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->anchor);
  if (self->popup) Arcadia_Object_visit(thread, (Arcadia_Object*)self->popup);
  if (self->selectionChanged) Arcadia_Object_visit(thread, (Arcadia_Object*)self->selectionChanged);
  if (self->connections) Arcadia_Object_visit(thread, (Arcadia_Object*)self->connections);
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

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.DropdownController", Arcadia_Engine_UI_DropdownController,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations)

static Arcadia_Natural64Value
count
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{ return Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)self->popup); }

static Arcadia_Engine_UI_ButtonNode*
at
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Natural64Value index
  )
{ return (Arcadia_Engine_UI_ButtonNode*)Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)self->popup, index); }

static Arcadia_Natural64Value
indexOf
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Engine_UI_ButtonNode* button
  )
{
  Arcadia_Natural64Value n = count(thread, self);
  for (Arcadia_Natural64Value i = 0; i < n; ++i) {
    if (at(thread, self, i) == button) return i;
  }
  return n;
}

static Arcadia_Natural64Value
nextEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Natural64Value start,
    Arcadia_BooleanValue forward
  )
{
  Arcadia_Natural64Value n = count(thread, self);
  if (!n) return Arcadia_Natural64Value_Maximum;
  Arcadia_Natural64Value i = start == Arcadia_Natural64Value_Maximum ? (forward ? n - 1 : 0) : start;
  for (Arcadia_Natural64Value step = 0; step < n; ++step) {
    i = forward ? (i + 1 == n ? 0 : i + 1) : (i ? i - 1 : n - 1);
    if (at(thread, self, i)->enabled) return i;
  }
  return Arcadia_Natural64Value_Maximum;
}

static void
highlight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Natural64Value index
  )
{
  if (self->highlightedIndex == index) return;
  if (self->highlightedIndex != Arcadia_Natural64Value_Maximum)
    Arcadia_Engine_UI_ButtonNode_setHovered(thread, at(thread, self, self->highlightedIndex), Arcadia_BooleanValue_False);
  self->highlightedIndex = index;
  if (index != Arcadia_Natural64Value_Maximum)
    Arcadia_Engine_UI_ButtonNode_setHovered(thread, at(thread, self, index), Arcadia_BooleanValue_True);
}

Arcadia_BooleanValue
Arcadia_Engine_UI_DropdownController_isOpen
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{ return Arcadia_Engine_UI_CanvasNode_getPopup(thread, self->canvas) == (Arcadia_Object*)self->popup; }

void
Arcadia_Engine_UI_DropdownController_close
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  highlight(thread, self, Arcadia_Natural64Value_Maximum);
  if (Arcadia_Engine_UI_DropdownController_isOpen(thread, self))
    Arcadia_Engine_UI_CanvasNode_hidePopup(thread, self->canvas);
}

void
Arcadia_Engine_UI_DropdownController_reposition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  Arcadia_Engine_UI_CanvasNode_placePopup(thread, self->canvas, (Arcadia_Engine_UI_WidgetNode*)self->anchor,
    (Arcadia_Engine_UI_WidgetNode*)self->popup, self->requestedPopupWidth, self->requestedPopupHeight);
}

static Arcadia_BooleanValue
onPopupKey
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_DropdownController* self = (Arcadia_Engine_UI_DropdownController*)object;
  if (Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event) != Arcadia_Engine_Input_KeyboardKeyAction_Released)
    return Arcadia_BooleanValue_False;
  Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
  Arcadia_Natural64Value index = Arcadia_Natural64Value_Maximum;
  if (key == Arcadia_Engine_Input_KeyboardKey_DownArrow) {
    index = nextEnabled(thread, self, self->highlightedIndex, Arcadia_BooleanValue_True);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_UpArrow) {
    index = nextEnabled(thread, self, self->highlightedIndex, Arcadia_BooleanValue_False);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_Home) {
    index = nextEnabled(thread, self, Arcadia_Natural64Value_Maximum, Arcadia_BooleanValue_True);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_End) {
    index = nextEnabled(thread, self, Arcadia_Natural64Value_Maximum, Arcadia_BooleanValue_False);
  } else if (key == Arcadia_Engine_Input_KeyboardKey_Return || key == Arcadia_Engine_Input_KeyboardKey_Space) {
    if (self->highlightedIndex != Arcadia_Natural64Value_Maximum &&
        at(thread, self, self->highlightedIndex)->enabled) {
      Arcadia_Natural64Value chosen = self->highlightedIndex;
      Arcadia_Engine_UI_DropdownController_close(thread, self);
      Arcadia_Engine_UI_DropdownController_select(thread, self, chosen);
    } else {
      Arcadia_Engine_UI_DropdownController_close(thread, self);
    }
    return Arcadia_BooleanValue_True;
  } else {
    return Arcadia_BooleanValue_False;
  }
  highlight(thread, self, index);
  if (index != Arcadia_Natural64Value_Maximum) {
    // Keep a keyboard-highlighted option visible in long scrolling lists.
    Arcadia_Engine_UI_WidgetNode* option = (Arcadia_Engine_UI_WidgetNode*)at(thread, self, index);
    Arcadia_Integer32Value y = option->bottom;
    if (y < self->popup->scrollY) Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)self->popup, 0, y);
    else if (y + option->height > self->popup->scrollY + ((Arcadia_Engine_UI_WidgetNode*)self->popup)->height)
      Arcadia_Engine_UI_Scrollable_scrollTo(thread, (Arcadia_Object*)self->popup, 0,
        y + option->height - ((Arcadia_Engine_UI_WidgetNode*)self->popup)->height);
  }
  return Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_DropdownController_open
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  if (!self->anchor->enabled || Arcadia_Engine_UI_DropdownController_isOpen(thread, self)) return;
  Arcadia_Natural64Value n = count(thread, self);
  Arcadia_Natural64Value first = nextEnabled(thread, self, Arcadia_Natural64Value_Maximum, Arcadia_BooleanValue_True);
  if (first == Arcadia_Natural64Value_Maximum || !n) return;
  if (self->canvas->visualsCanvasWidth <= 0 || self->canvas->visualsCanvasHeight <= 0) return;
  Arcadia_Engine_UI_DropdownController_reposition(thread, self);
  Arcadia_Engine_UI_CanvasNode_showPopup(thread, self->canvas, (Arcadia_Object*)self->popup);
  Arcadia_Engine_UI_CanvasNode_setPopupKeyboardHandler(thread, self->canvas, (Arcadia_Object*)self, &onPopupKey);
  highlight(thread, self, self->selectedIndex != Arcadia_Natural64Value_Maximum &&
    at(thread, self, self->selectedIndex)->enabled ? self->selectedIndex : first);
}

void
Arcadia_Engine_UI_DropdownController_select
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_Natural64Value index
  )
{
  if (index >= count(thread, self) || !at(thread, self, index)->enabled) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (self->selectedIndex == index) return;
  if (self->selectedIndex != Arcadia_Natural64Value_Maximum)
    Arcadia_Engine_UI_ButtonNode_setSelected(thread, at(thread, self, self->selectedIndex), Arcadia_BooleanValue_False);
  self->selectedIndex = index;
  Arcadia_Engine_UI_ButtonNode_setSelected(thread, at(thread, self, index), Arcadia_BooleanValue_True);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, self->selectionChanged, (Arcadia_Object*)self);
  Arcadia_ValueStack_popValues(thread, 1);
}

Arcadia_Natural64Value
Arcadia_Engine_UI_DropdownController_getSelectedIndex
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{ return self->selectedIndex; }

Arcadia_String*
Arcadia_Engine_UI_DropdownController_getSelectedValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  return self->selectedIndex == Arcadia_Natural64Value_Maximum ? NULL :
    Arcadia_Engine_UI_WidgetNode_getName(thread, (Arcadia_Engine_UI_WidgetNode*)at(thread, self, self->selectedIndex));
}

Arcadia_String*
Arcadia_Engine_UI_DropdownController_getSelectedLabel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{
  return self->selectedIndex == Arcadia_Natural64Value_Maximum ? NULL :
    at(thread, self, self->selectedIndex)->source->text;
}

void
Arcadia_Engine_UI_DropdownController_selectValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_String* value
  )
{
  if (value) {
    Arcadia_Value boxed = Arcadia_Value_makeObjectReferenceValue(value);
    for (Arcadia_Natural64Value i = 0, n = count(thread, self); i < n; ++i) {
      Arcadia_String* name = Arcadia_Engine_UI_WidgetNode_getName(thread, (Arcadia_Engine_UI_WidgetNode*)at(thread, self, i));
      if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)name, &boxed)) {
        Arcadia_Engine_UI_DropdownController_select(thread, self, i);
        return;
      }
    }
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_Engine_UI_DropdownController_setEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self,
    Arcadia_BooleanValue enabled
  )
{
  if (!enabled) Arcadia_Engine_UI_DropdownController_close(thread, self);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, self->anchor, enabled);
}

Arcadia_Signal*
Arcadia_Engine_UI_DropdownController_getSelectionChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_DropdownController* self
  )
{ return self->selectionChanged; }

static void
onAnchorActivated
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Engine_UI_DropdownController* self = (Arcadia_Engine_UI_DropdownController*)
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_DropdownController_getType(thread));
  if (Arcadia_Engine_UI_DropdownController_isOpen(thread, self)) Arcadia_Engine_UI_DropdownController_close(thread, self);
  else Arcadia_Engine_UI_DropdownController_open(thread, self);
}

static void
onOptionActivated
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Engine_UI_DropdownController* self = (Arcadia_Engine_UI_DropdownController*)
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_DropdownController_getType(thread));
  Arcadia_Engine_UI_ButtonNode* button = (Arcadia_Engine_UI_ButtonNode*)
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_UI_ButtonNode_getType(thread));
  Arcadia_Natural64Value index = indexOf(thread, self, button);
  if (index != count(thread, self) && button->enabled && Arcadia_Engine_UI_DropdownController_isOpen(thread, self)) {
    Arcadia_Engine_UI_DropdownController_close(thread, self);
    Arcadia_Engine_UI_DropdownController_select(thread, self, index);
  }
}

static Arcadia_Engine_UI_DropdownController*
createEmpty
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_DropdownController);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_DropdownController);
}

Arcadia_Engine_UI_DropdownController*
Arcadia_Engine_UI_DropdownController_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_Engine_UI_ButtonNode* anchor,
    Arcadia_Engine_UI_ScrollPanelNode* popup
  )
{
  if (!canvas || !anchor || !popup || (Arcadia_Object*)anchor == (Arcadia_Object*)popup) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Validate all options and values before connecting anything.
  Arcadia_Natural64Value n = Arcadia_Engine_UI_Container_getChildCount(thread, (Arcadia_Object*)popup);
  for (Arcadia_Natural64Value i = 0; i < n; ++i) {
    Arcadia_Object* object = Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)popup, i);
    if (!object || !Arcadia_Object_isInstanceOf(thread, object, _Arcadia_Engine_UI_ButtonNode_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_String* name = Arcadia_Engine_UI_WidgetNode_getName(thread, (Arcadia_Engine_UI_WidgetNode*)object);
    if (!name) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Value boxed = Arcadia_Value_makeObjectReferenceValue(name);
    for (Arcadia_Natural64Value j = 0; j < i; ++j) {
      Arcadia_String* previous = Arcadia_Engine_UI_WidgetNode_getName(thread,
        (Arcadia_Engine_UI_WidgetNode*)Arcadia_Engine_UI_Container_getChildAt(thread, (Arcadia_Object*)popup, j));
      if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)previous, &boxed)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
    }
  }
  Arcadia_Engine_UI_DropdownController* self = createEmpty(thread);
  // The receiver is rooted through all potentially collecting signal connects.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
  self->canvas = canvas;
  self->anchor = anchor;
  self->popup = popup;
  self->requestedPopupWidth = ((Arcadia_Engine_UI_WidgetNode*)popup)->width;
  self->requestedPopupHeight = ((Arcadia_Engine_UI_WidgetNode*)popup)->height;
  Arcadia_Slot* anchorSlot = Arcadia_Signal_connect(thread,
    Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, anchor), (Arcadia_Object*)self, &onAnchorActivated);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)anchorSlot);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->connections, (Arcadia_Object*)anchorSlot);
  Arcadia_ValueStack_popValues(thread, 1);
  for (Arcadia_Natural64Value i = 0; i < n; ++i) {
    Arcadia_Slot* slot = Arcadia_Signal_connect(thread,
      Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, at(thread, self, i)),
      (Arcadia_Object*)self, &onOptionActivated);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)slot);
    Arcadia_List_insertBackObjectReferenceValue(thread, self->connections, (Arcadia_Object*)slot);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return self;
}
