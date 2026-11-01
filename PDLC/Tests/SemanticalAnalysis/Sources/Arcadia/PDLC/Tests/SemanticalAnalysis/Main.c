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
#include "Arcadia/PDLC/Tests/Include.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/SymbolInfo.h"
#include <stdlib.h>

static Arcadia_BooleanValue
builtInsHaveNoGeneratedFiles
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_PDLC_Backend_Implementation* implementation = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_List* symbolInfos = Arcadia_Map_getValues(thread, implementation->symbolInfos);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)symbolInfos);
  Arcadia_BooleanValue result = Arcadia_BooleanValue_True;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbolInfos); i < n; ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo = (Arcadia_PDLC_Backend_SymbolInfo*)Arcadia_List_getObjectReferenceValueCheckedAt
      (
        thread, symbolInfos, i, _Arcadia_PDLC_Backend_SymbolInfo_getType(thread)
      );
    if (!Arcadia_PDLC_Backend_SymbolInfo_isBuiltIn(thread, symbolInfo)) continue;
    if (Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFilePath(thread, symbolInfo)
        || Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo)) {
      result = Arcadia_BooleanValue_False;
      break;
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

// Keep transient strings/paths in the caller's compilation-test frame.
static Arcadia_FilePath*
makePath
  (
    Arcadia_Thread* thread,
    const char* text
  )
{
  Arcadia_String* string = Arcadia_String_createFromCxxString(thread, text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)string);
  Arcadia_FilePath* path = Arcadia_FilePath_parseGeneric(thread, string);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path);
  return path;
}

static Arcadia_BooleanValue
compileModules
  (
    Arcadia_Thread* thread,
    const char* moduleDirectoryName,
    const char* secondDirectoryName,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_FilePath* workingDirectoryPath = makePath(thread, "Assets");
  Arcadia_List* moduleDirectoryPaths = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)moduleDirectoryPaths);
  Arcadia_List_insertBackObjectReferenceValue(thread, moduleDirectoryPaths, (Arcadia_Object*)makePath(thread, moduleDirectoryName));
  if (secondDirectoryName) {
    Arcadia_List_insertBackObjectReferenceValue(thread, moduleDirectoryPaths, (Arcadia_Object*)makePath(thread, secondDirectoryName));
  }
  Arcadia_PDLC_CompilationTask* compilationTask = Arcadia_PDLC_CompilationTask_create(thread, context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)compilationTask);
  volatile Arcadia_BooleanValue completed = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_PDLC_CompilationTask_run(thread, compilationTask, workingDirectoryPath, moduleDirectoryPaths);
    completed = Arcadia_BooleanValue_True;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
  }
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  // Preserve the expected-error jump boundary; inspect/reset the captured status
  // before further allocation/assertion, not by substituting a success wrapper.
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_BooleanValue result = completed && !status && !Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics);
  if (result) result = builtInsHaveNoGeneratedFiles(thread, context);
  if (expected) {
    Arcadia_PDLC_Tests_assertDiagnostic(thread, context->diagnostics, status, expected);
    Arcadia_Tests_assertFalse(thread, completed);
  }
  Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static inline Arcadia_BooleanValue
safeExecute
  (
    Arcadia_Thread* thread,
    void (*testFunction)(Arcadia_Thread*)
  )
{
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) return Arcadia_BooleanValue_False;
  Arcadia_BooleanValue result = Arcadia_BooleanValue_False;
  Arcadia_Thread* thread1 = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread1, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    testFunction(thread1);
    if (!Arcadia_Thread_getStatus(thread1)) result = Arcadia_BooleanValue_True;
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

