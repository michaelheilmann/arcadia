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
#include <pthread.h>
#include <stdlib.h>

typedef struct Arcadia_Network_Internal_PosixMutex {
  pthread_mutex_t mutex;
} Arcadia_Network_Internal_PosixMutex;

int
Arcadia_Network_Internal_Mutex_initialize
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  if (!self || self->implementation) {
    return 0;
  }
  Arcadia_Network_Internal_PosixMutex* p = malloc(sizeof(*p));
  if (!p) {
    return 0;
  }
  if (pthread_mutex_init(&p->mutex, NULL)) {
    free(p);
    return 0;
  }
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
    Arcadia_Network_Internal_PosixMutex* p = self->implementation;
    pthread_mutex_destroy(&p->mutex);
    free(p);
    self->implementation = NULL;
  }
}

int
Arcadia_Network_Internal_Mutex_lock
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  return self && self->implementation &&
         !pthread_mutex_lock(&((Arcadia_Network_Internal_PosixMutex*)self->implementation)->mutex);
}

int
Arcadia_Network_Internal_Mutex_unlock
  (
    Arcadia_Network_Internal_Mutex* self
  )
{
  return self && self->implementation &&
         !pthread_mutex_unlock(&((Arcadia_Network_Internal_PosixMutex*)self->implementation)->mutex);
}
