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
#include "Arcadia/Ring1/Implementation/Objects/String.h"

#include "Arcadia/Ring1/Include.h"
#include <assert.h>

static void
Arcadia_String_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_String* self
  );

static void
Arcadia_String_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StringDispatch* self
  );

static void
isEqualToImpl
  (
    Arcadia_Thread* thread
  );

static void
getHashImpl
  (
    Arcadia_Thread* thread
  );

static void
Arcadia_String_visit
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_String_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_String_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_String_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
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

Arcadia_defineObjectType(u8"Arcadia.String", Arcadia_String,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

#define On(Type, Suffix) \
  static void \
  from##Suffix \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_RuntimeUTF8String** pRuntimeUTF8String, \
      Type##Value value \
    ) \
  { \
    *pRuntimeUTF8String = Arcadia_RuntimeUTF8String_createFrom##Suffix(thread, value); \
  }

On(Arcadia_Boolean, Boolean)

On(Arcadia_Integer16, Integer16)
On(Arcadia_Integer32, Integer32)
On(Arcadia_Integer64, Integer64)
On(Arcadia_Integer8, Integer8)

On(Arcadia_Natural16, Natural16)
On(Arcadia_Natural32, Natural32)
On(Arcadia_Natural64, Natural64)
On(Arcadia_Natural8, Natural8)

On(Arcadia_Void, Void)

static void
fromImmutableByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String** pRuntimeUTF8String,
    Arcadia_RuntimeByteArray* value
  )
{
  *pRuntimeUTF8String =
    Arcadia_RuntimeUTF8String_create
      (
        thread,
        Arcadia_RuntimeByteArray_getBytes(thread, value),
        Arcadia_RuntimeByteArray_getNumberOfBytes(thread, value)
      );
}

static void
fromRuntimeUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String** pRuntimeUTF8String,
    Arcadia_RuntimeUTF8String* value
  )
{
  *pRuntimeUTF8String = value;
}

static void
Arcadia_String_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_String* self
  )
{
  Arcadia_EnterConstructor(Arcadia_String);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1);
  if (Arcadia_Value_isBooleanValue(&x)) {
    fromBoolean(thread, &self->immutableUTF8String, Arcadia_Value_getBooleanValue(&x));
  } else if (Arcadia_Value_isInteger16Value(&x)) {
    fromInteger16(thread, &self->immutableUTF8String, Arcadia_Value_getInteger16Value(&x));
  } else if (Arcadia_Value_isInteger32Value(&x)) {
    fromInteger32(thread, &self->immutableUTF8String, Arcadia_Value_getInteger32Value(&x));
  } else if (Arcadia_Value_isInteger64Value(&x)) {
    fromInteger64(thread, &self->immutableUTF8String, Arcadia_Value_getInteger64Value(&x));
  } else if (Arcadia_Value_isInteger8Value(&x)) {
    fromInteger8(thread, &self->immutableUTF8String, Arcadia_Value_getInteger8Value(&x));
  } else if (Arcadia_Value_isNatural16Value(&x)) {
    fromNatural16(thread, &self->immutableUTF8String, Arcadia_Value_getNatural16Value(&x));
  } else if (Arcadia_Value_isNatural32Value(&x)) {
    fromNatural32(thread, &self->immutableUTF8String, Arcadia_Value_getNatural32Value(&x));
  } else if (Arcadia_Value_isNatural64Value(&x)) {
    fromNatural64(thread, &self->immutableUTF8String, Arcadia_Value_getNatural64Value(&x));
  } else if (Arcadia_Value_isNatural8Value(&x)) {
    fromNatural8(thread, &self->immutableUTF8String, Arcadia_Value_getNatural8Value(&x));
  } if (Arcadia_Value_isRuntimeByteArrayValue(&x)) {
    fromImmutableByteArray(thread, &self->immutableUTF8String, Arcadia_Value_getRuntimeByteArrayValue(&x));
  } else if (Arcadia_Value_isRuntimeUTF8StringValue(&x)) {
    fromRuntimeUTF8String(thread, &self->immutableUTF8String, Arcadia_Value_getRuntimeUTF8StringValue(&x));
  } else if (Arcadia_Value_isObjectReferenceValue(&x)) {
    Arcadia_ObjectReferenceValue referenceValue = Arcadia_Value_getObjectReferenceValue(&x);
    if (Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, referenceValue), _Arcadia_ByteArrayBuilder_getType(thread))) {
      Arcadia_ByteArrayBuilder* object = (Arcadia_ByteArrayBuilder*)referenceValue;
      self->immutableUTF8String =
        Arcadia_RuntimeUTF8String_create
          (
            thread,
            Arcadia_ByteArrayBuilder_getBytes(thread, object),
            Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, object)
          );
    } else if (Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, referenceValue), _Arcadia_String_getType(thread))) {
      Arcadia_String* object = (Arcadia_String*)referenceValue;
      self->immutableUTF8String = object->immutableUTF8String;
    } else if (Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, referenceValue), _Arcadia_StringBuilder_getType(thread))) {
      Arcadia_StringBuilder* object = (Arcadia_StringBuilder*)referenceValue;
      self->immutableUTF8String = Arcadia_RuntimeUTF8String_create(thread, Arcadia_StringBuilder_getBytes(thread, object), Arcadia_StringBuilder_getNumberOfBytes(thread, object));
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (Arcadia_Value_isVoidValue(&x)) {
    fromVoid(thread, &self->immutableUTF8String, Arcadia_Value_getVoidValue(&x));
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_String);
}

