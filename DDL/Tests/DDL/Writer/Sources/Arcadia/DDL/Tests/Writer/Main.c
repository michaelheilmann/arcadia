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

#include <string.h>

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Logging/Include.h"

static Arcadia_DDL_Node*
doRead
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis,
    Arcadia_DDL_Parser* parser,
    const char* p,
    size_t n
  )
{
  Arcadia_ByteArray* x = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, p, n));
  Arcadia_UnicodeCodePointReader* y = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, x));
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, y);
  Arcadia_DDL_Node* node = (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser), _Arcadia_DDL_Node_getType(thread));
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run(thread, semanticalAnalysis, node);
  return node;
}

static void
doWrite
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* unparser,
    Arcadia_DDL_Node* node,
    const char* p,
    size_t n
  )
{
  Arcadia_ByteArrayBuilder* targetByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_DataDefinitionLanguage_Unparser_run(thread, unparser, node, targetByteBuffer);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, targetByteBuffer) == n);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Memory_compare(thread, Arcadia_ByteArrayBuilder_getBytes(thread, targetByteBuffer), p, n));
}

static inline void
onTest
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis,
    Arcadia_DDL_Parser* parser,
    Arcadia_DataDefinitionLanguage_Unparser* unparser,
    const char *input,
    const char* output
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = doRead(thread, semanticalAnalysis, parser, input, strlen(input));
    doWrite(thread, unparser, node, output, strlen(output));
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_jump(thread);
  }
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis* semanticalAnalysis = Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create(thread);
  Arcadia_DDL_Parser* parser = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                                    Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
  Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  Arcadia_DataDefinitionLanguage_Unparser* unparser = Arcadia_DataDefinitionLanguage_Unparser_create(thread, encoder);
  onTest(thread, semanticalAnalysis, parser, unparser,
                 u8"{ prename : \"Michael\", surname: \"Heilmann\",\n}\n",
                 u8"{\n  prename : \"Michael\",\n  surname : \"Heilmann\",\n}\n");
  onTest(thread, semanticalAnalysis, parser, unparser,
    u8"{\n  value : {\n  },\n}\n",
    u8"{\n  value : {\n  },\n}\n");
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
