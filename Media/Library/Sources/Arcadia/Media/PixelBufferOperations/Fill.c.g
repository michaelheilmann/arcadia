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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Media/PixelBufferOperations/Fill.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/Media/PixelBufferOperation.h"
#include "Arcadia/Media/PixelBufferOperations/requireDefinition.h"

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
Arcadia_Media_PixelBufferOperations_Fill_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* p0
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
  (void)((Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread)));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Media_PixelBufferOperations_requireDefinition(thread, (Arcadia_ADL_Definition*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definition_getType(thread)));
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  pdlPut(thread, base, 2, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Media_PixelBufferOperations_Fill_getType(thread))) {
      ((Arcadia_Media_PixelBufferOperations_Fill*)Arcadia_Value_getObjectReferenceValue(&receiver))->definition = (Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread));
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
Arcadia_Media_PixelBufferOperations_Fill_apply_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_Media_PixelBuffer* p0
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
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  (void)((Arcadia_Media_PixelBuffer*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Media_PixelBuffer_getType(thread)));
  extern void Arcadia_Media_PixelBufferOperations_Fill_applyImpl(Arcadia_Thread* thread, Arcadia_Media_PixelBufferOperations_Fill* self, Arcadia_Media_PixelBuffer* p0);
  Arcadia_Media_PixelBufferOperations_Fill_applyImpl(thread, (Arcadia_Media_PixelBufferOperations_Fill*)self, (Arcadia_Media_PixelBuffer*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Media_PixelBuffer_getType(thread)));
  goto pdlReturn;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_Media_PixelBufferOperations_Fill_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_Media_PixelBuffer* p0
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Media_PixelBufferOperations_Fill_getType(thread)));
  Arcadia_Media_PixelBufferOperationDispatch* dispatch = (Arcadia_Media_PixelBufferOperationDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->apply) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Media_PixelBuffer_getType(thread)));
  dispatch->apply(thread, (Arcadia_Media_PixelBufferOperation*)self, p0);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_Media_PixelBufferOperations_Fill_apply_dispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperation* self,
    Arcadia_Media_PixelBuffer* p0
  )
{
  Arcadia_Media_PixelBufferOperations_Fill_apply_impl(thread, (Arcadia_Media_PixelBufferOperations_Fill*)self, p0);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_PixelBufferOperations_Fill);
  if (_numberOfArguments != 1) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->definition = (Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Media_PixelBufferOperations_Fill_construct(thread, self, (Arcadia_ADL_PixelBufferOperations_FillOperationDefinition*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread)));
  }
  Arcadia_LeaveConstructor(Arcadia_Media_PixelBufferOperations_Fill);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self
  )
{
  if (self->definition) Arcadia_Object_visit(thread, (Arcadia_Object*)self->definition);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_FillDispatch* self
  )
{
  ((Arcadia_Media_PixelBufferOperationDispatch*)self)->apply = Arcadia_Media_PixelBufferOperations_Fill_apply_dispatchImpl;
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

Arcadia_defineObjectType(u8"Arcadia.Media.PixelBufferOperations.Fill", Arcadia_Media_PixelBufferOperations_Fill,
                         u8"Arcadia.Media.PixelBufferOperation", Arcadia_Media_PixelBufferOperation,
                         &typeOperations);

Arcadia_Media_PixelBufferOperations_Fill*
Arcadia_Media_PixelBufferOperations_Fill_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* p0
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
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_Media_PixelBufferOperations_Fill* result = _Arcadia_EndCreate0(thread, _Arcadia_Media_PixelBufferOperations_Fill_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
