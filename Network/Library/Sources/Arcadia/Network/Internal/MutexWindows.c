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

#include "Arcadia/Network/Internal/Mutex.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>

typedef struct Arcadia_Network_Internal_WindowsMutex {
  SRWLOCK lock;
} Arcadia_Network_Internal_WindowsMutex;

int
Arcadia_Network_Internal_Mutex_initialize
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  if (!self || self->implementation) {
    return 0;
  }
  Arcadia_Network_Internal_WindowsMutex* p = malloc(sizeof(*p));
  if (!p) {
    return 0;
  }
  InitializeSRWLock(&p->lock);
  self->implementation = p;
  return 1;
}

void
Arcadia_Network_Internal_Mutex_uninitialize
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  if (self && self->implementation) {
    free(self->implementation);
    self->implementation = NULL;
  }
}

int
Arcadia_Network_Internal_Mutex_lock
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  if (!self || !self->implementation) {
    return 0;
  }
  AcquireSRWLockExclusive(&((Arcadia_Network_Internal_WindowsMutex*)self->implementation)->lock);
  return 1;
}

int
Arcadia_Network_Internal_Mutex_unlock
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  if (!self || !self->implementation) {
    return 0;
  }
  ReleaseSRWLockExclusive(&((Arcadia_Network_Internal_WindowsMutex*)self->implementation)->lock);
  return 1;
}
