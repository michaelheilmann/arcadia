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
#include "Arcadia/VPL/GLSL/BackendInternal.h"
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <limits.h>
#include <fenv.h>

static Arcadia_SizeValue
size
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_List* list
  )
{
  return Arcadia_Collection_getSize(s->thread, (Arcadia_Collection*)list);
}

static Arcadia_Object*
at
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_List* list,
    Arcadia_SizeValue i
  )
{
  return Arcadia_List_getObjectReferenceValueAt(s->thread, list, i);
}

static void
emit
  (
    Arcadia_VPL_GLSL_State* s,
    const char* text
  )
{
  Arcadia_VPL_Text_append(s->thread, s->output, text, strlen(text));
}

static void
number
  (
    Arcadia_VPL_GLSL_State* s,
    const char* prefix,
    Arcadia_SizeValue value
  )
{
  emit(s, prefix);
  Arcadia_String* string = (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_createFromSize(s->thread, value));
  Arcadia_StringBuilder_insertBack(s->thread, s->output, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)string));
}

static void
cName
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_String* string
  )
{
  Arcadia_VPL_Text_append(s->thread, s->output, Arcadia_String_getBytes(s->thread, string), Arcadia_String_getNumberOfBytes(s->thread, string) - 1);
}

static void
typeName
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type
  )
{
  switch (type->kind) {
    case Arcadia_VPL_IR_TypeKind_Void: emit(s, "void"); break;
    case Arcadia_VPL_IR_TypeKind_Scalar: {
      Arcadia_Natural8Value element = ((Arcadia_VPL_IR_ScalarType*)type)->elementKind;
      emit(s, element == Arcadia_VPL_IR_ScalarElementKind_Boolean ? "bool" : element == Arcadia_VPL_IR_ScalarElementKind_Integer ? "int" : "float");
    } break;
    case Arcadia_VPL_IR_TypeKind_Vector: {
      Arcadia_VPL_IR_VectorType* vector = (Arcadia_VPL_IR_VectorType*)type;
      number(s, vector->elementKind == Arcadia_VPL_IR_ScalarElementKind_Boolean ? "bvec" : vector->elementKind == Arcadia_VPL_IR_ScalarElementKind_Integer ? "ivec" : "vec", vector->size);
    } break;
    case Arcadia_VPL_IR_TypeKind_Matrix: {
      Arcadia_VPL_IR_MatrixType* matrix = (Arcadia_VPL_IR_MatrixType*)type;
      number(s, "mat", matrix->columns);
      if (matrix->rows != matrix->columns) number(s, "x", matrix->rows);
    } break;
    case Arcadia_VPL_IR_TypeKind_Sampler: emit(s, "sampler2D"); break;
    case Arcadia_VPL_IR_TypeKind_Record: {
      Arcadia_Value value = Arcadia_Map_get(s->thread, s->recordIds, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type));
      number(s, "vpl_R", Arcadia_Value_getSizeValue(&value));
    } break;
    case Arcadia_VPL_IR_TypeKind_Array:
      typeName(s, ((Arcadia_VPL_IR_ArrayType*)type)->element);
      number(s, "[", ((Arcadia_VPL_IR_ArrayType*)type)->count); emit(s, "]");
      break;
  }
}

static void
declaration
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type,
    const char* prefix,
    Arcadia_SizeValue index
  )
{
  typeName(s, type->kind == Arcadia_VPL_IR_TypeKind_Array ? ((Arcadia_VPL_IR_ArrayType*)type)->element : type);
  emit(s, " "); number(s, prefix, index);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) { number(s, "[", ((Arcadia_VPL_IR_ArrayType*)type)->count); emit(s, "]"); }
}

