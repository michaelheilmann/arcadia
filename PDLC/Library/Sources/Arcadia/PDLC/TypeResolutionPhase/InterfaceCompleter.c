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
#include "Arcadia/PDLC/TypeResolutionPhase/InterfaceCompleter.h"

#include "Arcadia/PDLC/Context.h"
#include "Arcadia/PDLC/Diagnostics/Include.h"
#include "Arcadia/PDLC/Symbols/Include.h"
#include "Arcadia/PDLC/Names.h"
#include "Arcadia/PDLC/Environment.h"
#include <assert.h>

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self
  );

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self
  );

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleterDispatch* self
  );

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self
  );

static void
resolveExtendedInterfaceTypes
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_InterfaceSymbol* symbol
  );

static void
onCompleteInterface
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_InterfaceSymbol* symbol
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.TypeResolutionPhase.InterfaceCompleter", Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter,
                         u8"Arcadia.PDLC.Completer", Arcadia_PDLC_Completer,
                         &_typeOperations);

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->cycleCheck = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->pending = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter);
}

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleterDispatch* self
  )
{
  ((Arcadia_PDLC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Completer*, Arcadia_PDLC_Context*, Arcadia_PDLC_Symbol*)) & completeImpl;
}

static void
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self
  )
{
  if (self->cycleCheck) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cycleCheck);
  }
  if (self->pending) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->pending);
  }
}

/// @brief Resolve the names of the extended interface types of @c symbol to interface symbols.
/// LANGUAGE DEFINITION: Each extended interface type must be an interface type.
static void
resolveExtendedInterfaceTypes
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_InterfaceSymbol* symbol
  )
{
  Arcadia_List* extendedInterfaceNames = symbol->ast->extendedInterfaceNames;
  if (!extendedInterfaceNames) {
    return;
  }
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbol->extendedInterfaceSymbols) ==
      Arcadia_Collection_getSize(thread, (Arcadia_Collection*)extendedInterfaceNames)) {
    return; /* Already resolved. */
  }
  Arcadia_PDLC_Symbol* moduleSymbol = (Arcadia_PDLC_Symbol*)symbol;
  do {
    moduleSymbol = moduleSymbol->enclosing;
    assert(NULL != moduleSymbol);
  } while (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)moduleSymbol, _Arcadia_PDLC_ModuleSymbol_getType(thread)));
  Arcadia_PDLC_Environment* e =
    (Arcadia_PDLC_Environment*)
    Arcadia_Map_getObjectReferenceValueChecked
      (
        thread, context->environments, Arcadia_Value_makeObjectReferenceValue(symbol),
        _Arcadia_PDLC_Environment_getType(thread)
      );
  Arcadia_Languages_InputFile* file =
    Arcadia_Languages_InputFileManager_createPhysicalInputFile
      (
        thread, context->inputFileManager,
        Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False),
        e->compilationUnitNode->filePath
      );
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)extendedInterfaceNames); i < n; ++i) {
    Arcadia_PDLC_AST_IdentifierNode* extendedInterfaceName = (Arcadia_PDLC_AST_IdentifierNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, extendedInterfaceNames, i, _Arcadia_PDLC_AST_IdentifierNode_getType(thread));
    Arcadia_String* typeName = Arcadia_PDLC_Names_toTypeName(thread, extendedInterfaceName->names);
    Arcadia_PDLC_Symbol* extendedInterfaceSymbol = (Arcadia_PDLC_Symbol*)Arcadia_Languages_Scope_lookup(thread, ((Arcadia_PDLC_ModuleSymbol*)moduleSymbol)->scope, typeName, Arcadia_BooleanValue_True);
    if (!extendedInterfaceSymbol) {
      Arcadia_Languages_Diagnostics_add
        (
          thread,
          context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_PDLC_Diagnostics_SymbolIsNotDefinedDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              file,
              Arcadia_SizeValue_Literal(0),
              typeName
            )
        );
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    if (extendedInterfaceSymbol->kind != Arcadia_PDLC_SymbolKind_Interface) {
      Arcadia_Languages_Diagnostics_add
        (
          thread,
          context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_PDLC_Diagnostics_SymbolIsNoInterfaceDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              file,
              Arcadia_SizeValue_Literal(0),
              typeName
            )
        );
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_List_insertBack(thread, symbol->extendedInterfaceSymbols, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)extendedInterfaceSymbol));
  }
}

