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
#include "Arcadia/VPL/GLSL/BackendInternal.h"
#include <string.h>
#include <limits.h>

Arcadia_Object*
_Arcadia_VPL_GLSL_own
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_Object* object
  )
{
  return Arcadia_VPL_Text_own(s->thread, s->memory, object);
}

void
_Arcadia_VPL_GLSL_error
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_Object* subject,
    const char* message
  )
{
  Arcadia_String* text = (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_createFromCxxString(s->thread, message));
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)_Arcadia_VPL_GLSL_own(s,
    (Arcadia_Object*)Arcadia_VPL_Diagnostics_VerificationDiagnostic_create(s->thread, text, subject));
  Arcadia_Languages_Diagnostics_add(s->thread, s->memory->diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_Thread_setStatus(s->thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(s->thread);
}

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

static Arcadia_String*
name
  (
    Arcadia_VPL_GLSL_State* s,
    const char* prefix,
    Arcadia_SizeValue index,
    Arcadia_BooleanValue numbered
  )
{
  Arcadia_StringBuilder* builder = (Arcadia_StringBuilder*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_StringBuilder_create(s->thread));
  Arcadia_VPL_Text_append(s->thread, builder, prefix, strlen(prefix));
  if (numbered) {
    Arcadia_String* suffix = (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_createFromSize(s->thread, index));
    Arcadia_StringBuilder_insertBack(s->thread, builder, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)suffix));
  }
  const char zero = 0;
  Arcadia_VPL_Text_append(s->thread, builder, &zero, 1);
  return (Arcadia_String*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_String_create(s->thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)builder)));
}

static void
collectType
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type,
    Arcadia_BooleanValue aggregate
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Sampler && aggregate) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)type, "GLSL 420: opaque types inside aggregates are unsupported\n");
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) {
    Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)type;
    if (array->count > INT_MAX || array->element->kind == Arcadia_VPL_IR_TypeKind_Array) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)type, "GLSL 420: oversized or multidimensional arrays are unsupported\n");
    collectType(s, array->element, Arcadia_BooleanValue_True);
  }
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_Value cached = Arcadia_Map_get(s->thread, s->recordIds, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type));
    if (Arcadia_Value_isSizeValue(&cached)) return;
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    if (!size(s, fields)) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)type, "GLSL 420: empty records are unsupported\n");
    for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) collectType(s, ((Arcadia_VPL_IR_Field*)at(s, fields, i))->type, Arcadia_BooleanValue_True);
    Arcadia_Map_set(s->thread, s->recordIds, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type), Arcadia_Value_makeSizeValue(size(s, s->records)), NULL, NULL);
    Arcadia_List_insertBackObjectReferenceValue(s->thread, s->records, (Arcadia_Object*)type);
  }
}

static Arcadia_BooleanValue
interfaceType
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type,
    Arcadia_BooleanValue aggregateAllowed,
    Arcadia_BooleanValue matrixAllowed
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Scalar) return ((Arcadia_VPL_IR_ScalarType*)type)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Boolean;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Vector) return ((Arcadia_VPL_IR_VectorType*)type)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Boolean;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Matrix) return matrixAllowed;
  if (!aggregateAllowed) return Arcadia_BooleanValue_False;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) return interfaceType(s, ((Arcadia_VPL_IR_ArrayType*)type)->element, aggregateAllowed, matrixAllowed);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) if (!interfaceType(s, ((Arcadia_VPL_IR_Field*)at(s, fields, i))->type, aggregateAllowed, matrixAllowed)) return Arcadia_BooleanValue_False;
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_Integer32Value
span
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Type* type
  )
{
  Arcadia_SizeValue count = 1;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Matrix) count = ((Arcadia_VPL_IR_MatrixType*)type)->columns;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Array) {
    Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)type;
    Arcadia_Integer32Value width = span(s, array->element);
    if (array->count > INT_MAX / (Arcadia_SizeValue)width) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)type, "GLSL 420: interface span is too large\n");
    count = array->count * width;
  }
  if (type->kind == Arcadia_VPL_IR_TypeKind_Record) {
    count = 0;
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
    for (Arcadia_SizeValue i = 0, n = size(s, fields); i < n; ++i) {
      Arcadia_Integer32Value width = span(s, ((Arcadia_VPL_IR_Field*)at(s, fields, i))->type);
      if (count > INT_MAX - (Arcadia_SizeValue)width) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)type, "GLSL 420: interface span is too large\n");
      count += width;
    }
  }
  return (Arcadia_Integer32Value)count;
}

static Arcadia_BooleanValue
overlaps
  (
    Arcadia_Integer32Value a,
    Arcadia_Integer32Value aw,
    Arcadia_Integer32Value b,
    Arcadia_Integer32Value bw
  )
{
  return a >= 0 && b >= 0 && (Arcadia_Integer64Value)a < (Arcadia_Integer64Value)b + bw && (Arcadia_Integer64Value)b < (Arcadia_Integer64Value)a + aw;
}

