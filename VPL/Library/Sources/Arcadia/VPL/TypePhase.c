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
#include "Arcadia/VPL/TypePhase.h"
#include "Arcadia/VPL/Symbols/Include.h"
#include "Arcadia/VPL/Tree/Include.h"
#include "Arcadia/VPL/Diagnostics/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self
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

Arcadia_defineObjectType(u8"Arcadia.VPL.TypePhase", Arcadia_VPL_TypePhase,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_TypePhase);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->diagnostics = (Arcadia_Languages_Diagnostics*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_Diagnostics_getType(thread));
  self->types = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->symbolTypes = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  self->program = NULL;
  Arcadia_LeaveConstructor(Arcadia_VPL_TypePhase);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self
  )
{
  if (self->diagnostics) Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  if (self->types) Arcadia_Object_visit(thread, (Arcadia_Object*)self->types);
  if (self->symbolTypes) Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbolTypes);
  if (self->program) Arcadia_Object_visit(thread, (Arcadia_Object*)self->program);
}

Arcadia_VPL_TypePhase*
Arcadia_VPL_TypePhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_TypePhase);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_TypePhase);
}

Arcadia_Languages_Diagnostics*
Arcadia_VPL_TypePhase_getDiagnostics
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self
  )
{
  return self->diagnostics;
}

static void
typeError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    const char* message
  )
{
  Arcadia_String* text = Arcadia_String_createFromCxxString(thread, message);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)text);
  Arcadia_VPL_Diagnostics_TypeErrorDiagnostic* diagnostic = Arcadia_VPL_Diagnostics_TypeErrorDiagnostic_create(thread, text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostic);
  Arcadia_Languages_Diagnostics_add(thread, self->diagnostics, (Arcadia_Languages_Diagnostic*)diagnostic);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static Arcadia_VPL_IR_Type*
remember
  (
    Arcadia_Thread* thread,
    Arcadia_Map* map,
    Arcadia_Object* key,
    Arcadia_VPL_IR_Type* type
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_Map_set(thread, map, Arcadia_Value_makeObjectReferenceValue(key), Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)type), NULL, NULL);
  Arcadia_ValueStack_popValues(thread, 1);
  return type;
}

Arcadia_VPL_IR_Type*
Arcadia_VPL_TypePhase_getType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Tree_Node* node
  )
{
  Arcadia_Value value = Arcadia_Map_get(thread, self->types, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)node));
  return Arcadia_Value_isObjectReferenceValue(&value) ? (Arcadia_VPL_IR_Type*)Arcadia_Value_getObjectReferenceValue(&value) : NULL;
}

static Arcadia_BooleanValue
isScalar
  (
    Arcadia_VPL_IR_Type* type,
    Arcadia_Natural8Value elementKind
  )
{
  return type->kind == Arcadia_VPL_IR_TypeKind_Scalar && ((Arcadia_VPL_IR_ScalarType*)type)->elementKind == elementKind;
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
    case Arcadia_VPL_IR_TypeKind_Scalar:
      return ((Arcadia_VPL_IR_ScalarType*)a)->elementKind == ((Arcadia_VPL_IR_ScalarType*)b)->elementKind;
    case Arcadia_VPL_IR_TypeKind_Vector:
      return ((Arcadia_VPL_IR_VectorType*)a)->elementKind == ((Arcadia_VPL_IR_VectorType*)b)->elementKind && ((Arcadia_VPL_IR_VectorType*)a)->size == ((Arcadia_VPL_IR_VectorType*)b)->size;
    case Arcadia_VPL_IR_TypeKind_Matrix:
      return ((Arcadia_VPL_IR_MatrixType*)a)->elementKind == ((Arcadia_VPL_IR_MatrixType*)b)->elementKind && ((Arcadia_VPL_IR_MatrixType*)a)->rows == ((Arcadia_VPL_IR_MatrixType*)b)->rows && ((Arcadia_VPL_IR_MatrixType*)a)->columns == ((Arcadia_VPL_IR_MatrixType*)b)->columns;
    case Arcadia_VPL_IR_TypeKind_Sampler:
      return ((Arcadia_VPL_IR_SamplerType*)a)->samplerKind == ((Arcadia_VPL_IR_SamplerType*)b)->samplerKind;
    case Arcadia_VPL_IR_TypeKind_Array:
      return ((Arcadia_VPL_IR_ArrayType*)a)->count == ((Arcadia_VPL_IR_ArrayType*)b)->count && sameType(((Arcadia_VPL_IR_ArrayType*)a)->element, ((Arcadia_VPL_IR_ArrayType*)b)->element);
    default: return Arcadia_BooleanValue_False; // Records are nominal.
  }
}

