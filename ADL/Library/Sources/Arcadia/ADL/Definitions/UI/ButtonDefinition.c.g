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
#include "Arcadia/ADL/Definitions/UI/ButtonDefinition.h"
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Definitions.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/UI/createOptionalColorReference.h"
#include "Arcadia/ADL/UI/requireLabelString.h"

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

static Arcadia_Natural8Value
pdlAsNatural8
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isNatural8Value(&v));
  return Arcadia_Value_getNatural8Value(&v);
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

static void
Arcadia_ADL_UI_ButtonDefinition_construct
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self,
    Arcadia_ADL_Definitions* p0,
    Arcadia_String* p1,
    Arcadia_Integer32Value p2,
    Arcadia_Integer32Value p3,
    Arcadia_Natural32Value p4,
    Arcadia_Natural32Value p5,
    Arcadia_String* p6,
    Arcadia_String* p7,
    Arcadia_String* p8,
    Arcadia_String* p9,
    Arcadia_String* p10,
    Arcadia_String* p11,
    Arcadia_String* p12,
    Arcadia_String* p13,
    Arcadia_Natural32Value p14,
    Arcadia_Natural32Value p15,
    Arcadia_Natural8Value p16,
    Arcadia_Natural8Value p17,
    Arcadia_Natural8Value p18
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
  for (Arcadia_SizeValue i = 0; i < 33; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 23, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  pdlPut(thread, base, 10, pdlBoxObjectReference(p6));
  pdlPut(thread, base, 11, pdlBoxObjectReference(p7));
  pdlPut(thread, base, 12, pdlBoxObjectReference(p8));
  pdlPut(thread, base, 13, pdlBoxObjectReference(p9));
  pdlPut(thread, base, 14, pdlBoxObjectReference(p10));
  pdlPut(thread, base, 15, pdlBoxObjectReference(p11));
  pdlPut(thread, base, 16, pdlBoxObjectReference(p12));
  pdlPut(thread, base, 17, pdlBoxObjectReference(p13));
  pdlPut(thread, base, 6, Arcadia_Value_makeInteger32Value(p2));
  pdlPut(thread, base, 7, Arcadia_Value_makeInteger32Value(p3));
  pdlPut(thread, base, 8, Arcadia_Value_makeNatural32Value(p4));
  pdlPut(thread, base, 9, Arcadia_Value_makeNatural32Value(p5));
  pdlPut(thread, base, 18, Arcadia_Value_makeNatural32Value(p14));
  pdlPut(thread, base, 19, Arcadia_Value_makeNatural32Value(p15));
  pdlPut(thread, base, 20, Arcadia_Value_makeNatural8Value(p16));
  pdlPut(thread, base, 21, Arcadia_Value_makeNatural8Value(p17));
  pdlPut(thread, base, 22, Arcadia_Value_makeNatural8Value(p18));
  (void)((Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_ADL_Definitions_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_String_getType(thread)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 6)));
  (void)((Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 7)));
  (void)((Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 8)));
  (void)((Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 9)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 10), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 11), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 12), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 13), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 14), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 15), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 16), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 17), _Arcadia_String_getType(thread)));
  (void)((Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 18)));
  (void)((Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 19)));
  (void)((Arcadia_Natural8Value)pdlAsNatural8(thread, pdlGet(thread, base, 20)));
  (void)((Arcadia_Natural8Value)pdlAsNatural8(thread, pdlGet(thread, base, 21)));
  (void)((Arcadia_Natural8Value)pdlAsNatural8(thread, pdlGet(thread, base, 22)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
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
    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _Arcadia_ADL_UI_ButtonDefinition_getType(thread));
  pdlPut(thread, base, 0, receiver);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 24, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->left = (Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 7));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->bottom = (Arcadia_Integer32Value)pdlAsInteger32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 8));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->width = (Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 9));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->height = (Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 10); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_requireLabelString(thread, (Arcadia_String*)pdlAsObjectReference(thread, args[0], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 25, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 25));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->text = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 11); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_requireLabelString(thread, (Arcadia_String*)pdlAsObjectReference(thread, args[0], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 26, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 26));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->horizontalAlignment = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 12); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_requireLabelString(thread, (Arcadia_String*)pdlAsObjectReference(thread, args[0], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 27, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 27));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->verticalAlignment = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 13); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_createOptionalColorReference(thread, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 28, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 28));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->backgroundColor = (Arcadia_ADL_Reference*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_ADL_Reference_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 14); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_createOptionalColorReference(thread, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 29, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 29));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->hoveredBackgroundColor = (Arcadia_ADL_Reference*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_ADL_Reference_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 15); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_createOptionalColorReference(thread, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 30, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 30));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->pressedBackgroundColor = (Arcadia_ADL_Reference*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_ADL_Reference_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 16); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_createOptionalColorReference(thread, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 31, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 31));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->disabledBackgroundColor = (Arcadia_ADL_Reference*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_ADL_Reference_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 17); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[3];
    args[0] = Arcadia_ValueStack_getValue(thread, 1);
    args[1] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_ADL_UI_createOptionalColorReference(thread, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 32, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 32));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->focusedBackgroundColor = (Arcadia_ADL_Reference*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_ADL_Reference_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 18));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->pixelSize = (Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 19));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->borderWidth = (Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 20));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->borderRed = (Arcadia_Natural8Value)pdlAsNatural8(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 21));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->borderGreen = (Arcadia_Natural8Value)pdlAsNatural8(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 23));
  pdlPut(thread, base, 2, pdlGet(thread, base, 22));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_ADL_UI_ButtonDefinition_getType(thread))) {
      ((Arcadia_ADL_UI_ButtonDefinition*)Arcadia_Value_getObjectReferenceValue(&receiver))->borderBlue = (Arcadia_Natural8Value)pdlAsNatural8(thread, pdlGet(thread, base, 2));
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
Arcadia_ADL_UI_ButtonDefinition_link_impl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
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
  extern void Arcadia_ADL_UI_ButtonDefinition_linkImpl(Arcadia_Thread* thread, Arcadia_ADL_UI_ButtonDefinition* self);
  Arcadia_ADL_UI_ButtonDefinition_linkImpl(thread, (Arcadia_ADL_UI_ButtonDefinition*)self);
  goto pdlReturn;
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

void
Arcadia_ADL_UI_ButtonDefinition_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
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
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_ADL_UI_ButtonDefinition_getType(thread)));
  Arcadia_ADL_DefinitionDispatch* dispatch = (Arcadia_ADL_DefinitionDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->link) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  dispatch->link(thread, (Arcadia_ADL_Definition*)self);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return;
}

