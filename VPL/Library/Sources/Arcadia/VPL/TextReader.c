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
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <fenv.h>

static Arcadia_SizeValue
count
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Node* node
  )
{
  return node->items ? Arcadia_Collection_getSize(thread, (Arcadia_Collection*)node->items) : 0;
}

static void
error
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node,
    const char* message
  )
{
  Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SyntacticalError, message);
}

static Arcadia_VPL_Text_Node*
item
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node,
    Arcadia_SizeValue i
  )
{
  if (i >= count(thread, node)) error(thread, c, node, "syntax error: missing form item\n");
  return (Arcadia_VPL_Text_Node*)Arcadia_List_getObjectReferenceValueAt(thread, node->items, i);
}

static Arcadia_BooleanValue
word
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Node* node,
    const char* text
  )
{
  return node->kind == VPL_TEXT_NAME && Arcadia_String_isEqualTo_pn(thread, node->text, text, strlen(text));
}

static void
shape
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node,
    const char* head,
    Arcadia_SizeValue minimum,
    Arcadia_SizeValue maximum
  )
{
  Arcadia_SizeValue n = count(thread, node);
  if (node->kind != VPL_TEXT_FORM || n < minimum || n > maximum || !word(thread, item(thread, c, node, 0), head)) error(thread, c, node, "syntax error: unexpected form or item count\n");
}

static Arcadia_String*
identifier
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  if (node->kind != VPL_TEXT_NAME || word(thread, node, "true") || word(thread, node, "false") || word(thread, node, "void")) error(thread, c, node, "syntax error: expected an identifier\n");
  return node->text;
}

static Arcadia_String*
string
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  if (node->kind != VPL_TEXT_STRING) error(thread, c, node, "syntax error: expected a quoted string\n");
  return node->text;
}

static Arcadia_Map*
map
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c
  )
{
  return (Arcadia_Map*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)));
}

static Arcadia_List*
list
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c
  )
{
  return (Arcadia_List*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_ArrayList_create(thread));
}

static Arcadia_Object*
located
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_Object* object,
    Arcadia_VPL_Text_Node* node
  )
{
  Arcadia_VPL_Text_own(thread, c, object);
  Arcadia_Map_set(thread, c->locations, Arcadia_Value_makeObjectReferenceValue(object), Arcadia_Value_makeSizeValue(node->offset), NULL, NULL);
  return object;
}

static void
define
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_Map* bindings,
    Arcadia_VPL_Text_Node* name,
    Arcadia_VPL_Text_Node* declaration
  )
{
  Arcadia_String* text = identifier(thread, c, name);
  Arcadia_Value key = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)text);
  Arcadia_Value old = Arcadia_Map_get(thread, bindings, key);
  if (!Arcadia_Value_isVoidValue(&old)) Arcadia_VPL_Text_fail(thread, c, name->offset, Arcadia_Status_SemanticalError, "binding error: duplicate identifier\n");
  Arcadia_Map_set(thread, bindings, key, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)declaration), NULL, NULL);
}

static Arcadia_VPL_Text_Node*
lookup
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_Map* bindings,
    Arcadia_VPL_Text_Node* name
  )
{
  Arcadia_String* text = identifier(thread, c, name);
  Arcadia_Value value = Arcadia_Map_get(thread, bindings, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)text));
  if (!Arcadia_Value_isObjectReferenceValue(&value)) Arcadia_VPL_Text_fail(thread, c, name->offset, Arcadia_Status_SemanticalError, "binding error: undefined identifier\n");
  return (Arcadia_VPL_Text_Node*)Arcadia_Value_getObjectReferenceValue(&value);
}

static Arcadia_SizeValue
unsignedNumber
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node,
    Arcadia_SizeValue start,
    Arcadia_SizeValue limit
  )
{
  if (!node->text) error(thread, c, node, "syntax error: expected decimal integer\n");
  const unsigned char* bytes = Arcadia_String_getBytes(thread, node->text);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, node->text), value = 0;
  if (start >= n) error(thread, c, node, "syntax error: empty decimal integer\n");
  for (Arcadia_SizeValue i = start; i < n; ++i) {
    if (bytes[i] < '0' || bytes[i] > '9') error(thread, c, node, "syntax error: expected decimal digits\n");
    Arcadia_SizeValue digit = bytes[i] - '0';
    if (value > limit / 10 || (value == limit / 10 && digit > limit % 10)) Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SemanticalError, "binding error: integer is out of range\n");
    value = value * 10 + digit;
  }
  return value;
}

static Arcadia_SizeValue
sizeNumber
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  if (node->kind != VPL_TEXT_NUMBER) error(thread, c, node, "syntax error: expected a size\n");
  return unsignedNumber(thread, c, node, 0, (Arcadia_SizeValue)-1);
}

