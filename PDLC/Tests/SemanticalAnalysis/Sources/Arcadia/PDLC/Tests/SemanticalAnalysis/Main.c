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

#include "Arcadia/PDLC/Include.h"

#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/SymbolInfo.h"
#include <stdio.h>

/// Check that the C backend generates no source or header file for a built-in type.
/// @return #Arcadia_BooleanValue_True if that invariant holds, #Arcadia_BooleanValue_False otherwise.
/// @remarks A built-in type has no module, so it has no generated source or header file of its own.
/// The built-in registry is the single source of truth for which types are built-in; this check
/// makes sure that a type registered as built-in also behaves as one, rather than being handed to
/// the backend as if it were declared by a module.
static Arcadia_BooleanValue
builtInsHaveNoGeneratedFiles
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_PDLC_Backend_Implementation* implementation =
    Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);
  Arcadia_List* symbolInfos = Arcadia_Map_getValues(thread, implementation->symbolInfos);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbolInfos); i < n; ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo = (Arcadia_PDLC_Backend_SymbolInfo*)Arcadia_List_getObjectReferenceValueCheckedAt
      (
        thread, symbolInfos, i,
        _Arcadia_PDLC_Backend_SymbolInfo_getType(thread)
      );
    if (!Arcadia_PDLC_Backend_SymbolInfo_isBuiltIn(thread, symbolInfo)) {
      continue;
    }
    if (Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFilePath(thread, symbolInfo)) {
      return Arcadia_BooleanValue_False;
    }
    if (Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo)) {
      return Arcadia_BooleanValue_False;
    }
  }
  return Arcadia_BooleanValue_True;
}

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
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);

  // The module directory paths are relative to the working directory path.
  Arcadia_FilePath* workingDirectoryPath = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets"));

  Arcadia_List* moduleDirectoryPaths = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackObjectReferenceValue
    (
      thread, moduleDirectoryPaths,
      (Arcadia_Object*)Arcadia_FilePath_parseGeneric(thread, moduleDirectoryName)
    );

  Arcadia_PDLC_CompilationTask* compilationTask = Arcadia_PDLC_CompilationTask_create(thread, context);

  Arcadia_BooleanValue result = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_PDLC_CompilationTask_run(thread, compilationTask, workingDirectoryPath, moduleDirectoryPaths);
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
  // A compilation task that reached the backend must not have asked the backend to generate files
  // for a built-in type.
  if (result) {
    if (!builtInsHaveNoGeneratedFiles(thread, context)) {
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

static void
testInterfaceOperationBody
  (
    Arcadia_Thread* thread
  )
{
  // Interface operations are declarations and must not have bodies.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"InterfaceOperationBody"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testParameters
  (
    Arcadia_Thread* thread
  )
{
  // An operation parameter with a defined type must resolve the type of its parameter symbol.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"Parameters"));
  if (!ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
testMissingModuleDefinition
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"MissingModuleDefinition"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testMultipleModuleDefinitions
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"MultipleModuleDefinitions"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testNativeClasses
  (
    Arcadia_Thread* thread
  )
{
  // A native class enters a class symbol and member symbols, but backend generation skips the class.
  // Interfaces can still use the native class type in operation signatures.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"NativeClasses"));
  if (!ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
testParameterTypeNotDefined
  (
    Arcadia_Thread* thread
  )
{
  // An operation parameter with an undefined type is a semantical error.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"ParameterTypeNotDefined"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testParameterTypeNotQualified
  (
    Arcadia_Thread* thread
  )
{
  // A type name is fully qualified: there is no relative resolution. The parameter refers to
  // `Greeter` rather than `MyModule.Greeter`, hence the type is not defined, which is a
  // semantical error.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"ParameterTypeNotQualified"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testProcedures
  (
    Arcadia_Thread* thread
  )
{
  // Procedures enter their parameter symbols and resolve both the parameter types and the
  // return value type.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"Procedures"));
  if (!ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
testProcedureParameterTypeNotDefined
  (
    Arcadia_Thread* thread
  )
{
  // A procedure enters its parameter symbols, so a procedure parameter with an undefined type is
  // a semantical error.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"ProcedureParameterTypeNotDefined"));
  if (ok) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testReturnTypeNotDefined
  (
    Arcadia_Thread* thread
  )
{
  // An operation with an undefined return type is a semantical error.
  Arcadia_BooleanValue ok = compile(thread, Arcadia_String_createFromCxxString(thread, u8"ReturnTypeNotDefined"));
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
  for (Arcadia_SizeValue i = 0; i < 13; ++i) {
    Arcadia_BooleanValue ok = Arcadia_BooleanValue_False;
    switch (i) {
      case 0: ok = safeExecute(NULL, &testInterfaces); break;
      case 1: ok = safeExecute(NULL, &testInterfaceNotAnInterface); break;
      case 2: ok = safeExecute(NULL, &testInterfaceCycle); break;
      case 3: ok = safeExecute(NULL, &testInterfaceOperationBody); break;
      case 4: ok = safeExecute(NULL, &testMissingModuleDefinition); break;
      case 5: ok = safeExecute(NULL, &testMultipleModuleDefinitions); break;
      case 6: ok = safeExecute(NULL, &testParameters); break;
      case 7: ok = safeExecute(NULL, &testNativeClasses); break;
      case 8: ok = safeExecute(NULL, &testParameterTypeNotDefined); break;
      case 9: ok = safeExecute(NULL, &testParameterTypeNotQualified); break;
      case 10: ok = safeExecute(NULL, &testProcedures); break;
      case 11: ok = safeExecute(NULL, &testProcedureParameterTypeNotDefined); break;
      case 12: ok = safeExecute(NULL, &testReturnTypeNotDefined); break;
      default: break;
    }
    if (!ok) {
      fprintf(stderr, "semantic analysis subtest %zu failed\n", (size_t)i);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