static Arcadia_BooleanValue
hasInteger
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Scalar) return ((Arcadia_VPL_IR_ScalarType*)type)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Integer;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Vector) return ((Arcadia_VPL_IR_VectorType*)type)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Integer;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) return hasInteger(s, ((Arcadia_VPL_IR_ArrayType*)type)->element);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) if (hasInteger(s, ((Arcadia_VPL_IR_Field*)at(s, fields, i))->type)) return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_SizeValue
fieldIndex
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type,
    Arcadia_String* name
  )
{
  Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
  for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) {
    Arcadia_String* field = ((Arcadia_VPL_IR_Field*)at(s, fields, i))->name;
    if (Arcadia_String_isEqualTo_pn(s->thread, name, Arcadia_String_getBytes(s->thread, field), Arcadia_String_getNumberOfBytes(s->thread, field))) return i;
  }
  _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)type, "GLSL: missing verified record field\n");
  return 0;
}

static Arcadia_VPL_IR_Type*
valueType
  (
    Arcadia_VPL_IR_Value* value
  )
{
  return value->kind == Arcadia_VPL_IR_ValueKind_Register ? ((Arcadia_VPL_IR_RegisterValue*)value)->type : ((Arcadia_VPL_IR_ConstantValue*)value)->type;
}

static Arcadia_Map*
map
  (
    Arcadia_VPL_GLSL_State* s
  )
{
  return (Arcadia_Map*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_HashMap_create(s->thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)));
}

static Arcadia_String*
samplerName
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_SizeValue reg
  )
{
  Arcadia_Value key = Arcadia_Value_makeSizeValue(reg);
  Arcadia_Value cached = Arcadia_Map_get(s->thread, s->samplerAliases, key);
  if (Arcadia_Value_isObjectReferenceValue(&cached)) return (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&cached);
  Arcadia_Value active = Arcadia_Map_get(s->thread, s->samplerActive, key);
  if (!Arcadia_Value_isVoidValue(&active)) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)s->function, "GLSL 420: cyclic sampler aliases are unsupported\n");
  Arcadia_Map_set(s->thread, s->samplerActive, key, Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_True), NULL, NULL);
  Arcadia_Value definition = Arcadia_Map_get(s->thread, s->samplerDefinitions, key);
  if (!Arcadia_Value_isObjectReferenceValue(&definition)) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)s->function, "GLSL 420: sampler register lacks a static source\n");
  Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)Arcadia_Value_getObjectReferenceValue(&definition);
  Arcadia_String* result = NULL;
  if (base->kind == Arcadia_VPL_IR_InstructionKind_Load) {
    Arcadia_VPL_IR_ReferenceValue* ref = ((Arcadia_VPL_IR_LoadInstruction*)base)->source;
    if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Uniform) result = ((Arcadia_VPL_Backends_GLSL_Binding*)at(s, s->artifact->uniforms, ref->slot))->glName;
    else if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Parameter) {
      Arcadia_StringBuilder* output = s->output;
      s->output = (Arcadia_StringBuilder*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_StringBuilder_create(s->thread));
      number(s, "vpl_p", ref->slot);
      const char zero = 0;
      Arcadia_VPL_Text_append(s->thread, s->output, &zero, 1);
      result = (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_create(s->thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)s->output)));
      s->output = output;
    } else if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Local) result = samplerName(s, ref->slot);
  } else {
    Arcadia_VPL_IR_Value* source = base->kind == Arcadia_VPL_IR_InstructionKind_Copy ? ((Arcadia_VPL_IR_CopyInstruction*)base)->source : ((Arcadia_VPL_IR_StoreInstruction*)base)->value;
    if (source->kind == Arcadia_VPL_IR_ValueKind_Register) result = samplerName(s, ((Arcadia_VPL_IR_RegisterValue*)source)->index);
  }
  if (!result) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)base, "GLSL 420: sampler must alias a uniform or sampler parameter\n");
  Arcadia_Map_set(s->thread, s->samplerAliases, key, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)result), NULL, NULL);
  return result;
}

