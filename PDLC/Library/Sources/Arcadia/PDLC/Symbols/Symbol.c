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
#include "Arcadia/PDLC/Symbols/Symbol.h"

static void
Arcadia_PDLC_Symbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbol* self
  );

static void
Arcadia_PDLC_Symbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolDispatch* self
  );

static void
Arcadia_PDLC_Symbol_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbol* self
  );

static void
Arcadia_PDLC_Symbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbol* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_Symbol_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_PDLC_Symbol_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_Symbol_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_Symbol_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.Symbol", Arcadia_PDLC_Symbol,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_PDLC_Symbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Symbol);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_EnumerationValue enumerationValue = Arcadia_ValueStack_getEnumerationValue(thread, 2);
  if (_Arcadia_PDLC_SymbolKind_getType(thread) != enumerationValue.type) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->kind = (Arcadia_PDLC_SymbolKind)enumerationValue.value;
  self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  self->enclosing = NULL;
  self->completer = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_PDLC_Symbol);
}

static void
Arcadia_PDLC_Symbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Symbol_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbol* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Symbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbol* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->enclosing) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enclosing);
  }
  if (self->completer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->completer);
  }
}

Arcadia_PDLC_Symbol*
Arcadia_PDLC_Symbol_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolKind kind,
    Arcadia_String* name
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Symbol);
  Arcadia_ValueStack_pushNatural32Value(thread, kind);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_PDLC_Symbol);
}
