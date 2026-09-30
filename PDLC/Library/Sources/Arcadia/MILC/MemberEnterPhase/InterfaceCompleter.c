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
#include "Arcadia/MILC/MemberEnterPhase/InterfaceCompleter.h"

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Diagnostics/Include.h"
#include "Arcadia/MILC/Symbols/Include.h"
#include "Arcadia/MILC/TypeResolutionPhase.h"
#include "Arcadia/MILC/Environment.h"
#include <assert.h>

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self
  );

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self
  );

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleterDispatch* self
  );

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self
  );

static void
onCompleteOperation
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_InterfaceSymbol* interfaceSymbol,
    Arcadia_MILC_AST_MethodDefinitionNode* node
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.MemberEnterPhase.InterfaceCompleter", Arcadia_MILC_MemberEnterPhase_InterfaceCompleter,
                         u8"Arcadia.MILC.Completer", Arcadia_MILC_Completer,
                         &_typeOperations);

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_MemberEnterPhase_InterfaceCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_MemberEnterPhase_InterfaceCompleter);
}

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleterDispatch* self
  )
{
  ((Arcadia_MILC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_MILC_Completer*, Arcadia_MILC_Context*, Arcadia_MILC_Symbol*)) & completeImpl;
}

static void
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self
  )
{/*Intentionally empty.*/}

static void
onCompleteOperation
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_InterfaceSymbol* interfaceSymbol,
    Arcadia_MILC_AST_MethodDefinitionNode* node
  )
{
  Arcadia_MILC_Environment* e =
    Arcadia_Value_getObjectReferenceValueChecked
      (
        thread,
        Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(interfaceSymbol)),
        _Arcadia_MILC_Environment_getType(thread)
      );
  // Enter the operation symbol into the interface symbol.
  Arcadia_MILC_MethodSymbol* methodSymbol = Arcadia_MILC_MethodSymbol_create(thread, node->name);
  methodSymbol->ast = node;
  ((Arcadia_MILC_Symbol*)methodSymbol)->enclosing = (Arcadia_MILC_Symbol*)interfaceSymbol;
  Arcadia_List_insertBack(thread, interfaceSymbol->operations, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)methodSymbol));
  if (Arcadia_Languages_Scope_contains(thread, interfaceSymbol->scope, ((Arcadia_MILC_Symbol*)methodSymbol)->name, Arcadia_BooleanValue_False)) {
    Arcadia_Languages_Diagnostics_add
      (
        thread,
        context->diagnostics,
        (Arcadia_Languages_Diagnostic*)
        Arcadia_MILC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
          (
            thread,
            Arcadia_Languages_DiagnosticType_Error,
            Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
            Arcadia_SizeValue_Literal(0),
            node->name
          )
      );
  } else {
    Arcadia_Languages_Scope_enter(thread, interfaceSymbol->scope, ((Arcadia_MILC_Symbol*)methodSymbol)->name, (Arcadia_Object*)methodSymbol);
  }
  // Enter the parameter symbols into the operation symbol.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->parameters); i < n; ++i) {
    Arcadia_MILC_AST_FieldDefinitionNode* fieldDefinitionNode = (Arcadia_MILC_AST_FieldDefinitionNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->parameters, i, _Arcadia_MILC_AST_FieldDefinitionNode_getType(thread));
    Arcadia_MILC_VariableSymbol* fieldSymbol = Arcadia_MILC_VariableSymbol_create(thread, fieldDefinitionNode->name);
    ((Arcadia_MILC_Symbol*)fieldSymbol)->enclosing = (Arcadia_MILC_Symbol*)methodSymbol;
    fieldSymbol->ast = fieldDefinitionNode;
    Arcadia_List_insertBack(thread, methodSymbol->parameters, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)fieldSymbol));
    if (Arcadia_Languages_Scope_contains(thread, methodSymbol->scope, ((Arcadia_MILC_Symbol*)fieldSymbol)->name, Arcadia_BooleanValue_False)) {
      Arcadia_Languages_Diagnostics_add
        (
          thread,
          context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_MILC_Diagnostics_SymbolIsAlreadyDefinedDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, context->inputFileManager, Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False), e->compilationUnitNode->filePath),
              Arcadia_SizeValue_Literal(0),
              fieldDefinitionNode->name
            )
        );
    } else {
      Arcadia_Languages_Scope_enter(thread, methodSymbol->scope, ((Arcadia_MILC_Symbol*)fieldSymbol)->name, (Arcadia_Object*)fieldSymbol);
    }
  }
  // This is why we use completers (among other reasons). We cannot resolve an operation's types here.
  ((Arcadia_MILC_Symbol*)methodSymbol)->completer = Arcadia_MILC_TypeResolutionPhase_getInstance(thread, context)->methodCompleter;
}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_InterfaceCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{
  assert(symbol->completer == (Arcadia_MILC_Completer*)self);
  Arcadia_MILC_AST_InterfaceDefinitionNode* node = ((Arcadia_MILC_InterfaceSymbol*)symbol)->ast;
  if (!node || !node->operations) {
    symbol->completer = NULL;
    return;
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->operations); i < n; ++i) {
    Arcadia_MILC_AST_DefinitionNode* childNode = (Arcadia_MILC_AST_DefinitionNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->operations, i, _Arcadia_MILC_AST_DefinitionNode_getType(thread));
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)childNode, _Arcadia_MILC_AST_MethodDefinitionNode_getType(thread))) {
      onCompleteOperation(thread, self, context, (Arcadia_MILC_InterfaceSymbol*)symbol, (Arcadia_MILC_AST_MethodDefinitionNode*)childNode);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  // This is for resolving the extended interface types and the operation types.
  symbol->completer = Arcadia_MILC_TypeResolutionPhase_getInstance(thread, context)->interfaceCompleter;
}

Arcadia_MILC_MemberEnterPhase_InterfaceCompleter*
Arcadia_MILC_MemberEnterPhase_InterfaceCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_MemberEnterPhase_InterfaceCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_MemberEnterPhase_InterfaceCompleter);
}
