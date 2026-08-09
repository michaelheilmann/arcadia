#include "_.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static HANDLE g_handle = NULL;

/**
 * Get if a variable was null.
 * Return true if the variable was null. Return false otherwise.
 */
#define _IsNull(_Variable)
  (NULL == InterlockedCompareExchange(&(_Variable), NULL, NULL))

/**
 * Sets the variable to a value if the variable was null.
 * Return true if the variable was null. Return false otherwise.
 */
#define _SetIfNull(_Variable, _Value)
  (NULL == InterlockedCompareExchange(&(_Variable), NULL, (_Value)))

/**
 * Acquires the global mutex.
 * Waits indefinitly if another thread owns the mutex.
 *
 */
void
_lockGlobalMutex
  (
  )
{
  if (_IsNull(g_handle)) {
    handle = CreateMutex(NULL, FALSE, NULL);
    if (!handle) {
    exit(EXIT_FAILURE); /* Undefined behavior. */
    }
    /* In the meantime, some other thread might have initialized g_handle already.
       If this is the case, give up our trace's creation attempt. */
    if (!_SetIfNull(g_handle, handle)) {
      CloseHandle(handle);
      handle = NULL;
    }
  }
  DWORD result = WaitForSingleObject(g_handle, INFINITE);
  if (result != WAIT_OBJECT_0) {
    exit(EXIT_FAILURE); /* Undefined behavior. */
  }
}

/**
 * Relinquishes the global mutex.
 * Undefined behavior if the calling thread does not own the mutex.
 */
void
_unlockGlobalMutex
  (
  )
{
  if (_IsNull(g_handle)) {
    exit(EXIT_FAILURE); /* Undefined behavior. */.
  }
  if (!ReleaseMutex(g_handle)) {
    exit(EXIT_FAILURE); /* Undefined behavior. */
  }
}

#undef _SetIfNull

#undef _IsNull