static void
resolveInterface
  (
    Arcadia_VPL_GLSL_State* s,
    Arcadia_VPL_IR_Stage* stage,
    Arcadia_List* bindings
  )
{
  for (Arcadia_SizeValue i = 0, n = size(s, stage->interface); i < n; ++i) {
    Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(s, stage->interface, i);
    collectType(s, io->type, Arcadia_BooleanValue_False);
    Arcadia_BooleanValue varying = (stage->kind == Arcadia_VPL_IR_Stage_Vertex) == (io->direction == Arcadia_VPL_IR_StageIO_Direction_Out);
    if (!varying && stage->kind == Arcadia_VPL_IR_Stage_Fragment && io->type->kind == Arcadia_VPL_IR_TypeKind_Array) {
      Arcadia_VPL_IR_Type* element = ((Arcadia_VPL_IR_ArrayType*)io->type)->element;
      if (element->kind != Arcadia_VPL_IR_TypeKind_Scalar && element->kind != Arcadia_VPL_IR_TypeKind_Vector) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)io, "GLSL 420: fragment output arrays require scalar/vector elements\n");
    }
    if (!interfaceType(s, io->type, varying || (stage->kind == Arcadia_VPL_IR_Stage_Fragment && io->type->kind == Arcadia_VPL_IR_TypeKind_Array), varying || stage->kind == Arcadia_VPL_IR_Stage_Vertex)) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)io, "GLSL 420: unsupported stage interface type\n");
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)_Arcadia_VPL_GLSL_newBinding(s->thread));
    binding->name = io->name;
    binding->type = io->type;
    binding->slot = i;
    binding->direction = io->direction;
    binding->semantics = io->semantics;
    binding->location = io->location;
    if (io->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position) binding->glName = name(s, stage->kind == Arcadia_VPL_IR_Stage_Vertex ? "gl_Position" : "gl_FragCoord", 0, Arcadia_BooleanValue_False);
    Arcadia_List_insertBackObjectReferenceValue(s->thread, bindings, (Arcadia_Object*)binding);
  }
  for (Arcadia_SizeValue i = 0, n = size(s, bindings); i < n; ++i) {
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)at(s, bindings, i);
    if (binding->glName) continue;
    Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(s, stage->interface, i);
    Arcadia_Integer32Value width = span(s, io->type);
    if (stage->kind == Arcadia_VPL_IR_Stage_Fragment && io->direction == Arcadia_VPL_IR_StageIO_Direction_In) {
      for (Arcadia_SizeValue j = 0, m = size(s, s->module->vertex->interface); j < m; ++j) {
        Arcadia_VPL_IR_StageIO* output = (Arcadia_VPL_IR_StageIO*)at(s, s->module->vertex->interface, j);
        if (output->direction != Arcadia_VPL_IR_StageIO_Direction_Out || output->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position) continue;
        if (io->location >= 0 ? output->location == io->location : output->location == -1 && Arcadia_String_isEqualTo_pn(s->thread, output->name, Arcadia_String_getBytes(s->thread, io->name), Arcadia_String_getNumberOfBytes(s->thread, io->name))) {
          binding->location = ((Arcadia_VPL_Backends_GLSL_Binding*)at(s, s->artifact->vertexInterface, j))->location;
          break;
        }
      }
    }
    if (binding->location < 0) {
      binding->location = 0;
      for (;;) {
        Arcadia_BooleanValue occupied = Arcadia_BooleanValue_False;
        for (Arcadia_SizeValue j = 0; j < n; ++j) {
          Arcadia_VPL_Backends_GLSL_Binding* other = (Arcadia_VPL_Backends_GLSL_Binding*)at(s, bindings, j);
          if (i == j || other->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position || other->direction != binding->direction) continue;
          if (overlaps(binding->location, width, other->location, span(s, ((Arcadia_VPL_IR_StageIO*)at(s, stage->interface, j))->type))) { occupied = Arcadia_BooleanValue_True; break; }
        }
        if (!occupied) break;
        if (binding->location == INT_MAX) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)io, "GLSL 420: no free interface location\n");
        ++binding->location;
      }
    }
    if (binding->location > INT_MAX - width) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)io, "GLSL 420: interface location range overflows\n");
    for (Arcadia_SizeValue j = 0; j < i; ++j) {
      Arcadia_VPL_Backends_GLSL_Binding* other = (Arcadia_VPL_Backends_GLSL_Binding*)at(s, bindings, j);
      if (other->semantics != Arcadia_VPL_IR_StageIO_Semantics_Position && other->direction == binding->direction && overlaps(binding->location, width, other->location, span(s, ((Arcadia_VPL_IR_StageIO*)at(s, stage->interface, j))->type))) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)io, "GLSL 420: overlapping interface location ranges\n");
    }
    const char* prefix = stage->kind == Arcadia_VPL_IR_Stage_Vertex && io->direction == Arcadia_VPL_IR_StageIO_Direction_In ? "vpl_attribute_" : stage->kind == Arcadia_VPL_IR_Stage_Fragment && io->direction == Arcadia_VPL_IR_StageIO_Direction_Out ? "vpl_output_" : "vpl_varying_";
    binding->glName = name(s, prefix, binding->location, Arcadia_BooleanValue_True);
    if (io->semantics == Arcadia_VPL_IR_StageIO_Semantics_FragmentColor) {
      s->artifact->fragmentColorOutput = binding->glName;
      s->artifact->fragmentColorLocation = binding->location;
    }
  }
  for (Arcadia_SizeValue i = 0, n = size(s, stage->functions); i < n; ++i) {
    Arcadia_VPL_IR_Function* function = (Arcadia_VPL_IR_Function*)at(s, stage->functions, i);
    collectType(s, function->returnType, Arcadia_BooleanValue_False);
    if (function->returnType->kind == Arcadia_VPL_IR_TypeKind_Sampler) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)function, "GLSL 420: sampler return values are unsupported\n");
    for (Arcadia_SizeValue j = 0, m = size(s, function->parameters); j < m; ++j) collectType(s, ((Arcadia_VPL_IR_ReferenceValue*)at(s, function->parameters, j))->type, Arcadia_BooleanValue_False);
    Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)at(s, function->blocks, 0);
    for (Arcadia_SizeValue j = 0, m = size(s, entry->instructions); j < m; ++j) {
      Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(s, entry->instructions, j);
      if (base->kind != Arcadia_VPL_IR_InstructionKind_DeclareLocal) break;
      collectType(s, ((Arcadia_VPL_IR_DeclareLocalInstruction*)base)->type, Arcadia_BooleanValue_False);
    }
  }
}