static void
samplerDefinitions
  (
    Arcadia_VPL_GLSL_State* s
  )
{
  s->samplerDefinitions = map(s);
  s->samplerAliases = map(s);
  s->samplerActive = map(s);
  for (Arcadia_SizeValue q = 0, b = size(s, s->function->blocks); q < b; ++q) {
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)at(s, s->function->blocks, q);
    for (Arcadia_SizeValue i = 0, n = size(s, block->instructions); i < n; ++i) {
      Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(s, block->instructions, i);
      Arcadia_VPL_IR_RegisterValue* result = NULL;
      Arcadia_SizeValue id = 0;
      Arcadia_BooleanValue assigned = Arcadia_BooleanValue_False;
      if (base->kind == Arcadia_VPL_IR_InstructionKind_Load) result = ((Arcadia_VPL_IR_LoadInstruction*)base)->result;
      if (base->kind == Arcadia_VPL_IR_InstructionKind_Copy) result = ((Arcadia_VPL_IR_CopyInstruction*)base)->result;
      if (base->kind == Arcadia_VPL_IR_InstructionKind_Call) result = ((Arcadia_VPL_IR_CallInstruction*)base)->result;
      if (result && result->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) { id = result->index; assigned = Arcadia_BooleanValue_True; }
      if (base->kind == Arcadia_VPL_IR_InstructionKind_Store && ((Arcadia_VPL_IR_StoreInstruction*)base)->target->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) {
        Arcadia_VPL_IR_ReferenceValue* ref = ((Arcadia_VPL_IR_StoreInstruction*)base)->target;
        if (ref->storageKind != Arcadia_VPL_IR_StorageKind_Local) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)base, "GLSL 420: sampler parameter/storage mutation is unsupported\n");
        id = ref->slot; assigned = Arcadia_BooleanValue_True;
      }
      if (!assigned) continue;
      Arcadia_Value key = Arcadia_Value_makeSizeValue(id);
      Arcadia_Value prior = Arcadia_Map_get(s->thread, s->samplerDefinitions, key);
      if (!Arcadia_Value_isVoidValue(&prior) || base->kind == Arcadia_VPL_IR_InstructionKind_Call) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)base, "GLSL 420: sampler registers require one static alias definition\n");
      Arcadia_Map_set(s->thread, s->samplerDefinitions, key, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)base), NULL, NULL);
    }
  }
}

static void
floatLiteral
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_Real32Value value
  )
{
  if (value == 0.f) { emit(s, signbit(value) ? "-0.0" : "0.0"); return; }
#if defined(_WIN32)
  _locale_t locale = _create_locale(LC_NUMERIC, "C");
#else
  locale_t locale = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
#endif
  if (!locale) { Arcadia_Thread_setStatus(s->thread, Arcadia_Status_AllocationFailed); Arcadia_Thread_jump(s->thread); }
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
  if (count < 0 || (size_t)count >= sizeof(bytes) || savedEnvironment) _Arcadia_VPL_GLSL_error(s, NULL, "GLSL: floating literal formatting failed\n");
  Arcadia_VPL_Text_append(s->thread, s->output, bytes, (Arcadia_SizeValue)count);
  if (!strchr(bytes, '.') && !strchr(bytes, 'e') && !strchr(bytes, 'E')) emit(s, ".0");
}

static void
operand
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Value* value
  )
{
  if (value->kind == Arcadia_VPL_IR_ValueKind_Register) {
    Arcadia_VPL_IR_RegisterValue* reg = (Arcadia_VPL_IR_RegisterValue*)value;
    if (reg->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) cName(s, samplerName(s, reg->index));
    else number(s, "vpl_r", reg->index);
    return;
  }
  Arcadia_VPL_IR_ConstantValue* constant = (Arcadia_VPL_IR_ConstantValue*)value;
  Arcadia_Natural8Value kind = ((Arcadia_VPL_IR_ScalarType*)constant->type)->elementKind;
  if (kind == Arcadia_VPL_IR_ScalarElementKind_Boolean) emit(s, Arcadia_Value_getBooleanValue(&constant->literal) ? "true" : "false");
  else if (kind == Arcadia_VPL_IR_ScalarElementKind_Integer) {
    Arcadia_Integer32Value integer = Arcadia_Value_getInteger32Value(&constant->literal);
    if (integer == (-2147483647 - 1)) emit(s, "(-2147483647 - 1)");
    else {
      Arcadia_String* text = (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_createFromInteger32(s->thread, integer));
      Arcadia_StringBuilder_insertBack(s->thread, s->output, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)text));
    }
  } else floatLiteral(s, Arcadia_Value_getReal32Value(&constant->literal));
}

