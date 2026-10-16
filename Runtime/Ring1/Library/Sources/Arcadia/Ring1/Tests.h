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

#if !defined(ARCADIA_RING1_TESTS_H_INCLUDED)
#define ARCADIA_RING1_TESTS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Boolean.h"
#include "Arcadia/Ring1/Diagnostics.h"
#include "Arcadia/Ring1/Process.h"
#include "Arcadia/Ring1/Thread.h"
#include "Arcadia/Ring1/Integer64.h"
#include "Arcadia/Ring1/Natural64.h"
#include "Arcadia/Ring1/Real64.h"
#include "Arcadia/Ring1/Size.h"
#include "Arcadia/Ring1/Status.h"

// Borrowed native name, scoped by the runner to a callback on this OS thread.
// Independent of Arcadia_Process lifetime; NULL means no selected case.
// Names are not propagated to worker threads. Callers outside the runner may
// scope a name explicitly; keep its storage alive until the scope is restored.
const char*
Arcadia_Tests_getCaseName
  (
    void
  );

// Return the previous borrowed pointer, which the caller must restore afterward.
// A nested suite restores its caller's name; discovery does not change the name.
const char*
Arcadia_Tests_setCaseName
  (
    const char* name
  );

#define Arcadia_Tests_DeclareEquality(Name, Type) \
  void \
  Arcadia_Tests_assertEqual##Name##Impl \
    ( \
      Arcadia_Thread* thread, \
      Type expected, \
      Type actual, \
      const char* file, \
      int line, \
      const char* expectedExpression, \
      const char* actualExpression \
    );

Arcadia_Tests_DeclareEquality(Integer64, Arcadia_Integer64Value)
Arcadia_Tests_DeclareEquality(Natural64, Arcadia_Natural64Value)
Arcadia_Tests_DeclareEquality(Real64, Arcadia_Real64Value)
Arcadia_Tests_DeclareEquality(Size, Arcadia_SizeValue)
Arcadia_Tests_DeclareEquality(Status, Arcadia_Status)
Arcadia_Tests_DeclareEquality(CxxString, const char*)
#undef Arcadia_Tests_DeclareEquality

void
Arcadia_Tests_assertEqualBytesImpl
  (
    Arcadia_Thread* thread,
    const void* expected,
    Arcadia_SizeValue expectedSize,
    const void* actual,
    Arcadia_SizeValue actualSize,
    const char* file,
    int line,
    const char* expectedExpression,
    const char* actualExpression
  );

// Opt-in typed comparisons: each argument expression is evaluated once, in the
// unspecified order of a C function call. Choose the correct signed/unsigned
// domain; Size is a separate API even when its width equals Natural64.
// Failure reports expected/actual values, expressions, selected case (or
// <unselected>), file and line, then sets TestFailed and jumps on thread.
// Real64 uses exact == semantics (NaNs differ; signed zeros compare equal).
// CxxString requires NUL termination and treats NULL distinctly from empty.
// Bytes compares explicit lengths, including embedded NULs; NULL is allowed for
// zero bytes, but a nonempty NULL span fails even against another NULL span.
// Readable inputs must cover their supplied lengths. Diagnostics escape controls,
// bound each preview to 32 bytes and report lengths/first difference even beyond
// the preview. Existing simple assertions below retain their original behavior.
#define Arcadia_Tests_assertEqualInteger64(thread, expected, actual) \
  Arcadia_Tests_assertEqualInteger64Impl((thread), (expected), (actual), __FILE__, __LINE__, #expected, #actual)
#define Arcadia_Tests_assertEqualNatural64(thread, expected, actual) \
  Arcadia_Tests_assertEqualNatural64Impl((thread), (expected), (actual), __FILE__, __LINE__, #expected, #actual)
#define Arcadia_Tests_assertEqualReal64(thread, expected, actual) \
  Arcadia_Tests_assertEqualReal64Impl((thread), (expected), (actual), __FILE__, __LINE__, #expected, #actual)
#define Arcadia_Tests_assertEqualSize(thread, expected, actual) \
  Arcadia_Tests_assertEqualSizeImpl((thread), (expected), (actual), __FILE__, __LINE__, #expected, #actual)
#define Arcadia_Tests_assertEqualStatus(thread, expected, actual) \
  Arcadia_Tests_assertEqualStatusImpl((thread), (expected), (actual), __FILE__, __LINE__, #expected, #actual)
#define Arcadia_Tests_assertEqualCxxString(thread, expected, actual) \
  Arcadia_Tests_assertEqualCxxStringImpl((thread), (expected), (actual), __FILE__, __LINE__, #expected, #actual)
#define Arcadia_Tests_assertEqualBytes(thread, expected, expectedSize, actual, actualSize) \
  Arcadia_Tests_assertEqualBytesImpl((thread), (expected), (expectedSize), (actual), (actualSize), __FILE__, __LINE__, #expected, #actual)

#define Arcadia_Tests_assertTrue(thread, expression) \
  if (!(expression)) { \
    Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: test assertion true == `%s` failed\n", __FILE__, __LINE__, #expression); \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
    Arcadia_Thread_jump(thread); \
  }

#define Arcadia_Tests_assertFalse(thread, expression) \
  if ((expression)) { \
    Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: test assertion false == `%s` failed\n", __FILE__, __LINE__, #expression); \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
    Arcadia_Thread_jump(thread); \
  }

static inline Arcadia_BooleanValue
Arcadia_Tests_safeExecute
  (
    void (*f)(Arcadia_Thread*)
  )
{
  Arcadia_BooleanValue result = Arcadia_BooleanValue_True;
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    result = Arcadia_BooleanValue_False;
    return result;
  }
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(Arcadia_Process_getThread(process), &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    (*f)(Arcadia_Process_getThread(process));
  } else {
    result = Arcadia_BooleanValue_False;
  }
  Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
  Arcadia_Status status = Arcadia_Thread_getStatus(Arcadia_Process_getThread(process));
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    result = Arcadia_BooleanValue_False;
  }
  return result;
}

#endif // ARCADIA_RING1_TESTS_H_INCLUDED