#define POSITIVE(Name) \
  static void \
  test##Name \
    ( \
      Arcadia_Thread* thread \
    ) \
  { \
    Arcadia_Tests_assertTrue(thread, compileModules(thread, #Name, NULL, NULL)); \
  } \
  static int \
  run##Name \
    ( \
      void \
    ) \
  { \
    return safeExecute(NULL, &test##Name) ? EXIT_SUCCESS : EXIT_FAILURE; \
  }

POSITIVE(Interfaces)
POSITIVE(Parameters)
POSITIVE(NativeClasses)
POSITIVE(Procedures)
POSITIVE(NativeEnumerationValue)
#undef POSITIVE

static void
testFileSystem
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, compileModules(thread, "FreshDirectories", NULL, NULL));
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fileSystem);
  Arcadia_FilePath* path = makePath(thread, "Assets/FreshDirectories/Library/Sources/MyModule/Nested/Fresh.h");
  Arcadia_Tests_assertTrue(thread, Arcadia_FileSystem_regularFileExists(thread, fileSystem, path));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static int
runFileSystem
  (
    void
  )
{
  return safeExecute(NULL, &testFileSystem) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#define PAIR_NEGATIVE(Name, First, Second, Type, File, Offset, Message) \
  static void \
  test##Name \
    ( \
      Arcadia_Thread* thread \
    ) \
  { \
    const Arcadia_PDLC_Tests_ExpectedDiagnostic expected = { #Name, Type, File, Offset, Message, Arcadia_Status_ValueRaised }; \
    Arcadia_Tests_assertFalse(thread, compileModules(thread, First, Second, &expected)); \
  } \
  static int \
  run##Name \
    ( \
      void \
    ) \
  { \
    return safeExecute(NULL, &test##Name) ? EXIT_SUCCESS : EXIT_FAILURE; \
  }
#define NEGATIVE(Name, Type, File, Offset, Message) PAIR_NEGATIVE(Name, #Name, NULL, Type, File, Offset, Message)
#define PDLC_INCLUDE_LEGACY_CASES
#define PDLC_INCLUDE_CALLABLE_CASES
#define PDLC_INCLUDE_FILESYSTEM_CASES
#include "NegativeCases.i"
#undef PDLC_INCLUDE_LEGACY_CASES
#undef PDLC_INCLUDE_CALLABLE_CASES
#undef PDLC_INCLUDE_FILESYSTEM_CASES
#undef NEGATIVE
#undef PAIR_NEGATIVE

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Interfaces", &runInterfaces },
    { "InterfaceNotAnInterface", &runInterfaceNotAnInterface },
    { "InterfaceCycle", &runInterfaceCycle },
    { "InterfaceOperationBody", &runInterfaceOperationBody },
    { "MissingModuleDefinition", &runMissingModuleDefinition },
    { "MultipleModuleDefinitions", &runMultipleModuleDefinitions },
    { "Parameters", &runParameters },
    { "NativeClasses", &runNativeClasses },
    { "ParameterTypeNotDefined", &runParameterTypeNotDefined },
    { "ParameterTypeNotQualified", &runParameterTypeNotQualified },
    { "Procedures", &runProcedures },
    { "ProcedureParameterTypeNotDefined", &runProcedureParameterTypeNotDefined },
    { "ReturnTypeNotDefined", &runReturnTypeNotDefined },
    { "NativeEnumerationValue", &runNativeEnumerationValue },
    // Former CallableErrors components retain their order, but now each has an
    // independent process boundary and T04 private fixture workspace.
#define NEGATIVE(Name, Type, File, Offset, Message) { #Name, &run##Name },
#define PAIR_NEGATIVE(Name, First, Second, Type, File, Offset, Message) { #Name, &run##Name },
#define PDLC_INCLUDE_CALLABLE_CASES
#include "NegativeCases.i"
#undef PDLC_INCLUDE_CALLABLE_CASES
#undef NEGATIVE
#undef PAIR_NEGATIVE
    { "FileSystem", &runFileSystem },
    { "MissingSourceDirectory", &runMissingSourceDirectory },
    { "BlockedOutputDirectory", &runBlockedOutputDirectory },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
