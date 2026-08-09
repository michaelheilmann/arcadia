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
#include "Arcadia/Ring1/Implementation/Objects/StringBuilder.h"

#include "Arcadia/Ring1/Include.h"

/// @param offset [out] Pointer to an Arcadia_SizeValue variable.
/// That variable is assigned
/// - the offset of the first Byte of the code point at the code point index if that code point index is within bounds.
/// - @a n otherwise where @a n is the number of Bytes in the buffer.
/// @return #Arcadia_BooleanValue_True if the code point index is within bounds. #Arcadia_BooleanValue_False otherwise.
static Arcadia_BooleanValue
getOffsetFromFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_SizeValue* offset
  );

/// @param offset [out] Pointer to an Arcadia_SizeValue variable.
/// That variable is assigned
/// - the offset of the first Byte behind the Bytes of the code point at the code point index if that code point index is within bounds.
/// - @a 0 otherwise.
/// @return #Arcadia_BooleanValue_True if the code point index is within bounds. #Arcadia_BooleanValue_False otherwise.
static Arcadia_BooleanValue
getOffsetFromBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_SizeValue* offset
  );

static void
Arcadia_StringBuilder_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  );

static void
Arcadia_StringBuilder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilderDispatch* self
  );

static void
Arcadia_StringBuilder_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  );

static void
ensureFreeCapacityBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue requiredFreeCapacity
  );

// Warning: This function does not check if the Bytes are UTF-8 Bytes.
static void
prependBytesInternal
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

// Warning: This function does not check if the Bytes are UTF-8 Bytes.
static void
appendBytesInternal
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_StringBuilder_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_StringBuilder_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_StringBuilder_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.StringBuilder", Arcadia_StringBuilder,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static Arcadia_BooleanValue
getOffsetFromFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_SizeValue* offset
  )
{
  // Warning: This assumes valid UTF-8 Byte sequences. However: Arcadia_StringBuilder only holds valid UTF-8 Byte sequences.
  const Arcadia_Natural8Value* start = self->elements;
  const Arcadia_Natural8Value* end = self->elements + self->size;
  const Arcadia_Natural8Value* current = self->elements;
  Arcadia_SizeValue currentCodePointIndex = 0;
  while (currentCodePointIndex != codePointIndex && current != end) {
    const Arcadia_Natural8Value x = *current;
    if ((x & 0x80) == 0x00) {
      current += 1;
    } else if ((x & 0xE0) == 0xC0) {
      current += 2;
    } else if ((x & 0xF0) == 0xE0) {
      current += 3;
    } else /*if ((x & 0xF8) == 0xF0)*/ {
      current += 4;
    }
    currentCodePointIndex++;
  }
  *offset = current - start;
  return currentCodePointIndex == codePointIndex;
}

static Arcadia_BooleanValue
getOffsetFromBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_SizeValue* offset
  )
{
  // Warning: This assumes valid UTF-8 Byte sequences. However: Arcadia_StringBuilder only holds valid UTF-8 Byte sequences.
  const Arcadia_Natural8Value* end = self->elements;
  const Arcadia_Natural8Value* start = self->elements + self->size;
  const Arcadia_Natural8Value* current = start;

  Arcadia_SizeValue currentCodePointIndex = 0;
  while (currentCodePointIndex != codePointIndex && current != end) {
    current--;
    while (((*current) & 0xC0) == 0x80) {
      current--;
    }
    currentCodePointIndex++;
  }
  *offset = start - current;
  return currentCodePointIndex == codePointIndex;
}

static void
Arcadia_StringBuilder_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  )
{
  Arcadia_EnterConstructor(Arcadia_StringBuilder);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elements = NULL;
  self->size = 0;
  self->capacity = 0;
  self->elements = Arcadia_Memory_allocateUnmanaged(thread, 0);
  Arcadia_LeaveConstructor(Arcadia_StringBuilder);
}

static void
Arcadia_StringBuilder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilderDispatch* self
  )
{ }

static void
Arcadia_StringBuilder_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  )
{
  if (self->elements) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->elements);
    self->elements = NULL;
  }
}

