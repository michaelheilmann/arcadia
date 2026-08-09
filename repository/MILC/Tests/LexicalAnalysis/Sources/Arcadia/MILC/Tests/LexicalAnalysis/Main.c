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

#include "Arcadia/MILC/Include.h"

#include <string.h>

static void
expectAndNext
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* scanner,
    Arcadia_MILC_WordType tokenKind,
    Arcadia_Natural8Value const* tokenText,
    Arcadia_SizeValue tokenTextLength
  )
{
  if (tokenKind != Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)scanner)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_String_isEqualTo_pn(thread, Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)scanner), tokenText, tokenTextLength)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)scanner);
}

static void
testScanner4
  (
    Arcadia_Thread* thread
  )
{
  static const char* input =
    u8"$0\n"
    u8"$1\n"
    u8"$512\n"
    u8"$_\n"
    ;
  Arcadia_MILC_Context* context = Arcadia_MILC_Context_create(thread);
  Arcadia_MILC_Scanner* scanner = Arcadia_MILC_Scanner_getInstance(thread, context);
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)scanner, (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, input, strlen(input))))));

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_StartOfInput, u8"<start of input>", sizeof(u8"<start of input>") - 1);

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Register, u8"$0", sizeof(u8"$0") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Register, u8"$1", sizeof(u8"$1") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Register, u8"$512", sizeof(u8"$512") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Register, u8"$_", sizeof(u8"$_") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_EndOfInput, u8"<end of input>", sizeof(u8"<end of input>") - 1);
}

static void
testScanner3
  (
    Arcadia_Thread* thread
  )
{
  static const char* input =
    //
    u8"multiply"
    u8"\n"
    u8"divide"
    u8"\n"
    u8"add"
    u8"\n"
    u8"subtract"
    u8"\n"
    //
    u8"negate"
    u8"\n"
    //
    u8"concatenate"
    u8"\n"
    //
    u8"isEqualTo"
    u8"\n"
    u8"isNotEqualTo"
    u8"\n"
    u8"isLowerThan"
    u8"\n"
    u8"isLowerThanOrEqualTo"
    u8"\n"
    u8"isGreaterThan"
    u8"\n"
    u8"isGreaterThanOrEqualTo"
    u8"\n"
    //
    u8"not"
    u8"\n"
    u8"and"
    u8"\n"
    u8"or"
    u8"\n"
    //
    u8"class"
    u8"\n"
    u8"extends"
    u8"\n"
    u8"implements"
    u8"\n"
    u8"native"
    u8"\n"
    u8"procedure"
    u8"\n"
    u8"method"
    u8"\n"
    u8"constructor"
    u8"\n"
    u8"variable"
    u8"\n"
    ;
  Arcadia_MILC_Context* context = Arcadia_MILC_Context_create(thread);
  Arcadia_MILC_Scanner* scanner = Arcadia_MILC_Scanner_getInstance(thread, context);
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)scanner, (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, input, strlen(input))))));

  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_StartOfInput, u8"<start of input>", sizeof(u8"<start of input>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Multiply, u8"multiply", sizeof(u8"multiply") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Divide, u8"divide", sizeof(u8"divide") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Add, u8"add", sizeof(u8"add") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Subtract, u8"subtract", sizeof(u8"subtract") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Negate, u8"negate", sizeof(u8"negate") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Concatenate, u8"concatenate", sizeof(u8"concatenate") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IsEqualTo, u8"isEqualTo", sizeof(u8"isEqualTo") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IsNotEqualTo, u8"isNotEqualTo", sizeof(u8"isNotEqualTo") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IsLowerThan, u8"isLowerThan", sizeof(u8"isLowerThan") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IsLowerThanOrEqualTo, u8"isLowerThanOrEqualTo", sizeof(u8"isLowerThanOrEqualTo") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IsGreaterThan, u8"isGreaterThan", sizeof(u8"isGreaterThan") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IsGreaterThanOrEqualTo, u8"isGreaterThanOrEqualTo", sizeof(u8"isGreaterThanOrEqualTo") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Not, u8"not", sizeof(u8"not") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_And, u8"and", sizeof(u8"and") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Or, u8"or", sizeof(u8"or") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Class, u8"class", sizeof(u8"class") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Extends, u8"extends", sizeof(u8"extends") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Implements, u8"implements", sizeof(u8"implements") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Native, u8"native", sizeof(u8"native") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Procedure, u8"procedure", sizeof(u8"procedure") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Method, u8"method", sizeof(u8"method") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Constructor, u8"constructor", sizeof(u8"constructor") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Variable, u8"variable", sizeof(u8"variable") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  //
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_EndOfInput, u8"<end of input>", sizeof(u8"<end of input>") - 1);
}

static void
testScanner2
  (
    Arcadia_Thread* thread
  )
{
  static const char* input =
    u8"Name"
    u8"\n"
    u8"17"
    u8"\n"
    u8"17.23"
    u8"\n"
    u8"\"Hello, World!\n\""
    u8"\n"
    u8"="
    u8"\n"
    u8"//\n"
    u8"/**/"
    u8"\n"
    u8":"
    u8"\n"
    u8","
    u8"\n"
    ;
  Arcadia_MILC_Context* context = Arcadia_MILC_Context_create(thread);
  Arcadia_MILC_Scanner* scanner = Arcadia_MILC_Scanner_getInstance(thread, context);
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)scanner, (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, input, strlen(input))))));

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_StartOfInput, u8"<start of input>", sizeof(u8"<start of input>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Name, u8"Name", sizeof(u8"Name") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_IntegerLiteral, u8"17", sizeof(u8"17") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_RealLiteral, u8"17.23", sizeof(u8"17.23") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_StringLiteral, u8"Hello, World!\n", sizeof(u8"Hello, World!\n") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_EqualsSign, u8"=", sizeof(u8"=") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_SingleLineComment, u8"//", sizeof(u8"//") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_MultiLineComment, u8"/**/", sizeof(u8"/**/") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Colon, u8":", sizeof(u8":") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_Comma, u8",", sizeof(u8",") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_LineTerminator, u8"<line terminator>", sizeof(u8"<line terminator>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_EndOfInput, u8"<end of input>", sizeof(u8"<end of input>") - 1);
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
  Arcadia_MILC_Context* context = Arcadia_MILC_Context_create(thread);
  Arcadia_MILC_Scanner* scanner = Arcadia_MILC_Scanner_getInstance(thread, context);
  Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)scanner, (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, input, strlen(input))))));

  expectAndNext(thread, scanner, Arcadia_MILC_WordType_StartOfInput, u8"<start of input>", sizeof(u8"<start of input>") - 1);
  expectAndNext(thread, scanner, Arcadia_MILC_WordType_EndOfInput, u8"<end of input>", sizeof(u8"<end of input>") - 1);
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
  if (!Arcadia_Tests_safeExecute(&testScanner3)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testScanner4)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
