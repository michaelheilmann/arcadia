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
#include "Arcadia/Engine/Visuals/Resources/MaterialResource.h"
#include "Arcadia/Engine/Visuals/BackendContext.h"
#include "Arcadia/Engine/Visuals/BlendFunction.h"
#include "Arcadia/Engine/Visuals/ColorSource.h"
#include "Arcadia/Engine/Visuals/Resource.h"

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

static void
Arcadia_Engine_Visuals_MaterialResource_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialResource* self,
    Arcadia_Engine_Visuals_BackendContext* p0,
    Arcadia_BooleanValue p1,
    Arcadia_Engine_Visuals_BlendFunction p2,
    Arcadia_Engine_Visuals_BlendFunction p3,
    Arcadia_Engine_Visuals_ColorSource p4
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
  for (Arcadia_SizeValue i = 0; i < 11; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 9, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, Arcadia_Value_makeBooleanValue(p1));
  pdlPut(thread, base, 6, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), p2)));
  pdlPut(thread, base, 7, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), p3)));
  pdlPut(thread, base, 8, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_ColorSource_getType(thread), p4)));
  (void)((Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  (void)((Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 5)));
  (void)((Arcadia_Engine_Visuals_BlendFunction)pdlAsEnumeration(thread, pdlGet(thread, base, 6), _Arcadia_Engine_Visuals_BlendFunction_getType(thread)));
  (void)((Arcadia_Engine_Visuals_BlendFunction)pdlAsEnumeration(thread, pdlGet(thread, base, 7), _Arcadia_Engine_Visuals_BlendFunction_getType(thread)));
  (void)((Arcadia_Engine_Visuals_ColorSource)pdlAsEnumeration(thread, pdlGet(thread, base, 8), _Arcadia_Engine_Visuals_ColorSource_getType(thread)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Value receiver = pdlGet(thread, base, 1);
    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);
    Arcadia_ValueStack_pushValue(thread, &args[0]);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_Type_getOperations(_Arcadia_Engine_Visuals_Resource_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_Engine_Visuals_MaterialResource_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 10, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Visuals_MaterialResource_getType(thread))) {
      ((Arcadia_Engine_Visuals_MaterialResource*)Arcadia_Value_getObjectReferenceValue(&receiver))->blendEnabled = (Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Visuals_MaterialResource_getType(thread))) {
      ((Arcadia_Engine_Visuals_MaterialResource*)Arcadia_Value_getObjectReferenceValue(&receiver))->blendSourceFunction = (Arcadia_Engine_Visuals_BlendFunction)pdlAsEnumeration(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Visuals_BlendFunction_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  pdlPut(thread, base, 2, pdlGet(thread, base, 7));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Visuals_MaterialResource_getType(thread))) {
      ((Arcadia_Engine_Visuals_MaterialResource*)Arcadia_Value_getObjectReferenceValue(&receiver))->blendDestinationFunction = (Arcadia_Engine_Visuals_BlendFunction)pdlAsEnumeration(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Visuals_BlendFunction_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  pdlPut(thread, base, 2, pdlGet(thread, base, 8));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Visuals_MaterialResource_getType(thread))) {
      ((Arcadia_Engine_Visuals_MaterialResource*)Arcadia_Value_getObjectReferenceValue(&receiver))->ambientColorSource = (Arcadia_Engine_Visuals_ColorSource)pdlAsEnumeration(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Visuals_ColorSource_getType(thread));
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
    Arcadia_Engine_Visuals_MaterialResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_MaterialResource);
  if (_numberOfArguments != 5) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->blendEnabled = (Arcadia_BooleanValue){0};
  self->blendSourceFunction = (Arcadia_Engine_Visuals_BlendFunction){0};
  self->blendDestinationFunction = (Arcadia_Engine_Visuals_BlendFunction){0};
  self->ambientColorSource = (Arcadia_Engine_Visuals_ColorSource){0};
  { Arcadia_Value args[6];
    args[0] = Arcadia_ValueStack_getValue(thread, 5);
    args[1] = Arcadia_ValueStack_getValue(thread, 4);
    args[2] = Arcadia_ValueStack_getValue(thread, 3);
    if (Arcadia_Value_isInteger32Value(&args[2])) args[2] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), Arcadia_Value_getInteger32Value(&args[2])));
    args[3] = Arcadia_ValueStack_getValue(thread, 2);
    if (Arcadia_Value_isInteger32Value(&args[3])) args[3] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), Arcadia_Value_getInteger32Value(&args[3])));
    args[4] = Arcadia_ValueStack_getValue(thread, 1);
    if (Arcadia_Value_isInteger32Value(&args[4])) args[4] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_ColorSource_getType(thread), Arcadia_Value_getInteger32Value(&args[4])));
    Arcadia_Engine_Visuals_MaterialResource_construct(thread, self, (Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_Visuals_BackendContext_getType(thread)), (Arcadia_BooleanValue)pdlAsBoolean(thread, args[1]), (Arcadia_Engine_Visuals_BlendFunction)pdlAsEnumeration(thread, args[2], _Arcadia_Engine_Visuals_BlendFunction_getType(thread)), (Arcadia_Engine_Visuals_BlendFunction)pdlAsEnumeration(thread, args[3], _Arcadia_Engine_Visuals_BlendFunction_getType(thread)), (Arcadia_Engine_Visuals_ColorSource)pdlAsEnumeration(thread, args[4], _Arcadia_Engine_Visuals_ColorSource_getType(thread)));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_MaterialResource);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialResource* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_MaterialResourceDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.MaterialResource", Arcadia_Engine_Visuals_MaterialResource,
                         u8"Arcadia.Engine.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &typeOperations);
