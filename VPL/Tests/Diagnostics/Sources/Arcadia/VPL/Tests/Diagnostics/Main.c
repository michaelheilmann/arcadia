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

#include "Arcadia/VPL/Include.h"

#include <string.h>

static void
testDiagnosticObject
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Context* context = Arcadia_VPL_Context_create(thread);
  Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  Arcadia_Tests_assertTrue(thread, context != NULL);
  Arcadia_Tests_assertTrue(thread, diagnostics != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics) == Arcadia_BooleanValue_False);

  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, u8"X");
  Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic* diagnostic =
    Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, name);
  Arcadia_Languages_Diagnostics_add(thread, diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics) == Arcadia_BooleanValue_True);

  Arcadia_String* message = Arcadia_Languages_Diagnostic_getMessage(thread, (Arcadia_Languages_Diagnostic*)diagnostic);
  const char* expected = "semantical error: symbol `X` already defined\n";
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, message, expected, strlen(expected)));
}

static Arcadia_VPL_Tree_VariableDefnNode*
findConstantDefn
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_List* elements = program->programDefnNode->elements;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)elements); i < n; ++i) {
    Arcadia_VPL_Tree_Node* node = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, elements, i, _Arcadia_VPL_Tree_Node_getType(thread));
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_VariableDefnNode_getType(thread))) {
      Arcadia_VPL_Tree_VariableDefnNode* defn = (Arcadia_VPL_Tree_VariableDefnNode*)node;
      if (Arcadia_VPL_Tree_VariableDefnNode_isConstant(thread, defn)) {
        return defn;
      }
    }
  }
  return NULL;
}

static void
testPhaseReportsThroughContext
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Symbols_Program* program = Arcadia_VPL_Symbols_Program_createProgram(thread, Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor);
  Arcadia_VPL_Context* context = Arcadia_VPL_Symbols_Program_getContext(thread, program);
  Arcadia_Tests_assertTrue(thread, context != NULL);
  Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  Arcadia_Tests_assertTrue(thread, diagnostics != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics) == Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_EnterPhase_getDiagnostics(thread, Arcadia_VPL_Context_getEnterPhase(thread, context)) == diagnostics);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_ResolvePhase_getDiagnostics(thread, Arcadia_VPL_Context_getResolvePhase(thread, context)) == diagnostics);

  Arcadia_VPL_Tree_VariableDefnNode* original = findConstantDefn(thread, program);
  Arcadia_Tests_assertTrue(thread, original != NULL);
  Arcadia_VPL_Tree_VariableDefnNode* duplicate = Arcadia_VPL_Tree_VariableDefnNode_create(thread, original->name, original->type);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)original->modifiers); i < n; ++i) {
    Arcadia_List_insertBack(thread, duplicate->modifiers, Arcadia_List_getAt(thread, original->modifiers, i));
  }
  Arcadia_List_insertBackObjectReferenceValue(thread, program->programDefnNode->elements, (Arcadia_Object*)duplicate);

  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_VPL_EnterPhase_run(thread, Arcadia_VPL_Context_getEnterPhase(thread, context), program);
  } else {
    raised = Arcadia_BooleanValue_True;
  }
  Arcadia_Thread_popJumpTarget(thread);

  Arcadia_Tests_assertTrue(thread, raised == Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics) == Arcadia_BooleanValue_True);
  Arcadia_Thread_setStatus(thread, 0);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testDiagnosticObject)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testPhaseReportsThroughContext)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