static void
reference
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_ReferenceValue* ref
  )
{
  switch (ref->storageKind) {
    case Arcadia_VPL_IR_StorageKind_Local:
      if (ref->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) cName(s, samplerName(s, ref->slot)); else number(s, "vpl_r", ref->slot);
      break;
    case Arcadia_VPL_IR_StorageKind_Parameter: number(s, "vpl_p", ref->slot); break;
    case Arcadia_VPL_IR_StorageKind_Uniform: number(s, "vpl_u", ref->slot); break;
    case Arcadia_VPL_IR_StorageKind_StageIO:
      cName(s, ((Arcadia_VPL_Backends_GLSL_Binding*)at(s, s->stage->kind == Arcadia_VPL_IR_Stage_Vertex ? s->artifact->vertexInterface : s->artifact->fragmentInterface, ref->slot))->glName);
      break;
    case Arcadia_VPL_IR_StorageKind_Member: {
      Arcadia_VPL_IR_MemberReferenceValue* member = (Arcadia_VPL_IR_MemberReferenceValue*)ref;
      reference(s, member->base); number(s, ".vpl_m", fieldIndex(s, member->base->type, member->fieldName));
    } break;
    case Arcadia_VPL_IR_StorageKind_Index: {
      Arcadia_VPL_IR_IndexReferenceValue* index = (Arcadia_VPL_IR_IndexReferenceValue*)ref;
      reference(s, index->base); emit(s, "["); operand(s, index->index); emit(s, "]");
    } break;
  }
}

static void
arguments
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_List* list
  )
{
  for (Arcadia_SizeValue i = 0, n = size(s, list); i < n; ++i) { if (i) emit(s, ", "); operand(s, (Arcadia_VPL_IR_Value*)at(s, list, i)); }
}

static void
loadValue
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_ReferenceValue* ref
  )
{
  if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Uniform && ((Arcadia_VPL_IR_Uniform*)at(s, s->module->uniforms, ref->slot))->kind == Arcadia_VPL_IR_UniformKind_Record) {
    typeName(s, ref->type); emit(s, "(");
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)ref->type)->fields;
    for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) { if (i) emit(s, ", "); number(s, "vpl_u", ref->slot); number(s, ".vpl_m", i); }
    emit(s, ")");
  } else reference(s, ref);
}

static Arcadia_SizeValue
functionIndex
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Function* function
  )
{
  for (Arcadia_SizeValue i = 0, n = size(s, s->stage->functions); i < n; ++i) if (at(s, s->stage->functions, i) == (Arcadia_Object*)function) return i;
  _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)function, "GLSL: foreign function after verification\n");
  return 0;
}