static Arcadia_BooleanValue
compatible
  (
    Arcadia_VPL_IR_Type* target,
    Arcadia_VPL_IR_Type* source
  )
{
  return sameType(target, source) || (isScalar(target, Arcadia_VPL_IR_ScalarElementKind_Floating) && isScalar(source, Arcadia_VPL_IR_ScalarElementKind_Integer));
}

static void
validateValueType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_IR_Type* type,
    Arcadia_SizeValue depth
  )
{
  if (!type || depth > 64) typeError(thread, self, "type error: missing or recursive value type\n");
  switch (type->kind) {
    case Arcadia_VPL_IR_TypeKind_Scalar:
      if (((Arcadia_VPL_IR_ScalarType*)type)->elementKind > Arcadia_VPL_IR_ScalarElementKind_Floating) typeError(thread, self, "type error: invalid scalar kind\n");
      break;
    case Arcadia_VPL_IR_TypeKind_Vector: {
      Arcadia_VPL_IR_VectorType* vector = (Arcadia_VPL_IR_VectorType*)type;
      if (vector->size < 2 || vector->size > 4 || vector->elementKind > Arcadia_VPL_IR_ScalarElementKind_Floating) typeError(thread, self, "type error: invalid vector type\n");
    } break;
    case Arcadia_VPL_IR_TypeKind_Matrix: {
      Arcadia_VPL_IR_MatrixType* matrix = (Arcadia_VPL_IR_MatrixType*)type;
      if (matrix->rows < 2 || matrix->rows > 4 || matrix->columns < 2 || matrix->columns > 4 || matrix->elementKind != Arcadia_VPL_IR_ScalarElementKind_Floating) typeError(thread, self, "type error: invalid matrix type\n");
    } break;
    case Arcadia_VPL_IR_TypeKind_Array: {
      Arcadia_VPL_IR_ArrayType* array = (Arcadia_VPL_IR_ArrayType*)type;
      if (!array->count) typeError(thread, self, "type error: array size must be positive\n");
      validateValueType(thread, self, array->element, depth + 1);
    } break;
    case Arcadia_VPL_IR_TypeKind_Record: {
      Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)type)->fields;
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); i < n; ++i) {
        Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, fields, i, _Arcadia_VPL_IR_Field_getType(thread));
        validateValueType(thread, self, field->type, depth + 1);
      }
    } break;
    case Arcadia_VPL_IR_TypeKind_Sampler:
      if (((Arcadia_VPL_IR_SamplerType*)type)->samplerKind != Arcadia_VPL_IR_SamplerKind_Sampler2D) typeError(thread, self, "type error: unsupported sampler type\n");
      break;
    default: typeError(thread, self, "type error: void is not a value type\n");
  }
}

