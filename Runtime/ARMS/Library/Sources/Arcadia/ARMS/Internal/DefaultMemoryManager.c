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

#include "Arcadia/ARMS/Internal/DefaultMemoryManager.h"

#include "Arcadia/ARMS/Include.h"

struct Arcadia_ARMS_DefaultMemoryManager {
  Arcadia_ARMS_MemoryManager parent;
};

static Arcadia_ARMS_MemoryManager_Status
allocate
  (
    Arcadia_ARMS_DefaultMemoryManager* self,
    void** p,
    Arcadia_ARMS_Size n
  )
{
  if (!p) {
    return Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid;
  }
  void* q = malloc(n > 0 ? n : 1);
  if (!q) {
    return Arcadia_ARMS_MemoryManager_Status_AllocationFailed;
  }
  *p = q;
  return Arcadia_ARMS_MemoryManager_Status_Success;
}

static Arcadia_ARMS_MemoryManager_Status
reallocate
  (
    Arcadia_ARMS_DefaultMemoryManager* self,
    void** p,
    Arcadia_ARMS_Size n
  )
{
  if (!p) {
    return Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid;
  }
  void* q = realloc(*p, n > 0 ? n : 1);
  if (!q) {
    return Arcadia_ARMS_MemoryManager_Status_AllocationFailed;
  }
  *p = q;
  return Arcadia_ARMS_MemoryManager_Status_Success;
}

static Arcadia_ARMS_MemoryManager_Status
deallocate
  (
    Arcadia_ARMS_DefaultMemoryManager* self,
    void* p
  )
{
  if (!p) {
    return Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid;
  }
  free(p);
  return Arcadia_ARMS_MemoryManager_Status_Success;
}

static Arcadia_ARMS_MemoryManagerStartupShutdown_Status
destroy
  (
    Arcadia_ARMS_DefaultMemoryManager* self
  )
{
  free(self);
  return Arcadia_ARMS_MemoryManagerStartupShutdown_Status_Success;
}

Arcadia_ARMS_MemoryManagerStartupShutdown_Status
Arcadia_ARMS_DefaultMemoryManager_create
  (
    Arcadia_ARMS_DefaultMemoryManager** result
  )
{
  if (!result) {
    return Arcadia_ARMS_MemoryManagerStartupShutdown_Status_ArgumentValueInvalid;
  }
  Arcadia_ARMS_DefaultMemoryManager* self = malloc(sizeof(Arcadia_ARMS_DefaultMemoryManager));
  if (!self) {
    return Arcadia_ARMS_MemoryManagerStartupShutdown_Status_AllocationFailed;
  }
  ((Arcadia_ARMS_MemoryManager*)self)->allocate = (Arcadia_ARMS_MemoryManager_Status (*)(Arcadia_ARMS_MemoryManager*, void**, Arcadia_ARMS_Size)) & allocate;
  ((Arcadia_ARMS_MemoryManager*)self)->reallocate = (Arcadia_ARMS_MemoryManager_Status(*)(Arcadia_ARMS_MemoryManager*, void**, Arcadia_ARMS_Size)) &reallocate;
  ((Arcadia_ARMS_MemoryManager*)self)->deallocate = (Arcadia_ARMS_MemoryManager_Status(*)(Arcadia_ARMS_MemoryManager*, void*)) &deallocate;
  ((Arcadia_ARMS_MemoryManager*)self)->destroy = (Arcadia_ARMS_MemoryManagerStartupShutdown_Status(*)(Arcadia_ARMS_MemoryManager*)) &destroy;
  *result = self;
  return Arcadia_ARMS_MemoryManagerStartupShutdown_Status_Success;
}