static void
Arcadia_String_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StringDispatch* self
  )
{
  ((Arcadia_ObjectDispatch*)self)->isEqualTo = &isEqualToImpl;
  ((Arcadia_ObjectDispatch*)self)->getHash = &getHashImpl;
}

static void
isEqualToImpl
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  Arcadia_Object* a0 = Arcadia_Value_getObjectReferenceValue(&x);
  if (!Arcadia_Value_isObjectReferenceValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  Arcadia_Object* b0 = Arcadia_Value_getObjectReferenceValue(&y);
  if (a0 == b0) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
    return;
  }
  assert(NULL != b0);
  assert(Arcadia_Object_isInstanceOf(thread, b0, _Arcadia_Object_getType(thread)));
  if (!Arcadia_Type_isDescendantType(thread, Arcadia_Object_getType(thread, b0), _Arcadia_String_getType(thread))) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  Arcadia_String* a1 = (Arcadia_String*)a0;
  Arcadia_String* b1 = (Arcadia_String*)b0;
  if (Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, a1->immutableUTF8String) ==
      Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, b1->immutableUTF8String) &&
      Arcadia_RuntimeUTF8String_getHash(thread, a1->immutableUTF8String) ==
      Arcadia_RuntimeUTF8String_getHash(thread, b1->immutableUTF8String)) {
    Arcadia_ValueStack_pushBooleanValue
      (
        thread,
        !Arcadia_Memory_compare
          (
            thread,
            Arcadia_RuntimeUTF8String_getBytes(thread, a1->immutableUTF8String),
            Arcadia_RuntimeUTF8String_getBytes(thread, b1->immutableUTF8String),
            Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, a1->immutableUTF8String)
          )
      );
  } else {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
  }
}

static void
getHashImpl
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_String* self1 = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&x);
  Arcadia_ValueStack_pushSizeValue(thread, Arcadia_RuntimeUTF8String_getHash(thread, self1->immutableUTF8String));
}

static void
Arcadia_String_visit
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_RuntimeUTF8String_visit(thread, string->immutableUTF8String);
}

Arcadia_String*
Arcadia_String_create_pn
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArray* runtimeByteArray
  )
{
  _Arcadia_BeginCreate(Arcadia_String);
  Arcadia_ValueStack_pushRuntimeByteArrayValue(thread, runtimeByteArray);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_String);
}

Arcadia_String*
Arcadia_String_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  _Arcadia_BeginCreate(Arcadia_String);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_String);
}

Arcadia_BooleanValue
Arcadia_String_endsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self->immutableUTF8String) < numberOfBytes) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue d = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self->immutableUTF8String) - numberOfBytes;
  return !Arcadia_Memory_compare(thread,
                                 Arcadia_RuntimeUTF8String_getBytes(thread, self->immutableUTF8String) + d,
                                 bytes, numberOfBytes);
}

Arcadia_BooleanValue
Arcadia_String_startsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self->immutableUTF8String) < numberOfBytes) {
    return Arcadia_BooleanValue_False;
  }
  return !Arcadia_Memory_compare(thread,
                                 Arcadia_RuntimeUTF8String_getBytes(thread, self->immutableUTF8String),
                                 bytes, numberOfBytes);
}

Arcadia_SizeValue
Arcadia_String_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{
  return Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self->immutableUTF8String);
}

Arcadia_Natural8Value const*
Arcadia_String_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{
  return Arcadia_RuntimeUTF8String_getBytes(thread, self->immutableUTF8String);
}

Arcadia_Natural8Value
Arcadia_String_getByteAt
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    Arcadia_SizeValue index
  )
{
  if (index >= Arcadia_String_getNumberOfBytes(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return *(Arcadia_String_getBytes(thread, self) + index);
}

Arcadia_SizeValue
Arcadia_String_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{
  return Arcadia_RuntimeUTF8String_getNumberOfCodePoints(thread, self->immutableUTF8String);
}

Arcadia_BooleanValue
Arcadia_String_isEqualTo_pn
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, self->immutableUTF8String) == numberOfBytes) {
    return !Arcadia_Memory_compare(thread, Arcadia_RuntimeUTF8String_getBytes(thread, self->immutableUTF8String),
                                   bytes, numberOfBytes);
  } else {
    return Arcadia_BooleanValue_False;
  }
}