static Arcadia_Integer32Value
integerNumber
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  if (node->kind != VPL_TEXT_NUMBER) error(thread, c, node, "syntax error: expected a signed integer\n");
  const unsigned char* bytes = Arcadia_String_getBytes(thread, node->text);
  Arcadia_BooleanValue negative = bytes[0] == '-';
  Arcadia_SizeValue start = negative || bytes[0] == '+' ? 1 : 0;
  Arcadia_SizeValue value = unsignedNumber(thread, c, node, start, negative ? (Arcadia_SizeValue)2147483648ULL : (Arcadia_SizeValue)2147483647);
  if (negative && value == (Arcadia_SizeValue)2147483648ULL) return (-2147483647 - 1);
  return negative ? -(Arcadia_Integer32Value)value : (Arcadia_Integer32Value)value;
}

static Arcadia_SizeValue
registerNumber
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  identifier(thread, c, node);
  const unsigned char* bytes = Arcadia_String_getBytes(thread, node->text);
  if (bytes[0] != 'r') error(thread, c, node, "syntax error: register identifiers are r followed by decimal digits\n");
  return unsignedNumber(thread, c, node, 1, (Arcadia_SizeValue)-1);
}

static Arcadia_Natural8Value
elementKind
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  if (word(thread, node, "bool")) return Arcadia_VPL_IR_ScalarElementKind_Boolean;
  if (word(thread, node, "int")) return Arcadia_VPL_IR_ScalarElementKind_Integer;
  if (word(thread, node, "float")) return Arcadia_VPL_IR_ScalarElementKind_Floating;
  error(thread, c, node, "syntax error: expected scalar element kind\n");
  return 0;
}

static Arcadia_VPL_IR_Type*
resolveType
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* name,
    Arcadia_SizeValue depth
  )
{
  Arcadia_VPL_Text_Node* declaration = lookup(thread, c, c->types, name);
  if (declaration->state == 2) return (Arcadia_VPL_IR_Type*)declaration->binding;
  if (declaration->state == 1 || depth > 128) Arcadia_VPL_Text_fail(thread, c, name->offset, Arcadia_Status_SemanticalError, "binding error: cyclic or excessively nested type\n");
  declaration->state = 1;
  Arcadia_VPL_Text_Node* kind = item(thread, c, declaration, 2);
  Arcadia_VPL_IR_Type* type = NULL;
  if (word(thread, kind, "void")) {
    shape(thread, c, declaration, "type", 3, 3);
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_VoidType_create(thread);
  } else if (word(thread, kind, "bool") || word(thread, kind, "int") || word(thread, kind, "float")) {
    shape(thread, c, declaration, "type", 3, 3);
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_ScalarType_create(thread, elementKind(thread, c, kind));
  } else if (word(thread, kind, "sampler2D")) {
    shape(thread, c, declaration, "type", 3, 3);
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_SamplerType_create(thread, Arcadia_VPL_IR_SamplerKind_Sampler2D);
  } else if (word(thread, kind, "vector")) {
    shape(thread, c, declaration, "type", 5, 5);
    Arcadia_SizeValue n = sizeNumber(thread, c, item(thread, c, declaration, 4));
    if (n < 2 || n > 4) error(thread, c, declaration, "syntax error: vector size must be 2 through 4\n");
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_VectorType_create(thread, elementKind(thread, c, item(thread, c, declaration, 3)), (Arcadia_Natural8Value)n);
  } else if (word(thread, kind, "matrix")) {
    shape(thread, c, declaration, "type", 5, 5);
    Arcadia_SizeValue rows = sizeNumber(thread, c, item(thread, c, declaration, 3)), columns = sizeNumber(thread, c, item(thread, c, declaration, 4));
    if (rows < 2 || rows > 4 || columns < 2 || columns > 4) error(thread, c, declaration, "syntax error: matrix dimensions must be 2 through 4\n");
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_MatrixType_create(thread, Arcadia_VPL_IR_ScalarElementKind_Floating, (Arcadia_Natural8Value)rows, (Arcadia_Natural8Value)columns);
  } else if (word(thread, kind, "array")) {
    shape(thread, c, declaration, "type", 5, 5);
    Arcadia_VPL_IR_Type* element = resolveType(thread, c, item(thread, c, declaration, 3), depth + 1);
    Arcadia_SizeValue size = sizeNumber(thread, c, item(thread, c, declaration, 4));
    if (!size || element->kind == Arcadia_VPL_IR_TypeKind_Void) Arcadia_VPL_Text_fail(thread, c, declaration->offset, Arcadia_Status_SemanticalError, "binding error: array requires a value element and positive size\n");
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_ArrayType_create(thread, element, size);
  } else if (word(thread, kind, "record")) {
    shape(thread, c, declaration, "type", 4, (Arcadia_SizeValue)-1);
    Arcadia_List* fields = list(thread, c);
    for (Arcadia_SizeValue i = 4, n = count(thread, declaration); i < n; ++i) {
      Arcadia_VPL_Text_Node* field = item(thread, c, declaration, i);
      shape(thread, c, field, "field", 3, 3);
      Arcadia_String* fieldName = string(thread, c, item(thread, c, field, 1));
      Arcadia_VPL_IR_Type* fieldType = resolveType(thread, c, item(thread, c, field, 2), depth + 1);
      if (!Arcadia_String_getNumberOfBytes(thread, fieldName) || fieldType->kind == Arcadia_VPL_IR_TypeKind_Void) Arcadia_VPL_Text_fail(thread, c, field->offset, Arcadia_Status_SemanticalError, "binding error: invalid record field\n");
      for (Arcadia_SizeValue j = 0, m = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); j < m; ++j) {
        Arcadia_VPL_IR_Field* old = (Arcadia_VPL_IR_Field*)Arcadia_List_getObjectReferenceValueAt(thread, fields, j);
        if (Arcadia_String_isEqualTo_pn(thread, old->name, Arcadia_String_getBytes(thread, fieldName), Arcadia_String_getNumberOfBytes(thread, fieldName))) Arcadia_VPL_Text_fail(thread, c, field->offset, Arcadia_Status_SemanticalError, "binding error: duplicate record field\n");
      }
      Arcadia_VPL_IR_Field* object = (Arcadia_VPL_IR_Field*)located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_Field_create(thread,
        fieldName, fieldType), field);
      Arcadia_List_insertBackObjectReferenceValue(thread, fields, (Arcadia_Object*)object);
    }
    Arcadia_String* recordName = string(thread, c, item(thread, c, declaration, 3));
    if (!Arcadia_String_getNumberOfBytes(thread, recordName)) Arcadia_VPL_Text_fail(thread, c, declaration->offset, Arcadia_Status_SemanticalError, "binding error: empty record name\n");
    type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_RecordType_create(thread, recordName, fields);
  } else error(thread, c, kind, "syntax error: unknown type shape\n");
  declaration->binding = located(thread, c, (Arcadia_Object*)type, declaration);
  declaration->state = 2;
  return type;
}

