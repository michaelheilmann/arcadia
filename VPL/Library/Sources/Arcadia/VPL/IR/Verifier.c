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
#include "Arcadia/VPL/IR/Include.h"
#include "Arcadia/VPL/IR/OutputVerification.h"
#include "Arcadia/VPL/Diagnostics/VerificationDiagnostic.h"
#include <string.h>
#include <math.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  );
static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.Verifier", Arcadia_VPL_IR_Verifier,
                         u8"Arcadia.Object", Arcadia_Object, &_typeOperations);

static void
reset
  (
    Arcadia_VPL_IR_Verifier* self
  )
{
  self->module = NULL;
  self->stage = NULL;
  self->function = NULL;
  self->registers = NULL;
  self->types = NULL;
  self->activeTypes = NULL;
  self->activeReferences = NULL;
  self->uniformTypes = NULL;
  self->blocks = NULL;
  self->normalReturns = NULL;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_Verifier);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->diagnostics = (Arcadia_Languages_Diagnostics*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_Diagnostics_getType(thread));
  reset(self);
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_Verifier);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  )
{
  if (self->diagnostics) Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  if (self->module) Arcadia_Object_visit(thread, (Arcadia_Object*)self->module);
  if (self->stage) Arcadia_Object_visit(thread, (Arcadia_Object*)self->stage);
  if (self->function) Arcadia_Object_visit(thread, (Arcadia_Object*)self->function);
  if (self->registers) Arcadia_Object_visit(thread, (Arcadia_Object*)self->registers);
  if (self->types) Arcadia_Object_visit(thread, (Arcadia_Object*)self->types);
  if (self->activeTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->activeTypes);
  if (self->activeReferences) Arcadia_Object_visit(thread, (Arcadia_Object*)self->activeReferences);
  if (self->uniformTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->uniformTypes);
  if (self->blocks) Arcadia_Object_visit(thread, (Arcadia_Object*)self->blocks);
  if (self->normalReturns) Arcadia_Object_visit(thread, (Arcadia_Object*)self->normalReturns);
}

Arcadia_VPL_IR_Verifier*
Arcadia_VPL_IR_Verifier_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_Verifier);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_IR_Verifier);
}

static void
fail
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_Object* subject,
    const char* message
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, subject);
  Arcadia_String* text = Arcadia_String_createFromCxxString(thread, message);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = Arcadia_VPL_Diagnostics_VerificationDiagnostic_create(thread, text, subject);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostic);
  Arcadia_Languages_Diagnostics_add(thread, self->diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

void
_Arcadia_VPL_IR_raiseVerificationError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_Object* subject,
    const char* message
  )
{
  fail(thread, self, subject, message);
}

static void
requireObject
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_Object* object,
    Arcadia_Type* type
  )
{
  if (!object || !Arcadia_Object_isInstanceOf(thread, object, type)) fail(thread, self, object, "verification error: missing object or incorrect object class\n");
}

static Arcadia_SizeValue
size
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
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_List* list,
    Arcadia_SizeValue index,
    Arcadia_Type* type
  )
{
  Arcadia_Value value = Arcadia_List_getAt(thread, list, index);
  if (!Arcadia_Value_isObjectReferenceValue(&value)) fail(thread, self, (Arcadia_Object*)list, "verification error: list element is not an object\n");
  Arcadia_Object* object = Arcadia_Value_getObjectReferenceValue(&value);
  requireObject(thread, self, object, type);
  return object;
}

static Arcadia_BooleanValue
sameName
  (
    Arcadia_Thread* thread,
    Arcadia_String* a,
    Arcadia_String* b
  )
{
  return Arcadia_String_isEqualTo_pn(thread, a, Arcadia_String_getBytes(thread, b), Arcadia_String_getNumberOfBytes(thread, b));
}

static void
checkName
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_String* name
  )
{
  requireObject(thread, self, (Arcadia_Object*)name, _Arcadia_String_getType(thread));
  if (!Arcadia_String_getNumberOfBytes(thread, name)) fail(thread, self, (Arcadia_Object*)name, "verification error: empty name\n");
}

static Arcadia_BooleanValue
sameType
  (
    Arcadia_VPL_IR_Type* a,
    Arcadia_VPL_IR_Type* b
  )
{
  if (a == b) return Arcadia_BooleanValue_True;
  if (a->kind != b->kind) return Arcadia_BooleanValue_False;
  switch (a->kind) {
    case Arcadia_VPL_IR_TypeKind_Void: return Arcadia_BooleanValue_True;
    case Arcadia_VPL_IR_TypeKind_Scalar: return ((Arcadia_VPL_IR_ScalarType*)a)->elementKind == ((Arcadia_VPL_IR_ScalarType*)b)->elementKind;
    case Arcadia_VPL_IR_TypeKind_Vector: return ((Arcadia_VPL_IR_VectorType*)a)->elementKind == ((Arcadia_VPL_IR_VectorType*)b)->elementKind && ((Arcadia_VPL_IR_VectorType*)a)->size == ((Arcadia_VPL_IR_VectorType*)b)->size;
    case Arcadia_VPL_IR_TypeKind_Matrix: return ((Arcadia_VPL_IR_MatrixType*)a)->elementKind == ((Arcadia_VPL_IR_MatrixType*)b)->elementKind && ((Arcadia_VPL_IR_MatrixType*)a)->rows == ((Arcadia_VPL_IR_MatrixType*)b)->rows && ((Arcadia_VPL_IR_MatrixType*)a)->columns == ((Arcadia_VPL_IR_MatrixType*)b)->columns;
    case Arcadia_VPL_IR_TypeKind_Sampler: return ((Arcadia_VPL_IR_SamplerType*)a)->samplerKind == ((Arcadia_VPL_IR_SamplerType*)b)->samplerKind;
    case Arcadia_VPL_IR_TypeKind_Array: return ((Arcadia_VPL_IR_ArrayType*)a)->count == ((Arcadia_VPL_IR_ArrayType*)b)->count && sameType(((Arcadia_VPL_IR_ArrayType*)a)->element, ((Arcadia_VPL_IR_ArrayType*)b)->element);
    default: return Arcadia_BooleanValue_False; // Records have nominal identity.
  }
}

