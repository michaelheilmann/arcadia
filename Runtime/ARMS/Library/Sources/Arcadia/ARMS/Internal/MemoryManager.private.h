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

#if !defined(ARCADIA_ARMS_MEMORYMANAGER_PRIVATE_H_INCLUDED)
#define ARCADIA_ARMS_MEMORYMANAGER_PRIVATE_H_INCLUDED

#include "Arcadia/ARMS/MemoryManager.h"
#include "Arcadia/ARMS/Configure.h"

typedef enum Arcadia_ARMS_MemoryManagerStartupShutdown_Status {
  Arcadia_ARMS_MemoryManagerStartupShutdown_Status_Success = 0,
  Arcadia_ARMS_MemoryManagerStartupShutdown_Status_ArgumentValueInvalid = 1,
  Arcadia_ARMS_MemoryManagerStartupShutdown_Status_AllocationFailed = 2,
} Arcadia_ARMS_MemoryManagerStartupShutdown_Status;

struct Arcadia_ARMS_MemoryManager {
  Arcadia_ARMS_MemoryManager_Status (*allocate)(Arcadia_ARMS_MemoryManager* self, void** p, size_t n);
  Arcadia_ARMS_MemoryManager_Status (*reallocate)(Arcadia_ARMS_MemoryManager* self, void** p, size_t n);
  Arcadia_ARMS_MemoryManager_Status (*deallocate)(Arcadia_ARMS_MemoryManager* self, void* p);
  Arcadia_ARMS_MemoryManagerStartupShutdown_Status(*destroy)(Arcadia_ARMS_MemoryManager* self);
};

Arcadia_ARMS_MemoryManagerStartupShutdown_Status
Arcadia_ARMS_MemoryManager_destroy
  (
    Arcadia_ARMS_MemoryManager* memoryManager
  );

#endif // ARCADIA_ARMS_MEMORYMANAGER_PRIVATE_H_INCLUDED
