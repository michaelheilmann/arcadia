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

#define ARCADIA_DDL_MODULE (1)
#include "Arcadia/DDL/Reader/Scanner.h"

#include "Arcadia/DDL/Include.h"

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
// Furthermore, it avoids to create a string twice by using a string table (see Arcadia_DataDefinitionLanguage_StringTable).

#define CodePoint_Start (Arcadia_Unicode_CodePoint_Last + 1)
#define CodePoint_End (Arcadia_Unicode_CodePoint_Last + 2)
#define CodePoint_Error (Arcadia_Unicode_CodePoint_Last + 3)

struct Arcadia_DDL_ScannerDispatch {
  Arcadia_Languages_ScannerDispatch _parent;
};

struct Arcadia_DDL_Scanner {
  Arcadia_Languages_Scanner _parent;
  // The string table.
  Arcadia_Languages_StringTable* stringTable;
  // The diagnostics.
  Arcadia_Languages_Diagnostics* diagnostics;

  // The current symbol.
  Arcadia_Natural32Value symbol;
  // The input Byte reader.
  Arcadia_UnicodeCodePointReader* input;
  // The keywords.
  Arcadia_DataDefinitionLanguage_Keywords* keywords;
  struct {
    /// The text of the word.
    Arcadia_StringBuilder* text;
    /// The type of the word.
    Arcadia_Natural32Value type;
    /// The start of the word as an offset in Bytes from the beginning of the input.
    Arcadia_Natural32Value start;
    /// The length of the word in Bytes.
    Arcadia_Natural32Value length;
  } word;
};

static void
Arcadia_DDL_Scanner_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
Arcadia_DDL_Scanner_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ScannerDispatch* self
  );

static void
Arcadia_DDL_Scanner_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
Arcadia_DDL_Scanner_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
write
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self,
    Arcadia_Natural32Value codePoint
  );

static void
save
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
saveAndNext
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
onEndWord
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self,
    Arcadia_DDL_WordType type
  );

static Arcadia_BooleanValue
isAlphabetic
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_BooleanValue
isHexadecimalDigit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_BooleanValue
isDecimalDigit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_UnicodeCodePointReader*
Arcadia_DDL_Scanner_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_String*
Arcadia_DDL_Scanner_getWordTextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_Integer32Value
Arcadia_DDL_Scanner_getWordTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_Natural32Value
Arcadia_DDL_Scanner_getWordStartImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_Natural32Value
Arcadia_DDL_Scanner_getWordLengthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
Arcadia_DDL_Scanner_stepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static void
Arcadia_DDL_Scanner_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self,
    Arcadia_UnicodeCodePointReader* input
  );

static Arcadia_Languages_StringTable*
Arcadia_DDL_Scanner_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static Arcadia_Languages_Diagnostics*
Arcadia_DDL_Scanner_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_Scanner_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_DDL_Scanner_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_Scanner_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_Scanner_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.Scanner", Arcadia_DDL_Scanner,
                         u8"Arcadia.Languages.Scanner", Arcadia_Languages_Scanner,
                         &_typeOperations);

static void
Arcadia_DDL_Scanner_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_Scanner);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->word.type = Arcadia_DDL_WordType_StartOfInput;
  self->word.start = 0;
  self->word.length = 0;
  self->word.text = NULL;
  self->stringTable = NULL;
  self->keywords = NULL;
  self->input = NULL;
  self->symbol = CodePoint_Start;
  //
  self->keywords = Arcadia_DataDefinitionLanguage_Keywords_create(thread);
  self->input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, u8"", sizeof(u8"") - 1))));
  self->stringTable = (Arcadia_Languages_StringTable*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Languages_StringTable_getType(thread));
  self->diagnostics = (Arcadia_Languages_Diagnostics*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_Diagnostics_getType(thread));
  //
  self->word.type = Arcadia_DDL_WordType_StartOfInput;
  self->word.text = Arcadia_StringBuilder_create(thread);
  self->word.start = 0;
  self->word.length = sizeof(u8"<start of input>") - 1;
  Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<start of input>");
  //
  Arcadia_StringBuilder* temporary = Arcadia_StringBuilder_create(thread);