static void
checkType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Type* type
  )
{
  requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_Type_getType(thread));
  if (Arcadia_List_contains(thread, self->types, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type))) return;
  if (Arcadia_List_contains(thread, self->activeTypes, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type))) fail(thread, self, (Arcadia_Object*)type, "verification error: recursive value type\n");
  Arcadia_List_insertBackObjectReferenceValue(thread, self->activeTypes, (Arcadia_Object*)type);
  switch (type->kind) {
    case Arcadia_VPL_IR_TypeKind_Void:
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_VoidType_getType(thread));
      break;
    case Arcadia_VPL_IR_TypeKind_Scalar:
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_ScalarType_getType(thread));
      if (((Arcadia_VPL_IR_ScalarType*)type)->elementKind > Arcadia_VPL_IR_ScalarElementKind_Floating) fail(thread, self, (Arcadia_Object*)type, "verification error: invalid scalar kind\n");
      break;
    case Arcadia_VPL_IR_TypeKind_Vector: {
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_VectorType_getType(thread));
      Arcadia_VPL_IR_VectorType* vector = (Arcadia_VPL_IR_VectorType*)type;
      if (vector->elementKind > Arcadia_VPL_IR_ScalarElementKind_Floating || vector->size < 2 || vector->size > 4) fail(thread, self, (Arcadia_Object*)type, "verification error: invalid vector shape\n");
    } break;
    case Arcadia_VPL_IR_TypeKind_Matrix: {
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_MatrixType_getType(thread));
      Arcadia_VPL_IR_MatrixType* matrix = (Arcadia_VPL_IR_MatrixType*)type;
      if (matrix->elementKind != Arcadia_VPL_IR_ScalarElementKind_Floating || matrix->rows < 2 || matrix->rows > 4 || matrix->columns < 2 || matrix->columns > 4) fail(thread, self, (Arcadia_Object*)type, "verification error: invalid matrix shape\n");
    } break;
    case Arcadia_VPL_IR_TypeKind_Sampler:
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_SamplerType_getType(thread));
      if (((Arcadia_VPL_IR_SamplerType*)type)->samplerKind != Arcadia_VPL_IR_SamplerKind_Sampler2D) fail(thread, self, (Arcadia_Object*)type, "verification error: unsupported sampler kind\n");
      break;
    case Arcadia_VPL_IR_TypeKind_Array: {
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_ArrayType_getType(thread));
      Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)type;
      checkType(thread, self, array->element);
      if (!array->count || array->element->kind == Arcadia_VPL_IR_TypeKind_Void) fail(thread, self, (Arcadia_Object*)type, "verification error: invalid array element or size\n");
    } break;
    case Arcadia_VPL_IR_TypeKind_Record: {
      requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_RecordType_getType(thread));
      Arcadia_VPL_IR_RecordType* record = (Arcadia_VPL_IR_RecordType*)type;
      checkName(thread, self, record->name);
      requireObject(thread, self, (Arcadia_Object*)record->fields, _Arcadia_List_getType(thread));
      for (Arcadia_SizeValue i = 0, n = size(thread, record->fields); i < n; ++i) {
        Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)at(thread, self, record->fields, i, _Arcadia_VPL_IR_Field_getType(thread));
        checkName(thread, self, field->name);
        checkType(thread, self, field->type);
        if (field->type->kind == Arcadia_VPL_IR_TypeKind_Void) fail(thread, self, (Arcadia_Object*)field, "verification error: void record field\n");
        for (Arcadia_SizeValue j = 0; j < i; ++j) {
          Arcadia_VPL_IR_Field* earlier = (Arcadia_VPL_IR_Field*)at(thread, self, record->fields, j, _Arcadia_VPL_IR_Field_getType(thread));
          if (sameName(thread, field->name, earlier->name)) fail(thread, self, (Arcadia_Object*)field, "verification error: duplicate record field\n");
        }
      }
    } break;
    default: fail(thread, self, (Arcadia_Object*)type, "verification error: unknown type kind\n");
  }
  Arcadia_List_removeBack(thread, self->activeTypes, 1);
  Arcadia_List_insertBackObjectReferenceValue(thread, self->types, (Arcadia_Object*)type);
}

static void
valueTypeRequired
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Type* type
  )
{
  checkType(thread, self, type);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Void) fail(thread, self, (Arcadia_Object*)type, "verification error: void is not a value type\n");
}

static void
equalTypes
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Type* a,
    Arcadia_VPL_IR_Type* b
  )
{
  checkType(thread, self, a);
  checkType(thread, self, b);
  if (!sameType(a, b)) fail(thread, self, (Arcadia_Object*)a, "verification error: operand or result type mismatch\n");
}

static Arcadia_BooleanValue
scalar
  (
    Arcadia_VPL_IR_Type* type,
    Arcadia_Natural8Value kind
  )
{
  return type->kind == Arcadia_VPL_IR_TypeKind_Scalar && ((Arcadia_VPL_IR_ScalarType*)type)->elementKind == kind;
}

static Arcadia_SizeValue
registerIndex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_SizeValue id
  )
{
  for (Arcadia_SizeValue i = 0, n = size(thread, self->registers); i < n; ++i) {
    Arcadia_VPL_IR_RegisterValue* reg = (Arcadia_VPL_IR_RegisterValue*)at(thread, self, self->registers, i, _Arcadia_VPL_IR_RegisterValue_getType(thread));
    if (reg->index == id) return i;
  }
  fail(thread, self, (Arcadia_Object*)self->function, "verification error: undeclared register\n");
  return 0;
}

static void
checkRegister
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_RegisterValue* reg,
    const unsigned char* assigned
  )
{
  requireObject(thread, self, (Arcadia_Object*)reg, _Arcadia_VPL_IR_RegisterValue_getType(thread));
  if (reg->_parent.kind != Arcadia_VPL_IR_ValueKind_Register) fail(thread, self, (Arcadia_Object*)reg, "verification error: inconsistent value kind\n");
  Arcadia_SizeValue i = registerIndex(thread, self, reg->index);
  Arcadia_VPL_IR_RegisterValue* declaration = (Arcadia_VPL_IR_RegisterValue*)at(thread, self, self->registers, i, _Arcadia_VPL_IR_RegisterValue_getType(thread));
  equalTypes(thread, self, reg->type, declaration->type);
  if (assigned && !assigned[i]) fail(thread, self, (Arcadia_Object*)reg, "verification error: register is not definitely assigned\n");
}

static Arcadia_VPL_IR_Type*
operand
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Value* value,
    const unsigned char* assigned
  )
{
  requireObject(thread, self, (Arcadia_Object*)value, _Arcadia_VPL_IR_Value_getType(thread));
  if (value->kind == Arcadia_VPL_IR_ValueKind_Register) {
    checkRegister(thread, self, (Arcadia_VPL_IR_RegisterValue*)value, assigned);
    return ((Arcadia_VPL_IR_RegisterValue*)value)->type;
  }
  if (value->kind == Arcadia_VPL_IR_ValueKind_Constant) {
    requireObject(thread, self, (Arcadia_Object*)value, _Arcadia_VPL_IR_ConstantValue_getType(thread));
    Arcadia_VPL_IR_ConstantValue* constant = (Arcadia_VPL_IR_ConstantValue*)value;
    valueTypeRequired(thread, self, constant->type);
    if (scalar(constant->type, Arcadia_VPL_IR_ScalarElementKind_Floating) && Arcadia_Value_isReal32Value(&constant->literal) && !isfinite(Arcadia_Value_getReal32Value(&constant->literal))) fail(thread, self, (Arcadia_Object*)value, "verification error: floating constants must be finite binary32 values\n");
    if ((scalar(constant->type, Arcadia_VPL_IR_ScalarElementKind_Boolean) && Arcadia_Value_isBooleanValue(&constant->literal)) ||
        (scalar(constant->type, Arcadia_VPL_IR_ScalarElementKind_Integer) && Arcadia_Value_isInteger32Value(&constant->literal)) ||
        (scalar(constant->type, Arcadia_VPL_IR_ScalarElementKind_Floating) && Arcadia_Value_isReal32Value(&constant->literal))) return constant->type;
    fail(thread, self, (Arcadia_Object*)value, "verification error: constant literal does not match scalar type\n");
  }
  fail(thread, self, (Arcadia_Object*)value, "verification error: computation requires a register or scalar constant\n");
  return NULL;
}

static void
checkReferenceShape
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_ReferenceValue* ref
  )
{
  requireObject(thread, self, (Arcadia_Object*)ref, _Arcadia_VPL_IR_ReferenceValue_getType(thread));
  if (ref->_parent.kind != Arcadia_VPL_IR_ValueKind_Reference) fail(thread, self, (Arcadia_Object*)ref, "verification error: inconsistent reference kind\n");
  Arcadia_BooleanValue member = Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)ref, _Arcadia_VPL_IR_MemberReferenceValue_getType(thread));
  Arcadia_BooleanValue index = Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)ref, _Arcadia_VPL_IR_IndexReferenceValue_getType(thread));
  if ((member && ref->storageKind != Arcadia_VPL_IR_StorageKind_Member) || (index && ref->storageKind != Arcadia_VPL_IR_StorageKind_Index)) fail(thread, self, (Arcadia_Object*)ref, "verification error: derived reference class and kind disagree\n");
  if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Member) requireObject(thread, self, (Arcadia_Object*)ref, _Arcadia_VPL_IR_MemberReferenceValue_getType(thread));
  if (ref->storageKind == Arcadia_VPL_IR_StorageKind_Index) requireObject(thread, self, (Arcadia_Object*)ref, _Arcadia_VPL_IR_IndexReferenceValue_getType(thread));
  if ((member || index) && ref->slot) fail(thread, self, (Arcadia_Object*)ref, "verification error: derived reference slot must be zero\n");
}

