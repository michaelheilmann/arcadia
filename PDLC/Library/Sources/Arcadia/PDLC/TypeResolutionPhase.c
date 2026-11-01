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
#include "Arcadia/PDLC/TypeResolutionPhase.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Diagnostics/Include.h"
#include "Arcadia/PDLC/Symbols/Include.h"
#include "Arcadia/PDLC/Completer.h"

#include "Arcadia/PDLC/TypeResolutionPhase/ClassCompleter.h"
#include "Arcadia/PDLC/TypeResolutionPhase/ConstructorCompleter.h"
#include "Arcadia/PDLC/TypeResolutionPhase/EnumerationCompleter.h"
#include "Arcadia/PDLC/TypeResolutionPhase/InterfaceCompleter.h"
#include "Arcadia/PDLC/TypeResolutionPhase/MethodCompleter.h"
#include "Arcadia/PDLC/TypeResolutionPhase/ProcedureCompleter.h"

#include "Arcadia/PDLC/MemberEnterPhase.h"
#include "Arcadia/PDLC/MemberEnterPhase/ClassCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/EnumerationCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/InterfaceCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/ProcedureCompleter.h"

#include "Arcadia/PDLC/Environment.h"

#include "Arcadia/PDLC/Names.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhaseDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  );

static void
onVisitVariableSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_VariableSymbol* symbol
  );

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_Symbol* symbol
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

Arcadia_defineObjectType(u8"Arcadia.PDLC.TypeResolutionPhase", Arcadia_PDLC_TypeResolutionPhase,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_TypeResolutionPhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->context = (Arcadia_PDLC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_Context_getType(thread));
  self->classCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_TypeResolutionPhase_ClassCompleter_create(thread);
  self->constructorCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_TypeResolutionPhase_ConstructorCompleter_create(thread);
  self->enumerationCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_TypeResolutionPhase_EnumerationCompleter_create(thread);
  self->interfaceCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_create(thread);
  self->methodCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_create(thread);
  self->procedureCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_TypeResolutionPhase_ProcedureCompleter_create(thread);
  Arcadia_LeaveConstructor(Arcadia_PDLC_TypeResolutionPhase);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhaseDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
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
  if (self->interfaceCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->interfaceCompleter);
  }
  if (self->methodCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->methodCompleter);
  }
  if (self->procedureCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->procedureCompleter);
  }
}

Arcadia_PDLC_TypeResolutionPhase*
Arcadia_PDLC_TypeResolutionPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_TypeResolutionPhase);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_TypeResolutionPhase);
}

Arcadia_PDLC_TypeResolutionPhase*
Arcadia_PDLC_TypeResolutionPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_PDLC_TypeResolutionPhase_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_PDLC_TypeResolutionPhase_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_PDLC_TypeResolutionPhase*)Arcadia_Value_getObjectReferenceValue(&v);
}

static void
onVisitVariableSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_VariableSymbol* symbol
  )
{
}

static void onVisitSymbol(Arcadia_Thread* thread, Arcadia_PDLC_TypeResolutionPhase* self, Arcadia_PDLC_Symbol* symbol);

