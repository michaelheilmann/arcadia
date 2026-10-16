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

#if !defined(_WIN32) && !defined(_GNU_SOURCE)
  #define _GNU_SOURCE
#endif
#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/TextInternal.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <locale.h>
#include <fenv.h>

static void
text
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    const char* bytes
  )
{
  Arcadia_VPL_Text_append(thread, c->output, bytes, strlen(bytes));
}

static void
objectText
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_String* string
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)string);
  Arcadia_StringBuilder_insertBack(thread, c->output, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)string));
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
id
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    const char* prefix,
    Arcadia_SizeValue number
  )
{
  text(thread, c, prefix);
  objectText(thread, c, Arcadia_String_createFromSize(thread, number));
}

static void
quote
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_String* string
  )
{
  text(thread, c, "\"");
  const unsigned char* bytes = Arcadia_String_getBytes(thread, string);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, string);
  for (Arcadia_SizeValue i = 0; i < n;) {
    unsigned char ch = bytes[i];
    if (ch == '"' || ch == '\\') { text(thread, c, "\\"); Arcadia_VPL_Text_append(thread, c->output, bytes + i, 1); ++i; }
    else if (ch < 0x20 || ch == 0x7f) {
      char escape[] = "\\u0000";
      escape[4] = "0123456789abcdef"[ch >> 4];
      escape[5] = "0123456789abcdef"[ch & 15];
      Arcadia_VPL_Text_append(thread, c->output, escape, 6);
      ++i;
    } else {
      Arcadia_SizeValue count = ch < 0x80 ? 1 : ch < 0xe0 ? 2 : ch < 0xf0 ? 3 : 4;
      Arcadia_VPL_Text_append(thread, c->output, bytes + i, count);
      i += count;
    }
  }
  text(thread, c, "\"");
}

static Arcadia_SizeValue
length
  (
    Arcadia_Thread* thread,
    Arcadia_List* list
  )
{
  return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list);
}

static Arcadia_Object*
at
  (
    Arcadia_Thread* thread,
    Arcadia_List* list,
    Arcadia_SizeValue i
  )
{
  return Arcadia_List_getObjectReferenceValueAt(thread, list, i);
}

static void
writerError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    const char* message
  )
{
  Arcadia_String* string = (Arcadia_String*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_String_createFromCxxString(thread, message));
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)Arcadia_VPL_Text_own(thread, c,
    (Arcadia_Object*)Arcadia_VPL_Diagnostics_VerificationDiagnostic_create(thread, string, NULL));
  Arcadia_Languages_Diagnostics_add(thread, c->diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static Arcadia_SizeValue
collectType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Type* type,
    Arcadia_SizeValue depth
  )
{
  Arcadia_Value key = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type);
  Arcadia_Value old = Arcadia_Map_get(thread, c->types, key);
  if (Arcadia_Value_isSizeValue(&old)) return Arcadia_Value_getSizeValue(&old);
  if (depth > 128) writerError(thread, c, "text error: type nesting exceeds version-1 limit\n");
  Arcadia_SizeValue index = length(thread, c->typeOrder);
  Arcadia_Map_set(thread, c->types, key, Arcadia_Value_makeSizeValue(index), NULL, NULL);
  Arcadia_List_insertBackObjectReferenceValue(thread, c->typeOrder, (Arcadia_Object*)type);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) collectType(thread, c, ((Arcadia_VPL_IR_ArrayType*)type)->element, depth + 1);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = length(thread, fields); i < n; ++i) collectType(thread, c, ((Arcadia_VPL_IR_Field*)at(thread, fields, i))->type, depth + 1);
  }
  return index;
}

static void
typeId
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Type* type
  )
{
  Arcadia_Value value = Arcadia_Map_get(thread, c->types, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type));
  if (!Arcadia_Value_isSizeValue(&value)) writerError(thread, c, "text error: uncollected type\n");
  id(thread, c, "t", Arcadia_Value_getSizeValue(&value));
}

