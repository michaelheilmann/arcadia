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
#include "Arcadia/Ring1/Objects/ByteArrayFactory.h"
#include "Arcadia/Ring1/Objects/ByteArrayBorrowedImpl.h"
#include "Arcadia/Ring1/Objects/ByteArrayDefaultImpl.h"
#include "Arcadia/Ring1/Objects/ByteArrayNativeImpl.h"
#include "Arcadia/Ring1/Objects/ByteArraySliceImpl.h"
#include "Arcadia/Ring1/Include.h"

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createImmutable(Arcadia_Thread* thread, Arcadia_RuntimeByteArray* source)
{ return (Arcadia_ByteArray*)Arcadia_ByteArrayDefaultImpl_create(thread, source); }

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createMutable(Arcadia_Thread* thread, Arcadia_SizeValue size)
{ return (Arcadia_ByteArray*)Arcadia_ByteArrayNativeImpl_create(thread, size, Arcadia_BooleanValue_False); }

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createResizable(Arcadia_Thread* thread, Arcadia_SizeValue size)
{ return (Arcadia_ByteArray*)Arcadia_ByteArrayNativeImpl_create(thread, size, Arcadia_BooleanValue_True); }

// These factory variants borrow external native storage; they never free it.
Arcadia_ByteArray*
Arcadia_ByteArrayFactory_borrowReadOnly(Arcadia_Thread* thread,
                                        Arcadia_Natural8Value const* bytes, Arcadia_SizeValue size)
{
  return (Arcadia_ByteArray*)Arcadia_ByteArrayBorrowedImpl_create(thread, bytes, size, Arcadia_BooleanValue_False);
}

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_borrowMutable(Arcadia_Thread* thread,
                                       Arcadia_Natural8Value* bytes, Arcadia_SizeValue size)
{
  return (Arcadia_ByteArray*)Arcadia_ByteArrayBorrowedImpl_create(thread, bytes, size, Arcadia_BooleanValue_True);
}

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_createSlice(Arcadia_Thread* thread, Arcadia_ByteArray* source,
                                     Arcadia_SizeValue start, Arcadia_SizeValue size)
{
  if (!source) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_ByteArray*)Arcadia_ByteArraySliceImpl_create(thread, source, start, size);
}

static Arcadia_ByteArray*
copyNative(Arcadia_Thread* thread, Arcadia_ByteArray* source, Arcadia_BooleanValue resizable)
{
  if (!source || !Arcadia_ByteArray_isContiguous(thread, source)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Keep both arrays alive: the virtual source getter may allocate and collect.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_SizeValue size = Arcadia_ByteArray_getSize(thread, source);
  Arcadia_ByteArrayNativeImpl* target = Arcadia_ByteArrayNativeImpl_create(thread, size, resizable);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  if (size) {
    Arcadia_Memory_copy(thread, target->bytes, Arcadia_ByteArray_getBytes(thread, source), size);
  }
  Arcadia_ValueStack_popValues(thread, 2);
  return (Arcadia_ByteArray*)target;
}

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_copyMutable(Arcadia_Thread* thread, Arcadia_ByteArray* source)
{ return copyNative(thread, source, Arcadia_BooleanValue_False); }

Arcadia_ByteArray*
Arcadia_ByteArrayFactory_copyResizable(Arcadia_Thread* thread, Arcadia_ByteArray* source)
{ return copyNative(thread, source, Arcadia_BooleanValue_True); }

// An immutable snapshot never aliases storage whose contents may later change.
// The source stays rooted across the runtime-value allocation and wrapper creation.
Arcadia_ByteArray*
Arcadia_ByteArrayFactory_copyImmutable(Arcadia_Thread* thread, Arcadia_ByteArray* source)
{
  if (!source || !Arcadia_ByteArray_isContiguous(thread, source)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_SizeValue size = Arcadia_ByteArray_getSize(thread, source);
  Arcadia_RuntimeByteArray* value = Arcadia_RuntimeByteArray_create(thread,
                                                    Arcadia_ByteArray_getBytes(thread, source), size);
  Arcadia_ValueStack_pushRuntimeByteArrayValue(thread, value);
  Arcadia_ByteArray* target = Arcadia_ByteArrayFactory_createImmutable(thread, value);
  Arcadia_ValueStack_popValues(thread, 2);
  return target;
}
