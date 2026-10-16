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

#include "Arcadia/Testing/Include.h"

#include "Arcadia/VPL/Include.h"

typedef struct Fixture {
  Arcadia_VPL_IR_Module* module;
  Arcadia_VPL_IR_Verifier* verifier;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_VPL_IR_Type* integer;
  Arcadia_VPL_IR_Type* boolean;
  Arcadia_VPL_IR_Type* real;
  Arcadia_VPL_IR_Type* voidType;
  Arcadia_VPL_IR_RegisterValue* value;
  Arcadia_VPL_IR_RegisterValue* condition;
  Arcadia_VPL_IR_ConstantValue* one;
  Arcadia_VPL_IR_ConstantValue* two;
  Arcadia_VPL_IR_ConstantValue* yes;
  Arcadia_VPL_IR_Function* function;
  Arcadia_VPL_IR_Block* entry;
  Arcadia_VPL_IR_Block* left;
  Arcadia_VPL_IR_Block* right;
  Arcadia_VPL_IR_Block* join;
  Arcadia_VPL_IR_StageIO* positionOutput;
  Arcadia_VPL_IR_StageIO* colorOutput;
} Fixture;

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

static void
append
  (
    Arcadia_Thread* thread,
    Arcadia_List* list,
    Arcadia_Object* object
  )
{
  root(thread, object);
  Arcadia_List_insertBackObjectReferenceValue(thread, list, object);
}

static Arcadia_VPL_IR_Block*
block
  (
    Arcadia_Thread* thread
  )
{
  return (Arcadia_VPL_IR_Block*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Block_create(thread, list(thread), NULL));
}

static Arcadia_VPL_IR_RegisterValue*
reg
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue index,
    Arcadia_VPL_IR_Type* type
  )
{
  return (Arcadia_VPL_IR_RegisterValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RegisterValue_create(thread, index, type));
}

static Arcadia_VPL_IR_ReferenceValue*
reference
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value kind,
    Arcadia_SizeValue slot,
    Arcadia_VPL_IR_Type* type
  )
{
  return (Arcadia_VPL_IR_ReferenceValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ReferenceValue_createWithStorage(thread, kind, slot, type));
}

static void
copy
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Block* block,
    Arcadia_VPL_IR_RegisterValue* result,
    Arcadia_VPL_IR_Value* source
  )
{
  append(thread, block->instructions, (Arcadia_Object*)Arcadia_VPL_IR_CopyInstruction_create(thread, result, source));
}

static void
declare
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Block* block,
    Arcadia_VPL_IR_RegisterValue* reg
  )
{
  append(thread, block->instructions, (Arcadia_Object*)Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, reg, reg->type));
}

static Arcadia_VPL_IR_Function*
entryFunction
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Type* voidType,
    const char* name
  )
{
  Arcadia_VPL_IR_Block* entry = block(thread);
  entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
  Arcadia_List* blocks = list(thread);
  append(thread, blocks, (Arcadia_Object*)entry);
  return (Arcadia_VPL_IR_Function*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Function_create(thread, text(thread, name), voidType, list(thread), blocks));
}

static Fixture
fixture
  (
    Arcadia_Thread* thread
  )
{
  Fixture f = { 0 };
  f.integer = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Integer));
  f.boolean = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Boolean));
  f.real = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating));
  f.voidType = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VoidType_create(thread));
  f.one = (Arcadia_VPL_IR_ConstantValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.integer, Arcadia_Value_makeInteger32Value(1)));
  f.two = (Arcadia_VPL_IR_ConstantValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.integer, Arcadia_Value_makeInteger32Value(2)));
  f.yes = (Arcadia_VPL_IR_ConstantValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.boolean, Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_True)));
  f.value = reg(thread, 42, f.integer); // Register IDs need not be dense.
  f.condition = reg(thread, 7, f.boolean);
  f.entry = block(thread);
  f.left = block(thread);
  f.right = block(thread);
  f.join = block(thread);
  Arcadia_List* blocks = list(thread);
  append(thread, blocks, (Arcadia_Object*)f.entry);
  append(thread, blocks, (Arcadia_Object*)f.left);
  append(thread, blocks, (Arcadia_Object*)f.right);
  append(thread, blocks, (Arcadia_Object*)f.join);
  f.function = (Arcadia_VPL_IR_Function*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Function_create(thread, text(thread, "compute"), f.integer, list(thread), blocks));
  Arcadia_List* functions = list(thread);
  append(thread, functions, (Arcadia_Object*)f.function);
  Arcadia_VPL_IR_Function* vertexEntry = entryFunction(thread, f.voidType, "vertex_start");
  Arcadia_List* vertexFunctions = list(thread);
  append(thread, vertexFunctions, (Arcadia_Object*)vertexEntry);
  Arcadia_VPL_IR_Function* fragmentEntry = entryFunction(thread, f.voidType, "fragment_start");
  append(thread, functions, (Arcadia_Object*)fragmentEntry);
  Arcadia_VPL_IR_Stage* vertex = (Arcadia_VPL_IR_Stage*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Stage_createWithEntryFunction(thread, Arcadia_VPL_IR_Stage_Vertex, list(thread), vertexFunctions, vertexEntry));
  Arcadia_VPL_IR_Stage* fragment = (Arcadia_VPL_IR_Stage*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Stage_createWithEntryFunction(thread, Arcadia_VPL_IR_Stage_Fragment, list(thread), functions, fragmentEntry));
  f.module = (Arcadia_VPL_IR_Module*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Module_create(thread, list(thread), vertex, fragment));
  Arcadia_VPL_IR_Type* vec4 = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4));
  f.positionOutput = (Arcadia_VPL_IR_StageIO*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, text(thread, "required_position"), vec4, -1, Arcadia_VPL_IR_StageIO_Semantics_Position));
  f.colorOutput = (Arcadia_VPL_IR_StageIO*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, text(thread, "required_color"), vec4, 0, Arcadia_VPL_IR_StageIO_Semantics_FragmentColor));
  append(thread, vertex->interface, (Arcadia_Object*)f.positionOutput);
  append(thread, fragment->interface, (Arcadia_Object*)f.colorOutput);
  Arcadia_VPL_Context* context = (Arcadia_VPL_Context*)root(thread, (Arcadia_Object*)Arcadia_VPL_Context_create(thread));
  f.diagnostics = Arcadia_VPL_Context_getDiagnostics(thread, context);
  f.verifier = (Arcadia_VPL_IR_Verifier*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Verifier_create(thread, f.diagnostics));
  declare(thread, f.entry, f.value);
  declare(thread, f.entry, f.condition);
  copy(thread, f.entry, f.condition, (Arcadia_VPL_IR_Value*)f.yes);
  f.entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ConditionalBranchTerminator_create(thread, (Arcadia_VPL_IR_Value*)f.condition, f.left, f.right);
  copy(thread, f.left, f.value, (Arcadia_VPL_IR_Value*)f.one);
  f.left->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, f.join);
  copy(thread, f.right, f.value, (Arcadia_VPL_IR_Value*)f.two);
  f.right->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, f.join);
  f.join->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, (Arcadia_VPL_IR_Value*)f.value);
  return f;
}