static void
referencePath
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_ReferenceValue* ref,
    Arcadia_BooleanValue write,
    Arcadia_BooleanValue partial,
    const unsigned char* assigned
  )
{
  checkReferenceShape(thread, self, ref);
  if (Arcadia_List_contains(thread, self->activeReferences, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)ref))) fail(thread, self, (Arcadia_Object*)ref, "verification error: cyclic storage reference\n");
  Arcadia_List_insertBackObjectReferenceValue(thread, self->activeReferences, (Arcadia_Object*)ref);
  valueTypeRequired(thread, self, ref->type);
  Arcadia_VPL_IR_Type* expected = NULL;
  switch (ref->storageKind) {
    case Arcadia_VPL_IR_StorageKind_Member: {
      Arcadia_VPL_IR_MemberReferenceValue* member = (Arcadia_VPL_IR_MemberReferenceValue*)ref;
      referencePath(thread, self, member->base, write, Arcadia_BooleanValue_True, assigned);
      checkName(thread, self, member->fieldName);
      if (member->base->type->kind != Arcadia_VPL_IR_TypeKind_Record) fail(thread, self, (Arcadia_Object*)ref, "verification error: member reference base is not record storage\n");
      Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)member->base->type)->fields;
      for (Arcadia_SizeValue i = 0, n = size(thread, fields); i < n; ++i) {
        Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)at(thread, self, fields, i, _Arcadia_VPL_IR_Field_getType(thread));
        if (sameName(thread, field->name, member->fieldName)) {
          expected = field->type;
          break;
        }
      }
      if (!expected) fail(thread, self, (Arcadia_Object*)ref, "verification error: unknown referenced record field\n");
    } break;
    case Arcadia_VPL_IR_StorageKind_Index: {
      Arcadia_VPL_IR_IndexReferenceValue* index = (Arcadia_VPL_IR_IndexReferenceValue*)ref;
      referencePath(thread, self, index->base, write, Arcadia_BooleanValue_True, assigned);
      Arcadia_VPL_IR_Type* subscript = operand(thread, self, index->index, assigned);
      if (index->base->type->kind != Arcadia_VPL_IR_TypeKind_Array || !scalar(subscript, Arcadia_VPL_IR_ScalarElementKind_Integer)) fail(thread, self, (Arcadia_Object*)ref, "verification error: index reference requires array storage and int\n");
      Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)index->base->type;
      expected = array->element;
      if (index->index->kind == Arcadia_VPL_IR_ValueKind_Constant) {
        Arcadia_Integer32Value value = Arcadia_Value_getInteger32Value(&((Arcadia_VPL_IR_ConstantValue*)index->index)->literal);
        if (value < 0 || (Arcadia_SizeValue)value >= array->count) fail(thread, self, (Arcadia_Object*)ref, "verification error: constant storage index is out of bounds\n");
      }
    } break;
    case Arcadia_VPL_IR_StorageKind_Local: {
      Arcadia_SizeValue i = registerIndex(thread, self, ref->slot);
      expected = ((Arcadia_VPL_IR_RegisterValue*)at(thread, self, self->registers, i, _Arcadia_VPL_IR_RegisterValue_getType(thread)))->type;
      if ((!write || partial) && assigned && !assigned[i]) fail(thread, self, (Arcadia_Object*)ref, "verification error: local storage is not definitely assigned\n");
    } break;
    case Arcadia_VPL_IR_StorageKind_Parameter:
      if (ref->slot >= size(thread, self->function->parameters)) fail(thread, self, (Arcadia_Object*)ref, "verification error: invalid parameter slot\n");
      expected = ((Arcadia_VPL_IR_ReferenceValue*)at(thread, self, self->function->parameters, ref->slot, _Arcadia_VPL_IR_ReferenceValue_getType(thread)))->type;
      break;
    case Arcadia_VPL_IR_StorageKind_Uniform:
      if (ref->slot >= size(thread, self->uniformTypes)) fail(thread, self, (Arcadia_Object*)ref, "verification error: invalid uniform slot\n");
      if (write) fail(thread, self, (Arcadia_Object*)ref, "verification error: uniform storage is read-only\n");
      expected = (Arcadia_VPL_IR_Type*)at(thread, self, self->uniformTypes, ref->slot, _Arcadia_VPL_IR_Type_getType(thread));
      break;
    case Arcadia_VPL_IR_StorageKind_StageIO: {
      if (ref->slot >= size(thread, self->stage->interface)) fail(thread, self, (Arcadia_Object*)ref, "verification error: invalid interface slot\n");
      Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(thread, self, self->stage->interface, ref->slot, _Arcadia_VPL_IR_StageIO_getType(thread));
      if (write && io->direction != Arcadia_VPL_IR_StageIO_Direction_Out) fail(thread, self, (Arcadia_Object*)ref, "verification error: stage input is read-only\n");
      expected = io->type;
    } break;
    default: fail(thread, self, (Arcadia_Object*)ref, "verification error: unknown storage namespace\n");
  }
  equalTypes(thread, self, ref->type, expected);
  Arcadia_List_removeBack(thread, self->activeReferences, 1);
}

static void
reference
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_ReferenceValue* ref,
    Arcadia_BooleanValue write,
    const unsigned char* assigned
  )
{
  referencePath(thread, self, ref, write, Arcadia_BooleanValue_False, assigned);
}

static Arcadia_SizeValue
blockIndex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Block* target
  )
{
  for (Arcadia_SizeValue i = 0, n = size(thread, self->function->blocks); i < n; ++i) {
    if ((Arcadia_Object*)target == at(thread, self, self->function->blocks, i, _Arcadia_VPL_IR_Block_getType(thread))) return i;
  }
  fail(thread, self, (Arcadia_Object*)target, "verification error: branch target is outside its function\n");
  return 0;
}

static Arcadia_BooleanValue
numeric
  (
    Arcadia_VPL_IR_Type* type
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Scalar) return !scalar(type, Arcadia_VPL_IR_ScalarElementKind_Boolean);
  if (type->kind == Arcadia_VPL_IR_TypeKind_Vector) return ((Arcadia_VPL_IR_VectorType*)type)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Boolean;
  return type->kind == Arcadia_VPL_IR_TypeKind_Matrix;
}

static void
binaryTypes
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_BinaryInstruction* instruction,
    Arcadia_VPL_IR_Type* lhs,
    Arcadia_VPL_IR_Type* rhs
  )
{
  Arcadia_VPL_IR_Type* result = instruction->result->type;
  Arcadia_Natural8Value op = instruction->operatorKind;
  if (op > Arcadia_VPL_IR_BinaryOperator_GreaterEqual) fail(thread, self, (Arcadia_Object*)instruction, "verification error: unknown binary operator\n");
  if (op >= Arcadia_VPL_IR_BinaryOperator_Equal) {
    equalTypes(thread, self, lhs, rhs);
    if (lhs->kind != Arcadia_VPL_IR_TypeKind_Scalar || !scalar(result, Arcadia_VPL_IR_ScalarElementKind_Boolean)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: comparison requires matching scalars and a bool result\n");
    if (op >= Arcadia_VPL_IR_BinaryOperator_Less && !numeric(lhs)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: ordered comparison requires numeric scalars\n");
    return;
  }
  if (!numeric(lhs) || !numeric(rhs)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: arithmetic requires numeric operands\n");
  if (op == Arcadia_VPL_IR_BinaryOperator_Divide && scalar(lhs, Arcadia_VPL_IR_ScalarElementKind_Integer) && scalar(rhs, Arcadia_VPL_IR_ScalarElementKind_Integer) && instruction->rhs->kind == Arcadia_VPL_IR_ValueKind_Constant) {
    Arcadia_Integer32Value divisor = Arcadia_Value_getInteger32Value(&((Arcadia_VPL_IR_ConstantValue*)instruction->rhs)->literal);
    if (!divisor) fail(thread, self, (Arcadia_Object*)instruction, "verification error: constant integer division by zero\n");
    if (divisor == -1 && instruction->lhs->kind == Arcadia_VPL_IR_ValueKind_Constant && Arcadia_Value_getInteger32Value(&((Arcadia_VPL_IR_ConstantValue*)instruction->lhs)->literal) == (-2147483647 - 1)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: constant integer division overflow\n");
  }
  if (op == Arcadia_VPL_IR_BinaryOperator_Multiply && lhs->kind == Arcadia_VPL_IR_TypeKind_Matrix) {
    Arcadia_VPL_IR_MatrixType* matrix = (Arcadia_VPL_IR_MatrixType*)lhs;
    if (rhs->kind == Arcadia_VPL_IR_TypeKind_Vector) {
      if (result->kind != Arcadia_VPL_IR_TypeKind_Vector || ((Arcadia_VPL_IR_VectorType*)rhs)->elementKind != matrix->elementKind || ((Arcadia_VPL_IR_VectorType*)result)->elementKind != matrix->elementKind || matrix->columns != ((Arcadia_VPL_IR_VectorType*)rhs)->size || matrix->rows != ((Arcadia_VPL_IR_VectorType*)result)->size) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incompatible matrix/vector multiplication\n");
      return;
    }
    if (rhs->kind == Arcadia_VPL_IR_TypeKind_Matrix) {
      Arcadia_VPL_IR_MatrixType* right = (Arcadia_VPL_IR_MatrixType*)rhs;
      if (result->kind != Arcadia_VPL_IR_TypeKind_Matrix || matrix->columns != right->rows || ((Arcadia_VPL_IR_MatrixType*)result)->rows != matrix->rows || ((Arcadia_VPL_IR_MatrixType*)result)->columns != right->columns) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incompatible matrix multiplication\n");
      return;
    }
  }
  if (sameType(lhs, rhs)) {
    equalTypes(thread, self, result, lhs);
    return;
  }
  if (op == Arcadia_VPL_IR_BinaryOperator_Multiply || op == Arcadia_VPL_IR_BinaryOperator_Divide) {
    if (scalar(rhs, Arcadia_VPL_IR_ScalarElementKind_Floating) && (lhs->kind == Arcadia_VPL_IR_TypeKind_Matrix || (lhs->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)lhs)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating))) {
      equalTypes(thread, self, result, lhs);
      return;
    }
    if (op == Arcadia_VPL_IR_BinaryOperator_Multiply && scalar(lhs, Arcadia_VPL_IR_ScalarElementKind_Floating) && (rhs->kind == Arcadia_VPL_IR_TypeKind_Matrix || (rhs->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)rhs)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating))) {
      equalTypes(thread, self, result, rhs);
      return;
    }
  }
  fail(thread, self, (Arcadia_Object*)instruction, "verification error: incompatible arithmetic types; conversions must be explicit\n");
}

