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

static void
assertText
  (
    Arcadia_Thread* thread,
    Arcadia_String* v,
    const char* p,
    size_t n
  )
{
  if (Arcadia_String_getNumberOfBytes(thread, v) != n) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Memory_compare(thread, Arcadia_String_getBytes(thread, v), p, n)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_DDL_Node*
parse
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Parser* parser,
    const char *p,
    size_t n
  )
{
  Arcadia_ByteArray* byteArray = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, p, n));
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, byteArray));
  Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)parser, reader);
  return (Arcadia_DDL_Node*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)parser), _Arcadia_DDL_Node_getType(thread));
}

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DDL_Parser* parser = Arcadia_DDL_Parser_create(thread, Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                                                    Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread))));
  Arcadia_DDL_Node* node = NULL;



  node = parse(thread, parser, u8"true", sizeof(u8"true") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Boolean);
  assertText(thread, ((Arcadia_DDL_BooleanNode*)node)->value, u8"true", sizeof(u8"true") - 1);

  node = parse(thread, parser, u8"false", sizeof(u8"false") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Boolean);
  assertText(thread, ((Arcadia_DDL_BooleanNode*)node)->value, u8"false", sizeof(u8"false") - 1);



  node = parse(thread, parser, u8"void", sizeof(u8"void") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Void);
  assertText(thread, ((Arcadia_DDL_VoidNode*)node)->value, u8"void", sizeof(u8"void") - 1);



  node = parse(thread, parser, u8"1", sizeof(u8"1") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"1", sizeof(u8"1") - 1);

  node = parse(thread, parser, u8"+1", sizeof(u8"+1") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"+1", sizeof(u8"+1") - 1);

  node = parse(thread, parser, u8"-1", sizeof(u8"-1") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"-1", sizeof(u8"-1") - 1);

  node = parse(thread, parser, u8"+5012", sizeof(u8"+5012") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"+5012", sizeof(u8"+5012") - 1);

  node = parse(thread, parser, u8"-5012", sizeof(u8"-5012") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"-5012", sizeof(u8"-5012") - 1);



  node = parse(thread, parser, u8"-1.0e-10", sizeof(u8"-1.0e-10") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"-1.0e-10", sizeof(u8"-1.0e-10") - 1);

  node = parse(thread, parser, u8"1.", sizeof(u8"1.") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"1.", sizeof(u8"1.") - 1);

  node = parse(thread, parser, u8".1", sizeof(u8".1") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8".1", sizeof(u8".1") - 1);

  node = parse(thread, parser, u8"+.1", sizeof(u8"+.1") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"+.1", sizeof(u8"+.1") - 1);

  node = parse(thread, parser, u8"-.1", sizeof(u8"-.1") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Number);
  assertText(thread, ((Arcadia_DDL_NumberNode*)node)->value, u8"-.1", sizeof(u8"-.1") - 1);


  // https://www.compart.com/en/unicode/U+1D28
  node = parse(thread, parser, u8"\"\\u1d28\"", sizeof(u8"\"\\u1d28\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, u8"\u1d28", sizeof(u8"\u1d28") - 1);

  node = parse(thread, parser, u8"\"Hello, World!\"", sizeof(u8"\"Hello, World!\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, u8"Hello, World!", sizeof(u8"Hello, World!") - 1);

  node = parse(thread, parser, u8"\"\\n\\r\\t\\v\\0\\\\\"", sizeof(u8"\"\\n\\r\\t\\v\\0\\\\\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, u8"\n\r\t\v\0\\", sizeof(u8"\n\r\t\v\0\\") - 1);



  node = parse(thread, parser, u8"[]", sizeof(u8"[]") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_List);

  node = parse(thread, parser, u8"[ 1 ]", sizeof(u8"[ 1 ]") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_List);

  node = parse(thread, parser, u8"[ 1, ]", sizeof(u8"[ 1, ]") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_List);



  node = parse(thread, parser, u8"{}", sizeof(u8"{}") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Map);

  node = parse(thread, parser, u8"{ x : 1 }", sizeof(u8"{ x : 1 }") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Map);

  node = parse(thread, parser, u8"{ x : 1, }", sizeof(u8"{ x : 1, }") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Map);
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
