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

#define ARCADIA_MILC_PRIVATE (1)
#include "Arcadia/MILC/Scanner.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/Keywords.h"

// This scanner maintains a tuple (i,n,s):
// i is the zero-based index of the Byte at which the current UTF8 symbol starts in the input stream.
// n is the length, in Bytes, of the current UTF8 symbol in the input stream.
// s denotes the current symbol.
//
// There are three symbols different from all UTF8 symbols. These are START, END, and ERROR each of a size, in Bytes, of 0.
//
// The initial values of a scanner is (0,0,START).
//
// When the scanner is in some state (i,n,s) and is advanced to the next symbol,
// then there are two possible outcomes:
// - if the next 1-4 Bytes form a valid next symbol:
//   i is incremented by the number of Bytes of the old symbol, n is assigned the size, in Bytes, of the next symbol, and s is assigned the next symbol.
// - the next 1-4 Bytes do not form a valid next symbol:
//   i is not incremented, n is assigned the size, in Bytes, of the ERROR symbol, and s is assigned the ERROR symbol.
// - there is no next symbol (that is, the end of the input was reached)
//   i is not incremented, n is assigned the size, in Bytes, of the EDN symbol, and s is assigned the END symbol.
//
// The scanner writes the lexeme to a string buffer.
// This allows for efficient creation of strings (see Arcadia_String_create).
// Furthermore, it avoids to create a string twice by using a string table (see Arcadia_Languages_StringTable).

#define CodePoint_Start (Arcadia_Unicode_CodePoint_Last + 1)
#define CodePoint_End (Arcadia_Unicode_CodePoint_Last + 2)
#define CodePoint_Error (Arcadia_Unicode_CodePoint_Last + 3)

struct Arcadia_MILC_ScannerDispatch {
  Arcadia_Languages_ScannerDispatch _parent;
};

struct Arcadia_MILC_Scanner {
  Arcadia_Languages_Scanner _parent;
  // The absolute file path of the file being parsed.
  Arcadia_Languages_InputFile* file;
  // The current symbol "s".
  Arcadia_Natural32Value symbol;
  // The input Unicode code point reader.
  Arcadia_UnicodeCodePointReader* input;
  // The context.
  Arcadia_MILC_Context* context;
  // The diagnostics.
  Arcadia_Languages_Diagnostics* diagnostics;
  // The string table.
  Arcadia_Languages_StringTable* stringTable;
  // The keywords.
  Arcadia_MILC_Keywords* keywords;
  struct {
    /// The text of the word.
    Arcadia_StringBuilder* text;
    /// The type of the word.
    Arcadia_Integer32Value type;
    /// The start of the word as an offset in Bytes from the beginning of the input.
    Arcadia_Natural32Value start;
    /// The length of the word in Bytes.
    Arcadia_Natural32Value length;
  } word;
};

static void
Arcadia_MILC_Scanner_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
Arcadia_MILC_Scanner_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_ScannerDispatch* self
  );

static void
Arcadia_MILC_Scanner_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
Arcadia_MILC_Scanner_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_UnicodeCodePointReader*
Arcadia_MILC_Scanner_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_Languages_StringTable*
Arcadia_MILC_Scanner_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_Languages_Diagnostics*
Arcadia_MILC_Scanner_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_String*
Arcadia_MILC_Scanner_getWordTextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_Integer32Value
Arcadia_MILC_Scanner_getWordTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_Natural32Value
Arcadia_MILC_Scanner_getWordStartImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_Natural32Value
Arcadia_MILC_Scanner_getWordLengthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
Arcadia_MILC_Scanner_stepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
Arcadia_MILC_Scanner_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_UnicodeCodePointReader* input
  );

static void
write
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_Natural32Value codePoint
  );

static void
save
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
saveAndNext
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static void
onEndToken
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_MILC_WordType type
  );

static Arcadia_BooleanValue
isAlphabetic
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_BooleanValue
isDigit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

static Arcadia_MILC_Scanner*
Arcadia_MILC_Scanner_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_Scanner_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_Scanner_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_Scanner_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_Scanner_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.Scanner", Arcadia_MILC_Scanner,
                         u8"Arcadia.Languages.Scanner", Arcadia_Languages_Scanner,
                         &_typeOperations);

static void
Arcadia_MILC_Scanner_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Scanner);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->file = NULL;
  //
  self->word.type = Arcadia_MILC_WordType_StartOfInput;
  self->word.text = NULL;
  self->word.start = 0;
  self->word.length = 0;
  self->stringTable = NULL;
  self->keywords = NULL;
  self->input = NULL;
  self->symbol = CodePoint_Start;
  // 
  self->context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Context_getType(thread));
  self->stringTable = self->context->stringTable;
  self->diagnostics = self->context->diagnostics;
  //
  self->keywords = Arcadia_MILC_Keywords_create(thread, self->context);
  //
  self->word.type = Arcadia_MILC_WordType_StartOfInput;
  self->word.start = 0;
  self->word.length = 0;
  self->input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, u8"", sizeof(u8"")))));
  self->word.text = Arcadia_StringBuilder_create(thread);
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<start of input>");
  //
  Arcadia_StringBuilder* temporary = Arcadia_StringBuilder_create(thread);