static void
instruction
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Instruction* base,
    const char* indent
  )
{
  static const char* operators[] = { "+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">=" };
  if (base->kind == Arcadia_VPL_IR_InstructionKind_DeclareLocal) return;
  if ((base->kind == Arcadia_VPL_IR_InstructionKind_Load && ((Arcadia_VPL_IR_LoadInstruction*)base)->result->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) ||
      (base->kind == Arcadia_VPL_IR_InstructionKind_Copy && ((Arcadia_VPL_IR_CopyInstruction*)base)->result->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) ||
      (base->kind == Arcadia_VPL_IR_InstructionKind_Store && ((Arcadia_VPL_IR_StoreInstruction*)base)->target->type->kind == Arcadia_VPL_IR_TypeKind_Sampler)) return;
  emit(s, indent);
  switch (base->kind) {
    case Arcadia_VPL_IR_InstructionKind_Copy: {
      Arcadia_VPL_IR_CopyInstruction* copy = (Arcadia_VPL_IR_CopyInstruction*)base;
      number(s, "vpl_r", copy->result->index); emit(s, " = "); operand(s, copy->source);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Load: {
      Arcadia_VPL_IR_LoadInstruction* load = (Arcadia_VPL_IR_LoadInstruction*)base;
      number(s, "vpl_r", load->result->index); emit(s, " = "); loadValue(s, load->source);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Store: {
      Arcadia_VPL_IR_StoreInstruction* store = (Arcadia_VPL_IR_StoreInstruction*)base;
      reference(s, store->target); emit(s, " = "); operand(s, store->value);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Binary: {
      Arcadia_VPL_IR_BinaryInstruction* binary = (Arcadia_VPL_IR_BinaryInstruction*)base;
      number(s, "vpl_r", binary->result->index); emit(s, " = ("); operand(s, binary->lhs); emit(s, " "); emit(s, operators[binary->operatorKind]); emit(s, " "); operand(s, binary->rhs); emit(s, ")");
    } break;
    case Arcadia_VPL_IR_InstructionKind_Construct: {
      Arcadia_VPL_IR_ConstructInstruction* construct = (Arcadia_VPL_IR_ConstructInstruction*)base;
      number(s, "vpl_r", construct->result->index); emit(s, " = "); typeName(s, construct->type); emit(s, "("); arguments(s, construct->arguments); emit(s, ")");
    } break;
    case Arcadia_VPL_IR_InstructionKind_Call: {
      Arcadia_VPL_IR_CallInstruction* call = (Arcadia_VPL_IR_CallInstruction*)base;
      if (call->result) { number(s, "vpl_r", call->result->index); emit(s, " = "); }
      if (call->calleeFunction) number(s, "vpl_f", functionIndex(s, call->calleeFunction)); else emit(s, "texture");
      emit(s, "("); arguments(s, call->arguments); emit(s, ")");
    } break;
    case Arcadia_VPL_IR_InstructionKind_Swizzle: {
      Arcadia_VPL_IR_SwizzleInstruction* swizzle = (Arcadia_VPL_IR_SwizzleInstruction*)base;
      number(s, "vpl_r", swizzle->result->index); emit(s, " = ("); operand(s, swizzle->source); emit(s, ").");
      Arcadia_VPL_Text_append(s->thread, s->output, Arcadia_String_getBytes(s->thread, swizzle->pattern), Arcadia_String_getNumberOfBytes(s->thread, swizzle->pattern));
    } break;
    case Arcadia_VPL_IR_InstructionKind_Member: {
      Arcadia_VPL_IR_MemberInstruction* member = (Arcadia_VPL_IR_MemberInstruction*)base;
      number(s, "vpl_r", member->result->index); emit(s, " = ("); operand(s, member->source); emit(s, ")"); number(s, ".vpl_m", fieldIndex(s, valueType(member->source), member->fieldName));
    } break;
    case Arcadia_VPL_IR_InstructionKind_Index: {
      Arcadia_VPL_IR_IndexInstruction* index = (Arcadia_VPL_IR_IndexInstruction*)base;
      number(s, "vpl_r", index->result->index); emit(s, " = ("); operand(s, index->source); emit(s, ")["); operand(s, index->index); emit(s, "]");
    } break;
    default: _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)base, "GLSL: unsupported verified instruction\n");
  }
  emit(s, ";\n");
}

static void
zeroValue
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Scalar) {
    emit(s, ((Arcadia_VPL_IR_ScalarType*)type)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Boolean ? "false" : ((Arcadia_VPL_IR_ScalarType*)type)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Integer ? "0" : "0.0");
    return;
  }
  typeName(s, type); emit(s, "(");
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) {
    Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)type;
    for (Arcadia_SizeValue i = 0; i < array->count; ++i) { if (i) emit(s, ", "); zeroValue(s, array->element); }
  } else if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) { if (i) emit(s, ", "); zeroValue(s, ((Arcadia_VPL_IR_Field*)at(s, fields, i))->type); }
  } else emit(s, "0");
  emit(s, ")");
}

static Arcadia_SizeValue
blockIndex
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Block* block
  )
{
  for (Arcadia_SizeValue i = 0, n = size(s, s->function->blocks); i < n; ++i) if (at(s, s->function->blocks, i) == (Arcadia_Object*)block) return i;
  _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)block, "GLSL: foreign block after verification\n");
  return 0;
}

