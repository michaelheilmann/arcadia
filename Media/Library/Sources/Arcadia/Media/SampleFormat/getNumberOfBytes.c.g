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
#include "Arcadia/Media/SampleFormat/getNumberOfBytes.h"
#include "Arcadia/Media/SampleFormat.h"
#include "Arcadia/Media/SampleFormat/checkedByteCount.h"

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

static Arcadia_SizeValue
pdlAsSize
  (
    Arcadia_Thread* t,
    Arcadia_Value v
  )
{
  pdlRequire(t, Arcadia_Value_isSizeValue(&v));
  return Arcadia_Value_getSizeValue(&v);
}

Arcadia_SizeValue
Arcadia_Media_SampleFormat_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_SampleFormat p0
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
  for (Arcadia_SizeValue i = 0; i < 8; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Media_SampleFormat_getType(thread), p0)));
  (void)((Arcadia_Media_SampleFormat)pdlAsEnumeration(thread, pdlGet(thread, base, 4), _Arcadia_Media_SampleFormat_getType(thread)));
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Media_SampleFormat_getType(thread), Arcadia_Media_SampleFormat_Natural8)));
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
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_oneByte;
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Media_SampleFormat_getType(thread), Arcadia_Media_SampleFormat_Integer8)));
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
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_oneByte;
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Media_SampleFormat_getType(thread), Arcadia_Media_SampleFormat_Natural16)));
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
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_twoBytes;
  pdlPut(thread, base, 1, pdlGet(thread, base, 4));
  pdlPut(thread, base, 2, Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Media_SampleFormat_getType(thread), Arcadia_Media_SampleFormat_Integer16)));
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
  pdlPut(thread, base, 5, pdlGet(thread, base, 0));
  pdlPut(thread, base, 1, pdlGet(thread, base, 5));
  if (pdlAsBoolean(thread, pdlGet(thread, base, 1))) goto pdlLabel_twoBytes;
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(UINT64_C(0)));
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  goto pdlLabel_finish;
pdlLabel_oneByte:;
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(UINT64_C(1)));
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
  goto pdlLabel_finish;
pdlLabel_twoBytes:;
  pdlPut(thread, base, 0, Arcadia_Value_makeNatural64Value(UINT64_C(2)));
  pdlPut(thread, base, 6, pdlGet(thread, base, 0));
pdlLabel_finish:;
  {
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    { Arcadia_Value v = pdlGet(thread, base, 6); Arcadia_ValueStack_pushValue(thread, &v); }
    Arcadia_Value args[2];
    args[0] = Arcadia_ValueStack_getValue(thread, 0);
  pdlPut(thread, base, 0, Arcadia_Value_makeSizeValue(Arcadia_Media_SampleFormat_checkedByteCount(thread, (Arcadia_Natural64Value)pdlAsNatural64(thread, args[0]))));
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);
  }
  pdlPut(thread, base, 7, pdlGet(thread, base, 0));
  pdlPut(thread, base, 0, pdlGet(thread, base, 7));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_SizeValue)pdlAsSize(thread, pdlGet(thread, base, 0)));
  Arcadia_SizeValue result = (Arcadia_SizeValue)pdlAsSize(thread, pdlGet(thread, base, 0));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

