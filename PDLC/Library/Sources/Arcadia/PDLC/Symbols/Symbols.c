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

#include "Arcadia/PDLC/Symbols/Symbols.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Environment.h"
#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolsDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
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

Arcadia_defineObjectType(u8"Arcadia.PDLC.Symbols", Arcadia_PDLC_Symbols,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

// The built-in types, and the single source of truth for which types are built-in.
//
// The fully qualified type name is the only intrinsic property a built-in type needs:
// `Arcadia_PDLC_Backend_Implementation_computeCxxName` derives the C++ name from it, and a
// built-in type has no module, hence no generated source or header file of its own. `isClass`
// distinguishes `Arcadia.Object`, the one built-in that is a class type rather than a primitive
// type.
//
// Adding a built-in type means adding a row here and nothing else: the constructor builds the
// registry from this table, the enter phase registers what the registry holds, and
// `Arcadia_PDLC_Symbols_isBuiltIn` asks the registry. None of those can drift apart.
static const struct {
  const char* name;
  Arcadia_BooleanValue isClass;
} BuiltInTypes[] = {
  { u8"Arcadia.Boolean",  Arcadia_BooleanValue_False },
  { u8"Arcadia.Integer8",  Arcadia_BooleanValue_False },
  { u8"Arcadia.Integer16", Arcadia_BooleanValue_False },
  { u8"Arcadia.Integer32", Arcadia_BooleanValue_False },
  { u8"Arcadia.Integer64", Arcadia_BooleanValue_False },
  { u8"Arcadia.Natural8",  Arcadia_BooleanValue_False },
  { u8"Arcadia.Natural16", Arcadia_BooleanValue_False },
  { u8"Arcadia.Natural32", Arcadia_BooleanValue_False },
  { u8"Arcadia.Natural64", Arcadia_BooleanValue_False },
  { u8"Arcadia.Object",    Arcadia_BooleanValue_True  },
  { u8"Arcadia.Real32",    Arcadia_BooleanValue_False },
  { u8"Arcadia.Real64",    Arcadia_BooleanValue_False },
  { u8"Arcadia.Void",      Arcadia_BooleanValue_False },
};

#define BuiltInTypesSize (sizeof(BuiltInTypes) / sizeof(BuiltInTypes[0]))

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Symbols);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->context = (Arcadia_PDLC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_Context_getType(thread));

  self->builtIns = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  for (Arcadia_SizeValue i = 0; i < BuiltInTypesSize; ++i) {
    Arcadia_String* typeName = Arcadia_String_createFromCxxString(thread, BuiltInTypes[i].name);
    Arcadia_PDLC_Symbol* typeSymbol = BuiltInTypes[i].isClass
      ? (Arcadia_PDLC_Symbol*)Arcadia_PDLC_ClassSymbol_create(thread, typeName)
      : (Arcadia_PDLC_Symbol*)Arcadia_PDLC_PrimitiveTypeSymbol_create(thread, typeName);
    Arcadia_Map_set
      (
        thread,
        self->builtIns,
        Arcadia_Value_makeObjectReferenceValue(typeName),
        Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)typeSymbol),
        NULL,
        NULL
      );
  }

  Arcadia_LeaveConstructor(Arcadia_PDLC_Symbols);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolsDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }

  if (self->builtIns) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->builtIns);
  }
}

Arcadia_PDLC_Symbols*
Arcadia_PDLC_Symbols_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Symbols);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_Symbols);
}

Arcadia_PDLC_Symbols*
Arcadia_PDLC_Symbols_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_PDLC_Symbols_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_PDLC_Symbols_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_PDLC_Symbols*)Arcadia_Value_getObjectReferenceValue(&v);
}

Arcadia_PDLC_Environment*
Arcadia_PDLC_Symbols_getEnvironment
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    Arcadia_PDLC_Symbol* symbol
  )
{
  while (symbol && !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_PDLC_ClassSymbol_getType(thread))) {
    symbol = symbol->enclosing;
  }
  if (!symbol) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_PDLC_Environment* environment = (Arcadia_PDLC_Environment*)Arcadia_Map_getObjectReferenceValueChecked(thread, self->context->environments, Arcadia_Value_makeObjectReferenceValue(symbol), _Arcadia_PDLC_Environment_getType(thread));
  return environment;
}

Arcadia_PDLC_Symbol*
Arcadia_PDLC_Symbols_lookupBuiltin
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    const char* name
  )
{
  Arcadia_String* typeName = Arcadia_String_createFromCxxString(thread, name);
  Arcadia_Value value = Arcadia_Map_get
    (
      thread,
      self->builtIns,
      Arcadia_Value_makeObjectReferenceValue(typeName)
    );
  if (Arcadia_Value_isVoidValue(&value)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_PDLC_Symbol*)Arcadia_Value_getObjectReferenceValueChecked
    (
      thread, value, _Arcadia_PDLC_Symbol_getType(thread)
    );
}

Arcadia_List*
Arcadia_PDLC_Symbols_getBuiltIns
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self
  )
{
  return Arcadia_Map_getValues(thread, self->builtIns);
}

Arcadia_BooleanValue
Arcadia_PDLC_Symbols_isBuiltIn
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Symbols* self,
    Arcadia_PDLC_Symbol* symbol
  )
{
  // A symbol is built-in if, and only if, it is the very symbol the built-in registry holds under
  // its name. Comparing the symbol as well as the name keeps a program-declared type that happens
  // to shadow a built-in type name from being misreported as built-in.
  Arcadia_Value value = Arcadia_Map_get
    (
      thread,
      self->builtIns,
      Arcadia_Value_makeObjectReferenceValue(symbol->name)
    );
  if (Arcadia_Value_isVoidValue(&value)) {
    return Arcadia_BooleanValue_False;
  }
  return (Arcadia_PDLC_Symbol*)Arcadia_Value_getObjectReferenceValue(&value) == symbol
    ? Arcadia_BooleanValue_True
    : Arcadia_BooleanValue_False;
}
