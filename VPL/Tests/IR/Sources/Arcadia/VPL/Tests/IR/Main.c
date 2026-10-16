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

static Arcadia_String*
createString
  (
    Arcadia_Thread* thread,
    const char* cxxString
  )
{
  return Arcadia_String_createFromCxxString(thread, cxxString);
}

static Arcadia_List*
createList
  (
    Arcadia_Thread* thread
  )
{
  return (Arcadia_List*)Arcadia_ArrayList_create(thread);
}

static Arcadia_VPL_IR_VectorType*
createVector3
  (
    Arcadia_Thread* thread
  )
{
  return Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 3);
}

static Arcadia_VPL_IR_VectorType*
createVector4
  (
    Arcadia_Thread* thread
  )
{
  return Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4);
}

static void
assertConstructorRaises
  (
    Arcadia_Thread* thread,
    void (*body)(Arcadia_Thread* thread)
  )
{
  Arcadia_BooleanValue raised = Arcadia_BooleanValue_False;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    body(thread);
  } else {
    raised = Arcadia_BooleanValue_True;
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Tests_assertTrue(thread, raised == Arcadia_BooleanValue_True);
  Arcadia_Thread_setStatus(thread, 0);
}

static void
testTypeFamily
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VoidType* voidType = Arcadia_VPL_IR_VoidType_create(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TypeKind_Void == ((Arcadia_VPL_IR_Type*)voidType)->kind);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)voidType, _Arcadia_VPL_IR_Type_getType(thread)) == Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)voidType, _Arcadia_Object_getType(thread)) == Arcadia_BooleanValue_True);

  Arcadia_VPL_IR_VectorType* vec4 = createVector4(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TypeKind_Vector == ((Arcadia_VPL_IR_Type*)vec4)->kind);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_ScalarElementKind_Floating == vec4->elementKind);
  Arcadia_Tests_assertTrue(thread, 4 == vec4->size);

  Arcadia_VPL_IR_MatrixType* mat4 = Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 4, 4);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TypeKind_Matrix == ((Arcadia_VPL_IR_Type*)mat4)->kind);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_ScalarElementKind_Floating == mat4->elementKind);
  Arcadia_Tests_assertTrue(thread, 4 == mat4->rows);
  Arcadia_Tests_assertTrue(thread, 4 == mat4->columns);

  Arcadia_VPL_IR_SamplerType* sampler = Arcadia_VPL_IR_SamplerType_create(thread, Arcadia_VPL_IR_SamplerKind_Sampler2D);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TypeKind_Sampler == ((Arcadia_VPL_IR_Type*)sampler)->kind);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_SamplerKind_Sampler2D == sampler->samplerKind);

  Arcadia_List* recordFields = createList(thread);
  Arcadia_VPL_IR_VectorType* vec2 = Arcadia_VPL_IR_VectorType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, 2);
  Arcadia_List_insertBackObjectReferenceValue(thread, recordFields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, createString(thread, "position"), (Arcadia_VPL_IR_Type*)vec2));
  Arcadia_List_insertBackObjectReferenceValue(thread, recordFields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, createString(thread, "color"), (Arcadia_VPL_IR_Type*)vec4));
  Arcadia_VPL_IR_RecordType* record = Arcadia_VPL_IR_RecordType_create(thread, createString(thread, "material"), recordFields);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TypeKind_Record == ((Arcadia_VPL_IR_Type*)record)->kind);
  Arcadia_Tests_assertTrue(thread, NULL != record->name);
  Arcadia_Tests_assertTrue(thread, 2 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)record->fields));

  Arcadia_VPL_IR_ArrayType* array = Arcadia_VPL_IR_ArrayType_create(thread, (Arcadia_VPL_IR_Type*)vec4, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TypeKind_Array == ((Arcadia_VPL_IR_Type*)array)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == array->element);
  Arcadia_Tests_assertTrue(thread, (Arcadia_SizeValue)3 == array->count);
}