static Arcadia_VPL_IR_Type*
symbolType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Symbol* symbol
  )
{
  if (!symbol) typeError(thread, self, "type error: unresolved symbol\n");
  Arcadia_Value cached = Arcadia_Map_get(thread, self->symbolTypes, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)symbol));
  if (Arcadia_Value_isObjectReferenceValue(&cached)) return (Arcadia_VPL_IR_Type*)Arcadia_Value_getObjectReferenceValue(&cached);
  Arcadia_VPL_IR_Type* type = NULL;
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
    type = ((Arcadia_VPL_Symbols_BuiltinType*)symbol)->valueType;
    if (!type) typeError(thread, self, "type error: named type has no value type\n");
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Variable_getType(thread))) {
    type = symbolType(thread, self, ((Arcadia_VPL_Symbols_Variable*)symbol)->type);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_VariableScalar_getType(thread))) {
    type = symbolType(thread, self, ((Arcadia_VPL_Symbols_VariableScalar*)symbol)->type);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_ConstantScalar_getType(thread))) {
    type = symbolType(thread, self, ((Arcadia_VPL_Symbols_ConstantScalar*)symbol)->type);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_ConstantRecord_getType(thread))) {
    Arcadia_VPL_Symbols_ConstantRecord* record = (Arcadia_VPL_Symbols_ConstantRecord*)symbol;
    Arcadia_List* fields = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fields);
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_RecordType_create(thread, record->name, fields);
    remember(thread, self->symbolTypes, (Arcadia_Object*)symbol, type);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)record->fields); i < n; ++i) {
      Arcadia_VPL_Symbols_Variable* field = (Arcadia_VPL_Symbols_Variable*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, record->fields, i, _Arcadia_VPL_Symbols_Variable_getType(thread));
      Arcadia_VPL_IR_Field* irField = Arcadia_VPL_IR_Field_create(thread, field->name, symbolType(thread, self, field->type));
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)irField);
      Arcadia_List_insertBackObjectReferenceValue(thread, fields, (Arcadia_Object*)irField);
      Arcadia_ValueStack_popValues(thread, 1);
    }
    Arcadia_ValueStack_popValues(thread, 1);
  } else {
    typeError(thread, self, "type error: symbol is not a value\n");
  }
  return remember(thread, self->symbolTypes, (Arcadia_Object*)symbol, type);
}

static Arcadia_VPL_IR_Type*
numberType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_String* literal
  )
{
  const char* bytes = (const char*)Arcadia_String_getBytes(thread, literal);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, literal), i = 0, digits = 0;
  Arcadia_BooleanValue real = Arcadia_BooleanValue_False;
  if (i < n && (bytes[i] == '+' || bytes[i] == '-')) ++i;
  while (i < n && bytes[i] >= '0' && bytes[i] <= '9') { ++i; ++digits; }
  if (i < n && bytes[i] == '.') {
    real = Arcadia_BooleanValue_True;
    ++i;
    while (i < n && bytes[i] >= '0' && bytes[i] <= '9') { ++i; ++digits; }
  }
  if (!digits) typeError(thread, self, "type error: invalid numeric literal\n");
  if (i < n && (bytes[i] == 'e' || bytes[i] == 'E')) {
    real = Arcadia_BooleanValue_True;
    ++i;
    if (i < n && (bytes[i] == '+' || bytes[i] == '-')) ++i;
    Arcadia_SizeValue start = i;
    while (i < n && bytes[i] >= '0' && bytes[i] <= '9') ++i;
    if (i == start) typeError(thread, self, "type error: invalid numeric literal\n");
  }
  if (i != n) typeError(thread, self, "type error: invalid numeric literal\n");
  return real ? self->program->FLOAT->valueType : self->program->INT->valueType;
}

static Arcadia_VPL_IR_Type*
onExpression
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_VPL_Tree_Node* node
  );

static Arcadia_VPL_IR_Type*
onMember
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_IR_Type* target,
    Arcadia_VPL_Tree_Node* node
  )
{
  if (node->flags == Arcadia_VPL_Tree_NodeFlags_BinaryExpr && ((Arcadia_VPL_Tree_BinaryExprNode*)node)->kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Access) {
    Arcadia_VPL_Tree_BinaryExprNode* access = (Arcadia_VPL_Tree_BinaryExprNode*)node;
    Arcadia_VPL_IR_Type* lhs = onMember(thread, self, target, access->lhs);
    return remember(thread, self->types, (Arcadia_Object*)node, onMember(thread, self, lhs, access->rhs));
  }
  if (node->flags != Arcadia_VPL_Tree_NodeFlags_NameExpr) typeError(thread, self, "type error: member must be a name\n");
  Arcadia_String* name = ((Arcadia_VPL_Tree_NameExprNode*)node)->name;
  Arcadia_VPL_IR_Type* result = NULL;
  if (target->kind == Arcadia_VPL_IR_TypeKind_Record) {
    Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)target)->fields;
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); i < n; ++i) {
      Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, fields, i, _Arcadia_VPL_IR_Field_getType(thread));
      if (Arcadia_String_isEqualTo_pn(thread, field->name, Arcadia_String_getBytes(thread, name), Arcadia_String_getNumberOfBytes(thread, name))) {
        result = field->type;
        break;
      }
    }
  } else if (target->kind == Arcadia_VPL_IR_TypeKind_Vector) {
    // Preserve the currently declared xy/xyz access surface; broader swizzles are a later step.
    Arcadia_VPL_IR_VectorType* vector = (Arcadia_VPL_IR_VectorType*)target;
    if (vector->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating && Arcadia_String_isEqualTo_pn(thread, name, "xy", 2) && vector->size >= 2) result = self->program->VEC2->valueType;
    if (vector->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating && Arcadia_String_isEqualTo_pn(thread, name, "xyz", 3) && vector->size >= 3) result = self->program->VEC3->valueType;
  }
  if (!result) typeError(thread, self, "type error: unknown member\n");
  return remember(thread, self->types, (Arcadia_Object*)node, result);
}

