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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/ADL/Definitions/DSP/SawtoothWaveDefinition.h"
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Definitions.h"

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

static void
Arcadia_ADL_SawtoothWaveDefinition_construct
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self,
    Arcadia_ADL_Definitions* p0,
    Arcadia_String* p1,
    Arcadia_Integer32Value p2
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
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  pdlPut(thread, base, 6, Arcadia_Value_makeInteger32Value(p2));
  (void)((Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_ADL_Definitions_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_String_getType(thread)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 6)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
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
    Arcadia_Type_getOperations(_Arcadia_ADL_Definition_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 7));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread))) {
      ((Arcadia_ADL_SawtoothWaveDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->frequency = (Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 2));
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
Arcadia_ADL_SawtoothWaveDefinition_link_impl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
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
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_ADL_SawtoothWaveDefinition_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread)));
  Arcadia_ADL_DefinitionDispatch* dispatch = (Arcadia_ADL_DefinitionDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->link) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->link(thread, (Arcadia_ADL_Definition*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_ADL_SawtoothWaveDefinition_link_dispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{
  Arcadia_ADL_SawtoothWaveDefinition_link_impl(thread, (Arcadia_ADL_SawtoothWaveDefinition*)self);
}

static Arcadia_Integer32Value
Arcadia_ADL_SawtoothWaveDefinition_getFrequency_impl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
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
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread))) {
  pdlPut(thread, base, 0, Arcadia_Value_makeInteger32Value(((Arcadia_ADL_SawtoothWaveDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->frequency));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 5));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 0)));
  Arcadia_Integer32Value result = (Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 0));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Integer32Value
Arcadia_ADL_SawtoothWaveDefinition_getFrequency
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread)));
  Arcadia_Integer32Value result = Arcadia_ADL_SawtoothWaveDefinition_getFrequency_impl(thread, self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_SawtoothWaveDefinition);
  if (_numberOfArguments != 3) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->frequency = (Arcadia_Integer32Value){0};
  { Arcadia_Value args[4];
    args[0] = Arcadia_ValueStack_getValue(thread, 3);
    args[1] = Arcadia_ValueStack_getValue(thread, 2);
    args[2] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ADL_SawtoothWaveDefinition_construct(thread, self, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)), (Arcadia_Integer32Value)pdlAsInteger32(thread, args[2]));
  }
  Arcadia_LeaveConstructor(Arcadia_ADL_SawtoothWaveDefinition);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = Arcadia_ADL_SawtoothWaveDefinition_link_dispatchImpl;
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

Arcadia_defineObjectType(u8"Arcadia.ADL.SawtoothWaveDefinition", Arcadia_ADL_SawtoothWaveDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &typeOperations);

Arcadia_ADL_SawtoothWaveDefinition*
Arcadia_ADL_SawtoothWaveDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* p0,
    Arcadia_String* p1,
    Arcadia_Integer32Value p2
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
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeInteger32Value(p2); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  Arcadia_ADL_SawtoothWaveDefinition* result = _Arcadia_EndCreate0(thread, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
