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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_VALUE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_VALUE_H_INCLUDED

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

#include "Arcadia/Ring1/Implementation/ObjectReference.h"

#include "Arcadia/Ring1/Implementation/Real32.h"
#include "Arcadia/Ring1/Implementation/Real64.h"

#include "Arcadia/Ring1/Implementation/Size.h"

#include "Arcadia/Ring1/Implementation/TypeSystem/Include.h"

#include "Arcadia/Ring1/Implementation/Void.h"

#define Arcadia_ValueTag_Atom (1)

#define Arcadia_ValueTag_BigInteger (2)

#define Arcadia_ValueTag_Boolean (3)

#define Arcadia_ValueTag_ForeignProcedure (4)

#define Arcadia_ValueTag_RuntimeByteArray (5)
#define Arcadia_ValueTag_RuntimeUTF8String (6)

#define Arcadia_ValueTag_Integer16 (7)
#define Arcadia_ValueTag_Integer32 (8)
#define Arcadia_ValueTag_Integer64 (9)
#define Arcadia_ValueTag_Integer8 (10)

#define Arcadia_ValueTag_Natural16 (11)
#define Arcadia_ValueTag_Natural32 (12)
#define Arcadia_ValueTag_Natural64 (13)
#define Arcadia_ValueTag_Natural8 (14)

#define Arcadia_ValueTag_ObjectReference (15)

#define Arcadia_ValueTag_Real32 (16)
#define Arcadia_ValueTag_Real64 (17)

#define Arcadia_ValueTag_Size (18)

#define Arcadia_ValueTag_Type (19)

#define Arcadia_ValueTag_Enumeration (20)

// The tag for type "Void" must be 0.
#define Arcadia_ValueTag_Void (0)

typedef struct Arcadia_Value {
  uint8_t tag;
  union {

  #define Define(Prefix, Suffix, Variable) \
    Prefix##_##Suffix##Value Variable##Value;

    Define(Arcadia, Atom, atom)

    Define(Arcadia, BigInteger, bigInteger)

    Define(Arcadia, Boolean, boolean)

    Define(Arcadia, ForeignProcedure, foreignProcedure)

    Define(Arcadia, RuntimeByteArray, runtimeByteArray)
    Define(Arcadia, RuntimeUTF8String, runtimeUTF8String)

    Define(Arcadia, Integer16, integer16)
    Define(Arcadia, Integer32, integer32)
    Define(Arcadia, Integer64, integer64)
    Define(Arcadia, Integer8, integer8)

    Define(Arcadia, Natural16, natural16)
    Define(Arcadia, Natural32, natural32)
    Define(Arcadia, Natural64, natural64)
    Define(Arcadia, Natural8, natural8)

    Define(Arcadia, ObjectReference, objectReference)

    Define(Arcadia, Real32, real32)
    Define(Arcadia, Real64, real64)

    Define(Arcadia, Size, size)

    Define(Arcadia, Type, type)

    Define(Arcadia, Void, void)

    Define(Arcadia, Enumeration, enumeration)

  #undef Define
  };
} Arcadia_Value;

static inline uint8_t
Arcadia_Value_getTag
  (
    Arcadia_Value const* value
  )
{ return value->tag; }

#define Arcadia_Value_Initializer() \
  (Arcadia_Value){ .tag = Arcadia_ValueTag_Void, .voidValue = Arcadia_VoidValue_Void }

#define Define(Prefix, Suffix, Variable) \
  static inline Prefix##_##Suffix##Value \
  Arcadia_Value_get##Suffix##Value \
    ( \
      Arcadia_Value const* value \
    ) \
  {  return value->Variable##Value; } \
\
  static inline bool \
  Arcadia_Value_is##Suffix##Value \
    ( \
      Arcadia_Value const* value \
    ) \
  { return Arcadia_ValueTag_##Suffix == value->tag; } \
  \
  static inline void \
  Arcadia_Value_set##Suffix##Value \
    ( \
      Arcadia_Value* value, \
      Prefix##_##Suffix##Value const Variable##Value \
    ) \
  { \
    value->tag = Arcadia_ValueTag_##Suffix; \
    value->Variable##Value = Variable##Value; \
  } \
  \
  static inline Arcadia_Value \
  Arcadia_Value_make##Suffix##Value \
    ( \
      Prefix##_##Suffix##Value const Variable##Value \
    ) \
  { \
    Arcadia_Value value = { .tag = Arcadia_ValueTag_##Suffix, .Variable##Value = Variable##Value, };  \
    return value; \
  }

Define(Arcadia, Atom, atom)

Define(Arcadia, BigInteger, bigInteger)

Define(Arcadia, Boolean, boolean)

Define(Arcadia, ForeignProcedure, foreignProcedure)

Define(Arcadia, RuntimeByteArray, runtimeByteArray)
Define(Arcadia, RuntimeUTF8String, runtimeUTF8String)

Define(Arcadia, Integer16, integer16)
Define(Arcadia, Integer32, integer32)
Define(Arcadia, Integer64, integer64)
Define(Arcadia, Integer8, integer8)

Define(Arcadia, Natural16, natural16)
Define(Arcadia, Natural32, natural32)
Define(Arcadia, Natural64, natural64)
Define(Arcadia, Natural8, natural8)

Define(Arcadia, ObjectReference, objectReference)

Define(Arcadia, Real32, real32)
Define(Arcadia, Real64, real64)

Define(Arcadia, Size, size)

Define(Arcadia, Type, type)

Define(Arcadia, Void, void)

Define(Arcadia, Enumeration, enumeration)

#undef Define

void
Arcadia_Value_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Value* self
  );

Arcadia_TypeValue
Arcadia_Value_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self
  );

Arcadia_BooleanValue
Arcadia_Value_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  );

Arcadia_BooleanValue
Arcadia_Value_isNotEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  );

Arcadia_BooleanValue
Arcadia_Value_isLowerThan
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  );

Arcadia_BooleanValue
Arcadia_Value_isLowerThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  );

Arcadia_BooleanValue
Arcadia_Value_isGreaterThan
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  );

Arcadia_BooleanValue
Arcadia_Value_isGreaterThanOrEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Value const* other
  );

Arcadia_SizeValue
Arcadia_Value_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_Value* self
  );

Arcadia_BooleanValue
Arcadia_Value_isInstanceOf
  (
    Arcadia_Thread* thread,
    Arcadia_Value const* self,
    Arcadia_Type* type
  );

/**
 * @extension
 * @brief
 * Get if a is an object value of the specified type.
 * @param thread
 * A pointer to this thread.
 * @param self
 * The value.
 * @param type
 * A pointer to a type.
 * @return
 * A pointer to the object value.
 */
Arcadia_ObjectReferenceValue
Arcadia_Value_getObjectReferenceValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Value self,
    Arcadia_Type* type
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_VALUE_H_INCLUDED