static Arcadia_BooleanValue
writable
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_VPL_Tree_Node* node
  )
{
  if (node->flags == Arcadia_VPL_Tree_NodeFlags_IndexExpr) return writable(thread, procedure, ((Arcadia_VPL_Tree_IndexExprNode*)node)->target);
  if (node->flags == Arcadia_VPL_Tree_NodeFlags_BinaryExpr && ((Arcadia_VPL_Tree_BinaryExprNode*)node)->kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Access) return writable(thread, procedure, ((Arcadia_VPL_Tree_BinaryExprNode*)node)->lhs);
  if (node->flags != Arcadia_VPL_Tree_NodeFlags_NameExpr) return Arcadia_BooleanValue_False;
  Arcadia_VPL_Symbols_Symbol* symbol = ((Arcadia_VPL_Tree_NameExprNode*)node)->symbol;
  if (!symbol) return Arcadia_BooleanValue_False;
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Variable_getType(thread))) {
    Arcadia_VPL_Symbols_Variable* variable = (Arcadia_VPL_Symbols_Variable*)symbol;
    if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)variable->enclosing, _Arcadia_VPL_Symbols_Procedure_getType(thread))) return Arcadia_BooleanValue_False;
    return !Arcadia_VPL_Tree_VariableDefnNode_isConstant(thread, (Arcadia_VPL_Tree_VariableDefnNode*)variable->node);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_VariableScalar_getType(thread))) {
    Arcadia_Natural8Value flags = ((Arcadia_VPL_Symbols_VariableScalar*)symbol)->flags;
    if (procedure->flags & Arcadia_VPL_Symbols_ProcedureFlags_VertexStage) return (flags & Arcadia_VPL_Symbols_VariableScalarFlags_FragmentProcessing) != 0;
    if (procedure->flags & Arcadia_VPL_Symbols_ProcedureFlags_FragmentStage) return (flags & Arcadia_VPL_Symbols_VariableScalarFlags_PixelProcessing) != 0;
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
numeric
  (
    Arcadia_VPL_IR_Type* type
  )
{
  if (type->kind == Arcadia_VPL_IR_TypeKind_Scalar) return ((Arcadia_VPL_IR_ScalarType*)type)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Boolean;
  if (type->kind == Arcadia_VPL_IR_TypeKind_Vector) return ((Arcadia_VPL_IR_VectorType*)type)->elementKind != Arcadia_VPL_IR_ScalarElementKind_Boolean;
  return type->kind == Arcadia_VPL_IR_TypeKind_Matrix;
}

static Arcadia_VPL_IR_Type*
arithmeticType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_Integer32Value kind,
    Arcadia_VPL_IR_Type* lhs,
    Arcadia_VPL_IR_Type* rhs
  )
{
  if (!numeric(lhs) || !numeric(rhs)) typeError(thread, self, "type error: arithmetic requires numeric operands\n");
  if (kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Multiply && lhs->kind == Arcadia_VPL_IR_TypeKind_Matrix) {
    Arcadia_VPL_IR_MatrixType* matrix = (Arcadia_VPL_IR_MatrixType*)lhs;
    if (rhs->kind == Arcadia_VPL_IR_TypeKind_Vector && matrix->columns == ((Arcadia_VPL_IR_VectorType*)rhs)->size && ((Arcadia_VPL_IR_VectorType*)rhs)->elementKind == matrix->elementKind) {
      return (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_VectorType_create(thread, matrix->elementKind, matrix->rows);
    }
    if (rhs->kind == Arcadia_VPL_IR_TypeKind_Matrix && matrix->columns == ((Arcadia_VPL_IR_MatrixType*)rhs)->rows) {
      return (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_MatrixType_create(thread, matrix->elementKind, matrix->rows, ((Arcadia_VPL_IR_MatrixType*)rhs)->columns);
    }
    if (rhs->kind != Arcadia_VPL_IR_TypeKind_Scalar) typeError(thread, self, "type error: incompatible matrix multiplication\n");
  }
  if (sameType(lhs, rhs)) return lhs;
  if (lhs->kind == Arcadia_VPL_IR_TypeKind_Scalar && rhs->kind == Arcadia_VPL_IR_TypeKind_Scalar) return self->program->FLOAT->valueType;
  if (kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Multiply || kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Divide) {
    if (isScalar(rhs, Arcadia_VPL_IR_ScalarElementKind_Floating) || isScalar(rhs, Arcadia_VPL_IR_ScalarElementKind_Integer)) {
      if (lhs->kind == Arcadia_VPL_IR_TypeKind_Matrix || (lhs->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)lhs)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating)) return lhs;
    }
    if (kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Multiply && (isScalar(lhs, Arcadia_VPL_IR_ScalarElementKind_Floating) || isScalar(lhs, Arcadia_VPL_IR_ScalarElementKind_Integer))) {
      if (rhs->kind == Arcadia_VPL_IR_TypeKind_Matrix || (rhs->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)rhs)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating)) return rhs;
    }
  }
  typeError(thread, self, "type error: incompatible arithmetic operands\n");
  return NULL;
}