#define On(text, type) \
  { \
    Arcadia_StringBuilder_clear(thread, temporary); \
    Arcadia_StringBuilder_insertBackCxxString(thread, temporary, text); \
    Arcadia_MILC_Keywords_add(thread, self->keywords, Arcadia_Languages_StringTable_getOrCreateString(thread, self->stringTable, temporary), Arcadia_Value_makeNatural32Value(Arcadia_MILC_WordType_##type)); \
  }
  //
  On(u8"class", Class);
  On(u8"constructor", Constructor);
  On(u8"enumeration", Enumeration);
  On(u8"entry", Entry)
  On(u8"extends", Extends);
  On(u8"implements", Implements);
  On(u8"method", Method);
  On(u8"module", Module);
  On(u8"procedure", Procedure);
  //
  On(u8"native", Native);
  //
  On(u8"return", Return);
  //
  On(u8"variable", Variable);
  //
  On(u8"jump", Jump);
  On(u8"jumpIfTrue", JumpIfTrue);
  On(u8"jumpIfFalse", JumpIfFalse);
  //
  On(u8"loadArgument", LoadArgument);
  On(u8"storeArgument", StoreArgument);
  On(u8"loadField", LoadField);
  On(u8"storeField", StoreField)
  //
  On(u8"raise", Raise);
  // arithmetic operations
  On(u8"add", Add);
  On(u8"subtract", Subtract);
  On(u8"multiply", Multiply);
  On(u8"divide", Divide);
  // arithmetic operations/logical operations
  On(u8"negate", Negate);
  // logcial operations
  On(u8"not", Not);
  On(u8"and", And);
  On(u8"or", Or);
  // relational operations
  On(u8"isEqualTo", IsEqualTo);
  On(u8"isNotEqualTo", IsNotEqualTo);
  On(u8"isLowerThan", IsLowerThan);
  On(u8"isLowerThanOrEqualTo", IsLowerThanOrEqualTo);
  On(u8"isGreaterThan", IsGreaterThan);
  On(u8"isGreaterThanOrEqualTo", IsGreaterThanOrEqualTo);
  // list operations
  On(u8"concatenate", Concatenate);
  // set operations
  On(u8"set", Set);
  // literals
  On(u8"void", VoidLiteral);
  On(u8"true", BooleanLiteral);
  On(u8"false", BooleanLiteral);
#undef On
  //
  Arcadia_LeaveConstructor(Arcadia_MILC_Scanner);
}

static void
Arcadia_MILC_Scanner_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_ScannerDispatch* self
  )
{
  ((Arcadia_Languages_ScannerDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics * (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getDiagnosticsImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getStringTableImpl;

  ((Arcadia_Languages_ScannerDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordLength = (Arcadia_Natural32Value(*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getWordLengthImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordStart = (Arcadia_Natural32Value(*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getWordStartImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordText = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getWordTextImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordType = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_getWordTypeImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*, Arcadia_UnicodeCodePointReader*)) & Arcadia_MILC_Scanner_setInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->step = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_MILC_Scanner_stepImpl;
}

static void
Arcadia_MILC_Scanner_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_Scanner_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }

  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }

  if (self->input) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->input);
  }

  if (self->word.text) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->word.text);
  }
  if (self->keywords) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->keywords);
  }

  if (self->file) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->file);
  }
}

static void
write
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_Natural32Value codePoint
  )
{
  Arcadia_StringBuilder_insertBackCodePoints(thread, self->word.text, &codePoint, Arcadia_SizeValue_Literal(1));
}

static void
save
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ write(thread, self, self->symbol); }

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  if (Arcadia_UnicodeCodePointReader_hasValue(thread, self->input)) {
    self->symbol = Arcadia_UnicodeCodePointReader_getValue(thread, self->input);
    Arcadia_SizeValue a, b;
    Arcadia_UnicodeCodePointReader_getByteRange(thread, self->input, &a, &b);
    self->word.length += b;
    Arcadia_UnicodeCodePointReader_nextValue(thread, self->input);
  } else {
    self->symbol = CodePoint_End;
  }
}

static void
saveAndNext
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  save(thread, self);
  next(thread, self);
}

static void
onEndToken
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_MILC_WordType type
  )
{
  self->word.type = type;
}

static Arcadia_BooleanValue
isAlphabetic
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  return ('A' <= self->symbol && self->symbol <= 'Z')
      || ('a' <= self->symbol && self->symbol <= 'z');
}

