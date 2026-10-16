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
#include "Arcadia/PDLC/MemberEnterPhase/ClassCompleter.h"

#include "Arcadia/PDLC/Context.h"
#include "Arcadia/PDLC/Diagnostics/Include.h"
#include "Arcadia/PDLC/Symbols/Include.h"
#include "Arcadia/PDLC/TypeResolutionPhase.h"
#include "Arcadia/PDLC/Environment.h"
#include <assert.h>

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self
  );

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self
  );

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleterDispatch* self
  );

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self
  );

static void
onCompleteConstructor
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* classSymbol,
    Arcadia_PDLC_AST_ConstructorDefinitionNode* node
  );

static void
onCompleteField
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* classSymbol,
    Arcadia_PDLC_AST_FieldDefinitionNode* node
  );

static void
onCompleteMethod
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* classSymbol,
    Arcadia_PDLC_AST_MethodDefinitionNode* node
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ClassCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ClassCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ClassCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ClassCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.MemberEnterPhase.ClassCompleter", Arcadia_PDLC_MemberEnterPhase_ClassCompleter,
                         u8"Arcadia.PDLC.Completer", Arcadia_PDLC_Completer,
                         &_typeOperations);

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_MemberEnterPhase_ClassCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_MemberEnterPhase_ClassCompleter);
}

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleterDispatch* self
  )
{
  ((Arcadia_PDLC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Completer*, Arcadia_PDLC_Context*, Arcadia_PDLC_Symbol*)) & completeImpl;
}

static void
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self
  )
{/*Intentionally empty.*/}

/// Enter the parameter symbols declared by @c parameterDefinitionNodes into a constructor or method
/// symbol.
/// @c memberSymbol is the constructor or method symbol that owns the parameters, @c memberScope is
/// its parameter scope, and @c memberParameters collects the parameter symbols in order of
/// appearance. The input file of a duplicate-definition diagnostic is taken from the compilation unit
/// of @c e.
/// @remarks A constructor and a method declare their parameters the same way, so both enter them
/// through this function.
static void
enterParameters
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Environment* e,
    Arcadia_PDLC_Symbol* memberSymbol,
    Arcadia_Languages_Scope* memberScope,
    Arcadia_List* memberParameters,
    Arcadia_List* parameterDefinitionNodes
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)parameterDefinitionNodes); i < n; ++i) {
    Arcadia_PDLC_AST_FieldDefinitionNode* fieldDefinitionNode = (Arcadia_PDLC_AST_FieldDefinitionNode*)Arcadia_List_getObjectReferenceValueCheckedAt
      (
        thread, parameterDefinitionNodes, i,
        _Arcadia_PDLC_AST_FieldDefinitionNode_getType(thread)
      );
    Arcadia_PDLC_VariableSymbol* parameterSymbol = Arcadia_PDLC_VariableSymbol_create(thread, fieldDefinitionNode->name);
    ((Arcadia_PDLC_Symbol*)parameterSymbol)->enclosing = memberSymbol;
    parameterSymbol->ast = fieldDefinitionNode;
    Arcadia_List_insertBack(thread, memberParameters, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)parameterSymbol));
    if (Arcadia_Languages_Scope_contains(thread, memberScope, ((Arcadia_PDLC_Symbol*)parameterSymbol)->name, Arcadia_BooleanValue_False)) {
      Arcadia_Languages_Diagnostics_add
        (
          thread, context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
              Arcadia_SizeValue_Literal(0),
              fieldDefinitionNode->name
            )
        );
    } else {
      Arcadia_Languages_Scope_enter(thread, memberScope, ((Arcadia_PDLC_Symbol*)parameterSymbol)->name, (Arcadia_Object*)parameterSymbol);
    }
  }
}

static void
onCompleteConstructor
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* classSymbol,
    Arcadia_PDLC_AST_ConstructorDefinitionNode* node
  )
{
  Arcadia_PDLC_Environment* e =
    Arcadia_Value_getObjectReferenceValueChecked
      (
        thread,
        Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(classSymbol)),
        _Arcadia_PDLC_Environment_getType(thread)
      );
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, u8"<constructor>");
  // Enter the constructor symbol into the class symbol.
  Arcadia_PDLC_ConstructorSymbol* constructorSymbol = Arcadia_PDLC_ConstructorSymbol_create(thread, name);
  ((Arcadia_PDLC_Symbol*)constructorSymbol)->enclosing = (Arcadia_PDLC_Symbol*)classSymbol;
  constructorSymbol->ast = node;
  Arcadia_List_insertBack(thread, ((Arcadia_PDLC_ClassSymbol*)classSymbol)->members, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)constructorSymbol));
  if (Arcadia_Languages_Scope_contains(thread, classSymbol->scope, ((Arcadia_PDLC_Symbol*)constructorSymbol)->name, Arcadia_BooleanValue_False)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread, context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            name
          )
      );
  } else {
    Arcadia_Languages_Scope_enter(thread, classSymbol->scope, ((Arcadia_PDLC_Symbol*)constructorSymbol)->name, (Arcadia_Object*)constructorSymbol);
  }
  // Enter the parameter symbols into the constructor symbol.
  if (!constructorSymbol->scope) {
    constructorSymbol->scope = Arcadia_Languages_Scope_create(thread, NULL);
  }
  enterParameters
    (
      thread, context, e,
      (Arcadia_PDLC_Symbol*)constructorSymbol,
      constructorSymbol->scope,
      constructorSymbol->parameters,
      node->parameters
    );
  // This is why we use completers (among other reasons). We cannot resolve a constructor's types here.
  ((Arcadia_PDLC_Symbol*)constructorSymbol)->completer = Arcadia_PDLC_TypeResolutionPhase_getInstance(thread, context)->constructorCompleter;
}

