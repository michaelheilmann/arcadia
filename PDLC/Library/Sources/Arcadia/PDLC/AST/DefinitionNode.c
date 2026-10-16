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

#include "Arcadia/PDLC/AST/DefinitionNode.h"

#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// constructor()
/// @endcode
static void
Arcadia_PDLC_AST_DefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_DefinitionNode* self
  );

static void
Arcadia_PDLC_AST_DefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_DefinitionNodeDispatch* self
  );

static void
Arcadia_PDLC_AST_DefinitionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_DefinitionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_PDLC_AST_DefinitionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_AST_DefinitionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_AST_DefinitionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_AST_DefinitionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_PDLC_AST_DefinitionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_PDLC_AST_DefinitionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.DefinitionNode", Arcadia_PDLC_AST_DefinitionNode,
                         u8"Arcadia.PDLC.AST.Node", Arcadia_PDLC_AST_Node,
                         &_Arcadia_PDLC_AST_DefinitionNode_typeOperations);

static void
Arcadia_PDLC_AST_DefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_DefinitionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_DefinitionNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->startOffset = Arcadia_ValueStack_getSizeValue(thread, 1);
  self->compilationUnitNode = NULL;
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_DefinitionNode);
}

static void
Arcadia_PDLC_AST_DefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_DefinitionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_AST_DefinitionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_DefinitionNode* self
  )
{
  if (self->compilationUnitNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->compilationUnitNode);
  }
}