#define On(text, type) \
  { \
    Arcadia_StringBuilder_clear(thread, temporary); \
    Arcadia_StringBuilder_insertBackCxxString(thread, temporary, text); \
    Arcadia_DataDefinitionLanguage_Keywords_add(thread, self->keywords, Arcadia_Languages_StringTable_getOrCreateString(thread, self->stringTable, temporary), Arcadia_DDL_WordType_##type); \
  }
  //
  // literals
  On(u8"void", VoidLiteral);
  On(u8"true", BooleanLiteral);
  On(u8"false", BooleanLiteral);
  //
#undef On
  //
  Arcadia_LeaveConstructor(Arcadia_DDL_Scanner);
}

static void
Arcadia_DDL_Scanner_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ScannerDispatch* self
  )
{
  ((Arcadia_Languages_ScannerDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getStringTableImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getDiagnosticsImpl;
  //
  ((Arcadia_Languages_ScannerDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordLength = (Arcadia_Natural32Value(*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getWordLengthImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordStart = (Arcadia_Natural32Value(*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getWordStartImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordText = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getWordTextImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordType = (Arcadia_Integer32Value (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_getWordTypeImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*, Arcadia_UnicodeCodePointReader*)) & Arcadia_DDL_Scanner_setInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->step = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_DDL_Scanner_stepImpl;
}

static void
Arcadia_DDL_Scanner_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_Scanner_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
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
}

static void
write
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self,
    Arcadia_Natural32Value codePoint
  )
{
  Arcadia_StringBuilder_insertBackCodePoints(thread, self->word.text, &codePoint, Arcadia_SizeValue_Literal(1));
}

static void
save
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ write(thread, self, self->symbol); }

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
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
    Arcadia_DDL_Scanner* self
  )
{
  save(thread, self);
  next(thread, self);
}

static void
onEndWord
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self,
    Arcadia_DDL_WordType type
  )
{
  self->word.type = type;
}

static Arcadia_BooleanValue
isAlphabetic
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
  return ('A' <= self->symbol && self->symbol <= 'Z')
      || ('a' <= self->symbol && self->symbol <= 'z');
}

static Arcadia_BooleanValue
isHexadecimalDigit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
  return ('0' <= self->symbol && self->symbol <= '9')
      || ('a' <= self->symbol && self->symbol <= 'f')
      || ('A' <= self->symbol && self->symbol <= 'Z');
}

static Arcadia_BooleanValue
isDecimalDigit
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
  return ('0' <= self->symbol && self->symbol <= '9');
}

Arcadia_DDL_Scanner*
Arcadia_DDL_Scanner_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* stringTable,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_Scanner);
  if (stringTable) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)stringTable); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (diagnostics) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_DDL_Scanner);
}

static Arcadia_UnicodeCodePointReader*
Arcadia_DDL_Scanner_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return self->input; }

static Arcadia_String*
Arcadia_DDL_Scanner_getWordTextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return Arcadia_Languages_StringTable_getOrCreateString(thread, self->stringTable, self->word.text); }

static Arcadia_Integer32Value
Arcadia_DDL_Scanner_getWordTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return self->word.type; }

static Arcadia_Natural32Value
Arcadia_DDL_Scanner_getWordStartImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return self->word.start; }

static Arcadia_Natural32Value
Arcadia_DDL_Scanner_getWordLengthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return self->word.length; }

static void
onEndOfLine
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
  if ('\r' == self->symbol) {
    next(thread, self);
    if ('\n' == self->symbol) {
      next(thread, self);
    }
    onEndWord(thread, self, Arcadia_DDL_WordType_LineTerminator);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<line terminator>");
    return;
  } else if ('\n' == self->symbol) {
    next(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_LineTerminator);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<line terminator>");
    return;
  }
}

