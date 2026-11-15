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

#include <stdlib.h>

// Tests are serial. Each case resets the state used by its callbacks.
static Arcadia_Signal* activeSignal;
static Arcadia_Slot* disconnectTarget;
static unsigned primaryCalls;
static unsigned secondaryCalls;
static unsigned lateCalls;
static unsigned failedCalls;
static unsigned expectedArity;
static unsigned connectOnce;
static unsigned nestOnce;
static unsigned failOnce;

static void
resetCallbacks
  (
    void
  )
{
  activeSignal = NULL;
  disconnectTarget = NULL;
  primaryCalls = 0;
  secondaryCalls = 0;
  lateCalls = 0;
  failedCalls = 0;
  expectedArity = 0;
  connectOnce = 0;
  nestOnce = 0;
  failOnce = 0;
}

static Arcadia_Signal*
createRootedSignal
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Signal* signal = Arcadia_Signal_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)signal);
  return signal;
}

static void
emitZero
  (
    Arcadia_Thread* thread,
    Arcadia_Signal* signal
  )
{
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Signal_emit(thread, signal, (Arcadia_Object*)signal);
  Arcadia_Tests_assertEqualSize(thread, depth + 1, Arcadia_ValueStack_getSize(thread));
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
countArguments
  (
    Arcadia_Thread* thread
  )
{
  ++primaryCalls;
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == expectedArity + 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getObjectReferenceValue(thread, expectedArity + 2) == (Arcadia_Object*)activeSignal);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getObjectReferenceValue(thread, expectedArity + 1) == (Arcadia_Object*)activeSignal);
  for (Arcadia_SizeValue i = 0; i < expectedArity; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, expectedArity - i) == (Arcadia_Natural8Value)i);
  }
  // Signal_emit owns the callback frame, as in the current Engine/Nodes callers.
}

static void
testArgumentBoundary
  (
    Arcadia_Thread* thread
  )
{
  resetCallbacks();
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Signal* signal = createRootedSignal(thread);
  activeSignal = signal;
  Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &countArguments);
  Arcadia_SizeValue rooted = Arcadia_ValueStack_getSize(thread);

  const Arcadia_Natural8Value validCounts[] = { 0, 1, 253 };
  for (Arcadia_SizeValue j = 0; j < sizeof(validCounts) / sizeof(validCounts[0]); ++j) {
    Arcadia_Natural8Value count = validCounts[j];
    expectedArity = count;
    for (Arcadia_SizeValue i = 0; i < count; ++i) {
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)i);
    }
    Arcadia_ValueStack_pushNatural8Value(thread, count);
    Arcadia_Signal_emit(thread, signal, (Arcadia_Object*)signal);
    Arcadia_Tests_assertEqualSize(thread, rooted + count + 1, Arcadia_ValueStack_getSize(thread));
    Arcadia_ValueStack_popValues(thread, count + 1);
    Arcadia_Tests_assertEqualSize(thread, rooted, Arcadia_ValueStack_getSize(thread));
  }
  Arcadia_Tests_assertTrue(thread, primaryCalls == 3);

  const Arcadia_Natural8Value invalidCounts[] = { 254, 255 };
  for (Arcadia_SizeValue j = 0; j < sizeof(invalidCounts) / sizeof(invalidCounts[0]); ++j) {
    Arcadia_Natural8Value count = invalidCounts[j];
    for (Arcadia_SizeValue i = 0; i < count; ++i) {
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)i);
    }
    Arcadia_ValueStack_pushNatural8Value(thread, count);
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Signal_emit(thread, signal, (Arcadia_Object*)signal);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid, status);
      Arcadia_Tests_assertEqualSize(thread, depth, Arcadia_ValueStack_getSize(thread));
    }
    Arcadia_ValueStack_popValues(thread, count + 1);
    Arcadia_Tests_assertEqualSize(thread, rooted, Arcadia_ValueStack_getSize(thread));
    Arcadia_Tests_assertTrue(thread, primaryCalls == 3);
  }
  // Rejected emissions must not poison the signal's reentrancy state.
  expectedArity = 0;
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 4);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static void
onLate
  (
    Arcadia_Thread* thread
  )
{
  ++lateCalls;
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural8Value(thread, 0) == 2);
}

