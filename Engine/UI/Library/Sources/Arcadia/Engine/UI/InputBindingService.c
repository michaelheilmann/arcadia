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
#include "Arcadia/Engine/UI/InputBindingService.h"
#include "Arcadia/Engine/UI/Include.h"
#include <string.h>

static Arcadia_SizeValue
count
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{ return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->actionNames); }

static Arcadia_SizeValue
indexOf
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  )
{
  Arcadia_SizeValue n = count(thread, self);
  if (!action) return n;
  Arcadia_Value expected = Arcadia_Value_makeObjectReferenceValue(action);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_String* current = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, self->actionNames, i);
    if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)current, &expected)) return i;
  }
  return n;
}

static void
invalidAction
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_BooleanValue
sameBinding
  (
    Arcadia_Engine_UI_InputBinding a,
    Arcadia_Engine_UI_InputBinding b
  )
{
  if (!a.bound || !b.bound) return !a.bound && !b.bound;
  return a.device == b.device && a.code == b.code && a.modifiers == b.modifiers;
}

static Arcadia_BooleanValue
modifierKey
  (
    Arcadia_Engine_Input_KeyboardKey key
  )
{
  return key == Arcadia_Engine_Input_KeyboardKey_LeftShift || key == Arcadia_Engine_Input_KeyboardKey_RightShift ||
    key == Arcadia_Engine_Input_KeyboardKey_LeftControl || key == Arcadia_Engine_Input_KeyboardKey_RightControl ||
    key == Arcadia_Engine_Input_KeyboardKey_LeftMenu || key == Arcadia_Engine_Input_KeyboardKey_RightMenu;
}

static Arcadia_BooleanValue
isValid
  (
    Arcadia_Engine_UI_InputBinding binding
  )
{
  if (!binding.bound) return Arcadia_BooleanValue_True;
  if (binding.modifiers & ~7) return Arcadia_BooleanValue_False;
  if (binding.device == Arcadia_Engine_UI_InputBindingDevice_MouseButton)
    return binding.code <= 4;
  if (binding.device != Arcadia_Engine_UI_InputBindingDevice_Keyboard ||
      binding.code == 0 || binding.code > 255) return Arcadia_BooleanValue_False;
  Arcadia_Engine_Input_KeyboardKey key = (Arcadia_Engine_Input_KeyboardKey)binding.code;
  if (modifierKey(key) || key == Arcadia_Engine_Input_KeyboardKey_Escape ||
      key == Arcadia_Engine_Input_KeyboardKey_Tabulator ||
      (key == Arcadia_Engine_Input_KeyboardKey_F4 && (binding.modifiers & Arcadia_Engine_UI_InputBindingModifier_Alt)) ||
      (key == Arcadia_Engine_Input_KeyboardKey_Delete &&
       (binding.modifiers & (Arcadia_Engine_UI_InputBindingModifier_Control | Arcadia_Engine_UI_InputBindingModifier_Alt)) ==
       (Arcadia_Engine_UI_InputBindingModifier_Control | Arcadia_Engine_UI_InputBindingModifier_Alt)))
    return Arcadia_BooleanValue_False;
  return Arcadia_BooleanValue_True;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_InputBindingService);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->actionNames = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->bindings = NULL;
  self->bindingCapacity = 0;
  self->changed = Arcadia_Signal_create(thread);
  self->captureCanvas = NULL;
  self->captureAction = NULL;
  self->conflictAction = NULL;
  self->modifierSides = 0;
  self->captureState = Arcadia_Engine_UI_InputCaptureState_Idle;
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_InputBindingService);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{
  if (self->bindings) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->bindings);
    self->bindings = NULL;
  }
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{
  if (self->actionNames) Arcadia_Object_visit(thread, (Arcadia_Object*)self->actionNames);
  if (self->changed) Arcadia_Object_visit(thread, (Arcadia_Object*)self->changed);
  if (self->captureCanvas) Arcadia_Object_visit(thread, (Arcadia_Object*)self->captureCanvas);
  if (self->captureAction) Arcadia_Object_visit(thread, (Arcadia_Object*)self->captureAction);
  if (self->conflictAction) Arcadia_Object_visit(thread, (Arcadia_Object*)self->conflictAction);
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.InputBindingService", Arcadia_Engine_UI_InputBindingService,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations)

Arcadia_Engine_UI_InputBindingService*
Arcadia_Engine_UI_InputBindingService_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_InputBindingService);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_InputBindingService);
}

