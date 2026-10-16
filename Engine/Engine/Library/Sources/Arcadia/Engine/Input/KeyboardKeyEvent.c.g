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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Input/KeyboardKeyEvent.h"
#include "Arcadia/Engine/Event.h"
#include "Arcadia/Engine/Input/KeyboardKey.h"
#include "Arcadia/Engine/Input/KeyboardKeyAction.h"
_Static_assert(sizeof(Arcadia_Engine_Input_KeyboardKey) <= sizeof(Arcadia_Integer32Value), "native enum value ABI must fit Integer32");

static Arcadia_Value
pdlGet
  (
    Arcadia_Thread* t,
    Arcadia_SizeValue b,
    Arcadia_SizeValue i
  )
{
  return Arcadia_ValueStack_getValue(t, Arcadia_ValueStack_getSize(t)-b-i-1);
}

static void
pdlPut
  (
    Arcadia_Thread* t,
    Arcadia_SizeValue b,
    Arcadia_SizeValue i,
    Arcadia_Value v
  )
{
  Arcadia_ValueStack_setValue(t, Arcadia_ValueStack_getSize(t)-b-i-1, &v);
}

static void
pdlRequire
  (
    Arcadia_Thread* t,
    Arcadia_BooleanValue b
  )
{
  if (!b) { Arcadia_Thread_setStatus(t, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(t); }
}

static Arcadia_Value
pdlBoxObjectReference
  (
    Arcadia_ObjectReferenceValue object
  )
{
  return object ? Arcadia_Value_makeObjectReferenceValue(object) : Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}

static Arcadia_Value
pdlBoxNativeEnumeration
  (
    Arcadia_Thread* thread,
    Arcadia_Type* type,
    Arcadia_Integer64Value value
  )
{
  pdlRequire(thread, value >= Arcadia_Integer32Value_Minimum && value <= Arcadia_Integer32Value_Maximum);
  return Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, (Arcadia_Integer32Value)value));
}

static Arcadia_Integer32Value
pdlAsEnumeration
  (
    Arcadia_Thread* t,
    Arcadia_Value v,
    Arcadia_Type* type
  )
{
  pdlRequire(t, Arcadia_Value_isEnumerationValue(&v) && Arcadia_Value_getEnumerationValue(&v).type == type);
  return Arcadia_Value_getEnumerationValue(&v).value;
}

static Arcadia_Natural64Value
pdlAsNatural64
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isNatural64Value(&v));
  return Arcadia_Value_getNatural64Value(&v);
}

static Arcadia_Engine_Input_KeyboardKey
pdlAsNativeEnumeration_Arcadia_Engine_Input_KeyboardKey
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  Arcadia_Integer32Value numeric = pdlAsEnumeration(thread, value, _Arcadia_Engine_Input_KeyboardKey_getType(thread));
  Arcadia_Engine_Input_KeyboardKey result = (Arcadia_Engine_Input_KeyboardKey)numeric;
  pdlRequire(thread, (Arcadia_Integer64Value)result == (Arcadia_Integer64Value)numeric);
  return result;
}

static void
Arcadia_Engine_Input_KeyboardKeyEvent_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self,
    Arcadia_Natural64Value p0,
    Arcadia_Engine_Input_KeyboardKeyAction p1,
    Arcadia_Engine_Input_KeyboardKey p2
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 9; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 7, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, Arcadia_Value_makeNatural64Value(p0));
  pdlPut(thread, base, 5, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Input_KeyboardKeyAction_getType(thread), p1)));
  pdlPut(thread, base, 6, pdlBoxNativeEnumeration(thread, _Arcadia_Engine_Input_KeyboardKey_getType(thread), p2));
  (void)((Arcadia_Natural64Value)pdlAsNatural64(thread, pdlGet(thread, base, 4)));
  (void)((Arcadia_Engine_Input_KeyboardKeyAction)pdlAsEnumeration(thread, pdlGet(thread, base, 5), _Arcadia_Engine_Input_KeyboardKeyAction_getType(thread)));
  (void)(pdlAsNativeEnumeration_Arcadia_Engine_Input_KeyboardKey(thread, pdlGet(thread, base, 6)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);
    Arcadia_ValueStack_pushValue(thread, &args[0]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Type_getOperations(_Arcadia_Engine_Event_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread))) {
      ((Arcadia_Engine_Input_KeyboardKeyEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->action = (Arcadia_Engine_Input_KeyboardKeyAction)pdlAsEnumeration(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Input_KeyboardKeyAction_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread))) {
      ((Arcadia_Engine_Input_KeyboardKeyEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->key = pdlAsNativeEnumeration_Arcadia_Engine_Input_KeyboardKey(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

static Arcadia_Engine_Input_KeyboardKeyAction
Arcadia_Engine_Input_KeyboardKeyEvent_getAction_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 6; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread))) {
  pdlPut(thread, base, 0, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Input_KeyboardKeyAction_getType(thread), ((Arcadia_Engine_Input_KeyboardKeyEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->action)));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 5));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Engine_Input_KeyboardKeyAction)pdlAsEnumeration(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Input_KeyboardKeyAction_getType(thread)));
  Arcadia_Engine_Input_KeyboardKeyAction result = (Arcadia_Engine_Input_KeyboardKeyAction)pdlAsEnumeration(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Input_KeyboardKeyAction_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Input_KeyboardKeyAction
Arcadia_Engine_Input_KeyboardKeyEvent_getAction
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread)));
  Arcadia_Engine_Input_KeyboardKeyAction result = Arcadia_Engine_Input_KeyboardKeyEvent_getAction_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static Arcadia_Engine_Input_KeyboardKey
