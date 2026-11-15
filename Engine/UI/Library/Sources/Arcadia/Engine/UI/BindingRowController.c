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
#include "Arcadia/Engine/UI/BindingRowController.h"
#include "Arcadia/Engine/UI/StatusLabel.h"
#include "Arcadia/Engine/UI/Include.h"

static void
constructImpl(Arcadia_Thread* thread, Arcadia_Engine_UI_BindingRowController* self)
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_BindingRowController);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->service = NULL;
  self->canvas = NULL;
  self->action = NULL;
  self->edit = NULL;
  self->reset = NULL;
  self->valueLabel = NULL;
  self->statusLabel = NULL;
  self->connections = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->enabled = Arcadia_BooleanValue_True;
  self->captureSeen = Arcadia_BooleanValue_False;
  self->presentationValid = Arcadia_BooleanValue_False;
  self->previousCaptureSeen = Arcadia_BooleanValue_False;
  self->previousBinding = (Arcadia_Engine_UI_InputBinding){0};
  self->previousState = Arcadia_Engine_UI_InputCaptureState_Idle;
  self->defaultBinding = (Arcadia_Engine_UI_InputBinding){0};
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_BindingRowController);
}

static void
visitImpl(Arcadia_Thread* thread, Arcadia_Engine_UI_BindingRowController* self)
{
  if (self->service) Arcadia_Object_visit(thread, (Arcadia_Object*)self->service);
  if (self->canvas) Arcadia_Object_visit(thread, (Arcadia_Object*)self->canvas);
  if (self->action) Arcadia_Object_visit(thread, (Arcadia_Object*)self->action);
  if (self->edit) Arcadia_Object_visit(thread, (Arcadia_Object*)self->edit);
  if (self->reset) Arcadia_Object_visit(thread, (Arcadia_Object*)self->reset);
  if (self->valueLabel) Arcadia_Object_visit(thread, (Arcadia_Object*)self->valueLabel);
  if (self->statusLabel) Arcadia_Object_visit(thread, (Arcadia_Object*)self->statusLabel);
  if (self->connections) Arcadia_Object_visit(thread, (Arcadia_Object*)self->connections);
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};
static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations,
};
Arcadia_defineObjectType(u8"Arcadia.Engine.UI.BindingRowController", Arcadia_Engine_UI_BindingRowController,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations)

static Arcadia_String*
formatBinding(Arcadia_Thread* thread, Arcadia_Engine_UI_InputBinding binding)
{
  if (!binding.bound) return Arcadia_String_createFromCxxString(thread, u8"Unbound");
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)builder);
  if (binding.modifiers & Arcadia_Engine_UI_InputBindingModifier_Control)
    Arcadia_StringBuilder_insertBackCxxString(thread, builder, "Ctrl+");
  if (binding.modifiers & Arcadia_Engine_UI_InputBindingModifier_Shift)
    Arcadia_StringBuilder_insertBackCxxString(thread, builder, "Shift+");
  if (binding.modifiers & Arcadia_Engine_UI_InputBindingModifier_Alt)
    Arcadia_StringBuilder_insertBackCxxString(thread, builder, "Alt+");
  if (binding.device == Arcadia_Engine_UI_InputBindingDevice_MouseButton) {
    Arcadia_StringBuilder_insertBackCxxString(thread, builder, "Mouse ");
    Arcadia_StringBuilder_insertBackCxxInt(thread, builder, (int)binding.code);
  } else {
    Arcadia_String* key = Arcadia_Engine_Input_KeyboardKey_toString(thread,
      (Arcadia_Engine_Input_KeyboardKey)binding.code);
    Arcadia_StringBuilder_insertBackString(thread, builder, key);
  }
  Arcadia_String* result = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(builder));
  Arcadia_ValueStack_popValues(thread, 1);
  return result;
}

void
Arcadia_Engine_UI_BindingRowController_refresh
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_BindingRowController* self
  )
{
  Arcadia_Engine_UI_InputBinding binding = Arcadia_Engine_UI_InputBindingService_get(thread, self->service, self->action);
  Arcadia_Engine_UI_InputCaptureState state = Arcadia_Engine_UI_InputBindingService_getCaptureState(thread, self->service);
  Arcadia_Value actionValue = Arcadia_Value_makeObjectReferenceValue(self->action);
  if (self->service->captureAction &&
      !Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)self->service->captureAction, &actionValue))
    self->captureSeen = Arcadia_BooleanValue_False;
  if (self->presentationValid && self->previousState == state &&
      self->previousCaptureSeen == self->captureSeen &&
      self->previousBinding.bound == binding.bound &&
      (!binding.bound || (self->previousBinding.device == binding.device &&
       self->previousBinding.code == binding.code && self->previousBinding.modifiers == binding.modifiers))) return;
  self->presentationValid = Arcadia_BooleanValue_True;
  self->previousState = state;
  self->previousCaptureSeen = self->captureSeen;
  self->previousBinding = binding;
  Arcadia_String* formatted = formatBinding(thread, binding);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)formatted);
  Arcadia_Engine_UI_LabelNode_setText(thread, self->valueLabel, formatted);
  Arcadia_ValueStack_popValues(thread, 1);
  if (!self->enabled) {
    Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel,
      Arcadia_Engine_UI_StatusSeverity_Informational, Arcadia_String_createFromCxxString(thread, u8"Disabled"));
    return;
  }
  if (!self->captureSeen) {
    Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel,
      Arcadia_Engine_UI_StatusSeverity_Informational, Arcadia_String_createFromCxxString(thread, u8""));
    return;
  }
  switch (state) {
    case Arcadia_Engine_UI_InputCaptureState_Listening:
      Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel, Arcadia_Engine_UI_StatusSeverity_Informational,
        Arcadia_String_createFromCxxString(thread, u8"Listening: Esc to cancel")); break;
    case Arcadia_Engine_UI_InputCaptureState_Conflict:
      Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel, Arcadia_Engine_UI_StatusSeverity_Warning,
        Arcadia_String_createFromCxxString(thread, u8"Conflict: choose another")); break;
    case Arcadia_Engine_UI_InputCaptureState_Rejected:
      Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel, Arcadia_Engine_UI_StatusSeverity_Error,
        Arcadia_String_createFromCxxString(thread, u8"Invalid key: retry")); break;
    case Arcadia_Engine_UI_InputCaptureState_Committed:
      Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel, Arcadia_Engine_UI_StatusSeverity_Success,
        Arcadia_String_createFromCxxString(thread, u8"Saved (session only)")); break;
    case Arcadia_Engine_UI_InputCaptureState_Cancelled:
      Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel, Arcadia_Engine_UI_StatusSeverity_Informational,
        Arcadia_String_createFromCxxString(thread, u8"Cancelled")); break;
    default:
      Arcadia_Engine_UI_StatusLabel_set(thread, self->statusLabel, Arcadia_Engine_UI_StatusSeverity_Informational,
        Arcadia_String_createFromCxxString(thread, u8"")); break;
  }
}