void
Arcadia_Engine_UI_InputBindingService_addAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  )
{
  Arcadia_SizeValue n = count(thread, self);
  if (!action || indexOf(thread, self, action) != n) invalidAction(thread);
  Arcadia_Engine_UI_InputBinding* replacement = NULL;
  Arcadia_SizeValue nextCapacity = self->bindingCapacity;
  if (n == self->bindingCapacity) {
    if (n > Arcadia_SizeValue_Maximum / 2 || n > Arcadia_SizeValue_Maximum / sizeof(Arcadia_Engine_UI_InputBinding))
      invalidAction(thread);
    nextCapacity = n ? n * 2 : 8;
    if (nextCapacity > Arcadia_SizeValue_Maximum / sizeof(Arcadia_Engine_UI_InputBinding))
      invalidAction(thread);
    replacement = Arcadia_Memory_allocateUnmanaged(thread, nextCapacity * sizeof(Arcadia_Engine_UI_InputBinding));
    if (n) memcpy(replacement, self->bindings, n * sizeof(Arcadia_Engine_UI_InputBinding));
  }
  // Root receiver and action across collecting list insertion.
  Arcadia_SizeValue stackBase = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)action);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_List_insertBackObjectReferenceValue(thread, self->actionNames, (Arcadia_Object*)action);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (replacement) Arcadia_Memory_deallocateUnmanaged(thread, replacement);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackBase);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackBase);
  if (replacement) {
    if (self->bindings) Arcadia_Memory_deallocateUnmanaged(thread, self->bindings);
    self->bindings = replacement;
    self->bindingCapacity = nextCapacity;
  }
  self->bindings[n] = (Arcadia_Engine_UI_InputBinding){0};
}

void
Arcadia_Engine_UI_InputBindingService_removeAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  )
{
  Arcadia_SizeValue n = count(thread, self);
  Arcadia_SizeValue i = indexOf(thread, self, action);
  if (i == n || (self->captureAction && indexOf(thread, self, self->captureAction) == i)) invalidAction(thread);
  Arcadia_List_removeAt(thread, self->actionNames, i, 1);
  if (i + 1 < n) memmove(self->bindings + i, self->bindings + i + 1, (n - i - 1) * sizeof(Arcadia_Engine_UI_InputBinding));
}

Arcadia_Engine_UI_InputBinding
Arcadia_Engine_UI_InputBindingService_get
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action
  )
{
  Arcadia_SizeValue i = indexOf(thread, self, action);
  if (i == count(thread, self)) invalidAction(thread);
  return self->bindings[i];
}

Arcadia_Engine_UI_InputBindingResult
Arcadia_Engine_UI_InputBindingService_assign
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_String* action,
    Arcadia_Engine_UI_InputBinding binding
  )
{
  Arcadia_SizeValue n = count(thread, self);
  Arcadia_SizeValue i = indexOf(thread, self, action);
  if (i == n) invalidAction(thread);
  self->conflictAction = NULL;
  if (!isValid(binding)) return Arcadia_Engine_UI_InputBindingResult_Rejected;
  if (!binding.bound) binding = (Arcadia_Engine_UI_InputBinding){0};
  if (sameBinding(self->bindings[i], binding)) return Arcadia_Engine_UI_InputBindingResult_Unchanged;
  if (binding.bound) {
    for (Arcadia_SizeValue j = 0; j < n; ++j) {
      if (j != i && sameBinding(self->bindings[j], binding)) {
        self->conflictAction = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, self->actionNames, j);
        return Arcadia_Engine_UI_InputBindingResult_Conflict;
      }
    }
  }
  self->bindings[i] = binding;
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, self->changed, (Arcadia_Object*)self);
  Arcadia_ValueStack_popValues(thread, 1);
  return Arcadia_Engine_UI_InputBindingResult_Changed;
}

static Arcadia_Natural8Value
modifierFlags
  (
    Arcadia_Natural8Value sides
  )
{
  return ((sides & 3) ? Arcadia_Engine_UI_InputBindingModifier_Shift : 0) |
         ((sides & 12) ? Arcadia_Engine_UI_InputBindingModifier_Control : 0) |
         ((sides & 48) ? Arcadia_Engine_UI_InputBindingModifier_Alt : 0);
}

static Arcadia_Natural8Value
modifierSide
  (
    Arcadia_Engine_Input_KeyboardKey key
  )
{
  switch (key) {
    case Arcadia_Engine_Input_KeyboardKey_LeftShift: return 1;
    case Arcadia_Engine_Input_KeyboardKey_RightShift: return 2;
    case Arcadia_Engine_Input_KeyboardKey_LeftControl: return 4;
    case Arcadia_Engine_Input_KeyboardKey_RightControl: return 8;
    case Arcadia_Engine_Input_KeyboardKey_LeftMenu: return 16;
    case Arcadia_Engine_Input_KeyboardKey_RightMenu: return 32;
    default: return 0;
  }
}

static void
finishCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_Engine_UI_InputCaptureState state
  )
{
  Arcadia_Engine_UI_CanvasNode* canvas = self->captureCanvas;
  self->captureAction = NULL;
  self->captureCanvas = NULL;
  self->modifierSides = 0;
  self->captureState = state;
  if (canvas) Arcadia_Engine_UI_CanvasNode_endInputCapture(thread, canvas, (Arcadia_Object*)self);
}

