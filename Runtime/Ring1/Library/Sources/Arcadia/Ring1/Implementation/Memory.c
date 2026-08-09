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
#include "Arcadia/Ring1/Implementation/Memory.h"

#include "Arcadia/ARMS/Include.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"
#include "Arcadia/Ring1/Implementation/Thread.module.h"
#include <stdbool.h>
#include <string.h>

void
Arcadia_Memory_fill
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n,
    uint8_t v
  )
{
  Arcadia_StaticAssert(UINTPTR_MAX == SIZE_MAX, "environment not (yet) supported");

  if (UINTPTR_MAX - n < ((uintptr_t)p)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  memset(p, v, n);
}

void
Arcadia_Memory_fillZero
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  )
{
  Arcadia_StaticAssert(UINTPTR_MAX == SIZE_MAX, "environment not (yet) supported");

  if (UINTPTR_MAX - n < ((uintptr_t)p)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  memset(p, UINT8_C(0), n);
}

void
Arcadia_Memory_copy
  (
    Arcadia_Thread* thread,
    void* p,
    const void* q,
    size_t n
  )
{
  Arcadia_StaticAssert(UINTPTR_MAX == SIZE_MAX, "environment not (yet) supported");

  if (UINTPTR_MAX - n < ((uintptr_t)p) || UINTPTR_MAX - n < ((uintptr_t)q)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Determine if the intervals a = [a.start, a.end] and b = [b.start, b.end] DO overlap.
  //
  // They DO NOT overlap if condition [1] or condition [2] is fulfilled:
  // [1] a is completely behind b that is a.start > b.end.
  // [2] b is completely behind a that is b.start > a.end
  // That is, they do not overlap if
  // ([1] OR [1])
  // holds.
  // Consequently, a and b DO overlap if
  // NOT ([1] OR [2])
  // holds which can be rewritten
  //     NOT ([1] OR [2])
  // <=> NOT [1] AND NOT [2]
  // <=> NOT (a.start > b.end) AND NOT (b.start > a.end)
  // <=> a.start <= b.end AND b.start <= a.end
  //
  // Now let s.start = ((uintptr_t)p), a.end = ((uintptr_t)p) + n, b.start = ((uintptr_t const)q), b.end = ((uintptr_t const)q) + n to obtain
  bool overlapping = (((uintptr_t)p) <= ((uintptr_t const)q) + n)
                  && (((uintptr_t const)q) <= ((uintptr_t)p) + n);
  if (overlapping) {
    memmove(p, q, n);
  } else {
    memcpy(p, q, n);
  }
}

int8_t
Arcadia_Memory_compare
  (
    Arcadia_Thread* thread,
    const void* p,
    const void* q,
    size_t n
  )
{
  int t = memcmp(p, q, n);
  if (t < 0) return INT8_C(- 1);
  else if (t > 0) return INT8_C(+1);
  else return INT8_C(0);
}

void*
Arcadia_Memory_allocateUnmanaged
  (
    Arcadia_Thread* thread,
    size_t n
  )
{
  void* q = NULL;
  Arcadia_ARMS_MemoryManager_Status status = Arcadia_ARMS_MemoryManager_allocate(Arcadia_ARMS_getDefaultMemoryManager(), &q, n);
  if (status) {
    if (status == Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    } else if (status == Arcadia_ARMS_MemoryManager_Status_AllocationFailed) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed); /*@todo As ARMs behaves incorrectly, we should use Arcadia_Status_EnvironmentInvalid.*/
    }
    Arcadia_Thread_jump(thread);
  }
  return q;
}

void
Arcadia_Memory_deallocateUnmanaged
  (
    Arcadia_Thread* thread,
    void* p
  )
{
  Arcadia_ARMS_MemoryManager_Status status = Arcadia_ARMS_MemoryManager_deallocate(Arcadia_ARMS_getDefaultMemoryManager(), p);
  if (status) {
    if (status == Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed); /*@todo As ARMs behaves incorrectly, we should use Arcadia_Status_EnvironmentInvalid.*/
    }
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Memory_reallocateUnmanaged
  (
    Arcadia_Thread* thread,
    void** p,
    size_t n
  )
{
  Arcadia_ARMS_MemoryManager_Status status = Arcadia_ARMS_MemoryManager_reallocate(Arcadia_ARMS_getDefaultMemoryManager(), p, n);
  if (status) {
    if (status == Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    } else if (status == Arcadia_ARMS_MemoryManager_Status_AllocationFailed) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed); /*@todo As ARMs behaves incorrectly, we should use Arcadia_Status_EnvironmentInvalid.*/
    }
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Memory_reverseMemory16
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  )
{
  if (n) {
    Arcadia_Natural16Value* front = p;
    Arcadia_Natural16Value* back = front + (n - 1);
    for (Arcadia_SizeValue i = 0, m = n / 2; i < m; ++i) {
      Arcadia_Natural16Value temporary = *front;
      *front = *back;
      *back = temporary;
      ++front;
      --back;
    }
  }
}

void
Arcadia_Memory_reverseMemory32
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  )
{
  if (n) {
    Arcadia_Natural32Value* front = p;
    Arcadia_Natural32Value* back = front + (n - 1);
    for (Arcadia_SizeValue i = 0, m = n / 2; i < m; ++i) {
      Arcadia_Natural32Value temporary = *front;
      *front = *back;
      *back = temporary;
      ++front;
      --back;
    }
  }
}

void
Arcadia_Memory_reverseMemory64
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  )
{
  if (n) {
    Arcadia_Natural64Value* front = p;
    Arcadia_Natural64Value* back = front + (n - 1);
    for (Arcadia_SizeValue i = 0, m = n / 2; i < m; ++i) {
      Arcadia_Natural64Value temporary = *front;
      *front = *back;
      *back = temporary;
      ++front;
      --back;
    }
  }
}

void
Arcadia_Memory_reverseMemory8
  (
    Arcadia_Thread* thread,
    void* p,
    size_t n
  )
{
  if (n) {
    Arcadia_Natural8Value* front = p;
    Arcadia_Natural8Value* back = front + (n - 1);
    for (Arcadia_SizeValue i = 0, m = n / 2; i < m; ++i) {
      Arcadia_Natural8Value temporary = *front;
      *front = *back;
      *back = temporary;
      ++front;
      --back;
    }
  }
}

void
Arcadia_Memory_swap
  (
    Arcadia_Thread* thread,
    void* p,
    void* q,
    size_t n
  )
{
  Arcadia_Natural8Value* p0 = (Arcadia_Natural8Value*)p,
                       * q0 = (Arcadia_Natural8Value*)q;
  for (size_t i = 0; i < n; ++i) {
    Arcadia_Natural8Value temporary = *p0;
    *p0 = *q0;
    *q0 = temporary;
    p0++;
    q0++;
  }
}

bool
Arcadia_Memory_startsWith
  (
    Arcadia_Thread* thread,
    void const* p,
    size_t n,
    void const* q,
    size_t m
  )
{
  if (0 == m) {
    return true;
  }
  // If haystack is shorter than needle, then the former cannot start with the latter.
  if (n < m) {
    return false;
  }
  return !memcmp(p, q, m);
}

bool
Arcadia_Memory_endsWith
  (
    Arcadia_Thread* thread,
    void const* p,
    size_t n,
    void const* q,
    size_t m
  )
{
  if (0 == m) {
    return true;
  }
  // If haystack is shorter than needle, then the former cannot end with the latter.
  if (n < m) {
    return false;
  }
  return !memcmp(((uint8_t const*)p) + n - m, q, m);
}
