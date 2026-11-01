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

#include "Arcadia/VPL/Include.h"
#include <string.h>

static Arcadia_Object*
root
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  return object;
}

static void
testDiagnosticObject
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
  Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics));
  const char expected[] = "verification error: duplicate uniform name\n";
  Arcadia_String* message = (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, expected));
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Diagnostics_VerificationDiagnostic_create(thread, message, (Arcadia_Object*)context));
  Arcadia_Languages_Diagnostics_add(thread, diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics));
  Arcadia_Tests_assertTrue(thread, diagnostic->subject == (Arcadia_Object*)context);
  Arcadia_Tests_assertTrue(thread, diagnostic->sourceName == NULL && diagnostic->line == 0 && diagnostic->column == 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, Arcadia_Languages_Diagnostic_getMessage(thread, (Arcadia_Languages_Diagnostic*)diagnostic), expected, sizeof(expected) - 1));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testVerifierReportsThroughContext
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
  Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  Arcadia_VPL_VerifiedModule* program = (Arcadia_VPL_VerifiedModule*)root(thread, (Arcadia_Object*)Arcadia_VPL_Programs_create(thread, Arcadia_VPL_ProgramFlags_MeshAmbientColor));
  Arcadia_VPL_IR_Module* module = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_VerifiedModule_materialize(thread, program, diagnostics));
  Arcadia_Object* original = Arcadia_List_getObjectReferenceValueAt(thread, module->uniforms, 0);
  Arcadia_List_insertBackObjectReferenceValue(thread, module->uniforms, original);
  Arcadia_VPL_IR_Verifier* verifier = (Arcadia_VPL_IR_Verifier*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Verifier_create(thread, diagnostics));
  Arcadia_SizeValue before = Arcadia_ValueStack_getSize(thread);
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) Arcadia_VPL_IR_Verifier_run(thread, verifier, module);
  else raised = Arcadia_BooleanValue_True;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_setStatus(thread, 0);
  Arcadia_Tests_assertTrue(thread, raised && status == Arcadia_Status_SemanticalError && before == Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics) == 1);
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, diagnostics->diagnostics, 0, _Arcadia_VPL_Diagnostics_VerificationDiagnostic_getType(thread));
  const char expected[] = "verification error: duplicate uniform name\n";
  Arcadia_Tests_assertTrue(thread, diagnostic->subject == original);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, diagnostic->message, expected, sizeof(expected) - 1));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static int
runDiagnosticObject
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDiagnosticObject) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runVerifierReportsThroughContext
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testVerifierReportsThroughContext) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "DiagnosticObject", &runDiagnosticObject },
    { "VerifierReportsThroughContext", &runVerifierReportsThroughContext },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