static Arcadia_VPL_IR_Value*
defaultOutputValue
  (
    Arcadia_Thread* thread,
    Fixture* f,
    Arcadia_VPL_IR_Type* type,
    Arcadia_List* declarations,
    Arcadia_List* operations,
    Arcadia_SizeValue* nextRegister
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Scalar) {
    Arcadia_Natural8Value kind = ((Arcadia_VPL_IR_ScalarType*)type)->elementKind;
    Arcadia_Value value = kind == Arcadia_VPL_IR_ScalarElementKind_Boolean ? Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_False)
      : kind == Arcadia_VPL_IR_ScalarElementKind_Integer ? Arcadia_Value_makeInteger32Value(0) : Arcadia_Value_makeReal32Value(0.f);
    return (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, type, value));
  }
  Arcadia_List* arguments = list(thread);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); i < n; ++i) {
      Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)Arcadia_List_getObjectReferenceValueAt(thread, fields, i);
      append(thread, arguments, (Arcadia_Object*)defaultOutputValue(thread, f, field->type, declarations, operations, nextRegister));
    }
  } else if (type->kind == Arcadia_VPL_IR_TypeKind_Array) {
    for (Arcadia_SizeValue i = 0; i < ((Arcadia_VPL_IR_ArrayType*)type)->count; ++i) append(thread, arguments, (Arcadia_Object*)defaultOutputValue(thread, f, ((Arcadia_VPL_IR_ArrayType*)type)->element, declarations, operations, nextRegister));
  } else {
    Arcadia_Natural8Value element = type->kind == Arcadia_VPL_IR_TypeKind_Vector ? ((Arcadia_VPL_IR_VectorType*)type)->elementKind : Arcadia_VPL_IR_ScalarElementKind_Floating;
    Arcadia_VPL_IR_Type* scalarType = element == Arcadia_VPL_IR_ScalarElementKind_Boolean ? f->boolean : element == Arcadia_VPL_IR_ScalarElementKind_Integer ? f->integer : f->real;
    append(thread, arguments, (Arcadia_Object*)defaultOutputValue(thread, f, scalarType, declarations, operations, nextRegister));
  }
  Arcadia_VPL_IR_RegisterValue* result = reg(thread, (*nextRegister)++, type);
  append(thread, declarations, (Arcadia_Object*)Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, result, type));
  append(thread, operations, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, result, type, arguments));
  return (Arcadia_VPL_IR_Value*)result;
}

static void
prepareStageOutputs
  (
    Arcadia_Thread* thread,
    Fixture* f,
    Arcadia_VPL_IR_Stage* stage
  )
{
  Arcadia_VPL_IR_Function* entry = stage->entryFunction;
  if (!entry || !Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)entry, _Arcadia_VPL_IR_Function_getType(thread)) || entry->returnType->kind != Arcadia_VPL_IR_TypeKind_Void || Arcadia_Collection_getSize(thread, (Arcadia_Collection*)entry->parameters) || !Arcadia_Collection_getSize(thread, (Arcadia_Collection*)entry->blocks)) return;
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, entry->blocks, 0);
  Arcadia_List* declarations = list(thread);
  Arcadia_List* operations = list(thread);
  Arcadia_SizeValue nextRegister = 0;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)stage->interface); i < n; ++i) {
    Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)Arcadia_List_getObjectReferenceValueAt(thread, stage->interface, i);
    if (io->direction != Arcadia_VPL_IR_StageIO_Direction_Out) continue;
    Arcadia_VPL_IR_Value* value = defaultOutputValue(thread, f, io->type, declarations, operations, &nextRegister);
    append(thread, operations, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, reference(thread, Arcadia_VPL_IR_StorageKind_StageIO, i, io->type), value));
  }
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)block->instructions);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)declarations); i < n; ++i) append(thread, block->instructions, Arcadia_List_getObjectReferenceValueAt(thread, declarations, i));
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)operations); i < n; ++i) append(thread, block->instructions, Arcadia_List_getObjectReferenceValueAt(thread, operations, i));
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
verify
  (
    Arcadia_Thread* thread,
    Fixture* f,
    Arcadia_BooleanValue valid
  )
{
  // These core-operation tests use independent, valid output-producing stage entries.
  prepareStageOutputs(thread, f, f->module->vertex);
  prepareStageOutputs(thread, f, f->module->fragment);
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) Arcadia_VPL_IR_Verifier_run(thread, f->verifier, f->module);
  else raised = Arcadia_BooleanValue_True;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_setStatus(thread, 0);
  Arcadia_Tests_assertTrue(thread, stackSize == Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, raised == !valid);
  Arcadia_Tests_assertTrue(thread, valid ? status == 0 : status == Arcadia_Status_SemanticalError);
  Arcadia_Tests_assertTrue(thread, f->verifier->module == NULL && f->verifier->registers == NULL && f->verifier->types == NULL && f->verifier->activeReferences == NULL);
  if (!valid) {
    Arcadia_Tests_assertTrue(thread, Arcadia_Languages_Diagnostics_hasErrors(thread, f->diagnostics));
    Arcadia_Languages_Diagnostic* diagnostic = (Arcadia_Languages_Diagnostic*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, f->diagnostics->diagnostics, 0, _Arcadia_VPL_Diagnostics_VerificationDiagnostic_getType(thread));
    Arcadia_Tests_assertTrue(thread, Arcadia_String_getNumberOfBytes(thread, Arcadia_Languages_Diagnostic_getMessage(thread, diagnostic)) > 20);
  }
}

static Arcadia_VPL_IR_Function*
callee
  (
    Arcadia_Thread* thread,
    Fixture* f,
    Arcadia_VPL_IR_Type* type
  )
{
  Arcadia_List* blocks = list(thread);
  Arcadia_VPL_IR_Block* entry = block(thread);
  entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, type->kind == Arcadia_VPL_IR_TypeKind_Void ? NULL : (Arcadia_VPL_IR_Value*)f->one);
  append(thread, blocks, (Arcadia_Object*)entry);
  Arcadia_VPL_IR_Function* result = (Arcadia_VPL_IR_Function*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_Function_create(thread, text(thread, "helper"), type, list(thread), blocks));
  append(thread, f->module->fragment->functions, (Arcadia_Object*)result);
  return result;
}