static const char*
elementName
  (
    Arcadia_Natural8Value kind
  )
{
  return kind == Arcadia_VPL_IR_ScalarElementKind_Boolean ? "bool" : kind == Arcadia_VPL_IR_ScalarElementKind_Integer ? "int" : "float";
}

static void
writeType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Type* type,
    Arcadia_SizeValue index
  )
{
  text(thread, c, "    (type "); id(thread, c, "t", index); text(thread, c, " ");
  switch (type->kind) {
    case Arcadia_VPL_IR_TypeKind_Void: text(thread, c, "void"); break;
    case Arcadia_VPL_IR_TypeKind_Scalar: text(thread, c, elementName(((Arcadia_VPL_IR_ScalarType*)type)->elementKind)); break;
    case Arcadia_VPL_IR_TypeKind_Sampler: text(thread, c, "sampler2D"); break;
    case Arcadia_VPL_IR_TypeKind_Vector:
      text(thread, c, "vector "); text(thread, c, elementName(((Arcadia_VPL_IR_VectorType*)type)->elementKind)); id(thread, c, " ", ((Arcadia_VPL_IR_VectorType*)type)->size);
      break;
    case Arcadia_VPL_IR_TypeKind_Matrix:
      text(thread, c, "matrix"); id(thread, c, " ", ((Arcadia_VPL_IR_MatrixType*)type)->rows); id(thread, c, " ", ((Arcadia_VPL_IR_MatrixType*)type)->columns);
      break;
    case Arcadia_VPL_IR_TypeKind_Array:
      text(thread, c, "array "); typeId(thread, c, ((Arcadia_VPL_IR_ArrayType*)type)->element); id(thread, c, " ", ((Arcadia_VPL_IR_ArrayType*)type)->count);
      break;
    case Arcadia_VPL_IR_TypeKind_Record: {
      Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)type;
      text(thread, c, "record "); quote(thread, c, record->name);
      for (Arcadia_SizeValue i = 0, n = length(thread, record->fields); i < n; ++i) {
        Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)at(thread, record->fields, i);
        text(thread, c, " (field "); quote(thread, c, field->name); text(thread, c, " "); typeId(thread, c, field->type); text(thread, c, ")");
      }
    } break;
  }
  text(thread, c, ")\n");
}

static Arcadia_SizeValue
objectIndex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_List* objects,
    Arcadia_Object* object
  )
{
  for (Arcadia_SizeValue i = 0, n = length(thread, objects); i < n; ++i) if (at(thread, objects, i) == object) return i;
  writerError(thread, c, "text error: object is outside its declared scope\n");
  return 0;
}

static void
floatingText
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_Real32Value value
  )
{
  if (value == 0.f) { text(thread, c, signbit(value) ? "-0.0" : "0.0"); return; }
#if defined(_WIN32)
  _locale_t locale = _create_locale(LC_NUMERIC, "C");
#else
  locale_t locale = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
#endif
  if (!locale) { Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed); Arcadia_Thread_jump(thread); }
  fenv_t environment;
  int savedEnvironment = fegetenv(&environment);
  if (!savedEnvironment) fesetround(FE_TONEAREST);
  char bytes[64];
#if defined(_WIN32)
  int count = _snprintf_l(bytes, sizeof(bytes), "%.9g", locale, (double)value);
  _free_locale(locale);
#else
  locale_t previous = uselocale(locale);
  int count = snprintf(bytes, sizeof(bytes), "%.9g", (double)value);
  uselocale(previous);
  freelocale(locale);
#endif
  if (!savedEnvironment) fesetenv(&environment);
  if (count < 0 || (size_t)count >= sizeof(bytes) || savedEnvironment) writerError(thread, c, "text error: binary32 formatting failed\n");
  Arcadia_VPL_Text_append(thread, c->output, bytes, (Arcadia_SizeValue)count);
}

