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

#include "Arcadia/PDLC/Backend/CallableWriter.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/ModuleSymbolInfo.h"
#include "Arcadia/PDLC/Backend/SymbolInfo.h"
#include "Arcadia/PDLC/Environment.h"
#include <string.h>

typedef struct Writer Writer;

struct Writer {
  Arcadia_Thread* thread;
  Arcadia_PDLC_Context* context;
  Arcadia_PDLC_Symbol* symbol;
  Arcadia_StringBuilder* output;
  Arcadia_Map* slots;
  Arcadia_Map* labels;
  Arcadia_List* dependencies;
  Arcadia_List* nativeMethods;
  Arcadia_List* nativeEnumerationValues;
  Arcadia_List* nativeEnumerationConversions;
  Arcadia_List* includedHeaders;
  Arcadia_String* modelName;
  unsigned conversions;
  Arcadia_SizeValue slotCount;
  Arcadia_SizeValue offset;
  Arcadia_BooleanValue header;
};

static Arcadia_String*
nativeTarget
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s
  );

static void
checkType
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type,
    Arcadia_String* expression
  );

typedef struct Arcadia_PDLC_CallableDiagnostic Arcadia_PDLC_CallableDiagnostic;
typedef struct Arcadia_PDLC_CallableDiagnosticDispatch Arcadia_PDLC_CallableDiagnosticDispatch;

struct Arcadia_PDLC_CallableDiagnostic {
  Arcadia_Languages_Diagnostic parent;
  Arcadia_String* message;
};

struct Arcadia_PDLC_CallableDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch parent;
};

Arcadia_declareObjectType(u8"Arcadia.PDLC.CallableDiagnostic", Arcadia_PDLC_CallableDiagnostic,
                         u8"Arcadia.Languages.Diagnostic");

static void
diagnosticConstruct
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CallableDiagnostic* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_CallableDiagnostic);
  Arcadia_Value file = Arcadia_ValueStack_getValue(thread, 3);
  Arcadia_Value offset = Arcadia_ValueStack_getValue(thread, 2);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Languages_DiagnosticType_getType(thread), Arcadia_Languages_DiagnosticType_Error));
  Arcadia_ValueStack_pushValue(thread, &file);
  Arcadia_ValueStack_pushValue(thread, &offset);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->message = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_CallableDiagnostic);
}

static Arcadia_String*
diagnosticMessage
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CallableDiagnostic* self
  )
{ return self->message; }

static void
diagnosticVisit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CallableDiagnostic* self
  )
{ Arcadia_Object_visit(thread, (Arcadia_Object*)self->message); }

static void
diagnosticDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_CallableDiagnosticDispatch* self
  )
{ self->parent.getMessage = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*))diagnosticMessage; }

static const Arcadia_ObjectType_Operations diagnosticObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)diagnosticConstruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)diagnosticVisit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)diagnosticDispatch,
};

static const Arcadia_Type_Operations diagnosticOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &diagnosticObjectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.CallableDiagnostic", Arcadia_PDLC_CallableDiagnostic,
                        u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                        &diagnosticOperations);

static void
fail
  (
    Writer* w,
    Arcadia_String* message
  )
{
  Arcadia_Thread* thread = w->thread;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, message);
  Arcadia_PDLC_Environment* e = NULL;
  for (Arcadia_PDLC_Symbol* s = w->symbol; s && !e; s = s->enclosing) {
    Arcadia_Value v = Arcadia_Map_get(thread, w->context->environments, Arcadia_Value_makeObjectReferenceValue(s));
    if (!Arcadia_Value_isVoidValue(&v)) e = (void*)Arcadia_Value_getObjectReferenceValue(&v);
  }
  Arcadia_Languages_InputFile* file = NULL;
  if (e && e->compilationUnitNode) {
    Arcadia_FilePath* path = e->compilationUnitNode->filePath;
    file = Arcadia_Languages_InputFileManager_createPhysicalInputFile(thread, w->context->inputFileManager, Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_False), path);
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  if (file) Arcadia_ValueStack_pushObjectReferenceValue(thread, file); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushSizeValue(thread, w->offset);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, message);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  Arcadia_Languages_Diagnostic* d = _Arcadia_EndCreate0(thread, _Arcadia_PDLC_CallableDiagnostic_getType(thread), base);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Languages_Diagnostics_add(thread, w->context->diagnostics, d);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_SemanticalError);
  Arcadia_Thread_jump(thread);
}

static void
text
  (
    Writer* w,
    const char* s
  )
{ Arcadia_StringBuilder_insertBackCxxString(w->thread, w->output, s); }

static void
string
  (
    Writer* w,
    Arcadia_String* s
  )
{ Arcadia_StringBuilder_insertBackString(w->thread, w->output, s); }

static void
number
  (
    Writer* w,
    Arcadia_SizeValue n
  )
{
  Arcadia_String* s = Arcadia_String_createFromSize(w->thread, n);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, s);
  string(w, s);
  Arcadia_ValueStack_popValues(w->thread, 1);
}

static Arcadia_SizeValue
size
  (
    Writer* w,
    Arcadia_List* list
  )
{ return list ? Arcadia_Collection_getSize(w->thread, (Arcadia_Collection*)list) : 0; }

static void*
at
  (
    Writer* w,
    Arcadia_List* list,
    Arcadia_SizeValue index
  )
{ return Arcadia_List_getObjectReferenceValueAt(w->thread, list, index); }

#define IS(w, o, T) ((o) && Arcadia_Object_isInstanceOf((w)->thread, (Arcadia_Object*)(o), _Arcadia_PDLC_AST_##T##_getType((w)->thread)))

static Arcadia_BooleanValue
equals
  (
    Writer* w,
    Arcadia_String* s,
    const char* value
  )
{ return Arcadia_String_isEqualTo_pn(w->thread, s, value, strlen(value)); }

static Arcadia_PDLC_ModuleSymbol*
module
  (
    Arcadia_PDLC_Symbol* s
  )
{
  while (s && s->kind != Arcadia_PDLC_SymbolKind_Module) s = s->enclosing;
  return (Arcadia_PDLC_ModuleSymbol*)s;
}

static Arcadia_String*
rawCname
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s
  )
{
  Arcadia_StringBuilder* b = Arcadia_StringBuilder_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, b);
  Arcadia_BooleanValue qualifiedProcedure = s->kind == Arcadia_PDLC_SymbolKind_Procedure &&
    memchr(Arcadia_String_getBytes(w->thread, s->name), '.', Arcadia_String_getNumberOfBytes(w->thread, s->name)) != NULL;
  if ((s->kind == Arcadia_PDLC_SymbolKind_Procedure && !qualifiedProcedure) || s->kind == Arcadia_PDLC_SymbolKind_Method || s->kind == Arcadia_PDLC_SymbolKind_Constructor) {
    Arcadia_StringBuilder_insertBackString(w->thread, b, s->enclosing->name);
    Arcadia_StringBuilder_insertBackCxxString(w->thread, b, "_");
  }
  if (s->kind == Arcadia_PDLC_SymbolKind_Constructor) Arcadia_StringBuilder_insertBackCxxString(w->thread, b, "construct");
  else Arcadia_StringBuilder_insertBackString(w->thread, b, s->name);
  Arcadia_String* name = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(b));
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, name);
  Arcadia_String* result = Arcadia_PDLC_Backend_Implementation_computeCxxName(w->thread, Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context), name);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, result);
  return result;
}

static void
addSymbol
  (
    Writer* w,
    Arcadia_List* symbols,
    Arcadia_PDLC_Symbol* symbol
  )
{
  if (!symbols) return;
  for (Arcadia_SizeValue i = 0; i < size(w, symbols); ++i) {
    if (at(w, symbols, i) == symbol) return;
  }
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(w->thread);
  Arcadia_String* name = rawCname(w, symbol);
  Arcadia_SizeValue index = 0;
  for (; index < size(w, symbols); ++index) {
    Arcadia_String* other = rawCname(w, at(w, symbols, index));
    Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(w->thread, name);
    Arcadia_SizeValue m = Arcadia_String_getNumberOfBytes(w->thread, other);
    int order = memcmp(Arcadia_String_getBytes(w->thread, name), Arcadia_String_getBytes(w->thread, other), n < m ? n : m);
    if (order < 0 || (!order && n < m)) break;
  }
  Arcadia_List_insertObjectReferenceValueAt(w->thread, symbols, index, symbol);
  Arcadia_ValueStack_popValues(w->thread, Arcadia_ValueStack_getSize(w->thread) - base);
}

static Arcadia_String*
cname
  (
    Writer* w,
    Arcadia_PDLC_Symbol* symbol
  )
{
  // During source rendering, every emitted PDL name contributes its declaring header.
  Arcadia_PDLC_Symbol* dependency = symbol;
  if (symbol->kind == Arcadia_PDLC_SymbolKind_Method || symbol->kind == Arcadia_PDLC_SymbolKind_Constructor) dependency = symbol->enclosing;
  if (dependency->kind == Arcadia_PDLC_SymbolKind_Class || dependency->kind == Arcadia_PDLC_SymbolKind_Interface || dependency->kind == Arcadia_PDLC_SymbolKind_Enumeration || dependency->kind == Arcadia_PDLC_SymbolKind_Procedure) {
    addSymbol(w, w->dependencies, dependency);
  }
  return rawCname(w, symbol);
}

static Arcadia_List*
parameters
  (
    Arcadia_PDLC_Symbol* s
  )
{
  if (s->kind == Arcadia_PDLC_SymbolKind_Procedure) return ((Arcadia_PDLC_ProcedureSymbol*)s)->parameters;
  if (s->kind == Arcadia_PDLC_SymbolKind_Method) return ((Arcadia_PDLC_MethodSymbol*)s)->parameters;
  return ((Arcadia_PDLC_ConstructorSymbol*)s)->parameters;
}

static void
memberName
  (
    Writer* w,
    Arcadia_String* name
  )
{
  static char const* const keywords[] = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break",
    "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept",
    "const", "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await", "co_return", "co_yield",
    "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern",
    "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
    "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public",
    "register", "reinterpret_cast", "requires", "restrict", "return", "short", "signed", "sizeof", "static",
    "static_assert", "static_cast", "struct", "switch", "template", "this", "thread_local", "throw", "true",
    "try", "typedef", "typeid", "typename", "typeof", "typeof_unqual", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
  };
  char const* bytes = Arcadia_String_getBytes(w->thread, name);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(w->thread, name);
  if (n >= 2 && bytes[0] == '_' && bytes[1] >= 'A' && bytes[1] <= 'Z') fail(w, Arcadia_String_createFromCxxString(w->thread, "member name is reserved by C/C++"));
  for (Arcadia_SizeValue i = 1; i < n; ++i) {
    if (bytes[i - 1] == '_' && bytes[i] == '_') fail(w, Arcadia_String_createFromCxxString(w->thread, "member name is reserved by C/C++"));
  }
  Arcadia_BooleanValue escape = equals(w, name, "_parent") || (n >= 4 && !memcmp(bytes + n - 4, "_pdl", 4));
  for (Arcadia_SizeValue i = 0; !escape && i < sizeof(keywords) / sizeof(keywords[0]); ++i) escape = equals(w, name, keywords[i]);
  string(w, name);
  if (escape) text(w, "_pdl");
}

static Arcadia_PDLC_Symbol*
returnType
  (
    Arcadia_PDLC_Symbol* s
  )
{
  if (s->kind == Arcadia_PDLC_SymbolKind_Procedure) return ((Arcadia_PDLC_ProcedureSymbol*)s)->returnValueType;
  if (s->kind == Arcadia_PDLC_SymbolKind_Method) return ((Arcadia_PDLC_MethodSymbol*)s)->returnValueType;
  return NULL;
}

static Arcadia_List*
body
  (
    Arcadia_PDLC_Symbol* s
  )
{
  if (s->kind == Arcadia_PDLC_SymbolKind_Procedure) return ((Arcadia_PDLC_ProcedureSymbol*)s)->ast ? ((Arcadia_PDLC_ProcedureSymbol*)s)->ast->body : NULL;
  if (s->kind == Arcadia_PDLC_SymbolKind_Method) return ((Arcadia_PDLC_MethodSymbol*)s)->ast ? ((Arcadia_PDLC_MethodSymbol*)s)->ast->body : NULL;
  return ((Arcadia_PDLC_ConstructorSymbol*)s)->ast ? ((Arcadia_PDLC_ConstructorSymbol*)s)->ast->body : NULL;
}

static Arcadia_BooleanValue
isVoid
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type
  )
{ return !type || equals(w, type->name, "Arcadia.Void"); }

static void
typeName
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type
  )
{
  if (isVoid(w, type)) { text(w, "void"); return; }
  if (type->kind == Arcadia_PDLC_SymbolKind_Interface) { text(w, "Arcadia_Object*"); return; }
  string(w, cname(w, type));
  if (type->kind == Arcadia_PDLC_SymbolKind_Class) text(w, "*");
  else if (type->kind == Arcadia_PDLC_SymbolKind_PrimitiveType && !equals(w, type->name, "Arcadia.Value")) text(w, "Value");
}

static void
variableType
  (
    Writer* w,
    Arcadia_PDLC_VariableSymbol* variable
  )
{
  if (variable->recordFields) {
    text(w, "struct { ");
    for (Arcadia_SizeValue i = 0; i < size(w, variable->recordFields); ++i) {
      Arcadia_PDLC_VariableSymbol* field = at(w, variable->recordFields, i);
      variableType(w, field); text(w, " "); memberName(w, ((Arcadia_PDLC_Symbol*)field)->name); text(w, "; ");
    }
    text(w, "}");
    return;
  }
  if (variable->isReadOnly) text(w, "const ");
  if (isVoid(w, variable->type) && !variable->isReference) text(w, "Arcadia_VoidValue");
  else typeName(w, variable->type);
  if (variable->isReference) text(w, "*");
}

static void
modelName
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  if (w->modelName) string(w, w->modelName);
  else string(w, cname(w, (Arcadia_PDLC_Symbol*)c));
}

