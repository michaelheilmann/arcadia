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
#include "Arcadia/Engine/Audials/Providers/AudioStreamProvider.h"
#include "Arcadia/Media/Include.h"

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

static Arcadia_Natural32Value
pdlAsNatural32
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isNatural32Value(&v));
  return Arcadia_Value_getNatural32Value(&v);
}

Arcadia_BooleanValue
Arcadia_Engine_Audials_AudioStreamProvider_read
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self,
    Arcadia_Media_DSP_Buffer* p0,
    Arcadia_SizeValue* p1
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread)));
  Arcadia_Engine_Audials_AudioStreamProviderDispatch* dispatch = (Arcadia_Engine_Audials_AudioStreamProviderDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->read) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Media_DSP_Buffer_getType(thread)));
  Arcadia_BooleanValue result = dispatch->read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)self, p0, p1);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static Arcadia_Natural32Value
Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
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
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread))) {
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural32Value(((Arcadia_Engine_Audials_AudioStreamProvider*)Arcadia_Value_getObjectReferenceValue(&receiver))->sampleRate));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 5));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 0)));
  Arcadia_Natural32Value result = (Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 0));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Natural32Value
Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_AudioStreamProvider_getType(thread)));
  Arcadia_Natural32Value result = Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_AudioStreamProvider);
  if (_numberOfArguments != 0) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->sampleRate = (Arcadia_Natural32Value){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_AudioStreamProvider);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProvider* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_AudioStreamProviderDispatch* self
  )
{
  ((Arcadia_Engine_Audials_AudioStreamProviderDispatch*)self)->read = NULL;
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.AudioStreamProvider", Arcadia_Engine_Audials_AudioStreamProvider,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);
