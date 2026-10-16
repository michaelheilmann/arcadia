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

#include "Arcadia/VPL/Include.h"

static Arcadia_Object*
root
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, object);
  return object;
}

static Arcadia_String*
string
  (
    Arcadia_Thread* thread,
    const char* text
  )
{
  return (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, text));
}

static Arcadia_List*
list
  (
    Arcadia_Thread* thread
  )
{
  return (Arcadia_List*)root(thread, (Arcadia_Object*)Arcadia_ArrayList_create(thread));
}

static Arcadia_VPL_Tree_Node*
name
  (
    Arcadia_Thread* thread,
    const char* text
  )
{
  return (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeNameExpr(thread, string(thread, text)));
}

static Arcadia_VPL_Tree_Node*
variable
  (
    Arcadia_Thread* thread,
    const char* text
  )
{
  return (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_VariableDefnNode_create(thread, string(thread, text), string(thread, "vec4")));
}

static void
append
  (
    Arcadia_Thread* thread,
    Arcadia_List* body,
    Arcadia_VPL_Tree_Node* node
  )
{
  root(thread, (Arcadia_Object*)node);
  Arcadia_List_insertBackObjectReferenceValue(thread, body, (Arcadia_Object*)node);
}

static Arcadia_VPL_Symbols_Program*
program
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_Symbols_Program* result = (Arcadia_VPL_Symbols_Program*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Symbols_Program_create(thread, Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor));
  result->context = Arcadia_VPL_Context_create(thread);
  result->programDefnNode = Arcadia_VPL_Tree_ProgramDefnNode_create(thread);
  return result;
}

static Arcadia_VPL_Tree_ProcedureDefnNode*
procedure
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program,
    const char* procedureName
  )
{
  Arcadia_VPL_Tree_ProcedureDefnNode* result = (Arcadia_VPL_Tree_ProcedureDefnNode*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeProcedureDefn(thread, list(thread),
      (Arcadia_VPL_Tree_NameExprNode*)name(thread, "void"), string(thread, procedureName), list(thread), list(thread)));
  Arcadia_List_insertBackObjectReferenceValue(thread, result->modifiers, (Arcadia_Object*)string(thread, "fragment"));
  append(thread, program->programDefnNode->elements, (Arcadia_VPL_Tree_Node*)result);
  return result;
}

static void
run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_EnterPhase_run(thread, Arcadia_VPL_Context_getEnterPhase(thread, program->context), program);
  Arcadia_VPL_ResolvePhase_run(thread, Arcadia_VPL_Context_getResolvePhase(thread, program->context), program);
  Arcadia_Tests_assertTrue(thread, stackSize == Arcadia_ValueStack_getSize(thread));
}

static void
assertBinding
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* expression,
    Arcadia_VPL_Tree_Node* declaration
  )
{
  Arcadia_VPL_Symbols_Symbol* symbol = ((Arcadia_VPL_Tree_NameExprNode*)expression)->symbol;
  Arcadia_Tests_assertTrue(thread, symbol && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Variable_getType(thread)));
  Arcadia_Tests_assertTrue(thread, declaration == ((Arcadia_VPL_Symbols_Variable*)symbol)->node);
}

