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
#include "Arcadia/Engine/UI/NumericEditState.h"

#include <math.h>

static void
invalid
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

static Arcadia_BooleanValue
equal
  (
    Arcadia_Value a,
    Arcadia_Value b
  )
{
  if (Arcadia_Value_isInteger64Value(&a) && Arcadia_Value_isInteger64Value(&b)) {
    return Arcadia_Value_getInteger64Value(&a) == Arcadia_Value_getInteger64Value(&b);
  }
  if (Arcadia_Value_isReal64Value(&a) && Arcadia_Value_isReal64Value(&b)) {
    return Arcadia_Value_getReal64Value(&a) == Arcadia_Value_getReal64Value(&b);
  }
  return false;
}

static Arcadia_BooleanValue
acceptable
  (
    Arcadia_Value value,
    Arcadia_Value minimum,
    Arcadia_Value maximum
  )
{
  if (Arcadia_Value_isInteger64Value(&value)) {
    Arcadia_Integer64Value x = Arcadia_Value_getInteger64Value(&value);
    return (Arcadia_Value_isVoidValue(&minimum) ||
            (Arcadia_Value_isInteger64Value(&minimum) && x >= Arcadia_Value_getInteger64Value(&minimum))) &&
           (Arcadia_Value_isVoidValue(&maximum) ||
            (Arcadia_Value_isInteger64Value(&maximum) && x <= Arcadia_Value_getInteger64Value(&maximum)));
  }
  if (Arcadia_Value_isReal64Value(&value)) {
    Arcadia_Real64Value x = Arcadia_Value_getReal64Value(&value);
    return isfinite(x) &&
           (Arcadia_Value_isVoidValue(&minimum) ||
            (Arcadia_Value_isReal64Value(&minimum) && isfinite(Arcadia_Value_getReal64Value(&minimum)) && x >= Arcadia_Value_getReal64Value(&minimum))) &&
           (Arcadia_Value_isVoidValue(&maximum) ||
            (Arcadia_Value_isReal64Value(&maximum) && isfinite(Arcadia_Value_getReal64Value(&maximum)) && x <= Arcadia_Value_getReal64Value(&maximum)));
  }
  return false;
}

static Arcadia_String*
format
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  if (Arcadia_Value_isInteger64Value(&value)) {
    return Arcadia_String_createFromInteger64(thread, Arcadia_Value_getInteger64Value(&value));
  }
  return Arcadia_String_createFromReal64(thread, Arcadia_Value_getReal64Value(&value));
}

static Arcadia_BooleanValue
parse
  (
    Arcadia_Thread* thread,
    Arcadia_String* text,
    Arcadia_BooleanValue integer,
    Arcadia_Value* result
  )
{
  Arcadia_Natural8Value const* p = Arcadia_String_getBytes(thread, text);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, text);
  Arcadia_NumberLiteral literal;
  if (!n || !Arcadia_parseNumberLiteral(thread, &literal, p, n) ||
      (!literal.significand.integral.length && !literal.significand.fractional.length) ||
      (integer && (literal.significand.radixPoint.length || literal.exponent.length)) ||
      (literal.exponent.length && p[literal.exponent.powerSymbol.start] == '^')) {
    return false;
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    if (integer) {
      *result = Arcadia_Value_makeInteger64Value(Arcadia_String_toInteger64(thread, text));
    } else {
      *result = Arcadia_Value_makeReal64Value(Arcadia_String_toReal64(thread, text));
    }
    Arcadia_Thread_popJumpTarget(thread);
    return true;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    if (Arcadia_Thread_getStatus(thread) == Arcadia_Status_ConversionFailed) {
      Arcadia_Thread_setStatus(thread, 0);
      return false;
    }
    Arcadia_Thread_jump(thread);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_NumericEditState);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->editor = NULL;
  self->value = Arcadia_Value_makeInteger64Value(0);
  self->minimum = self->maximum = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
    self->editor = Arcadia_Engine_UI_TextEditState_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self->editor);
    Arcadia_Engine_UI_NumericEditState_setValue(thread, self, self->value);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_NumericEditState);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  )
{
  if (self->editor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->editor);
  }
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_TypeDescriptor_Operations typeOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.NumericEditState", Arcadia_Engine_UI_NumericEditState,
                         u8"Arcadia.Object", Arcadia_Object, &typeOperations)

Arcadia_Engine_UI_NumericEditState*
Arcadia_Engine_UI_NumericEditState_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_UI_NumericEditState);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_UI_NumericEditState);
}

Arcadia_Engine_UI_TextEditState*
Arcadia_Engine_UI_NumericEditState_getEditor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  )
{
  return self->editor;
}

Arcadia_Value
Arcadia_Engine_UI_NumericEditState_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  )
{
  return self->value;
}

void
Arcadia_Engine_UI_NumericEditState_setValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self,
    Arcadia_Value value
  )
{
  if (!acceptable(value, self->minimum, self->maximum)) {
    invalid(thread);
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_String* text = format(thread, value);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
    Arcadia_Engine_UI_TextEditState_setText(thread, self->editor, text);
    self->value = value;
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Engine_UI_NumericEditState_setBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self,
    Arcadia_Value minimum,
    Arcadia_Value maximum
  )
{
  if (!acceptable(self->value, minimum, maximum)) {
    invalid(thread);
  }
  self->minimum = minimum;
  self->maximum = maximum;
}

Arcadia_Engine_UI_NumericEditStateResult
Arcadia_Engine_UI_NumericEditState_commit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_String* draft = Arcadia_Engine_UI_TextEditState_getText(thread, self->editor);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)draft);
    Arcadia_Value candidate;
    if (!parse(thread, draft, Arcadia_Value_isInteger64Value(&self->value), &candidate) ||
        !acceptable(candidate, self->minimum, self->maximum)) {
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
      Arcadia_Thread_popJumpTarget(thread);
      return Arcadia_Engine_UI_NumericEditStateResult_Rejected;
    }
    Arcadia_BooleanValue changed = !equal(candidate, self->value);
    Arcadia_Engine_UI_NumericEditState_setValue(thread, self, candidate);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_popJumpTarget(thread);
    return changed ? Arcadia_Engine_UI_NumericEditStateResult_Changed : Arcadia_Engine_UI_NumericEditStateResult_Unchanged;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_BooleanValue
Arcadia_Engine_UI_NumericEditState_cancel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_NumericEditState* self
  )
{
  return Arcadia_Engine_UI_TextEditState_cancel(thread, self->editor);
}
