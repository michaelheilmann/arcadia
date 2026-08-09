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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/ResolvePhase.h"

#include "Arcadia/VPL/Symbols/Include.h"
#include "Arcadia/VPL/Tree/Include.h"
#include "Arcadia/VPL/ModifiersUtilities.h"

static void
Arcadia_VPL_ResolvePhase_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self
  );

static void
Arcadia_VPL_ResolvePhase_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhaseDispatch* self
  );

static void
Arcadia_VPL_ResolvePhase_visit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self
  );

static void
Arcadia_VPL_ResolvePhase_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self
  );

static void
conflictingDefinitionError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_ResolvePhase_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_VPL_ResolvePhase_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_ResolvePhase_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_ResolvePhase_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.ResolvePhase", Arcadia_VPL_ResolvePhase,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_ResolvePhase_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_ResolvePhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_VPL_ResolvePhase);
}

static void
Arcadia_VPL_ResolvePhase_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhaseDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_VPL_ResolvePhase_visit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_VPL_ResolvePhase_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self
  )
{/*Intentionally empty.*/}

Arcadia_VPL_ResolvePhase*
Arcadia_VPL_ResolvePhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_ResolvePhase);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_ResolvePhase);
}

static void
conflictingDefinitionError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  /*@todo Add and use exceptions. The concept is already in Arcadia.Ring2 and Arcadia.Languages but it was not yet used so far.*/
  Arcadia_logf(Arcadia_LogFlags_Error, "conflicting definition\n");
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

// we expect a variable
#define ResolveFlags_Variable (1 << 0)

// we expect a type
#define ResolveFlags_Type (1 << 2)

#define ResolveFlags_ConstantRecord (1 << 3)

#define ResolveFlags_ConstantScalar (1 << 4)

#define ResolveFlags_VariableScalar (1 << 5)

#define ResolveFlags_Procedure (1 << 6)

typedef uint8_t ResolveFlags;

static void
onResolveName
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    ResolveFlags expectedResolveFlags,
    Arcadia_VPL_Tree_NameExprNode* nameExprNode
  )
{
  Arcadia_Object* symbol = Arcadia_Languages_Scope_lookup(thread, scope, nameExprNode->name, Arcadia_BooleanValue_True);
  if (!symbol) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
  if ((expectedResolveFlags & ResolveFlags_Variable) && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Variable_getType(thread))) {
    nameExprNode->symbol = (Arcadia_VPL_Symbols_Symbol*)symbol;
  } else if ((expectedResolveFlags & ResolveFlags_Type) && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
    nameExprNode->symbol = (Arcadia_VPL_Symbols_Symbol*)symbol;
  } else if ((expectedResolveFlags & ResolveFlags_ConstantRecord) && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_ConstantRecord_getType(thread))) {
    nameExprNode->symbol = (Arcadia_VPL_Symbols_Symbol*)symbol;
  } else if ((expectedResolveFlags & ResolveFlags_ConstantScalar) && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_ConstantScalar_getType(thread))) {
    nameExprNode->symbol = (Arcadia_VPL_Symbols_Symbol*)symbol;
  } else if ((expectedResolveFlags & ResolveFlags_VariableScalar) && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_VariableScalar_getType(thread))) {
    nameExprNode->symbol = (Arcadia_VPL_Symbols_Symbol*)symbol;
  } else if ((expectedResolveFlags & ResolveFlags_Procedure) && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
    nameExprNode->symbol = (Arcadia_VPL_Symbols_Symbol*)symbol;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_BooleanValue
isNameExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* node
  )
{
  return Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_NameExprNode_getType(thread));
}

static Arcadia_BooleanValue
isAccessExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* node
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_BinaryExprNode_getType(thread))) {
    Arcadia_VPL_Tree_BinaryExprNode* binaryExprNode = (Arcadia_VPL_Tree_BinaryExprNode*)node;
    return binaryExprNode->kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Access;
  }
  return Arcadia_BooleanValue_False;
}

static void
onAccessExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    Arcadia_VPL_Tree_BinaryExprNode* accessExprNode
  );

static void
onExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    Arcadia_VPL_Tree_Node* exprNode
  );

static void
onStatNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    Arcadia_VPL_Tree_Node* statNode
  );

static void
onProcedure
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure
  );

static void
onStage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Stage* stage
  );

static void
onProgram
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Program* program
  );

