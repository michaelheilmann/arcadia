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
#include "Arcadia/Ring1/Implementation/Arrays.h"

#include "Arcadia/Ring1/Implementation/Thread.h"

void
Arcadia_Arrays_resizeByAdditionalCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ARMS_MemoryManager* memoryManager,
    void** elements,
    Arcadia_SizeValue elementSize,
    Arcadia_SizeValue size,
    Arcadia_SizeValue* capacity,
    Arcadia_SizeValue requiredAdditionalCapacity,
    Arcadia_Arrays_ResizeStrategy strategy
  )
{
  if (!elements || !elementSize || !capacity || size > (*capacity)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue oldCapacity = *capacity;
  Arcadia_SizeValue newCapacity;
  Arcadia_SizeValue const maximalCapacity = Arcadia_SizeValue_Maximum / elementSize;
  switch (strategy) {
    case Arcadia_Arrays_ResizeStrategy_Type1: {
      if (maximalCapacity - oldCapacity < requiredAdditionalCapacity) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
        Arcadia_Thread_jump(thread);
      }
      newCapacity = oldCapacity + requiredAdditionalCapacity;
      if (Arcadia_ARMS_MemoryManager_reallocate(memoryManager, elements, newCapacity * elementSize)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      *capacity = newCapacity;
    } break;
    case Arcadia_Arrays_ResizeStrategy_Type2: {
      Arcadia_SizeValue currentNewCapacity = oldCapacity ? oldCapacity : 1; // 1 = 2^n with n = 0
      Arcadia_SizeValue currentAdditionalCapacity = currentNewCapacity - oldCapacity;
      while (currentAdditionalCapacity < requiredAdditionalCapacity && currentNewCapacity <= maximalCapacity / 2) {
        currentNewCapacity *= 2;
        currentAdditionalCapacity = currentNewCapacity - oldCapacity;
      }
      if (currentAdditionalCapacity < requiredAdditionalCapacity) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
        Arcadia_Thread_jump(thread);
      }
      newCapacity = currentNewCapacity;
      if (Arcadia_ARMS_MemoryManager_reallocate(memoryManager, elements, newCapacity * elementSize)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      *capacity = newCapacity;
    } break;
    case Arcadia_Arrays_ResizeStrategy_Type3: {
      Arcadia_SizeValue newCapacity = maximalCapacity;
      if (requiredAdditionalCapacity > newCapacity - oldCapacity) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists);
        Arcadia_Thread_jump(thread);
      }
      if (Arcadia_ARMS_MemoryManager_reallocate(memoryManager, elements, newCapacity * elementSize)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      }
      *capacity = newCapacity;
    } break;
    case Arcadia_Arrays_ResizeStrategy_Type4: {
      Arcadia_JumpTarget jumpTarget;
      Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
      if (Arcadia_JumpTarget_save(&jumpTarget)) {
        Arcadia_Arrays_resizeByAdditionalCapacity(thread, memoryManager, elements, elementSize, size, capacity, requiredAdditionalCapacity, Arcadia_Arrays_ResizeStrategy_Type2);
        Arcadia_Thread_popJumpTarget(thread);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Arrays_resizeByAdditionalCapacity(thread, memoryManager, elements, elementSize, size, capacity, requiredAdditionalCapacity, Arcadia_Arrays_ResizeStrategy_Type3);
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

void
Arcadia_Arrays_resizeByFreeCapacity
  (
    Arcadia_Thread* thread,
    Arcadia_ARMS_MemoryManager* memoryManager,
    void** elements,
    Arcadia_SizeValue elementSize,
    Arcadia_SizeValue size,
    Arcadia_SizeValue* capacity,
    Arcadia_SizeValue requiredFreeCapacity,
    Arcadia_Arrays_ResizeStrategy strategy
  )
{
  if (!capacity || size > (*capacity)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue availableFreeCapacity = (*capacity) - size;
  if (availableFreeCapacity < requiredFreeCapacity) {
    Arcadia_Arrays_resizeByAdditionalCapacity(thread, memoryManager, elements, elementSize, size, capacity, requiredFreeCapacity - availableFreeCapacity, strategy);
  }
}