static void
constructTypes
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_ConstructInstruction* instruction,
    const unsigned char* assigned
  )
{
  equalTypes(thread, self, instruction->result->type, instruction->type);
  requireObject(thread, self, (Arcadia_Object*)instruction->arguments, _Arcadia_List_getType(thread));
  Arcadia_SizeValue n = size(thread, instruction->arguments), components = 0;
  Arcadia_VPL_IR_Type* target = instruction->type;
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_VPL_IR_Value* argument = (Arcadia_VPL_IR_Value*)at(thread, self, instruction->arguments, i, _Arcadia_VPL_IR_Value_getType(thread));
    Arcadia_VPL_IR_Type* type = operand(thread, self, argument, assigned);
    if (target->kind == Arcadia_VPL_IR_TypeKind_Scalar) {
      if (n != 1 || (!sameType(target, type) && !(scalar(target, Arcadia_VPL_IR_ScalarElementKind_Floating) && scalar(type, Arcadia_VPL_IR_ScalarElementKind_Integer)))) fail(thread, self, (Arcadia_Object*)instruction, "verification error: invalid scalar construction or conversion\n");
    } else if (target->kind == Arcadia_VPL_IR_TypeKind_Vector || target->kind == Arcadia_VPL_IR_TypeKind_Matrix) {
      Arcadia_Natural8Value element = target->kind == Arcadia_VPL_IR_TypeKind_Vector ? ((Arcadia_VPL_IR_VectorType*)target)->elementKind : Arcadia_VPL_IR_ScalarElementKind_Floating;
      if (scalar(type, element) || (element == Arcadia_VPL_IR_ScalarElementKind_Floating && scalar(type, Arcadia_VPL_IR_ScalarElementKind_Integer))) ++components;
      else if (type->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)type)->elementKind == element) components += ((Arcadia_VPL_IR_VectorType*)type)->size;
      else fail(thread, self, (Arcadia_Object*)instruction, "verification error: incompatible construction component\n");
    } else if (target->kind == Arcadia_VPL_IR_TypeKind_Array) {
      if (n != ((Arcadia_VPL_IR_ArrayType*)target)->count) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect array constructor length\n");
      equalTypes(thread, self, type, ((Arcadia_VPL_IR_ArrayType*)target)->element);
    } else if (target->kind == Arcadia_VPL_IR_TypeKind_Record) {
      Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)target)->fields;
      if (n != size(thread, fields)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect record constructor length\n");
      equalTypes(thread, self, type, ((Arcadia_VPL_IR_Field*)at(thread, self, fields, i, _Arcadia_VPL_IR_Field_getType(thread)))->type);
    } else fail(thread, self, (Arcadia_Object*)instruction, "verification error: unsupported construction type\n");
  }
  if (target->kind == Arcadia_VPL_IR_TypeKind_Scalar && n != 1) fail(thread, self, (Arcadia_Object*)instruction, "verification error: scalar construction requires one operand\n");
  if (target->kind == Arcadia_VPL_IR_TypeKind_Vector && ((!n) || (components != ((Arcadia_VPL_IR_VectorType*)target)->size && !(n == 1 && components == 1)))) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect vector constructor length\n");
  if (target->kind == Arcadia_VPL_IR_TypeKind_Matrix && ((!n) || (components != ((Arcadia_VPL_IR_MatrixType*)target)->rows * ((Arcadia_VPL_IR_MatrixType*)target)->columns && !(n == 1 && components == 1)))) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect matrix constructor length\n");
  if (target->kind == Arcadia_VPL_IR_TypeKind_Array && n != ((Arcadia_VPL_IR_ArrayType*)target)->count) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect array constructor length\n");
  if (target->kind == Arcadia_VPL_IR_TypeKind_Record && n != size(thread, ((Arcadia_VPL_IR_RecordType*)target)->fields)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect record constructor length\n");
  if (target->kind == Arcadia_VPL_IR_TypeKind_Sampler) fail(thread, self, (Arcadia_Object*)instruction, "verification error: sampler construction is unsupported\n");
}

static void
callTypes
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_CallInstruction* instruction,
    const unsigned char* assigned
  )
{
  requireObject(thread, self, (Arcadia_Object*)instruction->arguments, _Arcadia_List_getType(thread));
  Arcadia_SizeValue n = size(thread, instruction->arguments);
  if (instruction->calleeFunction) {
    requireObject(thread, self, (Arcadia_Object*)instruction->calleeFunction, _Arcadia_VPL_IR_Function_getType(thread));
    if (instruction->builtin != Arcadia_VPL_IR_BuiltinFunction_None || !Arcadia_List_contains(thread, self->stage->functions, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)instruction->calleeFunction))) fail(thread, self, (Arcadia_Object*)instruction, "verification error: invalid or foreign callee\n");
    Arcadia_VPL_IR_Function* callee = instruction->calleeFunction;
    requireObject(thread, self, (Arcadia_Object*)callee->parameters, _Arcadia_List_getType(thread));
    checkType(thread, self, callee->returnType);
    if (n != size(thread, callee->parameters)) fail(thread, self, (Arcadia_Object*)instruction, "verification error: incorrect call argument count\n");
    for (Arcadia_SizeValue i = 0; i < n; ++i) {
      Arcadia_VPL_IR_ReferenceValue* parameter = (Arcadia_VPL_IR_ReferenceValue*)at(thread, self, callee->parameters, i, _Arcadia_VPL_IR_ReferenceValue_getType(thread));
      Arcadia_VPL_IR_Value* argument = (Arcadia_VPL_IR_Value*)at(thread, self, instruction->arguments, i, _Arcadia_VPL_IR_Value_getType(thread));
      equalTypes(thread, self, parameter->type, operand(thread, self, argument, assigned));
    }
    if (callee->returnType->kind == Arcadia_VPL_IR_TypeKind_Void) {
      if (instruction->result) fail(thread, self, (Arcadia_Object*)instruction, "verification error: void call must not have a result\n");
    } else {
      checkRegister(thread, self, instruction->result, NULL);
      equalTypes(thread, self, instruction->result->type, callee->returnType);
    }
  } else {
    if (instruction->builtin != Arcadia_VPL_IR_BuiltinFunction_Texture || n != 2) fail(thread, self, (Arcadia_Object*)instruction, "verification error: invalid built-in call\n");
    checkRegister(thread, self, instruction->result, NULL);
    Arcadia_VPL_IR_Type* sampler = operand(thread, self, (Arcadia_VPL_IR_Value*)at(thread, self, instruction->arguments, 0, _Arcadia_VPL_IR_Value_getType(thread)), assigned);
    Arcadia_VPL_IR_Type* coordinates = operand(thread, self, (Arcadia_VPL_IR_Value*)at(thread, self, instruction->arguments, 1, _Arcadia_VPL_IR_Value_getType(thread)), assigned);
    Arcadia_VPL_IR_Type* result = instruction->result->type;
    if (sampler->kind != Arcadia_VPL_IR_TypeKind_Sampler || coordinates->kind != Arcadia_VPL_IR_TypeKind_Vector || ((Arcadia_VPL_IR_VectorType*)coordinates)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Floating || ((Arcadia_VPL_IR_VectorType*)coordinates)->size != 2 || result->kind != Arcadia_VPL_IR_TypeKind_Vector || ((Arcadia_VPL_IR_VectorType*)result)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Floating || ((Arcadia_VPL_IR_VectorType*)result)->size != 4) fail(thread, self, (Arcadia_Object*)instruction, "verification error: texture requires sampler2D and vec2, producing vec4\n");
  }
}