static void
Arcadia_ADL_UI_ButtonDefinition_link_dispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{
  Arcadia_ADL_UI_ButtonDefinition_link_impl(thread, (Arcadia_ADL_UI_ButtonDefinition*)self);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_UI_ButtonDefinition);
  if (_numberOfArguments != 19) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->left = (Arcadia_Integer32Value){0};
  self->bottom = (Arcadia_Integer32Value){0};
  self->width = (Arcadia_Natural32Value){0};
  self->height = (Arcadia_Natural32Value){0};
  self->text = (Arcadia_String*){0};
  self->horizontalAlignment = (Arcadia_String*){0};
  self->verticalAlignment = (Arcadia_String*){0};
  self->backgroundColor = (Arcadia_ADL_Reference*){0};
  self->hoveredBackgroundColor = (Arcadia_ADL_Reference*){0};
  self->pressedBackgroundColor = (Arcadia_ADL_Reference*){0};
  self->disabledBackgroundColor = (Arcadia_ADL_Reference*){0};
  self->focusedBackgroundColor = (Arcadia_ADL_Reference*){0};
  self->pixelSize = (Arcadia_Natural32Value){0};
  self->borderWidth = (Arcadia_Natural32Value){0};
  self->borderRed = (Arcadia_Natural8Value){0};
  self->borderGreen = (Arcadia_Natural8Value){0};
  self->borderBlue = (Arcadia_Natural8Value){0};
  { Arcadia_Value args[20];
    args[0] = Arcadia_ValueStack_getValue(thread, 19);
    args[1] = Arcadia_ValueStack_getValue(thread, 18);
    args[2] = Arcadia_ValueStack_getValue(thread, 17);
    args[3] = Arcadia_ValueStack_getValue(thread, 16);
    args[4] = Arcadia_ValueStack_getValue(thread, 15);
    args[5] = Arcadia_ValueStack_getValue(thread, 14);
    args[6] = Arcadia_ValueStack_getValue(thread, 13);
    args[7] = Arcadia_ValueStack_getValue(thread, 12);
    args[8] = Arcadia_ValueStack_getValue(thread, 11);
    args[9] = Arcadia_ValueStack_getValue(thread, 10);
    args[10] = Arcadia_ValueStack_getValue(thread, 9);
    args[11] = Arcadia_ValueStack_getValue(thread, 8);
    args[12] = Arcadia_ValueStack_getValue(thread, 7);
    args[13] = Arcadia_ValueStack_getValue(thread, 6);
    args[14] = Arcadia_ValueStack_getValue(thread, 5);
    args[15] = Arcadia_ValueStack_getValue(thread, 4);
    args[16] = Arcadia_ValueStack_getValue(thread, 3);
    args[17] = Arcadia_ValueStack_getValue(thread, 2);
    args[18] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ADL_UI_ButtonDefinition_construct(thread, self, (Arcadia_ADL_Definitions*)pdlAsObjectReference(thread, args[0], _Arcadia_ADL_Definitions_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)), (Arcadia_Integer32Value)pdlAsInteger32(thread, args[2]), (Arcadia_Integer32Value)pdlAsInteger32(thread, args[3]), (Arcadia_Natural32Value)pdlAsNatural32(thread, args[4]), (Arcadia_Natural32Value)pdlAsNatural32(thread, args[5]), (Arcadia_String*)pdlAsObjectReference(thread, args[6], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[7], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[8], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[9], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[10], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[11], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[12], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[13], _Arcadia_String_getType(thread)), (Arcadia_Natural32Value)pdlAsNatural32(thread, args[14]), (Arcadia_Natural32Value)pdlAsNatural32(thread, args[15]), (Arcadia_Natural8Value)pdlAsNatural8(thread, args[16]), (Arcadia_Natural8Value)pdlAsNatural8(thread, args[17]), (Arcadia_Natural8Value)pdlAsNatural8(thread, args[18]));
  }
  Arcadia_LeaveConstructor(Arcadia_ADL_UI_ButtonDefinition);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinition* self
  )
{
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  if (self->horizontalAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->horizontalAlignment);
  if (self->verticalAlignment) Arcadia_Object_visit(thread, (Arcadia_Object*)self->verticalAlignment);
  if (self->backgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->backgroundColor);
  if (self->hoveredBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->hoveredBackgroundColor);
  if (self->pressedBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->pressedBackgroundColor);
  if (self->disabledBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->disabledBackgroundColor);
  if (self->focusedBackgroundColor) Arcadia_Object_visit(thread, (Arcadia_Object*)self->focusedBackgroundColor);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_ButtonDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = Arcadia_ADL_UI_ButtonDefinition_link_dispatchImpl;
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

Arcadia_defineObjectType(u8"Arcadia.ADL.UI.ButtonDefinition", Arcadia_ADL_UI_ButtonDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &typeOperations);

Arcadia_ADL_UI_ButtonDefinition*
Arcadia_ADL_UI_ButtonDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* p0,
    Arcadia_String* p1,
    Arcadia_Integer32Value p2,
    Arcadia_Integer32Value p3,
    Arcadia_Natural32Value p4,
    Arcadia_Natural32Value p5,
    Arcadia_String* p6,
    Arcadia_String* p7,
    Arcadia_String* p8,
    Arcadia_String* p9,
    Arcadia_String* p10,
    Arcadia_String* p11,
    Arcadia_String* p12,
    Arcadia_String* p13,
    Arcadia_Natural32Value p14,
    Arcadia_Natural32Value p15,
    Arcadia_Natural8Value p16,
    Arcadia_Natural8Value p17,
    Arcadia_Natural8Value p18
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
  { Arcadia_Value value = pdlBoxObjectReference(p6); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p7); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p8); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p9); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p10); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p11); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p12); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p13); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeInteger32Value(p2); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeInteger32Value(p3); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural32Value(p4); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural32Value(p5); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p6); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p7); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p8); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p9); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p10); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p11); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p12); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p13); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural32Value(p14); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural32Value(p15); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural8Value(p16); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural8Value(p17); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural8Value(p18); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 19);
  Arcadia_ADL_UI_ButtonDefinition* result = _Arcadia_EndCreate0(thread, _Arcadia_ADL_UI_ButtonDefinition_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