static void
testGraphs
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 36; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Fixture f = fixture(thread);
    Arcadia_BooleanValue valid = Arcadia_BooleanValue_False;
    switch (test) {
      case 0: valid = Arcadia_BooleanValue_True; break;
      case 1: Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.right->instructions); break;
      case 2:
        Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.left->instructions);
        copy(thread, f.left, f.value, (Arcadia_VPL_IR_Value*)f.value);
        break;
      case 3: copy(thread, f.right, f.value, (Arcadia_VPL_IR_Value*)f.yes); break;
      case 4: copy(thread, f.right, reg(thread, 42, f.real), (Arcadia_VPL_IR_Value*)f.one); break;
      case 5: copy(thread, f.right, reg(thread, 100, f.integer), (Arcadia_VPL_IR_Value*)f.one); break;
      case 6:
        Arcadia_List_insertFront(thread, f.entry->instructions, Arcadia_Value_makeObjectReferenceValue(root(thread, (Arcadia_Object*)Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, f.value, f.integer))));
        break;
      case 7: declare(thread, f.entry, reg(thread, 99, f.integer)); break;
      case 8: f.right->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, block(thread)); break;
      case 9: f.right->terminator = NULL; break;
      case 10: ((Arcadia_VPL_IR_ConditionalBranchTerminator*)f.entry->terminator)->condition = (Arcadia_VPL_IR_Value*)f.one; break;
      case 11: f.join->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, (Arcadia_VPL_IR_Value*)f.yes); break;
      case 12: append(thread, f.join->instructions, (Arcadia_Object*)Arcadia_VPL_IR_Instruction_create(thread, 9)); break; // Retired discard opcode stays invalid.
      case 13:
        Arcadia_List_removeAt(thread, f.module->fragment->functions, 0, 1);
        append(thread, f.module->vertex->functions, (Arcadia_Object*)f.function);
        f.join->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_DiscardTerminator_create(thread);
        break;
      case 14:
        f.join->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_DiscardTerminator_create(thread);
        valid = Arcadia_BooleanValue_True;
        break;
      case 15:
        copy(thread, f.right, f.value, (Arcadia_VPL_IR_Value*)f.one);
        append(thread, f.join->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, f.value, Arcadia_VPL_IR_BinaryOperator_Add, (Arcadia_VPL_IR_Value*)f.value, (Arcadia_VPL_IR_Value*)f.one));
        valid = Arcadia_BooleanValue_True;
        break;
      case 16:
        append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, f.value, reference(thread, Arcadia_VPL_IR_StorageKind_Local, 42, f.integer)));
        break;
      case 17:
        append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, reference(thread, Arcadia_VPL_IR_StorageKind_Local, 42, f.integer), (Arcadia_VPL_IR_Value*)f.one));
        Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.left->instructions);
        Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.right->instructions);
        valid = Arcadia_BooleanValue_True;
        break;
      case 18:
      case 19:
      case 20:
        append(thread, f.module->uniforms, (Arcadia_Object*)Arcadia_VPL_IR_ScalarUniform_create(thread, text(thread, "constant"), f.integer));
        if (test == 18) append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, reference(thread, Arcadia_VPL_IR_StorageKind_Uniform, 0, f.integer), (Arcadia_VPL_IR_Value*)f.one));
        else append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, f.value, reference(thread, Arcadia_VPL_IR_StorageKind_Uniform, test == 19 ? 0 : 1, f.integer)));
        valid = test == 19;
        break;
      case 21:
      case 22:
        append(thread, f.function->parameters, (Arcadia_Object*)reference(thread, Arcadia_VPL_IR_StorageKind_Parameter, 0, f.integer));
        append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, f.value, reference(thread, Arcadia_VPL_IR_StorageKind_Parameter, test == 22 ? 0 : 1, f.integer)));
        valid = test == 22;
        break;
      case 23:
      case 24:
      case 25:
      case 26: {
        Arcadia_VPL_IR_Function* target = callee(thread, &f, test < 25 ? f.voidType : f.integer);
        append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_CallInstruction_create(thread, test == 24 || test == 25 ? NULL : f.value, target, Arcadia_VPL_IR_BuiltinFunction_None, list(thread)));
        valid = test == 24 || test == 26;
      } break;
      case 27: {
        Arcadia_VPL_IR_Function* target = callee(thread, &f, f.integer);
        append(thread, target->parameters, (Arcadia_Object*)reference(thread, Arcadia_VPL_IR_StorageKind_Parameter, 0, f.integer));
        append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_CallInstruction_create(thread, f.value, target, Arcadia_VPL_IR_BuiltinFunction_None, list(thread)));
      } break;
      case 28: append(thread, f.function->parameters, (Arcadia_Object*)reference(thread, Arcadia_VPL_IR_StorageKind_Local, 0, f.integer)); break;
      case 29: {
        Arcadia_VPL_IR_Function* target = callee(thread, &f, f.integer);
        Arcadia_Collection_clear(thread, (Arcadia_Collection*)target->blocks);
        append(thread, target->blocks, (Arcadia_Object*)f.entry);
      } break;
      case 30: {
        Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Cycle"), list(thread)));
        append(thread, record->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "self"), (Arcadia_VPL_IR_Type*)record));
        append(thread, f.module->uniforms, (Arcadia_Object*)Arcadia_VPL_IR_RecordUniform_create(thread, text(thread, "cycle"), record));
      } break;
      case 31: f.module->fragment->kind = Arcadia_VPL_IR_Stage_Vertex; break;
      case 32: f.one->literal = Arcadia_Value_makeReal32Value(1.f); break;
      case 33: copy(thread, f.entry, f.value, (Arcadia_VPL_IR_Value*)reference(thread, Arcadia_VPL_IR_StorageKind_Local, 42, f.integer)); break;
      case 34:
      case 35:
        append(thread, f.join->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, f.condition,
          test == 34 ? Arcadia_VPL_IR_BinaryOperator_Less : Arcadia_VPL_IR_BinaryOperator_Equal, (Arcadia_VPL_IR_Value*)f.condition, (Arcadia_VPL_IR_Value*)f.yes));
        valid = test == 35;
        break;
    }
    verify(thread, &f, valid);
    if (valid) verify(thread, &f, valid); // Reusable verifier, no retained scratch state.
    if (test == 1) {
      copy(thread, f.right, f.value, (Arcadia_VPL_IR_Value*)f.two);
      verify(thread, &f, Arcadia_BooleanValue_True); // Recovery after a failed verification.
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testLoops
  (
    Arcadia_Thread* thread
  )
{
  for (int initialized = 0; initialized < 2; ++initialized) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Fixture f = fixture(thread);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.entry->instructions);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.left->instructions);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.right->instructions);
    declare(thread, f.entry, f.value);
    declare(thread, f.entry, f.condition);
    if (initialized) copy(thread, f.entry, f.value, (Arcadia_VPL_IR_Value*)f.one);
    f.entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, f.left);
    append(thread, f.left->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, f.condition, Arcadia_VPL_IR_BinaryOperator_Less, (Arcadia_VPL_IR_Value*)f.value, (Arcadia_VPL_IR_Value*)f.two));
    f.left->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ConditionalBranchTerminator_create(thread, (Arcadia_VPL_IR_Value*)f.condition, f.right, f.join);
    append(thread, f.right->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, f.value, Arcadia_VPL_IR_BinaryOperator_Add, (Arcadia_VPL_IR_Value*)f.value, (Arcadia_VPL_IR_Value*)f.one));
    f.right->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, f.left);
    verify(thread, &f, initialized != 0);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testAggregatesAndTexture
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Fixture f = fixture(thread);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.function->blocks);
  append(thread, f.function->blocks, (Arcadia_Object*)f.entry);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.entry->instructions);
  f.function->returnType = f.voidType;
  f.entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
  Arcadia_VPL_IR_Type* vec4 = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4));
  Arcadia_VPL_IR_Type* vec3 = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 3));
  Arcadia_VPL_IR_Type* vec2 = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 2));
  Arcadia_VPL_IR_Type* sampler = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_SamplerType_create(thread, Arcadia_VPL_IR_SamplerKind_Sampler2D));
  Arcadia_VPL_IR_Type* matrix = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 2, 3));
  Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Light"), list(thread)));
  append(thread, record->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "color"), vec4));
  Arcadia_VPL_IR_Type* array = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, (Arcadia_VPL_IR_Type*)record, 2));
  append(thread, f.module->uniforms, (Arcadia_Object*)Arcadia_VPL_IR_ArrayUniform_create(thread, text(thread, "lights"), (Arcadia_VPL_IR_Type*)record, 2));
  append(thread, f.module->uniforms, (Arcadia_Object*)Arcadia_VPL_IR_ScalarUniform_create(thread, text(thread, "sampler"), sampler));
  Arcadia_VPL_IR_RegisterValue* registers[] = {
    reg(thread, 0, array), reg(thread, 1, (Arcadia_VPL_IR_Type*)record), reg(thread, 2, vec4),
    reg(thread, 3, vec3), reg(thread, 4, vec4), reg(thread, 5, sampler), reg(thread, 6, vec2),
    reg(thread, 7, f.real), reg(thread, 8, matrix), reg(thread, 9, f.boolean)
  };
  for (int i = 0; i < 10; ++i) declare(thread, f.entry, registers[i]);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, registers[0], reference(thread, Arcadia_VPL_IR_StorageKind_Uniform, 0, array)));
  Arcadia_VPL_IR_IndexInstruction* index = (Arcadia_VPL_IR_IndexInstruction*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_IndexInstruction_create(thread, registers[1], (Arcadia_VPL_IR_Value*)registers[0], (Arcadia_VPL_IR_Value*)f.one));
  append(thread, f.entry->instructions, (Arcadia_Object*)index);
  Arcadia_String* colorName = text(thread, "color");
  Arcadia_VPL_IR_MemberInstruction* member = (Arcadia_VPL_IR_MemberInstruction*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MemberInstruction_create(thread, registers[2], (Arcadia_VPL_IR_Value*)registers[1], colorName));
  append(thread, f.entry->instructions, (Arcadia_Object*)member);
  Arcadia_String* pattern = text(thread, "zyx");
  Arcadia_VPL_IR_SwizzleInstruction* swizzle = (Arcadia_VPL_IR_SwizzleInstruction*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_SwizzleInstruction_create(thread, registers[3], (Arcadia_VPL_IR_Value*)registers[2], pattern));
  append(thread, f.entry->instructions, (Arcadia_Object*)swizzle);
  Arcadia_VPL_IR_ConstantValue* one = (Arcadia_VPL_IR_ConstantValue*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.real, Arcadia_Value_makeReal32Value(1.f)));
  Arcadia_List* components = list(thread);
  append(thread, components, (Arcadia_Object*)registers[3]);
  append(thread, components, (Arcadia_Object*)one);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[4], vec4, components));
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, registers[5], reference(thread, Arcadia_VPL_IR_StorageKind_Uniform, 1, sampler)));
  Arcadia_List* coordinates = list(thread);
  append(thread, coordinates, (Arcadia_Object*)one);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[6], vec2, coordinates));
  Arcadia_List* conversion = list(thread);
  append(thread, conversion, (Arcadia_Object*)f.one);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[7], f.real, conversion));
  Arcadia_List* diagonal = list(thread);
  append(thread, diagonal, (Arcadia_Object*)registers[7]);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[8], matrix, diagonal));
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, registers[6], Arcadia_VPL_IR_BinaryOperator_Multiply, (Arcadia_VPL_IR_Value*)registers[8], (Arcadia_VPL_IR_Value*)registers[3]));
  for (Arcadia_Natural8Value op = Arcadia_VPL_IR_BinaryOperator_Equal; op <= Arcadia_VPL_IR_BinaryOperator_GreaterEqual; ++op) {
    append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, registers[9], op, (Arcadia_VPL_IR_Value*)registers[7], (Arcadia_VPL_IR_Value*)one));
  }
  Arcadia_List* arguments = list(thread);
  append(thread, arguments, (Arcadia_Object*)registers[5]);
  append(thread, arguments, (Arcadia_Object*)registers[6]);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_CallInstruction_create(thread, registers[4], NULL, Arcadia_VPL_IR_BuiltinFunction_Texture, arguments));
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, registers[4], Arcadia_VPL_IR_BinaryOperator_Multiply, (Arcadia_VPL_IR_Value*)registers[4], (Arcadia_VPL_IR_Value*)registers[2]));
  Arcadia_VPL_IR_StoreInstruction* store = (Arcadia_VPL_IR_StoreInstruction*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, reference(thread, Arcadia_VPL_IR_StorageKind_StageIO, 0, vec4), (Arcadia_VPL_IR_Value*)registers[4]));
  append(thread, f.entry->instructions, (Arcadia_Object*)store);
  verify(thread, &f, Arcadia_BooleanValue_True);
  index->index = (Arcadia_VPL_IR_Value*)f.two;
  verify(thread, &f, Arcadia_BooleanValue_False);
  index->index = (Arcadia_VPL_IR_Value*)f.one;
  member->fieldName = text(thread, "missing");
  verify(thread, &f, Arcadia_BooleanValue_False);
  member->fieldName = colorName;
  swizzle->pattern = text(thread, "qyx");
  verify(thread, &f, Arcadia_BooleanValue_False);
  swizzle->pattern = pattern;
  Arcadia_List_removeBack(thread, components, 1);
  verify(thread, &f, Arcadia_BooleanValue_False);
  append(thread, components, (Arcadia_Object*)one);
  // The same typed reference becomes read-only if its interface declaration is an input.
  Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)Arcadia_List_getObjectReferenceValueAt(thread, f.module->fragment->interface, 0);
  io->direction = Arcadia_VPL_IR_StageIO_Direction_In;
  io->semantics = Arcadia_VPL_IR_StageIO_Semantics_None;
  verify(thread, &f, Arcadia_BooleanValue_False);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testStorageReferences
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 30; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Fixture f = fixture(thread);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.function->blocks);
    append(thread, f.function->blocks, (Arcadia_Object*)f.entry);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.entry->instructions);
    f.function->returnType = f.voidType;
    f.entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
    Arcadia_VPL_IR_RecordType* cell = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Cell"), list(thread)));
    append(thread, cell->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "value"), f.integer));
    append(thread, cell->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "other"), f.integer));
    Arcadia_VPL_IR_Type* cells = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, (Arcadia_VPL_IR_Type*)cell, 2));
    Arcadia_VPL_IR_RecordType* container = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Container"), list(thread)));
    append(thread, container->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "cells"), cells));
    append(thread, container->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "other"), f.integer));
    Arcadia_VPL_IR_RegisterValue* registers[] = {
      reg(thread, 0, (Arcadia_VPL_IR_Type*)container), reg(thread, 1, f.integer), reg(thread, 2, f.integer),
      reg(thread, 3, (Arcadia_VPL_IR_Type*)cell), reg(thread, 4, cells), reg(thread, 5, f.real)
    };
    for (int i = 0; i < 6; ++i) declare(thread, f.entry, registers[i]);
    Arcadia_List* cellValues = list(thread);
    append(thread, cellValues, (Arcadia_Object*)f.one);
    append(thread, cellValues, (Arcadia_Object*)f.two);
    append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[3], (Arcadia_VPL_IR_Type*)cell, cellValues));
    Arcadia_List* arrayValues = list(thread);
    append(thread, arrayValues, (Arcadia_Object*)registers[3]);
    append(thread, arrayValues, (Arcadia_Object*)registers[3]);
    append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[4], cells, arrayValues));
    Arcadia_List* containerValues = list(thread);
    append(thread, containerValues, (Arcadia_Object*)registers[4]);
    append(thread, containerValues, (Arcadia_Object*)f.two);
    if (test != 19 && test != 20) append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, registers[0], (Arcadia_VPL_IR_Type*)container, containerValues));
    if (test != 18) copy(thread, f.entry, registers[2], (Arcadia_VPL_IR_Value*)f.one);

    Arcadia_Natural8Value kind = Arcadia_VPL_IR_StorageKind_Local;
    if (test == 1) {
      kind = Arcadia_VPL_IR_StorageKind_Parameter;
      append(thread, f.function->parameters, (Arcadia_Object*)reference(thread, kind, 0, (Arcadia_VPL_IR_Type*)container));
    } else if (test == 2 || test == 3 || test == 16) {
      kind = Arcadia_VPL_IR_StorageKind_Uniform;
      append(thread, f.module->uniforms, (Arcadia_Object*)Arcadia_VPL_IR_RecordUniform_create(thread, text(thread, "container"), container));
    } else if (test == 4 || test == 5) {
      kind = Arcadia_VPL_IR_StorageKind_StageIO;
      append(thread, f.module->fragment->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread,
        test == 4 ? Arcadia_VPL_IR_StageIO_Direction_Out : Arcadia_VPL_IR_StageIO_Direction_In,
        text(thread, "container"), (Arcadia_VPL_IR_Type*)container, 1, Arcadia_VPL_IR_StageIO_Semantics_None));
    }
    Arcadia_VPL_IR_ReferenceValue* base = reference(thread, kind, kind == Arcadia_VPL_IR_StorageKind_StageIO ? 1 : 0, (Arcadia_VPL_IR_Type*)container);
    Arcadia_VPL_IR_MemberReferenceValue* arrayPath = (Arcadia_VPL_IR_MemberReferenceValue*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_IR_MemberReferenceValue_create(thread, base, text(thread, "cells"), cells));
    Arcadia_VPL_IR_IndexReferenceValue* elementPath = (Arcadia_VPL_IR_IndexReferenceValue*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_IR_IndexReferenceValue_create(thread, (Arcadia_VPL_IR_ReferenceValue*)arrayPath, (Arcadia_VPL_IR_Value*)registers[2], (Arcadia_VPL_IR_Type*)cell));
    Arcadia_VPL_IR_MemberReferenceValue* fieldPath = (Arcadia_VPL_IR_MemberReferenceValue*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_IR_MemberReferenceValue_create(thread, (Arcadia_VPL_IR_ReferenceValue*)elementPath, text(thread, "value"), f.integer));
    Arcadia_Tests_assertTrue(thread, fieldPath->_parent.storageKind == Arcadia_VPL_IR_StorageKind_Member && fieldPath->_parent.slot == 0 && fieldPath->_parent._parent.kind == Arcadia_VPL_IR_ValueKind_Reference);
    Arcadia_Tests_assertTrue(thread, elementPath->_parent.storageKind == Arcadia_VPL_IR_StorageKind_Index && elementPath->_parent.slot == 0);
    Arcadia_VPL_IR_ReferenceValue* target = (Arcadia_VPL_IR_ReferenceValue*)fieldPath;
    Arcadia_VPL_IR_Value* value = (Arcadia_VPL_IR_Value*)f.two;
    switch (test) {
      case 6: fieldPath->fieldName = text(thread, "missing"); break;
      case 7: fieldPath->base = reference(thread, Arcadia_VPL_IR_StorageKind_Local, 1, f.integer); break;
      case 8: elementPath->base = reference(thread, Arcadia_VPL_IR_StorageKind_Local, 1, f.integer); break;
      case 9: elementPath->index = (Arcadia_VPL_IR_Value*)f.yes; break;
      case 10:
        elementPath->index = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.integer, Arcadia_Value_makeInteger32Value(-1)));
        break;
      case 11: elementPath->index = (Arcadia_VPL_IR_Value*)f.two; break;
      case 12: fieldPath->_parent.type = f.real; break;
      case 13: elementPath->_parent.type = (Arcadia_VPL_IR_Type*)container; break;
      case 14: elementPath->base = NULL; break;
      case 15: elementPath->base = (Arcadia_VPL_IR_ReferenceValue*)elementPath; break;
      case 16:
        fieldPath->_parent.storageKind = Arcadia_VPL_IR_StorageKind_Local;
        fieldPath->_parent.slot = 1;
        break;
      case 17: target = reference(thread, Arcadia_VPL_IR_StorageKind_Member, 0, f.integer); break;
      case 21: fieldPath->_parent.slot = 1; break;
      case 22: arrayPath->base = (Arcadia_VPL_IR_ReferenceValue*)fieldPath; break;
      case 23: elementPath->_parent.storageKind = Arcadia_VPL_IR_StorageKind_Member; break;
      case 24:
        fieldPath->_parent.storageKind = Arcadia_VPL_IR_StorageKind_Parameter;
        append(thread, f.function->parameters, (Arcadia_Object*)fieldPath);
        break;
      case 25: fieldPath->_parent.type = f.voidType; break;
      case 26: fieldPath->fieldName = text(thread, ""); break;
      case 27: elementPath->index = (Arcadia_VPL_IR_Value*)reference(thread, Arcadia_VPL_IR_StorageKind_Local, 2, f.integer); break;
      case 29:
        value = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.real, Arcadia_Value_makeReal32Value(2.f)));
        break;
    }
    if (test != 2 && test != 20 && test != 24) append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, target, value));
    if (test != 19 && test != 24) append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, registers[test == 28 ? 5 : 1], target));
    Arcadia_BooleanValue valid = test <= 4 && test != 3;
    if (valid) {
      // Reuse the descriptor after a write to its index register; no address is frozen at creation.
      Arcadia_VPL_IR_Value* zero = (Arcadia_VPL_IR_Value*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, f.integer, Arcadia_Value_makeInteger32Value(0)));
      copy(thread, f.entry, registers[2], zero);
      append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, registers[1], target));
    }
    verify(thread, &f, valid);
    if (valid) verify(thread, &f, valid);
    if (test == 15) {
      elementPath->base = (Arcadia_VPL_IR_ReferenceValue*)arrayPath;
      verify(thread, &f, Arcadia_BooleanValue_True);
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
assertConstructorRejects
  (
    Arcadia_Thread* thread,
    Arcadia_Object* object,
    Arcadia_Type* type,
    Arcadia_Value* arguments,
    Arcadia_Natural8Value count,
    Arcadia_Status expected
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
    Arcadia_ValueStack_pushNatural8Value(thread, count);
    // Check the callback's diagnostic rather than create's failed-stack-frame status.
    Arcadia_Type_getOperations(type)->objectTypeOperations->construct(thread, object);
  } else raised = Arcadia_BooleanValue_True;
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_setStatus(thread, 0);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  Arcadia_Object_setType(thread, object, type);
  Arcadia_Tests_assertTrue(thread, raised && status == expected);
}