static void
checkConstantIndex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Tree_Node* node,
    Arcadia_SizeValue count
  )
{
  if (node->flags != Arcadia_VPL_Tree_NodeFlags_NumberExpr) return;
  Arcadia_String* literal = ((Arcadia_VPL_Tree_NumberExprNode*)node)->literal;
  const char* bytes = (const char*)Arcadia_String_getBytes(thread, literal);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, literal), i = 0, value = 0;
  Arcadia_BooleanValue negative = bytes[0] == '-';
  if (bytes[0] == '-' || bytes[0] == '+') ++i;
  for (; i < n; ++i) {
    Arcadia_SizeValue digit = bytes[i] - '0';
    if (value > (count - 1) / 10 || (value == (count - 1) / 10 && digit > (count - 1) % 10)) typeError(thread, self, "type error: constant array index is out of bounds\n");
    value = value * 10 + digit;
  }
  if (negative && value) typeError(thread, self, "type error: constant array index is out of bounds\n");
}

static Arcadia_VPL_IR_Type*
onExpression
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_VPL_Tree_Node* node
  )
{
  Arcadia_VPL_IR_Type* result = NULL;
  switch (node->flags) {
    case Arcadia_VPL_Tree_NodeFlags_NameExpr:
      result = symbolType(thread, self, ((Arcadia_VPL_Tree_NameExprNode*)node)->symbol);
      break;
    case Arcadia_VPL_Tree_NodeFlags_NumberExpr:
      result = numberType(thread, self, ((Arcadia_VPL_Tree_NumberExprNode*)node)->literal);
      break;
    case Arcadia_VPL_Tree_NodeFlags_IndexExpr: {
      Arcadia_VPL_Tree_IndexExprNode* index = (Arcadia_VPL_Tree_IndexExprNode*)node;
      Arcadia_VPL_IR_Type* target = onExpression(thread, self, procedure, index->target);
      Arcadia_VPL_IR_Type* subscript = onExpression(thread, self, procedure, index->index);
      if (target->kind != Arcadia_VPL_IR_TypeKind_Array || !isScalar(subscript, Arcadia_VPL_IR_ScalarElementKind_Integer)) typeError(thread, self, "type error: indexing requires an array and an integer index\n");
      validateValueType(thread, self, target, 0);
      checkConstantIndex(thread, self, index->index, ((Arcadia_VPL_IR_ArrayType*)target)->count);
      result = ((Arcadia_VPL_IR_ArrayType*)target)->element;
    } break;
    case Arcadia_VPL_Tree_NodeFlags_BinaryExpr: {
      Arcadia_VPL_Tree_BinaryExprNode* binary = (Arcadia_VPL_Tree_BinaryExprNode*)node;
      Arcadia_VPL_IR_Type* lhs = onExpression(thread, self, procedure, binary->lhs);
      if (binary->kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Access) {
        result = onMember(thread, self, lhs, binary->rhs);
      } else {
        Arcadia_VPL_IR_Type* rhs = onExpression(thread, self, procedure, binary->rhs);
        if (binary->kind == Arcadia_VPL_Tree_BinaryExprNodeFlags_Assignment) {
          if (!writable(thread, procedure, binary->lhs)) typeError(thread, self, "type error: assignment target is read-only or not storage\n");
          if (!compatible(lhs, rhs)) typeError(thread, self, "type error: incompatible assignment\n");
          result = lhs;
        } else if (binary->kind >= Arcadia_VPL_Tree_BinaryExprNodeFlags_Add && binary->kind <= Arcadia_VPL_Tree_BinaryExprNodeFlags_Divide) {
          result = arithmeticType(thread, self, binary->kind, lhs, rhs);
        } else {
          typeError(thread, self, "type error: unknown binary operation\n");
        }
      }
    } break;
    case Arcadia_VPL_Tree_NodeFlags_CallExpr: {
      Arcadia_VPL_Tree_CallExprNode* call = (Arcadia_VPL_Tree_CallExprNode*)node;
      if (call->target->flags != Arcadia_VPL_Tree_NodeFlags_NameExpr) typeError(thread, self, "type error: call target must be a resolved name\n");
      Arcadia_VPL_Symbols_Symbol* symbol = ((Arcadia_VPL_Tree_NameExprNode*)call->target)->symbol;
      if (!symbol) typeError(thread, self, "type error: unresolved call target\n");
      Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)call->arguments);
      if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
        Arcadia_VPL_Symbols_Procedure* callee = (Arcadia_VPL_Symbols_Procedure*)symbol;
        if (n != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)callee->parameters)) typeError(thread, self, "type error: incorrect call argument count\n");
        for (Arcadia_SizeValue i = 0; i < n; ++i) {
          Arcadia_VPL_Tree_Node* argument = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, call->arguments, i, _Arcadia_VPL_Tree_Node_getType(thread));
          Arcadia_VPL_Symbols_Variable* parameter = (Arcadia_VPL_Symbols_Variable*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, callee->parameters, i, _Arcadia_VPL_Symbols_Variable_getType(thread));
          if (!compatible(symbolType(thread, self, parameter->type), onExpression(thread, self, procedure, argument))) typeError(thread, self, "type error: incompatible call argument\n");
        }
        result = symbolType(thread, self, (Arcadia_VPL_Symbols_Symbol*)callee->returnValueType);
      } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)symbol, _Arcadia_VPL_Symbols_BuiltinType_getType(thread))) {
        result = symbolType(thread, self, symbol);
        if (result->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)result)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating) {
          Arcadia_SizeValue components = 0;
          for (Arcadia_SizeValue i = 0; i < n; ++i) {
            Arcadia_VPL_Tree_Node* argument = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, call->arguments, i, _Arcadia_VPL_Tree_Node_getType(thread));
            Arcadia_VPL_IR_Type* type = onExpression(thread, self, procedure, argument);
            if (isScalar(type, Arcadia_VPL_IR_ScalarElementKind_Floating) || isScalar(type, Arcadia_VPL_IR_ScalarElementKind_Integer)) ++components;
            else if (type->kind == Arcadia_VPL_IR_TypeKind_Vector && ((Arcadia_VPL_IR_VectorType*)type)->elementKind == Arcadia_VPL_IR_ScalarElementKind_Floating) components += ((Arcadia_VPL_IR_VectorType*)type)->size;
            else typeError(thread, self, "type error: invalid vector constructor argument\n");
          }
          if (!n || (components != ((Arcadia_VPL_IR_VectorType*)result)->size && !(n == 1 && components == 1))) typeError(thread, self, "type error: incorrect vector constructor component count\n");
        } else if (result->kind == Arcadia_VPL_IR_TypeKind_Scalar) {
          if (n != 1) typeError(thread, self, "type error: scalar constructor requires one argument\n");
          Arcadia_VPL_Tree_Node* argument = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, call->arguments, 0, _Arcadia_VPL_Tree_Node_getType(thread));
          if (!compatible(result, onExpression(thread, self, procedure, argument))) typeError(thread, self, "type error: incompatible scalar constructor argument\n");
        } else {
          typeError(thread, self, "type error: unsupported type constructor\n");
        }
      } else {
        typeError(thread, self, "type error: symbol is not callable\n");
      }
      remember(thread, self->types, (Arcadia_Object*)call->target, result);
    } break;
    default: typeError(thread, self, "type error: expected an expression\n");
  }
  return remember(thread, self->types, (Arcadia_Object*)node, result);
}

