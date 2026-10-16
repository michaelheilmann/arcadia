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

#include "Arcadia/PDLC/MemberEnterPhase.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/MemberEnterPhase/ClassCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/EnumerationCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/InterfaceCompleter.h"
#include "Arcadia/PDLC/MemberEnterPhase/ProcedureCompleter.h"
#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhaseDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  );

static void
onVisitClassSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_ClassSymbol* symbol
  );

static void
onVisitInterfaceSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_InterfaceSymbol* symbol
  );

static void
onVisitEnumerationSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_EnumerationSymbol* symbol
  );

static void
onVisitProcedureSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_ProcedureSymbol* symbol
  );

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
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

Arcadia_defineObjectType(u8"Arcadia.PDLC.MemberEnterPhase", Arcadia_PDLC_MemberEnterPhase,
                         u8"Arcadia.PDLC.AST.Visitor", Arcadia_PDLC_AST_Visitor,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_MemberEnterPhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->classCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_MemberEnterPhase_ClassCompleter_create(thread);
  self->enumerationCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_MemberEnterPhase_EnumerationCompleter_create(thread);
  self->interfaceCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_MemberEnterPhase_InterfaceCompleter_create(thread);
  self->procedureCompleter = (Arcadia_PDLC_Completer*)Arcadia_PDLC_MemberEnterPhase_ProcedureCompleter_create(thread);
  self->context = (Arcadia_PDLC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_Context_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_MemberEnterPhase);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhaseDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
  if (self->classCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->classCompleter);
  }
  if (self->enumerationCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enumerationCompleter);
  }
  if (self->interfaceCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->interfaceCompleter);
  }
  if (self->procedureCompleter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->procedureCompleter);
  }
}

static void
onVisitClassSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_ClassSymbol* symbol
  )
{
  ((Arcadia_PDLC_Symbol*)symbol)->completer = self->classCompleter;
}

static void
onVisitEnumerationSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_EnumerationSymbol* symbol
  )
{
  ((Arcadia_PDLC_Symbol*)symbol)->completer = self->enumerationCompleter;
}

static void
onVisitInterfaceSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_InterfaceSymbol* symbol
  )
{
  ((Arcadia_PDLC_Symbol*)symbol)->completer = self->interfaceCompleter;
}

static void
onVisitProcedureSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_ProcedureSymbol* symbol
  )
{
  ((Arcadia_PDLC_Symbol*)symbol)->completer = self->procedureCompleter;
}

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self,
    Arcadia_PDLC_Symbol* symbol
  )
{
  switch (symbol->kind) {
    case Arcadia_PDLC_SymbolKind_Class: {
      onVisitClassSymbol(thread, self, (Arcadia_PDLC_ClassSymbol*)symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_Enumeration: {
      onVisitEnumerationSymbol(thread, self, (Arcadia_PDLC_EnumerationSymbol*)symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_Interface: {
      onVisitInterfaceSymbol(thread, self, (Arcadia_PDLC_InterfaceSymbol*)symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_Procedure: {
      onVisitProcedureSymbol(thread, self, (Arcadia_PDLC_ProcedureSymbol*)symbol);
    } break;
    case Arcadia_PDLC_SymbolKind_Module: {
      Arcadia_List* symbols = Arcadia_Map_getValues(thread, ((Arcadia_PDLC_ModuleSymbol*)symbol)->scope->entries);
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
        Arcadia_PDLC_Symbol* symbol = (Arcadia_PDLC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_PDLC_Symbol_getType(thread));
        onVisitSymbol(thread, self, symbol);
      }
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

Arcadia_PDLC_MemberEnterPhase*
Arcadia_PDLC_MemberEnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_MemberEnterPhase);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_MemberEnterPhase);
}

Arcadia_PDLC_MemberEnterPhase*
Arcadia_PDLC_MemberEnterPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_PDLC_MemberEnterPhase_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_PDLC_MemberEnterPhase_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_PDLC_MemberEnterPhase*)Arcadia_Value_getObjectReferenceValue(&v);
}

void
Arcadia_PDLC_MemberEnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  )
{
  Arcadia_List* symbols = Arcadia_Map_getValues(thread, self->context->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
    Arcadia_PDLC_Symbol* symbol = (Arcadia_PDLC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_PDLC_Symbol_getType(thread));
    onVisitSymbol(thread, self, symbol);
  }
}

