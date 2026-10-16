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

void
Arcadia_VPL_Tests_Tree_testResolution
  (
    Arcadia_Thread* thread
  );

void
Arcadia_VPL_Tests_Tree_testResolutionFailures
  (
    Arcadia_Thread* thread
  );

void
Arcadia_VPL_Tests_Tree_testTyping
  (
    Arcadia_Thread* thread
  );

void
Arcadia_VPL_Tests_Tree_testTypingFailures
  (
    Arcadia_Thread* thread
  );

static Arcadia_List*
createList
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_List* list = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)list);
  return list;
}

static Arcadia_VPL_Tree_Node*
createName
  (
    Arcadia_Thread* thread,
    const char* name
  )
{
  Arcadia_String* string = Arcadia_String_createFromCxxString(thread, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)string);
  Arcadia_VPL_Tree_Node* node = Arcadia_VPL_Tree_NodeFactory_makeNameExpr(thread, string);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
  return node;
}

static void
assertNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* node,
    Arcadia_Type* type,
    Arcadia_Integer32Value flags
  )
{
  Arcadia_Tests_assertTrue(thread, flags == node->flags);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_getType(thread, (Arcadia_Object*)node) == type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_VPL_Tree_Node_getType(thread)));
}

static void
testConstructors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Tree_Node* condition = createName(thread, "enabled");
  Arcadia_VPL_Tree_Node* initializer = createName(thread, "initialize");
  Arcadia_VPL_Tree_Node* update = createName(thread, "advance");
  Arcadia_VPL_Tree_Node* target = createName(thread, "lights");
  Arcadia_VPL_Tree_Node* subscript = createName(thread, "i");
  Arcadia_List* body = createList(thread);
  Arcadia_List* elseBody = createList(thread);
  Arcadia_SizeValue rootedSize = Arcadia_ValueStack_getSize(thread);

  Arcadia_VPL_Tree_IndexExprNode* index = Arcadia_VPL_Tree_IndexExprNode_create(thread, target, subscript);
  assertNode(thread, (Arcadia_VPL_Tree_Node*)index, _Arcadia_VPL_Tree_IndexExprNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_IndexExpr);
  Arcadia_Tests_assertTrue(thread, target == index->target && subscript == index->index);
  Arcadia_List_insertBackObjectReferenceValue(thread, body, (Arcadia_Object*)index);

  Arcadia_VPL_Tree_IfStmtNode* branch = Arcadia_VPL_Tree_IfStmtNode_create(thread, condition, body, elseBody);
  assertNode(thread, (Arcadia_VPL_Tree_Node*)branch, _Arcadia_VPL_Tree_IfStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_IfStmt);
  Arcadia_Tests_assertTrue(thread, condition == branch->condition && body == branch->thenBody && elseBody == branch->elseBody);
  branch = Arcadia_VPL_Tree_IfStmtNode_create(thread, condition, body, NULL);
  Arcadia_Tests_assertTrue(thread, NULL == branch->elseBody);

  Arcadia_VPL_Tree_WhileStmtNode* loop = Arcadia_VPL_Tree_WhileStmtNode_create(thread, condition, body);
  assertNode(thread, (Arcadia_VPL_Tree_Node*)loop, _Arcadia_VPL_Tree_WhileStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_WhileStmt);
  Arcadia_Tests_assertTrue(thread, condition == loop->condition && body == loop->body);

  // Exercise every combination of omitted for clauses, including for (;;).
  for (int mask = 0; mask < 8; ++mask) {
    Arcadia_VPL_Tree_Node* init = (mask & 1) ? initializer : NULL;
    Arcadia_VPL_Tree_Node* cond = (mask & 2) ? condition : NULL;
    Arcadia_VPL_Tree_Node* step = (mask & 4) ? update : NULL;
    Arcadia_VPL_Tree_ForStmtNode* forLoop = Arcadia_VPL_Tree_ForStmtNode_create(thread, init, cond, step, body);
    assertNode(thread, (Arcadia_VPL_Tree_Node*)forLoop, _Arcadia_VPL_Tree_ForStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_ForStmt);
    Arcadia_Tests_assertTrue(thread, init == forLoop->initializer && cond == forLoop->condition && step == forLoop->update && body == forLoop->body);
  }

  Arcadia_VPL_Tree_ReturnStmtNode* returnValue = Arcadia_VPL_Tree_ReturnStmtNode_create(thread, (Arcadia_VPL_Tree_Node*)index);
  assertNode(thread, (Arcadia_VPL_Tree_Node*)returnValue, _Arcadia_VPL_Tree_ReturnStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_ReturnStmt);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_Tree_Node*)index == returnValue->value);
  returnValue = Arcadia_VPL_Tree_ReturnStmtNode_create(thread, NULL);
  Arcadia_Tests_assertTrue(thread, NULL == returnValue->value);
  Arcadia_VPL_Tree_DiscardStmtNode* discard = Arcadia_VPL_Tree_DiscardStmtNode_create(thread);
  assertNode(thread, (Arcadia_VPL_Tree_Node*)discard, _Arcadia_VPL_Tree_DiscardStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_DiscardStmt);
  Arcadia_Tests_assertTrue(thread, rootedSize == Arcadia_ValueStack_getSize(thread));
  Arcadia_ValueStack_popValues(thread, rootedSize - stackSize);
}

