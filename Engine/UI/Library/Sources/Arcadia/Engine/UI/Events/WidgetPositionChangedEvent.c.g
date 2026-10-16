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
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/UI/Events/WidgetPositionChangedEvent.h"
#include "Arcadia/Engine/UI/WidgetEvent.h"
#include "Arcadia/Engine/UI/WidgetNode.h"

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

static Arcadia_Object*
pdlAsObjectReference
  (
    Arcadia_Thread* thread,
    Arcadia_Value v,
    Arcadia_Type* type
  )
{
  if (Arcadia_Value_isVoidValue(&v)) return NULL;
  pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&v) && Arcadia_Value_getObjectReferenceValue(&v));
  return (Arcadia_Object*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, type);
}

static Arcadia_Integer32Value
pdlAsInteger32
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isInteger32Value(&v));
  return Arcadia_Value_getInteger32Value(&v);
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
Arcadia_Engine_UI_WidgetPositionChangedEvent_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self,
    Arcadia_Natural64Value p0,
    Arcadia_Engine_UI_WidgetNode* p1,
    Arcadia_Integer32Value p2,
    Arcadia_Integer32Value p3
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
  for (Arcadia_SizeValue i = 0; i < 10; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 8, pdlBoxObjectReference(self));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  pdlPut(thread, base, 4, Arcadia_Value_makeNatural64Value(p0));
  pdlPut(thread, base, 6, Arcadia_Value_makeInteger32Value(p2));
  pdlPut(thread, base, 7, Arcadia_Value_makeInteger32Value(p3));
  (void)((Arcadia_Natural64Value)pdlAsNatural64(thread, pdlGet(thread, base, 4)));
  (void)((Arcadia_Engine_UI_WidgetNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_Engine_UI_WidgetNode_getType(thread)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 6)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 7)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 8));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);
    Arcadia_ValueStack_pushValue(thread, &args[0]);
    Arcadia_ValueStack_pushValue(thread, &args[1]);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_Type_getOperations(_Arcadia_Engine_UI_WidgetEvent_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_Engine_UI_WidgetPositionChangedEvent_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 9, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 8));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_UI_WidgetPositionChangedEvent_getType(thread))) {
      ((Arcadia_Engine_UI_WidgetPositionChangedEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->horizontalPosition = (Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 8));
  pdlPut(thread, base, 2, pdlGet(thread, base, 7));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_UI_WidgetPositionChangedEvent_getType(thread))) {
      ((Arcadia_Engine_UI_WidgetPositionChangedEvent*)Arcadia_Value_getObjectReferenceValue(&receiver))->verticalPosition = (Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 2));
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
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_UI_WidgetPositionChangedEvent);
  if (_numberOfArguments != 4) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->horizontalPosition = (Arcadia_Integer32Value){0};
  self->verticalPosition = (Arcadia_Integer32Value){0};
  { Arcadia_Value args[5];
    args[0] = Arcadia_ValueStack_getValue(thread, 4);
    args[1] = Arcadia_ValueStack_getValue(thread, 3);
    args[2] = Arcadia_ValueStack_getValue(thread, 2);
    args[3] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_UI_WidgetPositionChangedEvent_construct(thread, self, (Arcadia_Natural64Value)pdlAsNatural64(thread, args[0]), (Arcadia_Engine_UI_WidgetNode*)pdlAsObjectReference(thread, args[1], _Arcadia_Engine_UI_WidgetNode_getType(thread)), (Arcadia_Integer32Value)pdlAsInteger32(thread, args[2]), (Arcadia_Integer32Value)pdlAsInteger32(thread, args[3]));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_UI_WidgetPositionChangedEvent);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEvent* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_UI_WidgetPositionChangedEventDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Engine.UI.WidgetPositionChangedEvent", Arcadia_Engine_UI_WidgetPositionChangedEvent,
                         u8"Arcadia.Engine.UI.WidgetEvent", Arcadia_Engine_UI_WidgetEvent,
                         &typeOperations);

Arcadia_Engine_UI_WidgetPositionChangedEvent*
Arcadia_Engine_UI_WidgetPositionChangedEvent_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value p0,
    Arcadia_Engine_UI_WidgetNode* p1,
    Arcadia_Integer32Value p2,
    Arcadia_Integer32Value p3
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
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = Arcadia_Value_makeNatural64Value(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeInteger32Value(p2); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeInteger32Value(p3); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  Arcadia_Engine_UI_WidgetPositionChangedEvent* result = _Arcadia_EndCreate0(thread, _Arcadia_Engine_UI_WidgetPositionChangedEvent_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
