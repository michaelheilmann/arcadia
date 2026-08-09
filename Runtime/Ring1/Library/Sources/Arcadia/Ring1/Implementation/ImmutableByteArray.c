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
#include "Arcadia/Ring1/Implementation/ImmutableByteArray.h"

#include "Arcadia/Ring1/Include.h"

#define TypeName u8"Arcadia.ImmutableByteArray"

static Arcadia_BooleanValue g_registered = Arcadia_BooleanValue_False;

static void
onTypeRemoved
  (
    Arcadia_Process* process,
    const uint8_t* bytes,
    size_t numberOfBytes
  )
{ g_registered = Arcadia_BooleanValue_False; }

Arcadia_RuntimeByteArray*
Arcadia_RuntimeByteArray_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  if (!bytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (SIZE_MAX - sizeof(Arcadia_RuntimeByteArray) < numberOfBytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!g_registered) {
    Arcadia_Process_registerType(Arcadia_Thread_getProcess(thread),
                                 TypeName, sizeof(TypeName) - 1,
                                 Arcadia_Thread_getProcess(thread),
                                 (Arcadia_Process_TypeRemovedCallback*)&onTypeRemoved,
                                 NULL,
                                 NULL);
    g_registered = Arcadia_BooleanValue_True;
  }
  Arcadia_RuntimeByteArray*array = NULL;
  Arcadia_Process_allocate(Arcadia_Thread_getProcess(thread), (void**)&array, TypeName, sizeof(TypeName) - 1, sizeof(Arcadia_RuntimeByteArray) + numberOfBytes);
  Arcadia_Memory_copy(thread, array->bytes, bytes, numberOfBytes);
  array->numberOfBytes = numberOfBytes;
  return array;
}

void
Arcadia_RuntimeByteArray_visit
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArrayValue self
  )
{ Arcadia_Process_visitObject(Arcadia_Thread_getProcess(thread), self); }

#if defined(Arcadia_ARMS_Configuration_WithBarriers) && 1 == Arcadia_ARMS_Configuration_WithBarriers

void
Arcadia_RuntimeByteArray_ensureGray
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArrayValue self
  )
{ /*Arcadia_Process_ensureGray(Arcadia_Thread_getProcess(thread), self);*/ }

#endif

Arcadia_Natural8Value const*
Arcadia_RuntimeByteArray_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArrayValue self
  )
{ return self->bytes; }

Arcadia_SizeValue
Arcadia_RuntimeByteArray_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArrayValue self
  )
{ return self->numberOfBytes; }

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
  if (Arcadia_Value_isRuntimeByteArrayValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getRuntimeByteArrayValue(&x) == Arcadia_Value_getRuntimeByteArrayValue(&y));
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
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)(uintptr_t)Arcadia_Value_getRuntimeByteArrayValue(&x));
}

static void
isNotEqualTo
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  if (Arcadia_Value_isRuntimeByteArrayValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_Value_getRuntimeByteArrayValue(&x) != Arcadia_Value_getRuntimeByteArrayValue(&y));
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
_Arcadia_RuntimeByteArrayValue_getType
  (
    Arcadia_Thread* thread
  )
{
  if (!g_type) {
    g_type = Arcadia_registerInternalType(thread, Arcadia_Names_getOrCreateName(thread, TypeName, sizeof(TypeName) - 1), &_typeOperations, &typeDestructing);
  }
  return g_type;
}