// TODO: This currently only covers the case of `<name>.<name>`.
static void
onAccessExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    Arcadia_VPL_Tree_BinaryExprNode* accessExprNode
  )
{
  Arcadia_VPL_Tree_Node* lhs = accessExprNode->lhs;
  Arcadia_VPL_Tree_Node* rhs = accessExprNode->rhs;
  while (1) {
    // If the LHS is a name expr, resolve it.
    if (isNameExprNode(thread, lhs)) {
      onResolveName(thread, self, procedure, scope, ResolveFlags_Variable | ResolveFlags_ConstantRecord | ResolveFlags_ConstantScalar | ResolveFlags_VariableScalar, (Arcadia_VPL_Tree_NameExprNode*)lhs);
      if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)((Arcadia_VPL_Tree_NameExprNode*)lhs)->symbol, _Arcadia_VPL_Symbols_ConstantRecord_getType(thread))) {
        // The constant record's scope is the scope under which to evaluate the RHS.
        scope = ((Arcadia_VPL_Symbols_ConstantRecord*)((Arcadia_VPL_Tree_NameExprNode*)lhs)->symbol)->scope;
      } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)((Arcadia_VPL_Tree_NameExprNode*)lhs)->symbol, _Arcadia_VPL_Symbols_VariableScalar_getType(thread))) {
        // The variable scalar's type's scope is the scope under which to evaluate the RHS.
        Arcadia_VPL_Symbols_VariableScalar* variableScalar = (Arcadia_VPL_Symbols_VariableScalar*)(Arcadia_Object*)((Arcadia_VPL_Tree_NameExprNode*)lhs)->symbol;
        scope = ((Arcadia_VPL_Symbols_BuiltinType*)(variableScalar->type))->scope;
      } else {
        scope = NULL;
      }
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    // If the RHS is a name expr, resolve it in the current scope (if any). If there is no current scope, then this is an error.
    if (isNameExprNode(thread, rhs)) {
      if (!scope) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      onResolveName(thread, self, procedure, scope, ResolveFlags_Variable, (Arcadia_VPL_Tree_NameExprNode*)rhs);
      break;
    } else if (isAccessExprNode(thread, rhs)) {
      // If the RHS is a name expr, then the LHS@new = RHS@old.lhs, RHS@new = RHS@old.rhs new
      if (!scope) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
        Arcadia_Thread_jump(thread);
      }
      lhs = ((Arcadia_VPL_Tree_BinaryExprNode*)rhs)->lhs;
      rhs = ((Arcadia_VPL_Tree_BinaryExprNode*)rhs)->rhs;
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
onExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    Arcadia_VPL_Tree_Node* exprNode
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)exprNode, _Arcadia_VPL_Tree_BinaryExprNode_getType(thread))) {
    Arcadia_VPL_Tree_BinaryExprNode* binaryExprNode = (Arcadia_VPL_Tree_BinaryExprNode*)exprNode;
    switch (binaryExprNode->kind) {
      case Arcadia_VPL_Tree_BinaryExprNodeFlags_Add:
      case Arcadia_VPL_Tree_BinaryExprNodeFlags_Subtract:
      case Arcadia_VPL_Tree_BinaryExprNodeFlags_Multiply:
      case Arcadia_VPL_Tree_BinaryExprNodeFlags_Divide: {
        onExprNode(thread, self, procedure, scope, binaryExprNode->lhs);
        onExprNode(thread, self, procedure, scope, binaryExprNode->rhs);
      } break;
      case Arcadia_VPL_Tree_BinaryExprNodeFlags_Assignment: {
        if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)binaryExprNode->lhs, _Arcadia_VPL_Tree_NameExprNode_getType(thread))) {
          onResolveName(thread, self, procedure, scope, ResolveFlags_Variable|ResolveFlags_ConstantScalar| ResolveFlags_VariableScalar, (Arcadia_VPL_Tree_NameExprNode*)binaryExprNode->lhs);
        }
        onExprNode(thread, self, procedure, scope, binaryExprNode->rhs);
      } break;
      case Arcadia_VPL_Tree_BinaryExprNodeFlags_Access: {
        onAccessExprNode(thread, self, procedure, scope, binaryExprNode);
      } break;
    }
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)exprNode, _Arcadia_VPL_Tree_CallExprNode_getType(thread))) {
    Arcadia_VPL_Tree_CallExprNode* callExprNode = (Arcadia_VPL_Tree_CallExprNode*)exprNode;
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)callExprNode->target, _Arcadia_VPL_Tree_NameExprNode_getType(thread))) {
      onResolveName(thread, self, procedure, scope, ResolveFlags_Procedure|ResolveFlags_Type, (Arcadia_VPL_Tree_NameExprNode*)callExprNode->target);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
      Arcadia_Thread_jump(thread);
    }
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)callExprNode->arguments); i < n; ++i) {
      /* TODO: We should have a Arcadia_VPL_Tree_ExprNode type. */
      Arcadia_VPL_Tree_Node* argumentExprNode = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, callExprNode->arguments, i, _Arcadia_VPL_Tree_Node_getType(thread));
      onExprNode(thread, self, procedure, scope, argumentExprNode);
    }
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)exprNode, _Arcadia_VPL_Tree_NameExprNode_getType(thread))) {
    Arcadia_VPL_Tree_NameExprNode* nameExprNode = (Arcadia_VPL_Tree_NameExprNode*)exprNode;
    onResolveName(thread, self, procedure, scope, ResolveFlags_Variable | ResolveFlags_VariableScalar | ResolveFlags_ConstantScalar, nameExprNode);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)exprNode, _Arcadia_VPL_Tree_NumberExprNode_getType(thread))) {
    /*
     * TODO: We can get away with not assigning a type for now.
     * The float and double built-in types as well as the built-in integer types should be added.
     * The NumberExprNode should convey the required information to determine its type.
     */
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
    Arcadia_Thread_jump(thread);
  }
}

