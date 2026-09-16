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
#include "Arcadia/MILC/TypeResolutionPhase/EnumerationCompleter.h"

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Diagnostics/Include.h"
#include "Arcadia/MILC/Symbols/Include.h"
#include "Arcadia/MILC/Environment.h"
#include <assert.h>

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self
  );

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self
  );

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleterDispatch* self
  );

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self
  );

static void
onCompleteEnumeration
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_EnumerationSymbol* symbol
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.TypeResolutionPhase.EnumerationCompleter", Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter,
                         u8"Arcadia.MILC.Completer", Arcadia_MILC_Completer,
                         &_typeOperations);

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter);
}

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleterDispatch* self
  )
{
  ((Arcadia_MILC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_MILC_Completer*, Arcadia_MILC_Context*, Arcadia_MILC_Symbol*)) & completeImpl;
}

static void
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self
  )
{/*Intentionally empty.*/}

static void
onCompleteEnumerationConstant
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_EnumerationConstantSymbol* symbol
  )
{ 
  Arcadia_MILC_AST_EnumerationConstantDefinitionNode* node = symbol->ast;
  if (!node) {
    return;
  }
  Arcadia_MILC_AST_IntegerLiteralNode* initializer = node->initializer;
  if (!initializer) {
    // A diagnostic was already added by the member enter phase.
    return;
  }
  Arcadia_MILC_Symbols* symbols = Arcadia_MILC_Symbols_getInstance(thread, context);
  // Determine the value of the integer literal.
  Arcadia_BooleanValue overflow = Arcadia_BooleanValue_False;
  Arcadia_Natural64Value value = 0;
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, initializer->value));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    if (codePoint < (Arcadia_Natural32Value)'0' || codePoint >(Arcadia_Natural32Value)'9') {
      overflow = Arcadia_BooleanValue_True;
      break;
    }
    Arcadia_Natural64Value digit = (Arcadia_Natural64Value)(codePoint - (Arcadia_Natural32Value)'0');
    if (value > (Arcadia_Natural64Value_Maximum - digit) / 10) {
      overflow = Arcadia_BooleanValue_True;
      break;
    }
    value = value * 10 + digit;
  }
  if (overflow) {
    Arcadia_MILC_Environment* e = Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(symbol)),
                                                                                       _Arcadia_MILC_EnumerationSymbol_getType(thread));
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            symbol,
            initializer->value
          )
      );
    return;
  }
  // LANGUAGE DEFINITION: The type of an integer literal is the first of
  // Arcadia.Natural8, Arcadia.Natural16, Arcadia.Natural32, and Arcadia.Natural64
  // which is able to represent its value.
  if (value <= (Arcadia_Natural64Value)Arcadia_Natural8Value_Maximum) {
    initializer->type = symbols->natural8Symbol;
  } else if (value <= (Arcadia_Natural64Value)Arcadia_Natural16Value_Maximum) {
    initializer->type = symbols->natural16Symbol;
  } else if (value <= (Arcadia_Natural64Value)Arcadia_Natural32Value_Maximum) {
    initializer->type = symbols->natural32Symbol;
  } else {
    initializer->type = symbols->natural64Symbol;
  }
}

static void
onCompleteEnumeration
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_EnumerationSymbol* symbol
  )
{
  Arcadia_MILC_EnumerationSymbol* enumerationSymbol = (Arcadia_MILC_EnumerationSymbol*)symbol;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)enumerationSymbol->members); i < n; ++i) {
    Arcadia_MILC_EnumerationConstantSymbol* enumerationConstantSymbol =
      (Arcadia_MILC_EnumerationConstantSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, enumerationSymbol->members, i, _Arcadia_MILC_EnumerationConstantSymbol_getType(thread));
    onCompleteEnumerationConstant(thread, self, context, enumerationConstantSymbol);
  }
}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{ 
  assert(symbol->completer == (Arcadia_MILC_Completer*)self);
  onCompleteEnumeration(thread, self, context, (Arcadia_MILC_EnumerationSymbol*)symbol);
  symbol->completer = NULL;
}

Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter*
Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_create
  (
    Arcadia_Thread* thread
  )
{ 
  _Arcadia_BeginCreate(Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter);
}
