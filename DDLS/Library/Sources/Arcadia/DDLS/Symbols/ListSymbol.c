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

#include "Arcadia/DDLS/Symbols/ListSymbol.h"

#include "Arcadia/DDLS/Symbols/AnySymbol.h"

static void
Arcadia_DDLS_ListSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListSymbol* self
  );

static void
Arcadia_DDLS_ListSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListSymbolDispatch* self
  );

static void
Arcadia_DDLS_ListSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListSymbol* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_ListSymbol_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_ListSymbol_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_ListSymbol_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_ListSymbol_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_ListSymbol_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_ListSymbol_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.ListSymbol", Arcadia_DDLS_ListSymbol,
                         u8"Arcadia.DDLS.Symbol", Arcadia_DDLS_Symbol,
                         &_Arcadia_DDLS_ListSymbol_typeOperations);

static void
Arcadia_DDLS_ListSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListSymbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_ListSymbol);
  //
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDLS_SymbolKind_List);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->entry = (Arcadia_DDLS_Symbol*)Arcadia_DDLS_AnySymbol_create(thread);
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_ListSymbol);
}

static void
Arcadia_DDLS_ListSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListSymbolDispatch* self
  )
{ }

static void
Arcadia_DDLS_ListSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListSymbol* self
  )
{
  if (self->entry) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entry);
  }
}

Arcadia_DDLS_ListSymbol*
Arcadia_DDLS_ListSymbol_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_ListSymbol);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDLS_ListSymbol);
}
