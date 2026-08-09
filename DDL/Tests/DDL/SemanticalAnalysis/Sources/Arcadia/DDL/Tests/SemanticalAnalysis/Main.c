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

#include "Arcadia/DDL/Include.h"

#include "Arcadia/Logging/Include.h"
#include <string.h>

static Arcadia_DDL_Node*
doSyntacticalAnalysis
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* parser,
    const char *p,
    size_t n
  )
{
  Arcadia_ByteArray* x = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, p, n));
  Arcadia_UnicodeCodePointReader* y = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, x));
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, y);
  return (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser), _Arcadia_DDL_Node_getType(thread));
}

static Arcadia_DDL_Node*
doSemanticalAnalysis
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis,
    Arcadia_DDL_Node* node
  )
{
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run(thread, semanticalAnalysis, node);
  return node;
}

static inline void
onTest
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis,
    Arcadia_DDL_Parser* parser,
    const char *text,
    Arcadia_BooleanValue expectedJump,
    Arcadia_Status expectedStatus
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_BooleanValue receivedJump = Arcadia_BooleanValue_False;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = doSyntacticalAnalysis(thread, parser, text, strlen(text));
    node = doSemanticalAnalysis(thread, semanticalAnalysis, node);
    Arcadia_Thread_popJumpTarget(thread);
    receivedJump = Arcadia_BooleanValue_False;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    receivedJump = Arcadia_BooleanValue_True;
  }
  Arcadia_Tests_assertTrue(thread, receivedJump == expectedJump);
  Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == expectedStatus);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DDL_Parser* syntacticalAnalyser = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                                                 Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis = Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create(thread);
  onTest(thread, semanticalAnalysis, syntacticalAnalyser, u8"{ prename : \"Michael\", surname: \"Heilmann\" }", Arcadia_BooleanValue_False, Arcadia_Status_Success);
  onTest(thread, semanticalAnalysis, syntacticalAnalyser, u8"{ prename : \"Michael\", surname: \"Heilmann\", prename : \"Martin\" }", Arcadia_BooleanValue_True, Arcadia_Status_SemanticalError);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