static void
onBody
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_List* body
  );

static void
onDeclaration
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Tree_VariableDefnNode* node
  )
{
  Arcadia_VPL_IR_Type* type = symbolType(thread, self, node->typeSymbol);
  validateValueType(thread, self, type, 0);
  remember(thread, self->types, (Arcadia_Object*)node, type);
}

static void
condition
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_VPL_Tree_Node* node
  )
{
  if (!isScalar(onExpression(thread, self, procedure, node), Arcadia_VPL_IR_ScalarElementKind_Boolean)) typeError(thread, self, "type error: condition must have type bool\n");
}

static void
onStatement
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_VPL_Tree_Node* node
  )
{
  switch (node->flags) {
    case Arcadia_VPL_Tree_NodeFlags_VariableDefinition:
      onDeclaration(thread, self, (Arcadia_VPL_Tree_VariableDefnNode*)node);
      return;
    case Arcadia_VPL_Tree_NodeFlags_IfStmt: {
      Arcadia_VPL_Tree_IfStmtNode* branch = (Arcadia_VPL_Tree_IfStmtNode*)node;
      condition(thread, self, procedure, branch->condition);
      onBody(thread, self, procedure, branch->thenBody);
      if (branch->elseBody) onBody(thread, self, procedure, branch->elseBody);
    } break;
    case Arcadia_VPL_Tree_NodeFlags_WhileStmt: {
      Arcadia_VPL_Tree_WhileStmtNode* loop = (Arcadia_VPL_Tree_WhileStmtNode*)node;
      condition(thread, self, procedure, loop->condition);
      onBody(thread, self, procedure, loop->body);
    } break;
    case Arcadia_VPL_Tree_NodeFlags_ForStmt: {
      Arcadia_VPL_Tree_ForStmtNode* loop = (Arcadia_VPL_Tree_ForStmtNode*)node;
      if (loop->initializer) {
        if (loop->initializer->flags == Arcadia_VPL_Tree_NodeFlags_VariableDefinition) onDeclaration(thread, self, (Arcadia_VPL_Tree_VariableDefnNode*)loop->initializer);
        else onExpression(thread, self, procedure, loop->initializer);
      }
      if (loop->condition) condition(thread, self, procedure, loop->condition);
      if (loop->update) onExpression(thread, self, procedure, loop->update);
      onBody(thread, self, procedure, loop->body);
    } break;
    case Arcadia_VPL_Tree_NodeFlags_ReturnStmt: {
      Arcadia_VPL_Tree_Node* value = ((Arcadia_VPL_Tree_ReturnStmtNode*)node)->value;
      Arcadia_VPL_IR_Type* target = symbolType(thread, self, (Arcadia_VPL_Symbols_Symbol*)procedure->returnValueType);
      if (!value) {
        if (target->kind != Arcadia_VPL_IR_TypeKind_Void) typeError(thread, self, "type error: non-void return requires a value\n");
      } else {
        Arcadia_VPL_IR_Type* source = onExpression(thread, self, procedure, value);
        if (target->kind == Arcadia_VPL_IR_TypeKind_Void || !compatible(target, source)) typeError(thread, self, "type error: incompatible return value\n");
      }
    } break;
    case Arcadia_VPL_Tree_NodeFlags_DiscardStmt:
      if (!(procedure->flags & Arcadia_VPL_Symbols_ProcedureFlags_FragmentStage)) typeError(thread, self, "type error: discard is restricted to fragment procedures\n");
      break;
    default:
      onExpression(thread, self, procedure, node);
      return;
  }
  remember(thread, self->types, (Arcadia_Object*)node, self->program->VOID->valueType);
}

