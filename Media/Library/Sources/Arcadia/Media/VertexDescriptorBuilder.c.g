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
#include "Arcadia/Media/VertexDescriptorBuilder.h"
#include "Arcadia/Collections/Include.h"
#include "Arcadia/Media/VertexDescriptor.h"
#include "Arcadia/Media/createVertexDescriptor.h"
#include "Arcadia/Media/createVertexElements.h"
#include "Arcadia/Media/vertexElementCount.h"
#include "Arcadia/Media/vertexElementEndAt.h"

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
Arcadia_Media_VertexDescriptorBuilder_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
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
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Media_createVertexElements(thread)));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Media_VertexDescriptorBuilder_getType(thread))) {
      ((Arcadia_Media_VertexDescriptorBuilder*)Arcadia_Value_getObjectReferenceValue(&receiver))->vertexElementDescriptors = (Arcadia_List*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_List_getType(thread));
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

static Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexDescriptorBuilder_build_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
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
  for (Arcadia_SizeValue i = 0; i < 13; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Media_VertexDescriptorBuilder_getType(thread))) {
  pdlPut(thread, base, 0, pdlBoxObjectReference(((Arcadia_Media_VertexDescriptorBuilder*)Arcadia_Value_getObjectReferenceValue(&receiver))->vertexElementDescriptors));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(Arcadia_Media_vertexElementCount(thread, (Arcadia_List*)pdlAsObjectReference(thread, args[0], _Arcadia_List_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(UINT64_C(0)));
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(UINT64_C(0)));
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
pdlLabel_nextElement:;
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value a = pdlGet(thread, base, 1);
    Arcadia_ForeignProcedure* op = Arcadia_Type_getOperations(Arcadia_Value_getType(thread, &a))->isEqualTo;
    if (!op && Arcadia_Value_isEnumerationValue(&a)) {
      Arcadia_Value b = pdlGet(thread, base, 2);
      Arcadia_BooleanValue equal = Arcadia_Value_isEnumerationValue(&b) && Arcadia_Value_isEqualTo(thread, &a, &b);
  pdlPut(thread, base, 0, Arcadia_Value_makeBooleanValue(equal));
    } else {
    if (!op) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    { Arcadia_Value v = pdlGet(thread, base, 1); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 2); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    op(thread);
  pdlPut(thread, base, 0, Arcadia_ValueStack_getValue(thread, 0));
    Arcadia_ValueStack_popValues(thread, 1);
    }
  }
  pdlPut(thread, base, 9, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_finish;
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 7); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(Arcadia_Media_vertexElementEndAt(thread, (Arcadia_List*)pdlAsObjectReference(thread, args[0], _Arcadia_List_getType(thread)), (Arcadia_Natural64Value)pdlAsNatural64(thread, args[1]))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 10, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 10));
  pdlPut(thread, base, 2, pdlGet(thread, base, 8));
  { Arcadia_Value a = pdlGet(thread, base, 1);
    Arcadia_ForeignProcedure* op = Arcadia_Type_getOperations(Arcadia_Value_getType(thread, &a))->isGreaterThan;
    if (!op) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    { Arcadia_Value v = pdlGet(thread, base, 1); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 2); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    op(thread);
  pdlPut(thread, base, 0, Arcadia_ValueStack_getValue(thread, 0));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  pdlPut(thread, base, 11, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  if (!pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_advance;
  pdlPut(thread, base, 0, pdlGet(thread, base, 10));
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
pdlLabel_advance:;
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  pdlPut(thread, base, 2, Arcadia_Value_makeNatural64Value(UINT64_C(1)));
  { Arcadia_Value a = pdlGet(thread, base, 1);
    Arcadia_ForeignProcedure* op = Arcadia_Type_getOperations(Arcadia_Value_getType(thread, &a))->add;
    if (!op) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
    { Arcadia_Value v = pdlGet(thread, base, 1); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 2); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    op(thread);
  pdlPut(thread, base, 0, Arcadia_ValueStack_getValue(thread, 0));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  goto pdlLabel_nextElement;
pdlLabel_finish:;
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 8); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Media_createVertexDescriptor(thread, (Arcadia_List*)pdlAsObjectReference(thread, args[0], _Arcadia_List_getType(thread)), (Arcadia_Natural64Value)pdlAsNatural64(thread, args[1]))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 12, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 12));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Media_VertexDescriptor*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Media_VertexDescriptor_getType(thread)));
  Arcadia_Media_VertexDescriptor* result = (Arcadia_Media_VertexDescriptor*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Media_VertexDescriptor_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexDescriptorBuilder_build
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Media_VertexDescriptorBuilder_getType(thread)));
  Arcadia_Media_VertexDescriptor* result = Arcadia_Media_VertexDescriptorBuilder_build_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_VertexDescriptorBuilder);
  if (_numberOfArguments != 0) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->flags = (Arcadia_Natural8Value){0};
  self->stride = (Arcadia_SizeValue){0};
  self->vertexElementDescriptors = (Arcadia_List*){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[1];
    Arcadia_Media_VertexDescriptorBuilder_construct(thread, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Media_VertexDescriptorBuilder);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  )
{
  if (self->vertexElementDescriptors) Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexElementDescriptors);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilderDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Media.VertexDescriptorBuilder", Arcadia_Media_VertexDescriptorBuilder,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);

Arcadia_Media_VertexDescriptorBuilder*
Arcadia_Media_VertexDescriptorBuilder_create
  (
    Arcadia_Thread* thread
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
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Media_VertexDescriptorBuilder* result = _Arcadia_EndCreate0(thread, _Arcadia_Media_VertexDescriptorBuilder_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