static Arcadia_BooleanValue
onKeyboard
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_KeyboardKeyEvent* event
  )
{
  Arcadia_Engine_UI_InputBindingService* self = (Arcadia_Engine_UI_InputBindingService*)object;
  if (!self->captureAction) return Arcadia_BooleanValue_False;
  Arcadia_Engine_Input_KeyboardKey key = Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, event);
  Arcadia_Engine_Input_KeyboardKeyAction action = Arcadia_Engine_Input_KeyboardKeyEvent_getAction(thread, event);
  Arcadia_Natural8Value side = modifierSide(key);
  if (side) {
    if (action == Arcadia_Engine_Input_KeyboardKeyAction_Pressed) self->modifierSides |= side;
    else if (action == Arcadia_Engine_Input_KeyboardKeyAction_Released) self->modifierSides &= ~side;
    return Arcadia_BooleanValue_True;
  }
  if (action != Arcadia_Engine_Input_KeyboardKeyAction_Pressed) return Arcadia_BooleanValue_True;
  if (key == Arcadia_Engine_Input_KeyboardKey_Escape || key == Arcadia_Engine_Input_KeyboardKey_Tabulator) {
    Arcadia_Engine_UI_CanvasNode_suppressKeyboardRelease(thread, self->captureCanvas, key);
    finishCapture(thread, self, Arcadia_Engine_UI_InputCaptureState_Cancelled);
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Engine_UI_InputBinding binding = {
    .device = Arcadia_Engine_UI_InputBindingDevice_Keyboard,
    .code = (Arcadia_Natural32Value)key,
    .modifiers = modifierFlags(self->modifierSides),
    .bound = Arcadia_BooleanValue_True,
  };
  Arcadia_Engine_UI_InputBindingResult result = Arcadia_Engine_UI_InputBindingService_assign(thread, self, self->captureAction, binding);
  if (result == Arcadia_Engine_UI_InputBindingResult_Conflict) self->captureState = Arcadia_Engine_UI_InputCaptureState_Conflict;
  else if (result == Arcadia_Engine_UI_InputBindingResult_Rejected) self->captureState = Arcadia_Engine_UI_InputCaptureState_Rejected;
  else {
    Arcadia_Engine_UI_CanvasNode_suppressKeyboardRelease(thread, self->captureCanvas, key);
    finishCapture(thread, self, Arcadia_Engine_UI_InputCaptureState_Committed);
  }
  return Arcadia_BooleanValue_True;
}

static Arcadia_BooleanValue
onMouseButton
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Engine_Input_MouseButtonEvent* event
  )
{
  Arcadia_Engine_UI_InputBindingService* self = (Arcadia_Engine_UI_InputBindingService*)object;
  if (!self->captureAction) return Arcadia_BooleanValue_False;
  if (event->action != Arcadia_Engine_Input_MouseButtonAction_Pressed) return Arcadia_BooleanValue_True;
  // Mouse button ids are Natural64; reject before narrowing to the binding code.
  if (event->button > 4) {
    self->captureState = Arcadia_Engine_UI_InputCaptureState_Rejected;
    return Arcadia_BooleanValue_True;
  }
  Arcadia_Engine_UI_InputBinding binding = {
    .device = Arcadia_Engine_UI_InputBindingDevice_MouseButton,
    .code = (Arcadia_Natural32Value)event->button,
    .modifiers = modifierFlags(self->modifierSides),
    .bound = Arcadia_BooleanValue_True,
  };
  Arcadia_Engine_UI_InputBindingResult result = Arcadia_Engine_UI_InputBindingService_assign(thread, self, self->captureAction, binding);
  if (result == Arcadia_Engine_UI_InputBindingResult_Conflict) self->captureState = Arcadia_Engine_UI_InputCaptureState_Conflict;
  else if (result == Arcadia_Engine_UI_InputBindingResult_Rejected) self->captureState = Arcadia_Engine_UI_InputCaptureState_Rejected;
  else {
    Arcadia_Engine_UI_CanvasNode_suppressMouseButtonRelease(thread, self->captureCanvas, event->button);
    finishCapture(thread, self, Arcadia_Engine_UI_InputCaptureState_Committed);
  }
  return Arcadia_BooleanValue_True;
}

void
Arcadia_Engine_UI_InputBindingService_beginCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self,
    Arcadia_Engine_UI_CanvasNode* canvas,
    Arcadia_String* action
  )
{
  if (!canvas || indexOf(thread, self, action) == count(thread, self) || self->captureAction)
    invalidAction(thread);
  Arcadia_Engine_UI_CanvasNode_beginInputCapture(thread, canvas, (Arcadia_Object*)self, &onKeyboard, &onMouseButton);
  self->captureCanvas = canvas;
  self->captureAction = action;
  self->conflictAction = NULL;
  self->modifierSides = 0;
  self->captureState = Arcadia_Engine_UI_InputCaptureState_Listening;
}

void
Arcadia_Engine_UI_InputBindingService_cancelCapture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{
  if (self->captureAction) finishCapture(thread, self, Arcadia_Engine_UI_InputCaptureState_Cancelled);
}

Arcadia_Engine_UI_InputCaptureState
Arcadia_Engine_UI_InputBindingService_getCaptureState
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{ return self->captureState; }

Arcadia_String*
Arcadia_Engine_UI_InputBindingService_getConflictingAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{ return self->conflictAction; }

Arcadia_Signal*
Arcadia_Engine_UI_InputBindingService_getChangedSignal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_InputBindingService* self
  )
{ return self->changed; }
