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
#include "Arcadia/Engine/UI/instantiateDefinitionChild.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/Include.h"

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

Arcadia_Object*
Arcadia_Engine_UI_instantiateDefinitionChild
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* p0,
    Arcadia_ADL_Definition* p1
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
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  (void)((Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
  (void)((Arcadia_ADL_Definition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_Definition_getType(thread)));
  extern Arcadia_Object* Arcadia_Engine_UI_createNodeFromDefinition(Arcadia_Thread* thread, Arcadia_Engine_Visuals_BackendContext* p0, Arcadia_ADL_Definition* p1);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_UI_createNodeFromDefinition(thread, (Arcadia_Engine_Visuals_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Visuals_BackendContext_getType(thread)), (Arcadia_ADL_Definition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_Definition_getType(thread)))));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Object*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Object_getType(thread)));
  Arcadia_Object* result = (Arcadia_Object*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Object_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

