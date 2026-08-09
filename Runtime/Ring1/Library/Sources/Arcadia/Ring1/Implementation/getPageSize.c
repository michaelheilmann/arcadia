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
#include "Arcadia/Ring1/Implementation/getPageSize.h"

#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/Ring1/Implementation/Thread.h"

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  // SYSTEM_INFO, GetSystemInfo
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  // sysconf
  #include <unistd.h>
#else
  #error("operating system not (yet) supported")
#endif

Arcadia_Natural64Value
Arcadia_getPageSize
  (
    Arcadia_Thread* thread
  )
{
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  SYSTEM_INFO systemInfo;
  GetSystemInfo(&systemInfo);
  if (systemInfo.dwPageSize > Arcadia_Natural64Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural64Value)systemInfo.dwPageSize;
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  long temporary = sysconf(_SC_PAGESIZE);
  if (temporary < Arcadia_Natural64Value_Minimum || temporary > Arcadia_Natural64Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural64Value)temporary;
#else
  #error("environment not (yet) supported")
#endif
}