static void
testNestedBuilders
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Tree_Node* condition = createName(thread, "enabled");
  Arcadia_VPL_Tree_Node* target = createName(thread, "lights");
  Arcadia_VPL_Tree_Node* subscript = createName(thread, "i");
  Arcadia_List* thenBody = createList(thread);
  Arcadia_List* elseBody = createList(thread);
  Arcadia_List* whileBody = createList(thread);
  Arcadia_List* forBody = createList(thread);

  Arcadia_VPL_Tree_Node* index = Arcadia_VPL_Tree_NodeFactory_makeIndexExpr(thread, target, subscript);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)index);
  Arcadia_VPL_Tree_Node* returned = Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, index);
  Arcadia_List_insertBackObjectReferenceValue(thread, thenBody, (Arcadia_Object*)returned);
  Arcadia_VPL_Tree_Node* discarded = Arcadia_VPL_Tree_NodeFactory_makeDiscardStmt(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, elseBody, (Arcadia_Object*)discarded);
  Arcadia_VPL_Tree_Node* branch = Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, condition, thenBody, elseBody);
  Arcadia_List_insertBackObjectReferenceValue(thread, whileBody, (Arcadia_Object*)branch);
  Arcadia_VPL_Tree_Node* whileLoop = Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, condition, whileBody);
  Arcadia_List_insertBackObjectReferenceValue(thread, forBody, (Arcadia_Object*)whileLoop);
  Arcadia_VPL_Tree_Node* forLoop = Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, NULL, NULL, NULL, forBody);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)forLoop);

  assertNode(thread, index, _Arcadia_VPL_Tree_IndexExprNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_IndexExpr);
  assertNode(thread, returned, _Arcadia_VPL_Tree_ReturnStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_ReturnStmt);
  assertNode(thread, discarded, _Arcadia_VPL_Tree_DiscardStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_DiscardStmt);
  assertNode(thread, branch, _Arcadia_VPL_Tree_IfStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_IfStmt);
  assertNode(thread, whileLoop, _Arcadia_VPL_Tree_WhileStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_WhileStmt);
  assertNode(thread, forLoop, _Arcadia_VPL_Tree_ForStmtNode_getType(thread), Arcadia_VPL_Tree_NodeFlags_ForStmt);
  Arcadia_Tests_assertTrue(thread, index == ((Arcadia_VPL_Tree_ReturnStmtNode*)returned)->value);
  Arcadia_Value child = Arcadia_List_getAt(thread, ((Arcadia_VPL_Tree_ForStmtNode*)forLoop)->body, 0);
  Arcadia_Tests_assertTrue(thread, (Arcadia_Object*)whileLoop == Arcadia_Value_getObjectReferenceValue(&child));
  child = Arcadia_List_getAt(thread, ((Arcadia_VPL_Tree_WhileStmtNode*)whileLoop)->body, 0);
  Arcadia_Tests_assertTrue(thread, (Arcadia_Object*)branch == Arcadia_Value_getObjectReferenceValue(&child));
  child = Arcadia_List_getAt(thread, ((Arcadia_VPL_Tree_IfStmtNode*)branch)->thenBody, 0);
  Arcadia_Tests_assertTrue(thread, (Arcadia_Object*)returned == Arcadia_Value_getObjectReferenceValue(&child));
  child = Arcadia_List_getAt(thread, ((Arcadia_VPL_Tree_IfStmtNode*)branch)->elseBody, 0);
  Arcadia_Tests_assertTrue(thread, (Arcadia_Object*)discarded == Arcadia_Value_getObjectReferenceValue(&child));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