static void
onBody
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Procedure* procedure,
    Arcadia_List* body
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)body); i < n; ++i) {
    Arcadia_VPL_Tree_Node* node = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, body, i, _Arcadia_VPL_Tree_Node_getType(thread));
    onStatement(thread, self, procedure, node);
  }
}

static void
onScope
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_Languages_Scope* scope
  )
{
  Arcadia_List* entries = Arcadia_Map_getValues(thread, scope->entries);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)entries); i < n; ++i) {
    Arcadia_Object* entry = Arcadia_List_getObjectReferenceValueAt(thread, entries, i);
    if (Arcadia_Object_isInstanceOf(thread, entry, _Arcadia_VPL_Symbols_Stage_getType(thread))) {
      onScope(thread, self, ((Arcadia_VPL_Symbols_Stage*)entry)->scope);
    } else if (Arcadia_Object_isInstanceOf(thread, entry, _Arcadia_VPL_Symbols_Procedure_getType(thread))) {
      Arcadia_VPL_Symbols_Procedure* procedure = (Arcadia_VPL_Symbols_Procedure*)entry;
      Arcadia_VPL_IR_Type* returnType = symbolType(thread, self, (Arcadia_VPL_Symbols_Symbol*)procedure->returnValueType);
      if (returnType->kind != Arcadia_VPL_IR_TypeKind_Void) validateValueType(thread, self, returnType, 0);
      Arcadia_VPL_Tree_ProcedureDefnNode* node = (Arcadia_VPL_Tree_ProcedureDefnNode*)procedure->node;
      remember(thread, self->types, (Arcadia_Object*)node->returnValueType, returnType);
      remember(thread, self->types, (Arcadia_Object*)node, returnType);
      for (Arcadia_SizeValue j = 0, m = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->parameters); j < m; ++j) {
        onDeclaration(thread, self, (Arcadia_VPL_Tree_VariableDefnNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, node->parameters, j, _Arcadia_VPL_Tree_VariableDefnNode_getType(thread)));
      }
      if (!(procedure->flags & Arcadia_VPL_Symbols_ProcedureFlags_Builtin)) onBody(thread, self, procedure, node->body);
    }
  }
  Arcadia_ValueStack_popValues(thread, 1);
}