void
_Arcadia_VPL_GLSL_resolve
  (
    Arcadia_VPL_GLSL_State* s
  )
{
  Arcadia_Integer32Value blockBinding = 0;
  for (Arcadia_SizeValue i = 0, n = size(s, s->module->uniforms); i < n; ++i) {
    Arcadia_VPL_IR_Uniform* uniform = (Arcadia_VPL_IR_Uniform*)at(s, s->module->uniforms, i);
    Arcadia_VPL_IR_Type* type = uniform->kind == Arcadia_VPL_IR_UniformKind_Scalar ? ((Arcadia_VPL_IR_ScalarUniform*)uniform)->type : uniform->kind == Arcadia_VPL_IR_UniformKind_Record ? (Arcadia_VPL_IR_Type*)((Arcadia_VPL_IR_RecordUniform*)uniform)->recordType : ((Arcadia_VPL_IR_ArrayUniform*)uniform)->elementType;
    collectType(s, type, uniform->kind != Arcadia_VPL_IR_UniformKind_Scalar);
    if (uniform->kind == Arcadia_VPL_IR_UniformKind_Array && (((Arcadia_VPL_IR_ArrayUniform*)uniform)->count > INT_MAX || type->kind == Arcadia_VPL_IR_TypeKind_Array)) _Arcadia_VPL_GLSL_error(s, (Arcadia_Object*)uniform, "GLSL 420: oversized or multidimensional uniform array\n");
    Arcadia_VPL_Backends_GLSL_Binding* binding = (Arcadia_VPL_Backends_GLSL_Binding*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)_Arcadia_VPL_GLSL_newBinding(s->thread));
    binding->name = uniform->name;
    binding->type = type;
    if (uniform->kind == Arcadia_VPL_IR_UniformKind_Array) binding->type = (Arcadia_VPL_IR_Type*)_Arcadia_VPL_GLSL_own(s, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(s->thread, type, ((Arcadia_VPL_IR_ArrayUniform*)uniform)->count));
    binding->kind = uniform->kind;
    binding->slot = i;
    if (uniform->kind == Arcadia_VPL_IR_UniformKind_Record) {
      binding->glName = name(s, "vpl_Block", i, Arcadia_BooleanValue_True);
      binding->blockBinding = blockBinding++;
    } else binding->glName = name(s, "vpl_u", i, Arcadia_BooleanValue_True);
    Arcadia_List_insertBackObjectReferenceValue(s->thread, s->artifact->uniforms, (Arcadia_Object*)binding);
  }
  resolveInterface(s, s->module->vertex, s->artifact->vertexInterface);
  resolveInterface(s, s->module->fragment, s->artifact->fragmentInterface);
}