static const char*
suffix
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type
  )
{
  static const char* types[] = { "Boolean", "Integer8", "Integer16", "Integer32", "Integer64", "Natural8", "Natural16", "Natural32", "Natural64", "Real32", "Real64", "Size", "Void" };
  if (!type || type->kind == Arcadia_PDLC_SymbolKind_Class || type->kind == Arcadia_PDLC_SymbolKind_Interface) return "ObjectReference";
  if (type->kind == Arcadia_PDLC_SymbolKind_Enumeration) return "Enumeration";
  for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
    Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(w->thread);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, builder);
    Arcadia_StringBuilder_insertBackCxxString(w->thread, builder, "Arcadia.");
    Arcadia_StringBuilder_insertBackCxxString(w->thread, builder, types[i]);
    Arcadia_String* qualified = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(builder));
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, qualified);
    Arcadia_BooleanValue match = Arcadia_String_isEqualTo_pn(w->thread, type->name, Arcadia_String_getBytes(w->thread, qualified), Arcadia_String_getNumberOfBytes(w->thread, qualified));
    Arcadia_ValueStack_popValues(w->thread, 2);
    if (match) return types[i];
  }
  fail(w, Arcadia_String_createFromCxxString(w->thread, "unsupported signature type"));
  return NULL;
}

static void
quoted
  (
    Writer* w,
    Arcadia_String* s
  )
{
  const unsigned char* bytes = Arcadia_String_getBytes(w->thread, s);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(w->thread, s);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, s);
  text(w, "\"");
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(w->thread, w->output, '\\');
    Arcadia_StringBuilder_insertBackCodePoint(w->thread, w->output, '0' + ((bytes[i] >> 6) & 7));
    Arcadia_StringBuilder_insertBackCodePoint(w->thread, w->output, '0' + ((bytes[i] >> 3) & 7));
    Arcadia_StringBuilder_insertBackCodePoint(w->thread, w->output, '0' + (bytes[i] & 7));
  }
  text(w, "\"");
  Arcadia_ValueStack_popValues(w->thread, 1);
}

static Arcadia_SizeValue
slot
  (
    Writer* w,
    Arcadia_String* name,
    Arcadia_BooleanValue declare
  )
{
  Arcadia_Value k = Arcadia_Value_makeObjectReferenceValue(name);
  Arcadia_Value v = Arcadia_Map_get(w->thread, w->slots, k);
  if (!Arcadia_Value_isVoidValue(&v)) {
    if (declare) fail(w, Arcadia_String_createFromCxxString(w->thread, "duplicate local variable or parameter"));
    return Arcadia_Value_getSizeValue(&v);
  }
  if (!declare) {
    Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(w->thread);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, message);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, name);
    Arcadia_StringBuilder_insertBackCxxString(w->thread, message, "undefined local variable or parameter `");
    Arcadia_StringBuilder_insertBackString(w->thread, message, name);
    Arcadia_StringBuilder_insertBackCxxString(w->thread, message, "`");
    fail(w, Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(message)));
  }
  Arcadia_SizeValue i = w->slotCount++;
  Arcadia_Map_set(w->thread, w->slots, k, Arcadia_Value_makeSizeValue(i), NULL, NULL);
  return i;
}

static void
get
  (
    Writer* w,
    Arcadia_SizeValue i
  )
{ text(w, "pdlGet(thread, base, "); number(w, i); text(w, ")"); }

static Arcadia_String*
pdlGetExpression
  (
    Writer* w,
    Arcadia_SizeValue i
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, builder);
  Arcadia_StringBuilder_insertBackCxxString(w->thread, builder, "pdlGet(thread, base, ");
  Arcadia_String* index = Arcadia_String_createFromSize(w->thread, i);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, index);
  Arcadia_StringBuilder_insertBackString(w->thread, builder, index);
  Arcadia_StringBuilder_insertBackCxxString(w->thread, builder, ")");
  Arcadia_String* result = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(builder));
  Arcadia_ValueStack_popValues(w->thread, 2);
  return result;
}

static void
put
  (
    Writer* w,
    Arcadia_SizeValue i
  )
{ text(w, "  pdlPut(thread, base, "); number(w, i); text(w, ", "); }

static Arcadia_SizeValue
target
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  )
{
  if (IS(w, n, VariableOperandNode)) return slot(w, ((Arcadia_PDLC_AST_VariableOperandNode*)n)->value, Arcadia_BooleanValue_False);
  if (IS(w, n, RegisterOperandNode)) {
    Arcadia_String* name = ((Arcadia_PDLC_AST_RegisterOperandNode*)n)->value;
    Arcadia_Value v = Arcadia_Map_get(w->thread, w->slots, Arcadia_Value_makeObjectReferenceValue(name));
    return slot(w, name, Arcadia_Value_isVoidValue(&v));
  }
  fail(w, Arcadia_String_createFromCxxString(w->thread, "instruction target must be a register or local variable"));
  return 0;
}

static Arcadia_BooleanValue
enumerationOperand
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  );

static void
operand
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  )
{
  if (IS(w, n, VariableOperandNode) && enumerationOperand(w, n)) return;
  if (IS(w, n, VariableOperandNode) || IS(w, n, RegisterOperandNode)) { get(w, target(w, n)); return; }
  if (!IS(w, n, LiteralOperandNode)) fail(w, Arcadia_String_createFromCxxString(w->thread, "invalid instruction operand"));
  Arcadia_PDLC_AST_Node* l = ((Arcadia_PDLC_AST_LiteralOperandNode*)n)->literal;
  if (IS(w, l, VoidLiteralNode)) text(w, "Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)");
  else if (IS(w, l, BooleanLiteralNode)) {
    text(w, "Arcadia_Value_makeBooleanValue(");
    text(w, equals(w, ((Arcadia_PDLC_AST_BooleanLiteralNode*)l)->value, "true") ? "Arcadia_BooleanValue_True" : "Arcadia_BooleanValue_False"); text(w, ")");
  } else if (IS(w, l, IntegerLiteralNode)) {
    Arcadia_Natural64Value value = 0;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(w->thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      value = Arcadia_String_toNatural64(w->thread, ((Arcadia_PDLC_AST_IntegerLiteralNode*)l)->value);
      Arcadia_Thread_popJumpTarget(w->thread);
    } else {
      Arcadia_Thread_popJumpTarget(w->thread);
      Arcadia_Thread_setStatus(w->thread, 0);
      fail(w, Arcadia_String_createFromCxxString(w->thread, "integer operand is outside the Arcadia.Natural64 range"));
    }
    Arcadia_String* constant = Arcadia_String_createFromNatural64(w->thread, value);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, constant);
    text(w, "Arcadia_Value_makeNatural64Value(UINT64_C("); string(w, constant); text(w, "))");
    Arcadia_ValueStack_popValues(w->thread, 1);
  } else if (IS(w, l, RealLiteralNode)) {
    text(w, "Arcadia_Value_makeReal64Value("); string(w, ((Arcadia_PDLC_AST_RealLiteralNode*)l)->value); text(w, ")");
  } else if (IS(w, l, StringLiteralNode)) {
    Arcadia_String* value = ((Arcadia_PDLC_AST_StringLiteralNode*)l)->value;
    text(w, "Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, "); quoted(w, value); text(w, ", ");
    number(w, Arcadia_String_getNumberOfBytes(w->thread, value)); text(w, "))");
  } else fail(w, Arcadia_String_createFromCxxString(w->thread, "unsupported literal"));
}

static void
prepare
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n,
    Arcadia_SizeValue i
  )
{ put(w, i); operand(w, n); text(w, ");\n"); }

static void
assign
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  )
{
  if (IS(w, n, VariableOperandNode) && w->symbol->kind != Arcadia_PDLC_SymbolKind_Procedure && equals(w, ((Arcadia_PDLC_AST_VariableOperandNode*)n)->value, "self")) fail(w, Arcadia_String_createFromCxxString(w->thread, "self is not a writable target"));
  put(w, target(w, n)); get(w, 0); text(w, ");\n");
}

static Arcadia_PDLC_Symbol*
member
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c,
    Arcadia_String* name
  )
{
  for (; c; c = c->parentClassSymbol) {
    Arcadia_PDLC_Symbol* s = c->scope ? (void*)Arcadia_Languages_Scope_lookup(w->thread, c->scope, name, Arcadia_BooleanValue_False) : NULL;
    if (s) return s;
  }
  return NULL;
}

static const char* conversionTypes[] = {
  "Void", "ObjectReference", "Enumeration", "Boolean", "Integer8", "Integer16", "Integer32", "Integer64",
  "Natural8", "Natural16", "Natural32", "Natural64", "Real32", "Real64", "Size"
};

enum { Conversion_NativeEnumerationBox = 1u << 15 };

static const char*
useConversion
  (
    Writer* w,
    const char* name
  )
{
  for (size_t i = 0; i < sizeof(conversionTypes) / sizeof(conversionTypes[0]); ++i) {
    if (!strcmp(name, conversionTypes[i])) {
      w->conversions |= 1u << i;
      break;
    }
  }
  return name;
}

static void
convert
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type,
    Arcadia_String* expression
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, expression);
  if (type && type->kind == Arcadia_PDLC_SymbolKind_Enumeration && ((Arcadia_PDLC_EnumerationSymbol*)type)->isNative) {
    useConversion(w, "Enumeration");
    if (w->nativeEnumerationValues) addSymbol(w, w->nativeEnumerationValues, type);
    if (w->nativeEnumerationConversions) addSymbol(w, w->nativeEnumerationConversions, type);
    text(w, "pdlAsNativeEnumeration_"); string(w, cname(w, type)); text(w, "(thread, "); string(w, expression); text(w, ")");
    Arcadia_ValueStack_popValues(w->thread, 1);
    return;
  }
  if (type && equals(w, type->name, "Arcadia.Value")) {
    string(w, expression);
    Arcadia_ValueStack_popValues(w->thread, 1);
    return;
  }
  text(w, "(");
  if (isVoid(w, type)) text(w, "Arcadia_VoidValue"); else typeName(w, type);
  text(w, ")pdlAs"); text(w, useConversion(w, suffix(w, type))); text(w, "(thread, "); string(w, expression);
  if (type && (type->kind == Arcadia_PDLC_SymbolKind_Class || type->kind == Arcadia_PDLC_SymbolKind_Interface || type->kind == Arcadia_PDLC_SymbolKind_Enumeration)) {
    text(w, ", _"); string(w, cname(w, type)); text(w, "_getType(thread)");
  }
  text(w, ")");
  Arcadia_ValueStack_popValues(w->thread, 1);
}

static void
boxBegin
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type
  )
{
  if (type && type->kind == Arcadia_PDLC_SymbolKind_Enumeration && ((Arcadia_PDLC_EnumerationSymbol*)type)->isNative) {
    w->conversions |= Conversion_NativeEnumerationBox;
    if (w->nativeEnumerationValues) addSymbol(w, w->nativeEnumerationValues, type);
    text(w, "pdlBoxNativeEnumeration(thread, _"); string(w, cname(w, type)); text(w, "_getType(thread), ");
    return;
  }
  if (type && equals(w, type->name, "Arcadia.Value")) return;
  if (type && (type->kind == Arcadia_PDLC_SymbolKind_Class || type->kind == Arcadia_PDLC_SymbolKind_Interface)) {
    text(w, "pdlBoxObjectReference(");
    return;
  }
  text(w, "Arcadia_Value_make"); text(w, suffix(w, type)); text(w, "Value(");
  if (type && type->kind == Arcadia_PDLC_SymbolKind_Enumeration) {
    text(w, "Arcadia_EnumerationValue_make(_"); string(w, cname(w, type)); text(w, "_getType(thread), ");
  }
}

static void
parameterBoxBegin
  (
    Writer* w,
    Arcadia_PDLC_VariableSymbol* parameter
  )
{
  if (parameter->isReference || parameter->recordFields) fail(w, Arcadia_String_createFromCxxString(w->thread, "external references and record values require a runtime value representation for executable code"));
  boxBegin(w, parameter->type);
  if (parameter->isReadOnly) text(w, "(Arcadia_Object*)");
}

static void
boxEnd
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type
  )
{
  if (type && equals(w, type->name, "Arcadia.Value")) return;
  if (type && type->kind == Arcadia_PDLC_SymbolKind_Enumeration && ((Arcadia_PDLC_EnumerationSymbol*)type)->isNative) {
    text(w, ")");
    return;
  }
  if (type && type->kind == Arcadia_PDLC_SymbolKind_Enumeration) text(w, ")");
  text(w, ")");
}

static Arcadia_String*
argumentExpression
  (
    Writer* w,
    Arcadia_SizeValue index
  )
{
  Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, builder);
  Arcadia_StringBuilder_insertBackCxxString(w->thread, builder, "args[");
  Arcadia_String* number = Arcadia_String_createFromSize(w->thread, index);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, number);
  Arcadia_StringBuilder_insertBackString(w->thread, builder, number);
  Arcadia_StringBuilder_insertBackCxxString(w->thread, builder, "]");
  Arcadia_String* result = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(builder));
  Arcadia_ValueStack_popValues(w->thread, 2);
  return result;
}

static void
callArguments
  (
    Writer* w,
    Arcadia_PDLC_Symbol* callable
  )
{
  Arcadia_List* p = parameters(callable);
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    Arcadia_PDLC_VariableSymbol* parameter = at(w, p, i);
    if (parameter->isReference || parameter->recordFields) fail(w, Arcadia_String_createFromCxxString(w->thread, "external references and record values require a runtime value representation for executable code"));
    text(w, ", "); convert(w, ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type, argumentExpression(w, i));
  }
}