static Arcadia_VPL_IR_RegisterValue*
registerValue
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  Arcadia_SizeValue id = registerNumber(thread, c, node);
  Arcadia_Value value = Arcadia_Map_get(thread, c->registers, Arcadia_Value_makeSizeValue(id));
  if (!Arcadia_Value_isObjectReferenceValue(&value)) Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SemanticalError, "binding error: undeclared register\n");
  Arcadia_VPL_IR_RegisterValue* declaration = (Arcadia_VPL_IR_RegisterValue*)Arcadia_Value_getObjectReferenceValue(&value);
  return (Arcadia_VPL_IR_RegisterValue*)located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_RegisterValue_create(thread, id, declaration->type), node);
}

static Arcadia_Real32Value
floatingNumber
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, node->text);
  if (n == (Arcadia_SizeValue)-1) Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SemanticalError, "binding error: floating literal is too long\n");
  char* bytes = (char*)Arcadia_Memory_allocateUnmanaged(thread, n + 1);
  memcpy(bytes, Arcadia_String_getBytes(thread, node->text), n);
  bytes[n] = '\0';
#if defined(_WIN32)
  _locale_t locale = _create_locale(LC_NUMERIC, "C");
#else
  locale_t locale = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
#endif
  if (!locale) {
    Arcadia_Memory_deallocateUnmanaged(thread, bytes);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  fenv_t environment;
  int savedEnvironment = fegetenv(&environment);
  if (savedEnvironment) {
#if defined(_WIN32)
    _free_locale(locale);
#else
    freelocale(locale);
#endif
    Arcadia_Memory_deallocateUnmanaged(thread, bytes);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  fesetround(FE_TONEAREST);
  char* end = NULL;
#if defined(_WIN32)
  Arcadia_Real32Value value = _strtof_l(bytes, &end, locale);
  _free_locale(locale);
#else
  Arcadia_Real32Value value = strtof_l(bytes, &end, locale);
  freelocale(locale);
#endif
  if (!savedEnvironment) fesetenv(&environment);
  Arcadia_BooleanValue complete = end == bytes + n;
  Arcadia_Memory_deallocateUnmanaged(thread, bytes);
  if (!complete || !isfinite(value)) Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SemanticalError, "binding error: floating literal is outside finite binary32 range\n");
  return value;
}

static Arcadia_VPL_IR_Value*
operand
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  if (node->kind == VPL_TEXT_NAME) return (Arcadia_VPL_IR_Value*)registerValue(thread, c, node);
  Arcadia_VPL_Text_Node* kind = item(thread, c, node, 0);
  Arcadia_Natural8Value element = elementKind(thread, c, kind);
  shape(thread, c, node, element == Arcadia_VPL_IR_ScalarElementKind_Boolean ? "bool" : element == Arcadia_VPL_IR_ScalarElementKind_Integer ? "int" : "float", 2, 2);
  Arcadia_VPL_Text_Node* literal = item(thread, c, node, 1);
  Arcadia_Value value;
  if (element == Arcadia_VPL_IR_ScalarElementKind_Boolean) {
    if (!word(thread, literal, "true") && !word(thread, literal, "false")) error(thread, c, literal, "syntax error: expected true or false\n");
    value = Arcadia_Value_makeBooleanValue(word(thread, literal, "true"));
  } else if (element == Arcadia_VPL_IR_ScalarElementKind_Integer) {
    value = Arcadia_Value_makeInteger32Value(integerNumber(thread, c, literal));
  } else {
    if (literal->kind != VPL_TEXT_NUMBER) error(thread, c, literal, "syntax error: expected a floating number\n");
    Arcadia_Real32Value number = floatingNumber(thread, c, literal);
    value = Arcadia_Value_makeReal32Value(number);
  }
  Arcadia_VPL_IR_Type* type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_ScalarType_create(thread, element));
  return (Arcadia_VPL_IR_Value*)located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_ConstantValue_create(thread, type, value), node);
}