static void
ensureFreeCapacityBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue requiredFreeCapacity
  )
{
  Arcadia_SizeValue availableFreeCapacity = self->capacity - self->size;
  if (availableFreeCapacity < requiredFreeCapacity) {
    Arcadia_SizeValue additionalCapacity = requiredFreeCapacity - availableFreeCapacity;
    Arcadia_SizeValue oldCapacity = self->capacity;
    if (SIZE_MAX - oldCapacity < additionalCapacity) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_SizeValue newCapacity = oldCapacity + additionalCapacity;
    Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->elements, newCapacity);
    self->capacity = newCapacity;
  }
}

static void
prependBytesInternal
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  ensureFreeCapacityBytes(thread, self, numberOfBytes);
  Arcadia_Memory_copy(thread, self->elements + numberOfBytes, self->elements, self->size);
  Arcadia_Memory_copy(thread, self->elements, bytes, numberOfBytes);
  self->size += numberOfBytes;
}

static void
appendBytesInternal
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  ensureFreeCapacityBytes(thread, self, numberOfBytes);
  Arcadia_Memory_copy(thread, self->elements + self->size, bytes, numberOfBytes);
  self->size += numberOfBytes;
}

Arcadia_StringBuilder*
Arcadia_StringBuilder_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_StringBuilder);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_StringBuilder);
}

Arcadia_BooleanValue
Arcadia_StringBuilder_endsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (self->size < numberOfBytes) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_SizeValue d = self->size - numberOfBytes;
  return !Arcadia_Memory_compare(thread, self->elements + d, bytes, numberOfBytes);
}

Arcadia_BooleanValue
Arcadia_StringBuilder_startsWith_pn
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    void const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (self->size < numberOfBytes) {
    return Arcadia_BooleanValue_False;
  }
  return !Arcadia_Memory_compare(thread, self->elements, bytes, numberOfBytes);
}

