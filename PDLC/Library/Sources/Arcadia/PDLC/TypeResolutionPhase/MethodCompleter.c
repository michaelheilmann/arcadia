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

#define ARCADIA_PDLC_PRIVATE (1)
#include "Arcadia/PDLC/TypeResolutionPhase/MethodCompleter.h"

#include "Arcadia/PDLC/Context.h"
#include "Arcadia/PDLC/Diagnostics/Include.h"
#include "Arcadia/PDLC/Symbols/Include.h"
#include "Arcadia/PDLC/Names.h"
#include "Arcadia/PDLC/Environment.h"
#include "Arcadia/PDLC/TypeResolutionPhase.h"
#include <assert.h>

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self
  );

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self
  );

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleterDispatch* self
  );

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self
  );

static void
onCompleteParameter
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_VariableSymbol* variableSymbol
  );

static void
onCompleteMethod
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_MethodSymbol* symbol
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.TypeResolutionPhase.MethodCompleter", Arcadia_PDLC_TypeResolutionPhase_MethodCompleter,
                         u8"Arcadia.PDLC.Completer", Arcadia_PDLC_Completer,
                         &_typeOperations);

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_TypeResolutionPhase_MethodCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_TypeResolutionPhase_MethodCompleter);
}

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleterDispatch* self
  )
{
  ((Arcadia_PDLC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Completer*, Arcadia_PDLC_Context*, Arcadia_PDLC_Symbol*)) & completeImpl;
}

static void
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self
  )
{/*Intentionally empty.*/}

static void
onCompleteParameter
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_VariableSymbol* variableSymbol
  )
{
  Arcadia_PDLC_TypeResolutionPhase_resolveParameter(thread, Arcadia_PDLC_TypeResolutionPhase_getInstance(thread, context), variableSymbol);
}

static void
onCompleteMethod
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_MethodSymbol* symbol
  )
{
  // Complete parameter types.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbol->parameters); i < n; ++i) {
    Arcadia_PDLC_VariableSymbol* parameter = (Arcadia_PDLC_VariableSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbol->parameters, i, _Arcadia_PDLC_VariableSymbol_getType(thread));
    onCompleteParameter(thread, self, context, parameter);
  }
// Complete return value type.
  Arcadia_PDLC_TypeResolutionPhase_resolveReturnValue(thread, Arcadia_PDLC_TypeResolutionPhase_getInstance(thread, context), (Arcadia_PDLC_Symbol*)symbol);

  // Search for the method this method overrides if any.
  // We currently search 'by name'.
  // @note The enclosing symbol is not necessarily a class symbol: an operation of an interface type is
  // enclosed by an interface symbol, which has no parent class symbol, and the search is skipped then.
  Arcadia_String* n = ((Arcadia_PDLC_Symbol*)symbol)->name;
  Arcadia_PDLC_Symbol* enclosing = ((Arcadia_PDLC_Symbol*)symbol)->enclosing;
  if (!enclosing || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)enclosing, _Arcadia_PDLC_ClassSymbol_getType(thread))) {
    return;
  }
  Arcadia_PDLC_ClassSymbol* p = (Arcadia_PDLC_ClassSymbol*)enclosing;
  do {
    p = p->parentClassSymbol;
    if (!p) {
      break;
    }
    // The built-in class type `Arcadia.Object` has no scope: no method can be overridden there.
    if (!p->scope) {
      break;
    }
    Arcadia_PDLC_Symbol* overrideOf = (Arcadia_PDLC_Symbol*)Arcadia_Languages_Scope_lookup(thread, p->scope, n, Arcadia_BooleanValue_False);
    if (overrideOf) {
      if (overrideOf->kind != Arcadia_PDLC_SymbolKind_Method) {
        Arcadia_PDLC_Environment* e = Arcadia_PDLC_Symbols_getEnvironment(thread, Arcadia_PDLC_Symbols_getInstance(thread, context), (Arcadia_PDLC_Symbol*)symbol);
        Arcadia_Languages_Diagnostics_add
          (
            thread,
            context->diagnostics,
            (Arcadia_Languages_Diagnostic*)
            Arcadia_PDLC_Diagnostics_SymbolIsNoMethodDiagnostic_create
              (
                thread,
                Arcadia_Languages_DiagnosticType_Error,
                Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
                Arcadia_SizeValue_Literal(0),
                overrideOf->name
              )
          );
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      symbol->overrideOf = (Arcadia_PDLC_MethodSymbol*)overrideOf;
      break;
    }
  } while (NULL != p);
}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_MethodCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{
  assert(symbol->completer == (Arcadia_PDLC_Completer*)self);
  onCompleteMethod(thread, self, context, (Arcadia_PDLC_MethodSymbol*)symbol);
  symbol->completer = NULL;
}

Arcadia_PDLC_TypeResolutionPhase_MethodCompleter*
Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_TypeResolutionPhase_MethodCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_TypeResolutionPhase_MethodCompleter);
}
