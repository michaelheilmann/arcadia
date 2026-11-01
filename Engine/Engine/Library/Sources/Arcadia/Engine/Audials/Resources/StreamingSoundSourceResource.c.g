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
#include "Arcadia/Engine/Audials/Resources/StreamingSoundSourceResource.h"
#include "Arcadia/Engine/Audials/BackendContext.h"
#include "Arcadia/Engine/Audials/Resource.h"

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

static void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self,
    Arcadia_Engine_Audials_BackendContext* p0
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
  for (Arcadia_SizeValue i = 0; i < 7; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 5, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);
    Arcadia_ValueStack_pushValue(thread, &args[0]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Type_getOperations(_Arcadia_Engine_Audials_Implementation_Resource_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

Arcadia_BooleanValue
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread)));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* dispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->isPlaying) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_BooleanValue result = dispatch->isPlaying(thread, (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread)));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* dispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->play) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->play(thread, (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread)));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* dispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->pause) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->pause(thread, (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread)));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* dispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->stop) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->stop(thread, (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self,
    Arcadia_Real32Value p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread)));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* dispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->setVolume) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->setVolume(thread, (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

void
Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_getType(thread)));
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* dispatch = (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->update) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->update(thread, (Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource);
  if (_numberOfArguments != 1) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  { Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource_construct(thread, self, (Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch* self
  )
{
  ((Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self)->isPlaying = NULL;
  ((Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self)->play = NULL;
  ((Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self)->pause = NULL;
  ((Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self)->stop = NULL;
  ((Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self)->setVolume = NULL;
  ((Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResourceDispatch*)self)->update = NULL;
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.StreamingSoundSourceResource", Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource,
                         u8"Arcadia.Engine.Audials.Implementation.Resource", Arcadia_Engine_Audials_Implementation_Resource,
                         &typeOperations);
