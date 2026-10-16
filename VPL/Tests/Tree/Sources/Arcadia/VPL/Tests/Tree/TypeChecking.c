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
text
  (
    Arcadia_Thread* thread,
    const char* string
  )
{
  return (Arcadia_String*)root(thread, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, string));
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
    const char* string
  )
{
  return (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeNameExpr(thread, text(thread, string)));
}

static Arcadia_VPL_Tree_Node*
number
  (
    Arcadia_Thread* thread,
    const char* literal
  )
{
  return (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_NumberExprNode_create(thread, text(thread, literal)));
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

static Arcadia_VPL_Tree_Node*
variable
  (
    Arcadia_Thread* thread,
    const char* variableName,
    const char* typeName
  )
{
  return (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_VariableDefnNode_create(thread, text(thread, variableName), text(thread, typeName)));
}

static void
registerType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* program,
    const char* typeName,
    Arcadia_VPL_IR_Type* type
  )
{
  root(thread, (Arcadia_Object*)type);
  Arcadia_VPL_Symbols_BuiltinType* symbol = (Arcadia_VPL_Symbols_BuiltinType*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Symbols_BuiltinType_create(thread, text(thread, typeName)));
  symbol->valueType = type;
  Arcadia_List_insertBackObjectReferenceValue(thread, program->types, (Arcadia_Object*)symbol);
}

static Arcadia_VPL_Symbols_Program*
program
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue count
  )
{
  Arcadia_VPL_Symbols_Program* p = (Arcadia_VPL_Symbols_Program*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Symbols_Program_create(thread, Arcadia_VPL_Symbols_ProgramFlags_MeshAmbientColor));
  p->context = Arcadia_VPL_Context_create(thread);
  p->programDefnNode = Arcadia_VPL_Tree_ProgramDefnNode_create(thread);
  registerType(thread, p, "array4", (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_ArrayType_create(thread, p->VEC4->valueType, count));
  Arcadia_List* fields = list(thread);
  Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "color"), p->VEC4->valueType));
  Arcadia_List_insertBackObjectReferenceValue(thread, fields, (Arcadia_Object*)field);
  Arcadia_VPL_IR_Type* record = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Light"), fields));
  registerType(thread, p, "records", (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_ArrayType_create(thread, record, 2));
  return p;
}

static Arcadia_VPL_Tree_ProcedureDefnNode*
procedure
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* p,
    const char* procedureName,
    const char* returnType,
    const char* stage
  )
{
  Arcadia_VPL_Tree_ProcedureDefnNode* proc = (Arcadia_VPL_Tree_ProcedureDefnNode*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeProcedureDefn(thread, list(thread), (Arcadia_VPL_Tree_NameExprNode*)name(thread, returnType), text(thread, procedureName), list(thread), list(thread)));
  Arcadia_List_insertBackObjectReferenceValue(thread, proc->modifiers, (Arcadia_Object*)text(thread, stage));
  append(thread, p->programDefnNode->elements, (Arcadia_VPL_Tree_Node*)proc);
  return proc;
}

static void
parameters
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ProcedureDefnNode* proc
  )
{
  append(thread, proc->parameters, variable(thread, "gate", "bool"));
  append(thread, proc->parameters, variable(thread, "i", "int"));
  append(thread, proc->parameters, variable(thread, "v", "vec4"));
  append(thread, proc->parameters, variable(thread, "f", "float"));
  append(thread, proc->parameters, variable(thread, "a", "array4"));
  append(thread, proc->parameters, variable(thread, "r", "records"));
  append(thread, proc->parameters, variable(thread, "s", "sampler2D"));
  append(thread, proc->parameters, variable(thread, "m", "mat4"));
}

static Arcadia_VPL_Tree_Node*
call
  (
    Arcadia_Thread* thread,
    const char* callee,
    Arcadia_VPL_Tree_Node* argument
  )
{
  Arcadia_VPL_Tree_CallExprNode* node = (Arcadia_VPL_Tree_CallExprNode*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeCallExpr(thread, name(thread, callee)));
  if (argument) append(thread, node->arguments, argument);
  return (Arcadia_VPL_Tree_Node*)node;
}