void
Arcadia_VPL_TypePhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_TypePhase* self,
    Arcadia_VPL_Symbols_Program* program
  )
{
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)program);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->types);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->symbolTypes);
    self->program = program;
    if (!program->scope) typeError(thread, self, "type error: program has not been entered and resolved\n");
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)program->programDefnNode->elements); i < n; ++i) {
      Arcadia_VPL_Tree_Node* node = (Arcadia_VPL_Tree_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, program->programDefnNode->elements, i, _Arcadia_VPL_Tree_Node_getType(thread));
      if (node->flags == Arcadia_VPL_Tree_NodeFlags_VariableDefinition) {
        Arcadia_VPL_Tree_VariableDefnNode* declaration = (Arcadia_VPL_Tree_VariableDefnNode*)node;
        onDeclaration(thread, self, declaration);
      } else if (node->flags == Arcadia_VPL_Tree_NodeFlags_ConstantRecordDefn) {
        Arcadia_VPL_Symbols_Symbol* symbol = (Arcadia_VPL_Symbols_Symbol*)Arcadia_Languages_Scope_lookup(thread, program->scope, ((Arcadia_VPL_Tree_ConstantRecordDefnNode*)node)->name, Arcadia_BooleanValue_False);
        Arcadia_VPL_IR_Type* type = symbolType(thread, self, symbol);
        validateValueType(thread, self, type, 0);
        remember(thread, self->types, (Arcadia_Object*)node, type);
        Arcadia_List* fields = ((Arcadia_VPL_Tree_ConstantRecordDefnNode*)node)->fields;
        for (Arcadia_SizeValue j = 0, m = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); j < m; ++j) {
          onDeclaration(thread, self, (Arcadia_VPL_Tree_VariableDefnNode*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, fields, j, _Arcadia_VPL_Tree_VariableDefnNode_getType(thread)));
        }
      }
    }
    onScope(thread, self, program->scope);
    Arcadia_ValueStack_popValues(thread, 2);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->types);
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->symbolTypes);
    self->program = NULL;
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
  }
}