void
Arcadia_Engine_UI_BindingRowController_reset(Arcadia_Thread* thread, Arcadia_Engine_UI_BindingRowController* self)
{
  if (!self->enabled || self->service->captureAction) return;
  self->captureSeen = Arcadia_BooleanValue_False;
  Arcadia_Engine_UI_InputBindingService_assign(thread, self->service, self->action, self->defaultBinding);
  Arcadia_Engine_UI_BindingRowController_refresh(thread, self);
}

void
Arcadia_Engine_UI_BindingRowController_setEnabled(Arcadia_Thread* thread,
  Arcadia_Engine_UI_BindingRowController* self, Arcadia_BooleanValue enabled)
{
  if (!enabled && self->service->captureAction &&
      Arcadia_String_isEqualTo_pn(thread, self->service->captureAction,
        (char const*)Arcadia_String_getBytes(thread, self->action), Arcadia_String_getNumberOfBytes(thread, self->action))) {
    Arcadia_Engine_UI_InputBindingService_cancelCapture(thread, self->service);
  }
  self->enabled = enabled;
  self->presentationValid = Arcadia_BooleanValue_False;
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, self->edit, enabled);
  Arcadia_Engine_UI_ButtonNode_setEnabled(thread, self->reset, enabled);
  Arcadia_Engine_UI_BindingRowController_refresh(thread, self);
}

static void
onEdit(Arcadia_Thread* thread)
{
  Arcadia_Engine_UI_BindingRowController* self = (Arcadia_Engine_UI_BindingRowController*)
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_BindingRowController_getType(thread));
  if (!self->enabled || self->service->captureAction) return;
  Arcadia_Engine_UI_InputBindingService_beginCapture(thread, self->service, self->canvas, self->action);
  self->captureSeen = Arcadia_BooleanValue_True;
  Arcadia_Engine_UI_BindingRowController_refresh(thread, self);
}

static void
onReset(Arcadia_Thread* thread)
{
  Arcadia_Engine_UI_BindingRowController* self = (Arcadia_Engine_UI_BindingRowController*)
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_UI_BindingRowController_getType(thread));
  Arcadia_Engine_UI_BindingRowController_reset(thread, self);
}

static Arcadia_Engine_UI_BindingRowController*
createEmpty(Arcadia_Thread* thread)
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_BindingRowController);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_BindingRowController);
}

Arcadia_Engine_UI_BindingRowController*
Arcadia_Engine_UI_BindingRowController_create
  (
    Arcadia_Thread* thread, Arcadia_Engine_UI_InputBindingService* service,
    Arcadia_Engine_UI_CanvasNode* canvas, Arcadia_String* action,
    Arcadia_Engine_UI_ButtonNode* edit, Arcadia_Engine_UI_ButtonNode* reset,
    Arcadia_Engine_UI_LabelNode* valueLabel, Arcadia_Engine_UI_LabelNode* statusLabel,
    Arcadia_Engine_UI_InputBinding defaultBinding
  )
{
  if (!service || !canvas || !action || !edit || !reset || edit == reset || !valueLabel || !statusLabel) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Do not connect callbacks until action and default are verified.
  Arcadia_Engine_UI_InputBindingService_get(thread, service, action);
  Arcadia_Engine_UI_BindingRowController* self = createEmpty(thread);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
  self->service = service; self->canvas = canvas; self->action = action;
  self->edit = edit; self->reset = reset;
  self->valueLabel = valueLabel; self->statusLabel = statusLabel;
  self->defaultBinding = defaultBinding;
  Arcadia_Slot* a = Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, edit),
                                          (Arcadia_Object*)self, &onEdit);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)a);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->connections, (Arcadia_Object*)a);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Slot* b = Arcadia_Signal_connect(thread, Arcadia_Engine_UI_ButtonNode_getActivatedSignal(thread, reset),
                                          (Arcadia_Object*)self, &onReset);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)b);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->connections, (Arcadia_Object*)b);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_UI_BindingRowController_refresh(thread, self);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return self;
}
