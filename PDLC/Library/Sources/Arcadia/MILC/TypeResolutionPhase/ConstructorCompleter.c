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

#define ARCADIA_MILC_PRIVATE (1)
#include "Arcadia/MILC/TypeResolutionPhase/ConstructorCompleter.h"

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Diagnostics/Include.h"
#include "Arcadia/MILC/Symbols/Include.h"
#include "Arcadia/MILC/Names.h"
#include "Arcadia/MILC/Environment.h"
#include "Arcadia/MILC/TypeResolutionPhase.h"
#include <assert.h>

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self
  );

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self
  );

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleterDispatch* self
  );

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self
  );

static void
onCompleteParameter
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_VariableSymbol* variableSymbol
  );

static void
onCompleteConstructor
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_ConstructorSymbol* symbol
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.TypeResolutionPhase.ConstructorCompleter", Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter,
                         u8"Arcadia.MILC.Completer", Arcadia_MILC_Completer,
                         &_typeOperations);

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter);
}

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleterDispatch* self
  )
{
  ((Arcadia_MILC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_MILC_Completer*, Arcadia_MILC_Context*, Arcadia_MILC_Symbol*)) & completeImpl;
}

static void
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self
  )
{/*Intentionally empty.*/}

static void
onCompleteParameter
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_VariableSymbol* variableSymbol
  )
{ 
  Arcadia_MILC_TypeResolutionPhase_resolveParameter(thread, Arcadia_MILC_TypeResolutionPhase_getInstance(thread, context), variableSymbol);
}

static void
onCompleteConstructor
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_ConstructorSymbol* symbol
  )
{ 
  // Complete parameter types.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbol->parameters); i < n; ++i) {
    Arcadia_MILC_VariableSymbol* parameter = (Arcadia_MILC_VariableSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbol->parameters, i, _Arcadia_MILC_VariableSymbol_getType(thread));
    onCompleteParameter(thread, self, context, parameter);
  }
}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{ 
  assert(symbol->completer == (Arcadia_MILC_Completer*)self);
  onCompleteConstructor(thread, self, context, (Arcadia_MILC_ConstructorSymbol*)symbol);
  symbol->completer = NULL;
}

Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter*
Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter_create
  (
    Arcadia_Thread* thread
  )
{ 
  _Arcadia_BeginCreate(Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_TypeResolutionPhase_ConstructorCompleter);
}