assertInvalidArguments
  (
    Arcadia_Thread* thread,
    Arcadia_Type* type,
    Arcadia_Object* self,
    Arcadia_Value* arguments,
    Arcadia_Natural8Value count,
    Arcadia_Status expectedStatus
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    for (Arcadia_Natural8Value i = 0; i < count; ++i) {
      Arcadia_ValueStack_pushValue(thread, &arguments[i]);
    }
    Arcadia_ValueStack_pushNatural8Value(thread, count);
    // Test the callback on a correctly sized object so create's stack check does not mask the status.
    Arcadia_Type_getOperations(type)->objectTypeOperations->construct(thread, self);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Object_setType(thread, self, type);
    Arcadia_Tests_assertTrue(thread, expectedStatus == status);
  }
}

static void
testInvalidArguments
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Tree_Node* condition = createName(thread, "enabled");
  Arcadia_List* body = createList(thread);
  Arcadia_Value node = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)condition);
  Arcadia_Value list = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)body);
  Arcadia_Value null = Arcadia_Value_makeObjectReferenceValue(NULL);
  Arcadia_Value number = Arcadia_Value_makeNatural8Value(1);
  Arcadia_Value valid[][4] = {
    { node, list, null },
    { null, node, null, list },
    { node, list },
    { node },
    { null },
    { node, node },
  };
  Arcadia_Type* types[] = {
    _Arcadia_VPL_Tree_IfStmtNode_getType(thread),
    _Arcadia_VPL_Tree_ForStmtNode_getType(thread),
    _Arcadia_VPL_Tree_WhileStmtNode_getType(thread),
    _Arcadia_VPL_Tree_ReturnStmtNode_getType(thread),
    _Arcadia_VPL_Tree_DiscardStmtNode_getType(thread),
    _Arcadia_VPL_Tree_IndexExprNode_getType(thread),
  };
  Arcadia_Natural8Value counts[] = { 3, 4, 2, 1, 0, 2 };
  Arcadia_BooleanValue optional[][4] = {
    { Arcadia_BooleanValue_False, Arcadia_BooleanValue_False, Arcadia_BooleanValue_True },
    { Arcadia_BooleanValue_True, Arcadia_BooleanValue_True, Arcadia_BooleanValue_True, Arcadia_BooleanValue_False },
    { Arcadia_BooleanValue_False, Arcadia_BooleanValue_False },
    { Arcadia_BooleanValue_True },
    { Arcadia_BooleanValue_False },
    { Arcadia_BooleanValue_False, Arcadia_BooleanValue_False },
  };
  Arcadia_Object* samples[6];
  samples[0] = (Arcadia_Object*)Arcadia_VPL_Tree_IfStmtNode_create(thread, condition, body, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, samples[0]);
  samples[1] = (Arcadia_Object*)Arcadia_VPL_Tree_ForStmtNode_create(thread, NULL, condition, NULL, body);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, samples[1]);
  samples[2] = (Arcadia_Object*)Arcadia_VPL_Tree_WhileStmtNode_create(thread, condition, body);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, samples[2]);
  samples[3] = (Arcadia_Object*)Arcadia_VPL_Tree_ReturnStmtNode_create(thread, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, samples[3]);
  samples[4] = (Arcadia_Object*)Arcadia_VPL_Tree_DiscardStmtNode_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, samples[4]);
  samples[5] = (Arcadia_Object*)Arcadia_VPL_Tree_IndexExprNode_create(thread, condition, condition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, samples[5]);
  for (int i = 0; i < 6; ++i) {
    // Incorrect counts must be rejected by every concrete node constructor.
    assertInvalidArguments(thread, types[i], samples[i], valid[i], counts[i] ? 0 : 1, Arcadia_Status_NumberOfArgumentsInvalid);
    for (Arcadia_Natural8Value j = 0; j < counts[i]; ++j) {
      Arcadia_Value saved = valid[i][j];
      valid[i][j] = number;
      assertInvalidArguments(thread, types[i], samples[i], valid[i], counts[i], Arcadia_Status_ArgumentTypeInvalid);
      // An object of the wrong class must also be rejected, even in optional slots.
      valid[i][j] = (i == 0 && j > 0) || (i == 1 && j == 3) || (i == 2 && j == 1) ? node : list;
      assertInvalidArguments(thread, types[i], samples[i], valid[i], counts[i], Arcadia_Status_ArgumentTypeInvalid);
      if (!optional[i][j]) {
        valid[i][j] = null;
        assertInvalidArguments(thread, types[i], samples[i], valid[i], counts[i], Arcadia_Status_ArgumentValueInvalid);
      }
      valid[i][j] = saved;
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testConstructors)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testNestedBuilders)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testInvalidArguments)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&Arcadia_VPL_Tests_Tree_testResolution)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&Arcadia_VPL_Tests_Tree_testResolutionFailures)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&Arcadia_VPL_Tests_Tree_testTyping)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&Arcadia_VPL_Tests_Tree_testTypingFailures)) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
