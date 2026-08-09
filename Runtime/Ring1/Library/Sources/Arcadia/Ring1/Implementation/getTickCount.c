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
#include "Arcadia/Ring1/Implementation/getTickCount.h"

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include <time.h>
  #include <math.h> // round
#else
  #error("operating system not (yet) supported")
#endif

Arcadia_Natural64Value
Arcadia_getTickCount
  (
    Arcadia_Thread* thread
  )
{
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  return GetTickCount64();
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  // This is incorrect. It provides the time since some unspecified point in the past and not since the start of the process.
  struct timespec t;
  //t = (struct timespec *)malloc(sizeof(t));
  clock_gettime(CLOCK_MONOTONIC, &t);
  Arcadia_Natural64Value milliseconds = 0;
  // Nanoseconds to milliseconds.
  milliseconds = round(t.tv_nsec / 1.0e6);
  // Seconds to milliseconds.
  milliseconds += t.tv_sec*1000;
  return milliseconds;
#else
  #error("environment not (yet) supported")
#endif
}
