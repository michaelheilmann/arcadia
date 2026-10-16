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
#include "Arcadia/PDLC/Symbols/VariableSymbol.h"
#include "Arcadia/PDLC/AST/Definitions/FieldDefinitionNode.h"

static void
Arcadia_PDLC_VariableSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbol* self
  );

static void
Arcadia_PDLC_VariableSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbolDispatch* self
  );

static void
Arcadia_PDLC_VariableSymbol_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbol* self
  );

static void
Arcadia_PDLC_VariableSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbol* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_VariableSymbol_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_VariableSymbol_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_VariableSymbol_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_VariableSymbol_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.VariableSymbol", Arcadia_PDLC_VariableSymbol,
                         u8"Arcadia.PDLC.Symbol", Arcadia_PDLC_Symbol,
                         &_typeOperations);

static void
Arcadia_PDLC_VariableSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_VariableSymbol);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  {
    Arcadia_Value temporary = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_PDLC_SymbolKind_getType(thread), Arcadia_PDLC_SymbolKind_Variable));
    Arcadia_ValueStack_pushValue(thread, &temporary);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->ast = NULL;
  self->type = NULL;
  self->isReference = Arcadia_BooleanValue_False;
  self->isReadOnly = Arcadia_BooleanValue_False;
  self->isOutput = Arcadia_BooleanValue_False;
  self->ownership = Arcadia_PDLC_Ownership_Managed;
  self->recordFields = NULL;
  self->isConstant = Arcadia_BooleanValue_False;
  self->constantValue = 0;
  //
  Arcadia_LeaveConstructor(Arcadia_PDLC_VariableSymbol);
}

static void
Arcadia_PDLC_VariableSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbolDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_VariableSymbol_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbol* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_VariableSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_VariableSymbol* self
  )
{
  if (self->recordFields) Arcadia_Object_visit(thread, (Arcadia_Object*)self->recordFields);
  if (self->ast) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ast);
  }
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);

  }
}

Arcadia_PDLC_VariableSymbol*
Arcadia_PDLC_VariableSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_VariableSymbol);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_VariableSymbol);
}
