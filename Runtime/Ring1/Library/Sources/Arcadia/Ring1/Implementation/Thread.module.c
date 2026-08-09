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
#include "Arcadia/Ring1/Implementation/Thread.module.h"

#include "Arcadia/ARMS/Include.h"

void
Arcadia_Thread_initializeValueStack
  (
    Arcadia_Thread* thread
  )
{
  if (Arcadia_ARMS_MemoryManager_allocate(Arcadia_ARMS_getDefaultMemoryManager(), (void**)&thread->stack.elements, sizeof(Arcadia_Value) * 8)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 8; ++i) {
    thread->stack.elements[i] = Arcadia_Value_Initializer();
  }
  thread->stack.size = 0;
  thread->stack.capacity = 8;
}

void
Arcadia_Thread_uninitializeValueStack
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ARMS_MemoryManager_deallocate(Arcadia_ARMS_getDefaultMemoryManager(), thread->stack.elements);
  thread->stack.elements = NULL;
}

void
Arcadia_Thread_initialize
  (
    Arcadia_Thread* thread
  )
{
  thread->process = NULL;
  thread->status = Arcadia_Status_Success;
  thread->jumpTarget = NULL;
  thread->raisedValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  Arcadia_Thread_initializeValueStack(thread);
}

void
Arcadia_Thread_uninitialize
  (
    Arcadia_Thread* thread
  )
{
  thread->raisedValue = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
  Arcadia_Thread_uninitializeValueStack(thread);
  thread->jumpTarget = NULL;
  thread->status = Arcadia_Status_Success;
  thread->process = NULL;
}

void
Arcadia_Thread_increaseValueStackCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue additionalCapacity
  )
{
  static const Arcadia_SizeValue maximumCapacity = Arcadia_SizeValue_Maximum / sizeof(Arcadia_Value);
  static const Arcadia_SizeValue desiredAdditionalCapacity = 64;
  Arcadia_SizeValue oldCapacity = thread->stack.capacity;
  if (maximumCapacity - oldCapacity < additionalCapacity) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue maximumAdditionalCapacity = maximumCapacity - oldCapacity;
  // We want to grow in steps of desiredAdditionalCapacity.
  if (additionalCapacity < desiredAdditionalCapacity && maximumAdditionalCapacity >= desiredAdditionalCapacity) {
    additionalCapacity = desiredAdditionalCapacity;
  }

  Arcadia_SizeValue newCapacity = oldCapacity + additionalCapacity;
  if (Arcadia_ARMS_MemoryManager_reallocate(Arcadia_ARMS_getDefaultMemoryManager(), (void**)&thread->stack.elements, newCapacity * sizeof(Arcadia_Value))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = oldCapacity; i < newCapacity; ++i) {
    thread->stack.elements[i] = Arcadia_Value_Initializer();
  }
  thread->stack.capacity = newCapacity;
}

void
Arcadia_Thread_ensureValueStackFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue requiredFreeCapacity
  )
{
  Arcadia_SizeValue availableFreeCapacity = thread->stack.capacity - thread->stack.size;
  if (availableFreeCapacity < requiredFreeCapacity) {
    Arcadia_Thread_increaseValueStackCapacity(thread, requiredFreeCapacity - availableFreeCapacity);
  }
}
