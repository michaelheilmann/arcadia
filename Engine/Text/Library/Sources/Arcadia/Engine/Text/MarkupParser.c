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

#define ARCADIA_ENGINE_TEXT_PRIVATE (1)
#include "Arcadia/Engine/Text/MarkupParser.h"

#include "Arcadia/Collections/Include.h"
#include <string.h>

struct Arcadia_Engine_Text_MarkupParserDispatch { Arcadia_Languages_ParserDispatch parent; };

struct Arcadia_Engine_Text_MarkupParser {
  Arcadia_Languages_Parser parent;
  Arcadia_Engine_Text_MarkupScanner* scanner;
  Arcadia_Languages_StringTable* stringTable;
  Arcadia_Languages_Diagnostics* diagnostics;
};

static void constructImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self);
static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParserDispatch* self);
static void visitImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self);
static Arcadia_Languages_Diagnostics* getDiagnosticsImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self);
static Arcadia_UnicodeCodePointReader* getInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self);
static Arcadia_Languages_StringTable* getStringTableImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self);
static Arcadia_Value runImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self);
static void setInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self, Arcadia_UnicodeCodePointReader* input);

static const Arcadia_ObjectType_Operations objectTypeOperations = { Arcadia_ObjectType_Operations_Initializer, .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl, .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl, .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl };
static const Arcadia_Type_Operations typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectTypeOperations };
Arcadia_defineObjectType(u8"Arcadia.Engine.Text.MarkupParser", Arcadia_Engine_Text_MarkupParser, u8"Arcadia.Languages.Parser", Arcadia_Languages_Parser, &typeOperations);

static void constructImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) {
  Arcadia_EnterConstructor(Arcadia_Engine_Text_MarkupParser);
  { Arcadia_ValueStack_pushNatural8Value(thread, 0); Arcadia_superTypeConstructor(thread, _type, self); }
  if (0 != _numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->scanner = Arcadia_Engine_Text_MarkupScanner_create(thread);
  self->stringTable = Arcadia_Languages_Scanner_getStringTable(thread, (Arcadia_Languages_Scanner*)self->scanner);
  self->diagnostics = Arcadia_Languages_Scanner_getDiagnostics(thread, (Arcadia_Languages_Scanner*)self->scanner);
  Arcadia_LeaveConstructor(Arcadia_Engine_Text_MarkupParser);
}

static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParserDispatch* self) {
  ((Arcadia_Languages_ParserDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics* (*)(Arcadia_Thread*, Arcadia_Languages_Parser*))&getDiagnosticsImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader* (*)(Arcadia_Thread*, Arcadia_Languages_Parser*))&getInputImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable* (*)(Arcadia_Thread*, Arcadia_Languages_Parser*))&getStringTableImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->run = (Arcadia_Value (*)(Arcadia_Thread*, Arcadia_Languages_Parser*))&runImpl;
  ((Arcadia_Languages_ParserDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Parser*, Arcadia_UnicodeCodePointReader*))&setInputImpl;
}

