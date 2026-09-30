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
#include "Arcadia/Engine/Text/MarkupScanner.h"

#include "Arcadia/Collections/Include.h"
#include "Arcadia/Logging/Include.h"

struct Arcadia_Engine_Text_MarkupScannerDispatch { Arcadia_Languages_ScannerDispatch parent; };

struct Arcadia_Engine_Text_MarkupScanner {
  Arcadia_Languages_Scanner parent;
  Arcadia_UnicodeCodePointReader* input;
  Arcadia_Languages_StringTable* stringTable;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_StringBuilder* text;
  Arcadia_Integer32Value wordType;
  Arcadia_Natural32Value wordStart;
  Arcadia_Natural32Value wordLength;
  Arcadia_Natural32Value offset;
  Arcadia_BooleanValue inTag;
};

static void constructImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScannerDispatch* self);
static void visitImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static Arcadia_String* getWordTextImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static Arcadia_Integer32Value getWordTypeImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static Arcadia_Natural32Value getWordStartImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static Arcadia_Natural32Value getWordLengthImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static void stepImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static void setInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self, Arcadia_UnicodeCodePointReader* input);
static Arcadia_UnicodeCodePointReader* getInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static Arcadia_Languages_StringTable* getStringTableImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);
static Arcadia_Languages_Diagnostics* getDiagnosticsImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self);

static const Arcadia_ObjectType_Operations objectTypeOperations = { Arcadia_ObjectType_Operations_Initializer, .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl, .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl, .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl };
static const Arcadia_Type_Operations typeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectTypeOperations };
Arcadia_defineObjectType(u8"Arcadia.Engine.Text.MarkupScanner", Arcadia_Engine_Text_MarkupScanner, u8"Arcadia.Languages.Scanner", Arcadia_Languages_Scanner, &typeOperations);