static void
onStatNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_Languages_Scope* scope,
    Arcadia_VPL_Tree_Node* statNode
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)statNode, _Arcadia_VPL_Tree_VariableDefnNode_getType(thread))) {
    Arcadia_VPL_Tree_VariableDefnNode* variableDefnNode = (Arcadia_VPL_Tree_VariableDefnNode*)statNode;
    Arcadia_Object* typeSymbol = Arcadia_Languages_Scope_lookup(thread, scope, variableDefnNode->type, Arcadia_BooleanValue_True);
    if (!typeSymbol || !Arcadia_Object_isInstanceOf(thread, typeSymbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_LexicalError);
      Arcadia_Thread_jump(thread);
    }
    variableDefnNode->typeSymbol = (Arcadia_VPL_Symbols_Symbol*)typeSymbol;
    Arcadia_VPL_Symbols_Variable* symbol = Arcadia_VPL_Symbols_Variable_create(thread, variableDefnNode->name, (Arcadia_VPL_Symbols_Symbol*)typeSymbol, (Arcadia_VPL_Symbols_Symbol*)procedure, statNode);
    Arcadia_Languages_Scope_enter(thread, scope, symbol->name, (Arcadia_Object*)symbol);
  } else {
    onExprNode(thread, self, procedure, scope, statNode);
  }
}

static void
onProcedure
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure
  )
{
  Arcadia_VPL_Tree_ProcedureDefnNode* procedureDefnNode = (Arcadia_VPL_Tree_ProcedureDefnNode*)procedure->node;
  Arcadia_Languages_Scope* scope = Arcadia_Languages_Scope_create(thread, procedure->scope);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)procedureDefnNode->body); i < n; ++i) {
    Arcadia_VPL_Tree_Node* statementNode = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, procedureDefnNode->body, i, _Arcadia_VPL_Tree_Node_getType(thread));
    onStatNode(thread, self, procedure, scope, statementNode);
  }
}

static void
onStage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Stage* stage
  )
{
  Arcadia_List* entries = Arcadia_Map_getValues(thread, stage->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)entries); i < n; ++i) {
    Arcadia_VPL_Symbols_Symbol* symbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_List_getObjectReferenceValueAt(thread, entries, i);
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
      onProcedure(thread, self, (Arcadia_VPL_Symbols_Procedure*)symbol);
    }
  }
}

static void
onProgram
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_List* entries = Arcadia_Map_getValues(thread, program->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)entries); i < n; ++i) {
    Arcadia_VPL_Symbols_Symbol* symbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_List_getObjectReferenceValueAt(thread, entries, i);
    if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Stage_getType(thread))) {
      onStage(thread, self, (Arcadia_VPL_Symbols_Stage*)symbol);
    } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
      onProcedure(thread, self, (Arcadia_VPL_Symbols_Procedure*)symbol);
    }
  }
}

void
Arcadia_VPL_ResolvePhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_ResolvePhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  onProgram(thread, self, program);
}
