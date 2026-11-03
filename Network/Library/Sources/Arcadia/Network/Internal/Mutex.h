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

#if !defined(ARCADIA_NETWORK_INTERNAL_MUTEX_H_INCLUDED)
#define ARCADIA_NETWORK_INTERNAL_MUTEX_H_INCLUDED

// Private native mutex. May be used only to protect native memory; never
// pass Ring1/Arcadia managed objects or values into native workers.
// No platform headers are exposed from this type.
typedef struct Arcadia_Network_Internal_Mutex {
  void* implementation;
} Arcadia_Network_Internal_Mutex;

int
Arcadia_Network_Internal_Mutex_initialize
  (
    Arcadia_Network_Internal_Mutex* self
  );

void
Arcadia_Network_Internal_Mutex_uninitialize
  (
    Arcadia_Network_Internal_Mutex* self
  );

int
Arcadia_Network_Internal_Mutex_lock
  (
    Arcadia_Network_Internal_Mutex* self
  );

int
Arcadia_Network_Internal_Mutex_unlock
  (
    Arcadia_Network_Internal_Mutex* self
  );

#endif // ARCADIA_NETWORK_INTERNAL_MUTEX_H_INCLUDED