static Arcadia_VPL_Tree_Node*
index
  (
    Arcadia_Thread* thread,
    const char* target,
    Arcadia_VPL_Tree_Node* subscript
  )
{
  return (Arcadia_VPL_Tree_Node*)root(thread, (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeIndexExpr(thread, name(thread, target), subscript));
}

static void
resolve
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Program* p
  )
{
  Arcadia_VPL_EnterPhase_run(thread, Arcadia_VPL_Context_getEnterPhase(thread, p->context), p);
  Arcadia_VPL_ResolvePhase_run(thread, Arcadia_VPL_Context_getResolvePhase(thread, p->context), p);
}

void
Arcadia_VPL_Tests_Tree_testTyping
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Symbols_Program* p = program(thread, 4);
  Arcadia_VPL_Tree_ProcedureDefnNode* proc = procedure(thread, p, "main", "float", "fragment");
  parameters(thread, proc);
  Arcadia_List* thenBody = list(thread);
  Arcadia_List* elseBody = list(thread);
  Arcadia_List* whileBody = list(thread);
  append(thread, elseBody, Arcadia_VPL_Tree_NodeFactory_makeDiscardStmt(thread));
  append(thread, whileBody, Arcadia_VPL_Tree_NodeFactory_makeDiscardStmt(thread));
  append(thread, thenBody, Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, name(thread, "gate"), whileBody));
  Arcadia_VPL_Tree_Node* branch = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, name(thread, "gate"), thenBody, elseBody));
  append(thread, proc->body, branch);
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, variable(thread, "enabled", "bool"), name(thread, "enabled"), NULL, list(thread)));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, NULL, NULL, NULL, list(thread)));
  Arcadia_VPL_Tree_Node* indexed = index(thread, "a", name(thread, "i"));
  append(thread, proc->body, indexed);
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, index(thread, "a", number(thread, "3")), name(thread, "v")));
  Arcadia_VPL_Tree_Node* member = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeAccessExpr(thread, index(thread, "r", number(thread, "0")), name(thread, "color")));
  append(thread, proc->body, member);
  Arcadia_VPL_Tree_Node* swizzle = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeAccessExpr(thread, member, name(thread, "xyz")));
  append(thread, proc->body, swizzle);
  Arcadia_VPL_Tree_Node* integer = number(thread, "12");
  Arcadia_VPL_Tree_Node* real = number(thread, "1e-2");
  Arcadia_VPL_Tree_Node* addition = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeAddExpr(thread, integer, real));
  append(thread, proc->body, addition);
  Arcadia_VPL_Tree_Node* matrixProduct = (Arcadia_VPL_Tree_Node*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_Tree_NodeFactory_makeMultiplyExpr(thread, name(thread, "m"), name(thread, "v")));
  append(thread, proc->body, matrixProduct);
  Arcadia_VPL_Tree_Node* construction = call(thread, "vec4", number(thread, "1.0"));
  append(thread, proc->body, construction);
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, name(thread, "f"), integer));
  append(thread, proc->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, integer));
  Arcadia_VPL_Tree_ProcedureDefnNode* helper = procedure(thread, p, "helper", "float", "fragment");
  append(thread, helper->parameters, variable(thread, "x", "float"));
  append(thread, helper->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, name(thread, "x")));
  Arcadia_VPL_Tree_Node* invocation = call(thread, "helper", number(thread, "1"));
  append(thread, proc->body, invocation);

  resolve(thread, p);
  Arcadia_VPL_TypePhase* phase = Arcadia_VPL_Context_getTypePhase(thread, p->context);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_TypePhase_getDiagnostics(thread, phase) == Arcadia_VPL_Context_getDiagnostics(thread, p->context));
  Arcadia_SizeValue rootedSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_TypePhase_run(thread, phase, p);
  Arcadia_Tests_assertTrue(thread, rootedSize == Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, p->INT->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, integer));
  Arcadia_Tests_assertTrue(thread, p->FLOAT->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, real));
  Arcadia_Tests_assertTrue(thread, p->FLOAT->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, addition));
  Arcadia_Tests_assertTrue(thread, p->FLOAT->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, invocation));
  Arcadia_Tests_assertTrue(thread, p->VEC4->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, indexed));
  Arcadia_Tests_assertTrue(thread, p->VEC4->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, member));
  Arcadia_Tests_assertTrue(thread, p->VEC3->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, swizzle));
  Arcadia_Tests_assertTrue(thread, p->VEC4->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, construction));
  Arcadia_VPL_IR_Type* productType = Arcadia_VPL_TypePhase_getType(thread, phase, matrixProduct);
  Arcadia_Tests_assertTrue(thread, productType->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)productType)->size == 4);
  Arcadia_Tests_assertTrue(thread, p->VOID->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, branch));
  // Running a phase again must replace its metadata, without leaking value-stack roots.
  Arcadia_VPL_TypePhase_run(thread, phase, p);
  Arcadia_Tests_assertTrue(thread, rootedSize == Arcadia_ValueStack_getSize(thread));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

