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

#include "Arcadia/DDLS/Symbols/MapEntrySymbol.h"

static void
Arcadia_DDLS_MapEntrySymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapEntrySymbol* self
  );

static void
Arcadia_DDLS_MapEntrySymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapEntrySymbolDispatch* self
  );

static void
Arcadia_DDLS_MapEntrySymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapEntrySymbol* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_MapEntrySymbol_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_MapEntrySymbol_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_MapEntrySymbol_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_MapEntrySymbol_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_MapEntrySymbol_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_MapEntrySymbol_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.MapEntrySymbol", Arcadia_DDLS_MapEntrySymbol,
                         u8"Arcadia.DDLS.Symbol", Arcadia_DDLS_Symbol,
                         &_Arcadia_DDLS_MapEntrySymbol_typeOperations);

static void
Arcadia_DDLS_MapEntrySymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapEntrySymbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_MapEntrySymbol);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDLS_SymbolKind_MapEntry);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->optional = Arcadia_BooleanValue_False;
  self->entryName = NULL;
  self->entrySymbol = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_MapEntrySymbol);
}

static void
Arcadia_DDLS_MapEntrySymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapEntrySymbolDispatch* self
  )
{ }

static void
Arcadia_DDLS_MapEntrySymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapEntrySymbol* self
  )
{
  if (self->entryName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entryName);
  }
  if (self->entrySymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entrySymbol);
  }
}

Arcadia_DDLS_MapEntrySymbol*
Arcadia_DDLS_MapEntrySymbol_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_MapEntrySymbol);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDLS_MapEntrySymbol);
}