static void visitImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { if (self->scanner) Arcadia_Object_visit(thread, (Arcadia_Object*)self->scanner); if (self->stringTable) Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable); if (self->diagnostics) Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics); }
static Arcadia_Integer32Value type(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { return Arcadia_Languages_Scanner_getWordType(thread, (Arcadia_Languages_Scanner*)self->scanner); }
static Arcadia_String* text(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { return Arcadia_Languages_Scanner_getWordText(thread, (Arcadia_Languages_Scanner*)self->scanner); }
static void next(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { Arcadia_Languages_Scanner_step(thread, (Arcadia_Languages_Scanner*)self->scanner); }
static Arcadia_BooleanValue equal(Arcadia_Thread* thread, Arcadia_String* s, char const* bytes) { return Arcadia_String_isEqualTo_pn(thread, s, bytes, strlen(bytes)); }
static void fail(Arcadia_Thread* thread) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); Arcadia_Thread_jump(thread); }
static Arcadia_Natural32Value parseNatural32(Arcadia_Thread* thread, Arcadia_String* s) { return (Arcadia_Natural32Value)Arcadia_String_toInteger64(thread, s); }
static Arcadia_Integer32Value parseInteger32(Arcadia_Thread* thread, Arcadia_String* s) { return (Arcadia_Integer32Value)Arcadia_String_toInteger64(thread, s); }
static void setRectangle(Arcadia_Engine_Text_MarkupTextRun* run, Arcadia_BooleanValue hasRectangle, Arcadia_Integer32Value rectangleLeft, Arcadia_Integer32Value rectangleBottom, Arcadia_Integer32Value rectangleWidth, Arcadia_Integer32Value rectangleHeight) { run->hasRectangle = hasRectangle; run->rectangleLeft = rectangleLeft; run->rectangleBottom = rectangleBottom; run->rectangleWidth = rectangleWidth; run->rectangleHeight = rectangleHeight; }

static Arcadia_Value runImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) {
  Arcadia_List* runs = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_String* color = Arcadia_String_createFromCxxString(thread, "Colors.White");
  Arcadia_Natural32Value pixelSize = 16;
  Arcadia_Engine_Text_TextAlignment alignment = Arcadia_Engine_Text_TextAlignment_Center;
  Arcadia_BooleanValue hasRectangle = Arcadia_BooleanValue_False;
  Arcadia_Integer32Value rectangleLeft = 0, rectangleBottom = 0, rectangleWidth = 0, rectangleHeight = 0;
  Arcadia_BooleanValue fixed = Arcadia_BooleanValue_False;
  next(thread, self);
  while (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_EndOfInput) {
    if (type(thread, self) == Arcadia_Engine_Text_MarkupWordType_LeftBracket) {
      next(thread, self);
      while (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_RightBracket) {
        if (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_Name) fail(thread);
        Arcadia_String* key = text(thread, self); next(thread, self);
        if (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_EqualsSign) fail(thread);
        next(thread, self);
        if (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_Name) fail(thread);
        Arcadia_String* value = text(thread, self);
        if (equal(thread, key, "size")) pixelSize = parseNatural32(thread, value);
        else if (equal(thread, key, "color")) color = value;
        else if (equal(thread, key, "align")) { if (equal(thread, value, "left")) alignment = Arcadia_Engine_Text_TextAlignment_Left; else if (equal(thread, value, "center")) alignment = Arcadia_Engine_Text_TextAlignment_Center; else if (equal(thread, value, "right")) alignment = Arcadia_Engine_Text_TextAlignment_Right; else fail(thread); }
        else if (equal(thread, key, "fixed")) fixed = equal(thread, value, "true") ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
        else if (equal(thread, key, "rect")) { if (equal(thread, value, "canvas")) { hasRectangle = Arcadia_BooleanValue_False; rectangleLeft = rectangleBottom = rectangleWidth = rectangleHeight = 0; } else fail(thread); }
        else if (equal(thread, key, "x")) { rectangleLeft = parseInteger32(thread, value); hasRectangle = Arcadia_BooleanValue_True; }
        else if (equal(thread, key, "y")) { rectangleBottom = parseInteger32(thread, value); hasRectangle = Arcadia_BooleanValue_True; }
        else if (equal(thread, key, "width")) { rectangleWidth = parseInteger32(thread, value); hasRectangle = Arcadia_BooleanValue_True; }
        else if (equal(thread, key, "height")) { rectangleHeight = parseInteger32(thread, value); hasRectangle = Arcadia_BooleanValue_True; }
        else fail(thread);
        next(thread, self);
      }
      next(thread, self);
    } else if (type(thread, self) == Arcadia_Engine_Text_MarkupWordType_LineBreak) {
      next(thread, self);
    } else if (type(thread, self) == Arcadia_Engine_Text_MarkupWordType_Text) {
      Arcadia_String* runText = text(thread, self);
      Arcadia_Engine_Text_MarkupTextRun* run = Arcadia_Engine_Text_MarkupTextRun_create(thread, runText, runText, NULL, pixelSize, color, alignment, fixed);
      setRectangle(run, hasRectangle, rectangleLeft, rectangleBottom, rectangleWidth, rectangleHeight);
      Arcadia_List_insertBackObjectReferenceValue(thread, runs, (Arcadia_Object*)run);
      next(thread, self);
    } else if (type(thread, self) == Arcadia_Engine_Text_MarkupWordType_LeftBrace) {
      next(thread, self);
      if (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_Name) fail(thread);
      Arcadia_String* token = text(thread, self);
      next(thread, self);
      if (type(thread, self) != Arcadia_Engine_Text_MarkupWordType_RightBrace) fail(thread);
      Arcadia_String* empty = Arcadia_String_createFromCxxString(thread, "");
      Arcadia_Engine_Text_MarkupTextRun* run = Arcadia_Engine_Text_MarkupTextRun_create(thread, empty, token, token, pixelSize, color, alignment, fixed);
      setRectangle(run, hasRectangle, rectangleLeft, rectangleBottom, rectangleWidth, rectangleHeight);
      Arcadia_List_insertBackObjectReferenceValue(thread, runs, (Arcadia_Object*)run);
      next(thread, self);
    } else { fail(thread); }
  }
  return Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)runs);
}

static void setInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self, Arcadia_UnicodeCodePointReader* input) { Arcadia_Languages_Scanner_setInput(thread, (Arcadia_Languages_Scanner*)self->scanner, input); }
static Arcadia_Languages_Diagnostics* getDiagnosticsImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { return self->diagnostics; }
static Arcadia_UnicodeCodePointReader* getInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { return Arcadia_Languages_Scanner_getInput(thread, (Arcadia_Languages_Scanner*)self->scanner); }
static Arcadia_Languages_StringTable* getStringTableImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupParser* self) { return self->stringTable; }

Arcadia_Engine_Text_MarkupParser* Arcadia_Engine_Text_MarkupParser_create(Arcadia_Thread* thread) { _Arcadia_BeginCreate(Arcadia_Engine_Text_MarkupParser); Arcadia_ValueStack_pushNatural8Value(thread, 0); _Arcadia_EndCreate(Arcadia_Engine_Text_MarkupParser); }
