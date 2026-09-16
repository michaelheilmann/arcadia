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
#include "Arcadia/MILC/MemberEnterPhase/ProcedureCompleter.h"

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/Diagnostics/Include.h"
#include "Arcadia/MILC/Symbols/Include.h"
#include "Arcadia/MILC/TypeResolutionPhase.h"
#include <assert.h>

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self
  );

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self
  );

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleterDispatch* self
  );

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self
  );

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.MemberEnterPhase.ProcedureCompleter", Arcadia_MILC_MemberEnterPhase_ProcedureCompleter,
                         u8"Arcadia.MILC.Completer", Arcadia_MILC_Completer,
                         &_typeOperations);

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_MemberEnterPhase_ProcedureCompleter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_MemberEnterPhase_ProcedureCompleter);
}

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleterDispatch* self
  )
{
  ((Arcadia_MILC_CompleterDispatch*)self)->complete = (void (*)(Arcadia_Thread*, Arcadia_MILC_Completer*, Arcadia_MILC_Context*, Arcadia_MILC_Symbol*)) & completeImpl;
}

static void
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self
  )
{/*Intentionally empty.*/}

static void
completeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase_ProcedureCompleter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{ 
  assert(symbol->completer == (Arcadia_MILC_Completer*)self);
  Arcadia_MILC_AST_ProcedureDefinitionNode* node = ((Arcadia_MILC_ProcedureSymbol*)symbol)->ast;
  if (!node) {
    symbol->completer = NULL;
    return;
  }
  symbol->completer = Arcadia_MILC_TypeResolutionPhase_getInstance(thread, context)->procedureCompleter;
}

Arcadia_MILC_MemberEnterPhase_ProcedureCompleter*
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_MemberEnterPhase_ProcedureCompleter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_MemberEnterPhase_ProcedureCompleter);
}
