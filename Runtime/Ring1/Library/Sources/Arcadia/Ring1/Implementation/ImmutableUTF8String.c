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
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/fromBytes.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/hash.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toBoolean.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toInteger.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toNatural.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toReal.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toSize.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toVoid.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/type.h"

#include "Arcadia/Ring1/Implementation/BooleanToString/Include.h"
#include "Arcadia/Ring1/Implementation/IntegerToString/Include.h"
#include "Arcadia/Ring1/Implementation/NaturalToString/Include.h"
#include "Arcadia/Ring1/Implementation/SizeToString/Include.h"
#include "Arcadia/Ring1/Implementation/VoidToString/Include.h"

typedef struct Context {
  Arcadia_RuntimeUTF8String* string;
} Context;

static void
callback
  (
    Arcadia_Thread* thread,
    Context* context,
    const Arcadia_Natural8Value* p,
    Arcadia_SizeValue n
  )
{
  if (Arcadia_SizeValue_Maximum - sizeof(Arcadia_RuntimeUTF8String) < n) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  _ensureTypeRegistered(thread);
  Arcadia_RuntimeUTF8String* string = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&string, TypeName, sizeof(TypeName) - 1, sizeof(Arcadia_RuntimeUTF8String) + n);
  Arcadia_Memory_copy(thread, string->bytes, p, n);
  string->numberOfBytes = n;
  string->hash = _hashUTF8(thread, p, n);

  context->string = string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  return _createFromBytes(thread, bytes, numberOfBytes);
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue
  )
{
  Context context;
  Arcadia_BooleanValue_toUTF8String(thread, booleanValue, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue))&callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value
  )
{
  Context context;
  Arcadia_Integer16Value_toUTF8String(thread, integer16Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  )
{
  Context context;
  Arcadia_Integer32Value_toUTF8String(thread, integer32Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value
  )
{
  Context context;
  Arcadia_Integer64Value_toUTF8String(thread, integer64Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value
  )
{
  Context context;
  Arcadia_Integer8Value_toUTF8String(thread, integer8Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value
  )
{
  Context context;
  Arcadia_Natural16Value_toUTF8String(thread, natural16Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value
  )
{
  Context context;
  Arcadia_Natural32Value_toUTF8String(thread, natural32Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value
  )
{
  Context context;
  Arcadia_Natural64Value_toUTF8String(thread, natural64Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value
  )
{
  Context context;
  Arcadia_Natural8Value_toUTF8String(thread, natural8Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromReal32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value real32Value
  )
{
  Context context;
  Arcadia_Real32Value_toUTF8String(thread, real32Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromReal64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value real64Value
  )
{
  Context context;
  Arcadia_Real64Value_toUTF8String(thread, real64Value, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromSize
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue sizeValue
  )
{
  Context context;
  Arcadia_SizeValue_toUTF8String(thread, sizeValue, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createFromVoid
  (
    Arcadia_Thread* thread,
    Arcadia_VoidValue voidValue
  )
{
  Context context;
  Arcadia_VoidValue_toUTF8String(thread, voidValue, &context, (void (*)(Arcadia_Thread*, void*, const Arcadia_Natural8Value*, Arcadia_SizeValue)) &callback);
  return context.string;
}

void
Arcadia_RuntimeUTF8String_visit
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), self); }

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

void
Arcadia_RuntimeUTF8String_ensureGray
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ /*Arcadia_Process_ensureGray(Arcadia_Thread_getProcess(thread), self);*/ }

#endif

Arcadia_BooleanValue
Arcadia_RuntimeUTF8String_startsWith
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self,
    Arcadia_RuntimeUTF8StringValue other
  )
{
  // "this" cannot start with "other" if the formaer is shorter than the latter is shorter.
  if (Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self) < Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, other)) {
    return Arcadia_BooleanValue_False;
  }
  // The length of "other" is smaller than or equal to the length of "this".
  return !Arcadia_Memory_compare(thread,
                                 Arcadia_RuntimeUTF8String_getBytes(thread, self),
                                 Arcadia_RuntimeUTF8String_getBytes(thread, other),
                                 Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, other));
}

Arcadia_BooleanValue
Arcadia_RuntimeUTF8String_endsWith
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self,
    Arcadia_RuntimeUTF8StringValue other
  )
{
  // "this" cannot end with "other" if the formaer is shorter than the latter is shorter.
  if (Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self) < Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, other)) {
    return Arcadia_BooleanValue_False;
  }
  // The length of "other" is smaller than or equal to the length of "this".
  Arcadia_SizeValue d = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self) - Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, other);
  return !Arcadia_Memory_compare(thread,
                                  Arcadia_RuntimeUTF8String_getBytes(thread, self) + d,
                                  Arcadia_RuntimeUTF8String_getBytes(thread, other),
                                  Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, other));
}