void
Arcadia_StringBuilder_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue codePointIndex,
    Arcadia_Value value
  )
{
  Arcadia_SizeValue byteIndex = 0;
  if (!getOffsetFromFront(thread, self, codePointIndex, &byteIndex)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // byteIndex is the index at which to insert the elements
  Arcadia_Type* type = Arcadia_Value_getType(thread, &value);
  Arcadia_Natural8Value const* p = NULL;
  Arcadia_SizeValue n = 0;
  if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeUTF8StringValue_getType(thread))) {
    // The Byte sequence of Arcadia.RuntimeUTF8String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_RuntimeUTF8String* object = (Arcadia_RuntimeUTF8String*)Arcadia_Value_getRuntimeUTF8StringValue(&value);
    n = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeUTF8String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeByteArrayValue_getType(thread))) {
    Arcadia_RuntimeByteArray* object = (Arcadia_RuntimeByteArray*)Arcadia_Value_getRuntimeByteArrayValue(&value);
    if (!Arcadia_Unicode_isUTF8(thread, p, n, NULL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
    n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeByteArray_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_ByteArrayBuilder_getType(thread))) {
    Arcadia_ByteArrayBuilder* object = (Arcadia_ByteArrayBuilder*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_ByteArrayBuilder_getBytes(thread, object);
    if (!Arcadia_Unicode_isUTF8(thread, p, n, NULL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_String_getType(thread))) {
    // The Byte sequence of Arcadia.String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_String* object = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_String_getNumberOfBytes(thread, object);
    p = Arcadia_String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_StringBuilder_getType(thread))) {
    // The Byte sequence of Arcadia.StringBuilder is guaranteed to be an UTF8 Byte sequence.
    Arcadia_StringBuilder* object = (Arcadia_StringBuilder*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_StringBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_StringBuilder_getBytes(thread, object);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  ensureFreeCapacityBytes(thread, self, n);
  if (byteIndex < self->size) {
    Arcadia_Memory_copy(thread, self->elements + byteIndex, self->elements + byteIndex + n, self->size - byteIndex);
  }
  Arcadia_Memory_copy(thread, self->elements + byteIndex, p, n);
  self->size += n;
}

void
Arcadia_StringBuilder_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value value
  )
{
  Arcadia_Type* type = Arcadia_Value_getType(thread, &value);
  Arcadia_Natural8Value const* p = NULL;
  Arcadia_SizeValue n = 0;
  if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeUTF8StringValue_getType(thread))) {
    // The Byte sequence of Arcadia.RuntimeUTF8String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_RuntimeUTF8String* object = (Arcadia_RuntimeUTF8String*)Arcadia_Value_getRuntimeUTF8StringValue(&value);
    n = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeUTF8String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeByteArrayValue_getType(thread))) {
    Arcadia_RuntimeByteArray* object = (Arcadia_RuntimeByteArray*)Arcadia_Value_getRuntimeByteArrayValue(&value);
    n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeByteArray_getBytes(thread, object);
    if (!Arcadia_Unicode_isUTF8(thread, p, n, NULL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_ByteArrayBuilder_getType(thread))) {
    Arcadia_ByteArrayBuilder* object = (Arcadia_ByteArrayBuilder*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_ByteArrayBuilder_getBytes(thread, object);
    if (!Arcadia_Unicode_isUTF8(thread, p, n, NULL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_String_getType(thread))) {
    // The Byte sequence of Arcadia.String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_String* object = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_String_getNumberOfBytes(thread, object);
    p = Arcadia_String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_StringBuilder_getType(thread))) {
    // The Byte sequence of Arcadia.StringBuilder is guaranteed to be an UTF8 Byte sequence.
    Arcadia_StringBuilder* object = (Arcadia_StringBuilder*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_StringBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_StringBuilder_getBytes(thread, object);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  ensureFreeCapacityBytes(thread, self, n);
  Arcadia_Memory_copy(thread, self->elements + self->size, p, n);
  self->size += n;
}

void
Arcadia_StringBuilder_insertFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value value
  )
{
  Arcadia_Type* type = Arcadia_Value_getType(thread, &value);
  Arcadia_Natural8Value const* p = NULL;
  Arcadia_SizeValue n = 0;
  if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeUTF8StringValue_getType(thread))) {
    // The Byte sequence of Arcadia.RuntimeUTF8String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_RuntimeUTF8String* object = (Arcadia_RuntimeUTF8String*)Arcadia_Value_getRuntimeUTF8StringValue(&value);
    n = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeUTF8String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeByteArrayValue_getType(thread))) {
    Arcadia_RuntimeByteArray* object = (Arcadia_RuntimeByteArray*)Arcadia_Value_getRuntimeByteArrayValue(&value);
    if (!Arcadia_Unicode_isUTF8(thread, p, n, NULL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
    n = Arcadia_RuntimeByteArray_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeByteArray_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_ByteArrayBuilder_getType(thread))) {
    Arcadia_ByteArrayBuilder* object = (Arcadia_ByteArrayBuilder*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_ByteArrayBuilder_getBytes(thread, object);
    if (!Arcadia_Unicode_isUTF8(thread, p, n, NULL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EncodingInvalid);
      Arcadia_Thread_jump(thread);
    }
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_String_getType(thread))) {
    // The Byte sequence of Arcadia.String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_String* object = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_String_getNumberOfBytes(thread, object);
    p = Arcadia_String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_StringBuilder_getType(thread))) {
    // The Byte sequence of Arcadia.StringBuilder is guaranteed to be an UTF8 Byte sequence.
    Arcadia_StringBuilder* object = (Arcadia_StringBuilder*)Arcadia_Value_getObjectReferenceValue(&value);
    n = Arcadia_StringBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_StringBuilder_getBytes(thread, object);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  ensureFreeCapacityBytes(thread, self, n);
  Arcadia_Memory_copy(thread, self->elements + n, self->elements, self->size);
  Arcadia_Memory_copy(thread, self->elements, p, n);
  self->size += n;
}

void
Arcadia_StringBuilder_insertBackCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value codePoint
  )
{ Arcadia_StringBuilder_insertBackCodePoints(thread, self, &codePoint, 1); }

void
Arcadia_StringBuilder_insertBackCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_Unicode_encodeCodePointsUTF8(thread, codePoints, numberOfCodePoints, self, (Arcadia_Unicode_EncodeCodePointCallbackFunction*)&appendBytesInternal);
}

void
Arcadia_StringBuilder_insertFrontCodePoint
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value codePoint
  )
{ Arcadia_StringBuilder_insertFrontCodePoints(thread, self, &codePoint, 1); }

void
Arcadia_StringBuilder_insertFrontCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_StringBuilder* temporary = Arcadia_StringBuilder_create(thread);
  Arcadia_Unicode_encodeCodePointsUTF8(thread, codePoints, numberOfCodePoints, temporary, (Arcadia_Unicode_EncodeCodePointCallbackFunction*)&appendBytesInternal);
  prependBytesInternal(thread, self, Arcadia_StringBuilder_getBytes(thread, temporary), Arcadia_StringBuilder_getNumberOfBytes(thread, temporary));
}