static void
testReferenceConstructors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Fixture f = fixture(thread);
  Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Cell"), list(thread)));
  Arcadia_String* fieldName = text(thread, "value");
  append(thread, record->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, fieldName, f.integer));
  Arcadia_VPL_IR_Type* array = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, f.integer, 2));
  Arcadia_VPL_IR_ReferenceValue* recordRef = reference(thread, Arcadia_VPL_IR_StorageKind_Local, 0, (Arcadia_VPL_IR_Type*)record);
  Arcadia_VPL_IR_ReferenceValue* arrayRef = reference(thread, Arcadia_VPL_IR_StorageKind_Local, 1, array);
  Arcadia_Object* samples[] = {
    root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MemberReferenceValue_create(thread, recordRef, fieldName, f.integer)),
    root(thread, (Arcadia_Object*)Arcadia_VPL_IR_IndexReferenceValue_create(thread, arrayRef, (Arcadia_VPL_IR_Value*)f.one, f.integer))
  };
  Arcadia_Type* types[] = { _Arcadia_VPL_IR_MemberReferenceValue_getType(thread), _Arcadia_VPL_IR_IndexReferenceValue_getType(thread) };
  Arcadia_Value arguments[][3] = {
    { Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)recordRef), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)fieldName), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)f.integer) },
    { Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)arrayRef), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)f.one), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)f.integer) }
  };
  for (int i = 0; i < 2; ++i) {
    assertConstructorRejects(thread, samples[i], types[i], arguments[i], 0, Arcadia_Status_NumberOfArgumentsInvalid);
    for (int j = 0; j < 3; ++j) {
      Arcadia_Value saved = arguments[i][j];
      arguments[i][j] = Arcadia_Value_makeObjectReferenceValue(NULL);
      assertConstructorRejects(thread, samples[i], types[i], arguments[i], 3, Arcadia_Status_ArgumentValueInvalid);
      arguments[i][j] = Arcadia_Value_makeNatural8Value(0);
      assertConstructorRejects(thread, samples[i], types[i], arguments[i], 3, Arcadia_Status_ArgumentTypeInvalid);
      arguments[i][j] = saved;
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testNestedArrayReferences
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Fixture f = fixture(thread);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.function->blocks);
  append(thread, f.function->blocks, (Arcadia_Object*)f.entry);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.entry->instructions);
  f.function->returnType = f.voidType;
  f.entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
  Arcadia_VPL_IR_Type* inner = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, f.integer, 2));
  Arcadia_VPL_IR_Type* outer = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, inner, 2));
  Arcadia_VPL_IR_RegisterValue* array = reg(thread, 0, outer);
  Arcadia_VPL_IR_RegisterValue* row = reg(thread, 1, inner);
  Arcadia_VPL_IR_RegisterValue* result = reg(thread, 2, f.integer);
  declare(thread, f.entry, array);
  declare(thread, f.entry, row);
  declare(thread, f.entry, result);
  Arcadia_List* rowValues = list(thread);
  append(thread, rowValues, (Arcadia_Object*)f.one);
  append(thread, rowValues, (Arcadia_Object*)f.two);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, row, inner, rowValues));
  Arcadia_List* rows = list(thread);
  append(thread, rows, (Arcadia_Object*)row);
  append(thread, rows, (Arcadia_Object*)row);
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_ConstructInstruction_create(thread, array, outer, rows));
  Arcadia_VPL_IR_ReferenceValue* base = reference(thread, Arcadia_VPL_IR_StorageKind_Local, 0, outer);
  Arcadia_VPL_IR_IndexReferenceValue* first = (Arcadia_VPL_IR_IndexReferenceValue*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_IR_IndexReferenceValue_create(thread, base, (Arcadia_VPL_IR_Value*)f.one, inner));
  Arcadia_VPL_IR_IndexReferenceValue* second = (Arcadia_VPL_IR_IndexReferenceValue*)root(thread,
    (Arcadia_Object*)Arcadia_VPL_IR_IndexReferenceValue_create(thread, (Arcadia_VPL_IR_ReferenceValue*)first, (Arcadia_VPL_IR_Value*)f.one, f.integer));
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_StoreInstruction_create(thread, (Arcadia_VPL_IR_ReferenceValue*)second, (Arcadia_VPL_IR_Value*)f.two));
  append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, result, (Arcadia_VPL_IR_ReferenceValue*)second));
  verify(thread, &f, Arcadia_BooleanValue_True);
  second->_parent.type = f.real;
  verify(thread, &f, Arcadia_BooleanValue_False);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static void