static Arcadia_VPL_IR_RegisterValue*
instruction
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Instruction* base,
    unsigned char* assigned
  )
{
  Arcadia_VPL_IR_RegisterValue* result = NULL;
  switch (base->kind) {
    case Arcadia_VPL_IR_InstructionKind_DeclareLocal:
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_DeclareLocalInstruction_getType(thread));
      return NULL;
    case Arcadia_VPL_IR_InstructionKind_Copy: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_CopyInstruction_getType(thread));
      Arcadia_VPL_IR_CopyInstruction* copy = (Arcadia_VPL_IR_CopyInstruction*)base;
      result = copy->result;
      checkRegister(thread, self, result, NULL);
      equalTypes(thread, self, result->type, operand(thread, self, copy->source, assigned));
    } break;
    case Arcadia_VPL_IR_InstructionKind_Load: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_LoadInstruction_getType(thread));
      Arcadia_VPL_IR_LoadInstruction* load = (Arcadia_VPL_IR_LoadInstruction*)base;
      result = load->result;
      checkRegister(thread, self, result, NULL);
      reference(thread, self, load->source, Arcadia_BooleanValue_False, assigned);
      equalTypes(thread, self, result->type, load->source->type);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Store: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_StoreInstruction_getType(thread));
      Arcadia_VPL_IR_StoreInstruction* store = (Arcadia_VPL_IR_StoreInstruction*)base;
      reference(thread, self, store->target, Arcadia_BooleanValue_True, assigned);
      equalTypes(thread, self, store->target->type, operand(thread, self, store->value, assigned));
      if (assigned && store->target->storageKind == Arcadia_VPL_IR_StorageKind_Local) assigned[registerIndex(thread, self, store->target->slot)] = 1;
    } break;
    case Arcadia_VPL_IR_InstructionKind_Binary: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_BinaryInstruction_getType(thread));
      Arcadia_VPL_IR_BinaryInstruction* binary = (Arcadia_VPL_IR_BinaryInstruction*)base;
      result = binary->result;
      checkRegister(thread, self, result, NULL);
      Arcadia_VPL_IR_Type* lhs = operand(thread, self, binary->lhs, assigned);
      Arcadia_VPL_IR_Type* rhs = operand(thread, self, binary->rhs, assigned);
      binaryTypes(thread, self, binary, lhs, rhs);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Construct: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_ConstructInstruction_getType(thread));
      result = ((Arcadia_VPL_IR_ConstructInstruction*)base)->result;
      checkRegister(thread, self, result, NULL);
      constructTypes(thread, self, (Arcadia_VPL_IR_ConstructInstruction*)base, assigned);
    } break;
    case Arcadia_VPL_IR_InstructionKind_Call:
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_CallInstruction_getType(thread));
      callTypes(thread, self, (Arcadia_VPL_IR_CallInstruction*)base, assigned);
      result = ((Arcadia_VPL_IR_CallInstruction*)base)->result;
      break;
    case Arcadia_VPL_IR_InstructionKind_Swizzle: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_SwizzleInstruction_getType(thread));
      Arcadia_VPL_IR_SwizzleInstruction* swizzle = (Arcadia_VPL_IR_SwizzleInstruction*)base;
      result = swizzle->result;
      checkRegister(thread, self, result, NULL);
      Arcadia_VPL_IR_Type* source = operand(thread, self, swizzle->source, assigned);
      checkName(thread, self, swizzle->pattern);
      Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, swizzle->pattern);
      if (source->kind != Arcadia_VPL_IR_TypeKind_Vector || n > 4) fail(thread, self, (Arcadia_Object*)base, "verification error: invalid swizzle source or length\n");
      const char* bytes = (const char*)Arcadia_String_getBytes(thread, swizzle->pattern);
      for (Arcadia_SizeValue i = 0; i < n; ++i) {
        Arcadia_SizeValue j = 0;
        while (j < 4 && "xyzw"[j] != bytes[i]) ++j;
        if (j >= ((Arcadia_VPL_IR_VectorType*)source)->size) fail(thread, self, (Arcadia_Object*)base, "verification error: swizzle component is outside vector\n");
      }
      Arcadia_Natural8Value element = ((Arcadia_VPL_IR_VectorType*)source)->elementKind;
      if ((n == 1 && !scalar(result->type, element)) || (n > 1 && (result->type->kind != Arcadia_VPL_IR_TypeKind_Vector || ((Arcadia_VPL_IR_VectorType*)result->type)->elementKind != element || ((Arcadia_VPL_IR_VectorType*)result->type)->size != n))) fail(thread, self, (Arcadia_Object*)base, "verification error: incorrect swizzle result type\n");
    } break;
    case Arcadia_VPL_IR_InstructionKind_Member: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_MemberInstruction_getType(thread));
      Arcadia_VPL_IR_MemberInstruction* member = (Arcadia_VPL_IR_MemberInstruction*)base;
      result = member->result;
      checkRegister(thread, self, result, NULL);
      Arcadia_VPL_IR_Type* source = operand(thread, self, member->source, assigned);
      checkName(thread, self, member->fieldName);
      if (source->kind != Arcadia_VPL_IR_TypeKind_Record) fail(thread, self, (Arcadia_Object*)base, "verification error: member source is not a record\n");
      Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)source)->fields;
      Arcadia_BooleanValue found = Arcadia_BooleanValue_False;
      for (Arcadia_SizeValue i = 0, n = size(thread, fields); i < n; ++i) {
        Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)at(thread, self, fields, i, _Arcadia_VPL_IR_Field_getType(thread));
        if (sameName(thread, field->name, member->fieldName)) {
          equalTypes(thread, self, result->type, field->type);
          found = Arcadia_BooleanValue_True;
          break;
        }
      }
      if (!found) fail(thread, self, (Arcadia_Object*)base, "verification error: unknown record field\n");
    } break;
    case Arcadia_VPL_IR_InstructionKind_Index: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_IndexInstruction_getType(thread));
      Arcadia_VPL_IR_IndexInstruction* index = (Arcadia_VPL_IR_IndexInstruction*)base;
      result = index->result;
      checkRegister(thread, self, result, NULL);
      Arcadia_VPL_IR_Type* source = operand(thread, self, index->source, assigned);
      Arcadia_VPL_IR_Type* subscript = operand(thread, self, index->index, assigned);
      if (source->kind != Arcadia_VPL_IR_TypeKind_Array || !scalar(subscript, Arcadia_VPL_IR_ScalarElementKind_Integer)) fail(thread, self, (Arcadia_Object*)base, "verification error: indexing requires array and int\n");
      equalTypes(thread, self, result->type, ((Arcadia_VPL_IR_ArrayType*)source)->element);
      if (index->index->kind == Arcadia_VPL_IR_ValueKind_Constant) {
        Arcadia_Integer32Value value = Arcadia_Value_getInteger32Value(&((Arcadia_VPL_IR_ConstantValue*)index->index)->literal);
        if (value < 0 || (Arcadia_SizeValue)value >= ((Arcadia_VPL_IR_ArrayType*)source)->count) fail(thread, self, (Arcadia_Object*)base, "verification error: constant array index is out of bounds\n");
      }
    } break;
    default: fail(thread, self, (Arcadia_Object*)base, "verification error: unknown instruction; discard must be a terminator\n");
  }
  if (assigned && result) assigned[registerIndex(thread, self, result->index)] = 1;
  return result;
}