static void
onCompleteField
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* classSymbol,
    Arcadia_PDLC_AST_FieldDefinitionNode* node
  )
{
  Arcadia_PDLC_Environment* e =
    Arcadia_Value_getObjectReferenceValueChecked
      (
        thread,
        Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(classSymbol)),
        _Arcadia_PDLC_Environment_getType(thread)
      );
  // Enter the field symbol into the class symbol.
  Arcadia_PDLC_VariableSymbol* fieldSymbol = Arcadia_PDLC_VariableSymbol_create(thread, node->name);
  ((Arcadia_PDLC_Symbol*)fieldSymbol)->enclosing = (Arcadia_PDLC_Symbol*)classSymbol;
  fieldSymbol->ast = node;
  Arcadia_List_insertBack(thread, ((Arcadia_PDLC_ClassSymbol*)classSymbol)->members, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)fieldSymbol));
  if (Arcadia_Languages_Scope_contains(thread, classSymbol->scope, ((Arcadia_PDLC_Symbol*)fieldSymbol)->name, Arcadia_BooleanValue_False)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread, context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            node->name
          )
      );
  } else {
    Arcadia_Languages_Scope_enter(thread, classSymbol->scope, ((Arcadia_PDLC_Symbol*)fieldSymbol)->name, (Arcadia_Object*)fieldSymbol);
  }
}

static void
onCompleteMethod
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* classSymbol,
    Arcadia_PDLC_AST_MethodDefinitionNode* node
  )
{
  Arcadia_PDLC_Environment* e =
    Arcadia_Value_getObjectReferenceValueChecked
      (
        thread,
        Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(classSymbol)),
        _Arcadia_PDLC_Environment_getType(thread)
      );
  // Enter the method symbol into the class symbol.
  Arcadia_PDLC_MethodSymbol* methodSymbol = Arcadia_PDLC_MethodSymbol_create(thread, node->name);
  methodSymbol->ast = node;
  methodSymbol->isNonvirtual = node->isNonvirtual;
  methodSymbol->isAbstract = node->isAbstract;
  ((Arcadia_PDLC_Symbol*)methodSymbol)->enclosing = (Arcadia_PDLC_Symbol*)classSymbol;
  Arcadia_List_insertBack(thread, ((Arcadia_PDLC_ClassSymbol*)classSymbol)->members, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)methodSymbol));
  if (Arcadia_Languages_Scope_contains(thread, classSymbol->scope, ((Arcadia_PDLC_Symbol*)methodSymbol)->name, Arcadia_BooleanValue_False)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_PDLC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            node->name
          )
      );
  } else {
    Arcadia_Languages_Scope_enter(thread, classSymbol->scope, ((Arcadia_PDLC_Symbol*)methodSymbol)->name, (Arcadia_Object*)methodSymbol);
  }
  // Enter the parameter symbols into the method symbol.
  if (!methodSymbol->scope) {
    methodSymbol->scope = Arcadia_Languages_Scope_create(thread, NULL);
  }
  enterParameters
    (
      thread, context, e,
      (Arcadia_PDLC_Symbol*)methodSymbol,
      methodSymbol->scope,
      methodSymbol->parameters,
      node->parameters
    );
  // This is why we use completers (among other reasons). We cannot resolve a methods types here.
  ((Arcadia_PDLC_Symbol*)methodSymbol)->completer = Arcadia_PDLC_TypeResolutionPhase_getInstance(thread, context)->methodCompleter;
}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ClassCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{
  assert(symbol->completer == (Arcadia_PDLC_Completer*)self);
  Arcadia_PDLC_AST_ClassDefinitionNode* node = ((Arcadia_PDLC_ClassSymbol*)symbol)->ast;
  if (!node) {
    symbol->completer = NULL;
    return;
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->classBody); i < n; ++i) {
    Arcadia_PDLC_AST_DefinitionNode* childNode = (Arcadia_PDLC_AST_DefinitionNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->classBody, i, _Arcadia_PDLC_AST_DefinitionNode_getType(thread));
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)childNode, _Arcadia_PDLC_AST_ConstructorDefinitionNode_getType(thread))) {
      onCompleteConstructor(thread, self, context, (Arcadia_PDLC_ClassSymbol*)symbol, (Arcadia_PDLC_AST_ConstructorDefinitionNode*)childNode);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)childNode, _Arcadia_PDLC_AST_FieldDefinitionNode_getType(thread))) {
      onCompleteField(thread, self, context, (Arcadia_PDLC_ClassSymbol*)symbol, (Arcadia_PDLC_AST_FieldDefinitionNode*)childNode);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)childNode, _Arcadia_PDLC_AST_MethodDefinitionNode_getType(thread))) {
      onCompleteMethod(thread, self, context, (Arcadia_PDLC_ClassSymbol*)symbol, (Arcadia_PDLC_AST_MethodDefinitionNode*)childNode);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  // This is for resolving the field types.
  symbol->completer = Arcadia_PDLC_TypeResolutionPhase_getInstance(thread, context)->classCompleter;
}

Arcadia_PDLC_MemberEnterPhase_ClassCompleter*
Arcadia_PDLC_MemberEnterPhase_ClassCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_MemberEnterPhase_ClassCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_MemberEnterPhase_ClassCompleter);
}
