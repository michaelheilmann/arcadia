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
expectAndNext
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* scanner,
    Arcadia_DDL_WordType tokenType,
    Arcadia_Natural8Value const* wordTextBytes,
    Arcadia_SizeValue wordTextNumberOfBytes
  )
{
  Arcadia_Integer32Value receivedTokenType = Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)scanner);
  if (tokenType != receivedTokenType) {
    Arcadia_logf(Arcadia_LogFlags_Error, "expected token type %d, received %d\n", tokenType, receivedTokenType);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* wordText = Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)scanner);
  if (Arcadia_String_getNumberOfBytes(thread, wordText) != wordTextNumberOfBytes ||
      Arcadia_Memory_compare(thread, Arcadia_String_getBytes(thread, wordText), wordTextBytes, wordTextNumberOfBytes)) {
    Arcadia_logf(Arcadia_LogFlags_Error, "expected token text `%.*s`, received `%.*s`\n", (int)wordTextNumberOfBytes, wordTextBytes, (int)Arcadia_String_getNumberOfBytes(thread, wordText), Arcadia_String_getBytes(thread, wordText));
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)scanner);
}

static void
testScanner1
  (
    Arcadia_Thread* thread
  )
{
  static const char* input =
    u8""
    ;
  Arcadia_DDL_Scanner* scanner = Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                    Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread)));
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)scanner, (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, input, strlen(input))))));

  expectAndNext(thread, scanner, Arcadia_DDL_WordType_StartOfInput, u8"<start of input>", sizeof(u8"<start of input>") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_EndOfInput, u8"<end of input>", sizeof(u8"<end of input>") - 1);
}

static void
testScanner2
  (
    Arcadia_Thread* thread
  )
{
  static const char* input =
    u8","
    u8":"
    u8"{"
    u8"}"
    u8"["
    u8"]"
    u8"\r "
    u8"\n "
    u8"\r\n"
    u8"\"Hello, World!\\n\" "
    u8"true "
    u8"false "
    u8"void "
    u8"1 "
    u8"6.2831 "
    u8"+1 "
    u8"-1 "
    u8"1. "
    u8".1 "
    u8"+.1 "
    u8"-.1"
    ;
  Arcadia_DDL_Scanner* scanner = Arcadia_DDL_Scanner_create(thread, Arcadia_Languages_StringTable_getOrCreate(thread),
                                                                    Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread)));
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, input, strlen(input)))));
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)scanner, reader);

  expectAndNext(thread, scanner, Arcadia_DDL_WordType_StartOfInput, u8"<start of input>", sizeof(u8"<start of input>") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_Comma, u8",", sizeof(u8",") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_Colon, u8":", sizeof(u8":") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_LeftCurlyBracket, u8"{", sizeof(u8"{") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RightCurlyBracket, u8"}", sizeof(u8"}") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_LeftSquareBracket, u8"[", sizeof(u8"[") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RightSquareBracket, u8"]", sizeof(u8"]") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_LineTerminator, u8"\r", sizeof(u8"\r") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_LineTerminator, u8"\n", sizeof(u8"\n") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_LineTerminator, u8"\r\n", sizeof(u8"\r\n") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_StringLiteral, u8"Hello, World!\n", sizeof(u8"Hello, World!\n") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_BooleanLiteral, u8"true", sizeof(u8"true") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_BooleanLiteral, u8"false", sizeof(u8"false") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_VoidLiteral, u8"void", sizeof(u8"void") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_IntegerLiteral, u8"1", sizeof(u8"1") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RealLiteral, u8"6.2831", sizeof(u8"6.2831") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_IntegerLiteral, u8"+1", sizeof(u8"+1") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_IntegerLiteral, u8"-1", sizeof(u8"-1") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RealLiteral, u8"1.", sizeof(u8"1.") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RealLiteral, u8".1", sizeof(u8".1") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RealLiteral, u8"+.1", sizeof(u8"+.1") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_WhiteSpace, u8" ", sizeof(u8" ") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_RealLiteral, u8"-.1", sizeof(u8"-.1") - 1);
  expectAndNext(thread, scanner, Arcadia_DDL_WordType_EndOfInput, u8"<end of input>", sizeof(u8"<end of input>") - 1);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testScanner1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testScanner2)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
