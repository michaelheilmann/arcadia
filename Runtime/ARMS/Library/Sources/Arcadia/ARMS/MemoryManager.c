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

#include "Arcadia/ARMS/Internal/MemoryManager.private.h"

Arcadia_ARMS_MemoryManager_Status
Arcadia_ARMS_MemoryManager_allocate
  (
    Arcadia_ARMS_MemoryManager* memoryManager,
    void** p,
    Arcadia_ARMS_Size n
  )
{
  if (!memoryManager) {
    return Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid;
  }
  return memoryManager->allocate(memoryManager, p, n);
}

Arcadia_ARMS_MemoryManager_Status
Arcadia_ARMS_MemoryManager_reallocate
  (
    Arcadia_ARMS_MemoryManager* memoryManager,
    void** p,
    Arcadia_ARMS_Size n
  )
{
  if (!memoryManager) {
    return Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid;
  }
  return memoryManager->reallocate(memoryManager, p, n);
}

Arcadia_ARMS_MemoryManager_Status
Arcadia_ARMS_MemoryManager_deallocate
  (
    Arcadia_ARMS_MemoryManager* memoryManager,
    void* p
  )
{
  if (!memoryManager) {
    return Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid;
  }
  return memoryManager->deallocate(memoryManager, p);
}