testStageEntries
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 12; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Fixture f = fixture(thread);
    Arcadia_BooleanValue valid = Arcadia_BooleanValue_False;
    Arcadia_VPL_IR_Stage* stage = f.module->fragment;
    switch (test) {
      case 0: valid = Arcadia_BooleanValue_True; break; // Explicit entry is not the first function.
      case 1: f.module->vertex->entryFunction = NULL; break;
      case 2: stage->entryFunction = NULL; break;
      case 3: stage->entryFunction = entryFunction(thread, f.voidType, "foreign"); break;
      case 4: stage->entryFunction = f.module->vertex->entryFunction; break;
      case 5: stage->entryFunction = f.function; break; // Valid int-returning helper is not an entry.
      case 6:
        append(thread, stage->entryFunction->parameters, (Arcadia_Object*)reference(thread, Arcadia_VPL_IR_StorageKind_Parameter, 0, f.integer));
        break;
      case 7: stage->entryFunction = (Arcadia_VPL_IR_Function*)text(thread, "not a function"); break;
      case 8: Arcadia_Collection_clear(thread, (Arcadia_Collection*)stage->entryFunction->blocks); break;
      case 9:
        stage->entryFunction->name = text(thread, "main");
        stage->entryFunction = NULL; // A name does not imply entry-point selection.
        break;
      case 10: {
        Arcadia_VPL_IR_Function* replacement = entryFunction(thread, f.voidType, "explicit_replacement");
        append(thread, stage->functions, (Arcadia_Object*)replacement);
        stage->entryFunction = replacement;
        valid = Arcadia_BooleanValue_True;
      } break;
      case 11:
        stage->entryFunction = f.module->vertex->entryFunction;
        append(thread, stage->functions, (Arcadia_Object*)stage->entryFunction); // Cannot share a function's blocks across stages.
        break;
    }
    verify(thread, &f, valid);
    if (test == 2) {
      stage->entryFunction = (Arcadia_VPL_IR_Function*)Arcadia_List_getObjectReferenceValueAt(thread, stage->functions, 1);
      verify(thread, &f, Arcadia_BooleanValue_True);
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testInterfaceLinks
  (
    Arcadia_Thread* thread
  )
{
  for (int test = 0; test < 30; ++test) {
    Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
    Fixture f = fixture(thread);
    Arcadia_VPL_IR_Type* vec4 = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4));
    Arcadia_VPL_IR_StageIO* output = (Arcadia_VPL_IR_StageIO*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, text(thread, "vertex_value"), vec4, 2, Arcadia_VPL_IR_StageIO_Semantics_Color));
    Arcadia_VPL_IR_StageIO* input = (Arcadia_VPL_IR_StageIO*)root(thread,
      (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_In, text(thread, "fragment_value"), vec4, 2, Arcadia_VPL_IR_StageIO_Semantics_Color));
    append(thread, f.module->vertex->interface, (Arcadia_Object*)output);
    append(thread, f.module->fragment->interface, (Arcadia_Object*)input);
    Arcadia_BooleanValue valid = Arcadia_BooleanValue_False;
    switch (test) {
      case 0: valid = Arcadia_BooleanValue_True; break; // Explicit location; names may differ.
      case 1:
        output->location = -1;
        input->location = -1;
        input->name = output->name;
        valid = Arcadia_BooleanValue_True;
        break;
      case 2: Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.module->vertex->interface); break;
      case 3: input->location = 3; input->name = output->name; break; // Never fall back to names.
      case 4: output->location = -1; input->name = output->name; break;
      case 5: input->location = -1; input->name = output->name; break;
      case 6: output->location = -1; input->location = -1; break;
      case 7:
        input->type = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 3));
        break;
      case 8: input->semantics = Arcadia_VPL_IR_StageIO_Semantics_TexCoord; break;
      case 9: input->semantics = Arcadia_VPL_IR_StageIO_Semantics_None; valid = Arcadia_BooleanValue_True; break;
      case 10: output->semantics = Arcadia_VPL_IR_StageIO_Semantics_None; valid = Arcadia_BooleanValue_True; break;
      case 11:
        append(thread, f.module->vertex->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, text(thread, "duplicate_location"), vec4, 2, Arcadia_VPL_IR_StageIO_Semantics_None));
        break;
      case 12:
        append(thread, f.module->fragment->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_In, text(thread, "duplicate_location"), vec4, 2, Arcadia_VPL_IR_StageIO_Semantics_None));
        break;
      case 13: output->direction = Arcadia_VPL_IR_StageIO_Direction_In; break;
      case 14:
        Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.module->vertex->interface);
        append(thread, f.module->vertex->interface, (Arcadia_Object*)f.positionOutput);
        input->semantics = Arcadia_VPL_IR_StageIO_Semantics_Position;
        input->location = -1;
        valid = Arcadia_BooleanValue_True; // Fragment Position is a built-in, not a varying match.
        break;
      case 15:
        output->semantics = Arcadia_VPL_IR_StageIO_Semantics_Position;
        output->location = -1; // A built-in output cannot supply an ordinary input.
        break;
      case 16:
        input->semantics = Arcadia_VPL_IR_StageIO_Semantics_Position;
        input->location = -1;
        input->type = f.real;
        break;
      case 17: input->semantics = Arcadia_VPL_IR_StageIO_Semantics_Position; break; // Built-in has a user location.
      case 18: input->semantics = Arcadia_VPL_IR_StageIO_Semantics_FragmentColor; break; // Cannot be an input.
      case 19:
        Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.module->fragment->interface);
        append(thread, f.module->fragment->interface, (Arcadia_Object*)f.colorOutput);
        valid = Arcadia_BooleanValue_True; // Extra vertex outputs are permitted.
        break;
      case 20:
      case 21: {
        output->type = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, f.integer, 2));
        input->type = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, f.integer, test == 20 ? 2 : 3));
        output->semantics = input->semantics = Arcadia_VPL_IR_StageIO_Semantics_None;
        valid = test == 20;
      } break;
      case 22:
      case 23: {
        Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Packet"), list(thread)));
        append(thread, record->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "value"), f.integer));
        output->type = (Arcadia_VPL_IR_Type*)record;
        input->type = output->type;
        if (test == 23) {
          Arcadia_VPL_IR_RecordType* other = (Arcadia_VPL_IR_RecordType*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_RecordType_create(thread, text(thread, "Packet"), list(thread)));
          append(thread, other->fields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, text(thread, "value"), f.integer));
          input->type = (Arcadia_VPL_IR_Type*)other;
        }
        output->semantics = input->semantics = Arcadia_VPL_IR_StageIO_Semantics_None;
        valid = test == 22;
      } break;
      case 24:
        output->location = input->location = -1;
        input->name = output->name;
        append(thread, f.module->vertex->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, output->name, vec4, -1, Arcadia_VPL_IR_StageIO_Semantics_None));
        break;
      case 25:
        output->location = input->location = -1;
        input->name = output->name;
        output->semantics = Arcadia_VPL_IR_StageIO_Semantics_TexCoord;
        break;
      case 26:
        input->direction = Arcadia_VPL_IR_StageIO_Direction_Out;
        input->semantics = Arcadia_VPL_IR_StageIO_Semantics_None;
        valid = Arcadia_BooleanValue_True; // Fragment outputs are not linked to vertex outputs.
        break;
      case 27:
        append(thread, f.module->vertex->interface, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_In, text(thread, "external_attribute"), f.real, 2, Arcadia_VPL_IR_StageIO_Semantics_None));
        valid = Arcadia_BooleanValue_True; // Input/output location namespaces are distinct.
        break;
      case 28:
        input->type = (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4));
        valid = Arcadia_BooleanValue_True; // Structural vector equality, not pointer equality.
        break;
      case 29: output->semantics = Arcadia_VPL_IR_StageIO_Semantics_Position; break;
    }
    verify(thread, &f, valid);
    if (test == 7) {
      input->type = output->type;
      verify(thread, &f, Arcadia_BooleanValue_True);
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
  }
}