void
Arcadia_VPL_Tests_Tree_testTypingFailures
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 30; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_VPL_Symbols_Program* p = program(thread, test == 23 ? 0 : 4);
    Arcadia_VPL_Tree_ProcedureDefnNode* proc = procedure(thread, p, "main", test == 4 || test == 5 ? "float" : "void", test == 13 || test == 27 ? "vertex" : "fragment");
    parameters(thread, proc);
    Arcadia_List* body = list(thread);
    Arcadia_VPL_Tree_Node* bad = NULL;
    switch (test) {
      case 0: bad = Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, name(thread, "i"), body, NULL); break;
      case 1: bad = Arcadia_VPL_Tree_NodeFactory_makeWhileStmt(thread, name(thread, "v"), body); break;
      case 2: bad = Arcadia_VPL_Tree_NodeFactory_makeForStmt(thread, NULL, name(thread, "f"), NULL, body); break;
      case 3: bad = Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, number(thread, "1")); break;
      case 4: bad = Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, NULL); break;
      case 5: bad = Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, name(thread, "v")); break;
      case 6: bad = index(thread, "a", name(thread, "f")); break;
      case 7: bad = index(thread, "v", name(thread, "i")); break;
      case 8: bad = index(thread, "a", number(thread, "4")); break;
      case 9: bad = index(thread, "a", number(thread, "-1")); break;
      case 10: bad = index(thread, "a", number(thread, "9999999999999999999999999999999999")); break;
      case 11: bad = Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, name(thread, "v"), name(thread, "f")); break;
      case 12: {
        Arcadia_VPL_Tree_VariableDefnNode* uniform = (Arcadia_VPL_Tree_VariableDefnNode*)variable(thread, "uniformArray", "array4");
        Arcadia_List_insertBackObjectReferenceValue(thread, uniform->modifiers, (Arcadia_Object*)text(thread, "constant"));
        append(thread, p->programDefnNode->elements, (Arcadia_VPL_Tree_Node*)uniform);
        bad = Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, index(thread, "uniformArray", number(thread, "0")), name(thread, "v"));
      } break;
      case 13: bad = Arcadia_VPL_Tree_NodeFactory_makeDiscardStmt(thread); break;
      case 14: bad = call(thread, "vec4", call(thread, "vec3", number(thread, "1.0"))); break;
      case 15: bad = call(thread, "vec4", name(thread, "gate")); break;
      case 16:
      case 17: {
        Arcadia_VPL_Tree_ProcedureDefnNode* helper = procedure(thread, p, "helper", "float", "fragment");
        append(thread, helper->parameters, variable(thread, "x", "float"));
        append(thread, helper->body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, name(thread, "x")));
        bad = call(thread, "helper", test == 16 ? NULL : name(thread, "v"));
      } break;
      case 18: bad = number(thread, "1e"); break;
      case 19: bad = variable(thread, "invalid", "void"); break;
      case 20: bad = Arcadia_VPL_Tree_NodeFactory_makeAddExpr(thread, name(thread, "gate"), name(thread, "gate")); break;
      case 21: bad = Arcadia_VPL_Tree_NodeFactory_makeAccessExpr(thread, name(thread, "f"), name(thread, "unknown")); break;
      case 22: bad = Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, name(thread, "i"), name(thread, "f")); break;
      case 23: bad = name(thread, "a"); break;
      case 24: bad = call(thread, "int", number(thread, "1.5")); break;
      case 25: bad = Arcadia_VPL_Tree_NodeFactory_makeAccessExpr(thread, index(thread, "r", number(thread, "0")), name(thread, "unknown")); break;
      case 26: bad = Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, number(thread, "1"), body, NULL); break;
      case 27: {
        Arcadia_VPL_Tree_VariableDefnNode* input = (Arcadia_VPL_Tree_VariableDefnNode*)variable(thread, "stageInput", "vec4");
        Arcadia_List_insertBackObjectReferenceValue(thread, input->modifiers, (Arcadia_Object*)text(thread, "vertex"));
        Arcadia_List_insertBackObjectReferenceValue(thread, input->modifiers, (Arcadia_Object*)text(thread, "variable"));
        append(thread, p->programDefnNode->elements, (Arcadia_VPL_Tree_Node*)input);
        bad = Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr(thread, name(thread, "stageInput"), name(thread, "v"));
      } break;
      case 28: bad = Arcadia_VPL_Tree_NodeFactory_makeMultiplyExpr(thread, name(thread, "s"), name(thread, "s")); break;
      case 29:
        append(thread, body, Arcadia_VPL_Tree_NodeFactory_makeReturnStmt(thread, number(thread, "1")));
        bad = Arcadia_VPL_Tree_NodeFactory_makeIfStmt(thread, name(thread, "gate"), body, NULL);
        break;
    }
    append(thread, proc->body, bad);
    resolve(thread, p);
    Arcadia_VPL_TypePhase* phase = Arcadia_VPL_Context_getTypePhase(thread, p->context);
    Arcadia_SizeValue rootedSize = Arcadia_ValueStack_getSize(thread);
    Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_VPL_TypePhase_run(thread, phase, p);
    } else {
      raised = Arcadia_BooleanValue_True;
    }
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, raised && status == Arcadia_Status_SemanticalError);
    Arcadia_Tests_assertTrue(thread, rootedSize == Arcadia_ValueStack_getSize(thread));
    Arcadia_Tests_assertTrue(thread, NULL == Arcadia_VPL_TypePhase_getType(thread, phase, bad));
    Arcadia_Languages_Diagnostics* diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, p->context);
    Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, diagnostics));
    Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics));
    Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, diagnostics->diagnostics, 0, _Arcadia_VPL_Diagnostics_TypeErrorDiagnostic_getType(thread));
    Arcadia_String* message = Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_getNumberOfBytes(thread, message) > 12);
    if (test == 0) {
      const char expected[] = "type error: condition must have type bool\n";
      Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, message, expected, sizeof(expected) - 1));
      // Recover on the same phase after fixing the tree; the failed run published no types.
      ((Arcadia_VPL_Tree_IfStmtNode*)bad)->condition = name(thread, "gate");
      Arcadia_VPL_ResolvePhase_run(thread, Arcadia_VPL_Context_getResolvePhase(thread, p->context), p);
      Arcadia_VPL_TypePhase_run(thread, phase, p);
      Arcadia_Tests_assertTrue(thread, p->VOID->valueType == Arcadia_VPL_TypePhase_getType(thread, phase, bad));
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}