static Arcadia_VPL_IR_ReferenceValue*
reference
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  Arcadia_VPL_Text_Node* head = item(thread, c, node, 0);
  Arcadia_VPL_IR_ReferenceValue* result = NULL;
  if (word(thread, head, "member_ref") || word(thread, head, "index_ref")) {
    shape(thread, c, node, word(thread, head, "member_ref") ? "member_ref" : "index_ref", 3, 3);
    Arcadia_VPL_IR_ReferenceValue* base = reference(thread, c, item(thread, c, node, 1));
    if (word(thread, head, "member_ref")) {
      Arcadia_String* name = string(thread, c, item(thread, c, node, 2));
      if (base->type->kind != Arcadia_VPL_IR_TypeKind_Record) error(thread, c, node, "syntax error: member_ref base must have record type\n");
      Arcadia_List* fields = ((Arcadia_VPL_IR_RecordType*)base->type)->fields;
      Arcadia_VPL_IR_Type* type = NULL;
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)fields); i < n; ++i) {
        Arcadia_VPL_IR_Field* field = (Arcadia_VPL_IR_Field*)Arcadia_List_getObjectReferenceValueAt(thread, fields, i);
        if (Arcadia_String_isEqualTo_pn(thread, name, Arcadia_String_getBytes(thread, field->name), Arcadia_String_getNumberOfBytes(thread, field->name))) { type = field->type; break; }
      }
      if (!type) Arcadia_VPL_Text_fail(thread, c, node->offset, Arcadia_Status_SemanticalError, "binding error: unknown record field\n");
      result = (Arcadia_VPL_IR_ReferenceValue*)Arcadia_VPL_IR_MemberReferenceValue_create(thread, base, name, type);
    } else {
      if (base->type->kind != Arcadia_VPL_IR_TypeKind_Array) error(thread, c, node, "syntax error: index_ref base must have array type\n");
      result = (Arcadia_VPL_IR_ReferenceValue*)Arcadia_VPL_IR_IndexReferenceValue_create(thread, base, operand(thread, c, item(thread, c, node, 2)), ((Arcadia_VPL_IR_ArrayType*)base->type)->element);
    }
  } else {
    Arcadia_SizeValue slot = 0;
    Arcadia_Natural8Value kind = 0;
    Arcadia_VPL_IR_Type* type = NULL;
    if (word(thread, head, "local")) {
      shape(thread, c, node, "local", 2, 2);
      Arcadia_VPL_IR_RegisterValue* reg = registerValue(thread, c, item(thread, c, node, 1));
      slot = reg->index; type = reg->type; kind = Arcadia_VPL_IR_StorageKind_Local;
    } else if (word(thread, head, "parameter") || word(thread, head, "uniform") || word(thread, head, "stage")) {
      shape(thread, c, node, word(thread, head, "parameter") ? "parameter" : word(thread, head, "uniform") ? "uniform" : "stage", 2, 2);
      Arcadia_Map* bindings = word(thread, head, "parameter") ? c->parameters : word(thread, head, "uniform") ? c->uniforms : c->interface;
      Arcadia_VPL_Text_Node* declaration = lookup(thread, c, bindings, item(thread, c, node, 1));
      slot = declaration->ordinal;
      if (word(thread, head, "parameter")) { kind = Arcadia_VPL_IR_StorageKind_Parameter; type = ((Arcadia_VPL_IR_ReferenceValue*)declaration->binding)->type; }
      else if (word(thread, head, "stage")) { kind = Arcadia_VPL_IR_StorageKind_StageIO; type = ((Arcadia_VPL_IR_StageIO*)declaration->binding)->type; }
      else {
        kind = Arcadia_VPL_IR_StorageKind_Uniform;
        Arcadia_VPL_IR_Uniform* uniform = (Arcadia_VPL_IR_Uniform*)declaration->binding;
        if (uniform->kind == Arcadia_VPL_IR_UniformKind_Scalar) type = ((Arcadia_VPL_IR_ScalarUniform*)uniform)->type;
        else if (uniform->kind == Arcadia_VPL_IR_UniformKind_Record) type = (Arcadia_VPL_IR_Type*)((Arcadia_VPL_IR_RecordUniform*)uniform)->recordType;
        else type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_Text_own(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_ArrayType_create(thread, ((Arcadia_VPL_IR_ArrayUniform*)uniform)->elementType, ((Arcadia_VPL_IR_ArrayUniform*)uniform)->count));
      }
    } else error(thread, c, head, "syntax error: unknown storage reference\n");
    result = Arcadia_VPL_IR_ReferenceValue_createWithStorage(thread, kind, slot, type);
  }
  return (Arcadia_VPL_IR_ReferenceValue*)located(thread, c, (Arcadia_Object*)result, node);
}

