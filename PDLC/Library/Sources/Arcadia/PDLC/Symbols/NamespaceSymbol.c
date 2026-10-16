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
#include "Arcadia/PDLC/Symbols/NamespaceSymbol.h"

static void
Arcadia_PDLC_NamespaceSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbol* self
  );

static void
Arcadia_PDLC_NamespaceSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbolDispatch* self
  );

static void
Arcadia_PDLC_NamespaceSymbol_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbol* self
  );

static void
Arcadia_PDLC_NamespaceSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbol* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_NamespaceSymbol_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_NamespaceSymbol_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_NamespaceSymbol_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_NamespaceSymbol_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.NamespaceSymbol", Arcadia_PDLC_NamespaceSymbol,
                         u8"Arcadia.PDLC.Symbol", Arcadia_PDLC_Symbol,
                         &_typeOperations);

static void
Arcadia_PDLC_NamespaceSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_NamespaceSymbol);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  {
    Arcadia_Value temporary = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_PDLC_SymbolKind_getType(thread), Arcadia_PDLC_SymbolKind_Namespace));
    Arcadia_ValueStack_pushValue(thread, &temporary);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_PDLC_NamespaceSymbol);
}

static void
Arcadia_PDLC_NamespaceSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbolDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_NamespaceSymbol_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbol* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_NamespaceSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_NamespaceSymbol* self
  )
{
  if (self->scope) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->scope);
  }
}

Arcadia_PDLC_NamespaceSymbol*
Arcadia_PDLC_NamespaceSymbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_NamespaceSymbol);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_NamespaceSymbol);
}
