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
#include "Arcadia/Engine/Engine.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia/Engine/BackendContext.h"
#include "Arcadia/Engine/Event.h"
#include "Arcadia/Engine/NodeFactory.h"
#include "Arcadia/Engine/createEventQueue.h"
#include "Arcadia/Engine/createTypeSet.h"

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

static Arcadia_BooleanValue
Arcadia_Collection_isEmpty_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
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
  for (Arcadia_SizeValue i = 0; i < 5; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  extern Arcadia_BooleanValue Arcadia_Collection_isEmpty(Arcadia_Thread* thread, Arcadia_Collection* self);
  pdlPut(thread, base, 0, Arcadia_Value_makeBooleanValue(Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)self)));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 0)));
  Arcadia_BooleanValue result = (Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 0));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

static Arcadia_Value
Arcadia_Deque_getFront_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
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
  for (Arcadia_SizeValue i = 0; i < 5; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  extern Arcadia_Value Arcadia_Deque_getFront(Arcadia_Thread* thread, Arcadia_Deque* self);
  pdlPut(thread, base, 0, Arcadia_Deque_getFront(thread, (Arcadia_Deque*)self));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)(pdlGet(thread, base, 0));
  Arcadia_Value result = pdlGet(thread, base, 0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

static void
Arcadia_Deque_insertBack_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_Value p0
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
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, p0);
  (void)(pdlGet(thread, base, 4));
  extern void Arcadia_Deque_insertBack(Arcadia_Thread* thread, Arcadia_Deque* self, Arcadia_Value p0);
  Arcadia_Deque_insertBack(thread, (Arcadia_Deque*)self, pdlGet(thread, base, 4));
  goto pdlReturn;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

static void
Arcadia_Deque_removeFront_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
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
  for (Arcadia_SizeValue i = 0; i < 5; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  extern void Arcadia_Deque_removeFront(Arcadia_Thread* thread, Arcadia_Deque* self);
  Arcadia_Deque_removeFront(thread, (Arcadia_Deque*)self);
  goto pdlReturn;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

static void
Arcadia_Engine_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
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
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createTypeSet(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->visualsBackendTypes = (Arcadia_Set*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Set_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createTypeSet(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->visualsNodeFactoryTypes = (Arcadia_Set*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Set_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->visualsBackendContext = (Arcadia_Engine_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_BackendContext_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->visualsNodeFactory = (Arcadia_Engine_NodeFactory*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_NodeFactory_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createTypeSet(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->audialsBackendTypes = (Arcadia_Set*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Set_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createTypeSet(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->audialsNodeFactoryTypes = (Arcadia_Set*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Set_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->audialsBackendContext = (Arcadia_Engine_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_BackendContext_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->audialsNodeFactory = (Arcadia_Engine_NodeFactory*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Engine_NodeFactory_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_createEventQueue(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
      ((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->events = (Arcadia_Deque*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_Deque_getType(thread));
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
Arcadia_Engine_enqueEvent_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self,
    Arcadia_Engine_Event* p0
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
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine_Event*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Event_getType(thread)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxObjectReference(((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->events));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Deque_getType(thread))) {
      Arcadia_Deque_insertBack_impl(thread, (Arcadia_Deque*)Arcadia_Value_getObjectReferenceValue(&receiver), args[0]);
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Engine_enqueEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self,
    Arcadia_Engine_Event* p0
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
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_getType(thread)));
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Event_getType(thread)));
  Arcadia_Engine_enqueEvent_impl(thread, self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static Arcadia_Engine_Event*
Arcadia_Engine_dequeEvent_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
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
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxObjectReference(((Arcadia_Engine*)Arcadia_Value_getObjectReferenceValue(&receiver))->events));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Collection_getType(thread))) {
  pdlPut(thread, base, 0, Arcadia_Value_makeBooleanValue(Arcadia_Collection_isEmpty_impl(thread, (Arcadia_Collection*)Arcadia_Value_getObjectReferenceValue(&receiver))));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 6));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_empty;
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Deque_getType(thread))) {
  pdlPut(thread, base, 0, Arcadia_Deque_getFront_impl(thread, (Arcadia_Deque*)Arcadia_Value_getObjectReferenceValue(&receiver)));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_Value args[1];
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Deque_getType(thread))) {
      Arcadia_Deque_removeFront_impl(thread, (Arcadia_Deque*)Arcadia_Value_getObjectReferenceValue(&receiver));
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 7));
  goto pdlReturn;
pdlLabel_empty:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Engine_Event*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Event_getType(thread)));
  Arcadia_Engine_Event* result = (Arcadia_Engine_Event*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Event_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Event*
Arcadia_Engine_dequeEvent
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_getType(thread)));
  Arcadia_Engine_Event* result = Arcadia_Engine_dequeEvent_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

void
Arcadia_Engine_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_getType(thread)));
  Arcadia_EngineDispatch* dispatch = (Arcadia_EngineDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->update) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->update(thread, (Arcadia_Engine*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine);
  if (_numberOfArguments != 0) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->visualsBackendTypes = (Arcadia_Set*){0};
  self->visualsNodeFactoryTypes = (Arcadia_Set*){0};
  self->audialsBackendTypes = (Arcadia_Set*){0};
  self->audialsNodeFactoryTypes = (Arcadia_Set*){0};
  self->visualsBackendContext = (Arcadia_Engine_BackendContext*){0};
  self->visualsNodeFactory = (Arcadia_Engine_NodeFactory*){0};
  self->audialsBackendContext = (Arcadia_Engine_BackendContext*){0};
  self->audialsNodeFactory = (Arcadia_Engine_NodeFactory*){0};
  self->events = (Arcadia_Deque*){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[1];
    Arcadia_Engine_construct(thread, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* self
  )
{
  if (self->visualsBackendTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsBackendTypes);
  if (self->visualsNodeFactoryTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsNodeFactoryTypes);
  if (self->audialsBackendTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsBackendTypes);
  if (self->audialsNodeFactoryTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsNodeFactoryTypes);
  if (self->visualsBackendContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsBackendContext);
  if (self->visualsNodeFactory) Arcadia_Object_visit(thread, (Arcadia_Object*)self->visualsNodeFactory);
  if (self->audialsBackendContext) Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsBackendContext);
  if (self->audialsNodeFactory) Arcadia_Object_visit(thread, (Arcadia_Object*)self->audialsNodeFactory);
  if (self->events) Arcadia_Object_visit(thread, (Arcadia_Object*)self->events);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_EngineDispatch* self
  )
{
  ((Arcadia_EngineDispatch*)self)->update = NULL;
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

Arcadia_defineObjectType(u8"Arcadia.Engine", Arcadia_Engine,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);
