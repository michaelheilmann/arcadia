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

#include "Arcadia/MILC/Symbols/Symbols.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/Environment.h"
#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_SymbolsDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self
  );

static const Arcadia_ObjectType_Operations _objectTypesOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypesOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.Symbols", Arcadia_MILC_Symbols,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Symbols);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Context_getType(thread));

  self->integer16Symbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Integer16"));
  self->integer32Symbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Integer32"));
  self->integer64Symbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Integer64"));
  self->integer8Symbol  = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Integer8"));

  self->natural16Symbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Natural16"));
  self->natural32Symbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Natural32"));
  self->natural64Symbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Natural64"));
  self->natural8Symbol  = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Natural8"));

  self->booleanSymbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Boolean"));

  self->objectSymbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_ClassSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Object"));

  self->voidSymbol = (Arcadia_MILC_Symbol*)Arcadia_MILC_PrimitiveTypeSymbol_create(thread, Arcadia_String_createFromCxxString(thread, u8"Arcadia.Void"));

  Arcadia_LeaveConstructor(Arcadia_MILC_Symbols);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_SymbolsDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }

  if (self->integer16Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->integer16Symbol);
  }
  if  (self->integer32Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->integer32Symbol);
  }
  if (self->integer64Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->integer64Symbol);
  }
  if (self->integer8Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->integer8Symbol);
  }
  
  if (self->natural16Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->natural16Symbol);
  }
  if (self->natural32Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->natural32Symbol);
  }
  if (self->natural64Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->natural64Symbol);
  }
  if (self->natural8Symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->natural8Symbol);
  }

  if (self->booleanSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->booleanSymbol);
  }

  if (self->objectSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->objectSymbol);
  }

  if (self->voidSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->voidSymbol);
  }
}

Arcadia_MILC_Symbols*
Arcadia_MILC_Symbols_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{ 
  _Arcadia_BeginCreate(Arcadia_MILC_Symbols);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_Symbols);
}

Arcadia_MILC_Symbols*
Arcadia_MILC_Symbols_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{ 
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_MILC_Symbols_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);  
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_Symbols_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_MILC_Symbols*)Arcadia_Value_getObjectReferenceValue(&v);
}

Arcadia_MILC_Environment*
Arcadia_MILC_Symbols_getEnvironment
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self,
    Arcadia_MILC_Symbol* symbol
  )
{
  while (symbol && !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_MILC_ClassSymbol_getType(thread))) {
    symbol = symbol->enclosing;
  }
  if (!symbol) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_MILC_Environment* environment = (Arcadia_MILC_Environment*)Arcadia_Map_getObjectReferenceValueChecked(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(symbol), _Arcadia_MILC_Environment_getType(thread));
  return environment;
}

Arcadia_BooleanValue
Arcadia_MILC_Symbols_isBuiltIn
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbols* self,
    Arcadia_MILC_Symbol* symbol
  )
{
  Arcadia_BooleanValue isBuiltIn = Arcadia_BooleanValue_False;

  isBuiltIn |= symbol == self->booleanSymbol;

  isBuiltIn |= symbol == self->integer16Symbol;
  isBuiltIn |= symbol == self->integer32Symbol;
  isBuiltIn |= symbol == self->integer64Symbol;
  isBuiltIn |= symbol == self->integer8Symbol;

  isBuiltIn |= symbol == self->natural16Symbol;
  isBuiltIn |= symbol == self->natural32Symbol;
  isBuiltIn |= symbol == self->natural64Symbol;
  isBuiltIn |= symbol == self->natural8Symbol;

  isBuiltIn |= symbol == self->objectSymbol;

  isBuiltIn |= symbol == self->voidSymbol;

  return isBuiltIn;
}
