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

#define ARCADIA_ENGINE_TEXT_PRIVATE (1)
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Text/MarkupTextRun.h"
#include "Arcadia/Engine/Text/TextAlignment.h"
#include "Arcadia/Engine/Text/requireRunStrings.h"

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
Arcadia_Engine_Text_MarkupTextRun_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_MarkupTextRun* self,
    Arcadia_String* p0,
    Arcadia_String* p1,
    Arcadia_String* p2,
    Arcadia_Natural32Value p3,
    Arcadia_String* p4,
    Arcadia_Engine_Text_TextAlignment p5,
    Arcadia_BooleanValue p6
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
  pdlPut(thread, base, 11, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  pdlPut(thread, base, 6, pdlBoxObjectReference(p2));
  pdlPut(thread, base, 8, pdlBoxObjectReference(p4));
  pdlPut(thread, base, 7, Arcadia_Value_makeNatural32Value(p3));
  pdlPut(thread, base, 9, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Text_TextAlignment_getType(thread), p5)));
  pdlPut(thread, base, 10, Arcadia_Value_makeBooleanValue(p6));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_String_getType(thread)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 6), _Arcadia_String_getType(thread)));
  (void)((Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 7)));
  (void)((Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 8), _Arcadia_String_getType(thread)));
  (void)((Arcadia_Engine_Text_TextAlignment)pdlAsEnumeration(thread, pdlGet(thread, base, 9), _Arcadia_Engine_Text_TextAlignment_getType(thread)));
  (void)((Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 10)));
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 4); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 5); Arcadia_ValueStack_pushValue(thread, &v); }
    { Arcadia_Value v = pdlGet(thread, base, 8); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[4];
    args[0] = Arcadia_ValueStack_getValue(thread, 2);
    args[1] = Arcadia_ValueStack_getValue(thread, 1);
    args[2] = Arcadia_ValueStack_getValue(thread, 0);
    Arcadia_Engine_Text_requireRunStrings(thread, (Arcadia_String*)pdlAsObjectReference(thread, args[0], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[2], _Arcadia_String_getType(thread)));
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 12, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 4));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->text = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 5));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->glyphSource = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 6));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->dynamicToken = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 7));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->pixelSize = (Arcadia_Natural32Value)pdlAsNatural32(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 8));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->colorName = (Arcadia_String*)pdlAsObjectReference(thread, pdlGet(thread, base, 2), _Arcadia_String_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 9));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->alignment = (Arcadia_Engine_Text_TextAlignment)pdlAsEnumeration(thread, pdlGet(thread, base, 2), _Arcadia_Engine_Text_TextAlignment_getType(thread));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_False));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->hasRectangle = (Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_False));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->lineBreakBefore = (Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 2));
    }
    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }
  }
  pdlPut(thread, base, 1, pdlGet(thread, base, 11));
  pdlPut(thread, base, 2, pdlGet(thread, base, 10));
  { Arcadia_Value receiver = pdlGet(thread, base, 1);
    if (Arcadia_Value_isInstanceOf(thread, &receiver, _Arcadia_Engine_Text_MarkupTextRun_getType(thread))) {
      ((Arcadia_Engine_Text_MarkupTextRun*)Arcadia_Value_getObjectReferenceValue(&receiver))->fixedPlacement = (Arcadia_BooleanValue)pdlAsBoolean(thread, pdlGet(thread, base, 2));
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
    Arcadia_Engine_Text_MarkupTextRun* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Text_MarkupTextRun);
  if (_numberOfArguments != 7) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->text = (Arcadia_String*){0};
  self->glyphSource = (Arcadia_String*){0};
  self->dynamicToken = (Arcadia_String*){0};
  self->pixelSize = (Arcadia_Natural32Value){0};
  self->colorName = (Arcadia_String*){0};
  self->alignment = (Arcadia_Engine_Text_TextAlignment){0};
  self->hasRectangle = (Arcadia_BooleanValue){0};
  self->rectangleLeft = (Arcadia_Integer32Value){0};
  self->rectangleBottom = (Arcadia_Integer32Value){0};
  self->rectangleWidth = (Arcadia_Integer32Value){0};
  self->rectangleHeight = (Arcadia_Integer32Value){0};
  self->lineBreakBefore = (Arcadia_BooleanValue){0};
  self->fixedPlacement = (Arcadia_BooleanValue){0};
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[8];
    args[0] = Arcadia_ValueStack_getValue(thread, 7);
    args[1] = Arcadia_ValueStack_getValue(thread, 6);
    args[2] = Arcadia_ValueStack_getValue(thread, 5);
    args[3] = Arcadia_ValueStack_getValue(thread, 4);
    args[4] = Arcadia_ValueStack_getValue(thread, 3);
    args[5] = Arcadia_ValueStack_getValue(thread, 2);
    if (Arcadia_Value_isInteger32Value(&args[5])) args[5] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Text_TextAlignment_getType(thread), Arcadia_Value_getInteger32Value(&args[5])));
    args[6] = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_Engine_Text_MarkupTextRun_construct(thread, self, (Arcadia_String*)pdlAsObjectReference(thread, args[0], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[1], _Arcadia_String_getType(thread)), (Arcadia_String*)pdlAsObjectReference(thread, args[2], _Arcadia_String_getType(thread)), (Arcadia_Natural32Value)pdlAsNatural32(thread, args[3]), (Arcadia_String*)pdlAsObjectReference(thread, args[4], _Arcadia_String_getType(thread)), (Arcadia_Engine_Text_TextAlignment)pdlAsEnumeration(thread, args[5], _Arcadia_Engine_Text_TextAlignment_getType(thread)), (Arcadia_BooleanValue)pdlAsBoolean(thread, args[6]));
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Text_MarkupTextRun);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_MarkupTextRun* self
  )
{
  if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text);
  if (self->glyphSource) Arcadia_Object_visit(thread, (Arcadia_Object*)self->glyphSource);
  if (self->dynamicToken) Arcadia_Object_visit(thread, (Arcadia_Object*)self->dynamicToken);
  if (self->colorName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->colorName);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Text_MarkupTextRunDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.Engine.Text.MarkupTextRun", Arcadia_Engine_Text_MarkupTextRun,
                         u8"Arcadia.Object", Arcadia_Object,
                         &typeOperations);

Arcadia_Engine_Text_MarkupTextRun*
Arcadia_Engine_Text_MarkupTextRun_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* p0,
    Arcadia_String* p1,
    Arcadia_String* p2,
    Arcadia_Natural32Value p3,
    Arcadia_String* p4,
    Arcadia_Engine_Text_TextAlignment p5,
    Arcadia_BooleanValue p6
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
  { Arcadia_Value value = pdlBoxObjectReference(p2); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p4); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p2); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeNatural32Value(p3); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p4); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Text_TextAlignment_getType(thread), p5)); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = Arcadia_Value_makeBooleanValue(p6); Arcadia_ValueStack_pushValue(thread, &value); }
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  Arcadia_Engine_Text_MarkupTextRun* result = _Arcadia_EndCreate0(thread, _Arcadia_Engine_Text_MarkupTextRun_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
