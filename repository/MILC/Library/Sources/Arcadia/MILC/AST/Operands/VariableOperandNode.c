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

#include "Arcadia/MILC/AST/Operands/VariableOperandNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(value : Arcadia.String)
/// @endcode
static void
Arcadia_MILC_AST_VariableOperandNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNode* self
  );

static void
Arcadia_MILC_AST_VariableOperandNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNodeDispatch* self
  );

static void
Arcadia_MILC_AST_VariableOperandNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_VariableOperandNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_VariableOperandNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_VariableOperandNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.VariableOperandNode", Arcadia_MILC_AST_VariableOperandNode,
                         u8"Arcadia.MILC.AST.OperandNode", Arcadia_MILC_AST_OperandNode,
                         &_typeOperations);

void
Arcadia_MILC_AST_VariableOperandNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_VariableOperandNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->value = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_VariableOperandNode);
}

static void
Arcadia_MILC_AST_VariableOperandNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_VariableOperandNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_VariableOperandNode* self
  )
{ Arcadia_Object_visit(thread, (Arcadia_Object*)self->value); }

Arcadia_MILC_AST_VariableOperandNode*
Arcadia_MILC_AST_VariableOperandNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* value
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_VariableOperandNode);
  if (value) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, value);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_AST_VariableOperandNode);
}
