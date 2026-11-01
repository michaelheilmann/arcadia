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

#include "Arcadia/Logging/Include.h"
#include <string.h>
#include <stdio.h>

static void
assertText
  (
    Arcadia_Thread* thread,
    Arcadia_String* v,
    const char* p,
    size_t n
  )
{
  Arcadia_Tests_assertEqualBytes(thread, p, n, Arcadia_String_getBytes(thread, v), Arcadia_String_getNumberOfBytes(thread, v));
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
  node = parse(thread, parser, u8"\"\\u{1d28}\"", sizeof(u8"\"\\u{1d28}\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, u8"\u1d28", sizeof(u8"\u1d28") - 1);

  node = parse(thread, parser, u8"\"Hello, World!\"", sizeof(u8"\"Hello, World!\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, u8"Hello, World!", sizeof(u8"Hello, World!") - 1);

  node = parse(thread, parser, u8"\"\\n\\r\\t\\v\\0\\\\\"", sizeof(u8"\"\\n\\r\\t\\v\\0\\\\\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, u8"\n\r\t\v\0\\", sizeof(u8"\n\r\t\v\0\\") - 1);

  node = parse(thread, parser, "\"\\u{0}A\"", sizeof("\"\\u{0}A\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, "\0A", 2);

  node = parse(thread, parser, "\"A\\tB\\n\"", sizeof("\"A\\tB\\n\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, "A\tB\n", 4);

  node = parse(thread, parser, "\"\\0\"", sizeof("\"\\0\"") - 1);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_String);
  assertText(thread, ((Arcadia_DDL_StringNode*)node)->value, "\0", 1);



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

typedef struct ContractCase {
  const char* text;
  Arcadia_Status status;
  Arcadia_DDL_NodeType type;
  const char* value;
} ContractCase;

static void
testContract
  (
    Arcadia_Thread* thread
  )
{
  static const ContractCase cases[] = {
    { "true", Arcadia_Status_Success, Arcadia_DDL_NodeType_Boolean, "true" },
    { "false", Arcadia_Status_Success, Arcadia_DDL_NodeType_Boolean, "false" },
    { "void", Arcadia_Status_Success, Arcadia_DDL_NodeType_Void, "void" },
    { "True", Arcadia_Status_SyntacticalError },
    { "null", Arcadia_Status_SyntacticalError },
    { "", Arcadia_Status_SyntacticalError },
    { "true false", Arcadia_Status_SyntacticalError },
    { "true, false", Arcadia_Status_SyntacticalError },
    { "true,", Arcadia_Status_SyntacticalError },
    { "1,", Arcadia_Status_SyntacticalError },
    { "\"text\",", Arcadia_Status_SyntacticalError },
    { "void,", Arcadia_Status_SyntacticalError },
    { "[],", Arcadia_Status_SyntacticalError },
    { "{},", Arcadia_Status_SyntacticalError },
    { "[1,],", Arcadia_Status_SyntacticalError },
    { "{x:1,},", Arcadia_Status_SyntacticalError },
    { "true /* before comma */ , // after comma\n", Arcadia_Status_SyntacticalError },
    { "[1,] // before comma\n, /* after comma */", Arcadia_Status_SyntacticalError },
    { "true \t\r\n/* after value */ // final comment", Arcadia_Status_Success, Arcadia_DDL_NodeType_Boolean, "true" },
    { "[1,] /* after list */", Arcadia_Status_Success, Arcadia_DDL_NodeType_List },
    { "{x:1,} // after map", Arcadia_Status_Success, Arcadia_DDL_NodeType_Map },
    { "true,,", Arcadia_Status_SyntacticalError },
    { "[]", Arcadia_Status_Success, Arcadia_DDL_NodeType_List },
    { "{}", Arcadia_Status_Success, Arcadia_DDL_NodeType_Map },
    { "[1,]", Arcadia_Status_Success, Arcadia_DDL_NodeType_List },
    { "{x:1,}", Arcadia_Status_Success, Arcadia_DDL_NodeType_Map },
    { "[1 2]", Arcadia_Status_SyntacticalError },
    { "[,]", Arcadia_Status_SyntacticalError },
    { "{,}", Arcadia_Status_SyntacticalError },
    { "{x 1}", Arcadia_Status_SyntacticalError },
    { "{\"x\":1}", Arcadia_Status_SyntacticalError },
    { "{true:1}", Arcadia_Status_SyntacticalError },
    { "{false:1}", Arcadia_Status_SyntacticalError },
    { "{void:1}", Arcadia_Status_SyntacticalError },
    { "{1name:1}", Arcadia_Status_SyntacticalError },
    { "{:1}", Arcadia_Status_SyntacticalError },
    { "{\"_1\":1}", Arcadia_Status_SyntacticalError },
    { "{\xc3\xa9:1}", Arcadia_Status_LexicalError },
    { "{_x2:1, X:2}", Arcadia_Status_Success, Arcadia_DDL_NodeType_Map },
    { "{_:1}", Arcadia_Status_Success, Arcadia_DDL_NodeType_Map },
    { "/* before */ \t\r\n[true, // entry\n void] // after", Arcadia_Status_Success, Arcadia_DDL_NodeType_List },
    { "/* unfinished", Arcadia_Status_LexicalError },
    { "\vtrue", Arcadia_Status_LexicalError },
    { "01", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "01" },
    { "+1", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "+1" },
    { "-1.0e-10", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "-1.0e-10" },
    { "1.", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "1." },
    { ".1", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, ".1" },
    { "1E+2", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "1E+2" },
    { "123456789012345678901234567890", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "123456789012345678901234567890" },
    { "1e", Arcadia_Status_LexicalError },
    { ".", Arcadia_Status_LexicalError },
    { "0x10", Arcadia_Status_SyntacticalError },
    { "1x", Arcadia_Status_SyntacticalError },
    { "+", Arcadia_Status_LexicalError },
    { "-", Arcadia_Status_LexicalError },
    { "+e2", Arcadia_Status_LexicalError },
    { "-E2", Arcadia_Status_LexicalError },
    { "[+]", Arcadia_Status_LexicalError },
    { "{x:-}", Arcadia_Status_LexicalError },
    { "1e2", Arcadia_Status_Success, Arcadia_DDL_NodeType_Number, "1e2" },
    { "\"\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "" },
    { "\"Stone Lantern\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "Stone Lantern" },
    { "\"It's ready\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "It's ready" },
    { "\"He said \\\"hello\\\"\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "He said \"hello\"" },
    { "\"\\u{41}\\n\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "A\n" },
    { "\"\\u{004a}\\u{004A}\\u{004f}\\u{004F}\\u{0030}\\u{0039}\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "JJOO09" },
    { "\"\\u{000041}0\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "A0" },
    { "\"\\\\u0041\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\\u0041" },
    { "\"\\u{7f}\\u{80}\\u{7ff}\\u{800}\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\x7f\xc2\x80\xdf\xbf\xe0\xa0\x80" },
    { "\"\\u{D7FF}\\u{E000}\\u{FFFF}\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\xed\x9f\xbf\xee\x80\x80\xef\xbf\xbf" },
    { "\"\\u{10000}\\u{1F3EE}\\u{10FFFF}\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\xf0\x90\x80\x80\xf0\x9f\x8f\xae\xf4\x8f\xbf\xbf" },
    { "\"\\b\\f\\t\\v\\\\\\\"\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\b\f\t\v\\\"" },
    { "\"\xc3\xa9\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\xc3\xa9" },
    { "\"\xf0\x9f\x8f\xae\"", Arcadia_Status_Success, Arcadia_DDL_NodeType_String, "\xf0\x9f\x8f\xae" },
    { "\"\xed\xa0\x80\"", Arcadia_Status_LexicalError },
    { "\"\xf4\x90\x80\x80\"", Arcadia_Status_LexicalError },
    { "'text'", Arcadia_Status_LexicalError },
    { "''", Arcadia_Status_LexicalError },
    { "'a'", Arcadia_Status_LexicalError },
    { "'\\u{41}'", Arcadia_Status_LexicalError },
    { "\"text'", Arcadia_Status_LexicalError },
    { "\"It\\'s ready\"", Arcadia_Status_LexicalError },
    { "\"\\q\"", Arcadia_Status_LexicalError },
    { "\"\\d\"", Arcadia_Status_LexicalError },
    { "\"\\u12\"", Arcadia_Status_LexicalError },
    { "\"\\u\"", Arcadia_Status_LexicalError },
    { "\"\\u1\"", Arcadia_Status_LexicalError },
    { "\"\\u123\"", Arcadia_Status_LexicalError },
    { "\"raw\nline\"", Arcadia_Status_LexicalError },
    { "\"unfinished", Arcadia_Status_LexicalError },
    // Legacy escapes are rejected rather than accepted as an alternate spelling.
    { "\"\\u0041\"", Arcadia_Status_LexicalError },
    { "\"\\u00410\"", Arcadia_Status_LexicalError },
    { "\"\\u1F3EE\"", Arcadia_Status_LexicalError },
    { "\"\\uG000\"", Arcadia_Status_LexicalError },
    { "\"\\u0G00\"", Arcadia_Status_LexicalError },
    { "\"\\u00G0\"", Arcadia_Status_LexicalError },
    { "\"\\u000G\"", Arcadia_Status_LexicalError },
    { "\"\\u000Z\"", Arcadia_Status_LexicalError },
    { "\"\\u000g\"", Arcadia_Status_LexicalError },
    { "\"\\u000z\"", Arcadia_Status_LexicalError },
    { "\"\\u000@\"", Arcadia_Status_LexicalError },
    { "\"\\u000`\"", Arcadia_Status_LexicalError },
    { "\"\\u000/\"", Arcadia_Status_LexicalError },
    { "\"\\u000:\"", Arcadia_Status_LexicalError },
    { "\"\\u{}\"", Arcadia_Status_LexicalError },
    { "\"\\u{1234567}\"", Arcadia_Status_LexicalError },
    { "\"\\u{0000041}\"", Arcadia_Status_LexicalError },
    { "\"\\u{110000}\"", Arcadia_Status_LexicalError },
    { "\"\\u{D800}\"", Arcadia_Status_LexicalError },
    { "\"\\u{DFFF}\"", Arcadia_Status_LexicalError },
    { "\"\\u{D83C}\\u{DFEE}\"", Arcadia_Status_LexicalError },
    { "\"\\u{G000}\"", Arcadia_Status_LexicalError },
    { "\"\\u{0G00}\"", Arcadia_Status_LexicalError },
    { "\"\\u{00G0}\"", Arcadia_Status_LexicalError },
    { "\"\\u{000G}\"", Arcadia_Status_LexicalError },
    { "\"\\u{0000G}\"", Arcadia_Status_LexicalError },
    { "\"\\u{00000G}\"", Arcadia_Status_LexicalError },
    { "\"\\u{g}\"", Arcadia_Status_LexicalError },
    { "\"\\u{Z}\"", Arcadia_Status_LexicalError },
    { "\"\\u{z}\"", Arcadia_Status_LexicalError },
    { "\"\\u{@}\"", Arcadia_Status_LexicalError },
    { "\"\\u{`}\"", Arcadia_Status_LexicalError },
    { "\"\\u{/}\"", Arcadia_Status_LexicalError },
    { "\"\\u{:}\"", Arcadia_Status_LexicalError },
    { "\"\\u{+41}\"", Arcadia_Status_LexicalError },
    { "\"\\u{ 41}\"", Arcadia_Status_LexicalError },
    { "\"\\u{4_1}\"", Arcadia_Status_LexicalError },
    { "\"\\u{41\"", Arcadia_Status_LexicalError },
    { "\"\\u{41", Arcadia_Status_LexicalError },
  };
  Arcadia_DDL_Parser* parser = Arcadia_DDL_Parser_create(thread, NULL);
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    Arcadia_JumpTarget target;
    Arcadia_Thread_pushJumpTarget(thread, &target);
    if (Arcadia_JumpTarget_save(&target)) {
      Arcadia_DDL_Node* node = parse(thread, parser, cases[i].text, strlen(cases[i].text));
      Arcadia_Thread_popJumpTarget(thread);
      if (cases[i].status != Arcadia_Status_Success) {
        fprintf(stderr, "unexpected acceptance: %s\n", cases[i].text);
      }
      Arcadia_Tests_assertEqualStatus(thread, cases[i].status, Arcadia_Status_Success);
      Arcadia_Tests_assertTrue(thread, node->type == cases[i].type);
      if (!strcmp(cases[i].text, "[]")) {
        Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_DDL_ListNode*)node)->elements));
      } else if (!strcmp(cases[i].text, "{}")) {
        Arcadia_Tests_assertEqualSize(thread, 0, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_DDL_MapNode*)node)->entries));
      }
      if (cases[i].value) {
        Arcadia_String* value = NULL;
        switch (node->type) {
          case Arcadia_DDL_NodeType_Boolean: value = ((Arcadia_DDL_BooleanNode*)node)->value; break;
          case Arcadia_DDL_NodeType_Number: value = ((Arcadia_DDL_NumberNode*)node)->value; break;
          case Arcadia_DDL_NodeType_String: value = ((Arcadia_DDL_StringNode*)node)->value; break;
          case Arcadia_DDL_NodeType_Void: value = ((Arcadia_DDL_VoidNode*)node)->value; break;
        }
        Arcadia_Tests_assertTrue(thread, value != NULL);
        assertText(thread, value, cases[i].value, strlen(cases[i].value));
      }
    } else {
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      if (status != cases[i].status || cases[i].status == Arcadia_Status_Success) {
        fprintf(stderr, "unexpected failure: %s (status %d)\n", cases[i].text, (int)status);
      }
      Arcadia_Tests_assertTrue(thread, cases[i].status != Arcadia_Status_Success);
      Arcadia_Tests_assertEqualStatus(thread, cases[i].status, status);
    }
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_Success, Arcadia_Thread_getStatus(thread));
  }
}

static void
testMapNames
  (
    Arcadia_Thread* thread
  )
{
  const char* text = "{name:1, _:1, __:1, _1:1, layer_2:1, A0_z9:1, true_:1, false1:1, void_:1, True:1}";
  const char* keys[] = { "name", "_", "__", "_1", "layer_2", "A0_z9", "true_", "false1", "void_", "True" };
  Arcadia_DDL_Parser* parser = Arcadia_DDL_Parser_create(thread, NULL);
  Arcadia_DDL_Node* node = parse(thread, parser, text, strlen(text));
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Map);
  Arcadia_DDL_MapNode* map = (Arcadia_DDL_MapNode*)node;
  const Arcadia_SizeValue count = sizeof(keys) / sizeof(keys[0]);
  Arcadia_Tests_assertEqualSize(thread, count, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)map->entries));
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, map->entries, i);
    assertText(thread, entry->key->value, keys[i], strlen(keys[i]));
    Arcadia_Tests_assertTrue(thread, entry->value->type == Arcadia_DDL_NodeType_Number);
    assertText(thread, ((Arcadia_DDL_NumberNode*)entry->value)->value, "1", 1);
  }
}

static void
testFirstDocument
  (
    Arcadia_Thread* thread
  )
{
  const char* text =
    "{\n"
    "  name: \"Stone Lantern\",\n"
    "  enabled: true,\n"
    "  size: 1.5,\n"
    "  layers: [\"stone\", \"light\",],\n"
    "  details: {note: void,},\n"
    "}";
  Arcadia_ByteArray* bytes = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, text, strlen(text)));
  Arcadia_UnicodeCodePointReader* input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, bytes));
  Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, Arcadia_DDL_DefaultReader_create(thread), input);
  Arcadia_Tests_assertTrue(thread, node->type == Arcadia_DDL_NodeType_Map);
  Arcadia_DDL_MapNode* map = (Arcadia_DDL_MapNode*)node;
  Arcadia_Tests_assertEqualSize(thread, 5, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)map->entries));
  const char* keys[] = { "name", "enabled", "size", "layers", "details" };
  Arcadia_DDL_NodeType types[] = { Arcadia_DDL_NodeType_String, Arcadia_DDL_NodeType_Boolean,
    Arcadia_DDL_NodeType_Number, Arcadia_DDL_NodeType_List, Arcadia_DDL_NodeType_Map };
  for (Arcadia_SizeValue i = 0; i < 5; ++i) {
    Arcadia_DDL_MapEntryNode* entry = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, map->entries, i);
    assertText(thread, entry->key->value, keys[i], strlen(keys[i]));
    Arcadia_Tests_assertTrue(thread, entry->value->type == types[i]);
  }
  Arcadia_DDL_MapEntryNode* layers = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, map->entries, 3);
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_DDL_ListNode*)layers->value)->elements));
  Arcadia_DDL_MapEntryNode* details = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, map->entries, 4);
  Arcadia_DDL_MapEntryNode* note = (Arcadia_DDL_MapEntryNode*)Arcadia_List_getObjectReferenceValueAt(thread, ((Arcadia_DDL_MapNode*)details->value)->entries, 0);
  assertText(thread, note->key->value, "note", 4);
  Arcadia_Tests_assertTrue(thread, note->value->type == Arcadia_DDL_NodeType_Void);
  Arcadia_DataDefinitionLanguage_SemanticalAnalysis_run(thread, Arcadia_DataDefinitionLanguage_SemanticalAnalysis_create(thread), node);
}

static int
runContract
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testContract) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFirstDocument
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testFirstDocument) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runMapNames
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMapNames) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "SyntacticalAnalysis", &runTests },
    { "DocumentationContract", &runContract },
    { "MapNames", &runMapNames },
    { "FirstDocument", &runFirstDocument },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