static void
helpers
  (
    Writer* w
  )
{
  text(w,
    "static Arcadia_Value\n"
    "pdlGet\n"
    "  (\n"
    "    Arcadia_Thread* t,\n"
    "    Arcadia_SizeValue b,\n"
    "    Arcadia_SizeValue i\n"
    "  )\n"
    "{\n"
    "  return Arcadia_ValueStack_getValue(t, Arcadia_ValueStack_getSize(t)-b-i-1);\n"
    "}\n"
    "\n"
    "static void\n"
    "pdlPut\n"
    "  (\n"
    "    Arcadia_Thread* t,\n"
    "    Arcadia_SizeValue b,\n"
    "    Arcadia_SizeValue i,\n"
    "    Arcadia_Value v\n"
    "  )\n"
    "{\n"
    "  Arcadia_ValueStack_setValue(t, Arcadia_ValueStack_getSize(t)-b-i-1, &v);\n"
    "}\n"
    "\n"
    "static void\n"
    "pdlRequire\n"
    "  (\n"
    "    Arcadia_Thread* t,\n"
    "    Arcadia_BooleanValue b\n"
    "  )\n"
    "{\n"
    "  if (!b) { Arcadia_Thread_setStatus(t, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(t); }\n"
    "}\n");
  if (w->conversions & (1u << 0)) text(w,
    "\n"
    "static Arcadia_VoidValue\n"
    "pdlAsVoid\n"
    "  (\n"
    "    Arcadia_Thread* t,\n"
    "    Arcadia_Value v\n"
    "  )\n"
    "{\n"
    "  pdlRequire(t, Arcadia_Value_isVoidValue(&v));\n"
    "  return Arcadia_VoidValue_Void;\n"
    "}\n");
  text(w, "\n"
    "static Arcadia_Value\n"
    "pdlBoxObjectReference\n"
    "  (\n"
    "    Arcadia_ObjectReferenceValue object\n"
    "  )\n"
    "{\n"
    "  return object ? Arcadia_Value_makeObjectReferenceValue(object) : Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);\n"
    "}\n");
  if (w->conversions & (1u << 1)) text(w, "\n"
    "static Arcadia_Object*\n"
    "pdlAsObjectReference\n"
    "  (\n"
    "    Arcadia_Thread* thread,\n"
    "    Arcadia_Value v,\n"
    "    Arcadia_Type* type\n"
    "  )\n"
    "{\n"
    "  if (Arcadia_Value_isVoidValue(&v)) return NULL;\n"
    "  pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&v) && Arcadia_Value_getObjectReferenceValue(&v));\n"
    "  return (Arcadia_Object*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, type);\n"
    "}\n");
  if (w->conversions & Conversion_NativeEnumerationBox) text(w, "\n"
    "static Arcadia_Value\n"
    "pdlBoxNativeEnumeration\n"
    "  (\n"
    "    Arcadia_Thread* thread,\n"
    "    Arcadia_Type* type,\n"
    "    Arcadia_Integer64Value value\n"
    "  )\n"
    "{\n"
    "  pdlRequire(thread, value >= Arcadia_Integer32Value_Minimum && value <= Arcadia_Integer32Value_Maximum);\n"
    "  return Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(type, (Arcadia_Integer32Value)value));\n"
    "}\n");
  if (w->conversions & (1u << 2)) text(w, "\n"
    "static Arcadia_Integer32Value\n"
    "pdlAsEnumeration\n"
    "  (\n"
    "    Arcadia_Thread* t,\n"
    "    Arcadia_Value v,\n"
    "    Arcadia_Type* type\n"
    "  )\n"
    "{\n"
    "  pdlRequire(t, Arcadia_Value_isEnumerationValue(&v) && Arcadia_Value_getEnumerationValue(&v).type == type);\n"
    "  return Arcadia_Value_getEnumerationValue(&v).value;\n"
    "}\n");
  for (size_t i = 3; i < sizeof(conversionTypes) / sizeof(conversionTypes[0]); ++i) {
    if (!(w->conversions & (1u << i))) continue;
    text(w, "\nstatic Arcadia_"); text(w, conversionTypes[i]); text(w, "Value\npdlAs"); text(w, conversionTypes[i]);
    text(w, "\n  (\n    Arcadia_Thread* t,\n    Arcadia_Value v\n  )\n{\n  pdlRequire(t, Arcadia_Value_is");
    text(w, conversionTypes[i]); text(w, "Value(&v));\n  return Arcadia_Value_get"); text(w, conversionTypes[i]); text(w, "Value(&v);\n}\n");
  }
}

static void
operation
  (
    Writer* w,
    const char* name,
    Arcadia_SizeValue count
  )
{
  text(w, "  { Arcadia_Value a = "); get(w, 1);
  text(w, ";\n    Arcadia_ForeignProcedure* op = Arcadia_Type_getOperations(Arcadia_Value_getType(thread, &a))->"); text(w, name);
  text(w, ";\n");
  Arcadia_BooleanValue enumerationEquality = count == 2 && (!strcmp(name, "isEqualTo") || !strcmp(name, "isNotEqualTo"));
  if (enumerationEquality) {
    text(w, "    if (!op && Arcadia_Value_isEnumerationValue(&a)) {\n      Arcadia_Value b = "); get(w, 2);
    text(w, ";\n      Arcadia_BooleanValue equal = Arcadia_Value_isEnumerationValue(&b) && Arcadia_Value_isEqualTo(thread, &a, &b);\n");
    put(w, 0); text(w, "Arcadia_Value_makeBooleanValue(");
    text(w, !strcmp(name, "isNotEqualTo") ? "!equal" : "equal");
    text(w, "));\n    } else {\n");
  }
  text(w, "    if (!op) { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }\n");
  for (Arcadia_SizeValue i = 1; i <= count; ++i) {
    text(w, "    { Arcadia_Value v = "); get(w, i); text(w, "; Arcadia_ValueStack_pushValue(thread, &v); }\n");
  }
  text(w, "    Arcadia_ValueStack_pushNatural8Value(thread, "); number(w, count); text(w, ");\n    op(thread);\n");
  put(w, 0); text(w, "Arcadia_ValueStack_getValue(thread, 0));\n    Arcadia_ValueStack_popValues(thread, 1);\n");
  if (enumerationEquality) text(w, "    }\n");
  text(w, "  }\n");
}

static Arcadia_String*
operandName
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  )
{
  if (!IS(w, n, VariableOperandNode)) fail(w, Arcadia_String_createFromCxxString(w->thread, "expected a declared name"));
  return ((Arcadia_PDLC_AST_VariableOperandNode*)n)->value;
}

static Arcadia_SizeValue
parameterSlot
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  )
{
  Arcadia_String* name = operandName(w, n);
  Arcadia_List* p = parameters(w->symbol);
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, p, i);
    if (Arcadia_String_isEqualTo_pn(w->thread, s->name, Arcadia_String_getBytes(w->thread, name), Arcadia_String_getNumberOfBytes(w->thread, name))) return slot(w, name, Arcadia_BooleanValue_False);
  }
  fail(w, Arcadia_String_createFromCxxString(w->thread, "argument name is not a parameter"));
  return 0;
}

static Arcadia_List*
entries
  (
    Writer* w
  )
{
  Arcadia_PDLC_Backend_Implementation* impl = Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context);
  Arcadia_List* infos = Arcadia_Map_getValues(w->thread, impl->symbolInfos);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, infos);
  Arcadia_List* result = (Arcadia_List*)Arcadia_ArrayList_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, result);
  for (Arcadia_SizeValue i = 0; i < size(w, infos); ++i) {
    Arcadia_List_insertBackObjectReferenceValue(w->thread, result, ((Arcadia_PDLC_Backend_SymbolInfo*)at(w, infos, i))->symbol);
  }
  return result;
}

static Arcadia_BooleanValue
enumerationOperand
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* n
  )
{
  Arcadia_String* name = ((Arcadia_PDLC_AST_VariableOperandNode*)n)->value;
  // Existing parameters/locals retain precedence. A fully qualified enum
  // member is a read-only value, never an implicitly declared stack slot.
  Arcadia_Value local = Arcadia_Map_get(w->thread, w->slots, Arcadia_Value_makeObjectReferenceValue(name));
  if (!Arcadia_Value_isVoidValue(&local)) return false;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(w->thread);
  Arcadia_List* symbols = entries(w);
  for (Arcadia_SizeValue i = 0; i < size(w, symbols); ++i) {
    Arcadia_PDLC_Symbol* symbol = at(w, symbols, i);
    if (symbol->kind != Arcadia_PDLC_SymbolKind_Enumeration) continue;
    Arcadia_List* members = ((Arcadia_PDLC_EnumerationSymbol*)symbol)->members;
    for (Arcadia_SizeValue j = 0; j < size(w, members); ++j) {
      Arcadia_PDLC_Symbol* member = at(w, members, j);
      if (!Arcadia_String_isEqualTo_pn(w->thread, name, Arcadia_String_getBytes(w->thread, member->name), Arcadia_String_getNumberOfBytes(w->thread, member->name))) continue;
      boxBegin(w, symbol);
      Arcadia_PDLC_Backend_Implementation* impl = Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context);
      Arcadia_String* constantName = Arcadia_PDLC_Backend_Implementation_computeCxxName(w->thread, impl, member->name);
      Arcadia_ValueStack_pushObjectReferenceValue(w->thread, constantName);
      string(w, constantName);
      boxEnd(w, symbol);
      Arcadia_ValueStack_popValues(w->thread, Arcadia_ValueStack_getSize(w->thread) - base);
      return true;
    }
  }
  Arcadia_ValueStack_popValues(w->thread, Arcadia_ValueStack_getSize(w->thread) - base);
  return false;
}

static Arcadia_BooleanValue
sameString
  (
    Writer* w,
    Arcadia_String* a,
    Arcadia_String* b
  )
{ return Arcadia_String_isEqualTo_pn(w->thread, a, Arcadia_String_getBytes(w->thread, b), Arcadia_String_getNumberOfBytes(w->thread, b)); }

static void
arguments
  (
    Writer* w,
    Arcadia_List* operands,
    Arcadia_SizeValue first
  )
{
  text(w, "  {\n    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);\n");
  for (Arcadia_SizeValue i = first; i < size(w, operands); ++i) {
    text(w, "    { Arcadia_Value v = "); operand(w, at(w, operands, i)); text(w, "; Arcadia_ValueStack_pushValue(thread, &v); }\n");
  }
  text(w, "    Arcadia_Value args["); number(w, size(w, operands) - first + 1); text(w, "];\n");
  for (Arcadia_SizeValue i = first; i < size(w, operands); ++i) {
    text(w, "    args["); number(w, i - first); text(w, "] = Arcadia_ValueStack_getValue(thread, "); number(w, size(w, operands) - i - 1); text(w, ");\n");
  }
}

static void
endArguments
  (
    Writer* w
  )
{ text(w, "    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - argumentsBase);\n  }\n"); }

static void
invoke
  (
    Writer* w,
    Arcadia_PDLC_AST_InvokeInstructionNode* n
  )
{
  Arcadia_BooleanValue qualified = memchr(Arcadia_String_getBytes(w->thread, n->callee->value), '.', Arcadia_String_getNumberOfBytes(w->thread, n->callee->value)) != NULL;
  Arcadia_PDLC_Symbol* p = (void*)Arcadia_Languages_Scope_lookup(w->thread, module(w->symbol)->scope, n->callee->value, Arcadia_BooleanValue_False);
  if (!p && qualified) p = (void*)Arcadia_Languages_Scope_lookup(w->thread, w->context->scope, n->callee->value, Arcadia_BooleanValue_False);
  if (p && p->kind == Arcadia_PDLC_SymbolKind_Procedure) {
    if (size(w, parameters(p)) != size(w, n->operands)) fail(w, Arcadia_String_createFromCxxString(w->thread, "procedure argument count mismatch"));
    arguments(w, n->operands, 0);
    if (!isVoid(w, returnType(p))) { put(w, 0); boxBegin(w, returnType(p)); }
    else text(w, "    ");
    string(w, cname(w, p)); text(w, "(thread"); callArguments(w, p); text(w, ")");
    if (!isVoid(w, returnType(p))) { boxEnd(w, returnType(p)); text(w, ");\n"); }
    else { text(w, ";\n"); put(w, 0); text(w, "Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));\n"); }
    endArguments(w);
  } else {
    if (n->procedureOnly || qualified) fail(w, Arcadia_String_createFromCxxString(w->thread, "undefined procedure"));
    if (!size(w, n->operands)) fail(w, Arcadia_String_createFromCxxString(w->thread, "method invocation requires a receiver"));
    prepare(w, at(w, n->operands, 0), 1); arguments(w, n->operands, 1);
    text(w, "    Arcadia_Value receiver = "); get(w, 1); text(w, ";\n");
    Arcadia_List* all = entries(w);
    Arcadia_SizeValue matches = 0;
    for (Arcadia_SizeValue i = 0; i < size(w, all); ++i) {
      Arcadia_PDLC_Symbol* c = at(w, all, i);
      if (c->kind != Arcadia_PDLC_SymbolKind_Class) continue;
      Arcadia_PDLC_MethodSymbol* method = (void*)member(w, (void*)c, n->callee->value);
      if (!method || ((Arcadia_PDLC_Symbol*)method)->kind != Arcadia_PDLC_SymbolKind_Method || method->overrideOf || ((Arcadia_PDLC_Symbol*)method)->enclosing != c) continue;
      if (size(w, method->parameters) != size(w, n->operands) - 1) continue;
      Arcadia_BooleanValue supplied = !module(c) || ((Arcadia_PDLC_ClassSymbol*)c)->isNative;
      if (supplied && !nativeTarget(w, (void*)method)) continue;
      if (supplied) addSymbol(w, w->nativeMethods, (void*)method);
      text(w, matches++ ? "    else if (" : "    if ("); text(w, "Arcadia_Value_isInstanceOf(thread, &receiver, _"); string(w, cname(w, c)); text(w, "_getType(thread))) {\n");
      if (!isVoid(w, returnType((void*)method))) { put(w, 0); boxBegin(w, returnType((void*)method)); }
      else text(w, "      ");
      string(w, cname(w, (void*)method));
      if (supplied) text(w, "_impl");
      text(w, "(thread, ("); typeName(w, c); text(w, ")Arcadia_Value_getObjectReferenceValue(&receiver)"); callArguments(w, (void*)method); text(w, ")");
      if (!isVoid(w, returnType((void*)method))) { boxEnd(w, returnType((void*)method)); text(w, ");\n"); }
      else { text(w, ";\n"); put(w, 0); text(w, "Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));\n"); }
      text(w, "    }\n");
    }
    if (!matches) fail(w, Arcadia_String_createFromCxxString(w->thread, "undefined method or method argument count mismatch"));
    text(w, "    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }\n"); endArguments(w);
  }
  assign(w, n->target);
}

static Arcadia_PDLC_ClassSymbol*
findClass
  (
    Writer* w,
    Arcadia_String* name
  )
{
  Arcadia_List* all = entries(w);
  for (Arcadia_SizeValue i = 0; i < size(w, all); ++i) {
    Arcadia_PDLC_Symbol* c = at(w, all, i);
    if (c->kind == Arcadia_PDLC_SymbolKind_Class && sameString(w, c->name, name)) return (void*)c;
  }
  fail(w, Arcadia_String_createFromCxxString(w->thread, "undefined construction class"));
  return NULL;
}