static void
terminator
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Terminator* base,
    const unsigned char* assigned
  )
{
  requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_Terminator_getType(thread));
  switch (base->kind) {
    case Arcadia_VPL_IR_TerminatorKind_Branch:
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_BranchTerminator_getType(thread));
      blockIndex(thread, self, ((Arcadia_VPL_IR_BranchTerminator*)base)->target);
      break;
    case Arcadia_VPL_IR_TerminatorKind_ConditionalBranch: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_ConditionalBranchTerminator_getType(thread));
      Arcadia_VPL_IR_ConditionalBranchTerminator* branch = (Arcadia_VPL_IR_ConditionalBranchTerminator*)base;
      if (!scalar(operand(thread, self, branch->condition, assigned), Arcadia_VPL_IR_ScalarElementKind_Boolean)) fail(thread, self, (Arcadia_Object*)base, "verification error: branch condition must be bool\n");
      blockIndex(thread, self, branch->trueTarget);
      blockIndex(thread, self, branch->falseTarget);
    } break;
    case Arcadia_VPL_IR_TerminatorKind_Return: {
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_ReturnTerminator_getType(thread));
      Arcadia_VPL_IR_Value* value = ((Arcadia_VPL_IR_ReturnTerminator*)base)->value;
      if (self->function->returnType->kind == Arcadia_VPL_IR_TypeKind_Void) {
        if (value) fail(thread, self, (Arcadia_Object*)base, "verification error: void return must not supply a value\n");
      } else equalTypes(thread, self, self->function->returnType, operand(thread, self, value, assigned));
    } break;
    case Arcadia_VPL_IR_TerminatorKind_Discard:
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_DiscardTerminator_getType(thread));
      if (self->stage->kind != Arcadia_VPL_IR_Stage_Fragment) fail(thread, self, (Arcadia_Object*)base, "verification error: discard is fragment-only\n");
      break;
    default: fail(thread, self, (Arcadia_Object*)base, "verification error: unknown terminator\n");
  }
}

static Arcadia_VPL_IR_Block*
blockAt
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_SizeValue index
  )
{
  return (Arcadia_VPL_IR_Block*)at(thread, self, self->function->blocks, index, _Arcadia_VPL_IR_Block_getType(thread));
}

static Arcadia_BooleanValue
continuesAfter
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Instruction* base
  )
{
  if (base->kind != Arcadia_VPL_IR_InstructionKind_Call || !((Arcadia_VPL_IR_CallInstruction*)base)->calleeFunction) return Arcadia_BooleanValue_True;
  Arcadia_Value value = Arcadia_Map_get(thread, self->normalReturns, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)((Arcadia_VPL_IR_CallInstruction*)base)->calleeFunction));
  if (!Arcadia_Value_isBooleanValue(&value)) fail(thread, self, (Arcadia_Object*)base, "verification error: missing helper return summary\n");
  return Arcadia_Value_getBooleanValue(&value);
}

static Arcadia_BooleanValue
edge
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_SizeValue from,
    Arcadia_SizeValue to
  )
{
  Arcadia_VPL_IR_Block* block = blockAt(thread, self, from);
  for (Arcadia_SizeValue i = 0, n = size(thread, block->instructions); i < n; ++i) {
    if (!continuesAfter(thread, self, (Arcadia_VPL_IR_Instruction*)at(thread, self, block->instructions, i, _Arcadia_VPL_IR_Instruction_getType(thread)))) return Arcadia_BooleanValue_False;
  }
  Arcadia_VPL_IR_Terminator* term = block->terminator;
  Arcadia_VPL_IR_Block* target = blockAt(thread, self, to);
  if (term->kind == Arcadia_VPL_IR_TerminatorKind_Branch) return ((Arcadia_VPL_IR_BranchTerminator*)term)->target == target;
  if (term->kind == Arcadia_VPL_IR_TerminatorKind_ConditionalBranch) return ((Arcadia_VPL_IR_ConditionalBranchTerminator*)term)->trueTarget == target || ((Arcadia_VPL_IR_ConditionalBranchTerminator*)term)->falseTarget == target;
  return Arcadia_BooleanValue_False;
}

static void
markWrites
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Block* block,
    unsigned char* assigned
  )
{
  for (Arcadia_SizeValue i = 0, n = size(thread, block->instructions); i < n; ++i) {
    Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(thread, self, block->instructions, i, _Arcadia_VPL_IR_Instruction_getType(thread));
    Arcadia_VPL_IR_RegisterValue* result = instruction(thread, self, base, NULL);
    if (!continuesAfter(thread, self, base)) break;
    if (result) assigned[registerIndex(thread, self, result->index)] = 1;
    else if (base->kind == Arcadia_VPL_IR_InstructionKind_Store && ((Arcadia_VPL_IR_StoreInstruction*)base)->target->storageKind == Arcadia_VPL_IR_StorageKind_Local) assigned[registerIndex(thread, self, ((Arcadia_VPL_IR_StoreInstruction*)base)->target->slot)] = 1;
  }
}

static void
dataFlow
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  )
{
  Arcadia_SizeValue b = size(thread, self->function->blocks), r = size(thread, self->registers);
  if (r > (((Arcadia_SizeValue)-1) - 1) / 2 || b > (((Arcadia_SizeValue)-1) - r) / (2 * r + 1)) fail(thread, self, (Arcadia_Object*)self->function, "verification error: data-flow graph is too large\n");
  unsigned char* volatile allocation = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    allocation = (unsigned char*)Arcadia_Memory_allocateUnmanaged(thread, b * (2 * r + 1) + r);
    unsigned char* in = allocation;
    unsigned char* out = in + b * r;
    unsigned char* reachable = out + b * r;
    unsigned char* work = reachable + b;
    memset(reachable, 0, b);
    reachable[0] = 1;
    Arcadia_BooleanValue changed;
    do {
      changed = Arcadia_BooleanValue_False;
      for (Arcadia_SizeValue p = 0; p < b; ++p) {
        if (!reachable[p]) continue;
        for (Arcadia_SizeValue q = 0; q < b; ++q) {
          if (!reachable[q] && edge(thread, self, p, q)) { reachable[q] = 1; changed = Arcadia_BooleanValue_True; }
        }
      }
    } while (changed);
    // Must-analysis starts at the greatest set, except the invocation's empty entry state.
    memset(in, 1, b * r);
    memset(out, 1, b * r);
    memset(in, 0, r);
    memset(out, 0, r);
    markWrites(thread, self, blockAt(thread, self, 0), out);
    do {
      changed = Arcadia_BooleanValue_False;
      for (Arcadia_SizeValue q = 0; q < b; ++q) {
        if (!reachable[q]) continue;
        memset(work, q ? 1 : 0, r);
        if (q) {
          for (Arcadia_SizeValue p = 0; p < b; ++p) {
            if (reachable[p] && edge(thread, self, p, q)) {
              for (Arcadia_SizeValue j = 0; j < r; ++j) work[j] &= out[p * r + j];
            }
          }
        }
        if (memcmp(in + q * r, work, r)) { memcpy(in + q * r, work, r); changed = Arcadia_BooleanValue_True; }
        markWrites(thread, self, blockAt(thread, self, q), work);
        if (memcmp(out + q * r, work, r)) { memcpy(out + q * r, work, r); changed = Arcadia_BooleanValue_True; }
      }
    } while (changed);
    // Now inspect each read in instruction order, using the fixed-point incoming state.
    for (Arcadia_SizeValue q = 0; q < b; ++q) {
      if (!reachable[q]) continue;
      memcpy(work, in + q * r, r);
      Arcadia_VPL_IR_Block* block = blockAt(thread, self, q);
      Arcadia_BooleanValue continuation = Arcadia_BooleanValue_True;
      for (Arcadia_SizeValue i = 0, n = size(thread, block->instructions); i < n; ++i) {
        Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(thread, self, block->instructions, i, _Arcadia_VPL_IR_Instruction_getType(thread));
        instruction(thread, self, base, work);
        if (!continuesAfter(thread, self, base)) { continuation = Arcadia_BooleanValue_False; break; }
      }
      if (continuation) terminator(thread, self, block->terminator, work);
    }
    Arcadia_Memory_deallocateUnmanaged(thread, allocation);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (allocation) Arcadia_Memory_deallocateUnmanaged(thread, allocation);
    Arcadia_Thread_jump(thread);
  }
}