static Arcadia_BooleanValue
isDigit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  return ('0' <= self->symbol && self->symbol <= '9');
}

static Arcadia_MILC_Scanner*
Arcadia_MILC_Scanner_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Scanner);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_Scanner);
}

Arcadia_MILC_Scanner*
Arcadia_MILC_Scanner_getInstance
  (   
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_MILC_Scanner_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_Scanner_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_MILC_Scanner*)Arcadia_Value_getObjectReferenceValue(&v);
}

static Arcadia_UnicodeCodePointReader*
Arcadia_MILC_Scanner_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->input; }

static Arcadia_Languages_StringTable*
Arcadia_MILC_Scanner_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->stringTable; }

static Arcadia_Languages_Diagnostics*
Arcadia_MILC_Scanner_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->diagnostics; }

static Arcadia_String*
Arcadia_MILC_Scanner_getWordTextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return Arcadia_Languages_StringTable_getOrCreateString(thread, self->stringTable, self->word.text); }

static Arcadia_Integer32Value
Arcadia_MILC_Scanner_getWordTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->word.type; }

static void
onEndOfLine
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  if ('\r' == self->symbol) {
    next(thread, self);
    if ('\n' == self->symbol) {
      next(thread, self);
    }
    onEndToken(thread, self, Arcadia_MILC_WordType_LineTerminator);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<line terminator>");
    return;
  } else if ('\n' == self->symbol) {
    next(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_LineTerminator);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<line terminator>");
    return;
  }
}

static Arcadia_Natural32Value
Arcadia_MILC_Scanner_getWordStartImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->word.start; }

static Arcadia_Natural32Value
Arcadia_MILC_Scanner_getWordLengthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->word.length; }