static void
testValueFamily
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VectorType* vec4 = createVector4(thread);

  Arcadia_VPL_IR_RegisterValue* reg = Arcadia_VPL_IR_RegisterValue_create(thread, (Arcadia_SizeValue)7, (Arcadia_VPL_IR_Type*)vec4);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_ValueKind_Register == ((Arcadia_VPL_IR_Value*)reg)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_SizeValue)7 == reg->index);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == reg->type);

  Arcadia_VPL_IR_ConstantValue* constant = Arcadia_VPL_IR_ConstantValue_create(thread, (Arcadia_VPL_IR_Type*)vec4, Arcadia_Value_makeReal32Value(1.5f));
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_ValueKind_Constant == ((Arcadia_VPL_IR_Value*)constant)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == constant->type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isReal32Value(&constant->literal));
  Arcadia_Tests_assertTrue(thread, 1.5f == Arcadia_Value_getReal32Value(&constant->literal));

  Arcadia_VPL_IR_ReferenceValue* reference = Arcadia_VPL_IR_ReferenceValue_create(thread, (Arcadia_SizeValue)3, (Arcadia_VPL_IR_Type*)vec4);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_ValueKind_Reference == ((Arcadia_VPL_IR_Value*)reference)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_SizeValue)3 == reference->slot);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == reference->type);
}

static void
testInstructionFamily
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VectorType* vec3 = createVector3(thread);
  Arcadia_VPL_IR_VectorType* vec4 = createVector4(thread);
  Arcadia_VPL_IR_RegisterValue* r0 = Arcadia_VPL_IR_RegisterValue_create(thread, (Arcadia_SizeValue)0, (Arcadia_VPL_IR_Type*)vec3);
  Arcadia_VPL_IR_RegisterValue* r1 = Arcadia_VPL_IR_RegisterValue_create(thread, (Arcadia_SizeValue)1, (Arcadia_VPL_IR_Type*)vec3);
  Arcadia_VPL_IR_ReferenceValue* ref = Arcadia_VPL_IR_ReferenceValue_create(thread, (Arcadia_SizeValue)0, (Arcadia_VPL_IR_Type*)vec3);

  Arcadia_VPL_IR_DeclareLocalInstruction* decl = Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, r0, (Arcadia_VPL_IR_Type*)vec3);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_DeclareLocal == ((Arcadia_VPL_IR_Instruction*)decl)->kind);
  Arcadia_Tests_assertTrue(thread, r0 == decl->registerValue);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec3 == decl->type);

  Arcadia_VPL_IR_LoadInstruction* load = Arcadia_VPL_IR_LoadInstruction_create(thread, r1, ref);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Load == ((Arcadia_VPL_IR_Instruction*)load)->kind);
  Arcadia_Tests_assertTrue(thread, r1 == load->result);
  Arcadia_Tests_assertTrue(thread, ref == load->source);

  Arcadia_VPL_IR_StoreInstruction* store = Arcadia_VPL_IR_StoreInstruction_create(thread, ref, (Arcadia_VPL_IR_Value*)r1);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Store == ((Arcadia_VPL_IR_Instruction*)store)->kind);
  Arcadia_Tests_assertTrue(thread, ref == store->target);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Value*)r1 == store->value);

  Arcadia_VPL_IR_BinaryInstruction* binary = Arcadia_VPL_IR_BinaryInstruction_create(thread, r0, Arcadia_VPL_IR_BinaryOperator_Add, (Arcadia_VPL_IR_Value*)r0, (Arcadia_VPL_IR_Value*)r1);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Binary == ((Arcadia_VPL_IR_Instruction*)binary)->kind);
  Arcadia_Tests_assertTrue(thread, r0 == binary->result);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_BinaryOperator_Add == binary->operatorKind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Value*)r0 == binary->lhs);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Value*)r1 == binary->rhs);

  Arcadia_List* callArguments = createList(thread);
  Arcadia_Value integerArgument = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, (Arcadia_VPL_IR_Type*)vec4, Arcadia_Value_makeReal32Value(0.5f)));
  Arcadia_List_insertBack(thread, callArguments, integerArgument);
  Arcadia_VPL_IR_CallInstruction* call = Arcadia_VPL_IR_CallInstruction_create(thread, r0, NULL, Arcadia_VPL_IR_BuiltinFunction_Texture, callArguments);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Call == ((Arcadia_VPL_IR_Instruction*)call)->kind);
  Arcadia_Tests_assertTrue(thread, r0 == call->result);
  Arcadia_Tests_assertTrue(thread, NULL == call->calleeFunction);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_BuiltinFunction_Texture == call->builtin);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)call->arguments));

  Arcadia_List* constructArguments = createList(thread);
  Arcadia_VPL_IR_ConstructInstruction* construct = Arcadia_VPL_IR_ConstructInstruction_create(thread, r0, (Arcadia_VPL_IR_Type*)vec4, constructArguments);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Construct == ((Arcadia_VPL_IR_Instruction*)construct)->kind);
  Arcadia_Tests_assertTrue(thread, r0 == construct->result);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == construct->type);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)construct->arguments));

  Arcadia_VPL_IR_SwizzleInstruction* swizzle = Arcadia_VPL_IR_SwizzleInstruction_create(thread, r0, (Arcadia_VPL_IR_Value*)r1, createString(thread, "xyz"));
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Swizzle == ((Arcadia_VPL_IR_Instruction*)swizzle)->kind);
  Arcadia_Tests_assertTrue(thread, 3 == Arcadia_String_getNumberOfBytes(thread, swizzle->pattern));

  Arcadia_VPL_IR_MemberInstruction* member = Arcadia_VPL_IR_MemberInstruction_create(thread, r0, (Arcadia_VPL_IR_Value*)r1, createString(thread, "position"));
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Member == ((Arcadia_VPL_IR_Instruction*)member)->kind);
  Arcadia_Tests_assertTrue(thread, 8 == Arcadia_String_getNumberOfBytes(thread, member->fieldName));

  Arcadia_VPL_IR_ConstantValue* indexConstant = Arcadia_VPL_IR_ConstantValue_create(thread, (Arcadia_VPL_IR_Type*)vec3, Arcadia_Value_makeNatural8Value(2));
  Arcadia_VPL_IR_IndexInstruction* index = Arcadia_VPL_IR_IndexInstruction_create(thread, r0, (Arcadia_VPL_IR_Value*)r1, (Arcadia_VPL_IR_Value*)indexConstant);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Index == ((Arcadia_VPL_IR_Instruction*)index)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Value*)indexConstant == index->index);

  Arcadia_VPL_IR_CopyInstruction* copy = Arcadia_VPL_IR_CopyInstruction_create(thread, r0, (Arcadia_VPL_IR_Value*)r1);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_InstructionKind_Copy == ((Arcadia_VPL_IR_Instruction*)copy)->kind);
  Arcadia_Tests_assertTrue(thread, r0 == copy->result && (Arcadia_VPL_IR_Value*)r1 == copy->source);
}