static void
construct
  (
    Writer* w,
    Arcadia_PDLC_AST_ConstructInstructionNode* n
  )
{
  Arcadia_PDLC_ClassSymbol* c = findClass(w, n->callee->value);
  if (!size(w, n->operands)) fail(w, Arcadia_String_createFromCxxString(w->thread, "construction requires an allocation or receiver operand"));
  Arcadia_PDLC_AST_OperandNode* receiverNode = at(w, n->operands, 0);
  Arcadia_BooleanValue receiverIsSelf = IS(w, receiverNode, VariableOperandNode) && equals(w, ((Arcadia_PDLC_AST_VariableOperandNode*)receiverNode)->value, "self");
  Arcadia_BooleanValue superCall = w->symbol->kind == Arcadia_PDLC_SymbolKind_Constructor && ((Arcadia_PDLC_ClassSymbol*)w->symbol->enclosing)->parentClassSymbol == c && size(w, body(w->symbol)) && at(w, body(w->symbol), 0) == n && receiverIsSelf;
  if (c->isNative && !superCall) fail(w, Arcadia_String_createFromCxxString(w->thread, "native class construction has no generated allocation entry point"));
  if (receiverIsSelf && !superCall) fail(w, Arcadia_String_createFromCxxString(w->thread, "superclass construction must be the first constructor statement and name the immediate parent"));
  if (c->isAbstract && !superCall) fail(w, Arcadia_String_createFromCxxString(w->thread, "cannot instantiate an abstract class"));
  Arcadia_PDLC_Symbol* ctor = c->scope ? (void*)Arcadia_Languages_Scope_lookup(w->thread, c->scope, Arcadia_String_createFromCxxString(w->thread, "<constructor>"), Arcadia_BooleanValue_False) : NULL;
  if (size(w, n->operands) - 1 != (ctor ? size(w, parameters(ctor)) : 0)) fail(w, Arcadia_String_createFromCxxString(w->thread, "constructor argument count mismatch"));
  prepare(w, at(w, n->operands, 0), 1); arguments(w, n->operands, 1);
  text(w, "    Arcadia_Value receiver = "); get(w, 1); text(w, ";\n");
  if (superCall) {
    text(w, "    pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&receiver) && Arcadia_Value_getObjectReferenceValue(&receiver) == (Arcadia_Object*)self);\n");
    for (Arcadia_SizeValue i = 1; i < size(w, n->operands); ++i) {
      Arcadia_PDLC_VariableSymbol* parameter = at(w, parameters(ctor), i - 1);
      if (parameter->type->kind == Arcadia_PDLC_SymbolKind_Enumeration) {
        checkType(w, parameter->type, argumentExpression(w, i - 1));
      }
      text(w, "    Arcadia_ValueStack_pushValue(thread, &args["); number(w, i - 1); text(w, "]);\n");
    }
    text(w, "    Arcadia_ValueStack_pushNatural8Value(thread, "); number(w, size(w, n->operands) - 1); text(w, ");\n    Arcadia_Type_getOperations(_"); string(w, cname(w, (void*)c)); text(w, "_getType(thread))->objectTypeOperations->construct(thread, (Arcadia_Object*)self);\n");
    text(w, "    Arcadia_Object_setType(thread, (Arcadia_Object*)self, _"); string(w, cname(w, w->symbol->enclosing)); text(w, "_getType(thread));\n");
    put(w, 0); text(w, "receiver);\n");
  } else {
    text(w, "    pdlRequire(thread, Arcadia_Value_isVoidValue(&receiver));\n");
    put(w, 0); text(w, "Arcadia_Value_makeObjectReferenceValue("); string(w, cname(w, (void*)c)); text(w, "_create(thread"); if (ctor) callArguments(w, ctor); text(w, ")));\n");
  }
  endArguments(w); assign(w, n->target);
}

static void
field
  (
    Writer* w,
    Arcadia_PDLC_AST_OperandNode* receiver,
    Arcadia_PDLC_AST_OperandNode* nameNode,
    Arcadia_PDLC_AST_OperandNode* value,
    Arcadia_BooleanValue store
  )
{
  Arcadia_String* name = operandName(w, nameNode);
  // self has the declaring class's type, and fields cannot shadow inherited
  // members. Unrelated classes with the same field name are not candidates.
  Arcadia_PDLC_Symbol* selfFieldOwner = NULL;
  if (w->symbol->kind != Arcadia_PDLC_SymbolKind_Procedure &&
      IS(w, receiver, VariableOperandNode) && equals(w, ((Arcadia_PDLC_AST_VariableOperandNode*)receiver)->value, "self")) {
    Arcadia_PDLC_Symbol* declared = member(w, (void*)w->symbol->enclosing, name);
    if (!declared || declared->kind != Arcadia_PDLC_SymbolKind_Variable) fail(w, Arcadia_String_createFromCxxString(w->thread, "undefined field"));
    selfFieldOwner = declared->enclosing;
  }
  prepare(w, receiver, 1); if (store) prepare(w, value, 2);
  text(w, "  { Arcadia_Value receiver = "); get(w, 1); text(w, ";\n");
  Arcadia_List* all = entries(w);
  Arcadia_SizeValue matches = 0;
  for (Arcadia_SizeValue i = 0; i < size(w, all); ++i) {
    Arcadia_PDLC_Symbol* c = at(w, all, i);
    if (c->kind != Arcadia_PDLC_SymbolKind_Class || ((Arcadia_PDLC_ClassSymbol*)c)->isNative) continue;
    if (selfFieldOwner && selfFieldOwner != c) continue;
    Arcadia_PDLC_VariableSymbol* f = (void*)member(w, (void*)c, name);
    if (!f || ((Arcadia_PDLC_Symbol*)f)->kind != Arcadia_PDLC_SymbolKind_Variable || ((Arcadia_PDLC_Symbol*)f)->enclosing != c) continue;
    text(w, matches++ ? "    else if (" : "    if ("); text(w, "Arcadia_Value_isInstanceOf(thread, &receiver, _"); string(w, cname(w, c)); text(w, "_getType(thread))) {\n");
    if (store) {
      text(w, "      (("); typeName(w, c); text(w, ")Arcadia_Value_getObjectReferenceValue(&receiver))->"); memberName(w, name); text(w, " = "); convert(w, f->type, Arcadia_String_createFromCxxString(w->thread, "pdlGet(thread, base, 2)")); text(w, ";\n");
    } else {
      put(w, 0); boxBegin(w, f->type);
      text(w, "(("); typeName(w, c); text(w, ")Arcadia_Value_getObjectReferenceValue(&receiver))->"); memberName(w, name);
      boxEnd(w, f->type); text(w, ");\n");
    }
    text(w, "    }\n");
  }
  if (!matches) fail(w, Arcadia_String_createFromCxxString(w->thread, "undefined field"));
  text(w, "    else { Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(thread); }\n  }\n");
  if (!store) assign(w, value);
}

static void
statement
  (
    Writer* w,
    Arcadia_PDLC_AST_Node* node
  )
{
  w->offset = ((Arcadia_PDLC_AST_StatementNode*)node)->startOffset;
  if (IS(w, node, VariableDefinitionStatementNode)) return;
  if (IS(w, node, LabelDefinitionStatementNode)) {
    text(w, "pdlLabel_"); string(w, ((Arcadia_PDLC_AST_LabelDefinitionStatementNode*)node)->labelName); text(w, ":;\n");
  } else if (IS(w, node, BinaryInstructionNode)) {
    Arcadia_PDLC_AST_BinaryInstructionNode* n = (void*)node;
    static const char* names[] = { NULL, "add", "and", "concatenate", "divide", "multiply", "or", "subtract", "isEqualTo", "isNotEqualTo", "isLowerThan", "isLowerThanOrEqualTo", "isGreaterThan", "isGreaterThanOrEqualTo" };
    prepare(w, n->operand1, 1); prepare(w, n->operand2, 2); operation(w, names[n->kind], 2); assign(w, n->target);
  } else if (IS(w, node, UnaryInstructionNode)) {
    Arcadia_PDLC_AST_UnaryInstructionNode* n = (void*)node;
    prepare(w, n->operand1, n->kind == Arcadia_PDLC_AST_UnaryInstructionKind_Set ? 0 : 1);
    if (n->kind != Arcadia_PDLC_AST_UnaryInstructionKind_Set) operation(w, n->kind == Arcadia_PDLC_AST_UnaryInstructionKind_Negate ? "negate" : "not", 1);
    assign(w, n->target);
  } else if (IS(w, node, JumpInstructionNode)) {
    Arcadia_PDLC_AST_JumpInstructionNode* n = (void*)node;
    Arcadia_Value found = Arcadia_Map_get(w->thread, w->labels, Arcadia_Value_makeObjectReferenceValue(n->labelName));
    if (Arcadia_Value_isVoidValue(&found)) fail(w, Arcadia_String_createFromCxxString(w->thread, "undefined jump label"));
    if (n->condition) {
      prepare(w, n->condition, 1); text(w, "  if (");
      if (n->kind == Arcadia_PDLC_AST_JumpInstructionKind_IfFalse) text(w, "!");
      text(w, "pdlAs"); text(w, useConversion(w, "Boolean")); text(w, "(thread, "); get(w, 1); text(w, ")) ");
    } else text(w, "  ");
    text(w, "goto pdlLabel_"); string(w, n->labelName); text(w, ";\n");
  } else if (IS(w, node, ReturnInstructionNode)) {
    Arcadia_PDLC_AST_ReturnInstructionNode* n = (void*)node;
    if (n->operand && isVoid(w, returnType(w->symbol))) fail(w, Arcadia_String_createFromCxxString(w->thread, "void callable cannot return a value"));
    if (!n->operand && !isVoid(w, returnType(w->symbol))) fail(w, Arcadia_String_createFromCxxString(w->thread, "non-void callable requires a return value"));
    if (n->operand) prepare(w, n->operand, 0);
    text(w, "  goto pdlReturn;\n");
  } else if (IS(w, node, RaiseInstructionNode)) text(w, "  Arcadia_Thread_raise(thread, Arcadia_Thread_getRaisedValue(thread));\n");
  else if (IS(w, node, LoadArgumentInstructionNode)) {
    Arcadia_PDLC_AST_LoadArgumentInstructionNode* n = (void*)node;
    put(w, 0); get(w, parameterSlot(w, n->source)); text(w, ");\n"); assign(w, n->target);
  } else if (IS(w, node, StoreArgumentInstructionNode)) {
    Arcadia_PDLC_AST_StoreArgumentInstructionNode* n = (void*)node;
    prepare(w, n->source, parameterSlot(w, n->name)); put(w, 0); get(w, parameterSlot(w, n->name)); text(w, ");\n"); assign(w, n->target);
  } else if (IS(w, node, LoadFieldInstructionNode)) {
    Arcadia_PDLC_AST_LoadFieldInstructionNode* n = (void*)node; field(w, n->source, n->name, n->target, Arcadia_BooleanValue_False);
  } else if (IS(w, node, StoreFieldInstructionNode)) {
    Arcadia_PDLC_AST_StoreFieldInstructionNode* n = (void*)node; field(w, n->target, n->name, n->source, Arcadia_BooleanValue_True);
  } else if (IS(w, node, InvokeInstructionNode)) invoke(w, (void*)node);
  else if (IS(w, node, ConstructInstructionNode)) construct(w, (void*)node);
  else fail(w, Arcadia_String_createFromCxxString(w->thread, "unsupported statement"));
}

static Arcadia_String*
nativeTarget
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s
  )
{ return Arcadia_PDLC_Symbols_getNativeTarget(w->thread, Arcadia_PDLC_Symbols_getInstance(w->thread, w->context), s); }

static void
typedParameters
  (
    Writer* w,
    Arcadia_List* p,
    Arcadia_PDLC_Symbol* receiver
  )
{
  text(w, "\n  (\n    Arcadia_Thread* thread");
  if (receiver) { text(w, ",\n    "); typeName(w, receiver); text(w, " self"); }
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    Arcadia_PDLC_VariableSymbol* parameter = at(w, p, i);
    text(w, ",\n    ");
    variableType(w, parameter);
    text(w, " ");
    if (w->header) {
      Arcadia_String* name = ((Arcadia_PDLC_Symbol*)parameter)->name;
      if (equals(w, name, "thread") || equals(w, name, "self")) { string(w, name); text(w, "_pdl"); }
      else memberName(w, name);
    } else { text(w, "p"); number(w, i); }
  }
  text(w, "\n  )");
}

static void
signatureWithReceiver
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s,
    const char* tail,
    Arcadia_PDLC_Symbol* receiver
  )
{
  typeName(w, returnType(s)); text(w, "\n"); string(w, cname(w, s)); text(w, tail);
  typedParameters(w, parameters(s), receiver);
}

static void
signature
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s,
    const char* tail
  )
{ signatureWithReceiver(w, s, tail, s->kind == Arcadia_PDLC_SymbolKind_Procedure ? NULL : s->enclosing); }

static Arcadia_PDLC_Symbol*
constructor
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind == Arcadia_PDLC_SymbolKind_Constructor) return s;
  }
  return NULL;
}

static void
factorySignature
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  Arcadia_PDLC_Symbol* ctor = constructor(w, c);
  typeName(w, (void*)c); text(w, "\n"); string(w, cname(w, (void*)c)); text(w, "_create");
  typedParameters(w, ctor ? parameters(ctor) : NULL, NULL);
}

static void
checkType
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type,
    Arcadia_String* expression
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, expression);
  if (isVoid(w, type)) {
    text(w, "  { Arcadia_Value v = "); string(w, expression); text(w, "; pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }\n");
  } else {
    text(w, "  (void)("); convert(w, type, expression); text(w, ");\n");
  }
  Arcadia_ValueStack_popValues(w->thread, 1);
}