static void
readInstruction
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_IR_Block* block,
    Arcadia_VPL_Text_Node* node
  )
{
  if (block->terminator) error(thread, c, node, "syntax error: instruction follows a terminator\n");
  Arcadia_VPL_Text_Node* head = item(thread, c, node, 0);
  Arcadia_SizeValue n = count(thread, node);
  Arcadia_VPL_IR_Instruction* instruction = NULL;
  Arcadia_VPL_IR_Terminator* terminator = NULL;
  static const char* binaryNames[] = { "add", "subtract", "multiply", "divide", "equal", "not_equal", "less", "less_equal", "greater", "greater_equal" };
  int binary = -1;
  for (int i = 0; i < 10; ++i) if (word(thread, head, binaryNames[i])) binary = i;
  if (binary >= 0) {
    shape(thread, c, node, binaryNames[binary], 4, 4);
    instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_BinaryInstruction_create(thread,
      registerValue(thread, c, item(thread, c, node, 1)), (Arcadia_Natural8Value)binary,
      operand(thread, c, item(thread, c, node, 2)), operand(thread, c, item(thread, c, node, 3)));
  } else if (word(thread, head, "copy")) {
    shape(thread, c, node, "copy", 3, 3);
    instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_CopyInstruction_create(thread, registerValue(thread, c, item(thread, c, node, 1)), operand(thread, c, item(thread, c, node, 2)));
  } else if (word(thread, head, "load")) {
    shape(thread, c, node, "load", 3, 3);
    instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_LoadInstruction_create(thread, registerValue(thread, c, item(thread, c, node, 1)), reference(thread, c, item(thread, c, node, 2)));
  } else if (word(thread, head, "store")) {
    shape(thread, c, node, "store", 3, 3);
    instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_StoreInstruction_create(thread, reference(thread, c, item(thread, c, node, 1)), operand(thread, c, item(thread, c, node, 2)));
  } else if (word(thread, head, "call") || word(thread, head, "texture")) {
    Arcadia_BooleanValue texture = word(thread, head, "texture");
    shape(thread, c, node, texture ? "texture" : "call", texture ? 4 : 3, texture ? 4 : (Arcadia_SizeValue)-1);
    Arcadia_VPL_IR_RegisterValue* result = word(thread, item(thread, c, node, 1), "void") ? NULL : registerValue(thread, c, item(thread, c, node, 1));
    Arcadia_VPL_IR_Function* callee = texture ? NULL : (Arcadia_VPL_IR_Function*)lookup(thread, c, c->functions, item(thread, c, node, 2))->binding;
    Arcadia_List* arguments = list(thread, c);
    for (Arcadia_SizeValue i = texture ? 2 : 3; i < n; ++i) Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_Object*)operand(thread, c, item(thread, c, node, i)));
    instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_CallInstruction_create(thread, result, callee, texture ? Arcadia_VPL_IR_BuiltinFunction_Texture : Arcadia_VPL_IR_BuiltinFunction_None, arguments);
  } else if (word(thread, head, "construct")) {
    shape(thread, c, node, "construct", 2, (Arcadia_SizeValue)-1);
    Arcadia_VPL_IR_RegisterValue* result = registerValue(thread, c, item(thread, c, node, 1));
    Arcadia_List* arguments = list(thread, c);
    for (Arcadia_SizeValue i = 2; i < n; ++i) Arcadia_List_insertBackObjectReferenceValue(thread, arguments, (Arcadia_Object*)operand(thread, c, item(thread, c, node, i)));
    instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_ConstructInstruction_create(thread, result, result->type, arguments);
  } else if (word(thread, head, "swizzle") || word(thread, head, "member") || word(thread, head, "index")) {
    Arcadia_BooleanValue swizzle = word(thread, head, "swizzle"), member = word(thread, head, "member");
    shape(thread, c, node, swizzle ? "swizzle" : member ? "member" : "index", 4, 4);
    Arcadia_VPL_IR_RegisterValue* result = registerValue(thread, c, item(thread, c, node, 1));
    Arcadia_VPL_IR_Value* source = operand(thread, c, item(thread, c, node, 2));
    if (swizzle) instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_SwizzleInstruction_create(thread, result, source, string(thread, c, item(thread, c, node, 3)));
    else if (member) instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_MemberInstruction_create(thread, result, source, string(thread, c, item(thread, c, node, 3)));
    else instruction = (Arcadia_VPL_IR_Instruction*)Arcadia_VPL_IR_IndexInstruction_create(thread, result, source, operand(thread, c, item(thread, c, node, 3)));
  } else if (word(thread, head, "branch")) {
    shape(thread, c, node, "branch", 2, 2);
    terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_BranchTerminator_create(thread, (Arcadia_VPL_IR_Block*)lookup(thread, c, c->blocks, item(thread, c, node, 1))->binding);
  } else if (word(thread, head, "branch_if")) {
    shape(thread, c, node, "branch_if", 4, 4);
    terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ConditionalBranchTerminator_create(thread, operand(thread, c, item(thread, c, node, 1)),
      (Arcadia_VPL_IR_Block*)lookup(thread, c, c->blocks, item(thread, c, node, 2))->binding,
      (Arcadia_VPL_IR_Block*)lookup(thread, c, c->blocks, item(thread, c, node, 3))->binding);
  } else if (word(thread, head, "return")) {
    shape(thread, c, node, "return", 1, 2);
    terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_ReturnTerminator_create(thread, n == 1 ? NULL : operand(thread, c, item(thread, c, node, 1)));
  } else if (word(thread, head, "discard")) {
    shape(thread, c, node, "discard", 1, 1);
    terminator = (Arcadia_VPL_IR_Terminator*)Arcadia_VPL_IR_DiscardTerminator_create(thread);
  } else error(thread, c, head, "syntax error: unknown instruction or terminator\n");
  if (instruction) Arcadia_List_insertBackObjectReferenceValue(thread, block->instructions, located(thread, c, (Arcadia_Object*)instruction, node));
  if (terminator) block->terminator = (Arcadia_VPL_IR_Terminator*)located(thread, c, (Arcadia_Object*)terminator, node);
}

