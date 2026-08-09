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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_MEMORY_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_MEMORY_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct Arcadia_Thread Arcadia_Thread; // Forward declaration.

/// @brief
/// Assign the Bytes of a memory block the specified Byte value.
///
/// @param thread
/// A pointer to this thread.
///
/// @param p
/// A pointer to the memory block.
///
/// @param n
/// The number of Bytes of the memory block pointed to by @a p.
///
/// @param v
/// The value to assign to the Bytes of the Bytes of the memory block.
///
/// @remarks
/// @a p can be a null pointer
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
///
/// @error Arcadia_Status_ArgumentValueInvalid
/// p + n overflows
void
Arcadia_Memory_fill
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n,
    uint8_t v
  );

/// @brief
/// Assign the Bytes of a memory block the Byte value @a 0.
///
/// @param thread
/// A pointer to this thread.
///
/// @param p
/// A pointer to the memory block.
///
/// @param n
/// The number of Bytes of the memory block pointed to by @a p.
///
/// @remarks
/// @a p can be a null pointer
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
///
/// @error Arcadia_Status_ArgumentValueInvalid
/// p + n overflows
void
Arcadia_Memory_fillZero
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  );

/// @brief
/// Copy the contents of a memory region to another memory region.
///
/// @param process
/// A pointer to the Arcadia_Process object
///
/// @param p
/// The starting address of the target memory region
///
/// @param q
/// The starting address of the source memory region
///
/// @param n
/// The size, in Bytes, of the memory regions
///
/// @remarks
/// The memory regions may overlap.
///
/// @error Arcadia_Status_ArgumentValueInvalid
/// p + n overflows
///
/// @error Arcadia_Status_ArgumentValueInvalid
/// q + n overflows
///
/// @remarks
/// @a p and/or q can be a null pointers
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
void
Arcadia_Memory_copy
  (
    Arcadia_Thread* thread,
    void* p,
    const void* q,
    size_t n
  );

/// @brief
/// Allocate unmanaged memory.
/// @param process
/// A pointer to the Arcadia_Process object
/// @param p
/// A pointer to a <code>void*</code> variable.
/// @param n
/// The size, in Bytes, of the memory region to allocate.
/// @post
/// On success, <code>*p</code> was assigned a pointer to a memory region of @a n Bytes.
/// @error #Arcadia_Status_ArgumentValueInvalid
/// @a p is a null pointer
/// @error #Arcadia_Status_AllocationFailed
/// the allocation failed
void*
Arcadia_Memory_allocateUnmanaged
  (
    Arcadia_Thread* thread,
    size_t n
  );

/// @brief
/// Deallocate unmanaged memory.
///
/// @param thread
/// A pointer to the Arcadia_Thread object
///
/// @param p
/// A pointer to a memory region of @a n Bytes.
///
/// @error #Arcadia_Status_ArgumentValueInvalid
/// @a p is a null pointer
void
Arcadia_Memory_deallocateUnmanaged
  (
    Arcadia_Thread* thread,
    void* p
  );

/// @brief
/// Reallocate unmanaged memory.
///
/// @param thread
/// A pointer to this Arcadia_Thread object
/// @param p A pointer to a <code>void*</code> variable.
/// The variable points to the memory region to reallocate.
///
/// @param n
/// The size to reallocate the memory region to.
///
/// @post
/// On success, <code>*p</code> was assigned a pointer to a new memory region of @a n Bytes. The old memory region was deallocated.
///
/// @error #Arcadia_Status_ArgumentValueInvalid
/// @a p is a null pointer
///
/// @error #Arcadia_Status_ArgumentValueInvalid
/// @a *p is a null pointer
///
/// @error #Arcadia_Status_AllocationFailed
/// the allocation failed
void
Arcadia_Memory_reallocateUnmanaged
  (
    Arcadia_Thread* thread,
    void** p,
    size_t n
  );

/// p pointer to an array with elements a[0], ..., a[n]. Each element is 16 bit wide.
/// These value of the array are swapped such that a[i] = a[n - 1 - i] holds for all 0 <= i < n.
void
Arcadia_Process_reverseMemory16
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  );

