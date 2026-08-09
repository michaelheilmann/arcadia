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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_THREADEXTENSIONS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_THREADEXTENSIONS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Thread.h"

#include "Arcadia/Ring1/Implementation/Atoms.h"
#include "Arcadia/Ring1/Implementation/BigInteger/Include.h"
#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Enumeration.h"
#include "Arcadia/Ring1/Implementation/ForeignProcedure.h"
#include "Arcadia/Ring1/Implementation/ImmutableByteArray.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"
#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"
#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"
#include "Arcadia/Ring1/Implementation/Object.h"
#include "Arcadia/Ring1/Implementation/Real32.h"
#include "Arcadia/Ring1/Implementation/Real64.h"
#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/TypeSystem/Include.h"
#include "Arcadia/Ring1/Implementation/Void.h"
#include "Arcadia/Ring1/Implementation/Value.h"

#define Define(Type, Suffix) \
static inline Type \
Arcadia_ValueStack_get##Suffix \
  ( \
    Arcadia_Thread* thread, \
    Arcadia_SizeValue index \
  ) \
{ \
  Arcadia_Value value = Arcadia_ValueStack_getValue(thread, index); \
  if (!Arcadia_Value_is##Suffix(&value)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  return Arcadia_Value_get##Suffix(&value); \
} \
\
static inline void \
Arcadia_ValueStack_push##Suffix \
  ( \
    Arcadia_Thread* thread, \
    Type value \
  ) \
{ \
  Arcadia_Value temporary = Arcadia_Value_make##Suffix(value); \
  Arcadia_ValueStack_pushValue(thread, &temporary); \
} \
\
static inline Arcadia_BooleanValue \
Arcadia_ValueStack_is##Suffix \
  ( \
    Arcadia_Thread* thread, \
    Arcadia_SizeValue index \
  ) \
{ \
  Arcadia_Value value = Arcadia_ValueStack_getValue(thread, index); \
  return Arcadia_Value_is##Suffix(&value); \
}

Define(Arcadia_AtomValue, AtomValue)
Define(Arcadia_BigIntegerValue, BigIntegerValue)
Define(Arcadia_BooleanValue, BooleanValue)
Define(Arcadia_EnumerationValue, EnumerationValue)
Define(Arcadia_ForeignProcedureValue, ForeignProcedureValue)
Define(Arcadia_RuntimeByteArrayValue, RuntimeByteArrayValue)
Define(Arcadia_RuntimeUTF8StringValue, RuntimeUTF8StringValue)
Define(Arcadia_Integer16Value, Integer16Value)
Define(Arcadia_Integer32Value, Integer32Value)
Define(Arcadia_Integer64Value, Integer64Value)
Define(Arcadia_Integer8Value, Integer8Value)
Define(Arcadia_Natural16Value, Natural16Value)
Define(Arcadia_Natural32Value, Natural32Value)
Define(Arcadia_Natural64Value, Natural64Value)
Define(Arcadia_Natural8Value, Natural8Value)
Define(Arcadia_Real32Value, Real32Value)
Define(Arcadia_Real64Value, Real64Value)
Define(Arcadia_ObjectReferenceValue, ObjectReferenceValue)
Define(Arcadia_SizeValue, SizeValue)
Define(Arcadia_TypeValue, TypeValue)
Define(Arcadia_VoidValue, VoidValue)

#undef Define

static inline Arcadia_ObjectReferenceValue
Arcadia_ValueStack_getObjectReferenceValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue index,
    Arcadia_Type* type
  )
{
  Arcadia_ObjectReferenceValue objectReferenceValue = Arcadia_ValueStack_getObjectReferenceValue(thread, index);
  if (type) {
    if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, objectReferenceValue), type)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  return objectReferenceValue;
}

#endif // ARCADIA_RING1_IMPLEMENTATION_THREADEXTENSIONS_H_INCLUDED