static void
nativeSignature
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s,
    Arcadia_String* name
  )
{
  if (w->header) {
    typeName(w, returnType(s)); text(w, "\n"); string(w, name);
    typedParameters(w, parameters(s), s->kind == Arcadia_PDLC_SymbolKind_Procedure ? NULL : s->enclosing);
    return;
  }
  typeName(w, returnType(s)); text(w, " "); string(w, name); text(w, "(Arcadia_Thread* thread");
  if (s->kind != Arcadia_PDLC_SymbolKind_Procedure) { text(w, ", "); typeName(w, s->enclosing); text(w, " self"); }
  Arcadia_List* p = parameters(s);
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    text(w, ", "); variableType(w, at(w, p, i));
    text(w, " p"); number(w, i);
  }
  text(w, ")");
}

static void
nativeBody
  (
    Writer* w,
    Arcadia_String* name
  )
{
  const unsigned char* bytes = Arcadia_String_getBytes(w->thread, name);
  Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(w->thread, name);
  if (!length) fail(w, Arcadia_String_createFromCxxString(w->thread, "native target must be a C identifier"));
  Arcadia_PDLC_Symbol* owner = w->symbol->enclosing;
  Arcadia_BooleanValue nativeClass = owner && owner->kind == Arcadia_PDLC_SymbolKind_Class && ((Arcadia_PDLC_ClassSymbol*)owner)->isNative;
  Arcadia_String* entry = cname(w, w->symbol);
  Arcadia_SizeValue entryLength = Arcadia_String_getNumberOfBytes(w->thread, entry);
  Arcadia_BooleanValue adapterConflict = w->symbol->kind == Arcadia_PDLC_SymbolKind_Method && length == entryLength + sizeof("_impl") - 1 &&
    !memcmp(bytes, Arcadia_String_getBytes(w->thread, entry), entryLength) && !memcmp(bytes + entryLength, "_impl", sizeof("_impl") - 1);
  if ((!nativeClass && sameString(w, name, entry)) || adapterConflict) fail(w, Arcadia_String_createFromCxxString(w->thread, "native target conflicts with its generated entry point"));
  for (Arcadia_SizeValue i = 0; i < length; ++i) {
    unsigned char c = bytes[i];
    if (!(c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (i && c >= '0' && c <= '9'))) fail(w, Arcadia_String_createFromCxxString(w->thread, "native target must be a C identifier"));
  }
  text(w, "  extern "); nativeSignature(w, w->symbol, name); text(w, ";\n");
  if (!isVoid(w, returnType(w->symbol))) {
    put(w, 0); boxBegin(w, returnType(w->symbol));
  } else text(w, "  ");
  string(w, name); text(w, "(thread");
  if (w->symbol->kind != Arcadia_PDLC_SymbolKind_Procedure) {
    text(w, ", ("); typeName(w, w->symbol->enclosing); text(w, ")self");
  }
  Arcadia_List* p = parameters(w->symbol);
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    text(w, ", "); convert(w, ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type, pdlGetExpression(w, 4 + i));
  }
  if (isVoid(w, returnType(w->symbol))) text(w, ");\n");
  else { text(w, ")"); boxEnd(w, returnType(w->symbol)); text(w, ");\n"); }
}

static void
callableBody
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s
  )
{
  Arcadia_SizeValue temporaryBase = Arcadia_ValueStack_getSize(w->thread);
  w->symbol = s;
  w->offset = 0;
  w->slots = (Arcadia_Map*)Arcadia_HashMap_create(w->thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, w->slots);
  w->labels = (Arcadia_Map*)Arcadia_HashMap_create(w->thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, w->labels);
  w->slotCount = 4;
  Arcadia_List* p = parameters(s);
  if (size(w, p) > 255) fail(w, Arcadia_String_createFromCxxString(w->thread, "a callable may have at most 255 parameters"));
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    Arcadia_PDLC_VariableSymbol* parameter = at(w, p, i);
    if (parameter->isReference || parameter->recordFields) fail(w, Arcadia_String_createFromCxxString(w->thread, "external references and record values require a runtime value representation for executable code"));
  }
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) slot(w, ((Arcadia_PDLC_Symbol*)at(w, p, i))->name, Arcadia_BooleanValue_True);
  Arcadia_SizeValue selfSlot = 0;
  if (s->kind != Arcadia_PDLC_SymbolKind_Procedure) selfSlot = slot(w, Arcadia_String_createFromCxxString(w->thread, "self"), Arcadia_BooleanValue_True);
  Arcadia_List* b = body(s);
  Arcadia_String* native = nativeTarget(w, s);
  if (native && b) fail(w, Arcadia_String_createFromCxxString(w->thread, "native definitions must not have a PDL body"));
  // A missing ordinary body retains the existing empty-body interpretation.
  for (Arcadia_SizeValue i = 0; i < size(w, b); ++i) {
    Arcadia_PDLC_AST_Node* n = at(w, b, i);
    w->offset = ((Arcadia_PDLC_AST_StatementNode*)n)->startOffset;
    if (IS(w, n, VariableDefinitionStatementNode)) slot(w, ((Arcadia_PDLC_AST_VariableDefinitionStatementNode*)n)->variableName, Arcadia_BooleanValue_True);
    if (IS(w, n, LabelDefinitionStatementNode)) {
      Arcadia_Value key = Arcadia_Value_makeObjectReferenceValue(((Arcadia_PDLC_AST_LabelDefinitionStatementNode*)n)->labelName);
      Arcadia_Value old = Arcadia_Map_get(w->thread, w->labels, key);
      if (!Arcadia_Value_isVoidValue(&old)) fail(w, Arcadia_String_createFromCxxString(w->thread, "duplicate label"));
      Arcadia_Map_set(w->thread, w->labels, key, Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_True), NULL, NULL);
    }
  }
  // Analyze and render once into a temporary buffer to discover the registers actually used.
  Arcadia_StringBuilder* destination = w->output;
  w->output = Arcadia_StringBuilder_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, w->output);
  for (Arcadia_SizeValue i = 0; i < size(w, b); ++i) statement(w, at(w, b, i));
  Arcadia_String* instructions = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(w->output));
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, instructions);
  w->output = destination;
  if (s->kind != Arcadia_PDLC_SymbolKind_Procedure) text(w, "static ");
  signature(w, s, s->kind == Arcadia_PDLC_SymbolKind_Method ? "_impl" : "");
  text(w, "\n{\n  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);\n  Arcadia_JumpTarget jump;\n");
  text(w, "  Arcadia_Thread_pushJumpTarget(thread, &jump);\n  if (!Arcadia_JumpTarget_save(&jump)) {\n    Arcadia_Thread_popJumpTarget(thread);\n    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);\n    Arcadia_Thread_jump(thread);\n  }\n");
  text(w, "  for (Arcadia_SizeValue i = 0; i < "); number(w, w->slotCount); text(w, "; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);\n");
  if (selfSlot) { put(w, selfSlot); text(w, "pdlBoxObjectReference(self));\n"); }
  // Root all incoming objects/general values before resolving enum/type metadata.
  for (unsigned pass = 0; pass < 2; ++pass) {
    for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
      Arcadia_PDLC_Symbol* type = ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type;
      Arcadia_BooleanValue root = type->kind == Arcadia_PDLC_SymbolKind_Class || type->kind == Arcadia_PDLC_SymbolKind_Interface || equals(w, type->name, "Arcadia.Value");
      if (root != (pass == 0)) continue;
      put(w, 4 + i); parameterBoxBegin(w, at(w, p, i)); text(w, "p"); number(w, i); boxEnd(w, type); text(w, ");\n");
    }
  }
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) checkType(w, ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type, pdlGetExpression(w, 4 + i));
  if (native) { nativeBody(w, native); text(w, "  goto pdlReturn;\n"); }
  else string(w, instructions);
  if (!isVoid(w, returnType(s))) {
    text(w, "  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);\n");
  }
  text(w, "pdlReturn:;\n");
  if (isVoid(w, returnType(s))) { put(w, 0); text(w, "Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));\n"); }
  checkType(w, returnType(s), Arcadia_String_createFromCxxString(w->thread, "pdlGet(thread, base, 0)"));
  if (!isVoid(w, returnType(s))) {
    text(w, "  "); typeName(w, returnType(s)); text(w, " result = "); convert(w, returnType(s), Arcadia_String_createFromCxxString(w->thread, "pdlGet(thread, base, 0)")); text(w, ";\n");
  }
  text(w, "  Arcadia_Thread_popJumpTarget(thread);\n  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);\n  return");
  if (!isVoid(w, returnType(s))) text(w, " result");
  text(w, ";\n}\n\n");
  Arcadia_ValueStack_popValues(w->thread, Arcadia_ValueStack_getSize(w->thread) - temporaryBase);
}

static Arcadia_PDLC_MethodSymbol*
rootMethod
  (
    Arcadia_PDLC_MethodSymbol* m
  )
{
  while (m->overrideOf) m = m->overrideOf;
  return m;
}

static void
methodWrapper
  (
    Writer* w,
    Arcadia_PDLC_MethodSymbol* m
  )
{
  Arcadia_PDLC_MethodSymbol* root = rootMethod(m);
  Arcadia_PDLC_Symbol* s = (void*)m;
  signature(w, s, "");
  text(w, "\n{\n  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);\n  Arcadia_JumpTarget jump;\n  Arcadia_Thread_pushJumpTarget(thread, &jump);\n  if (!Arcadia_JumpTarget_save(&jump)) {\n    Arcadia_Thread_popJumpTarget(thread);\n    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);\n    Arcadia_Thread_jump(thread);\n  }\n  Arcadia_Value selfValue = pdlBoxObjectReference(self);\n  Arcadia_ValueStack_pushValue(thread, &selfValue);\n");
  for (Arcadia_SizeValue i = 0; i < size(w, m->parameters); ++i) {
    Arcadia_PDLC_Symbol* type = ((Arcadia_PDLC_VariableSymbol*)at(w, m->parameters, i))->type;
    if (type->kind != Arcadia_PDLC_SymbolKind_Class && type->kind != Arcadia_PDLC_SymbolKind_Interface && !equals(w, type->name, "Arcadia.Value")) continue;
    text(w, "  { Arcadia_Value value = "); parameterBoxBegin(w, at(w, m->parameters, i)); text(w, "p"); number(w, i); boxEnd(w, type); text(w, "; Arcadia_ValueStack_pushValue(thread, &value); }\n");
  }
  text(w, "  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _"); string(w, cname(w, s->enclosing));
  text(w, "_getType(thread)));\n");
  if (!m->isNonvirtual) {
    text(w, "  "); string(w, cname(w, ((Arcadia_PDLC_Symbol*)root)->enclosing)); text(w, "Dispatch* dispatch = ("); string(w, cname(w, ((Arcadia_PDLC_Symbol*)root)->enclosing));
    text(w, "Dispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));\n  if (!dispatch->"); memberName(w, ((Arcadia_PDLC_Symbol*)root)->name);
    text(w, ") { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }\n");
  }
  // A raw native override has no generated body to validate managed arguments.
  // All incoming objects are rooted above before descriptor lookup can collect.
  for (Arcadia_SizeValue i = 0; i < size(w, m->parameters); ++i) {
    Arcadia_PDLC_VariableSymbol* p = at(w, m->parameters, i);
    if (p->isReference || p->isOutput || (p->type->kind != Arcadia_PDLC_SymbolKind_Class && p->type->kind != Arcadia_PDLC_SymbolKind_Interface)) continue;
    text(w, "  pdlRequire(thread, !p"); number(w, i);
    text(w, " || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p"); number(w, i);
    text(w, ", _"); string(w, cname(w, p->type)); text(w, "_getType(thread)));\n");
  }
  text(w, "  ");
  if (!isVoid(w, returnType(s))) { typeName(w, returnType(s)); text(w, " result = "); }
  if (m->isNonvirtual) {
    string(w, cname(w, s)); text(w, "_impl(thread, self");
  } else {
    text(w, "dispatch->"); memberName(w, ((Arcadia_PDLC_Symbol*)root)->name);
    text(w, "(thread, ("); typeName(w, ((Arcadia_PDLC_Symbol*)root)->enclosing); text(w, ")self");
  }
  for (Arcadia_SizeValue i = 0; i < size(w, m->parameters); ++i) { text(w, ", p"); number(w, i); }
  text(w, ");\n  Arcadia_Thread_popJumpTarget(thread);\n  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);\n  return");
  if (!isVoid(w, returnType(s))) text(w, " result");
  text(w, ";\n}\n\n");
  if (m->overrideOf && !m->isAbstract) {
    text(w, "static "); signatureWithReceiver(w, s, "_dispatchImpl", ((Arcadia_PDLC_Symbol*)root)->enclosing);
    text(w, "\n{\n  "); if (!isVoid(w, returnType(s))) text(w, "return ");
    string(w, cname(w, s)); text(w, "_impl(thread, ("); typeName(w, s->enclosing); text(w, ")self");
    for (Arcadia_SizeValue i = 0; i < size(w, m->parameters); ++i) { text(w, ", p"); number(w, i); }
    text(w, ");\n}\n\n");
  }
}

static void
includeSymbol
  (
    Writer* w,
    Arcadia_PDLC_Symbol* s
  )
{
  Arcadia_PDLC_Backend_Implementation* impl = Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context);
  Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_PDLC_Backend_Implementation_getOrCreateInfo(w->thread, impl, s);
  Arcadia_PDLC_Backend_SymbolInfo* consumer = Arcadia_PDLC_Backend_Implementation_getOrCreateInfo(w->thread, impl, (void*)module(w->symbol));
  Arcadia_FilePath* path = Arcadia_PDLC_Backend_SymbolInfo_getDependencyHeaderFilePath(w->thread, info, consumer);
  if (path) {
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, path);
    Arcadia_String* name = Arcadia_PDLC_Backend_Implementation_pathToCxxPath(w->thread, impl, path);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, name);
    for (Arcadia_SizeValue i = 0; i < size(w, w->includedHeaders); ++i) {
      Arcadia_String* previous = at(w, w->includedHeaders, i);
      if (Arcadia_String_isEqualTo_pn(w->thread, name, Arcadia_String_getBytes(w->thread, previous), Arcadia_String_getNumberOfBytes(w->thread, previous))) return;
    }
    Arcadia_List_insertBackObjectReferenceValue(w->thread, w->includedHeaders, name);
    text(w, "#include \""); string(w, name); text(w, "\"\n");
  }
}