static void
testTerminatorsAndBlocks
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VectorType* vec3 = createVector3(thread);

  Arcadia_VPL_IR_Block* target = Arcadia_VPL_IR_Block_create(thread, createList(thread), NULL);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)target->instructions));
  Arcadia_Tests_assertTrue(thread, NULL == target->terminator);

  Arcadia_VPL_IR_BranchTerminator* branch = Arcadia_VPL_IR_BranchTerminator_create(thread, target);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TerminatorKind_Branch == ((Arcadia_VPL_IR_Terminator*)branch)->kind);
  Arcadia_Tests_assertTrue(thread, target == branch->target);

  Arcadia_VPL_IR_RegisterValue* condition = Arcadia_VPL_IR_RegisterValue_create(thread, (Arcadia_SizeValue)2, (Arcadia_VPL_IR_Type*)vec3);
  Arcadia_VPL_IR_Block* trueTarget = Arcadia_VPL_IR_Block_create(thread, createList(thread), NULL);
  Arcadia_VPL_IR_Block* falseTarget = Arcadia_VPL_IR_Block_create(thread, createList(thread), NULL);
  Arcadia_VPL_IR_ConditionalBranchTerminator* conditional = Arcadia_VPL_IR_ConditionalBranchTerminator_create(thread, (Arcadia_VPL_IR_Value*)condition, trueTarget, falseTarget);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TerminatorKind_ConditionalBranch == ((Arcadia_VPL_IR_Terminator*)conditional)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Value*)condition == conditional->condition);
  Arcadia_Tests_assertTrue(thread, trueTarget == conditional->trueTarget);
  Arcadia_Tests_assertTrue(thread, falseTarget == conditional->falseTarget);

  Arcadia_VPL_IR_ReturnTerminator* returnVoid = Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TerminatorKind_Return == ((Arcadia_VPL_IR_Terminator*)returnVoid)->kind);
  Arcadia_Tests_assertTrue(thread, NULL == returnVoid->value);
  Arcadia_VPL_IR_DiscardTerminator* discard = Arcadia_VPL_IR_DiscardTerminator_create(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TerminatorKind_Discard == ((Arcadia_VPL_IR_Terminator*)discard)->kind);

  Arcadia_VPL_IR_RegisterValue* result = Arcadia_VPL_IR_RegisterValue_create(thread, (Arcadia_SizeValue)3, (Arcadia_VPL_IR_Type*)vec3);
  Arcadia_VPL_IR_ReturnTerminator* returnValue = Arcadia_VPL_IR_ReturnTerminator_create(thread, (Arcadia_VPL_IR_Value*)result);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_TerminatorKind_Return == ((Arcadia_VPL_IR_Terminator*)returnValue)->kind);
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Value*)result == returnValue->value);

  Arcadia_List* instructions = createList(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, instructions, (Arcadia_Object*)Arcadia_VPL_IR_CopyInstruction_create(thread, result, (Arcadia_VPL_IR_Value*)condition));
  Arcadia_VPL_IR_Block* block = Arcadia_VPL_IR_Block_create(thread, instructions, (Arcadia_VPL_IR_Terminator*)returnValue);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)block->instructions));
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Terminator*)returnValue == block->terminator);
}

