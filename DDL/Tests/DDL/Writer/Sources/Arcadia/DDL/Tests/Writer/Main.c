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
  Arcadia_Tests_assertEqualBytes(thread, p, n, Arcadia_ByteArrayBuilder_getBytes(thread, targetByteBuffer), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, targetByteBuffer));
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
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_DDL_Node* node = doRead(thread, semanticalAnalysis, parser, input, strlen(input));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
    doWrite(thread, unparser, node, output, strlen(output));
    Arcadia_DDL_Node* restored = doRead(thread, semanticalAnalysis, parser, output, strlen(output));
    Arcadia_Tests_assertTrue(thread, restored->type == node->type);
    if (node->type == Arcadia_DDL_NodeType_String) {
      Arcadia_String* expected = ((Arcadia_DDL_StringNode*)node)->value;
      Arcadia_String* actual = ((Arcadia_DDL_StringNode*)restored)->value;
      Arcadia_Tests_assertEqualBytes(thread, Arcadia_String_getBytes(thread, expected), Arcadia_String_getNumberOfBytes(thread, expected),
        Arcadia_String_getBytes(thread, actual), Arcadia_String_getNumberOfBytes(thread, actual));
    }
    Arcadia_Tests_assertEqualSize(thread, stackSize + 1, Arcadia_ValueStack_getSize(thread));
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (Arcadia_ValueStack_getSize(thread) > stackSize) {
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    }
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
  onTest(thread, semanticalAnalysis, parser, unparser,
    "\"\\u{1F3EE}\"", "\"\xf0\x9f\x8f\xae\"\n");
  onTest(thread, semanticalAnalysis, parser, unparser,
    "\"\xf0\x9f\x8f\xae\"", "\"\xf0\x9f\x8f\xae\"\n");
  onTest(thread, semanticalAnalysis, parser, unparser,
    "\"\\u{0}\\u{1}\\u{1F}\\n\"", "\"\\u{0000}\\u{0001}\\u{001F}\\u{000A}\"\n");
  onTest(thread, semanticalAnalysis, parser, unparser,
    "\"It's ready\"", "\"It's ready\"\n");
  onTest(thread, semanticalAnalysis, parser, unparser,
    "\"He said \\\"hello\\\"\"", "\"He said \\\"hello\\\"\"\n");
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Writer", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