static void constructImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) {
  Arcadia_EnterConstructor(Arcadia_Engine_Text_MarkupScanner);
  { Arcadia_ValueStack_pushNatural8Value(thread, 0); Arcadia_superTypeConstructor(thread, _type, self); }
  if (0 != _numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->input = NULL;
  self->stringTable = Arcadia_Languages_StringTable_getOrCreate(thread);
  self->diagnostics = Arcadia_Languages_Diagnostics_create(thread, (Arcadia_Log*)Arcadia_ConsoleLog_create(thread));
  self->text = Arcadia_StringBuilder_create(thread);
  self->wordType = Arcadia_Engine_Text_MarkupWordType_StartOfInput;
  self->wordStart = 0;
  self->wordLength = 0;
  self->offset = 0;
  self->inTag = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(Arcadia_Engine_Text_MarkupScanner);
}

static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScannerDispatch* self) {
  ((Arcadia_Languages_ScannerDispatch*)self)->getDiagnostics = (Arcadia_Languages_Diagnostics* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getDiagnosticsImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getInput = (Arcadia_UnicodeCodePointReader* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getStringTable = (Arcadia_Languages_StringTable* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getStringTableImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordLength = (Arcadia_Natural32Value (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getWordLengthImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordStart = (Arcadia_Natural32Value (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getWordStartImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordText = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getWordTextImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->getWordType = (Arcadia_Integer32Value (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&getWordTypeImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->setInput = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*, Arcadia_UnicodeCodePointReader*))&setInputImpl;
  ((Arcadia_Languages_ScannerDispatch*)self)->step = (void (*)(Arcadia_Thread*, Arcadia_Languages_Scanner*))&stepImpl;
}

static void visitImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { if (self->input) Arcadia_Object_visit(thread, (Arcadia_Object*)self->input); if (self->stringTable) Arcadia_Object_visit(thread, (Arcadia_Object*)self->stringTable); if (self->diagnostics) Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics); if (self->text) Arcadia_Object_visit(thread, (Arcadia_Object*)self->text); }
static Arcadia_BooleanValue has(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->input && Arcadia_UnicodeCodePointReader_hasValue(thread, self->input); }
static Arcadia_Natural32Value cp(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return Arcadia_UnicodeCodePointReader_getValue(thread, self->input); }
static void advance(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { if (has(thread, self)) { self->offset += 1; Arcadia_UnicodeCodePointReader_nextValue(thread, self->input); } }
static void append(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self, Arcadia_Natural32Value codePoint) { Arcadia_StringBuilder_insertBackCodePoint(thread, self->text, codePoint); }
static Arcadia_BooleanValue nameChar(Arcadia_Natural32Value c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-'; }

static void stepImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) {
  Arcadia_StringBuilder_clear(thread, self->text); self->wordStart = self->offset; self->wordLength = 0;
  if (!has(thread, self)) { self->wordType = Arcadia_Engine_Text_MarkupWordType_EndOfInput; return; }
  Arcadia_Natural32Value c = cp(thread, self);
  if ('[' == c) { append(thread, self, c); advance(thread, self); self->inTag = Arcadia_BooleanValue_True; self->wordType = Arcadia_Engine_Text_MarkupWordType_LeftBracket; self->wordLength = 1; return; }
  if (']' == c) { append(thread, self, c); advance(thread, self); self->inTag = Arcadia_BooleanValue_False; self->wordType = Arcadia_Engine_Text_MarkupWordType_RightBracket; self->wordLength = 1; return; }
  if ('{' == c) { append(thread, self, c); advance(thread, self); self->inTag = Arcadia_BooleanValue_True; self->wordType = Arcadia_Engine_Text_MarkupWordType_LeftBrace; self->wordLength = 1; return; }
  if ('}' == c) { append(thread, self, c); advance(thread, self); self->inTag = Arcadia_BooleanValue_False; self->wordType = Arcadia_Engine_Text_MarkupWordType_RightBrace; self->wordLength = 1; return; }
  if ('=' == c) { append(thread, self, c); advance(thread, self); self->wordType = Arcadia_Engine_Text_MarkupWordType_EqualsSign; self->wordLength = 1; return; }
  if ('/' == c) { append(thread, self, c); advance(thread, self); self->wordType = Arcadia_Engine_Text_MarkupWordType_Slash; self->wordLength = 1; return; }
  if (self->inTag) {
    while (has(thread, self) && (cp(thread, self) == ' ' || cp(thread, self) == '\t' || cp(thread, self) == '\r' || cp(thread, self) == '\n')) { advance(thread, self); self->wordStart = self->offset; }
    while (has(thread, self) && nameChar(cp(thread, self))) { append(thread, self, cp(thread, self)); advance(thread, self); ++self->wordLength; }
    if (0 == self->wordLength) { append(thread, self, cp(thread, self)); advance(thread, self); self->wordLength = 1; }
    self->wordType = Arcadia_Engine_Text_MarkupWordType_Name; return;
  }
  if ('\r' == c || '\n' == c) { append(thread, self, c); advance(thread, self); self->wordLength = 1; if ('\r' == c && has(thread, self) && '\n' == cp(thread, self)) { append(thread, self, cp(thread, self)); advance(thread, self); self->wordLength = 2; } self->wordType = Arcadia_Engine_Text_MarkupWordType_LineBreak; return; }
  while (has(thread, self) && cp(thread, self) != '[' && cp(thread, self) != '{' && cp(thread, self) != '}' && cp(thread, self) != '\r' && cp(thread, self) != '\n') { append(thread, self, cp(thread, self)); advance(thread, self); ++self->wordLength; }
  self->wordType = Arcadia_Engine_Text_MarkupWordType_Text;
}

static void setInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self, Arcadia_UnicodeCodePointReader* input) { self->input = input; self->offset = 0; self->inTag = Arcadia_BooleanValue_False; self->wordType = Arcadia_Engine_Text_MarkupWordType_StartOfInput; }
static Arcadia_String* getWordTextImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)self->text)); }
static Arcadia_Integer32Value getWordTypeImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->wordType; }
static Arcadia_Natural32Value getWordStartImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->wordStart; }
static Arcadia_Natural32Value getWordLengthImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->wordLength; }
static Arcadia_UnicodeCodePointReader* getInputImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->input; }
static Arcadia_Languages_StringTable* getStringTableImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->stringTable; }
static Arcadia_Languages_Diagnostics* getDiagnosticsImpl(Arcadia_Thread* thread, Arcadia_Engine_Text_MarkupScanner* self) { return self->diagnostics; }

Arcadia_Engine_Text_MarkupScanner* Arcadia_Engine_Text_MarkupScanner_create(Arcadia_Thread* thread) { _Arcadia_BeginCreate(Arcadia_Engine_Text_MarkupScanner); Arcadia_ValueStack_pushNatural8Value(thread, 0); _Arcadia_EndCreate(Arcadia_Engine_Text_MarkupScanner); }