static void
operand
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Value* value
  )
{
  if (value->kind == Arcadia_VPL_IR_ValueKind_Register) { id(thread, c, "r", ((Arcadia_VPL_IR_RegisterValue*)value)->index); return; }
  Arcadia_VPL_IR_ConstantValue* constant = (Arcadia_VPL_IR_ConstantValue*)value;
  Arcadia_Natural8Value kind = ((Arcadia_VPL_IR_ScalarType*)constant->type)->elementKind;
  text(thread, c, "("); text(thread, c, elementName(kind)); text(thread, c, " ");
  if (kind == Arcadia_VPL_IR_ScalarElementKind_Boolean) text(thread, c, Arcadia_Value_getBooleanValue(&constant->literal) ? "true" : "false");
  else if (kind == Arcadia_VPL_IR_ScalarElementKind_Integer) objectText(thread, c, Arcadia_String_createFromInteger32(thread, Arcadia_Value_getInteger32Value(&constant->literal)));
  else floatingText(thread, c, Arcadia_Value_getReal32Value(&constant->literal));
  text(thread, c, ")");
}

static void
reference
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_ReferenceValue* ref,
    Arcadia_SizeValue depth
  )
{
  if (depth > 120) writerError(thread, c, "text error: reference path exceeds version-1 nesting limit\n");
  text(thread, c, "(");
  switch (ref->storageKind) {
    case Arcadia_VPL_IR_StorageKind_Local: text(thread, c, "local "); id(thread, c, "r", ref->slot); break;
    case Arcadia_VPL_IR_StorageKind_Parameter: text(thread, c, "parameter "); id(thread, c, "p", ref->slot); break;
    case Arcadia_VPL_IR_StorageKind_Uniform: text(thread, c, "uniform "); id(thread, c, "u", ref->slot); break;
    case Arcadia_VPL_IR_StorageKind_StageIO: text(thread, c, "stage "); id(thread, c, "s", ref->slot); break;
    case Arcadia_VPL_IR_StorageKind_Member: {
      Arcadia_VPL_IR_MemberReferenceValue* member = (Arcadia_VPL_IR_MemberReferenceValue*)ref;
      text(thread, c, "member_ref "); reference(thread, c, member->base, depth + 1); text(thread, c, " "); quote(thread, c, member->fieldName);
    } break;
    case Arcadia_VPL_IR_StorageKind_Index: {
      Arcadia_VPL_IR_IndexReferenceValue* index = (Arcadia_VPL_IR_IndexReferenceValue*)ref;
      text(thread, c, "index_ref "); reference(thread, c, index->base, depth + 1); text(thread, c, " "); operand(thread, c, index->index);
    } break;
  }
  text(thread, c, ")");
}

static void
arguments
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_List* values
  )
{
  for (Arcadia_SizeValue i = 0, n = length(thread, values); i < n; ++i) { text(thread, c, " "); operand(thread, c, (Arcadia_VPL_IR_Value*)at(thread, values, i)); }
}