void
Arcadia_VPL_Tests_Tree_testResolution
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Symbols_Program* p = program(thread);
  Arcadia_VPL_Tree_ProcedureDefnNode* proc = procedure(thread, p, "main");
  Arcadia_VPL_Tree_Node* parameter = variable(thread, "input");
  append(thread, proc->parameters, parameter);
  Arcadia_VPL_Tree_Node* outer = variable(thread, "value");
  append(thread, proc->body, outer);

  Arcadia_List* thenBody = list(thread);
  Arcadia_List* elseBody = list(thread);
  Arcadia_VPL_Tree_Node* thenVariable = variable(thread, "value");
  Arcadia_VPL_Tree_Node* elseVariable = variable(thread, "value");
  append(thread, thenBody, thenVariable);
  append(thread, elseBody, elseVariable);
  Arcadia_VPL_Tree_Node* thenUse = name(thread, "value");
  Arcadia_VPL_Tree_Node* indexUse = name(thread, "input");
  Arcadia_VPL_Tree_Node* indexed = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeIndexExpr(thread, thenUse, indexUse));
  append(thread, thenBody, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, indexed));
  Arcadia_VPL_Tree_Node* elseUse = name(thread, "value");
  append(thread, elseBody, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, elseUse));
  append(thread, elseBody, Arcadia_VPL_Tree_NodeFactory_makeDiscardStmt(thread));
  Arcadia_VPL_Tree_Node* ifCondition = name(thread, "input");
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, ifCondition, thenBody, elseBody));

  Arcadia_List* whileBody = list(thread);
  Arcadia_VPL_Tree_Node* whileVariable = variable(thread, "value");
  append(thread, whileBody, whileVariable);
  Arcadia_VPL_Tree_Node* whileUse = name(thread, "value");
  append(thread, whileBody, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, whileUse));
  Arcadia_VPL_Tree_Node* whileCondition = name(thread, "value");
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, whileCondition, whileBody));

  Arcadia_VPL_Tree_Node* loopVariable = variable(thread, "i");
  Arcadia_VPL_Tree_Node* loopCondition = name(thread, "i");
  Arcadia_VPL_Tree_Node* updateTarget = name(thread, "value");
  Arcadia_VPL_Tree_Node* updateIndex = name(thread, "i");
  Arcadia_VPL_Tree_Node* updateRhs = name(thread, "input");
  Arcadia_VPL_Tree_Node* updateLhs = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeIndexExpr(thread, updateTarget, updateIndex));
  Arcadia_VPL_Tree_Node* update = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, updateLhs, updateRhs));
  Arcadia_List* loopBody = list(thread);
  Arcadia_VPL_Tree_Node* bodyVariable = variable(thread, "i");
  append(thread, loopBody, bodyVariable);
  Arcadia_VPL_Tree_Node* bodyUse = name(thread, "i");
  // Nest a while loop within the for body to exercise scope chains beyond one level.
  Arcadia_List* nestedBody = list(thread);
  append(thread, nestedBody, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, bodyUse));
  append(thread, loopBody, Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, name(thread, "input"), nestedBody));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, loopVariable, loopCondition, update, loopBody));
  Arcadia_VPL_Tree_Node* afterUse = name(thread, "value");
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, afterUse));

  // Local/parameter member access must use the variable's type scope, not the lexical scope.
  Arcadia_VPL_Tree_Node* localMemberBase = name(thread, "value");
  Arcadia_VPL_Tree_Node* localMember = name(thread, "xyz");
  Arcadia_VPL_Tree_Node* access = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeAccessExpr(thread, localMemberBase, localMember));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, access, name(thread, "input")));
  Arcadia_List* arguments = list(thread);
  Arcadia_VPL_Tree_Node* argument = name(thread, "value");
  append(thread, arguments, argument);
  Arcadia_VPL_Tree_Node* callee = name(thread, "vec4");
  Arcadia_VPL_Tree_Node* call = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeCallExpr(thread, callee));
  ((Arcadia_VPL_Tree_CallExprNode*)call)->arguments = arguments;
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, call));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, NULL, NULL, NULL, list(thread)));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, name(thread, "value"), NULL, NULL, list(thread)));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, name(thread, "input"), list(thread), NULL));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, NULL));

  run(thread, p);
  assertBinding(thread, ifCondition, parameter);
  assertBinding(thread, thenUse, thenVariable);
  assertBinding(thread, indexUse, parameter);
  assertBinding(thread, elseUse, elseVariable);
  assertBinding(thread, whileCondition, outer);
  assertBinding(thread, whileUse, whileVariable);
  assertBinding(thread, loopCondition, loopVariable);
  assertBinding(thread, updateTarget, outer);
  assertBinding(thread, updateIndex, loopVariable);
  assertBinding(thread, updateRhs, parameter);
  assertBinding(thread, bodyUse, bodyVariable);
  assertBinding(thread, afterUse, outer);
  assertBinding(thread, localMemberBase, outer);
  assertBinding(thread, argument, outer);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_VPL_Tree_NameExprNode*)callee)->symbol == (Arcadia_VPL_Symbols_Symbol*)p->VEC4);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_VPL_Tree_NameExprNode*)localMember)->symbol ==
    (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, p->VEC4->scope, string(thread, "xyz"), Arcadia_BooleanValue_False));
  Arcadia_Tests_assertTrue(thread, ((Arcadia_VPL_Tree_VariableDefnNode*)loopVariable)->typeSymbol == (Arcadia_VPL_Symbols_Symbol*)p->VEC4);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* p,
    Arcadia_BooleanValue duplicate
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    run(thread, p);
  } else {
    raised = Arcadia_BooleanValue_True;
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_setStatus(thread, 0);
  Arcadia_Tests_assertTrue(thread, raised && Arcadia_Status_SemanticalError == status);
  Arcadia_Tests_assertTrue(thread, stackSize == Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, duplicate == Arcadia_Languages_Diagnostics_hasErrors(thread, Arcadia_VPL_Context_getDiagnostics(thread, p->context)));
}