/// p pointer to an array with elements a[0], ..., a[n]. Each element is 32 bit wide.
/// These value of the array are swapped such that a[i] = a[n - 1 - i] holds for all 0 <= i < n.
void
Arcadia_Memory_reverseMemory32
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  );

/// p pointer to an array with elements a[0], ..., a[n]. Each element is 64 bit wide.
/// These value of the array are swapped such that a[i] = a[n - 1 - i] holds for all 0 <= i < n.
void
Arcadia_Memory_reverseMemory64
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  );

/// p pointer to an array with elements a[0], ..., a[n]. Each element is 8 bit wide.
/// These value of the array are swapped such that a[i] = a[n - 1 - i] holds for all 0 <= i < n.
void
Arcadia_Memory_reverseMemory8
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  );

/// @brief
/// Swap the values of teo memory regions.
///
/// @param p
/// A pointer to the first memory region.
///
/// @param q
/// A pointer to the second memory region.
///
/// @param n
/// The number of Bytes of the memory regions pointed to by @a p and @a q, respectively.
///
/// @remarks
/// @a p and/or q can be a null pointers
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
void
Arcadia_Memory_swap
  (
    Arcadia_Thread* thread,
    void* p,
    void* q,
    size_t n
  );

/// @brief
/// Compare the values of two memory regions.
///
/// @param p
/// A pointer to the first memory region.
///
/// @param q
/// A pointer to the second memory region.
///
/// @param n
/// The number of Bytes of the memory regions pointed to by @a p and @a q, respectively.
///
/// @return
/// @a -1 if the first memory region is smaller than the second memory region.
/// @a +1 if the first memory region is greater than the second memory region.
/// @a  0 if the memory regions are equal.
///
/// @remarks
/// Comparison is performed by comparing the Bytes @a i-th Byte in the first memory region with the i-th Byte of the second memory region.
/// If all Bytes are equal, the memory regions are equal.
/// Otherwise there exist at least one pair of Bytes which are not equal.
/// The pair at the least index @a j is considered.
/// The first memory region is smaller than the second memory region if the Byte of that pair in the first memory region is smaller than the Byte in the second memory region.
/// The first memory region is greater than the second memory region if the Byte of that pair in the first memory region is greater than the Byte in the second memory region.
///
/// @remarks
/// @a p and/or q can be a null pointers
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
int8_t
Arcadia_Memory_compare
  (
    Arcadia_Thread* thread,
    const void* p,
    const void* q,
    size_t n
  );

/// @brief
/// Get if the sequence of values of an array are the prefix of the sequence values of another array.
///
/// @param p
/// A pointer to an array of @a n Bytes.
///
/// @param n
/// The number of Bytes in the array pointed to by @a p.
///
/// @param q
/// A pointer to an array of @a m Bytes.
///
/// @param m
/// The number of Bytes in the array pointed to by @a q.
///
/// @return
/// @a true if the sequence of values of the array pointed to by @a q are the prefix of the sequence of values of the array pointed to by @a p.
/// @a false otherwise.
///
/// @remarks
/// @a p and/or q can be a null pointers
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
bool
Arcadia_Memory_startsWith
  (
    Arcadia_Thread* thread,
    void const* p,
    size_t n,
    void const* q,
    size_t m
  );

/// @brief
/// Get if the sequence of values of an array are the suffix of the sequence values of another array.
///
/// @param p
/// A pointer to an array of @a n Bytes.
///
/// @param n
/// The number of Bytes in the array pointed to by @a p.
///
/// @param q
/// A pointer to an array of @a m Bytes.
///
/// @param m
/// The number of Bytes in the array pointed to by @a q.
///
/// @return
/// @a true if the sequence of values of the array pointed to by @a q are the suffix of the sequence of values of the array pointed to by @a p.
/// @a false otherwise.
///
/// @remarks
/// @a p and/or q can be a null pointers
///
/// @remarks
/// Only the specified errors under the specified conditions are raised by calls to this function.
bool
Arcadia_Memory_endsWith
  (
    Arcadia_Thread* thread,
    void const* p,
    size_t n,
    void const* q,
    size_t m
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_MEMORY_H_INCLUDED
