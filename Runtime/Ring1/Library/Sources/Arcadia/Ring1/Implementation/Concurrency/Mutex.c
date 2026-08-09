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

#include "Arcadia/Ring1/Implementation/Concurrency/Mutex.h"

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
  pthread_mutex_t mutex;
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  HANDLE mutex;
#else
  #error("environment not (yet) supported")
#endif
};

Arcadia_Concurrency_Mutex_Result
Arcadia_Concurrency_Mutex_initialize
  (
    Arcadia_Concurrency_Mutex* mutex
  )
{
  if (!mutex) {
    return Arcadia_Concurrency_Mutex_Result_ArgumentInvalid;
  }
  Impl* pimpl = malloc(sizeof(Impl));
  if (!pimpl) {
    return Arcadia_Concurrency_Mutex_Result_AllocationFailed;
  }
#if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
    Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
    Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  int result = pthread_mutex_init(&pimpl->mutex, &attr);
  pthread_mutexattr_destroy(&attr);
  if (result) {
    free(pimpl);
    pimpl = NULL;
    return Arcadia_Concurrency_Mutex_Result_EnvironmentFailed;
  }
#elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  pimpl->mutex = CreateMutex(NULL, FALSE, NULL);
  if (!pimpl->mutex) {
    free(pimpl);
    pimpl = NULL;
    return Arcadia_Concurrency_Mutex_Result_EnvironmentFailed;
  }
#else
  #error("operating system not (yet) supported")
#endif
  Impl* oldValue = Arcadia_Memory_compareAndSwap(&mutex->pimpl, NULL, pimpl);
  if (oldValue) {
  #if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
      Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
      Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
    pthread_mutex_destroy(&pimpl->mutex);
  #elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
    CloseHandle(pimpl->mutex);
    pimpl->mutex = NULL;
  #else
    #error("operating system not (yet) supported")
  #endif
    free(pimpl);
    pimpl = NULL;
    return Arcadia_Concurrency_Mutex_Result_Initialized;
  }
  return Arcadia_Concurrency_Mutex_Result_Success;
}

Arcadia_Concurrency_Mutex_Result
Arcadia_Concurrency_Mutex_uninitialize
  (
    Arcadia_Concurrency_Mutex* mutex
  )
{
  if (!mutex) {
    return Arcadia_Concurrency_Mutex_Result_ArgumentInvalid;
  }
  Impl* pimpl = Arcadia_Memory_compareAndSwap(&mutex->pimpl, mutex->pimpl, NULL);
  if (pimpl) {
  #if Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem  || \
      Arcadia_Configuration_OperatingSystem_Cygwin == Arcadia_Configuration_OperatingSystem || \
      Arcadia_Configuration_OperatingSystem_Macos == Arcadia_Configuration_OperatingSystem
    pthread_mutex_destroy(&pimpl->mutex);
  #elif Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
    CloseHandle(pimpl->mutex);
    pimpl->mutex = NULL;
  #else
    #error("operating system not (yet) supported")
  #endif
    free(pimpl);
    pimpl = NULL;
  }
  return Arcadia_Concurrency_Mutex_Result_Success;
}

Arcadia_Concurrency_Mutex_Result
Arcadia_Concurrency_Mutex_lock
  (
    Arcadia_Concurrency_Mutex* mutex
  )
{
  if (!mutex) {
    return Arcadia_Concurrency_Mutex_Result_ArgumentInvalid;
  }
  return Arcadia_Concurrency_Mutex_Result_Success;
}

Arcadia_Concurrency_Mutex_Result
Arcadia_Concurrency_Mutex_unlock
  (
    Arcadia_Concurrency_Mutex* mutex
  )
{
  if (!mutex) {
    return Arcadia_Concurrency_Mutex_Result_ArgumentInvalid;
  }
  return Arcadia_Concurrency_Mutex_Result_Success;
}