static void
onNumberLiteral
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
  if ('+' == self->symbol || '-' == self->symbol) {
    saveAndNext(thread, self);
  }
  Arcadia_Integer32Value type = Arcadia_DDL_WordType_IntegerLiteral;
  if (isDecimalDigit(thread, self)) {
    // We have a non-empty integral part.
    // We do not require a non-empty fractional part.
    do {
      saveAndNext(thread, self);
    } while (isDecimalDigit(thread, self));
    if ('.' == self->symbol) {
      type = Arcadia_DDL_WordType_RealLiteral;
      saveAndNext(thread, self);
      while (isDecimalDigit(thread, self)) {
        saveAndNext(thread, self);
      }
    }
  } else if ('.' == self->symbol) {
    // We have an empty integral part.
    // We require a non-empty fractional part.
    type = Arcadia_DDL_WordType_RealLiteral;
    saveAndNext(thread, self);
    if (!isDecimalDigit(thread, self)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
      Arcadia_Thread_jump(thread);
    }
    do {
      saveAndNext(thread, self);
    } while (isDecimalDigit(thread, self));
  }
  if ('e' == self->symbol || 'E' == self->symbol) {
    type = Arcadia_DDL_WordType_RealLiteral;
    saveAndNext(thread, self);
    if ('+' == self->symbol || '-' == self->symbol) {
      saveAndNext(thread, self);
    }
    if (!isDecimalDigit(thread, self)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
      Arcadia_Thread_jump(thread);
    }
    do {
      saveAndNext(thread, self);
    } while (isDecimalDigit(thread, self));
  }
  onEndWord(thread, self, type);
}

static Arcadia_Natural32Value
decimalDigitValue
  (
    Arcadia_Natural32Value digit
  )
{
  return digit - '0';
}

static Arcadia_Natural32Value
hexadecimalDigitValue
  (
    Arcadia_Natural32Value digit
  )
{
  if ('a' <= digit && digit <= 'f') return digit - 'a' + 10;
  if ('A' <= digit && digit <= 'F') return digit - 'A' + 10;
  else return digit - '0';
}

