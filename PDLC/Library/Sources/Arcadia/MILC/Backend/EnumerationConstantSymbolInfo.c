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

#include "Arcadia/MILC/Backend/EnumerationConstantSymbolInfo.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Backend/Implementation.h"
#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfoDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  );

static void
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_dumpImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  );

static Arcadia_MILC_Symbol*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getSymbolImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  );

static Arcadia_String*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getCxxNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  );

static Arcadia_String*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getCxxNameUpperCaseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
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

Arcadia_defineObjectType(u8"Arcadia.MILC.Backend.EnumerationConstantSymbolInfo", Arcadia_MILC_Backend_EnumerationConstantSymbolInfo,
                         u8"Arcadia.MILC.Backend.SymbolInfo", Arcadia_MILC_Backend_SymbolInfo,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Backend_EnumerationConstantSymbolInfo);
  {
    Arcadia_Value symbol = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_Value context = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &symbol);
    Arcadia_ValueStack_pushValue(thread, &context);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_MILC_Context* context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_Context_getType(thread));
  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);
 
  self->symbol = (Arcadia_MILC_Symbol*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Symbol_getType(thread));

  self->cxxName = Arcadia_MILC_Backend_Implementation_computeCxxName(thread, implementation, self->symbol->name);
  self->cxxNameUpperCase = Arcadia_MILC_Backend_Implementation_computeCxxNameUpperCase(thread, implementation, self->symbol->name);

  Arcadia_LeaveConstructor(Arcadia_MILC_Backend_EnumerationConstantSymbolInfo);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfoDispatch* self
  )
{
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->dump = (void (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*, Arcadia_SizeValue, Arcadia_StringBuilder*)) & Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_dumpImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getSymbol = (Arcadia_MILC_Symbol * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) & Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getSymbolImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) & Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getCxxNameImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxNameUpperCase = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) & Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getCxxNameUpperCaseImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  )
{
  if (self->symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbol);
  }
  
  if (self->cxxName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxName);
  }
  if (self->cxxNameUpperCase) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxNameUpperCase);
  }
}

static void
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_dumpImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  )
{
  Arcadia_MILC_Symbol* symbol = self->symbol;
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackString(thread, target, Arcadia_MILC_SymbolKind_toString(thread, symbol->kind));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ");
  Arcadia_StringBuilder_insertBackString(thread, target, ((Arcadia_MILC_Symbol*)symbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxName = ");
  Arcadia_StringBuilder_insertBackString(thread, target, self->cxxName);
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
}

static Arcadia_MILC_Symbol*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getSymbolImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  )
{ return self->symbol; }

static Arcadia_String*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getCxxNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  )
{ return self->cxxName; }

static Arcadia_String*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getCxxNameUpperCaseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* self
  )
{ return self->cxxNameUpperCase; }

Arcadia_MILC_Backend_EnumerationConstantSymbolInfo*
Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Backend_EnumerationConstantSymbolInfo);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (symbol) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)symbol); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_MILC_Backend_EnumerationConstantSymbolInfo);
}
