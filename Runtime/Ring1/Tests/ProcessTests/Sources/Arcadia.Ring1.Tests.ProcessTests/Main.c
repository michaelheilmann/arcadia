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

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Ring1/Include.h"
#include <stdio.h>
#include <stdlib.h>
#if defined(_MSC_VER) && defined(_DEBUG) && !defined(__SANITIZE_ADDRESS__)
  #include <crtdbg.h>
  #define WITH_DEBUG_HEAP_BALANCE 1
#endif

static void
visitProbe
  (
    Arcadia_Thread* thread
  )
{/*Intentionally empty: the callback owns no managed references.*/}

static void
preMarkProbe
  (
    Arcadia_Thread* thread,
    bool purgeCaches
  )
{/*Intentionally empty: the callback owns no cache.*/}

static void
finalizeProbe
  (
    Arcadia_Thread* thread,
    size_t* finalized
  )
{
  *finalized = 0;
}

static void
exerciseProcess
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Process* process = Arcadia_Thread_getProcess(thread);
  Arcadia_Process* second = NULL;
  Arcadia_Tests_assertTrue(thread, Arcadia_Process_get(&second) == Arcadia_ProcessStatus_Success);
  Arcadia_Tests_assertTrue(thread, second == process);
  Arcadia_Tests_assertTrue(thread, Arcadia_Process_acquire(process) == Arcadia_ProcessStatus_Success);
  Arcadia_Tests_assertTrue(thread, Arcadia_Process_relinquish(second) == Arcadia_ProcessStatus_Success);
  Arcadia_Tests_assertTrue(thread, Arcadia_Process_relinquish(process) == Arcadia_ProcessStatus_Success);
  Arcadia_BigInteger* number = Arcadia_BigInteger_create(thread);
  Arcadia_BigInteger_setNatural64(thread, number, UINT64_MAX);
  Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(
    Arcadia_RuntimeUTF8String_create(thread, u8"process lifetime", sizeof(u8"process lifetime") - 1)));
  // Each owner must unregister its callbacks before process destruction.
  Arcadia_Process_addArenaVisitCallback(process, &visitProbe);
  Arcadia_Process_removeArenaVisitCallback(process, &visitProbe);
  Arcadia_Process_addArenaPreMarkCallback(process, &preMarkProbe);
  Arcadia_Process_removeArenaPreMarkCallback(process, &preMarkProbe);
  Arcadia_Process_addArenaFinalizeCallback(process, &finalizeProbe);
  Arcadia_Process_removeArenaFinalizeCallback(process, &finalizeProbe);
}

static Arcadia_BooleanValue failureTriggered;

static void
failProcess
  (
    Arcadia_Thread* thread
  )
{
  exerciseProcess(thread);
  failureTriggered = true;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
  Arcadia_Thread_jump(thread);
}

static int
startupShutdown
  (
    void
  )
{
  for (unsigned i = 0; i < 32; ++i) {
    if (!Arcadia_Tests_safeExecute(&exerciseProcess)) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
failureRecovery
  (
    void
  )
{
  for (unsigned i = 0; i < 16; ++i) {
    failureTriggered = false;
    if (Arcadia_Tests_safeExecute(&failProcess) || !failureTriggered) return EXIT_FAILURE;
    if (!Arcadia_Tests_safeExecute(&exerciseProcess)) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
allocationBalance
  (
    void
  )
{
  // Warm one-time CRT/library initialization before comparing normal heap blocks.
  if (failureRecovery()) return EXIT_FAILURE;
#if defined(WITH_DEBUG_HEAP_BALANCE)
  _CrtMemState before, after;
  _CrtMemCheckpoint(&before);
#endif
  if (startupShutdown() || failureRecovery()) return EXIT_FAILURE;
#if defined(WITH_DEBUG_HEAP_BALANCE)
  _CrtMemCheckpoint(&after);
  if (before.lCounts[_NORMAL_BLOCK] != after.lCounts[_NORMAL_BLOCK]
      || before.lSizes[_NORMAL_BLOCK] != after.lSizes[_NORMAL_BLOCK]) {
    fprintf(stderr, "Process cycles retain normal heap blocks: before=%zu/%zu after=%zu/%zu\n",
      before.lCounts[_NORMAL_BLOCK], before.lSizes[_NORMAL_BLOCK],
      after.lCounts[_NORMAL_BLOCK], after.lSizes[_NORMAL_BLOCK]);
    return EXIT_FAILURE;
  }
#endif
  // Linux sanitizer runs additionally check all allocations at process exit.
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  const Arcadia_Testing_Case cases[] = {
    { "StartupShutdown", startupShutdown },
    { "FailureRecovery", failureRecovery },
    { "AllocationBalance", allocationBalance },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