static void
bindFunctionBody
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* declaration
  )
{
  Arcadia_VPL_IR_Function* function = (Arcadia_VPL_IR_Function*)declaration->binding;
  c->parameters = map(thread, c);
  c->registers = map(thread, c);
  c->blocks = map(thread, c);
  Arcadia_VPL_Text_Node* parameters = item(thread, c, declaration, 4);
  for (Arcadia_SizeValue i = 1, n = count(thread, parameters); i < n; ++i) {
    Arcadia_VPL_Text_Node* parameter = item(thread, c, parameters, i);
    define(thread, c, c->parameters, item(thread, c, parameter, 1), parameter);
  }
  Arcadia_VPL_Text_Node* registers = item(thread, c, declaration, 5);
  shape(thread, c, registers, "registers", 1, (Arcadia_SizeValue)-1);
  for (Arcadia_SizeValue i = 1, n = count(thread, registers); i < n; ++i) {
    Arcadia_VPL_Text_Node* definition = item(thread, c, registers, i);
    shape(thread, c, definition, "register", 3, 3);
    Arcadia_VPL_Text_Node* name = item(thread, c, definition, 1);
    Arcadia_SizeValue id = registerNumber(thread, c, name);
    Arcadia_Value old = Arcadia_Map_get(thread, c->registers, Arcadia_Value_makeSizeValue(id));
    if (!Arcadia_Value_isVoidValue(&old)) Arcadia_VPL_Text_fail(thread, c, name->offset, Arcadia_Status_SemanticalError, "binding error: duplicate numeric register ID\n");
    definition->binding = located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_RegisterValue_create(thread, id, resolveType(thread, c, item(thread, c, definition, 2), 0)), definition);
    Arcadia_Map_set(thread, c->registers, Arcadia_Value_makeSizeValue(id), Arcadia_Value_makeObjectReferenceValue(definition->binding), NULL, NULL);
  }
  for (Arcadia_SizeValue i = 6, n = count(thread, declaration); i < n; ++i) {
    Arcadia_VPL_Text_Node* definition = item(thread, c, declaration, i);
    shape(thread, c, definition, "block", 2, (Arcadia_SizeValue)-1);
    define(thread, c, c->blocks, item(thread, c, definition, 1), definition);
    definition->binding = located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_Block_create(thread, list(thread, c), NULL), definition);
    Arcadia_List_insertBackObjectReferenceValue(thread, function->blocks, definition->binding);
  }
  Arcadia_VPL_IR_Block* entry = (Arcadia_VPL_IR_Block*)Arcadia_List_getObjectReferenceValueAt(thread, function->blocks, 0);
  for (Arcadia_SizeValue i = 1, n = count(thread, registers); i < n; ++i) {
    Arcadia_VPL_Text_Node* definition = item(thread, c, registers, i);
    Arcadia_VPL_IR_RegisterValue* reg = (Arcadia_VPL_IR_RegisterValue*)definition->binding;
    Arcadia_List_insertBackObjectReferenceValue(thread, entry->instructions, located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_DeclareLocalInstruction_create(thread, reg, reg->type), definition));
  }
  for (Arcadia_SizeValue i = 6, n = count(thread, declaration); i < n; ++i) {
    Arcadia_VPL_Text_Node* definition = item(thread, c, declaration, i);
    for (Arcadia_SizeValue j = 2, m = count(thread, definition); j < m; ++j) readInstruction(thread, c, (Arcadia_VPL_IR_Block*)definition->binding, item(thread, c, definition, j));
  }
}

static Arcadia_Natural8Value
semantics
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  static const char* names[] = { "none", "position", "color", "texcoord", "fragment_color" };
  for (Arcadia_Natural8Value i = 0; i < 5; ++i) if (word(thread, node, names[i])) return i;
  error(thread, c, node, "syntax error: unknown interface semantic\n");
  return 0;
}