Arcadia_Natural8Value const*
Arcadia_RuntimeUTF8String_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return self->bytes; }

Arcadia_SizeValue
Arcadia_RuntimeUTF8String_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return self->numberOfBytes; }

Arcadia_SizeValue
Arcadia_RuntimeUTF8String_getHash
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return self->hash; }

Arcadia_SizeValue
Arcadia_RuntimeUTF8String_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{
  Arcadia_SizeValue numberOfCodePoints;
  Arcadia_Unicode_isUTF8(thread, Arcadia_RuntimeUTF8String_getBytes(thread, self), Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self), &numberOfCodePoints);
  return numberOfCodePoints;
}

Arcadia_BooleanValue
Arcadia_RuntimeUTF8String_toBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toBoolean(thread, self); }

Arcadia_Integer16Value
Arcadia_RuntimeUTF8String_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toInteger16(thread, self); }

Arcadia_Integer32Value
Arcadia_RuntimeUTF8String_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toInteger32(thread, self); }

Arcadia_Integer64Value
Arcadia_RuntimeUTF8String_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toInteger64(thread, self); }

Arcadia_Integer8Value
Arcadia_RuntimeUTF8String_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toInteger8(thread, self); }

Arcadia_Natural16Value
Arcadia_RuntimeUTF8String_toNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toNatural16(thread, self); }

Arcadia_Natural32Value
Arcadia_RuntimeUTF8String_toNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toNatural32(thread, self); }

Arcadia_Natural64Value
Arcadia_RuntimeUTF8String_toNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toNatural64(thread, self); }

Arcadia_Natural8Value
Arcadia_RuntimeUTF8String_toNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toNatural8(thread, self); }

Arcadia_Real32Value
Arcadia_RuntimeUTF8String_toReal32
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toReal32(thread, self); }

Arcadia_Real64Value
Arcadia_RuntimeUTF8String_toReal64
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toReal64(thread, self); }

Arcadia_SizeValue
Arcadia_RuntimeUTF8String_toSize
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toSize(thread, self); }

Arcadia_VoidValue
Arcadia_RuntimeUTF8String_toVoid
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8StringValue self
  )
{ return _toVoid(thread, self); }

static void
isEqualTo
  (
    Arcadia_Thread* thread
  );

static void
getHash
  (
    Arcadia_Thread* thread
  );

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  );

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .isEqualTo = &isEqualTo,
  .getHash = &getHash,
  .isNotEqualTo = &isNotEqualTo,
};

#define BINARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 3) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (2 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 2); \
  Arcadia_Value y = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 3);

#define UNARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 2) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (1 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 2);

static void
isEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isRuntimeUTF8StringValue(&y)) {
    Arcadia_RuntimeUTF8String* a1 = Arcadia_Value_getRuntimeUTF8StringValue(&x);
    Arcadia_RuntimeUTF8String* a2 = Arcadia_Value_getRuntimeUTF8StringValue(&y);
    if (a1 == a2) {
      Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
    } else {
      if (a1->numberOfBytes != a2->numberOfBytes) {
        Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
      } else {
        Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Memory_compare(thread, a1->bytes, a2->bytes, a1->numberOfBytes) == 0);
      }
    }
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
getHash
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)(uintptr_t)Arcadia_Value_getRuntimeUTF8StringValue(&x));
}

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isRuntimeUTF8StringValue(&y)) {
    Arcadia_RuntimeUTF8String* a1 = Arcadia_Value_getRuntimeUTF8StringValue(&x);
    Arcadia_RuntimeUTF8String* a2 = Arcadia_Value_getRuntimeUTF8StringValue(&y);
    if (a1 == a2) {
      Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    } else {
      if (a1->numberOfBytes != a2->numberOfBytes) {
        Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
      } else {
        Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Memory_compare(thread, a1->bytes, a2->bytes, a1->numberOfBytes) != 0);
      }
    }
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
  }
}

static Arcadia_TypeValue g_type = NULL;

static void
typeDestructing
  (
    void* context
  )
{
  g_type = NULL;
}

Arcadia_TypeValue
_Arcadia_RuntimeUTF8StringValue_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_type) {
    g_type = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, TypeName, sizeof(TypeName) - 1), &_typeOperations, &typeDestructing);
  }
  return g_type;
}
