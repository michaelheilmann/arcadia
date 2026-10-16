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
#include "Arcadia/PDLC/MemberEnterPhase/ProcedureCompleter.h"

#include "Arcadia/PDLC/Context.h"
#include "Arcadia/PDLC/Diagnostics/Include.h"
#include "Arcadia/PDLC/Symbols/Include.h"
#include "Arcadia/PDLC/TypeResolutionPhase.h"
#include "Arcadia/PDLC/Environment.h"
#include <assert.h>

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self
  );

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self
  );

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleterDispatch* self
  );

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.MemberEnterPhase.ProcedureCompleter", Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter,
                         u8"Arcadia.PDLC.Completer", Arcadia_PDLC_Completer,
                         &_typeOperations);

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter);
}

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleterDispatch* self
  )
{
  ((Arcadia_PDLC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Completer*, Arcadia_PDLC_Context*, Arcadia_PDLC_Symbol*)) & completeImpl;
}

static void
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self
  )
{/*Intentionally empty.*/}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{
  assert(symbol->completer == (Arcadia_PDLC_Completer*)self);
  Arcadia_PDLC_AST_ProcedureDefinitionNode* node = ((Arcadia_PDLC_ProcedureSymbol*)symbol)->ast;
  if (!node) {
    symbol->completer = NULL;
    return;
  }
  Arcadia_PDLC_ProcedureSymbol* procedureSymbol = (Arcadia_PDLC_ProcedureSymbol*)symbol;
  Arcadia_PDLC_Environment* e =
    Arcadia_Value_getObjectReferenceValueChecked
      (
        thread,
        Arcadia_Map_get(thread, context->environments, Arcadia_Value_makeObjectReferenceValue(procedureSymbol)),
        _Arcadia_PDLC_Environment_getType(thread)
      );
  // Enter the parameter symbols into the procedure symbol.
  if (!procedureSymbol->scope) {
    procedureSymbol->scope = Arcadia_Languages_Scope_create(thread, NULL);
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->parameters); i < n; ++i) {
    Arcadia_PDLC_AST_FieldDefinitionNode* fieldDefinitionNode = (Arcadia_PDLC_AST_FieldDefinitionNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->parameters, i, _Arcadia_PDLC_AST_FieldDefinitionNode_getType(thread));
    Arcadia_PDLC_VariableSymbol* fieldSymbol = Arcadia_PDLC_VariableSymbol_create(thread, fieldDefinitionNode->name);
    ((Arcadia_PDLC_Symbol*)fieldSymbol)->enclosing = (Arcadia_PDLC_Symbol*)procedureSymbol;
    fieldSymbol->ast = fieldDefinitionNode;
    Arcadia_List_insertBack(thread, procedureSymbol->parameters, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)fieldSymbol));
    if (Arcadia_Languages_Scope_contains(thread, procedureSymbol->scope, ((Arcadia_PDLC_Symbol*)fieldSymbol)->name, Arcadia_BooleanValue_False)) {
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
      Arcadia_Languages_Scope_enter(thread, procedureSymbol->scope, ((Arcadia_PDLC_Symbol*)fieldSymbol)->name, (Arcadia_Object*)fieldSymbol);
    }
  }
  symbol->completer = Arcadia_PDLC_TypeResolutionPhase_getInstance(thread, context)->procedureCompleter;
}

Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter*
Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter);
}