static void
returnDefault
  (
    Arcadia_VPL_GLSL_State* s
  )
{
  emit(s, "return");
  if (s->function->returnType->kind != Arcadia_VPL_IR_TypeKind_Void) { emit(s, " "); zeroValue(s, s->function->returnType); }
  emit(s, ";\n");
}

static void
terminator
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Terminator* term,
    const char* indent
  )
{
  emit(s, indent);
  if (term->kind == Arcadia_VPL_IR_TerminatorKind_Return) {
    emit(s, "return");
    Arcadia_VPL_IR_Value* value = ((Arcadia_VPL_IR_ReturnTerminator*)term)->value;
    if (value) { emit(s, " "); operand(s, value); }
    emit(s, ";\n");
  } else if (term->kind == Arcadia_VPL_IR_TerminatorKind_Discard) {
    emit(s, "discard;\n"); emit(s, indent); returnDefault(s);
  } else {
    emit(s, "vpl_pc = ");
    if (term->kind == Arcadia_VPL_IR_TerminatorKind_Branch) number(s, "", blockIndex(s, ((Arcadia_VPL_IR_BranchTerminator*)term)->target));
    else {
      Arcadia_VPL_IR_ConditionalBranchTerminator* branch = (Arcadia_VPL_IR_ConditionalBranchTerminator*)term;
      emit(s, "("); operand(s, branch->condition); emit(s, ") ? "); number(s, "", blockIndex(s, branch->trueTarget)); number(s, " : ", blockIndex(s, branch->falseTarget));
    }
    emit(s, ";\n"); emit(s, indent); emit(s, "break;\n");
  }
}

static void
signature
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Function* function,
    Arcadia_SizeValue index
  )
{
  typeName(s, function->returnType); number(s, " vpl_f", index); emit(s, "(");
  for (Arcadia_SizeValue i = 0, n = size(s, function->parameters); i < n; ++i) {
    if (i) emit(s, ", ");
    declaration(s, ((Arcadia_VPL_IR_ReferenceValue*)at(s, function->parameters, i))->type, "vpl_p", i);
  }
  emit(s, ")");
}

static void
function
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Function* function,
    Arcadia_SizeValue index
  )
{
  s->function = function;
  samplerDefinitions(s);
  signature(s, function, index); emit(s, " {\n");
  Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)at(s, function->blocks, 0);
  for (Arcadia_SizeValue i = 0, n = size(s, entry->instructions); i < n; ++i) {
    Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(s, entry->instructions, i);
    if (base->kind != Arcadia_VPL_IR_InstructionKind_DeclareLocal) break;
    Arcadia_VPL_IR_DeclareLocalInstruction* local = (Arcadia_VPL_IR_DeclareLocalInstruction*)base;
    if (local->type->kind == Arcadia_VPL_IR_TypeKind_Sampler) continue;
    emit(s, "  "); declaration(s, local->type, "vpl_r", local->registerValue->index); emit(s, ";\n");
  }
  Arcadia_SizeValue blocks = size(s, function->blocks);
  Arcadia_BooleanValue direct = blocks == 1 && (entry->terminator->kind == Arcadia_VPL_IR_TerminatorKind_Return || entry->terminator->kind == Arcadia_VPL_IR_TerminatorKind_Discard);
  if (!direct) {
    if (blocks > INT_MAX) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)function, "GLSL 420: too many basic blocks\n");
    emit(s, "  int vpl_pc = 0;\n  while (true) {\n    switch (vpl_pc) {\n");
  }
  for (Arcadia_SizeValue q = 0; q < blocks; ++q) {
    Arcadia_VPL_IR_Block* block = (Arcadia_VPL_IR_Block*)at(s, function->blocks, q);
    if (!direct) { number(s, "      case ", q); emit(s, ": {\n"); }
    for (Arcadia_SizeValue i = 0, n = size(s, block->instructions); i < n; ++i) instruction(s, (Arcadia_VPL_IR_Instruction*)at(s, block->instructions, i), direct ? "  " : "        ");
    terminator(s, block->terminator, direct ? "  " : "        ");
    if (!direct) emit(s, "      }\n");
  }
  if (!direct) { emit(s, "      default: "); returnDefault(s); emit(s, "    }\n  }\n"); }
  emit(s, "}\n");
}