static Arcadia_VPL_IR_Stage*
readStage
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node,
    Arcadia_Natural8Value kind
  )
{
  shape(thread, c, node, kind == Arcadia_VPL_IR_Stage_Vertex ? "vertex" : "fragment", 4, 4);
  c->interface = map(thread, c);
  c->functions = map(thread, c);
  Arcadia_List* interface = list(thread, c);
  Arcadia_List* functions = list(thread, c);
  Arcadia_VPL_Text_Node* ioForm = item(thread, c, node, 2);
  shape(thread, c, ioForm, "interface", 1, (Arcadia_SizeValue)-1);
  for (Arcadia_SizeValue i = 1, n = count(thread, ioForm); i < n; ++i) {
    Arcadia_VPL_Text_Node* definition = item(thread, c, ioForm, i);
    Arcadia_BooleanValue in = word(thread, item(thread, c, definition, 0), "in");
    shape(thread, c, definition, in ? "in" : "out", 6, 6);
    define(thread, c, c->interface, item(thread, c, definition, 1), definition);
    definition->ordinal = i - 1;
    definition->binding = located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_StageIO_create(thread, in ? Arcadia_VPL_IR_StageIO_Direction_In : Arcadia_VPL_IR_StageIO_Direction_Out,
      string(thread, c, item(thread, c, definition, 2)), resolveType(thread, c, item(thread, c, definition, 3), 0),
      integerNumber(thread, c, item(thread, c, definition, 4)), semantics(thread, c, item(thread, c, definition, 5))), definition);
    Arcadia_List_insertBackObjectReferenceValue(thread, interface, definition->binding);
  }
  Arcadia_VPL_Text_Node* fnForm = item(thread, c, node, 3);
  shape(thread, c, fnForm, "functions", 1, (Arcadia_SizeValue)-1);
  for (Arcadia_SizeValue i = 1, n = count(thread, fnForm); i < n; ++i) {
    Arcadia_VPL_Text_Node* definition = item(thread, c, fnForm, i);
    shape(thread, c, definition, "function", 7, (Arcadia_SizeValue)-1);
    define(thread, c, c->functions, item(thread, c, definition, 1), definition);
    Arcadia_VPL_Text_Node* params = item(thread, c, definition, 4);
    shape(thread, c, params, "parameters", 1, (Arcadia_SizeValue)-1);
    Arcadia_List* parameters = list(thread, c);
    Arcadia_Map* names = map(thread, c);
    for (Arcadia_SizeValue j = 1, m = count(thread, params); j < m; ++j) {
      Arcadia_VPL_Text_Node* parameter = item(thread, c, params, j);
      shape(thread, c, parameter, "parameter", 3, 3);
      define(thread, c, names, item(thread, c, parameter, 1), parameter);
      parameter->ordinal = j - 1;
      parameter->binding = located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_ReferenceValue_createWithStorage(thread, Arcadia_VPL_IR_StorageKind_Parameter, j - 1, resolveType(thread, c, item(thread, c, parameter, 2), 0)), parameter);
      Arcadia_List_insertBackObjectReferenceValue(thread, parameters, parameter->binding);
    }
    definition->binding = located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_Function_create(thread, string(thread, c, item(thread, c, definition, 2)),
      resolveType(thread, c, item(thread, c, definition, 3), 0), parameters, list(thread, c)), definition);
    Arcadia_List_insertBackObjectReferenceValue(thread, functions, definition->binding);
  }
  for (Arcadia_SizeValue i = 1, n = count(thread, fnForm); i < n; ++i) bindFunctionBody(thread, c, item(thread, c, fnForm, i));
  return (Arcadia_VPL_IR_Stage*)located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_Stage_createWithEntryFunction(thread, kind, interface, functions,
    (Arcadia_VPL_IR_Function*)lookup(thread, c, c->functions, item(thread, c, node, 1))->binding), node);
}