static void
testBinaryConformanceTable
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_IR_Type* types[] = {
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Boolean)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Integer)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 2)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 3)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Integer, 3)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Boolean, 3)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 2, 3)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 3, 4)),
    (Arcadia_VPL_IR_Type*)root(thread, (Arcadia_Object*)Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 2, 4))
  };

  // Masks enumerate accepted operations in the specification's complete operand table.
  static const struct {
    int lhs;
    int rhs;
    int arithmeticResult;
    unsigned int allowed;
  } rows[] = {
    { 0, 0, 0, (1u << 4) | (1u << 5) },
    { 1, 1, 1, 0x3ffu },
    { 2, 2, 2, 0x3ffu },
    { 1, 2, 2, 0 },
    { 0, 1, 1, 0 },
    { 4, 4, 4, 0x0fu },
    { 5, 5, 5, 0x0fu },
    { 6, 6, 6, 0 },
    { 4, 2, 4, (1u << 2) | (1u << 3) },
    { 2, 4, 4, 1u << 2 },
    { 5, 1, 5, 0 },
    { 7, 7, 7, (1u << 0) | (1u << 1) | (1u << 3) },
    { 7, 8, 9, 1u << 2 },
    { 7, 4, 3, 1u << 2 },
    { 7, 3, 3, 0 },
    { 4, 7, 4, 0 },
    { 7, 2, 7, (1u << 2) | (1u << 3) },
    { 2, 7, 7, 1u << 2 }
  };

  Arcadia_SizeValue typeRoots = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue row = 0; row < sizeof(rows) / sizeof(rows[0]); ++row) {
    for (Arcadia_Natural8Value op = 0; op < 10; ++op) {
      Fixture f = fixture(thread);
      Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.function->blocks);
      append(thread, f.function->blocks, (Arcadia_Object*)f.entry);
      Arcadia_Collection_clear(thread, (Arcadia_Collection*)f.entry->instructions);
      Arcadia_VPL_IR_Type* resultType = types[op >= Arcadia_VPL_IR_BinaryOperator_Equal ? 0 : rows[row].arithmeticResult];
      f.function->returnType = resultType;
      Arcadia_VPL_IR_ReferenceValue* lhsParameter = reference(thread, Arcadia_VPL_IR_StorageKind_Parameter, 0, types[rows[row].lhs]);
      Arcadia_VPL_IR_ReferenceValue* rhsParameter = reference(thread, Arcadia_VPL_IR_StorageKind_Parameter, 1, types[rows[row].rhs]);
      append(thread, f.function->parameters, (Arcadia_Object*)lhsParameter);
      append(thread, f.function->parameters, (Arcadia_Object*)rhsParameter);
      Arcadia_VPL_IR_RegisterValue* lhs = reg(thread, 0, types[rows[row].lhs]);
      Arcadia_VPL_IR_RegisterValue* rhs = reg(thread, 1, types[rows[row].rhs]);
      Arcadia_VPL_IR_RegisterValue* result = reg(thread, 2, resultType);
      declare(thread, f.entry, lhs); declare(thread, f.entry, rhs); declare(thread, f.entry, result);
      append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, lhs, lhsParameter));
      append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_LoadInstruction_create(thread, rhs, rhsParameter));
      append(thread, f.entry->instructions, (Arcadia_Object*)Arcadia_VPL_IR_BinaryInstruction_create(thread, result, op, (Arcadia_VPL_IR_Value*)lhs, (Arcadia_VPL_IR_Value*)rhs));
      f.entry->terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, (Arcadia_VPL_IR_Value*)result);
      verify(thread, &f, (rows[row].allowed & (1u << op)) != 0);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - typeRoots);
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
}

static int
runGraphs
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testGraphs) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runLoops
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testLoops) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAggregatesAndTexture
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAggregatesAndTexture) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runStorageReferences
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testStorageReferences) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNestedArrayReferences
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNestedArrayReferences) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runReferenceConstructors
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testReferenceConstructors) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runStageEntries
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testStageEntries) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runInterfaceLinks
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterfaceLinks) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runBinaryConformanceTable
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testBinaryConformanceTable) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Graphs", &runGraphs },
    { "Loops", &runLoops },
    { "AggregatesAndTexture", &runAggregatesAndTexture },
    { "StorageReferences", &runStorageReferences },
    { "NestedArrayReferences", &runNestedArrayReferences },
    { "ReferenceConstructors", &runReferenceConstructors },
    { "StageEntries", &runStageEntries },
    { "InterfaceLinks", &runInterfaceLinks },
    { "BinaryConformanceTable", &runBinaryConformanceTable },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