static void
instruction
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Stage* stage,
    Arcadia_VPL_IR_Instruction* base
  )
{
  static const char* binaryNames[] = { "add", "subtract", "multiply", "divide", "equal", "not_equal", "less", "less_equal", "greater", "greater_equal" };
  if (base->kind == Arcadia_VPL_IR_InstructionKind_DeclareLocal) return;
  text(thread, c, "          (");
  switch (base->kind) {
    case Arcadia_VPL_IR_InstructionKind_Copy: {
      Arcadia_VPL_IR_CopyInstruction* copy = (Arcadia_VPL_IR_CopyInstruction*)base;
      text(thread, c, "copy "); id(thread, c, "r", copy->result->index); text(thread, c, " "); operand(thread, c, copy->source);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Load: {
      Arcadia_VPL_IR_LoadInstruction* load = (Arcadia_VPL_IR_LoadInstruction*)base;
      text(thread, c, "load "); id(thread, c, "r", load->result->index); text(thread, c, " "); reference(thread, c, load->source, 0);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Store: {
      Arcadia_VPL_IR_StoreInstruction* store = (Arcadia_VPL_IR_StoreInstruction*)base;
      text(thread, c, "store "); reference(thread, c, store->target, 0); text(thread, c, " "); operand(thread, c, store->value);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Binary: {
      Arcadia_VPL_IR_BinaryInstruction* binary = (Arcadia_VPL_IR_BinaryInstruction*)base;
      text(thread, c, binaryNames[binary->operatorKind]); id(thread, c, " r", binary->result->index); text(thread, c, " "); operand(thread, c, binary->lhs); text(thread, c, " "); operand(thread, c, binary->rhs);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Construct: {
      Arcadia_VPL_IR_ConstructInstruction* construct = (Arcadia_VPL_IR_ConstructInstruction*)base;
      text(thread, c, "construct "); id(thread, c, "r", construct->result->index); arguments(thread, c, construct->arguments);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Call: {
      Arcadia_VPL_IR_CallInstruction* call = (Arcadia_VPL_IR_CallInstruction*)base;
      text(thread, c, call->calleeFunction ? "call " : "texture ");
      if (call->result) id(thread, c, "r", call->result->index); else text(thread, c, "void");
      if (call->calleeFunction) id(thread, c, " f", objectIndex(thread, c, stage->functions, (Arcadia_Object*)call->calleeFunction));
      arguments(thread, c, call->arguments);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Swizzle: {
      Arcadia_VPL_IR_SwizzleInstruction* swizzle = (Arcadia_VPL_IR_SwizzleInstruction*)base;
      text(thread, c, "swizzle "); id(thread, c, "r", swizzle->result->index); text(thread, c, " "); operand(thread, c, swizzle->source); text(thread, c, " "); quote(thread, c, swizzle->pattern);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Member: {
      Arcadia_VPL_IR_MemberInstruction* member = (Arcadia_VPL_IR_MemberInstruction*)base;
      text(thread, c, "member "); id(thread, c, "r", member->result->index); text(thread, c, " "); operand(thread, c, member->source); text(thread, c, " "); quote(thread, c, member->fieldName);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Index: {
      Arcadia_VPL_IR_IndexInstruction* index = (Arcadia_VPL_IR_IndexInstruction*)base;
      text(thread, c, "index "); id(thread, c, "r", index->result->index); text(thread, c, " "); operand(thread, c, index->source); text(thread, c, " "); operand(thread, c, index->index);
    } break;
  }
  text(thread, c, ")\n");
}

static void
writeFunction
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Stage* stage,
    Arcadia_VPL_IR_Function* function,
    Arcadia_SizeValue index
  )
{
  text(thread, c, "      (function "); id(thread, c, "f", index); text(thread, c, " "); quote(thread, c, function->name); text(thread, c, " "); typeId(thread, c, function->returnType); text(thread, c, "\n        (parameters");
  for (Arcadia_SizeValue i = 0, n = length(thread, function->parameters); i < n; ++i) {
    text(thread, c, " (parameter "); id(thread, c, "p", i); text(thread, c, " "); typeId(thread, c, ((Arcadia_VPL_IR_ReferenceValue*)at(thread, function->parameters, i))->type); text(thread, c, ")");
  }
  text(thread, c, ")\n        (registers");
  Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)at(thread, function->blocks, 0);
  for (Arcadia_SizeValue i = 0, n = length(thread, entry->instructions); i < n; ++i) {
    Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(thread, entry->instructions, i);
    if (base->kind != Arcadia_VPL_IR_InstructionKind_DeclareLocal) break;
    Arcadia_VPL_IR_DeclareLocalInstruction* declare = (Arcadia_VPL_IR_DeclareLocalInstruction*)base;
    text(thread, c, " (register "); id(thread, c, "r", declare->registerValue->index); text(thread, c, " "); typeId(thread, c, declare->type); text(thread, c, ")");
  }
  text(thread, c, ")\n");
  for (Arcadia_SizeValue q = 0, b = length(thread, function->blocks); q < b; ++q) {
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)at(thread, function->blocks, q);
    text(thread, c, "        (block "); id(thread, c, "b", q); text(thread, c, "\n");
    for (Arcadia_SizeValue i = 0, n = length(thread, block->instructions); i < n; ++i) instruction(thread, c, stage, (Arcadia_VPL_IR_Instruction*)at(thread, block->instructions, i));
    text(thread, c, "          (");
    if (block->terminator->kind == Arcadia_VPL_IR_TerminatorKind_Branch) {
      text(thread, c, "branch "); id(thread, c, "b", objectIndex(thread, c, function->blocks, (Arcadia_Object*)((Arcadia_VPL_IR_BranchTerminator*)block->terminator)->target));
    } else if (block->terminator->kind == Arcadia_VPL_IR_TerminatorKind_ConditionalBranch) {
      Arcadia_VPL_IR_ConditionalBranchTerminator* branch = (Arcadia_VPL_IR_ConditionalBranchTerminator*)block->terminator;
      text(thread, c, "branch_if "); operand(thread, c, branch->condition); id(thread, c, " b", objectIndex(thread, c, function->blocks, (Arcadia_Object*)branch->trueTarget)); id(thread, c, " b", objectIndex(thread, c, function->blocks, (Arcadia_Object*)branch->falseTarget));
    } else if (block->terminator->kind == Arcadia_VPL_IR_TerminatorKind_Return) {
      text(thread, c, "return");
      Arcadia_VPL_IR_Value* value = ((Arcadia_VPL_IR_ReturnTerminator*)block->terminator)->value;
      if (value) { text(thread, c, " "); operand(thread, c, value); }
    } else text(thread, c, "discard");
    text(thread, c, "))\n");
  }
  text(thread, c, "      )\n");
}

static void
collectStage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Stage* stage
  )
{
  for (Arcadia_SizeValue i = 0, n = length(thread, stage->interface); i < n; ++i) collectType(thread, c, ((Arcadia_VPL_IR_StageIO*)at(thread, stage->interface, i))->type, 0);
  for (Arcadia_SizeValue i = 0, n = length(thread, stage->functions); i < n; ++i) {
    Arcadia_VPL_IR_Function* function = (Arcadia_VPL_IR_Function*)at(thread, stage->functions, i);
    collectType(thread, c, function->returnType, 0);
    for (Arcadia_SizeValue j = 0, m = length(thread, function->parameters); j < m; ++j) collectType(thread, c, ((Arcadia_VPL_IR_ReferenceValue*)at(thread, function->parameters, j))->type, 0);
    Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)at(thread, function->blocks, 0);
    for (Arcadia_SizeValue j = 0, m = length(thread, entry->instructions); j < m; ++j) {
      Arcadia_VPL_IR_Instruction* instruction = (Arcadia_VPL_IR_Instruction*)at(thread, entry->instructions, j);
      if (instruction->kind != Arcadia_VPL_IR_InstructionKind_DeclareLocal) break;
      collectType(thread, c, ((Arcadia_VPL_IR_DeclareLocalInstruction*)instruction)->type, 0);
    }
  }
}

static void
writeStage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Stage* stage
  )
{
  static const char* semanticNames[] = { "none", "position", "color", "texcoord", "fragment_color" };
  text(thread, c, stage->kind == Arcadia_VPL_IR_Stage_Vertex ? "  (vertex " : "  (fragment ");
  id(thread, c, "f", objectIndex(thread, c, stage->functions, (Arcadia_Object*)stage->entryFunction));
  text(thread, c, "\n    (interface\n");
  for (Arcadia_SizeValue i = 0, n = length(thread, stage->interface); i < n; ++i) {
    Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(thread, stage->interface, i);
    text(thread, c, io->direction == Arcadia_VPL_IR_StageIO_Direction_In ? "      (in " : "      (out ");
    id(thread, c, "s", i); text(thread, c, " "); quote(thread, c, io->name); text(thread, c, " "); typeId(thread, c, io->type); text(thread, c, " ");
    objectText(thread, c, Arcadia_String_createFromInteger32(thread, io->location)); text(thread, c, " "); text(thread, c, semanticNames[io->semantics]); text(thread, c, ")\n");
  }
  text(thread, c, "    )\n    (functions\n");
  for (Arcadia_SizeValue i = 0, n = length(thread, stage->functions); i < n; ++i) writeFunction(thread, c, stage, (Arcadia_VPL_IR_Function*)at(thread, stage->functions, i), i);
  text(thread, c, "    )\n  )\n");
}

Arcadia_String*
Arcadia_VPL_Text_write
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Module* module,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  if (!module || !diagnostics) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)module);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
    Arcadia_VPL_Text_Context* c = Arcadia_VPL_Text_createContext(thread, diagnostics);
    Arcadia_VPL_IR_Verifier* verifier = (Arcadia_VPL_IR_Verifier*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_Verifier_create(thread, diagnostics));
    Arcadia_VPL_IR_Verifier_run(thread, verifier, module);
    c->typeOrder = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    for (Arcadia_SizeValue i = 0, n = length(thread, module->uniforms); i < n; ++i) {
      Arcadia_VPL_IR_Uniform* uniform = (Arcadia_VPL_IR_Uniform*)at(thread, module->uniforms, i);
      collectType(thread, c, uniform->kind == Arcadia_VPL_IR_UniformKind_Scalar ? ((Arcadia_VPL_IR_ScalarUniform*)uniform)->type : uniform->kind == Arcadia_VPL_IR_UniformKind_Record ? (Arcadia_VPL_IR_Type*)((Arcadia_VPL_IR_RecordUniform*)uniform)->recordType : ((Arcadia_VPL_IR_ArrayUniform*)uniform)->elementType, 0);
    }
    collectStage(thread, c, module->vertex);
    collectStage(thread, c, module->fragment);
    text(thread, c, "(module 1\n  (types\n");
    for (Arcadia_SizeValue i = 0, n = length(thread, c->typeOrder); i < n; ++i) writeType(thread, c, (Arcadia_VPL_IR_Type*)at(thread, c->typeOrder, i), i);
    text(thread, c, "  )\n  (uniforms\n");
    for (Arcadia_SizeValue i = 0, n = length(thread, module->uniforms); i < n; ++i) {
      Arcadia_VPL_IR_Uniform* uniform = (Arcadia_VPL_IR_Uniform*)at(thread, module->uniforms, i);
      text(thread, c, uniform->kind == Arcadia_VPL_IR_UniformKind_Scalar ? "    (scalar " : uniform->kind == Arcadia_VPL_IR_UniformKind_Record ? "    (record " : "    (array ");
      id(thread, c, "u", i); text(thread, c, " "); quote(thread, c, uniform->name); text(thread, c, " ");
      if (uniform->kind == Arcadia_VPL_IR_UniformKind_Scalar) typeId(thread, c, ((Arcadia_VPL_IR_ScalarUniform*)uniform)->type);
      else if (uniform->kind == Arcadia_VPL_IR_UniformKind_Record) typeId(thread, c, (Arcadia_VPL_IR_Type*)((Arcadia_VPL_IR_RecordUniform*)uniform)->recordType);
      else { typeId(thread, c, ((Arcadia_VPL_IR_ArrayUniform*)uniform)->elementType); id(thread, c, " ", ((Arcadia_VPL_IR_ArrayUniform*)uniform)->count); }
      text(thread, c, ")\n");
    }
    text(thread, c, "  )\n"); writeStage(thread, c, module->vertex); writeStage(thread, c, module->fragment); text(thread, c, ")\n");
    Arcadia_String* result = (Arcadia_String*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)c->output)));
    // Publication must not emit a graph outside the reader's version-1 representability limits.
    Arcadia_VPL_Text_read(thread, Arcadia_String_getBytes(thread, result), Arcadia_String_getNumberOfBytes(thread, result), diagnostics, NULL);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_popJumpTarget(thread);
    return result;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
    return NULL;
  }
}
