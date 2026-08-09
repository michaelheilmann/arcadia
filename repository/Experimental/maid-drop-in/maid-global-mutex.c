/**
 * @file maid-global-mutex.c
 * @brief Header file of the global mutex functionality.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#include "maid-global-mutex.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>

static HANDLE g_hMutex = NULL;

int
maid_global_mutex_lock
  (
  )
{
  // (atomically) read the value of g_hMutex.
  // If it is null, then create a mutex to store that mutex in g_hMutex.
  if (!InterlockedCompareExchangePointer(&g_hMutex, NULL, NULL)) {
    fprintf(stdout, "[cex global-mutex] begin create mutex\n");
    HANDLE hMutex =
      CreateMutex
      (
        NULL, // default security attributes
        TRUE, // this thread owns the mutex initially
        NULL  // unnamed mutex
      );
    if (NULL == hMutex) {
      fprintf(stderr, "[cex global-mutex] end create mutex with failure\n");
      return 1;
    }
    fprintf(stdout, "[engine] end create mutex with success\n");
    // While we were creating the mutex to store it in g_hMutex, someone else might have created another mutex and stored that other mutex in g_hMutex already.
    // In that case, throw away our mutex.
    if (InterlockedCompareExchangePointer(&g_hMutex, hMutex, NULL)) {
      CloseHandle(hMutex);
      hMutex = NULL;

    }
  }
  DWORD dwWaitResult = WaitForSingleObject(g_hMutex, INFINITE);
  if (dwWaitResult != WAIT_OBJECT_0) {
    return 1;
  }
  return 0;
}

int
maid_global_mutex_unlock
  (
  )
{
  if (!InterlockedCompareExchangePointer(&g_hMutex, NULL, NULL)) {
    return 1;
  }
  if (!ReleaseMutex(InterlockedCompareExchangePointer(&g_hMutex, NULL, NULL))) {
    return 1;
  }
  return 0;
}