static void
headerType
  (
    Writer* w,
    Arcadia_PDLC_Symbol* type,
    Arcadia_List* includes,
    Arcadia_List* declarations
  )
{
  if (!type) return;
  if (type->kind == Arcadia_PDLC_SymbolKind_Enumeration) addSymbol(w, includes, type);
  if (type->kind == Arcadia_PDLC_SymbolKind_Class && module(type)) addSymbol(w, declarations, type);
}

static void
headerCallable
  (
    Writer* w,
    Arcadia_PDLC_Symbol* callable,
    Arcadia_List* includes,
    Arcadia_List* declarations
  )
{
  headerType(w, returnType(callable), includes, declarations);
  Arcadia_List* p = parameters(callable);
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) headerType(w, ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type, includes, declarations);
}

static void
classHeader
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  text(w, "Arcadia_declareObjectType(u8\""); string(w, ((Arcadia_PDLC_Symbol*)c)->name); text(w, "\", "); modelName(w, c); text(w, ",\n                          u8\""); string(w, ((Arcadia_PDLC_Symbol*)c->parentClassSymbol)->name); text(w, "\");\n\n");
  text(w, "struct "); modelName(w, c); text(w, "Dispatch {\n  "); string(w, cname(w, (void*)c->parentClassSymbol)); text(w, "Dispatch _parent;\n");
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind == Arcadia_PDLC_SymbolKind_Method && !((Arcadia_PDLC_MethodSymbol*)s)->overrideOf && !((Arcadia_PDLC_MethodSymbol*)s)->isNonvirtual) {
      text(w, "\n  "); typeName(w, returnType(s)); text(w, " (*"); memberName(w, s->name); text(w, ")(Arcadia_Thread*, "); typeName(w, (void*)c);
      Arcadia_List* p = parameters(s);
      for (Arcadia_SizeValue j = 0; j < size(w, p); ++j) {
        text(w, ", "); variableType(w, at(w, p, j));
      }
      text(w, ");\n");
    }
  }
  text(w, "};\n\nstruct "); modelName(w, c); text(w, " {\n  "); string(w, cname(w, (void*)c->parentClassSymbol)); text(w, " _parent;\n");
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind == Arcadia_PDLC_SymbolKind_Variable) {
      Arcadia_PDLC_VariableSymbol* field = (Arcadia_PDLC_VariableSymbol*)s;
      if (field->isConstant) continue;
      text(w, "  "); variableType(w, field); text(w, " "); memberName(w, s->name); text(w, ";\n");
    }
  }
  text(w, "};\n");
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind != Arcadia_PDLC_SymbolKind_Variable || !((Arcadia_PDLC_VariableSymbol*)s)->isConstant) continue;
    text(w, "\n#define "); modelName(w, c); text(w, "_"); memberName(w, s->name); text(w, " ");
    if (((Arcadia_PDLC_VariableSymbol*)s)->constantValue == Arcadia_Integer32Value_Minimum) {
      text(w, "(-INT32_C(2147483647) - INT32_C(1))\n");
      continue;
    }
    text(w, "INT32_C(");
    Arcadia_String* value = Arcadia_String_createFromInteger32(w->thread, ((Arcadia_PDLC_VariableSymbol*)s)->constantValue);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, value);
    string(w, value); text(w, ")\n");
    Arcadia_ValueStack_popValues(w->thread, 1);
  }
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind == Arcadia_PDLC_SymbolKind_Method) {
      text(w, "\n");
      Arcadia_String* target = c->isNative ? nativeTarget(w, s) : NULL;
      if (target) nativeSignature(w, s, target); else signature(w, s, "");
      text(w, ";\n");
    }
  }
  if (!c->isAbstract && !c->isNative) {
    text(w, "\n");
    factorySignature(w, c); text(w, ";\n");
  }
}

static void
classSource
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  Arcadia_PDLC_Symbol* ctor = NULL;
  Arcadia_BooleanValue explicitSuper = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind == Arcadia_PDLC_SymbolKind_Constructor) ctor = s;
    if (s->kind == Arcadia_PDLC_SymbolKind_Constructor || (s->kind == Arcadia_PDLC_SymbolKind_Method && !((Arcadia_PDLC_MethodSymbol*)s)->isAbstract)) callableBody(w, s);
    if (s->kind == Arcadia_PDLC_SymbolKind_Method) methodWrapper(w, (void*)s);
  }
  if (ctor && size(w, body(ctor))) {
    Arcadia_PDLC_AST_Node* first = at(w, body(ctor), 0);
    if (IS(w, first, ConstructInstructionNode)) {
      Arcadia_PDLC_AST_ConstructInstructionNode* n = (void*)first;
      Arcadia_PDLC_AST_OperandNode* receiver = size(w, n->operands) ? at(w, n->operands, 0) : NULL;
      explicitSuper = findClass(w, n->callee->value) == c->parentClassSymbol && IS(w, receiver, VariableOperandNode) && equals(w, ((Arcadia_PDLC_AST_VariableOperandNode*)receiver)->value, "self");
    }
  }
  w->symbol = (void*)c;
  text(w, "static void\nconstructImpl\n  (\n    Arcadia_Thread* thread,\n    "); typeName(w, (void*)c); text(w, " self\n  )\n{\n  Arcadia_EnterConstructor("); string(w, cname(w, (void*)c)); text(w, ");\n  if (_numberOfArguments != "); number(w, ctor ? size(w, parameters(ctor)) : 0);
  text(w, ") { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }\n");
  if (!explicitSuper) {
    Arcadia_PDLC_Symbol* parentCtor = member(w, c->parentClassSymbol, Arcadia_String_createFromCxxString(w->thread, "<constructor>"));
    if (parentCtor && size(w, parameters(parentCtor))) fail(w, Arcadia_String_createFromCxxString(w->thread, "constructor must start with an explicit superclass construction"));
    text(w, "  Arcadia_ValueStack_pushNatural8Value(thread, 0);\n  Arcadia_superTypeConstructor(thread, _type, self);\n");
  }
  // Publish the current layout before executing PDL field access; all fields start initialized.
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind != Arcadia_PDLC_SymbolKind_Variable) continue;
    text(w, "  self->"); memberName(w, s->name); text(w, " = ("); typeName(w, ((Arcadia_PDLC_VariableSymbol*)s)->type); text(w, "){0};\n");
  }
  if (!explicitSuper) text(w, "  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);\n");
  if (ctor) {
    text(w, "  { Arcadia_Value args["); number(w, size(w, parameters(ctor)) + 1); text(w, "];\n");
    for (Arcadia_SizeValue i = 0; i < size(w, parameters(ctor)); ++i) {
      text(w, "    args["); number(w, i); text(w, "] = Arcadia_ValueStack_getValue(thread, "); number(w, size(w, parameters(ctor)) - i); text(w, ");\n");
      Arcadia_PDLC_Symbol* type = ((Arcadia_PDLC_VariableSymbol*)at(w, parameters(ctor), i))->type;
      if (type->kind == Arcadia_PDLC_SymbolKind_Enumeration) {
        // Legacy C constructors pushed enum arguments as Integer32. Normalize
        // only this runtime callback boundary; PDL calls remain type checked.
        text(w, "    if (Arcadia_Value_isInteger32Value(&args["); number(w, i); text(w, "])) args["); number(w, i);
        text(w, "] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_"); string(w, cname(w, type));
        text(w, "_getType(thread), Arcadia_Value_getInteger32Value(&args["); number(w, i); text(w, "])));\n");
      }
    }
    text(w, "    "); string(w, cname(w, ctor)); text(w, "(thread, self"); callArguments(w, ctor); text(w, ");\n  }\n");
  }
  text(w, "  Arcadia_LeaveConstructor("); string(w, cname(w, (void*)c)); text(w, ");\n}\n\n");
  text(w, "static void\nvisitImpl\n  (\n    Arcadia_Thread* thread,\n    "); typeName(w, (void*)c); text(w, " self\n  )\n{\n");
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind == Arcadia_PDLC_SymbolKind_Variable && (((Arcadia_PDLC_VariableSymbol*)s)->type->kind == Arcadia_PDLC_SymbolKind_Class || ((Arcadia_PDLC_VariableSymbol*)s)->type->kind == Arcadia_PDLC_SymbolKind_Interface)) {
      text(w, "  if (self->"); memberName(w, s->name); text(w, ") Arcadia_Object_visit(thread, (Arcadia_Object*)self->"); memberName(w, s->name); text(w, ");\n");
    }
    if (s->kind == Arcadia_PDLC_SymbolKind_Variable && equals(w, ((Arcadia_PDLC_VariableSymbol*)s)->type->name, "Arcadia.Value")) {
      text(w, "  Arcadia_Value_visit(thread, &self->"); memberName(w, s->name); text(w, ");\n");
    }
  }
  text(w, "}\n\nstatic void\ninitializeDispatchImpl\n  (\n    Arcadia_Thread* thread,\n    "); string(w, cname(w, (void*)c)); text(w, "Dispatch* self\n  )\n{\n");
  for (Arcadia_SizeValue i = 0; i < size(w, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = at(w, c->members, i);
    if (s->kind != Arcadia_PDLC_SymbolKind_Method || ((Arcadia_PDLC_MethodSymbol*)s)->isNonvirtual) continue;
    Arcadia_PDLC_MethodSymbol* root = rootMethod((void*)s);
    text(w, "  (("); string(w, cname(w, ((Arcadia_PDLC_Symbol*)root)->enclosing)); text(w, "Dispatch*)self)->"); memberName(w, ((Arcadia_PDLC_Symbol*)root)->name);
    text(w, " = ");
    if (((Arcadia_PDLC_MethodSymbol*)s)->isAbstract) text(w, "NULL;\n");
    else { string(w, cname(w, s)); text(w, ((Arcadia_PDLC_MethodSymbol*)s)->overrideOf ? "_dispatchImpl;\n" : "_impl;\n"); }
  }
  text(w, "}\n\nstatic const Arcadia_ObjectType_Operations objectOperations = {\n  Arcadia_ObjectType_Operations_Initializer,\n  .construct = (Arcadia_Object_ConstructCallbackFunction*)constructImpl,\n  .visit = (Arcadia_Object_VisitCallbackFunction*)visitImpl,\n  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)initializeDispatchImpl,\n};\n\nstatic const Arcadia_Type_Operations typeOperations = {\n  Arcadia_Type_Operations_Initializer,\n  .objectTypeOperations = &objectOperations,\n};\n\n");
  text(w, "Arcadia_defineObjectType(u8\""); string(w, ((Arcadia_PDLC_Symbol*)c)->name); text(w, "\", "); string(w, cname(w, (void*)c));
  text(w, ",\n                         u8\""); string(w, ((Arcadia_PDLC_Symbol*)c->parentClassSymbol)->name); text(w, "\", "); string(w, cname(w, (void*)c->parentClassSymbol));
  text(w, ",\n                         &typeOperations);\n");
  if (c->isAbstract) return;
  text(w, "\n");
  factorySignature(w, c);
  text(w, "\n{\n  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);\n  Arcadia_JumpTarget jump;\n  Arcadia_Thread_pushJumpTarget(thread, &jump);\n  if (!Arcadia_JumpTarget_save(&jump)) {\n    Arcadia_Thread_popJumpTarget(thread);\n    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);\n    Arcadia_Thread_jump(thread);\n  }\n");
  Arcadia_List* p = ctor ? parameters(ctor) : NULL;
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    Arcadia_PDLC_Symbol* type = ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type;
    if (type->kind != Arcadia_PDLC_SymbolKind_Class && type->kind != Arcadia_PDLC_SymbolKind_Interface && !equals(w, type->name, "Arcadia.Value")) continue;
    text(w, "  { Arcadia_Value value = "); parameterBoxBegin(w, at(w, p, i)); text(w, "p"); number(w, i); boxEnd(w, type); text(w, "; Arcadia_ValueStack_pushValue(thread, &value); }\n");
  }
  text(w, "  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);\n");
  for (Arcadia_SizeValue i = 0; i < size(w, p); ++i) {
    Arcadia_PDLC_Symbol* type = ((Arcadia_PDLC_VariableSymbol*)at(w, p, i))->type;
    text(w, "  { Arcadia_Value value = "); parameterBoxBegin(w, at(w, p, i)); text(w, "p"); number(w, i); boxEnd(w, type); text(w, "; Arcadia_ValueStack_pushValue(thread, &value); }\n");
  }
  text(w, "  Arcadia_ValueStack_pushNatural8Value(thread, "); number(w, size(w, p)); text(w, ");\n  ");
  typeName(w, (void*)c); text(w, " result = _Arcadia_EndCreate0(thread, _"); string(w, cname(w, (void*)c)); text(w, "_getType(thread), argumentsBase);\n  Arcadia_Thread_popJumpTarget(thread);\n  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);\n  return result;\n}\n");
}

static void
sharedNativeEnums
  (
    Writer* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  if (!c->isNativeImplementation || !c->ast || !c->ast->nativeHeaderPath) return;
  Arcadia_List* infos = Arcadia_Map_getValues(w->thread, Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context)->symbolInfos);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, infos);
  Arcadia_List* enums = (Arcadia_List*)Arcadia_ArrayList_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, enums);
  for (Arcadia_SizeValue i = 0; i < size(w, infos); ++i) {
    Arcadia_PDLC_Symbol* s = ((Arcadia_PDLC_Backend_SymbolInfo*)at(w, infos, i))->symbol;
    if (s->kind != Arcadia_PDLC_SymbolKind_Enumeration || module(s) != module((Arcadia_PDLC_Symbol*)c)) continue;
    Arcadia_PDLC_EnumerationSymbol* e = (Arcadia_PDLC_EnumerationSymbol*)s;
    if (e->isNative && e->ast && e->ast->nativeHeaderPath && sameString(w, e->ast->nativeHeaderPath, c->ast->nativeHeaderPath)) addSymbol(w, enums, s);
  }
  for (Arcadia_SizeValue i = 0; i < size(w, enums); ++i) {
    Arcadia_PDLC_EnumerationSymbol* e = at(w, enums, i);
    text(w, "typedef enum "); string(w, cname(w, (Arcadia_PDLC_Symbol*)e)); text(w, " {\n");
    for (Arcadia_SizeValue j = 0; j < size(w, e->members); ++j) {
      Arcadia_PDLC_EnumerationConstantSymbol* constant = at(w, e->members, j);
      Arcadia_String* name = Arcadia_PDLC_Backend_Implementation_computeCxxName(w->thread,
        Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context), ((Arcadia_PDLC_Symbol*)constant)->name);
      Arcadia_ValueStack_pushObjectReferenceValue(w->thread, name);
      text(w, "  "); string(w, name); text(w, " = "); string(w, constant->ast->initializer->value); text(w, ",\n");
    }
    text(w, "} "); string(w, cname(w, (Arcadia_PDLC_Symbol*)e)); text(w, ";\n\n");
  }
}

