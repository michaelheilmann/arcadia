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

#include "Arcadia/Ring1/Implementation/Concurrency/Condition.h"

#include "Arcadia/Ring1/Implementation/Concurrency/CompareAndSwap.h"
#include <malloc.h>

#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
      Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
      Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
  #include <pthread.h>
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("environment not (yet) supported")
#endif

typedef struct Impl Impl;

struct Impl {
  #if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
      Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
      Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
    pthread_cond_t condition;
  #elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
    CONDITION_VARIABLE conditionVariable;
    CRITICAL_SECTION criticalSection;
  #else
    #error("environment not (yet) supported")
  #endif
};

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_initialize
  (
    Arcadia_Concurrency_Condition* condition
  )
{
  if (!condition) {
    return Arcadia_Concurrency_Condition_Result_ArgumentInvalid;
  }
  Impl* pimpl = malloc(sizeof(Impl));
  if (!pimpl) {
    return Arcadia_Concurrency_Condition_Result_AllocationFailed;
  }
#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
    Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
    Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
  if (pthread_cond_init(&pimpl->condition, NULL)) {
    free(pimpl);
    pimpl = NULL;
    return Arcadia_Concurrency_Condition_Result_EnvironmentFailed;
  }
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  InitializeConditionVariable(&pimpl->conditionVariable);
#else
  #error("operating system not (yet) supported")
#endif
  Impl* oldValue = Arcadia_Memory_compareAndSwap(&condition->pimpl, NULL, pimpl);
  if (oldValue) {
  #if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
      Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
      Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
    pthread_cond_destroy(&pimpl->condition);
  #elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
    /* Intentionally empty. */
  #else
    #error("operating system not (yet) supported")
  #endif
    free(pimpl);
    pimpl = NULL;
    return Arcadia_Concurrency_Condition_Result_Initialized;
  }
  return Arcadia_Concurrency_Condition_Result_Success;
}

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_uninitialize
  (
    Arcadia_Concurrency_Condition* condition
  )
{
  if (!condition) {
    return Arcadia_Concurrency_Condition_Result_ArgumentInvalid;
  }
  Impl* pimpl = Arcadia_Memory_compareAndSwap(&condition->pimpl, condition->pimpl, NULL);
  if (pimpl) {
  #if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
      Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
      Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
    pthread_cond_destroy(&pimpl->condition);
  #elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
    /* Intentionally empty. */
  #else
    #error("operating system not (yet) supported")
  #endif
    free(pimpl);
    pimpl = NULL;
  }
  return Arcadia_Concurrency_Condition_Result_Success;
}

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_wait
  (
    Arcadia_Concurrency_Condition* condition
  )
{ return Arcadia_Concurrency_Condition_Result_Success; }

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_signalOne
  (
    Arcadia_Concurrency_Condition* condition
  )
{
  if (!condition) {
    return Arcadia_Concurrency_Condition_Result_ArgumentInvalid;
  }
  return Arcadia_Concurrency_Condition_Result_Success;
}

Arcadia_Concurrency_Condition_Result
Arcadia_Concurrency_Condition_signalAll
  (
    Arcadia_Concurrency_Condition* condition
  )
{
  if (!condition) {
    return Arcadia_Concurrency_Condition_Result_ArgumentInvalid;
  }
  return Arcadia_Concurrency_Condition_Result_Success;
}