static void
function
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Function* function
  )
{
  self->function = function;
  checkName(thread, self, function->name);
  checkType(thread, self, function->returnType);
  requireObject(thread, self, (Arcadia_Object*)function->parameters, _Arcadia_List_getType(thread));
  requireObject(thread, self, (Arcadia_Object*)function->blocks, _Arcadia_List_getType(thread));
  for (Arcadia_SizeValue i = 0, n = size(thread, function->parameters); i < n; ++i) {
    Arcadia_VPL_IR_ReferenceValue* parameter = (Arcadia_VPL_IR_ReferenceValue*)at(thread, self, function->parameters, i, _Arcadia_VPL_IR_ReferenceValue_getType(thread));
    checkReferenceShape(thread, self, parameter);
    valueTypeRequired(thread, self, parameter->type);
    if (parameter->_parent.kind != Arcadia_VPL_IR_ValueKind_Reference || parameter->storageKind != Arcadia_VPL_IR_StorageKind_Parameter || parameter->slot != i) fail(thread, self, (Arcadia_Object*)parameter, "verification error: parameters must use their ordered parameter slots\n");
  }
  Arcadia_SizeValue b = size(thread, function->blocks);
  if (!b) fail(thread, self, (Arcadia_Object*)function, "verification error: function has no entry block\n");
  self->registers = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_BooleanValue declarationsFinished = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue q = 0; q < b; ++q) {
    Arcadia_VPL_IR_Block* block = blockAt(thread, self, q);
    if (Arcadia_List_contains(thread, self->blocks, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)block))) fail(thread, self, (Arcadia_Object*)block, "verification error: block has duplicate or multiple function ownership\n");
    Arcadia_List_insertBackObjectReferenceValue(thread, self->blocks, (Arcadia_Object*)block);
    requireObject(thread, self, (Arcadia_Object*)block->instructions, _Arcadia_List_getType(thread));
    for (Arcadia_SizeValue i = 0, n = size(thread, block->instructions); i < n; ++i) {
      Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(thread, self, block->instructions, i, _Arcadia_VPL_IR_Instruction_getType(thread));
      if (base->kind != Arcadia_VPL_IR_InstructionKind_DeclareLocal) { declarationsFinished = Arcadia_BooleanValue_True; continue; }
      requireObject(thread, self, (Arcadia_Object*)base, _Arcadia_VPL_IR_DeclareLocalInstruction_getType(thread));
      Arcadia_VPL_IR_DeclareLocalInstruction* declaration = (Arcadia_VPL_IR_DeclareLocalInstruction*)base;
      if (q || declarationsFinished) fail(thread, self, (Arcadia_Object*)base, "verification error: register declarations must form the entry-block prefix\n");
      requireObject(thread, self, (Arcadia_Object*)declaration->registerValue, _Arcadia_VPL_IR_RegisterValue_getType(thread));
      if (declaration->registerValue->_parent.kind != Arcadia_VPL_IR_ValueKind_Register) fail(thread, self, (Arcadia_Object*)base, "verification error: inconsistent declared register kind\n");
      valueTypeRequired(thread, self, declaration->type);
      equalTypes(thread, self, declaration->type, declaration->registerValue->type);
      for (Arcadia_SizeValue j = 0, m = size(thread, self->registers); j < m; ++j) {
        if (((Arcadia_VPL_IR_RegisterValue*)at(thread, self, self->registers, j, _Arcadia_VPL_IR_RegisterValue_getType(thread)))->index == declaration->registerValue->index) fail(thread, self, (Arcadia_Object*)base, "verification error: duplicate register declaration\n");
      }
      Arcadia_List_insertBackObjectReferenceValue(thread, self->registers, (Arcadia_Object*)declaration->registerValue);
    }
  }
  // Validate every block structurally, including unreachable ones, before following any edges.
  for (Arcadia_SizeValue q = 0; q < b; ++q) {
    Arcadia_VPL_IR_Block* block = blockAt(thread, self, q);
    for (Arcadia_SizeValue i = 0, n = size(thread, block->instructions); i < n; ++i) {
      instruction(thread, self, (Arcadia_VPL_IR_Instruction*)at(thread, self, block->instructions, i, _Arcadia_VPL_IR_Instruction_getType(thread)), NULL);
    }
    terminator(thread, self, block->terminator, NULL);
  }
  self->registers = NULL;
  self->function = NULL;
}

void
_Arcadia_VPL_IR_verifyFunctionFlow
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Function* function
  )
{
  self->function = function;
  self->registers = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_VPL_IR_Block* entry = blockAt(thread, self, 0);
  for (Arcadia_SizeValue i = 0, n = size(thread, entry->instructions); i < n; ++i) {
    Arcadia_VPL_IR_Instruction* base = (Arcadia_VPL_IR_Instruction*)at(thread, self, entry->instructions, i, _Arcadia_VPL_IR_Instruction_getType(thread));
    if (base->kind != Arcadia_VPL_IR_InstructionKind_DeclareLocal) break;
    Arcadia_List_insertBackObjectReferenceValue(thread, self->registers, (Arcadia_Object*)((Arcadia_VPL_IR_DeclareLocalInstruction*)base)->registerValue);
  }
  dataFlow(thread, self);
  self->registers = NULL;
  self->function = NULL;
}