static void
headerVariable
  (
    Writer* w,
    Arcadia_PDLC_VariableSymbol* v,
    Arcadia_List* includes,
    Arcadia_List* declarations
  )
{
  if (v->recordFields) {
    for (Arcadia_SizeValue i = 0; i < size(w, v->recordFields); ++i) headerVariable(w, at(w, v->recordFields, i), includes, declarations);
  } else headerType(w, v->type, includes, declarations);
}

void
Arcadia_PDLC_Backend_writeExecutable
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol,
    Arcadia_BooleanValue header,
    Arcadia_StringBuilder* output
  )
{
  Writer w = { .thread = thread, .context = context, .symbol = symbol, .output = output, .header = header };
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  text(&w,
    "// Arcadia\n"
    "// Copyright (C) 2024-2026 Michael Heilmann\n"
    "//\n"
    "// This program is free software: you can redistribute it and/or modify it under\n"
    "// the terms of the GNU Affero General Public License as published by the Free\n"
    "// Software Foundation, either version 3 of the License, or (at your option) any\n"
    "// later version.\n"
    "//\n"
    "// This program is distributed in the hope that it will be useful, but WITHOUT\n"
    "// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\n"
    "// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more\n"
    "// details.\n"
    "//\n"
    "// You should have received a copy of the GNU Affero General Public License\n"
    "// along with this program. If not, see <https://www.gnu.org/licenses/>.\n\n");
  Arcadia_PDLC_Backend_Implementation* implementation = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);
  Arcadia_PDLC_Backend_SymbolInfo* symbolInfo = Arcadia_PDLC_Backend_Implementation_getOrCreateInfo(thread, implementation, symbol);
  Arcadia_PDLC_Backend_SymbolInfo* moduleSymbolInfo = Arcadia_PDLC_Backend_SymbolInfo_getModule(thread, symbolInfo);
  Arcadia_String* moduleUpperCaseName = Arcadia_PDLC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, moduleUpperCaseName);
  Arcadia_String* guard = NULL;
  if (header) {
    guard = Arcadia_PDLC_Backend_Implementation_computeCxxNameUpperCase(thread, implementation, cname(&w, symbol));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, guard);
    if (symbol->kind == Arcadia_PDLC_SymbolKind_Class && ((Arcadia_PDLC_ClassSymbol*)symbol)->ast && ((Arcadia_PDLC_ClassSymbol*)symbol)->ast->nativeHeaderPath) {
      Arcadia_FilePath* path = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
      Arcadia_String* name = Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation, path);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
      Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, name);
      const unsigned char* bytes = Arcadia_String_getBytes(thread, name);
      Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, builder);
      if (length < 2 || bytes[length - 2] != '.' || bytes[length - 1] != 'h') fail(&w, Arcadia_String_createFromCxxString(thread, "native implementation declarations require an .h output path"));
      for (Arcadia_SizeValue i = 0; i < length - 2; ++i) {
        unsigned char ch = bytes[i];
        if (ch >= 'a' && ch <= 'z') ch -= 'a' - 'A';
        if (!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))) ch = '_';
        Arcadia_StringBuilder_insertBackCodePoint(thread, builder, ch);
      }
      guard = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(builder));
      Arcadia_ValueStack_pushObjectReferenceValue(thread, guard);
    }
    text(&w, "#if !defined("); string(&w, guard); text(&w, "_H_INCLUDED)\n#define "); string(&w, guard); text(&w, "_H_INCLUDED\n\n");
    Arcadia_FilePath* thisHeaderPath = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo);
    Arcadia_FilePath* moduleHeaderPath = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, moduleSymbolInfo);
    text(&w, "#if !defined("); string(&w, moduleUpperCaseName); text(&w, "_PRIVATE) || 1 != "); string(&w, moduleUpperCaseName); text(&w, "_PRIVATE\n");
    text(&w, "  #error(\"do not include `");
    string(&w, Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation, thisHeaderPath));
    text(&w, "` directly, include `");
    string(&w, Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation, moduleHeaderPath));
    text(&w, "` instead\")\n");
    text(&w, "#endif\n\n");
  } else {
    text(&w, "#define "); string(&w, moduleUpperCaseName); text(&w, "_PRIVATE (1)\n");
  }
  text(&w, "#include \"Arcadia/Ring1/Include.h\"\n");
  Arcadia_List* includes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, includes);
  w.includedHeaders = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, w.includedHeaders);
  if (header) {
    Arcadia_FilePath* path = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
    Arcadia_List_insertBackObjectReferenceValue(thread, w.includedHeaders, Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation, path));
  }
  Arcadia_List* declarations = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, declarations);
  if (header && symbol->kind == Arcadia_PDLC_SymbolKind_Class) {
    Arcadia_PDLC_ClassSymbol* c = (void*)symbol;
    addSymbol(&w, includes, (void*)c->parentClassSymbol);
    addSymbol(&w, declarations, symbol);
    for (Arcadia_SizeValue i = 0; i < size(&w, c->members); ++i) {
      Arcadia_PDLC_Symbol* member = at(&w, c->members, i);
      if (member->kind == Arcadia_PDLC_SymbolKind_Variable) headerVariable(&w, (Arcadia_PDLC_VariableSymbol*)member, includes, declarations);
      else if (member->kind == Arcadia_PDLC_SymbolKind_Method || (member->kind == Arcadia_PDLC_SymbolKind_Constructor && !c->isAbstract)) headerCallable(&w, member, includes, declarations);
    }
  }
  if (header && symbol->kind == Arcadia_PDLC_SymbolKind_Procedure) headerCallable(&w, symbol, includes, declarations);
  Arcadia_StringBuilder* content = NULL;
  Arcadia_StringBuilder* adapters = NULL;
  if (!header) {
    w.dependencies = includes;
    w.nativeMethods = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, w.nativeMethods);
    w.nativeEnumerationValues = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, w.nativeEnumerationValues);
    w.nativeEnumerationConversions = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, w.nativeEnumerationConversions);
    content = Arcadia_StringBuilder_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, content);
    adapters = Arcadia_StringBuilder_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, adapters);
    w.output = content;
    if (symbol->kind == Arcadia_PDLC_SymbolKind_Class) classSource(&w, (void*)symbol);
    else callableBody(&w, symbol);
    w.output = adapters;
    for (Arcadia_SizeValue i = 0; i < size(&w, w.nativeMethods); ++i) callableBody(&w, at(&w, w.nativeMethods, i));
    w.output = output;
    w.symbol = symbol;
    w.dependencies = NULL;
    includeSymbol(&w, symbol);
  }
  for (Arcadia_SizeValue i = 0; i < size(&w, includes); ++i) {
    Arcadia_PDLC_Symbol* s = at(&w, includes, i);
    if (header || s != symbol) includeSymbol(&w, s);
  }
  if (!header) {
    for (Arcadia_SizeValue i = 0; i < size(&w, w.nativeEnumerationValues); ++i) {
      Arcadia_PDLC_Symbol* s = at(&w, w.nativeEnumerationValues, i);
      text(&w, "_Static_assert(sizeof("); string(&w, cname(&w, s));
      text(&w, ") <= sizeof(Arcadia_Integer32Value), \"native enum value ABI must fit Integer32\");\n");
    }
  }
  text(&w, "\n");
  for (Arcadia_SizeValue i = 0; i < size(&w, declarations); ++i) {
    Arcadia_PDLC_Symbol* s = at(&w, declarations, i);
    Arcadia_BooleanValue included = Arcadia_BooleanValue_False;
    for (Arcadia_SizeValue j = 0; j < size(&w, includes); ++j) if (at(&w, includes, j) == s && s != symbol) included = Arcadia_BooleanValue_True;
    if (included) continue;
    text(&w, "typedef struct "); string(&w, cname(&w, s)); text(&w, " "); string(&w, cname(&w, s)); text(&w, ";\n");
  }
  if (size(&w, declarations)) text(&w, "\n");
  if (!header) {
    helpers(&w);
    for (Arcadia_SizeValue i = 0; i < size(&w, w.nativeEnumerationConversions); ++i) {
      Arcadia_PDLC_Symbol* s = at(&w, w.nativeEnumerationConversions, i);
      text(&w, "\nstatic "); string(&w, cname(&w, s)); text(&w, "\npdlAsNativeEnumeration_"); string(&w, cname(&w, s));
      text(&w, "\n  (\n    Arcadia_Thread* thread,\n    Arcadia_Value value\n  )\n{\n  Arcadia_Integer32Value numeric = pdlAsEnumeration(thread, value, _"); string(&w, cname(&w, s));
      text(&w, "_getType(thread));\n  "); string(&w, cname(&w, s)); text(&w, " result = ("); string(&w, cname(&w, s));
      text(&w, ")numeric;\n  pdlRequire(thread, (Arcadia_Integer64Value)result == (Arcadia_Integer64Value)numeric);\n  return result;\n}\n");
    }
    text(&w, "\n");
    Arcadia_String* adapterText = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(adapters));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, adapterText);
    string(&w, adapterText);
    Arcadia_String* contentText = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(content));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, contentText);
    string(&w, contentText);
  } else if (symbol->kind == Arcadia_PDLC_SymbolKind_Class) { sharedNativeEnums(&w, (Arcadia_PDLC_ClassSymbol*)symbol); classHeader(&w, (void*)symbol); }
  else { signature(&w, symbol, ""); text(&w, ";\n"); }
  if (header) { text(&w, "\n#endif // "); string(&w, guard); text(&w, "_H_INCLUDED\n"); }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  Arcadia_Thread_popJumpTarget(thread);
}

static void
validateVariable
  (
    Writer* w,
    Arcadia_PDLC_VariableSymbol* v,
    Arcadia_BooleanValue parameter,
    Arcadia_BooleanValue nativeOwner,
    Arcadia_BooleanValue nativeBinding
  )
{
  if (v->isOutput && (!parameter || !v->isReference || v->isReadOnly)) fail(w, Arcadia_String_createFromCxxString(w->thread, "output requires a writable external-reference parameter"));
  if (v->recordFields) {
    if (parameter || !nativeOwner || !size(w, v->recordFields) || v->isReference || v->isReadOnly || v->ownership || v->isConstant)
      fail(w, Arcadia_String_createFromCxxString(w->thread, "record fields currently require an unqualified native value field"));
    for (Arcadia_SizeValue i = 0; i < size(w, v->recordFields); ++i) validateVariable(w, at(w, v->recordFields, i), Arcadia_BooleanValue_False, nativeOwner, nativeBinding);
    return;
  }
  if (!v->type) fail(w, Arcadia_String_createFromCxxString(w->thread, "variable has no resolved type"));
  if (v->isReference && (v->type->kind != Arcadia_PDLC_SymbolKind_PrimitiveType || equals(w, v->type->name, "Arcadia.Value")))
    fail(w, Arcadia_String_createFromCxxString(w->thread, "external references currently require a scalar or Arcadia.Void referent"));
  if (v->isReadOnly && !v->isReference && v->type->kind != Arcadia_PDLC_SymbolKind_Class)
    fail(w, Arcadia_String_createFromCxxString(w->thread, "readonly requires an object view or external reference"));
  if (parameter) {
    if (v->ownership || v->isConstant) fail(w, Arcadia_String_createFromCxxString(w->thread, "parameter ownership is borrowed; field ownership modifiers are not parameter modifiers"));
    if ((v->isReference || v->isReadOnly) && !nativeBinding) fail(w, Arcadia_String_createFromCxxString(w->thread, "qualified parameters currently require a native binding"));
    return;
  }
  if (v->isConstant && (!equals(w, v->type->name, "Arcadia.Integer32") || v->isReference || v->isReadOnly || v->ownership))
    fail(w, Arcadia_String_createFromCxxString(w->thread, "class constants currently require unqualified Arcadia.Integer32 values"));
  if (!v->isReference && isVoid(w, v->type)) fail(w, Arcadia_String_createFromCxxString(w->thread, "a field cannot have type Arcadia.Void"));
  if (v->isReadOnly) fail(w, Arcadia_String_createFromCxxString(w->thread, "readonly currently qualifies native inputs, not mutable fields"));
  if (v->ownership == Arcadia_PDLC_Ownership_Owned && !v->isReference) fail(w, Arcadia_String_createFromCxxString(w->thread, "owned storage requires an external reference"));
  if (v->ownership == Arcadia_PDLC_Ownership_Retained && (v->isReference || v->type->kind != Arcadia_PDLC_SymbolKind_Class)) fail(w, Arcadia_String_createFromCxxString(w->thread, "retained requires an object type"));
  if (v->ownership == Arcadia_PDLC_Ownership_Retained) {
    Arcadia_PDLC_Symbol* ref = member(w, (Arcadia_PDLC_ClassSymbol*)v->type, Arcadia_String_createFromCxxString(w->thread, "ref"));
    Arcadia_PDLC_Symbol* unref = member(w, (Arcadia_PDLC_ClassSymbol*)v->type, Arcadia_String_createFromCxxString(w->thread, "unref"));
    if (!ref || !unref || ref->kind != Arcadia_PDLC_SymbolKind_Method || unref->kind != Arcadia_PDLC_SymbolKind_Method ||
        !((Arcadia_PDLC_MethodSymbol*)ref)->isNonvirtual || !((Arcadia_PDLC_MethodSymbol*)unref)->isNonvirtual ||
        size(w, parameters(ref)) || size(w, parameters(unref)) || !isVoid(w, returnType(ref)) || !isVoid(w, returnType(unref)))
      fail(w, Arcadia_String_createFromCxxString(w->thread, "retained types require nonvirtual ref/unref lifetime operations"));
  }
  if (v->ownership == Arcadia_PDLC_Ownership_Borrowed && !v->isReference && v->type->kind != Arcadia_PDLC_SymbolKind_Class && v->type->kind != Arcadia_PDLC_SymbolKind_Interface)
    fail(w, Arcadia_String_createFromCxxString(w->thread, "borrowed requires a reference type"));
  if (!nativeOwner && (v->ownership || v->isReference || v->isConstant)) fail(w, Arcadia_String_createFromCxxString(w->thread, "ownership-qualified storage and class constants currently require native implementations"));
}