/// Visit the symbols entered in the scope of @c symbol.
/// @remark A null scope is tolerated: the built-in types and the built-in class type `Arcadia.Object`
/// have no scope.
static void
onVisitSymbolScope
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_Symbol* symbol
  )
{
  Arcadia_Languages_Scope* scope = NULL;
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_PDLC_ClassSymbol_getType(thread))) {
    scope = ((Arcadia_PDLC_ClassSymbol*)symbol)->scope;
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_PDLC_InterfaceSymbol_getType(thread))) {
    scope = ((Arcadia_PDLC_InterfaceSymbol*)symbol)->scope;
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_PDLC_ModuleSymbol_getType(thread))) {
    scope = ((Arcadia_PDLC_ModuleSymbol*)symbol)->scope;
  } else {
    /*Intentionally empty: the symbol does not own a scope of member symbols.*/
  }
  if (!scope) {
    return;
  }
  Arcadia_List* symbols = Arcadia_Map_getValues(thread, scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
    Arcadia_PDLC_Symbol* memberSymbol = (Arcadia_PDLC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_PDLC_Symbol_getType(thread));
    onVisitSymbol(thread, self, memberSymbol);
  }
}

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_Symbol* symbol
  )
{
  // The member enter phase defers entering the member-level symbols to completers.
  // The completers are usually invoked by the backend. However, the type resolution
  // phase requires the member-level symbols (e.g. the symbols of enumeration constants)
  // to be entered. Hence, complete this symbol if it has a pending completer.
  if (symbol->completer) {
    Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
  }
  switch (symbol->kind) {
    case Arcadia_PDLC_SymbolKind_Constructor: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_TypeResolutionPhase_ConstructorCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_PDLC_SymbolKind_Class: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_MemberEnterPhase_ClassCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_TypeResolutionPhase_ClassCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      // Visit the member symbols because the member enter phase deferred completing them to
      // completers which are invoked here: the constructors and methods have pending completers,
      // and the parameter type of a constructor or method parameter is resolved by those.
      onVisitSymbolScope(thread, self, symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_EnumerationConstant: {
      /*Intentionally empty.*/
    } break;
    case Arcadia_PDLC_SymbolKind_Enumeration: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_MemberEnterPhase_EnumerationCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_TypeResolutionPhase_EnumerationCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_PDLC_SymbolKind_Interface: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_MemberEnterPhase_InterfaceCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      // Visit the member symbols: the operations of an interface type have pending completers.
      onVisitSymbolScope(thread, self, symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_Method: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_TypeResolutionPhase_MethodCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_PDLC_SymbolKind_Module: {
      onVisitSymbolScope(thread, self, symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_Procedure: {
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
      if (symbol->completer && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol->completer, _Arcadia_PDLC_TypeResolutionPhase_ProcedureCompleter_getType(thread))) {
        Arcadia_PDLC_Completer_complete(thread, symbol->completer, self->context, symbol);
      }
    } break;
    case Arcadia_PDLC_SymbolKind_Variable: {
      /*Intentionally empty.*/
    } break;
    case Arcadia_PDLC_SymbolKind_PrimitiveType: {
      /* Intentionally empty. */
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

void
Arcadia_PDLC_TypeResolutionPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  )
{
  Arcadia_List* symbols = Arcadia_Map_getValues(thread, self->context->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
    Arcadia_PDLC_Symbol* symbol = (Arcadia_PDLC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_PDLC_Symbol_getType(thread));
    onVisitSymbol(thread, self, symbol);
  }
}

/// Search the environment of @c symbol or of the closest enclosing symbol that has one.
/// @remark Environments are recorded for module, class, interface, enumeration, and operation
/// symbols, but not for variable symbols. A variable symbol therefore has to consult its
/// enclosing operation or type symbol.
static Arcadia_PDLC_Environment*
getEnclosingEnvironment
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{
  do {
    Arcadia_Value value = Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(symbol));
    if (!Arcadia_Value_isVoidValue(&value)) {
      return (Arcadia_PDLC_Environment*)
        Arcadia_Value_getObjectReferenceValueChecked
          (
            thread, value,
            _Arcadia_PDLC_Environment_getType(thread)
          );
    }
    symbol = symbol->enclosing;
  } while (symbol);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

/// Resolve the type name @c type, which is recorded in the abstract syntax tree of @c symbol.
/// @remark Arcadia PDL type names are fully qualified; there is no relative resolution of type
/// names. The type name is looked up as-is, and the enclosing module symbol is used because that
/// is the scope which holds the fully qualified names of the types declared by the enclosing
/// module.
static Arcadia_PDLC_Symbol*
resolveType
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_Symbol* symbol,
    Arcadia_PDLC_AST_IdentifierNode* type
  )
{
  if (!type) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_PDLC_Symbol* moduleSymbol = symbol;
  do {
    moduleSymbol = moduleSymbol->enclosing;
    if (!moduleSymbol) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
  } while (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)moduleSymbol, _Arcadia_PDLC_ModuleSymbol_getType(thread)));
  Arcadia_String* typeName = Arcadia_PDLC_Names_toTypeName(thread, type->names);
  Arcadia_PDLC_Symbol* typeSymbol = (Arcadia_PDLC_Symbol*)Arcadia_Languages_Scope_lookup
    (
      thread,
      ((Arcadia_PDLC_ModuleSymbol*)moduleSymbol)->scope,
      typeName,
      Arcadia_BooleanValue_True
    );
  if (!typeSymbol) {
    Arcadia_PDLC_Environment* e = getEnclosingEnvironment(thread, self->context, symbol);
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        self->context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_SymbolIsNotDefinedDiagnostic_create
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
  return typeSymbol;
}

void
Arcadia_PDLC_TypeResolutionPhase_resolveParameter
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_VariableSymbol* variableSymbol
  )
{
  // The type of a parameter is specified by its abstract syntax tree node.
  if (!variableSymbol->ast) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_PDLC_AST_FieldDefinitionNode* ast = variableSymbol->ast;
  variableSymbol->isReference = ast->isReference;
  variableSymbol->isReadOnly = ast->isReadOnly;
  variableSymbol->isOutput = ast->isOutput;
  variableSymbol->ownership = ast->ownership;
  variableSymbol->isConstant = ast->initializer != NULL;
  if (ast->recordFields) {
    variableSymbol->recordFields = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)ast->recordFields); ++i) {
      Arcadia_PDLC_AST_FieldDefinitionNode* childAst = Arcadia_List_getObjectReferenceValueAt(thread, ast->recordFields, i);
      Arcadia_PDLC_VariableSymbol* child = Arcadia_PDLC_VariableSymbol_create(thread, childAst->name);
      ((Arcadia_PDLC_Symbol*)child)->enclosing = ((Arcadia_PDLC_Symbol*)variableSymbol)->enclosing;
      child->ast = childAst;
      Arcadia_List_insertBackObjectReferenceValue(thread, variableSymbol->recordFields, child);
      for (Arcadia_SizeValue j = 0; j < i; ++j) {
        Arcadia_PDLC_VariableSymbol* previous = Arcadia_List_getObjectReferenceValueAt(thread, variableSymbol->recordFields, j);
        if (Arcadia_String_isEqualTo_pn(thread, ((Arcadia_PDLC_Symbol*)child)->name,
              Arcadia_String_getBytes(thread, ((Arcadia_PDLC_Symbol*)previous)->name),
              Arcadia_String_getNumberOfBytes(thread, ((Arcadia_PDLC_Symbol*)previous)->name))) {
          Arcadia_PDLC_Environment* e = getEnclosingEnvironment(thread, self->context, (Arcadia_PDLC_Symbol*)variableSymbol);
          Arcadia_Languages_Diagnostics_add(thread, self->context->diagnostics, (Arcadia_Languages_Diagnostic*)
            Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, self->context->inputFileManager,
                Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
              ((Arcadia_PDLC_AST_DefinitionNode*)childAst)->startOffset, childAst->name));
          Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
          Arcadia_Thread_jump(thread);
        }
      }
      Arcadia_PDLC_TypeResolutionPhase_resolveParameter(thread, self, child);
    }
    return;
  }
  variableSymbol->type = resolveType
    (
      thread,
      self,
      (Arcadia_PDLC_Symbol*)variableSymbol,
      variableSymbol->ast->type
    );
  if (ast->initializer) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      variableSymbol->constantValue = Arcadia_String_toInteger32(thread, ast->initializer->value);
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_PDLC_Environment* e = getEnclosingEnvironment(thread, self->context, (Arcadia_PDLC_Symbol*)variableSymbol);
      Arcadia_Languages_Diagnostics_add(thread, self->context->diagnostics, (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic_createForSymbol(thread, Arcadia_Languages_DiagnosticType_Error,
          Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, self->context->inputFileManager,
            Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
          ((Arcadia_PDLC_AST_DefinitionNode*)ast)->startOffset, (Arcadia_PDLC_Symbol*)variableSymbol, ast->initializer->value));
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
  }
}

void
Arcadia_PDLC_TypeResolutionPhase_resolveReturnValue
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_Symbol* symbol
  )
{
  // The return type of a method or procedure is specified by its abstract syntax tree node.
  // A native method or procedure carries no abstract syntax tree: its return type cannot be
  // resolved and remains unset.
  switch (symbol->kind) {
    case Arcadia_PDLC_SymbolKind_Method: {
      Arcadia_PDLC_MethodSymbol* methodSymbol = (Arcadia_PDLC_MethodSymbol*)symbol;
      if (!methodSymbol->ast) {
        return;
      }
      methodSymbol->returnValueType = resolveType(thread, self, symbol, methodSymbol->ast->returnType);
    } break;
    case Arcadia_PDLC_SymbolKind_Procedure: {
      Arcadia_PDLC_ProcedureSymbol* procedureSymbol = (Arcadia_PDLC_ProcedureSymbol*)symbol;
      if (!procedureSymbol->ast) {
        return;
      }
      procedureSymbol->returnValueType = resolveType(thread, self, symbol, procedureSymbol->ast->returnType);
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}
