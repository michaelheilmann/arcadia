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

static const char* directory;

static void
generate
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_List* paths = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, paths);
  Arcadia_List_insertBackObjectReferenceValue(thread, paths, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "A")));
  Arcadia_List_insertBackObjectReferenceValue(thread, paths, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "B")));
  Arcadia_PDLC_CompilationTask* task = Arcadia_PDLC_CompilationTask_create(thread, context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, task);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_PDLC_CompilationTask_run(thread, task, Arcadia_FilePath_parseNative(thread, Arcadia_String_createFromCxxString(thread, directory)), paths);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
    Arcadia_Thread_jump(thread);
  }
  // Both lookup domains survive collection and preserve distinct symbols.
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Example.A");
  Arcadia_PDLC_Symbol* type = (void*)Arcadia_Languages_Scope_lookup(thread, context->scope, name, false);
  Arcadia_PDLC_Symbol* module = (void*)Arcadia_Languages_Scope_lookup(thread, context->moduleScope, name, false);
  Arcadia_Tests_assertTrue(thread, type && module && type != module);
  Arcadia_Tests_assertTrue(thread, type->kind == Arcadia_PDLC_SymbolKind_Class && module->kind == Arcadia_PDLC_SymbolKind_Module);
  Arcadia_Tests_assertTrue(thread, type->enclosing == module);
  // Re-running entry must reset both domains without dropping runtime types.
  Arcadia_PDLC_EnterPhase_run(thread, Arcadia_PDLC_EnterPhase_getInstance(thread, context));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics));
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Scope_lookup(thread, context->scope,
    Arcadia_String_createFromCxxString(thread, "Arcadia.Object"), false) != NULL);
  Arcadia_ValueStack_popValues(thread, 3);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (argc != 2) return EXIT_FAILURE;
  directory = argv[1];
  return Arcadia_Tests_safeExecute(generate) ? EXIT_SUCCESS : EXIT_FAILURE;
}