Arcadia_Engine_Input_KeyboardKeyEvent_getKey_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 6; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxNativeEnumeration(thread, _Arcadia_Engine_Input_KeyboardKey_getType(thread), ((Arcadia_Engine_Input_KeyboardKeyEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->key));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 5));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)(pdlAsNativeEnumeration_Arcadia_Engine_Input_KeyboardKey(thread, pdlGet(thread, base, 0)));
  Arcadia_Engine_Input_KeyboardKey result = pdlAsNativeEnumeration_Arcadia_Engine_Input_KeyboardKey(thread, pdlGet(thread, base, 0));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Input_KeyboardKey
Arcadia_Engine_Input_KeyboardKeyEvent_getKey
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread)));
  Arcadia_Engine_Input_KeyboardKey result = Arcadia_Engine_Input_KeyboardKeyEvent_getKey_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Input_KeyboardKeyEvent);
  if (_numberOfArguments != 3) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->key = (Arcadia_Engine_Input_KeyboardKey){0};
  self->action = (Arcadia_Engine_Input_KeyboardKeyAction){0};
  { Arcadia_Value args[4];
    args[0] = Arcadia_ValueStack_getValue(thread, 3);
    args[1] = Arcadia_ValueStack_getValue(thread, 2);
    if (Arcadia_Value_isInteger32Value(&args[1])) args[1] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Input_KeyboardKeyAction_getType(thread), Arcadia_Value_getInteger32Value(&args[1])));
    args[2] = Arcadia_ValueStack_getValue(thread, 1);
    if (Arcadia_Value_isInteger32Value(&args[2])) args[2] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Input_KeyboardKey_getType(thread), Arcadia_Value_getInteger32Value(&args[2])));
    Arcadia_Engine_Input_KeyboardKeyEvent_construct(thread, self, (Arcadia_Natural64Value)pdlAsNatural64(thread, args[0]), (Arcadia_Engine_Input_KeyboardKeyAction)pdlAsEnumeration(thread, args[1], _Arcadia_Engine_Input_KeyboardKeyAction_getType(thread)), pdlAsNativeEnumeration_Arcadia_Engine_Input_KeyboardKey(thread, args[2]));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Input_KeyboardKeyEvent);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEvent* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Input_KeyboardKeyEventDispatch* self
  )
{
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Input.KeyboardKeyEvent", Arcadia_Engine_Input_KeyboardKeyEvent,
                         u8"Arcadia.Engine.Event", Arcadia_Engine_Event,
                         &typeOperations);

Arcadia_Engine_Input_KeyboardKeyEvent*
Arcadia_Engine_Input_KeyboardKeyEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value p0,
    Arcadia_Engine_Input_KeyboardKeyAction p1,
    Arcadia_Engine_Input_KeyboardKey p2
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = Arcadia_Value_makeNatural64Value(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Input_KeyboardKeyAction_getType(thread), p1)); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxNativeEnumeration(thread, _Arcadia_Engine_Input_KeyboardKey_getType(thread), p2); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  Arcadia_Engine_Input_KeyboardKeyEvent* result = _Arcadia_EndCreate0(thread, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
