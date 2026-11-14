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

#if !defined(ARCADIA_RING1_OBJECTS_BYTEARRAY_H_INCLUDED)
#define ARCADIA_RING1_OBJECTS_BYTEARRAY_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Object.h"

/// @code
/// class Arcadia.ByteArray {
///   construct(runtimeByteArray:Arcadia.RuntimeByteArray)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.ByteArray", Arcadia_ByteArray,
                          u8"Arcadia.Object");

struct Arcadia_ByteArrayDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_Natural8Value
  (*getAt)
    (
      Arcadia_Thread* thread,
      Arcadia_ByteArray const* self,
      Arcadia_SizeValue index
    );

  Arcadia_Natural8Value const*
  (*getBytes)
    (
      Arcadia_Thread* thread,
      Arcadia_ByteArray const* self
    );

  // Native C-only borrowed mutable pointer; implementations without writable
  // contiguous storage raise OperationInvalid rather than returning a pointer.
  Arcadia_Natural8Value* (*getMutableBytes)(Arcadia_Thread* thread, Arcadia_ByteArray* self);

  Arcadia_SizeValue
  (*getNumberOfBytes)
    (
      Arcadia_Thread* thread,
      Arcadia_ByteArray const* self
    );

  Arcadia_SizeValue
  (*getSize)
    (
      Arcadia_Thread* thread,
      Arcadia_ByteArray const* self
    );

  // Capability queries are side-effect-free and stable for each instance.
  Arcadia_BooleanValue (*isMutable)(Arcadia_Thread* thread, Arcadia_ByteArray const* self);
  Arcadia_BooleanValue (*isResizable)(Arcadia_Thread* thread, Arcadia_ByteArray const* self);
  Arcadia_BooleanValue (*isContiguous)(Arcadia_Thread* thread, Arcadia_ByteArray const* self);

  void (*setAt)(Arcadia_Thread* thread, Arcadia_ByteArray* self,
                Arcadia_SizeValue index, Arcadia_Natural8Value value);
  void (*resize)(Arcadia_Thread* thread, Arcadia_ByteArray* self,
                 Arcadia_SizeValue size);
  // Bulk insertion is supported by resizable native-backed implementations.
  void (*insertBytes)(Arcadia_Thread* thread, Arcadia_ByteArray* self,
                      Arcadia_SizeValue index, void const* bytes, Arcadia_SizeValue numberOfBytes);

  Arcadia_BooleanValue
  (*isEmpty)
    (
      Arcadia_Thread* thread,
      Arcadia_ByteArray const* self
    );

};

struct Arcadia_ByteArray {
  Arcadia_Object _parent;
};

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_getAt
Arcadia_Natural8Value
Arcadia_ByteArray_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self,
    Arcadia_SizeValue index
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_getBytes
Arcadia_Natural8Value const*
Arcadia_ByteArray_getBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_getMutableBytes
// Native C-only. Requires mutable and contiguous storage. The returned pointer
// is borrowed and can be invalidated by resizing; root the owner while using it.
// Unsupported calls raise Arcadia_Status_OperationInvalid.
Arcadia_Natural8Value*
Arcadia_ByteArray_getMutableBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_getNumberOfBytes
Arcadia_SizeValue
Arcadia_ByteArray_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_getSize
Arcadia_SizeValue
Arcadia_ByteArray_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_isEmpty
Arcadia_BooleanValue
Arcadia_ByteArray_isEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_isMutable
Arcadia_BooleanValue
Arcadia_ByteArray_isMutable
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_isResizable
Arcadia_BooleanValue
Arcadia_ByteArray_isResizable
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_isContiguous
Arcadia_BooleanValue
Arcadia_ByteArray_isContiguous
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray const* self
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_setAt
void
Arcadia_ByteArray_setAt
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* self,
    Arcadia_SizeValue index,
    Arcadia_Natural8Value value
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_resize
void
Arcadia_ByteArray_resize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* self,
    Arcadia_SizeValue size
  );

// Insert a native byte span into mutable, resizable storage.
// Self-insertion is supported, even if resize relocates the allocation.
// A nonzero source span must be valid; invalid operations raise a status.
void
Arcadia_ByteArray_insertBytes(Arcadia_Thread* thread, Arcadia_ByteArray* self,
                              Arcadia_SizeValue index, void const* bytes, Arcadia_SizeValue numberOfBytes);

void
Arcadia_ByteArray_appendBytes(Arcadia_Thread* thread, Arcadia_ByteArray* self,
                              void const* bytes, Arcadia_SizeValue numberOfBytes);

void
Arcadia_ByteArray_prependBytes(Arcadia_Thread* thread, Arcadia_ByteArray* self,
                               void const* bytes, Arcadia_SizeValue numberOfBytes);

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_slice
Arcadia_ByteArray*
Arcadia_ByteArray_slice
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* self,
    Arcadia_SizeValue start,
    Arcadia_SizeValue size
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ByteArray_createByteArray
Arcadia_ByteArray*
Arcadia_ByteArray_createByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeByteArray* runtimeByteArray
  );

#endif // ARCADIA_RING1_OBJECTS_BYTEARRAY_H_INCLUDED