static void
onStringLiteral
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{
    // <string>
  next(thread, self);
  Arcadia_BooleanValue lastWasSlash = Arcadia_BooleanValue_False;
  while (Arcadia_BooleanValue_True) {
    if (self->symbol <= 0x1f) {
      // Must have had been escaped.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
      Arcadia_Thread_jump(thread);
    }
    if (lastWasSlash) {
      // \uxxxx, \b, \f, \n, \r, \t, \0, \v
      switch (self->symbol) {
        case 'u': {
          Arcadia_Natural32Value codePoint = 0;
          next(thread, self);
          for (Arcadia_SizeValue i = 0; i < 4; ++i) {
            if (!isHexadecimalDigit(thread, self)) {
              Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
              Arcadia_Thread_jump(thread);
            }
            codePoint *= 16;
            Arcadia_Natural32Value digitValue = hexadecimalDigitValue(self->symbol);
            codePoint += digitValue;
            next(thread, self);
          }
          write(thread, self, codePoint);
        } break;
        case '"': {
          write(thread, self, '"');
          next(thread, self);
        } break;
        case '\\': {
          write(thread, self, '\\');
          next(thread, self);
        } break;
        case 'b': {
          write(thread, self, '\b');
          next(thread, self);
        } break;
        case 'f': {
          write(thread, self, '\f');
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
        case 't': {
          write(thread, self, '\t');
          next(thread, self);
        } break;
        case '0': {
          write(thread, self, '\0');
          next(thread, self);
        } break;
        case 'v': {
          write(thread, self, '\v');
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
  onEndWord(thread, self, Arcadia_DDL_WordType_StringLiteral);
}

static void
Arcadia_DDL_Scanner_stepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
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
    onEndWord(thread, self, Arcadia_DDL_WordType_EndOfInput);
    Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<end of input>");
    return;
  }
  // Whitespace :  <Whitespace> | <Tabulator>
  if (' ' == self->symbol || '\t' == self->symbol) {
    do {
      saveAndNext(thread, self);
    } while (' ' == self->symbol || '\t' == self->symbol);
    onEndWord(thread, self, Arcadia_DDL_WordType_WhiteSpace);
    return;
  }
  // LineTerminator : <LineFeed>
  //                  <CarriageReturn>
  //                  <CarriageReturn> <LineFeed>
  if ('\r' == self->symbol) {
    saveAndNext(thread, self);
    if ('\n' == self->symbol) {
      saveAndNext(thread, self);
    }
    onEndWord(thread, self, Arcadia_DDL_WordType_LineTerminator);
    return;
  } else if ('\n' == self->symbol) {
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_LineTerminator);
    return;
  }
  if (':' == self->symbol) {
     // <colon>
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_Colon);
    return;
  } else if (',' == self->symbol) {
     // <comma>
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_Comma);
    return;
  } else if ('{' == self->symbol) {
     // <left curly bracket>
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_LeftCurlyBracket);
    return;
  } else if ('}' == self->symbol) {
     // <right curly bracket>
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_RightCurlyBracket);
    return;
  } else if ('[' == self->symbol) {
     // <left square bracket>
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_LeftSquareBracket);
    return;
  } else if (']' == self->symbol) {
     // <right square bracket>
    saveAndNext(thread, self);
    onEndWord(thread, self, Arcadia_DDL_WordType_RightSquareBracket);
    return;
  } else if ('"' == self->symbol) {
    // <string>
    onStringLiteral(thread, self);
  } else if ('_' == self->symbol || isAlphabetic(thread, self)) {
    // <name>
    saveAndNext(thread, self);
    while ('_' == self->symbol || isAlphabetic(thread, self) || isDecimalDigit(thread, self)) {
      saveAndNext(thread, self);
    }
    onEndWord(thread, self, Arcadia_DDL_WordType_Name);
    Arcadia_DataDefinitionLanguage_Keywords_scan(thread, self->keywords, Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self), &self->word.type);
    return;
  } else if (isDecimalDigit(thread, self) || '+' == self->symbol || '-' == self->symbol) {
    // <number>
    onNumberLiteral(thread, self);
  } else if ('/' == self->symbol) {
    next(thread, self);
    if ('*' == self->symbol) {
      // multi line comment
      next(thread, self);
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
          next(thread, self);
          if ('/' == self->symbol) {
            next(thread, self);
            break;
          } else {
            write(thread, self, '*');
          }
        } else {
          saveAndNext(thread, self);
        }
      }
      onEndWord(thread, self, Arcadia_DDL_WordType_MultiLineComment);
    } else if ('/' == self->symbol) {
      // single line comment
      next(thread, self);
      while (CodePoint_End != self->symbol && '\n' != self->symbol && '\r' != self->symbol) {
        saveAndNext(thread, self);
      }
      onEndWord(thread, self, Arcadia_DDL_WordType_SingleLineComment);
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
Arcadia_DDL_Scanner_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self,
    Arcadia_UnicodeCodePointReader* input
  )
{
  self->input = input;
  self->symbol = CodePoint_Start;
  self->word.type = Arcadia_DDL_WordType_StartOfInput;
  self->word.start = 0;
  self->word.length = 0;
  Arcadia_StringBuilder_clear(thread, self->word.text);
  Arcadia_StringBuilder_insertBackCxxString(thread, self->word.text, u8"<start of input>");
}

static Arcadia_Languages_StringTable*
Arcadia_DDL_Scanner_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return self->stringTable; }

static Arcadia_Languages_Diagnostics*
Arcadia_DDL_Scanner_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* self
  )
{ return self->diagnostics; }