void
Arcadia_VPL_Tests_Tree_testResolutionFailures
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 16; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_VPL_Symbols_Program* p = program(thread);
    Arcadia_VPL_Tree_ProcedureDefnNode* proc = procedure(thread, p, "main");
    append(thread, proc->parameters, variable(thread, "input"));
    Arcadia_List* body = list(thread);
    Arcadia_List* otherBody = list(thread);
    Arcadia_VPL_Tree_Node* input = name(thread, "input");
    Arcadia_VPL_Tree_Node* missing = name(thread, "missing");
    Arcadia_BooleanValue duplicate = Arcadia_BooleanValue_False;
    switch (test) {
      case 0: // Undefined condition.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, missing, body, NULL));
        break;
      case 1: // Then declarations are not visible in else.
        append(thread, body, variable(thread, "missing"));
        append(thread, otherBody, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, missing));
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, input, body, otherBody));
        break;
      case 2: // While body declarations do not escape.
        append(thread, body, variable(thread, "missing"));
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, input, body));
        append(thread, proc->body, missing);
        break;
      case 3: // For initializer declarations do not escape.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, variable(thread, "missing"), NULL, NULL, body));
        append(thread, proc->body, missing);
        break;
      case 4: // Body declarations are not visible in update.
        append(thread, body, variable(thread, "missing"));
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, NULL, NULL, missing, body));
        break;
      case 5: // Body declarations are not visible in condition.
        append(thread, body, variable(thread, "missing"));
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, NULL, missing, NULL, body));
        break;
      case 6: // Unresolved index target.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeIndexExpr(thread, missing, input));
        break;
      case 7: // Unresolved index operand on an assignment LHS.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread,
          (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeIndexExpr(thread, input, missing)), input));
        break;
      case 8: // Use before declaration.
        append(thread, body, missing);
        append(thread, body, variable(thread, "missing"));
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, input, body, NULL));
        break;
      case 9: // Duplicate local declaration.
        duplicate = Arcadia_BooleanValue_True;
        append(thread, body, variable(thread, "value"));
        append(thread, body, variable(thread, "value"));
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, input, body));
        break;
      case 10: // Duplicate parameter declaration.
        duplicate = Arcadia_BooleanValue_True;
        append(thread, proc->parameters, variable(thread, "input"));
        break;
      case 11: // Duplicate procedure declaration.
        duplicate = Arcadia_BooleanValue_True;
        procedure(thread, p, "main");
        break;
      case 12: // Return values must be resolved.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, missing));
        break;
      case 13: // Undefined while condition.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, missing, body));
        break;
      case 14: // Expression initializer must be resolved.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, missing, NULL, NULL, body));
        break;
      case 15: // A statement cannot occupy an expression initializer slot.
        append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread,
          (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeDiscardStmt(thread)), NULL, NULL, body));
        break;
    }
    expectFailure(thread, p, duplicate);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}
