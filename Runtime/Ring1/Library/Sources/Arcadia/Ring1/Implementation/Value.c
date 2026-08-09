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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/Value.h"

#include "Arcadia/Ring1/Implementation/Diagnostics.h"
#include "Arcadia/Ring1/Implementation/Object.h"
#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/Ring1/Implementation/Thread.h"
#include "Arcadia/Ring1/Implementation/ThreadExtensions.h"

// exit, EXIT_FAILURE
#include <stdlib.h>

void
Arcadia_Value_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Value* self
  )
{
  switch (self->tag) {
    case Arcadia_ValueTag_Atom: {
      Arcadia_Atom_visit(thread, self->atomValue);
    } break;
    case Arcadia_ValueTag_BigInteger: {
      Arcadia_BigInteger_visit(thread, self->bigIntegerValue);
    } break;
    case Arcadia_ValueTag_Boolean: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_ForeignProcedure: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_RuntimeByteArray: {
      Arcadia_RuntimeByteArray_visit(thread, self->runtimeByteArrayValue);
    } break;
    case Arcadia_ValueTag_RuntimeUTF8String: {
      Arcadia_RuntimeUTF8String_visit(thread, self->runtimeUTF8StringValue);
    } break;
    case Arcadia_ValueTag_Integer16: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Integer32: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Integer64: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Integer8: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Natural16: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Natural32: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Natural64: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Natural8: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_ObjectReference: {
      Arcadia_Object_visit(thread, self->objectReferenceValue);
    } break;
    case Arcadia_ValueTag_Real32: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Real64: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Size: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Type: {
      Arcadia_Type_visit(thread, self->typeValue);
    } break;
    case Arcadia_ValueTag_Void: {
      /* Intentionally empty. */
    } break;
    case Arcadia_ValueTag_Enumeration: {
      Arcadia_Type_visit(thread, self->enumerationValue.type);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

Arcadia_TypeValue
Arcadia_Value_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self
  )
{
  switch (self->tag) {
    case Arcadia_ValueTag_Atom: {
      return _Arcadia_AtomValue_getType(thread);
    } break;
    case Arcadia_ValueTag_BigInteger: {
      return _Arcadia_BigIntegerValue_getType(thread);
    } break;
    case Arcadia_ValueTag_Boolean: {
      return _Arcadia_BooleanValue_getType(thread);
    } break;
    case Arcadia_ValueTag_ForeignProcedure: {
      return _Arcadia_ForeignProcedureValue_getType(thread);
    } break;
    case Arcadia_ValueTag_RuntimeByteArray: {
      return _Arcadia_RuntimeByteArrayValue_getType(thread);
    } break;
    case Arcadia_ValueTag_RuntimeUTF8String: {
      return _Arcadia_RuntimeUTF8StringValue_getType(thread);
    } break;
    case Arcadia_ValueTag_Integer16: {
      return _Arcadia_Integer16Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Integer32: {
      return _Arcadia_Integer32Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Integer64: {
      return _Arcadia_Integer64Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Integer8: {
      return _Arcadia_Integer8Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Natural16: {
      return _Arcadia_Natural16Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Natural32: {
      return _Arcadia_Natural32Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Natural64: {
      return _Arcadia_Natural64Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Natural8: {
      return _Arcadia_Natural8Value_getType(thread);
    } break;
    case Arcadia_ValueTag_ObjectReference: {
      return Arcadia_Object_getType(thread, self->objectReferenceValue);
    } break;
    case Arcadia_ValueTag_Real32: {
      return _Arcadia_Real32Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Real64: {
      return _Arcadia_Real64Value_getType(thread);
    } break;
    case Arcadia_ValueTag_Size: {
      return _Arcadia_SizeValue_getType(thread);
    } break;
    case Arcadia_ValueTag_Type: {
      return _Arcadia_Type_getType(thread);
    } break;
    case Arcadia_ValueTag_Void: {
      return _Arcadia_VoidValue_getType(thread);
    } break;
    case Arcadia_ValueTag_Enumeration: {
      return self->enumerationValue.type;
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

#define OnRelational(Type, Operation) \
  case Arcadia_ValueTag_##Type: { \
      Arcadia_SizeValue oldSize = Arcadia_ValueStack_getSize(thread); \
      Arcadia_TypeValue type = _Arcadia_##Type##Value_getType(thread); \
      Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(type); \
      Arcadia_ValueStack_pushValue(thread, self); \
      Arcadia_ValueStack_pushValue(thread, other); \
      Arcadia_ValueStack_pushNatural8Value(thread, 2); \
      operations->Operation(thread); \
      if (Arcadia_ValueStack_getSize(thread) != oldSize + 1) { \
        Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption); \
        Arcadia_Thread_jump(thread); \
      } \
      Arcadia_BooleanValue v = Arcadia_ValueStack_getBooleanValue(thread, 0); \
      Arcadia_ValueStack_popValues(thread, 1); \
      return v; \
  } break;

Arcadia_BooleanValue
Arcadia_Value_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  )
{
  switch (self->tag) {
    case Arcadia_ValueTag_Atom: {
      // TODO: Add and use isEqualTo similar to BigInteger, RuntimeByteArray, RuntimeUTF8String, etc.
      if (!Arcadia_Value_isAtomValue(other)) {
        return Arcadia_BooleanValue_False;
      }
      return self->atomValue == other->atomValue;
    } break;
    OnRelational(BigInteger, isEqualTo);
    OnRelational(Boolean, isEqualTo);
    OnRelational(ForeignProcedure, isEqualTo);
    OnRelational(RuntimeByteArray, isEqualTo);
    OnRelational(RuntimeUTF8String, isEqualTo);
    OnRelational(Integer16, isEqualTo);
    OnRelational(Integer32, isEqualTo);
    OnRelational(Integer64, isEqualTo);
    OnRelational(Integer8, isEqualTo);
    OnRelational(Natural16, isEqualTo);
    OnRelational(Natural32, isEqualTo);
    OnRelational(Natural64, isEqualTo);
    OnRelational(Natural8, isEqualTo);
    OnRelational(Real32, isEqualTo);
    OnRelational(Real64, isEqualTo);
    OnRelational(Size, isEqualTo);
    OnRelational(Void, isEqualTo);
    case Arcadia_ValueTag_ObjectReference: {
      return Arcadia_Object_isEqualTo(thread, self->objectReferenceValue, other);
    } break;
    case Arcadia_ValueTag_Type: {
      if (!Arcadia_Value_isTypeValue(other)) {
        return Arcadia_BooleanValue_False;
      }
      return self->typeValue == other->typeValue;
    } break;
    case Arcadia_ValueTag_Enumeration: {
      return self->enumerationValue.type == other->enumerationValue.type
          && self->enumerationValue.value == other->enumerationValue.value;
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

Arcadia_BooleanValue
Arcadia_Value_isNotEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  )
{ return !Arcadia_Value_isEqualTo(thread, self, other); }

Arcadia_BooleanValue
Arcadia_Value_isLowerThan
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  )
{
  switch (self->tag) {
    OnRelational(Atom, isLowerThan);
    OnRelational(BigInteger, isLowerThan);
    OnRelational(Boolean, isLowerThan);
    OnRelational(ForeignProcedure, isLowerThan);
    OnRelational(RuntimeByteArray, isLowerThan);
    OnRelational(RuntimeUTF8String, isLowerThan);
    OnRelational(Integer16, isLowerThan);
    OnRelational(Integer32, isLowerThan);
    OnRelational(Integer64, isLowerThan);
    OnRelational(Integer8, isLowerThan);
    OnRelational(Natural16, isLowerThan);
    OnRelational(Natural32, isLowerThan);
    OnRelational(Natural64, isLowerThan);
    OnRelational(Natural8, isLowerThan);
    OnRelational(Real32, isLowerThan);
    OnRelational(Real64, isLowerThan);
    OnRelational(Size, isLowerThan);
    OnRelational(Void, isLowerThan);
    case Arcadia_ValueTag_ObjectReference: {
      Arcadia_TypeValue type = Arcadia_Object_getType(thread, self->objectReferenceValue);
      Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(type);
      Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushValue(thread, self);
      Arcadia_ValueStack_pushValue(thread, other);
      Arcadia_ValueStack_pushNatural8Value(thread, 2);
      operations->isLowerThan(thread);
      if (Arcadia_ValueStack_getSize(thread) != oldValueStackSize + 1) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_BooleanValue v = Arcadia_ValueStack_getBooleanValue(thread, 0);
      Arcadia_ValueStack_popValues(thread, 1);
      return v;
    } break;
    case Arcadia_ValueTag_Type: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
      Arcadia_Thread_jump(thread);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

Arcadia_BooleanValue
Arcadia_Value_isLowerThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  )
{
  switch (self->tag) {
    OnRelational(Atom, isLowerThanOrEqualTo);
    OnRelational(BigInteger, isLowerThanOrEqualTo);
    OnRelational(Boolean, isLowerThanOrEqualTo);
    OnRelational(ForeignProcedure, isLowerThanOrEqualTo);
    OnRelational(RuntimeByteArray, isLowerThanOrEqualTo);
    OnRelational(RuntimeUTF8String, isLowerThanOrEqualTo);
    OnRelational(Integer16, isLowerThanOrEqualTo);
    OnRelational(Integer32, isLowerThanOrEqualTo);
    OnRelational(Integer64, isLowerThanOrEqualTo);
    OnRelational(Integer8, isLowerThanOrEqualTo);
    OnRelational(Natural16, isLowerThanOrEqualTo);
    OnRelational(Natural32, isLowerThanOrEqualTo);
    OnRelational(Natural64, isLowerThanOrEqualTo);
    OnRelational(Natural8, isLowerThanOrEqualTo);
    OnRelational(Real32, isLowerThanOrEqualTo);
    OnRelational(Real64, isLowerThanOrEqualTo);
    OnRelational(Size, isLowerThanOrEqualTo);
    OnRelational(Void, isLowerThanOrEqualTo);
    case Arcadia_ValueTag_ObjectReference: {
      Arcadia_TypeValue type = Arcadia_Object_getType(thread, self->objectReferenceValue);
      Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(type);
      Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushValue(thread, self);
      Arcadia_ValueStack_pushValue(thread, other);
      Arcadia_ValueStack_pushNatural8Value(thread, 2);
      operations->isLowerThanOrEqualTo(thread);
      if (Arcadia_ValueStack_getSize(thread) != oldValueStackSize + 1) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_BooleanValue v = Arcadia_ValueStack_getBooleanValue(thread, 0);
      Arcadia_ValueStack_popValues(thread, 1);
      return v;
    } break;
    case Arcadia_ValueTag_Type: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
      Arcadia_Thread_jump(thread);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

Arcadia_BooleanValue
Arcadia_Value_isGreaterThan
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  )
{
  switch (self->tag) {
    OnRelational(Atom, isGreaterThan);
    OnRelational(BigInteger, isGreaterThan);
    OnRelational(Boolean, isGreaterThan);
    OnRelational(ForeignProcedure, isGreaterThan);
    OnRelational(RuntimeByteArray, isGreaterThan);
    OnRelational(RuntimeUTF8String, isGreaterThan);
    OnRelational(Integer16, isGreaterThan);
    OnRelational(Integer32, isGreaterThan);
    OnRelational(Integer64, isGreaterThan);
    OnRelational(Integer8, isGreaterThan);
    OnRelational(Natural16, isGreaterThan);
    OnRelational(Natural32, isGreaterThan);
    OnRelational(Natural64, isGreaterThan);
    OnRelational(Natural8, isGreaterThan);
    OnRelational(Real32, isGreaterThan);
    OnRelational(Real64, isGreaterThan);
    OnRelational(Size, isGreaterThan);
    OnRelational(Void, isGreaterThan);
    case Arcadia_ValueTag_ObjectReference: {
      Arcadia_TypeValue type = Arcadia_Object_getType(thread, self->objectReferenceValue);
      Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(type);
      Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushValue(thread, self);
      Arcadia_ValueStack_pushValue(thread, other);
      Arcadia_ValueStack_pushNatural8Value(thread, 2);
      operations->isGreaterThan(thread);
      if (Arcadia_ValueStack_getSize(thread) != oldValueStackSize + 1) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_BooleanValue v = Arcadia_ValueStack_getBooleanValue(thread, 0);
      Arcadia_ValueStack_popValues(thread, 1);
      return v;
    } break;
    case Arcadia_ValueTag_Type: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
      Arcadia_Thread_jump(thread);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

Arcadia_BooleanValue
Arcadia_Value_isGreaterThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  )
{
  switch (self->tag) {
    OnRelational(Atom, isGreaterThanOrEqualTo);
    OnRelational(BigInteger, isGreaterThanOrEqualTo);
    OnRelational(Boolean, isGreaterThanOrEqualTo);
    OnRelational(ForeignProcedure, isGreaterThanOrEqualTo);
    OnRelational(RuntimeByteArray, isGreaterThanOrEqualTo);
    OnRelational(RuntimeUTF8String, isGreaterThanOrEqualTo);
    OnRelational(Integer16, isGreaterThanOrEqualTo);
    OnRelational(Integer32, isGreaterThanOrEqualTo);
    OnRelational(Integer64, isGreaterThanOrEqualTo);
    OnRelational(Integer8, isGreaterThanOrEqualTo);
    OnRelational(Natural16, isGreaterThanOrEqualTo);
    OnRelational(Natural32, isGreaterThanOrEqualTo);
    OnRelational(Natural64, isGreaterThanOrEqualTo);
    OnRelational(Natural8, isGreaterThanOrEqualTo);
    OnRelational(Real32, isGreaterThanOrEqualTo);
    OnRelational(Real64, isGreaterThanOrEqualTo);
    OnRelational(Size, isGreaterThanOrEqualTo);
    OnRelational(Void, isGreaterThanOrEqualTo);
    case Arcadia_ValueTag_ObjectReference: {
      Arcadia_TypeValue type = Arcadia_Object_getType(thread, self->objectReferenceValue);
      Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(type);
      Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushValue(thread, self);
      Arcadia_ValueStack_pushValue(thread, other);
      Arcadia_ValueStack_pushNatural8Value(thread, 2);
      operations->isGreaterThanOrEqualTo(thread);
      if (Arcadia_ValueStack_getSize(thread) != oldValueStackSize + 1) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_BooleanValue v = Arcadia_ValueStack_getBooleanValue(thread, 0);
      Arcadia_ValueStack_popValues(thread, 1);
      return v;
    } break;
    case Arcadia_ValueTag_Type: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
      Arcadia_Thread_jump(thread);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

#undef OnRelational

#define OnGetHash(Type) \
  case Arcadia_ValueTag_##Type: { \
    Arcadia_TypeValue type = _Arcadia_##Type##Value_getType(thread); \
    Arcadia_Type_Operations const* operations = Arcadia_Type_getOperations(type); \
\
    Arcadia_SizeValue n = Arcadia_ValueStack_getSize(thread); \
\
    Arcadia_ValueStack_pushValue(thread, self); \
    Arcadia_ValueStack_pushNatural8Value(thread, 1); \
\
    operations->getHash(thread); \
    if (n + 1 != Arcadia_ValueStack_getSize(thread)) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption); \
      Arcadia_Thread_jump(thread); \
    } \
    Arcadia_SizeValue v = Arcadia_ValueStack_getSizeValue(thread, 0); \
    Arcadia_ValueStack_popValues(thread, 1); \
    return v; \
  } break;

Arcadia_SizeValue
Arcadia_Value_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_Value* self
  )
{
  switch (self->tag) {
    OnGetHash(Atom);
    OnGetHash(BigInteger);
    OnGetHash(Boolean);
    OnGetHash(ForeignProcedure);
    OnGetHash(RuntimeByteArray);
    OnGetHash(RuntimeUTF8String);
    OnGetHash(Integer16);
    OnGetHash(Integer32);
    OnGetHash(Integer64);
    OnGetHash(Integer8);
    OnGetHash(Natural16);
    OnGetHash(Natural32);
    OnGetHash(Natural64);
    OnGetHash(Natural8);
    OnGetHash(Real32);
    OnGetHash(Real64);
    OnGetHash(Size);
    OnGetHash(Void);
    case Arcadia_ValueTag_ObjectReference: {
      return Arcadia_Object_getHash(thread, self->objectReferenceValue);
    } break;
    case Arcadia_ValueTag_Type: {
      return Arcadia_Type_getHash(thread, self->typeValue);
    } break;
    case Arcadia_ValueTag_Enumeration: {
      return (Arcadia_SizeValue)self->enumerationValue.value ^ Arcadia_Type_getHash(thread, self->enumerationValue.type);
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

#undef OnGetHash

#define OnIsInstanceOf(Type) \
  case Arcadia_ValueTag_##Type: { \
    Arcadia_TypeValue type1 = _Arcadia_##Type##Value_getType(thread); \
\
    return (type1 == type); \
  } break;

Arcadia_BooleanValue
Arcadia_Value_isInstanceOf
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Type* type
  )
{
  switch (self->tag) {
    OnIsInstanceOf(Atom);
    OnIsInstanceOf(BigInteger);
    OnIsInstanceOf(Boolean);
    OnIsInstanceOf(ForeignProcedure);
    OnIsInstanceOf(RuntimeByteArray);
    OnIsInstanceOf(RuntimeUTF8String);
    OnIsInstanceOf(Integer16);
    OnIsInstanceOf(Integer32);
    OnIsInstanceOf(Integer64);
    OnIsInstanceOf(Integer8);
    OnIsInstanceOf(Natural16);
    OnIsInstanceOf(Natural32);
    OnIsInstanceOf(Natural64);
    OnIsInstanceOf(Natural8);
    OnIsInstanceOf(Real32);
    OnIsInstanceOf(Real64);
    OnIsInstanceOf(Size);
    OnIsInstanceOf(Void);
    case Arcadia_ValueTag_ObjectReference: {
      return Arcadia_Object_isInstanceOf(thread, self->objectReferenceValue, type);
    } break;
    case Arcadia_ValueTag_Type: {
      return _Arcadia_Type_getType(thread) == type;
    } break;
    case Arcadia_ValueTag_Enumeration: {
      return self->enumerationValue.type == type;
    } break;
    default: {
      Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    } break;
  };
}

#undef OnIsInstanceOF

/*@extension*/
Arcadia_ObjectReferenceValue
Arcadia_Value_getObjectReferenceValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Value self,
    Arcadia_Type* type
  )
{
  if (!Arcadia_Value_isObjectReferenceValue(&self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object* o = Arcadia_Value_getObjectReferenceValue(&self);
  if (!Arcadia_Object_isInstanceOf(thread, o, type)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  return o;
}
