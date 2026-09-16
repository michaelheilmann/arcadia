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
#include "Arcadia/MILC/TypeResolutionPhase.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/Diagnostics/Include.h"
#include "Arcadia/MILC/Symbols/Include.h"
#include "Arcadia/MILC/Completer.h"

#include "Arcadia/MILC/TypeResolutionPhase/ClassCompleter.h"
#include "Arcadia/MILC/TypeResolutionPhase/ConstructorCompleter.h"
#include "Arcadia/MILC/TypeResolutionPhase/EnumerationCompleter.h"
#include "Arcadia/MILC/TypeResolutionPhase/MethodCompleter.h"
#include "Arcadia/MILC/TypeResolutionPhase/ProcedureCompleter.h"

#include "Arcadia/MILC/MemberEnterPhase.h"
#include "Arcadia/MILC/MemberEnterPhase/ClassCompleter.h"
#include "Arcadia/MILC/MemberEnterPhase/EnumerationCompleter.h"
#include "Arcadia/MILC/MemberEnterPhase/ProcedureCompleter.h"

#include "Arcadia/MILC/Environment.h"

#include "Arcadia/MILC/Names.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhaseDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  );

static void
onVisitVariableSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self,
    Arcadia_MILC_VariableSymbol* symbol
  );

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self,
    Arcadia_MILC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.TypeResolutionPhase", Arcadia_MILC_TypeResolutionPhase,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_TypeResolutionPhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Context_getType(thread));
  self->classCompleter = (Arcadia_MILC_Completer*)Arcadia_MILC_TypeResolutionPhase_ClassCompleter_create(thread);
  self->constructorCompleter = (Arcadia_MILC_Completer*)Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_create(thread);
  self->enumerationCompleter = (Arcadia_MILC_Completer*)Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_create(thread);
  self->methodCompleter = (Arcadia_MILC_Completer*)Arcadia_MILC_TypeResolutionPhase_MethodCompleter_create(thread);
  self->procedureCompleter = (Arcadia_MILC_Completer*)Arcadia_MILC_TypeResolutionPhase_ProcedureCompleter_create(thread);
  Arcadia_LeaveConstructor(Arcadia_MILC_TypeResolutionPhase);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhaseDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
  if (self->classCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->classCompleter);
  }
  if (self->constructorCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->constructorCompleter);
  }
  if (self->enumerationCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enumerationCompleter);
  }
  if (self->methodCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->methodCompleter);
  }
  if (self->procedureCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->procedureCompleter);
  }
}

Arcadia_MILC_TypeResolutionPhase*
Arcadia_MILC_TypeResolutionPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_TypeResolutionPhase);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_TypeResolutionPhase);
}

Arcadia_MILC_TypeResolutionPhase*
Arcadia_MILC_TypeResolutionPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_MILC_TypeResolutionPhase_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_TypeResolutionPhase_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_MILC_TypeResolutionPhase*)Arcadia_Value_getObjectReferenceValue(&v);
}

static void
onVisitVariableSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self,
    Arcadia_MILC_VariableSymbol* symbol
  )
{
}

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self,
    Arcadia_MILC_Symbol* symbol
  )
{
  // The member enter phase defers entering the member-level symbols to completers.
  // The completers are usually invoked by the backend. However, the type resolution
  // phase requires the member-level symbols (e.g. the symbols of enumeration constants)
  // to be entered. Hence, complete this symbol if it has a pending completer.
  if (symbol->completer) {
    Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
  }
  switch (symbol->kind) {
    case Arcadia_MILC_SymbolKind_Constructor: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_MILC_SymbolKind_Class: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_MemberEnterPhase_ClassCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_TypeResolutionPhase_ClassCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_MILC_SymbolKind_EnumerationConstant: {
      /*Intentionally empty.*/
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_MemberEnterPhase_EnumerationCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_TypeResolutionPhase_EnumerationCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_MILC_SymbolKind_Method: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_TypeResolutionPhase_MethodCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_MILC_SymbolKind_Module: {
      Arcadia_MILC_ModuleSymbol* moduleSymbol = (Arcadia_MILC_ModuleSymbol*)symbol;
      if (moduleSymbol->scope) {
        Arcadia_List* symbols = Arcadia_Map_getValues(thread, moduleSymbol->scope->entries);
        for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
          Arcadia_MILC_Symbol* symbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_MILC_Symbol_getType(thread));
          onVisitSymbol(thread, self, symbol);
        }
      }
    } break;
    case Arcadia_MILC_SymbolKind_Procedure: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_MILC_TypeResolutionPhase_ProcedureCompleter_getType(thread))) {
        Arcadia_MILC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_MILC_SymbolKind_Variable: {
      /*Intentionally empty.*/
    } break;
    case Arcadia_MILC_SymbolKind_PrimitiveType: {
      /* Intentionally empty. */
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

void
Arcadia_MILC_TypeResolutionPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  )
{
  Arcadia_List* symbols = Arcadia_Map_getValues(thread, self->context->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
    Arcadia_MILC_Symbol* symbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_MILC_Symbol_getType(thread));
    onVisitSymbol(thread, self, symbol);
  }
}

void
Arcadia_MILC_TypeResolutionPhase_resolveParameter
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self,
    Arcadia_MILC_VariableSymbol* variableSymbol
  )
{ 
  Arcadia_MILC_Symbol* moduleSymbol = (Arcadia_MILC_Symbol*)variableSymbol;
  do {
    moduleSymbol = moduleSymbol->enclosing;
  } while (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)moduleSymbol, _Arcadia_MILC_ModuleSymbol_getType(thread)));
  Arcadia_String* typeName = Arcadia_MILC_Names_toTypeName(thread, variableSymbol->ast->type->names);
  Arcadia_MILC_Symbol* typeSymbol = (Arcadia_MILC_Symbol*)Arcadia_Languages_Scope_lookup(thread, ((Arcadia_MILC_ModuleSymbol*)moduleSymbol)->scope, typeName, Arcadia_BooleanValue_True);
  if (!typeSymbol) {
    Arcadia_MILC_Environment* e = Arcadia_MILC_Symbols_getEnvironment(thread, Arcadia_MILC_Symbols_getInstance(thread, self->context), (Arcadia_MILC_Symbol*)variableSymbol);
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        self->context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_MILC_Diagnostics_SymbolIsNotDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, self->context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            typeName
          )
      );
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_MILC_TypeResolutionPhase_resolveReturnValue
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  )
{ }
