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

#include "Arcadia/Ring1/Implementation/Concurrency/CompareAndSwap.h"

#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
    Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
    Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
  /* Intentionally empty. Nothing to include. */
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("environment not (yet) supported")
#endif

void*
Arcadia_Memory_compareAndSwap
  (
    void* volatile *destination,
    void* comperand,
    void* exchange
  )
{
#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
    Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
    Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
  return __sync_val_compare_and_swap(destination, comperand, exchange);
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  return InterlockedCompareExchangePointer(destination, exchange, comperand);
#else
  #error("environment not (yet) supported")
#endif
}
