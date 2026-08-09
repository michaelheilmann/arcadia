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

#include "Arcadia.TemplateEngine/Scanner/Scanner.h"

#include "Arcadia/Logging/Include.h"
#include "Arcadia.TemplateEngine/Diagnostics/UnclosedStringLiteralDiagnostic.h"
#include "Arcadia.TemplateEngine/Diagnostics/UnexpectedSymbolDiagnostic.h"
#include "Arcadia.TemplateEngine/Diagnostics/UnexpectedWordDiagnostic.h"

static void
Arcadia_TemplateEngine_Scanner_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
Arcadia_TemplateEngine_Scanner_visit
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
Arcadia_TemplateEngine_Scanner_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
Arcadia_TemplateEngine_Scanner_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_ScannerDispatch* self
  );

static Arcadia_Languages_Diagnostics*
Arcadia_TemplateEngine_Scanner_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_UnicodeCodePointReader*
Arcadia_TemplateEngine_Scanner_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_Languages_StringTable*
Arcadia_TemplateEngine_Scanner_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_String*
Arcadia_TemplateEngine_Scanner_getWordTextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_Integer32Value
Arcadia_TemplateEngine_Scanner_getWordTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
Arcadia_TemplateEngine_Scanner_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self,
    Arcadia_UnicodeCodePointReader* reader
  );

static void
Arcadia_TemplateEngine_Scanner_stepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
Arcadia_TemplateEngine_Scanner_textStepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
Arcadia_TemplateEngine_Scanner_directivesStepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_BooleanValue
is
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self,
    Arcadia_Natural32Value expectedCodePoint
  );

static Arcadia_BooleanValue
isSign
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_BooleanValue
isLetter
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_BooleanValue
isDigit
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static Arcadia_BooleanValue
isUnderscore
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
scanName
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static void
scanString
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_TemplateEngine_Scanner_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_TemplateEngine_Scanner_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_TemplateEngine_Scanner_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_TemplateEngine_Scanner_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.Scanner", Arcadia_TemplateEngine_Scanner,
                         u8"Arcadia.Languages.Scanner", Arcadia_Languages_Scanner,
                         &_typeOperations);

static void
Arcadia_TemplateEngine_Scanner_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_TemplateEngine_Scanner_visit
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }
  if (self->stringTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable);
  }
  if (self->file) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->file);
  }
  if (self->reader) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->reader);
  }
  if (self->temporary) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->temporary);
  }
  if (self->temporaryBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->temporaryBuffer);
  }
}

static void
Arcadia_TemplateEngine_Scanner_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  Arcadia_EnterConstructor(Arcadia_TemplateEngine_Scanner);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->textMode = Arcadia_BooleanValue_False;

  self->diagnostics = (Arcadia_Languages_Diagnostics*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_Languages_Diagnostics_getType(thread));
  self->stringTable = Arcadia_Languages_StringTable_getOrCreate(thread);

  self->file = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  self->line = Arcadia_ValueStack_getNatural64Value(thread, 2);
  self->reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_UnicodeCodePointReader_getType(thread));

  self->temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  self->temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  self->wordType = Arcadia_TemplateEngine_WordType_StartOfInput;

  Arcadia_LeaveConstructor(Arcadia_TemplateEngine_Scanner);
}

static void
Arcadia_TemplateEngine_Scanner_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_ScannerDispatch* self
  )
{
  ((Arcadia_Languages_ScannerDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics * (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_TemplateEngine_Scanner_getDiagnosticsImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_TemplateEngine_Scanner_getInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable * (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_TemplateEngine_Scanner_getStringTableImpl;
  
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordText = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_TemplateEngine_Scanner_getWordTextImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordType = (Arcadia_Integer32Value (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_TemplateEngine_Scanner_getWordTypeImpl;

  ((Arcadia_Languages_ScannerDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*, Arcadia_UnicodeCodePointReader*)) & Arcadia_TemplateEngine_Scanner_setInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->step = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*)) & Arcadia_TemplateEngine_Scanner_stepImpl;
}

static Arcadia_Languages_Diagnostics*
Arcadia_TemplateEngine_Scanner_getDiagnosticsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ return self->diagnostics; }

static Arcadia_UnicodeCodePointReader*
Arcadia_TemplateEngine_Scanner_getInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ return self->reader; }

static Arcadia_Languages_StringTable*
Arcadia_TemplateEngine_Scanner_getStringTableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ return self->stringTable; }

static Arcadia_String*
Arcadia_TemplateEngine_Scanner_getWordTextImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ return Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, self->temporaryBuffer->p, self->temporaryBuffer->sz)); }

static Arcadia_Integer32Value
Arcadia_TemplateEngine_Scanner_getWordTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ return self->wordType; }

static void
Arcadia_TemplateEngine_Scanner_setInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  self->reader = reader;
  self->wordType = Arcadia_TemplateEngine_WordType_StartOfInput;
}

static void
Arcadia_TemplateEngine_Scanner_textStepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ 
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
  if ('@' != codePoint) {
    do {
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->temporary, &codePoint, 1, self->temporaryBuffer);
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      if (!Arcadia_UnicodeCodePointReader_hasValue(thread, self->reader)) {
        break; // Deliver the text until the end of the input.
      }
      if (Arcadia_UnicodeCodePointReader_hasError(thread, self->reader)) {
        break; // Deliver the text until the error of the input.
      }
      codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
    } while ('@' != codePoint);
    self->wordType = Arcadia_TemplateEngine_WordType_Text;
    return;
  } else {
    self->wordType = Arcadia_TemplateEngine_WordType_StartOfDirective;
    Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
    return;
  }
}