Arcadia_Integer32Value
Arcadia_StringBuilder_compareTo
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_Value other
  )
{
  Arcadia_Type* type = Arcadia_Value_getType(thread, &other);
  Arcadia_Natural8Value const* p = NULL;
  Arcadia_SizeValue n = 0;
  if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_RuntimeUTF8StringValue_getType(thread))) {
    // The Byte sequence of Arcadia.RuntimeUTF8String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_RuntimeUTF8String* object = (Arcadia_RuntimeUTF8String*)Arcadia_Value_getRuntimeUTF8StringValue(&other);
    n = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, object);
    p = Arcadia_RuntimeUTF8String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_String_getType(thread))) {
    // The Byte sequence of Arcadia.String is guaranteed to be an UTF8 Byte sequence.
    Arcadia_String* object = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&other);
    n = Arcadia_String_getNumberOfBytes(thread, object);
    p = Arcadia_String_getBytes(thread, object);
  } else if (Arcadia_Type_isDescendantType(thread, type, _Arcadia_StringBuilder_getType(thread))) {
    // The Byte sequence of Arcadia.StringBuilder is guaranteed to be an UTF8 Byte sequence.
    Arcadia_StringBuilder* object = (Arcadia_StringBuilder*)Arcadia_Value_getObjectReferenceValue(&other);
    n = Arcadia_StringBuilder_getNumberOfBytes(thread, object);
    p = Arcadia_StringBuilder_getBytes(thread, object);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue k = n <= self->size ? n : self->size;
  Arcadia_Integer32Value result = Arcadia_Memory_compare(thread, self->elements, p, k);
  if (result) {
    return result;
  }
  if (n < self->size) {
    return -1;
  } else if (n > self->size) {
    return +1;
  } else {
    return 0;
  }
}

void
Arcadia_StringBuilder_clear
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  )
{ self->size = 0; }

Arcadia_SizeValue
Arcadia_StringBuilder_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder const* self
  )
{ return self->size; }

Arcadia_SizeValue
Arcadia_StringBuilder_getNumberOfCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self
  )
{
  Arcadia_Value index = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  // It's still linear time, its fine :)
  _Arcadia_UTF8ArrayIterator  it;
  _Arcadia_UTF8ArrayIterator_initialize(thread, &it, self->elements, self->size);
  Arcadia_JumpTarget jt;
  Arcadia_Thread_pushJumpTarget(thread, &jt);
  if (Arcadia_JumpTarget_save(&jt)) {
    while (_Arcadia_UTF8ArrayIterator_hasCodePoint(thread, &it)) {
      _Arcadia_UTF8ArrayIterator_next(thread, &it);
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_SizeValue index = _Arcadia_UTF8ArrayIterator_getNumberOfCodePoints(thread, &it);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
    return index;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    _Arcadia_UTF8ArrayIterator_uninitialize(thread, &it);
    Arcadia_Thread_jump(thread);
  }
}

Arcadia_Natural8Value const*
Arcadia_StringBuilder_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder const* self
  )
{ return self->elements; }

void
Arcadia_StringBuilder_toUpperASCII
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  )
{ 
  Arcadia_Unicode_UTF8_toUpperASCII(thread, self->elements, self->size, index, length);
}

void
Arcadia_StringBuilder_toLowerASCII
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue* index,
    Arcadia_SizeValue* length
  )
{
  Arcadia_Unicode_UTF8_toLowerASCII(thread, self->elements, self->size, index, length);
}

void
Arcadia_StringBuilder_removeCodePointsFront
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_SizeValue offset;
  if (!getOffsetFromFront(thread, self, numberOfCodePoints, &offset)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (offset < self->size) {
    Arcadia_Memory_copy(thread, self->elements, self->elements + offset, self->size - offset);
  }
  self->size -= offset;
}

void
Arcadia_StringBuilder_removeCodePointsBack
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* self,
    Arcadia_SizeValue numberOfCodePoints
  )
{
  Arcadia_SizeValue offset;
  if (!getOffsetFromBack(thread, self, numberOfCodePoints, &offset)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->size -= offset;
}