static Arcadia_VPL_IR_Module*
readModule
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Text_Context* c,
    Arcadia_VPL_Text_Node* node
  )
{
  shape(thread, c, node, "module", 6, 6);
  if (sizeNumber(thread, c, item(thread, c, node, 1)) != 1) Arcadia_VPL_Text_fail(thread, c, item(thread, c, node, 1)->offset, Arcadia_Status_SemanticalError, "binding error: unsupported VPL text version\n");
  Arcadia_VPL_Text_Node* types = item(thread, c, node, 2);
  shape(thread, c, types, "types", 1, (Arcadia_SizeValue)-1);
  for (Arcadia_SizeValue i = 1, n = count(thread, types); i < n; ++i) {
    Arcadia_VPL_Text_Node* declaration = item(thread, c, types, i);
    shape(thread, c, declaration, "type", 3, (Arcadia_SizeValue)-1);
    define(thread, c, c->types, item(thread, c, declaration, 1), declaration);
  }
  // Resolve unused declarations too: malformed or cyclic types are not silently ignored.
  for (Arcadia_SizeValue i = 1, n = count(thread, types); i < n; ++i) resolveType(thread, c, item(thread, c, item(thread, c, types, i), 1), 0);
  Arcadia_VPL_Text_Node* uniforms = item(thread, c, node, 3);
  shape(thread, c, uniforms, "uniforms", 1, (Arcadia_SizeValue)-1);
  Arcadia_List* uniformObjects = list(thread, c);
  for (Arcadia_SizeValue i = 1, n = count(thread, uniforms); i < n; ++i) {
    Arcadia_VPL_Text_Node* declaration = item(thread, c, uniforms, i);
    Arcadia_VPL_Text_Node* head = item(thread, c, declaration, 0);
    Arcadia_BooleanValue array = word(thread, head, "array"), record = word(thread, head, "record");
    shape(thread, c, declaration, array ? "array" : record ? "record" : "scalar", array ? 5 : 4, array ? 5 : 4);
    define(thread, c, c->uniforms, item(thread, c, declaration, 1), declaration);
    declaration->ordinal = i - 1;
    Arcadia_String* name = string(thread, c, item(thread, c, declaration, 2));
    Arcadia_VPL_IR_Type* type = resolveType(thread, c, item(thread, c, declaration, 3), 0);
    Arcadia_VPL_IR_Uniform* uniform;
    if (array) uniform = (Arcadia_VPL_IR_Uniform*)Arcadia_VPL_IR_ArrayUniform_create(thread, name, type, sizeNumber(thread, c, item(thread, c, declaration, 4)));
    else if (record) {
      if (type->kind != Arcadia_VPL_IR_TypeKind_Record) error(thread, c, declaration, "syntax error: record uniform requires a record type\n");
      uniform = (Arcadia_VPL_IR_Uniform*)Arcadia_VPL_IR_RecordUniform_create(thread, name, (Arcadia_VPL_IR_RecordType*)type);
    } else uniform = (Arcadia_VPL_IR_Uniform*)Arcadia_VPL_IR_ScalarUniform_create(thread, name, type);
    declaration->binding = located(thread, c, (Arcadia_Object*)uniform, declaration);
    Arcadia_List_insertBackObjectReferenceValue(thread, uniformObjects, declaration->binding);
  }
  Arcadia_VPL_IR_Stage* vertex = readStage(thread, c, item(thread, c, node, 4), Arcadia_VPL_IR_Stage_Vertex);
  Arcadia_VPL_IR_Stage* fragment = readStage(thread, c, item(thread, c, node, 5), Arcadia_VPL_IR_Stage_Fragment);
  return (Arcadia_VPL_IR_Module*)located(thread, c, (Arcadia_Object*)Arcadia_VPL_IR_Module_create(thread, uniformObjects, vertex, fragment), node);
}

Arcadia_VPL_IR_Module*
Arcadia_VPL_Text_read
  (
    Arcadia_Thread* thread,
    const void* bytes,
    Arcadia_SizeValue size,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_String* sourceName
  )
{
  if ((!bytes && size) || !diagnostics) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_SizeValue stackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_VPL_Text_Context* volatile context = NULL;
  Arcadia_SizeValue diagnosticCount = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics);
  Arcadia_JumpTarget target;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sourceName);
    context = Arcadia_VPL_Text_createContext(thread, diagnostics);
    context->sourceName = sourceName;
    context->source = Arcadia_RuntimeByteArray_create(thread, bytes, size);
    Arcadia_VPL_Text_Node* form = Arcadia_VPL_Text_readForm(thread, context, 0);
    Arcadia_VPL_Text_trivia(thread, context);
    if (context->position != size) error(thread, context, form, "syntax error: trailing input after module\n");
    Arcadia_VPL_IR_Module* module = readModule(thread, context, form);
    Arcadia_VPL_IR_Verifier* verifier = (Arcadia_VPL_IR_Verifier*)Arcadia_VPL_Text_own(thread, context, (Arcadia_Object*)Arcadia_VPL_IR_Verifier_create(thread, diagnostics));
    Arcadia_VPL_IR_Verifier_run(thread, verifier, module);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_popJumpTarget(thread);
    return module;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (context && context->source && Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics) > diagnosticCount) {
      Arcadia_Object* object = Arcadia_List_getObjectReferenceValueAt(thread, diagnostics->diagnostics, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)diagnostics->diagnostics) - 1);
      if (Arcadia_Object_isInstanceOf(thread, object, _Arcadia_VPL_Diagnostics_VerificationDiagnostic_getType(thread))) {
        Arcadia_VPL_Diagnostics_VerificationDiagnostic* diagnostic = (Arcadia_VPL_Diagnostics_VerificationDiagnostic*)object;
        if (!diagnostic->line) {
          Arcadia_Value offset = diagnostic->subject ? Arcadia_Map_get(thread, context->locations, Arcadia_Value_makeObjectReferenceValue(diagnostic->subject)) : Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
          Arcadia_VPL_Text_locate(thread, context, diagnostic, Arcadia_Value_isSizeValue(&offset) ? Arcadia_Value_getSizeValue(&offset) : 0);
        }
      }
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - stackSize);
    Arcadia_Thread_jump(thread);
    return NULL;
  }
}