void
Arcadia_PDLC_Backend_writeNativeClassModel
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* c,
    Arcadia_StringBuilder* output
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, c);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, output);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Writer w = { .thread = thread, .context = context, .symbol = (Arcadia_PDLC_Symbol*)c, .output = output, .header = Arcadia_BooleanValue_True };
  Arcadia_StringBuilder* name = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_StringBuilder_insertBackString(thread, name, cname(&w, (Arcadia_PDLC_Symbol*)c));
  Arcadia_StringBuilder_insertBackCxxString(thread, name, "_PDLModel");
  w.modelName = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(name));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, w.modelName);
  classHeader(&w, c);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

void
Arcadia_PDLC_Backend_writeVariableType
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_VariableSymbol* variable,
    Arcadia_StringBuilder* output
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, variable);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, output);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Writer w = { .thread = thread, .context = context, .symbol = (Arcadia_PDLC_Symbol*)variable, .output = output };
  variableType(&w, variable);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

void
Arcadia_PDLC_Backend_validateExecutables
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_PDLC_Backend_Implementation* impl = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);
  Arcadia_List* infos = Arcadia_Map_getValues(thread, impl->symbolInfos);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushObjectReferenceValue(thread, infos);
  Arcadia_StringBuilder* output = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, output);
  Arcadia_Map* headerOwners = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, headerOwners);
  for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)infos); ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
    Arcadia_PDLC_Symbol* s = info->symbol;
    if (!module(s)) continue;
    Arcadia_BooleanValue owned = s->kind == Arcadia_PDLC_SymbolKind_Class
      ? !((Arcadia_PDLC_ClassSymbol*)s)->isNative || ((Arcadia_PDLC_ClassSymbol*)s)->isNativeImplementation
      : s->kind == Arcadia_PDLC_SymbolKind_Enumeration ? !((Arcadia_PDLC_EnumerationSymbol*)s)->isNative
      : s->kind == Arcadia_PDLC_SymbolKind_Interface || s->kind == Arcadia_PDLC_SymbolKind_Procedure;
    if (!owned) continue;
    Arcadia_FilePath* path = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
    Arcadia_String* key = Arcadia_FilePath_toGeneric(thread, path);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, key);
    Writer w = { .thread = thread, .context = context, .symbol = s, .output = output };
    Arcadia_Value previous = Arcadia_Map_get(thread, headerOwners, Arcadia_Value_makeObjectReferenceValue(key));
    if (!Arcadia_Value_isVoidValue(&previous)) fail(&w, Arcadia_String_createFromCxxString(thread, "multiple declarations own the same generated header"));
    Arcadia_Map_set(thread, headerOwners, Arcadia_Value_makeObjectReferenceValue(key), Arcadia_Value_makeObjectReferenceValue(s), NULL, NULL);
    if (s->kind == Arcadia_PDLC_SymbolKind_Class || s->kind == Arcadia_PDLC_SymbolKind_Procedure || s->kind == Arcadia_PDLC_SymbolKind_Enumeration) {
      Arcadia_PDLC_Backend_SymbolInfo* mi = Arcadia_PDLC_Backend_SymbolInfo_getModule(thread, info);
      Arcadia_FilePath* aggregate = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, mi);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, aggregate);
      Arcadia_String* aggregateKey = Arcadia_FilePath_toGeneric(thread, aggregate);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, aggregateKey);
      if (sameString(&w, key, aggregateKey)) fail(&w, Arcadia_String_createFromCxxString(thread, "a generated declaration cannot replace its module aggregate header"));
    }
  }
  for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)infos); ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
    if (!module(info->symbol)) continue;
    if (info->symbol->kind == Arcadia_PDLC_SymbolKind_Class) {
      Arcadia_PDLC_ClassSymbol* c = (Arcadia_PDLC_ClassSymbol*)info->symbol;
      if (!c->isNative || c->isNativeImplementation) continue;
    } else if (info->symbol->kind == Arcadia_PDLC_SymbolKind_Enumeration) {
      if (!((Arcadia_PDLC_EnumerationSymbol*)info->symbol)->isNative) continue;
    } else continue;
    Arcadia_PDLC_Backend_ModuleSymbolInfo* mi = (Arcadia_PDLC_Backend_ModuleSymbolInfo*)Arcadia_PDLC_Backend_SymbolInfo_getModule(thread, info);
    Arcadia_FilePath* path = Arcadia_FilePath_clone(thread, mi->cxxFilesPath);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
    Arcadia_FilePath* relative = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, info);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, relative);
    Arcadia_FilePath_append(thread, path, relative);
    Arcadia_String* key = Arcadia_FilePath_toGeneric(thread, path);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, key);
    Arcadia_Value owner = Arcadia_Map_get(thread, headerOwners, Arcadia_Value_makeObjectReferenceValue(key));
    if (!Arcadia_Value_isVoidValue(&owner)) {
      Arcadia_PDLC_Symbol* declaration = (void*)Arcadia_Value_getObjectReferenceValue(&owner);
      // Native-implementation classes explicitly publish their shared enums.
      if (info->symbol->kind == Arcadia_PDLC_SymbolKind_Enumeration && declaration->kind == Arcadia_PDLC_SymbolKind_Class
          && ((Arcadia_PDLC_ClassSymbol*)declaration)->isNativeImplementation) continue;
      Writer w = { .thread = thread, .context = context, .symbol = info->symbol, .output = output };
      fail(&w, Arcadia_String_createFromCxxString(thread, "a generated header cannot replace an opaque native definition"));
    }
  }
  // All classes have now completed. Override discovery must not depend on map iteration order.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)infos); i < n; ++i) {
    Arcadia_PDLC_Symbol* s = ((Arcadia_PDLC_Backend_SymbolInfo*)Arcadia_List_getObjectReferenceValueAt(thread, infos, i))->symbol;
    if (s->kind == Arcadia_PDLC_SymbolKind_Interface) {
      Writer w = { .thread = thread, .context = context, .symbol = s, .output = output };
      Arcadia_List* operations = ((Arcadia_PDLC_InterfaceSymbol*)s)->operations;
      for (Arcadia_SizeValue j = 0; j < size(&w, operations); ++j) {
        Arcadia_PDLC_MethodSymbol* op = at(&w, operations, j);
        if (op->ast && op->ast->isNonvirtual) fail(&w, Arcadia_String_createFromCxxString(thread, "interface operations cannot be nonvirtual"));
        if (op->ast && op->ast->isAbstract && (op->ast->body || nativeTarget(&w, (Arcadia_PDLC_Symbol*)op))) fail(&w, Arcadia_String_createFromCxxString(thread, "an explicit abstract operation cannot have a body or native target"));
        for (Arcadia_SizeValue k = 0; k < size(&w, op->parameters); ++k) {
          Arcadia_PDLC_VariableSymbol* p = at(&w, op->parameters, k);
          if (p->isReference || p->isReadOnly || p->recordFields || p->ownership || p->isOutput) fail(&w, Arcadia_String_createFromCxxString(thread, "qualified interface parameters are not supported by the C backend"));
        }
      }
    }
    if (s->kind != Arcadia_PDLC_SymbolKind_Class || !module(s)) continue;
    Writer w = { .thread = thread, .context = context, .symbol = s, .output = output };
    Arcadia_PDLC_ClassSymbol* c = (void*)s;
    for (Arcadia_SizeValue j = 0; j < size(&w, c->members); ++j) {
      Arcadia_PDLC_Symbol* m = at(&w, c->members, j);
      if (m->kind == Arcadia_PDLC_SymbolKind_Method && ((Arcadia_PDLC_MethodSymbol*)m)->isAbstract) {
        Arcadia_PDLC_MethodSymbol* method = (Arcadia_PDLC_MethodSymbol*)m;
        if (!c->isAbstract) fail(&w, Arcadia_String_createFromCxxString(thread, "abstract methods require an abstract declaring class"));
        if (method->isNonvirtual || nativeTarget(&w, m) || (method->ast && method->ast->body)) fail(&w, Arcadia_String_createFromCxxString(thread, "abstract methods cannot be nonvirtual or have a body or native target"));
      }
      if (m->kind == Arcadia_PDLC_SymbolKind_Variable) validateVariable(&w, (Arcadia_PDLC_VariableSymbol*)m, Arcadia_BooleanValue_False, c->isNative, Arcadia_BooleanValue_False);
      if (m->kind == Arcadia_PDLC_SymbolKind_Method || m->kind == Arcadia_PDLC_SymbolKind_Constructor) {
        Arcadia_List* p = parameters(m);
        Arcadia_BooleanValue declaration = nativeTarget(&w, m) != NULL || (m->kind == Arcadia_PDLC_SymbolKind_Method && ((Arcadia_PDLC_MethodSymbol*)m)->isAbstract);
        for (Arcadia_SizeValue k = 0; k < size(&w, p); ++k) validateVariable(&w, at(&w, p, k), Arcadia_BooleanValue_True, c->isNative, declaration);
      }
      if (m->kind == Arcadia_PDLC_SymbolKind_Variable && member(&w, c->parentClassSymbol, m->name)) fail(&w, Arcadia_String_createFromCxxString(w.thread, "a field must not shadow an inherited member"));
      if (m->kind != Arcadia_PDLC_SymbolKind_Method) continue;
      Arcadia_PDLC_Symbol* parent = member(&w, c->parentClassSymbol, m->name);
      if (!parent) continue;
      if (((Arcadia_PDLC_MethodSymbol*)m)->isNonvirtual || (parent->kind == Arcadia_PDLC_SymbolKind_Method && ((Arcadia_PDLC_MethodSymbol*)parent)->isNonvirtual)) fail(&w, Arcadia_String_createFromCxxString(w.thread, "nonvirtual members cannot participate in overrides"));
      if (parent->kind != Arcadia_PDLC_SymbolKind_Method || returnType(m) != returnType(parent) || size(&w, parameters(m)) != size(&w, parameters(parent))) fail(&w, Arcadia_String_createFromCxxString(w.thread, "incompatible method override"));
      for (Arcadia_SizeValue k = 0; k < size(&w, parameters(m)); ++k) {
        Arcadia_PDLC_VariableSymbol* a = at(&w, parameters(m), k);
        Arcadia_PDLC_VariableSymbol* b = at(&w, parameters(parent), k);
        if (a->type != b->type || a->isReference != b->isReference || a->isReadOnly != b->isReadOnly || a->isOutput != b->isOutput || a->recordFields || b->recordFields) fail(&w, Arcadia_String_createFromCxxString(w.thread, "incompatible override parameter type"));
      }
      ((Arcadia_PDLC_MethodSymbol*)m)->overrideOf = (void*)parent;
    }
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)infos); i < n; ++i) {
    Arcadia_PDLC_Symbol* s = ((Arcadia_PDLC_Backend_SymbolInfo*)Arcadia_List_getObjectReferenceValueAt(thread, infos, i))->symbol;
    if (s->kind == Arcadia_PDLC_SymbolKind_Class && module(s)) {
      Arcadia_PDLC_ClassSymbol* c = (Arcadia_PDLC_ClassSymbol*)s;
      if (!c->isAbstract && !c->isNative) {
        Writer w = { .thread = thread, .context = context, .symbol = s, .output = output };
        for (Arcadia_PDLC_ClassSymbol* parent = c; parent; parent = parent->parentClassSymbol) {
          for (Arcadia_SizeValue j = 0; j < size(&w, parent->members); ++j) {
            Arcadia_PDLC_Symbol* candidate = at(&w, parent->members, j);
            if (candidate->kind != Arcadia_PDLC_SymbolKind_Method || !((Arcadia_PDLC_MethodSymbol*)candidate)->isAbstract) continue;
            Arcadia_PDLC_Symbol* implementation = member(&w, c, candidate->name);
            if (!implementation || implementation->kind != Arcadia_PDLC_SymbolKind_Method || ((Arcadia_PDLC_MethodSymbol*)implementation)->isAbstract) {
              Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
              Arcadia_ValueStack_pushObjectReferenceValue(thread, message);
              Arcadia_StringBuilder_insertBackCxxString(thread, message, "concrete class `");
              Arcadia_StringBuilder_insertBackString(thread, message, s->name);
              Arcadia_StringBuilder_insertBackCxxString(thread, message, "` does not implement abstract method `");
              Arcadia_StringBuilder_insertBackString(thread, message, candidate->name);
              Arcadia_StringBuilder_insertBackCxxString(thread, message, "`");
              fail(&w, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(message)));
            }
          }
        }
      }
    }
    if (s->kind == Arcadia_PDLC_SymbolKind_Procedure) {
      Writer w = { .thread = thread, .context = context, .symbol = s, .output = output };
      for (Arcadia_SizeValue k = 0; k < size(&w, parameters(s)); ++k) validateVariable(&w, at(&w, parameters(s), k), Arcadia_BooleanValue_True, Arcadia_BooleanValue_False, nativeTarget(&w, s) != NULL);
    }
    if ((s->kind != Arcadia_PDLC_SymbolKind_Class && s->kind != Arcadia_PDLC_SymbolKind_Procedure) || !module(s)) continue;
    if (s->kind == Arcadia_PDLC_SymbolKind_Class && ((Arcadia_PDLC_ClassSymbol*)s)->isNative) {
      if (((Arcadia_PDLC_ClassSymbol*)s)->isNativeImplementation) {
        Arcadia_StringBuilder_clear(thread, output);
        Arcadia_PDLC_Backend_writeExecutable(thread, context, s, Arcadia_BooleanValue_True, output);
      }
      continue;
    }
    Arcadia_StringBuilder_clear(thread, output);
    Arcadia_PDLC_Backend_writeExecutable(thread, context, s, Arcadia_BooleanValue_False, output);
    Arcadia_StringBuilder_clear(thread, output);
    Arcadia_PDLC_Backend_writeExecutable(thread, context, s, Arcadia_BooleanValue_True, output);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  Arcadia_Thread_popJumpTarget(thread);
}
