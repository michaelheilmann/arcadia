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

#include "Arcadia/MILC/Include.h"

/// Run the compilation task over the module directory @c moduleDirectoryName in the `Assets`
/// directory. Return `Arcadia_BooleanValue_True` if the compilation task completed without an
/// error status and without any error diagnostic.
static Arcadia_BooleanValue
compile
  (
    Arcadia_Thread* thread,
    Arcadia_String* moduleDirectoryName
  )
{
  Arcadia_MILC_Context* context = Arcadia_MILC_Context_create(thread);

  // The module directory paths are relative to the working directory path.
  Arcadia_FilePath* workingDirectoryPath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets"));

  Arcadia_List* moduleDirectoryPaths = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue
    (
      thread, moduleDirectoryPaths,
      (Arcadia_Object*)Arcadia_FilePath_parseGeneric(thread, moduleDirectoryName)
    );

  Arcadia_MILC_CompilationTask* compilationTask = Arcadia_MILC_CompilationTask_create(thread, context);

  Arcadia_BooleanValue result = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_MILC_CompilationTask_run(thread, compilationTask, workingDirectoryPath, moduleDirectoryPaths);
    result = Arcadia_BooleanValue_True;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
  }
  // A compilation task that completed without raising must not have produced an error diagnostic.
  if (result) {
    if (Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics)) {
      result = Arcadia_BooleanValue_False;
    }
  }
  Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
  return result;
}

static void
testInterfaces
  (
    Arcadia_Thread* thread
  )
{
  // An interface type without an extended interface type, an interface type with an extended
  // interface type, and a class type implementing an interface type must all compile.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"Interfaces"));
  if (!ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
testInterfaceNotAnInterface
  (
    Arcadia_Thread* thread
  )
{
  // A class type implementing a class type is a semantical error.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"InterfaceNotAnInterface"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testInterfaceCycle
  (
    Arcadia_Thread* thread
  )
{
  // Two interface types extending each other form an inheritance cycle, which is a semantical error.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"InterfaceCycle"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static inline Arcadia_BooleanValue
safeExecute
  (
    Arcadia_Thread* thread,
    void (*testFunction)(Arcadia_Thread*)
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_BooleanValue result = Arcadia_BooleanValue_False;
  Arcadia_Thread* thread1 = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread1, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    testFunction(thread1);
    if (!Arcadia_Thread_getStatus(thread1)) {
      result = Arcadia_BooleanValue_True;
    }
    Arcadia_Thread_popJumpTarget(thread1);
  } else {
    Arcadia_Thread_popJumpTarget(thread1);
  }
  thread = NULL;
  thread1 = NULL;
  Arcadia_Process_relinquish(process);
  process = NULL;
  return result;
}

int
main
  (
    int argc,
    char** argv
  )
{
  // These tests deliberately raise, so they must not run under `Arcadia_Tests_safeExecute`.
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_BooleanValue ok = Arcadia_BooleanValue_False;
    switch (i) {
      case 0: ok = safeExecute(NULL, &testInterfaces); break;
      case 1: ok = safeExecute(NULL, &testInterfaceNotAnInterface); break;
      case 2: ok = safeExecute(NULL, &testInterfaceCycle); break;
      default: break;
    }
    if (!ok) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}