static void
Arcadia_TemplateEngine_Scanner_directivesStepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{ 
  Arcadia_Natural32Value current = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
  switch (current) {
    case '@': {
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      self->wordType = Arcadia_TemplateEngine_WordType_AtLiteral;
    } break;
    case '(': {
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      self->wordType = Arcadia_TemplateEngine_WordType_LeftParenthesis;
    } break;
    case ')': {
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      self->wordType = Arcadia_TemplateEngine_WordType_RightParenthesis;
    } break;
    case '{': {
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      self->wordType = Arcadia_TemplateEngine_WordType_LeftCurlyBracket;
    } break;
    case '}': {
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      self->wordType = Arcadia_TemplateEngine_WordType_RightCurlyBracket;
    } break;
    case '"': {
      scanString(thread, self);
    } break;
    case ',': {
      Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      self->wordType = Arcadia_TemplateEngine_WordType_Comma;
    } break;
    default: {
      scanName(thread, self);
    } break;
  }
}

static void
Arcadia_TemplateEngine_Scanner_stepImpl
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  Arcadia_ByteArrayBuilder_clear(thread, self->temporaryBuffer);
  if (self->wordType == Arcadia_TemplateEngine_WordType_Error || self->wordType == Arcadia_TemplateEngine_WordType_EndOfInput) {
    return;
  }
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, self->reader)) {
    self->wordType = Arcadia_TemplateEngine_WordType_EndOfInput;
    return;
  }
  if (Arcadia_UnicodeCodePointReader_hasError(thread, self->reader)) {
    self->wordType = Arcadia_TemplateEngine_WordType_Error;
    return;
  }
  if (!self->textMode) {
    Arcadia_TemplateEngine_Scanner_directivesStepImpl(thread, self);
  } else {
    Arcadia_TemplateEngine_Scanner_textStepImpl(thread, self);
  }
}

static Arcadia_BooleanValue
is
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self,
    Arcadia_Natural32Value expectedCodePoint
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, self->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value currentCodePoint = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
  return expectedCodePoint == currentCodePoint;
}

static Arcadia_BooleanValue
isSign
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  return is(thread, self, '+') || is(thread, self, '-');
}

static Arcadia_BooleanValue
isLetter
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, self->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value current = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
  return ('A' <= current && current <= 'Z')
      || ('a' <= current && current <= 'z');
}

static Arcadia_BooleanValue
isDigit
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, self->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value current = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
  return ('0' <= current && current <= '9');
}

static Arcadia_BooleanValue
isUnderscore
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  return is(thread, self, '_');
}

static void
scanName
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  Arcadia_ByteArrayBuilder_clear(thread, self->temporaryBuffer);
  if (!isUnderscore(thread, self) && !isLetter(thread, self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  do {
    Arcadia_Natural32Value targetCodePoint = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->temporary, &targetCodePoint, 1, self->temporaryBuffer);
    Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
  } while (isUnderscore(thread, self) || isLetter(thread, self) || isDigit(thread, self));
  self->wordType = Arcadia_TemplateEngine_WordType_Name;
}

static void
scanString
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self
  )
{
  Arcadia_ByteArrayBuilder_clear(thread, self->temporaryBuffer);
  if (!is(thread, self, '"')) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);

  Arcadia_BooleanValue lastWasSlash = Arcadia_BooleanValue_False;
  while (true) {
    if (!Arcadia_UnicodeCodePointReader_hasValue(thread, self->reader)) {
      Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticContext_EndOfInput);
      Arcadia_Languages_Diagnostics_add(thread, Arcadia_Languages_Parser_getDiagnostics(thread, (Arcadia_Languages_Parser*)self), diagnostic);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Natural32Value current = Arcadia_UnicodeCodePointReader_getValue(thread, self->reader);
    if (lastWasSlash) {
      switch (current) {
        case '\\': {
          Arcadia_Natural32Value targetCodePoint = '\\';
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->temporary, &targetCodePoint, 1, self->temporaryBuffer);
          lastWasSlash = Arcadia_BooleanValue_False;
        } break;
        case '"': {
          Arcadia_Natural32Value targetCodePoint = '"';
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->temporary, &targetCodePoint, 1, self->temporaryBuffer);
          lastWasSlash = Arcadia_BooleanValue_False;
        } break;
        default: {
          Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, current);
          Arcadia_Languages_Diagnostics_add(thread, Arcadia_Languages_Parser_getDiagnostics(thread, (Arcadia_Languages_Parser*)self), diagnostic);
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        } break;
      };
    } else {
      if (current == '"') {
        Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
        self->wordType = Arcadia_TemplateEngine_WordType_StringLiteral;
        return;
      } else if (current == '\\') {
        lastWasSlash = Arcadia_BooleanValue_True;
        Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      } else {
        Arcadia_Unicode_Encoder_encodeCodePoints(thread, self->temporary, &current, 1, self->temporaryBuffer);
        Arcadia_UnicodeCodePointReader_nextValue(thread, self->reader);
      }
    }
  }
}

Arcadia_TemplateEngine_Scanner*
Arcadia_TemplateEngine_Scanner_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_String* file,
	  Arcadia_Natural64Value line,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  _Arcadia_BeginCreate(Arcadia_TemplateEngine_Scanner);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)diagnostics);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)file);
  Arcadia_ValueStack_pushNatural64Value(thread, line);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_ObjectReferenceValue)reader);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_TemplateEngine_Scanner);
}

void
Arcadia_TemplateEngine_Scanner_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self,
    Arcadia_String* file,
    Arcadia_Natural64Value line
  )
{ 
  self->file = file;
  self->line = line;
}