static void
onConnect
  (
    Arcadia_Thread* thread
  )
{
  ++primaryCalls;
  if (!connectOnce) {
    connectOnce = 1;
    Arcadia_Signal_connect(thread, activeSignal, (Arcadia_Object*)activeSignal, &onLate);
  }
}

static void
testConnectDuringEmission
  (
    Arcadia_Thread* thread
  )
{
  resetCallbacks();
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Signal* signal = createRootedSignal(thread);
  activeSignal = signal;
  // Fill all eight initial slots: attaching the ninth forces realloc().
  for (unsigned i = 0; i < 8; ++i) {
    Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &onConnect);
  }
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 8 && lateCalls == 0);
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 16 && lateCalls == 1);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static void
onDisconnect
  (
    Arcadia_Thread* thread
  )
{
  ++primaryCalls;
  if (primaryCalls == 1) {
    Arcadia_Slot_disconnect(thread, disconnectTarget);
  }
}

static void
onSecondary
  (
    Arcadia_Thread* thread
  )
{
  (void)thread;
  ++secondaryCalls;
}

static void
testDisconnectDuringEmission
  (
    Arcadia_Thread* thread
  )
{
  resetCallbacks();
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Signal* signal = createRootedSignal(thread);
  Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &onDisconnect);
  disconnectTarget = Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &onSecondary);
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 1 && secondaryCalls == 0);
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 2 && secondaryCalls == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static void
onNested
  (
    Arcadia_Thread* thread
  )
{
  ++primaryCalls;
  if (!nestOnce) {
    nestOnce = 1;
    emitZero(thread, activeSignal);
  }
}

static void
testNestedEmission
  (
    Arcadia_Thread* thread
  )
{
  resetCallbacks();
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Signal* signal = createRootedSignal(thread);
  activeSignal = signal;
  Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &onNested);
  Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &onSecondary);
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 2 && secondaryCalls == 2);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static void
onCollectedReceiver
  (
    Arcadia_Thread* thread
  )
{
  (void)thread;
  ++primaryCalls;
}

static void
testDestroyedReceiver
  (
    Arcadia_Thread* thread
  )
{
  resetCallbacks();
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Signal* signal = createRootedSignal(thread);
  Arcadia_Signal* receiver = createRootedSignal(thread);
  Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)receiver, &onCollectedReceiver);
  Arcadia_ValueStack_popValues(thread, 1); // Receiver has no strong owner.
  Arcadia_Tests_assertFalse(thread, Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  emitZero(thread, signal);
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, primaryCalls == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static void
onFailure
  (
    Arcadia_Thread* thread
  )
{
  ++failedCalls;
  if (failOnce) {
    failOnce = 0;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
testErrorRecovery
  (
    Arcadia_Thread* thread
  )
{
  resetCallbacks();
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Signal* signal = createRootedSignal(thread);
  Arcadia_Signal_connect(thread, signal, (Arcadia_Object*)signal, &onFailure);
  failOnce = 1;
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Signal_emit(thread, signal, (Arcadia_Object*)signal);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationFailed, status);
    Arcadia_Tests_assertEqualSize(thread, depth, Arcadia_ValueStack_getSize(thread));
  }
  Arcadia_ValueStack_popValues(thread, 1);
  emitZero(thread, signal);
  Arcadia_Tests_assertTrue(thread, failedCalls == 2);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static int
runArguments
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testArgumentBoundary) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runConnect
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testConnectDuringEmission) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runDisconnect
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testDisconnectDuringEmission) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runNested
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testNestedEmission) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runDestroyed
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testDestroyedReceiver) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runRecovery
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testErrorRecovery) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ArgumentBoundary", &runArguments },
    { "ConnectDuringEmission", &runConnect },
    { "DisconnectDuringEmission", &runDisconnect },
    { "NestedEmission", &runNested },
    { "DestroyedReceiver", &runDestroyed },
    { "ErrorRecovery", &runRecovery },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
