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
#include "Arcadia/Engine/UI/populateContainerFromDefinition.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/UI/appendDefinitionChild.h"
#include "Arcadia/Engine/UI/containerDefinitionChildren.h"
#include "Arcadia/Engine/UI/definitionChildCount.h"
#include "Arcadia/Engine/UI/instantiateDefinitionChild.h"
#include "Arcadia/Engine/UI/resolveDefinitionChild.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Collections/Include.h"

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

void
Arcadia_Engine_UI_populateContainerFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* p0,
    Arcadia_Object* p1,
    Arcadia_Object* p2
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
  for (Arcadia_SizeValue i = 0; i < 14; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  pdlPut(thread, base, 6, pdlBoxObjectReference(p2));
  (void)((Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  (void)((Arcadia_Object*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_Object_getType(thread)));
  (void)((Arcadia_Object*)pdlAsObjectReference(thread, pdlGet(thread, base, 6), _Arcadia_Object_getType(thread)));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 6); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_UI_containerDefinitionChildren(thread, (Arcadia_Object*)pdlAsObjectReference(thread, args[0], _Arcadia_Object_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 7); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(Arcadia_Engine_UI_definitionChildCount(thread, (Arcadia_List*)pdlAsObjectReference(thread, args[0], _Arcadia_List_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 8, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(UINT64_C(0)));
  pdlPut(thread, base, 9, pdlGet(thread, base, 0));
pdlLabel_nextChild:;
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
  pdlPut(thread, base, 2, pdlGet(thread, base, 8));
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
  pdlPut(thread, base, 10, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 10));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_done;
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 7); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 9); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_UI_resolveDefinitionChild(thread, (Arcadia_List*)pdlAsObjectReference(thread, args[0], _Arcadia_List_getType(thread)), (Arcadia_Natural64Value)pdlAsNatural64(thread, args[1]))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 11, pdlGet(thread, base, 0));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 11); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_UI_instantiateDefinitionChild(thread, (Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, args[0], _Arcadia_Engine_Visuals_BackendContext_getType(thread)), (Arcadia_ADL_Definition*)pdlAsObjectReference(thread, args[1], _Arcadia_ADL_Definition_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 12, pdlGet(thread, base, 0));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 12); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Engine_UI_appendDefinitionChild(thread, (Arcadia_Object*)pdlAsObjectReference(thread, args[0], _Arcadia_Object_getType(thread)), (Arcadia_Object*)pdlAsObjectReference(thread, args[1], _Arcadia_Object_getType(thread)));
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 13, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 9));
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
  pdlPut(thread, base, 9, pdlGet(thread, base, 0));
  goto pdlLabel_nextChild;
pdlLabel_done:;
  goto pdlReturn;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

