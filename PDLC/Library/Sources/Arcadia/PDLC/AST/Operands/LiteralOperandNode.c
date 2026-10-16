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

#include "Arcadia/PDLC/AST/Operands/LiteralOperandNode.h"

#include "Arcadia/PDLC/AST/Include.h"

static void
Arcadia_PDLC_AST_LiteralOperandNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LiteralOperandNode* self
  );

static void
Arcadia_PDLC_AST_LiteralOperandNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LiteralOperandNodeDispatch* self
  );

static void
Arcadia_PDLC_AST_LiteralOperandNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LiteralOperandNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_PDLC_AST_LiteralOperandNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_AST_LiteralOperandNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_AST_LiteralOperandNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_AST_LiteralOperandNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_PDLC_AST_LiteralOperandNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_PDLC_AST_LiteralOperandNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.LiteralOperandNode", Arcadia_PDLC_AST_LiteralOperandNode,
                         u8"Arcadia.PDLC.AST.OperandNode", Arcadia_PDLC_AST_OperandNode,
                         &_Arcadia_PDLC_AST_LiteralOperandNode_typeOperations);

static void
Arcadia_PDLC_AST_LiteralOperandNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LiteralOperandNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_LiteralOperandNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value startOffset = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &startOffset);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_Object* argument = Arcadia_ValueStack_getObjectReferenceValue(thread, 1);
  if (Arcadia_Object_isInstanceOf(thread, argument, _Arcadia_PDLC_AST_BooleanLiteralNode_getType(thread)) ||
      Arcadia_Object_isInstanceOf(thread, argument, _Arcadia_PDLC_AST_IntegerLiteralNode_getType(thread)) ||
      Arcadia_Object_isInstanceOf(thread, argument, _Arcadia_PDLC_AST_RealLiteralNode_getType(thread))    ||
      Arcadia_Object_isInstanceOf(thread, argument, _Arcadia_PDLC_AST_StringLiteralNode_getType(thread))  ||
      Arcadia_Object_isInstanceOf(thread, argument, _Arcadia_PDLC_AST_VoidLiteralNode_getType(thread))) {
    self->literal = (Arcadia_PDLC_AST_Node*)argument;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_LiteralOperandNode);
}

static void
Arcadia_PDLC_AST_LiteralOperandNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LiteralOperandNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_AST_LiteralOperandNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LiteralOperandNode* self
  )
{ Arcadia_Object_visit(thread, (Arcadia_Object*)self->literal); }

Arcadia_PDLC_AST_LiteralOperandNode*
Arcadia_PDLC_AST_LiteralOperandNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_Node* literal
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_AST_LiteralOperandNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (literal) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, literal);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_PDLC_AST_LiteralOperandNode);
}