static void
testFunctionStageModule
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VoidType* voidType = Arcadia_VPL_IR_VoidType_create(thread);
  Arcadia_VPL_IR_VectorType* vec3 = createVector3(thread);
  Arcadia_VPL_IR_VectorType* vec4 = createVector4(thread);

  Arcadia_List* parameters = createList(thread);
  Arcadia_VPL_IR_ReferenceValue* parameter = Arcadia_VPL_IR_ReferenceValue_create(thread, (Arcadia_SizeValue)0, (Arcadia_VPL_IR_Type*)vec3);
  Arcadia_List_insertBackObjectReferenceValue(thread, parameters, (Arcadia_Object*)parameter);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)parameters));

  Arcadia_List* blocks = createList(thread);
  Arcadia_VPL_IR_ReturnTerminator* returnVoid = Arcadia_VPL_IR_ReturnTerminator_create(thread, NULL);
  Arcadia_VPL_IR_Block* block = Arcadia_VPL_IR_Block_create(thread, createList(thread), (Arcadia_VPL_IR_Terminator*)returnVoid);
  Arcadia_List_insertBackObjectReferenceValue(thread, blocks, (Arcadia_Object*)block);
  Arcadia_VPL_IR_Function* function = Arcadia_VPL_IR_Function_create(thread, createString(thread, "main"), (Arcadia_VPL_IR_Type*)voidType, parameters, blocks);
  Arcadia_Tests_assertTrue(thread, NULL != function->name);
  Arcadia_Tests_assertTrue(thread, 4 == Arcadia_String_getNumberOfBytes(thread, function->name));
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)voidType == function->returnType);
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)function->parameters));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)function->blocks));

  Arcadia_List* vertexInterface = createList(thread);
  Arcadia_VPL_IR_StageIO* position = Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, createString(thread, "position"), (Arcadia_VPL_IR_Type*)vec4, (Arcadia_Integer32Value)0, Arcadia_VPL_IR_StageIO_Semantics_Position);
  Arcadia_VPL_IR_StageIO* color = Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, createString(thread, "color"), (Arcadia_VPL_IR_Type*)vec4, (Arcadia_Integer32Value)1, Arcadia_VPL_IR_StageIO_Semantics_Color);
  Arcadia_List_insertBackObjectReferenceValue(thread, vertexInterface, (Arcadia_Object*)position);
  Arcadia_List_insertBackObjectReferenceValue(thread, vertexInterface, (Arcadia_Object*)color);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_StageIO_Direction_Out == position->direction);
  Arcadia_Tests_assertTrue(thread, (Arcadia_Integer32Value)0 == position->location);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_StageIO_Semantics_Position == position->semantics);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_StageIO_Semantics_Color == color->semantics);

  Arcadia_List* vertexFunctions = createList(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, vertexFunctions, (Arcadia_Object*)function);
  Arcadia_VPL_IR_Stage* vertex = Arcadia_VPL_IR_Stage_create(thread, Arcadia_VPL_IR_Stage_Vertex, vertexInterface, vertexFunctions);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_Stage_Vertex == vertex->kind);
  Arcadia_Tests_assertTrue(thread, 2 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)vertex->interface));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)vertex->functions));

  Arcadia_List* fragmentInterface = createList(thread);
  Arcadia_VPL_IR_StageIO* fragmentColor = Arcadia_VPL_IR_StageIO_create(thread, Arcadia_VPL_IR_StageIO_Direction_Out, createString(thread, "fragmentColor"), (Arcadia_VPL_IR_Type*)vec4, (Arcadia_Integer32Value)0, Arcadia_VPL_IR_StageIO_Semantics_FragmentColor);
  Arcadia_List_insertBackObjectReferenceValue(thread, fragmentInterface, (Arcadia_Object*)fragmentColor);
  Arcadia_VPL_IR_Stage* fragment = Arcadia_VPL_IR_Stage_create(thread, Arcadia_VPL_IR_Stage_Fragment, fragmentInterface, createList(thread));
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_Stage_Fragment == fragment->kind);

  Arcadia_List* uniforms = createList(thread);
  Arcadia_VPL_IR_Module* module = Arcadia_VPL_IR_Module_create(thread, uniforms, vertex, fragment);
  Arcadia_Tests_assertTrue(thread, uniforms == module->uniforms);
  Arcadia_Tests_assertTrue(thread, vertex == module->vertex);
  Arcadia_Tests_assertTrue(thread, fragment == module->fragment);
}

