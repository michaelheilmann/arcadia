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
#include "Arcadia/Engine/Visuals/CanvasActivationStateChangedEvent.h"
#include "Arcadia/Engine/Visuals/CanvasEvent.h"

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

static Arcadia_BooleanValue
pdlAsBoolean
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isBooleanValue(&v));
  return Arcadia_Value_getBooleanValue(&v);
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

static void
Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* self,
    Arcadia_Natural64Value p0,
    Arcadia_BooleanValue p1
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
  for (Arcadia_SizeValue i = 0; i < 8; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 6, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, Arcadia_Value_makeNatural64Value(p0));
  pdlPut(thread, base, 5, Arcadia_Value_makeBooleanValue(p1));
  (void)((Arcadia_Natural64Value)pdlAsNatural64(thread, pdlGet(thread, base, 4)));
  (void)((Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 5)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);
    Arcadia_ValueStack_pushValue(thread, &args[0]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Type_getOperations(_Arcadia_Engine_Visuals_CanvasEvent_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_getType(thread))) {
      ((Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->active = (Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 2));
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

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent);
  if (_numberOfArguments != 2) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->active = (Arcadia_BooleanValue){0};
  { Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 2);
    args[1] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_construct(thread, self, (Arcadia_Natural64Value)pdlAsNatural64(thread, args[0]), (Arcadia_BooleanValue)pdlAsBoolean(thread, args[1]));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_CanvasActivationStateChangedEventDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.CanvasActivationStateChangedEvent", Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent,
                         u8"Arcadia.Engine.Visuals.CanvasEvent", Arcadia_Engine_Visuals_CanvasEvent,
                         &typeOperations);

Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent*
Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value p0,
    Arcadia_BooleanValue p1
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
  { Arcadia_Value value = Arcadia_Value_makeBooleanValue(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent* result = _Arcadia_EndCreate0(thread, _Arcadia_Engine_Visuals_CanvasActivationStateChangedEvent_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