static void
stage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Stage* stage,
    Arcadia_Natural8Value kind
  )
{
  requireObject(thread, self, (Arcadia_Object*)stage, _Arcadia_VPL_IR_Stage_getType(thread));
  if (stage->kind != kind) fail(thread, self, (Arcadia_Object*)stage, "verification error: incorrect stage kind\n");
  self->stage = stage;
  requireObject(thread, self, (Arcadia_Object*)stage->interface, _Arcadia_List_getType(thread));
  requireObject(thread, self, (Arcadia_Object*)stage->functions, _Arcadia_List_getType(thread));
  if (!stage->entryFunction) fail(thread, self, (Arcadia_Object*)stage, "verification error: stage has no explicit entry function\n");
  requireObject(thread, self, (Arcadia_Object*)stage->entryFunction, _Arcadia_VPL_IR_Function_getType(thread));
  if (!Arcadia_List_contains(thread, stage->functions, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)stage->entryFunction))) fail(thread, self, (Arcadia_Object*)stage, "verification error: entry function does not belong to its stage\n");
  checkType(thread, self, stage->entryFunction->returnType);
  requireObject(thread, self, (Arcadia_Object*)stage->entryFunction->parameters, _Arcadia_List_getType(thread));
  if (stage->entryFunction->returnType->kind != Arcadia_VPL_IR_TypeKind_Void || size(thread, stage->entryFunction->parameters)) fail(thread, self, (Arcadia_Object*)stage->entryFunction, "verification error: stage entry must return void and have no parameters\n");
  for (Arcadia_SizeValue i = 0, n = size(thread, stage->interface); i < n; ++i) {
    Arcadia_VPL_IR_StageIO* io = (Arcadia_VPL_IR_StageIO*)at(thread, self, stage->interface, i, _Arcadia_VPL_IR_StageIO_getType(thread));
    checkName(thread, self, io->name);
    valueTypeRequired(thread, self, io->type);
    if (io->direction > Arcadia_VPL_IR_StageIO_Direction_Out || io->semantics > Arcadia_VPL_IR_StageIO_Semantics_FragmentColor || io->location < -1) fail(thread, self, (Arcadia_Object*)io, "verification error: invalid interface metadata\n");
    if (io->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position || io->semantics == Arcadia_VPL_IR_StageIO_Semantics_FragmentColor) {
      if (io->type->kind != Arcadia_VPL_IR_TypeKind_Vector || ((Arcadia_VPL_IR_VectorType*)io->type)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Floating || ((Arcadia_VPL_IR_VectorType*)io->type)->size != 4) fail(thread, self, (Arcadia_Object*)io, "verification error: position/color output semantics require vec4\n");
      if ((io->semantics == Arcadia_VPL_IR_StageIO_Semantics_FragmentColor && (kind != Arcadia_VPL_IR_Stage_Fragment || io->direction != Arcadia_VPL_IR_StageIO_Direction_Out)) || (io->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position && io->direction != (kind == Arcadia_VPL_IR_Stage_Vertex ? Arcadia_VPL_IR_StageIO_Direction_Out : Arcadia_VPL_IR_StageIO_Direction_In))) fail(thread, self, (Arcadia_Object*)io, "verification error: invalid built-in interface direction\n");
      if (io->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position && io->location != -1) fail(thread, self, (Arcadia_Object*)io, "verification error: built-in Position must not have a user location\n");
    }
    for (Arcadia_SizeValue j = 0; j < i; ++j) {
      Arcadia_VPL_IR_StageIO* earlier = (Arcadia_VPL_IR_StageIO*)at(thread, self, stage->interface, j, _Arcadia_VPL_IR_StageIO_getType(thread));
      if (sameName(thread, io->name, earlier->name) || (io->direction == earlier->direction && ((io->location >= 0 && io->location == earlier->location) || (io->semantics != Arcadia_VPL_IR_StageIO_Semantics_None && io->semantics == earlier->semantics)))) fail(thread, self, (Arcadia_Object*)io, "verification error: duplicate interface name, location, or semantics\n");
    }
  }
  for (Arcadia_SizeValue i = 0, n = size(thread, stage->functions); i < n; ++i) {
    Arcadia_VPL_IR_Function* current = (Arcadia_VPL_IR_Function*)at(thread, self, stage->functions, i, _Arcadia_VPL_IR_Function_getType(thread));
    checkName(thread, self, current->name);
    for (Arcadia_SizeValue j = 0; j < i; ++j) {
      Arcadia_VPL_IR_Function* earlier = (Arcadia_VPL_IR_Function*)at(thread, self, stage->functions, j, _Arcadia_VPL_IR_Function_getType(thread));
      if (sameName(thread, current->name, earlier->name)) fail(thread, self, (Arcadia_Object*)current, "verification error: duplicate function name\n");
    }
    function(thread, self, current);
  }
}

static void
linkInterfaces
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  )
{
  Arcadia_List* inputs = self->module->fragment->interface;
  Arcadia_List* outputs = self->module->vertex->interface;
  for (Arcadia_SizeValue i = 0, n = size(thread, inputs); i < n; ++i) {
    Arcadia_VPL_IR_StageIO* input = (Arcadia_VPL_IR_StageIO*)at(thread, self, inputs, i, _Arcadia_VPL_IR_StageIO_getType(thread));
    if (input->direction != Arcadia_VPL_IR_StageIO_Direction_In || input->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position) continue;
    Arcadia_VPL_IR_StageIO* match = NULL;
    for (Arcadia_SizeValue j = 0, m = size(thread, outputs); j < m; ++j) {
      Arcadia_VPL_IR_StageIO* output = (Arcadia_VPL_IR_StageIO*)at(thread, self, outputs, j, _Arcadia_VPL_IR_StageIO_getType(thread));
      if (output->direction != Arcadia_VPL_IR_StageIO_Direction_Out || output->semantics == Arcadia_VPL_IR_StageIO_Semantics_Position) continue;
      Arcadia_BooleanValue selected = input->location >= 0
        ? output->location == input->location
        : output->location == -1 && sameName(thread, input->name, output->name);
      if (!selected) continue;
      if (match) fail(thread, self, (Arcadia_Object*)input, "verification error: ambiguous vertex output for fragment input\n");
      match = output;
    }
    if (!match) fail(thread, self, (Arcadia_Object*)input, "verification error: fragment input has no matching vertex output\n");
    if (!sameType(input->type, match->type)) fail(thread, self, (Arcadia_Object*)input, "verification error: linked interface types do not match\n");
    if (input->semantics != Arcadia_VPL_IR_StageIO_Semantics_None && match->semantics != Arcadia_VPL_IR_StageIO_Semantics_None && input->semantics != match->semantics) fail(thread, self, (Arcadia_Object*)input, "verification error: linked interface semantics do not match\n");
  }
}

static void
uniforms
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  )
{
  requireObject(thread, self, (Arcadia_Object*)self->module->uniforms, _Arcadia_List_getType(thread));
  for (Arcadia_SizeValue i = 0, n = size(thread, self->module->uniforms); i < n; ++i) {
    Arcadia_VPL_IR_Uniform* uniform = (Arcadia_VPL_IR_Uniform*)at(thread, self, self->module->uniforms, i, _Arcadia_VPL_IR_Uniform_getType(thread));
    checkName(thread, self, uniform->name);
    for (Arcadia_SizeValue j = 0; j < i; ++j) {
      Arcadia_VPL_IR_Uniform* earlier = (Arcadia_VPL_IR_Uniform*)at(thread, self, self->module->uniforms, j, _Arcadia_VPL_IR_Uniform_getType(thread));
      if (sameName(thread, uniform->name, earlier->name)) fail(thread, self, (Arcadia_Object*)uniform, "verification error: duplicate uniform name\n");
    }
    Arcadia_VPL_IR_Type* type = NULL;
    switch (uniform->kind) {
      case Arcadia_VPL_IR_UniformKind_Scalar:
        requireObject(thread, self, (Arcadia_Object*)uniform, _Arcadia_VPL_IR_ScalarUniform_getType(thread));
        type = ((Arcadia_VPL_IR_ScalarUniform*)uniform)->type;
        break;
      case Arcadia_VPL_IR_UniformKind_Record:
        requireObject(thread, self, (Arcadia_Object*)uniform, _Arcadia_VPL_IR_RecordUniform_getType(thread));
        type = (Arcadia_VPL_IR_Type*)((Arcadia_VPL_IR_RecordUniform*)uniform)->recordType;
        requireObject(thread, self, (Arcadia_Object*)type, _Arcadia_VPL_IR_RecordType_getType(thread));
        break;
      case Arcadia_VPL_IR_UniformKind_Array: {
        requireObject(thread, self, (Arcadia_Object*)uniform, _Arcadia_VPL_IR_ArrayUniform_getType(thread));
        Arcadia_VPL_IR_ArrayUniform* array = (Arcadia_VPL_IR_ArrayUniform*)uniform;
        valueTypeRequired(thread, self, array->elementType);
        if (!array->count) fail(thread, self, (Arcadia_Object*)uniform, "verification error: unsized uniform arrays are unsupported\n");
        type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_ArrayType_create(thread, array->elementType, array->count);
      } break;
      default: fail(thread, self, (Arcadia_Object*)uniform, "verification error: unknown uniform kind\n");
    }
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
    valueTypeRequired(thread, self, type);
    Arcadia_List_insertBackObjectReferenceValue(thread, self->uniformTypes, (Arcadia_Object*)type);
    Arcadia_ValueStack_popValues(thread, 1);
  }
}

void
Arcadia_VPL_IR_Verifier_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Module* module
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)module);
    requireObject(thread, self, (Arcadia_Object*)module, _Arcadia_VPL_IR_Module_getType(thread));
    self->module = module;
    self->types = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    self->activeTypes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    self->activeReferences = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    self->uniformTypes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    self->blocks = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    self->normalReturns = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
    uniforms(thread, self);
    stage(thread, self, module->vertex, Arcadia_VPL_IR_Stage_Vertex);
    stage(thread, self, module->fragment, Arcadia_VPL_IR_Stage_Fragment);
    linkInterfaces(thread, self);
    _Arcadia_VPL_IR_verifyOutputs(thread, self);
    reset(self);
    Arcadia_ValueStack_popValues(thread, 2);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    reset(self);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
  }
}