static void
testUniforms
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VectorType* vec4 = createVector4(thread);

  Arcadia_VPL_IR_ScalarUniform* scalar = Arcadia_VPL_IR_ScalarUniform_create(thread, createString(thread, "lightColor"), (Arcadia_VPL_IR_Type*)vec4);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_UniformKind_Scalar == ((Arcadia_VPL_IR_Uniform*)scalar)->kind);
  Arcadia_Tests_assertTrue(thread, NULL != ((Arcadia_VPL_IR_Uniform*)scalar)->name);
  Arcadia_Tests_assertTrue(thread, 10 == Arcadia_String_getNumberOfBytes(thread, ((Arcadia_VPL_IR_Uniform*)scalar)->name));
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == scalar->type);

  Arcadia_List* recordFields = createList(thread);
  Arcadia_List_insertBackObjectReferenceValue(thread, recordFields, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread, createString(thread, "diffuse"), (Arcadia_VPL_IR_Type*)vec4));
  Arcadia_VPL_IR_RecordType* recordType = Arcadia_VPL_IR_RecordType_create(thread, createString(thread, "material"), recordFields);
  Arcadia_VPL_IR_RecordUniform* record = Arcadia_VPL_IR_RecordUniform_create(thread, createString(thread, "material"), recordType);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_UniformKind_Record == ((Arcadia_VPL_IR_Uniform*)record)->kind);
  Arcadia_Tests_assertTrue(thread, NULL != ((Arcadia_VPL_IR_Uniform*)record)->name);
  Arcadia_Tests_assertTrue(thread, recordType == record->recordType);

  Arcadia_VPL_IR_ArrayUniform* array = Arcadia_VPL_IR_ArrayUniform_create(thread, createString(thread, "lights"), (Arcadia_VPL_IR_Type*)vec4, (Arcadia_SizeValue)4);
  Arcadia_Tests_assertTrue(thread, Arcadia_VPL_IR_UniformKind_Array == ((Arcadia_VPL_IR_Uniform*)array)->kind);
  Arcadia_Tests_assertTrue(thread, NULL != ((Arcadia_VPL_IR_Uniform*)array)->name);
  Arcadia_Tests_assertTrue(thread, 6 == Arcadia_String_getNumberOfBytes(thread, ((Arcadia_VPL_IR_Uniform*)array)->name));
  Arcadia_Tests_assertTrue(thread, (Arcadia_VPL_IR_Type*)vec4 == array->elementType);
  Arcadia_Tests_assertTrue(thread, (Arcadia_SizeValue)4 == array->count);
}

static void
raiseArrayTypeWithNonTypeElement
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_Field* field = Arcadia_VPL_IR_Field_create(thread, createString(thread, "x"), (Arcadia_VPL_IR_Type*)createVector4(thread));
  Arcadia_VPL_IR_ArrayType_create(thread, (Arcadia_VPL_IR_Type*)field, (Arcadia_SizeValue)3);
}

static void
raiseScalarUniformWithNonType
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_Field* field = Arcadia_VPL_IR_Field_create(thread, createString(thread, "x"), (Arcadia_VPL_IR_Type*)createVector4(thread));
  Arcadia_VPL_IR_ScalarUniform_create(thread, createString(thread, "u"), (Arcadia_VPL_IR_Type*)field);
}

static void
raiseReturnWithNonValue
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_VPL_IR_VoidType* voidType = Arcadia_VPL_IR_VoidType_create(thread);
  Arcadia_VPL_IR_ReturnTerminator_create(thread, (Arcadia_VPL_IR_Value*)voidType);
}

static void
testRejectsWrongArgumentTypes
  (
    Arcadia_Thread* thread
  )
{
  assertConstructorRaises(thread, &raiseArrayTypeWithNonTypeElement);
  assertConstructorRaises(thread, &raiseScalarUniformWithNonType);
  assertConstructorRaises(thread, &raiseReturnWithNonValue);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&testTypeFamily)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testValueFamily)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testInstructionFamily)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testTerminatorsAndBlocks)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testFunctionStageModule)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testUniforms)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&testRejectsWrongArgumentTypes)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