Arcadia_String*
_Arcadia_VPL_GLSL_print
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Stage* stage
  )
{
  s->stage = stage;
  s->output = (Arcadia_StringBuilder*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_StringBuilder_create(s->thread));
  emit(s, "#version 420 core\n");
  for (Arcadia_SizeValue i = 0, n = size(s, s->records); i < n; ++i) {
    Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)at(s, s->records, i);
    number(s, "struct vpl_R", i); emit(s, " {\n");
    for (Arcadia_SizeValue j = 0, m = size(s, record->fields); j < m; ++j) { emit(s, "  "); declaration(s, ((Arcadia_VPL_IR_Field*)at(s, record->fields, j))->type, "vpl_m", j); emit(s, ";\n"); }
    emit(s, "};\n");
  }
  Arcadia_List* interface = stage->kind == Arcadia_VPL_IR_Stage_Vertex ? s->artifact->vertexInterface : s->artifact->fragmentInterface;
  for (Arcadia_SizeValue i = 0, n = size(s, stage->interface); i < n; ++i) {
    Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(s, stage->interface, i);
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)at(s, interface, i);
    if (io->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position) continue;
    number(s, "layout(location = ", binding->location); emit(s, ") ");
    if (((stage->kind == Arcadia_VPL_IR_Stage_Vertex && io->direction == Arcadia_VPL_IR_StageIO_Direction_Out) || (stage->kind == Arcadia_VPL_IR_Stage_Fragment && io->direction == Arcadia_VPL_IR_StageIO_Direction_In)) && hasInteger(s, io->type)) emit(s, "flat ");
    emit(s, io->direction == Arcadia_VPL_IR_StageIO_Direction_In ? "in " : "out ");
    typeName(s, io->type->kind == Arcadia_VPL_IR_TypeKind_Array ? ((Arcadia_VPL_IR_ArrayType*)io->type)->element : io->type); emit(s, " "); cName(s, binding->glName);
    if (io->type->kind == Arcadia_VPL_IR_TypeKind_Array) { number(s, "[", ((Arcadia_VPL_IR_ArrayType*)io->type)->count); emit(s, "]"); }
    emit(s, ";\n");
  }
  for (Arcadia_SizeValue i = 0, n = size(s, s->module->uniforms); i < n; ++i) {
    Arcadia_VPL_IR_Uniform* uniform = (Arcadia_VPL_IR_Uniform*)at(s, s->module->uniforms, i);
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)at(s, s->artifact->uniforms, i);
    if (uniform->kind == Arcadia_VPL_IR_UniformKind_Record) {
      number(s, "layout(std140, binding = ", binding->blockBinding); emit(s, ") uniform "); cName(s, binding->glName); emit(s, " {\n");
      Arcadia_List* fields = ((Arcadia_VPL_IR_RecordUniform*)uniform)->recordType->fields;
      for (Arcadia_SizeValue j = 0, m = size(s, fields); j < m; ++j) { emit(s, "  "); declaration(s, ((Arcadia_VPL_IR_Field*)at(s, fields, j))->type, "vpl_m", j); emit(s, ";\n"); }
      number(s, "} vpl_u", i); emit(s, ";\n");
    } else {
      emit(s, "uniform ");
      declaration(s, binding->type, "vpl_u", i); emit(s, ";\n");
    }
  }
  for (Arcadia_SizeValue i = 0, n = size(s, stage->functions); i < n; ++i) { signature(s, (Arcadia_VPL_IR_Function*)at(s, stage->functions, i), i); emit(s, ";\n"); }
  for (Arcadia_SizeValue i = 0, n = size(s, stage->functions); i < n; ++i) function(s, (Arcadia_VPL_IR_Function*)at(s, stage->functions, i), i);
  emit(s, "void main() {\n  "); number(s, "vpl_f", functionIndex(s, stage->entryFunction)); emit(s, "();\n}\n");
  return (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_create(s->thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)s->output)));
}