Arcadia_BooleanValue
Arcadia_String_toBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toBoolean(thread, self->immutableUTF8String); }

Arcadia_Integer16Value
Arcadia_String_toInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toInteger16(thread, self->immutableUTF8String); }

Arcadia_Integer32Value
Arcadia_String_toInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toInteger32(thread, self->immutableUTF8String); }

Arcadia_Integer64Value
Arcadia_String_toInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toInteger64(thread, self->immutableUTF8String); }

Arcadia_Integer8Value
Arcadia_String_toInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toInteger8(thread, self->immutableUTF8String); }

Arcadia_Natural16Value
Arcadia_String_toNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toNatural16(thread, self->immutableUTF8String); }

Arcadia_Natural32Value
Arcadia_String_toNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toNatural32(thread, self->immutableUTF8String); }

Arcadia_Natural64Value
Arcadia_String_toNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toNatural64(thread, self->immutableUTF8String); }

Arcadia_Natural8Value
Arcadia_String_toNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toNatural8(thread, self->immutableUTF8String); }

Arcadia_Real32Value
Arcadia_String_toReal32
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_toReal32(thread, Arcadia_String_getBytes(thread, self), Arcadia_String_getNumberOfBytes(thread, self)); }

Arcadia_Real64Value
Arcadia_String_toReal64
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_toReal64(thread, Arcadia_String_getBytes(thread, self), Arcadia_String_getNumberOfBytes(thread, self)); }

Arcadia_VoidValue
Arcadia_String_toVoid
  (
    Arcadia_Thread* thread,
    Arcadia_String const* self
  )
{ return Arcadia_RuntimeUTF8String_toVoid(thread, self->immutableUTF8String); }

Arcadia_String*
Arcadia_String_createFromBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromBoolean(thread, booleanValue))); }

Arcadia_String*
Arcadia_String_createFromInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger16(thread, integer16Value))); }

Arcadia_String*
Arcadia_String_createFromInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger32(thread, integer32Value))); }

Arcadia_String*
Arcadia_String_createFromInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger64(thread, integer64Value))); }

Arcadia_String*
Arcadia_String_createFromInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromInteger8(thread, integer8Value))); }

Arcadia_String*
Arcadia_String_createFromNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromNatural16(thread, natural16Value))); }

Arcadia_String*
Arcadia_String_createFromNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromNatural32(thread, natural32Value))); }


Arcadia_String*
Arcadia_String_createFromNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromNatural64(thread, natural64Value))); }

Arcadia_String*
Arcadia_String_createFromNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromNatural8(thread, natural8Value))); }

Arcadia_String*
Arcadia_String_createFromReal32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value real32Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromReal32(thread, real32Value))); }

Arcadia_String*
Arcadia_String_createFromReal64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value real64Value
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromReal64(thread, real64Value))); }

Arcadia_String*
Arcadia_String_createFromSize
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue sizeValue
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromSize(thread, sizeValue))); }

Arcadia_String*
Arcadia_String_createFromVoid
  (
    Arcadia_Thread* thread,
    Arcadia_VoidValue voidValue
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createFromVoid(thread, voidValue))); }

Arcadia_Value
Arcadia_String_findFirstOccurrence
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_Natural32Value codePoint
  )
{ return Arcadia_RuntimeUTF8String_findFirstOccurrence(thread, self->immutableUTF8String, codePoint); }

Arcadia_Value
Arcadia_String_findLastOccurrence
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_Natural32Value codePoint
  )
{ return Arcadia_RuntimeUTF8String_findLastOccurrence(thread, self->immutableUTF8String, codePoint); }

Arcadia_String*
Arcadia_String_createEmpty
  (
    Arcadia_Thread* thread
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_createEmpty(thread))); }

Arcadia_String*
Arcadia_String_substring
  (
    Arcadia_Thread* thread,
    Arcadia_String* self,
    Arcadia_SizeValue start,
    Arcadia_Value length
  )
{ return Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_substring(thread, self->immutableUTF8String, start, length))); }

Arcadia_ByteArray*
Arcadia_String_toByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_String* self
  )
{ return Arcadia_ByteArray_createByteArray(thread,Arcadia_RuntimeByteArray_create(thread, Arcadia_String_getBytes(thread, self), Arcadia_String_getNumberOfBytes(thread, self))); }