/// @brief LANGUAGE DEFINITION: Assert the inheritance graph of interface types is acyclic.
/// @remarks
/// An interface type may extend the same interface type at most once, transitively.
static void
onCompleteInterface
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_InterfaceSymbol* symbol
  )
{
  resolveExtendedInterfaceTypes(thread, self, context, symbol);
  // Recursively resolve the ancestor interface types and ensure there is no inheritance cycle.
  // @note `self->pending` is owned by this completer, which is owned by the type resolution phase,
  // which is owned by the context. The work list is therefore reachable for the garbage collector.
  Arcadia_List* pending = self->pending;
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)pending);
  Arcadia_List_insertBackObjectReferenceValue(thread, pending, (Arcadia_ObjectReferenceValue)symbol);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->cycleCheck);
  while (!Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)pending)) {
    Arcadia_PDLC_InterfaceSymbol* p =
      (Arcadia_PDLC_InterfaceSymbol*)
      Arcadia_List_getObjectReferenceValueAt(thread, pending, 0);
    Arcadia_List_removeFront(thread, pending, 1);
    Arcadia_Value value = Arcadia_Map_get(thread, self->cycleCheck, Arcadia_Value_makeObjectReferenceValue(p));
    if (!Arcadia_Value_isVoidValue(&value)) {
      Arcadia_PDLC_Environment* e =
        (Arcadia_PDLC_Environment*)
        Arcadia_Map_getObjectReferenceValueChecked
          (
            thread, context->environments, Arcadia_Value_makeObjectReferenceValue(symbol),
            _Arcadia_PDLC_Environment_getType(thread)
          );
      Arcadia_Languages_Diagnostics_add
        (
          thread,
          context->diagnostics,
          (Arcadia_Languages_Diagnostic*)
          Arcadia_PDLC_Diagnostics_CyclicInterfaceInheritanceDiagnostic_create
            (
              thread,
              Arcadia_Languages_DiagnosticType_Error,
              Arcadia_Languages_InputFileManager_createPhysicalInputFile
                (
                  thread, context->inputFileManager,
                  Arcadia_FilePath_toNative(thread, e->compilationUnitNode->filePath, Arcadia_BooleanValue_False),
                  e->compilationUnitNode->filePath
                ),
              Arcadia_SizeValue_Literal(0),
              ((Arcadia_PDLC_Symbol*)symbol)->name
            )
        );
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Map_set(thread, self->cycleCheck, Arcadia_Value_makeObjectReferenceValue(p), Arcadia_Value_makeObjectReferenceValue(p), NULL, NULL);
    resolveExtendedInterfaceTypes(thread, self, context, p);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)p->extendedInterfaceSymbols); i < n; ++i) {
      Arcadia_PDLC_InterfaceSymbol* q =
        (Arcadia_PDLC_InterfaceSymbol*)
        Arcadia_List_getObjectReferenceValueCheckedAt(thread, p->extendedInterfaceSymbols, i, _Arcadia_PDLC_InterfaceSymbol_getType(thread));
      Arcadia_List_insertBackObjectReferenceValue(thread, pending, (Arcadia_ObjectReferenceValue)q);
    }
  }
}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{
  assert(symbol->completer == (Arcadia_PDLC_Completer*)self);
  Arcadia_PDLC_InterfaceSymbol* interfaceSymbol = (Arcadia_PDLC_InterfaceSymbol*)symbol;
  if (!interfaceSymbol->ast) {
    symbol->completer = NULL;
    return;
  }
  onCompleteInterface(thread, self, context, interfaceSymbol);
  symbol->completer = NULL;
}

Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter*
Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_TypeResolutionPhase_InterfaceCompleter);
}
