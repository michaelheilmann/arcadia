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

#if !defined (ARCADIA_RING2_IMPLEMENTATION_ARGUMENTSVALIDATION_H_INCLUDED)
#define ARCADIA_RING2_IMPLEMENTATION_ARGUMENTSVALIDATION_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

static inline Arcadia_ObjectReferenceValue
Arcadia_ArgumentsValidation_getObjectReferenceValue
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value,
    Arcadia_TypeValue type
  )
{
  if (!Arcadia_Value_isObjectReferenceValue(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ObjectReferenceValue objectReferenceValue = Arcadia_Value_getObjectReferenceValue(value);
  if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, objectReferenceValue), type)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return objectReferenceValue;
}

static inline Arcadia_ObjectReferenceValue
Arcadia_ArgumentsValidation_getObjectReferenceValueOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value,
    Arcadia_TypeValue type
  )
{
  if (Arcadia_Value_isVoidValue(value)) {
    return NULL;
  }
  if (!Arcadia_Value_isObjectReferenceValue(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ObjectReferenceValue objectReferenceValue = Arcadia_Value_getObjectReferenceValue(value);
  if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, objectReferenceValue), type)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return objectReferenceValue;
}

static inline Arcadia_BooleanValue
Arcadia_ArgumentsValidation_getBooleanValue
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isBooleanValue(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_BooleanValue booleanValue = Arcadia_Value_getBooleanValue(value);
  return booleanValue;
}

static inline Arcadia_Integer32Value
Arcadia_ArgumentsValidation_getInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isInteger32Value(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getInteger32Value(value);
}

static inline Arcadia_Integer64Value
Arcadia_ArgumentsValidation_getInteger64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isInteger64Value(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getInteger64Value(value);
}

static inline Arcadia_Natural64Value
Arcadia_ArgumentsValidation_getNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isNatural16Value(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getNatural16Value(value);
}

static inline Arcadia_Natural64Value
Arcadia_ArgumentsValidation_getNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isNatural32Value(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getNatural32Value(value);
}

static inline Arcadia_Natural64Value
Arcadia_ArgumentsValidation_getNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isNatural64Value(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getNatural64Value(value);
}

static inline Arcadia_RuntimeUTF8StringValue
Arcadia_ArgumentsValidation_getRuntimeUTF8StringValue
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (!Arcadia_Value_isRuntimeUTF8StringValue(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getRuntimeUTF8StringValue(value);
}

static inline Arcadia_RuntimeUTF8StringValue
Arcadia_ArgumentsValidation_getRuntimeUTF8StringValueOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* value
  )
{
  if (Arcadia_Value_isVoidValue(value)) {
    return NULL;
  }
  if (!Arcadia_Value_isRuntimeUTF8StringValue(value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_Value_getRuntimeUTF8StringValue(value);
}

#endif // ARCADIA_RING2_IMPLEMENTATION_ARGUMENTSVALIDATION_H_INCLUDED