static void
Arcadia_MILC_Scanner_stepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{
  // Prepare for next token.
  Arcadia_StringBuilder_clear(thread, self->word.text);
  self->word.start = self->word.start + self->word.length;
  self->word.length = 0;

  if (CodePoint_Start == self->symbol) {
    next(thread, self);
  }
  if (CodePoint_End == self->symbol) {
    onEndToken(thread, self, Arcadia_MILC_WordType_EndOfInput);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<end of input>");
    return;
  }
  // Whitespace :  <Whitespace> | <Tabulator>
  if (' ' == self->symbol || '\t' == self->symbol) {
    do {
      next(thread, self);
    } while (' ' == self->symbol || '\t' == self->symbol);
    onEndToken(thread, self, Arcadia_MILC_WordType_WhiteSpaces);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<whitespaces>");
    return;
  }
  // LineTerminator : <LineFeed>
  //                  <CarriageReturn>
  //                  <CarriageReturn> <LineFeed>
  if ('\r' == self->symbol) {
    next(thread, self);
    if ('\n' == self->symbol) {
      next(thread, self);
    }
    onEndToken(thread, self, Arcadia_MILC_WordType_LineTerminator);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<line terminator>");
    return;
  } else if ('\n' == self->symbol) {
    next(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_LineTerminator);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<line terminator>");
    return;
  }
  if ('=' == self->symbol) {
    // <equals sign>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_EqualsSign);
    return;
  } else if ('.' == self->symbol) {
     // <period>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_Period);
    return;
  } else if (':' == self->symbol) {
     // <colon>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_Colon);
    return;
  } else if (',' == self->symbol) {
     // <comma>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_Comma);
    return;
  } else if ('{' == self->symbol) {
     // <left curly bracket>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_LeftCurlyBracket);
    return;
  } else if ('}' == self->symbol) {
     // <right curly bracket>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_RightCurlyBracket);
    return;
  } else if ('(' == self->symbol) {
     // <left parenthesis>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_LeftParenthesis);
    return;
  } else if (')' == self->symbol) {
     // <right parenthesis>
    saveAndNext(thread, self);
    onEndToken(thread, self, Arcadia_MILC_WordType_RightParenthesis);
    return;
  } else if ('"' == self->symbol) {
    // <string>
    next(thread, self);
    Arcadia_BooleanValue lastWasSlash = Arcadia_BooleanValue_False;
    while (Arcadia_BooleanValue_True) {
      if (lastWasSlash) {
        switch (self->symbol) {
          case '"': {
            write(thread, self, '"');
            next(thread, self);
          } break;
          case '\\': {
            write(thread, self, '\\');
            next(thread, self);
          } break;
          case 'n': {
            write(thread, self, '\n');
            next(thread, self);
          } break;
          case 'r': {
            write(thread, self, '\r');
            next(thread, self);
          } break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
            Arcadia_Thread_jump(thread);
          } break;
        }
        lastWasSlash = Arcadia_BooleanValue_False;
      } else {
        if ('\\' == self->symbol) {
          lastWasSlash = Arcadia_BooleanValue_True;
          next(thread, self);
        } else if ('"' == self->symbol) {
          next(thread, self);
          break;
        } else if (CodePoint_End == self->symbol) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
          Arcadia_Thread_jump(thread);
        } else {
          saveAndNext(thread, self);
        }
      }
    }
    onEndToken(thread, self, Arcadia_MILC_WordType_StringLiteral);
    return;
  } else if ('$' == self->symbol) {
    // <register>
    saveAndNext(thread, self);
    // a <register> is one of the following:
    // a) a '$' followed by a digit between 0 and 9
    // b) a '$' followed by a digit between 1 and 9 folloed by one or more digits between 0 and 9
    // c) a '$' followed by a <name>
    if ('0' == self->symbol) {
      saveAndNext(thread, self);
    } else if ('1' <= self->symbol && self->symbol <= '9') {
      saveAndNext(thread, self);
      while (isDigit(thread, self)) {
        saveAndNext(thread, self);
      }
    } else if ('_' == self->symbol || isAlphabetic(thread, self)) {
      saveAndNext(thread, self);
      while ('_' == self->symbol || isAlphabetic(thread, self) || isDigit(thread, self)) {
        saveAndNext(thread, self);
      }
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
      Arcadia_Thread_jump(thread);
    }
    onEndToken(thread, self, Arcadia_MILC_WordType_Register);
    return;
  } else if ('_' == self->symbol || isAlphabetic(thread, self)) {
    // <name>
    saveAndNext(thread, self);
    while ('_' == self->symbol || isAlphabetic(thread, self) || isDigit(thread, self)) {
      saveAndNext(thread, self);
    }
    onEndToken(thread, self, Arcadia_MILC_WordType_Name);
    Arcadia_Value temporary = Arcadia_MILC_Keywords_scan(thread, self->keywords, Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self));
    if (!Arcadia_Value_isVoidValue(&temporary)) {
      self->word.type = Arcadia_Value_getNatural32Value(&temporary);
    }
    return;
  } else if (isDigit(thread, self)) {
    do {
      saveAndNext(thread, self);
    } while (isDigit(thread,self));
    if ('.' == self->symbol) {
      saveAndNext(thread, self);
      while (isDigit(thread, self)) {
        saveAndNext(thread, self);
      }
      if ('e' == self->symbol || 'E' == self->symbol) {
        saveAndNext(thread, self);
        if ('+' == self->symbol || '-' == self->symbol) {
          saveAndNext(thread, self);
        }
        if (!isDigit(thread, self)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
          Arcadia_Thread_jump(thread);
        }
        do {
          saveAndNext(thread, self);
        } while (isDigit(thread, self));
      }
      onEndToken(thread, self, Arcadia_MILC_WordType_RealLiteral);
    } else {
      onEndToken(thread, self, Arcadia_MILC_WordType_IntegerLiteral);
    }
  } else if ('/' == self->symbol) {
    saveAndNext(thread, self);
    if ('*' == self->symbol) {
      // multi line comment
      saveAndNext(thread, self);
      while (true) {
        if (CodePoint_End == self->symbol) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
          Arcadia_Thread_jump(thread);
        } else if ('\n' == self->symbol) {
          saveAndNext(thread, self);
        } else if ('\r' == self->symbol) {
          saveAndNext(thread, self);
          if ('\n' == self->symbol) {
            saveAndNext(thread, self);
          }
        } else if ('*' == self->symbol) {
          saveAndNext(thread, self);
          if ('/' == self->symbol) {
            saveAndNext(thread, self);
            break;
          }
        } else {
          saveAndNext(thread, self);
        }
      }
      onEndToken(thread, self, Arcadia_MILC_WordType_MultiLineComment);
    } else if ('/' == self->symbol) {
      // single line comment
      saveAndNext(thread, self);
      while (CodePoint_End != self->symbol && '\n' != self->symbol && '\r' != self->symbol) {
        saveAndNext(thread, self);
      }
      onEndToken(thread, self, Arcadia_MILC_WordType_SingleLineComment);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
      Arcadia_Thread_jump(thread);
    }
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_MILC_Scanner_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_UnicodeCodePointReader* input
  )
{
  self->input = input;
  self->symbol = CodePoint_Start;
  self->word.type = Arcadia_MILC_WordType_StartOfInput;
  self->word.start = 0;
  self->word.length = 0;
  Arcadia_StringBuilder_clear(thread, self->word.text);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<start of input>");
}

void
Arcadia_MILC_Scanner_setFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_Languages_InputFile* file
  )
{ self->file = file; }

Arcadia_Languages_InputFile*
Arcadia_MILC_Scanner_getFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->file; }

Arcadia_SizeValue
Arcadia_MILC_Scanner_getStartOffset
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  )
{ return self->word.start; }
