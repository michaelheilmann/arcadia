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

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/CallableWriter.h"
#include <string.h>

static const char* stage;
static const char* destination;

typedef struct CheckWriter {
  Arcadia_Thread* thread;
  Arcadia_PDLC_Context* context;
  Arcadia_StringBuilder* output;
  Arcadia_String* owner;
  Arcadia_String* model;
  Arcadia_SizeValue fields, nested, constants, methods, virtuals, references, readonly, outputs;
  Arcadia_SizeValue policies[4];
} CheckWriter;

static void
text
  (
    CheckWriter* w,
    const char* s
  )
{ Arcadia_StringBuilder_insertBackCxxString(w->thread, w->output, s); }

static void
string
  (
    CheckWriter* w,
    Arcadia_String* s
  )
{ Arcadia_StringBuilder_insertBackString(w->thread, w->output, s); }

static Arcadia_SizeValue
count
  (
    Arcadia_Thread* thread,
    Arcadia_List* list
  )
{ return list ? Arcadia_Collection_getSize(thread, (Arcadia_Collection*)list) : 0; }

static Arcadia_String*
join
  (
    CheckWriter* w,
    Arcadia_String* a,
    const char* separator,
    Arcadia_String* b
  )
{
  Arcadia_StringBuilder* out = Arcadia_StringBuilder_create(w->thread);
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, out);
  Arcadia_StringBuilder_insertBackString(w->thread, out, a);
  Arcadia_StringBuilder_insertBackCxxString(w->thread, out, separator);
  if (b) Arcadia_StringBuilder_insertBackString(w->thread, out, b);
  Arcadia_String* result = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(out));
  Arcadia_ValueStack_pushObjectReferenceValue(w->thread, result);
  return result;
}

static Arcadia_String*
cName
  (
    CheckWriter* w,
    Arcadia_PDLC_Symbol* s
  )
{
  if (s->kind == Arcadia_PDLC_SymbolKind_Method) return join(w, cName(w, s->enclosing), "_", s->name);
  if (s->kind == Arcadia_PDLC_SymbolKind_EnumerationConstant) {
    Arcadia_String* result = Arcadia_PDLC_Backend_Implementation_computeCxxName(w->thread,
      Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context), s->name);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, result);
    return result;
  }
  return Arcadia_PDLC_Backend_SymbolInfo_getCxxName(w->thread,
    Arcadia_PDLC_Backend_Implementation_getOrCreateInfo(w->thread, Arcadia_PDLC_Backend_Implementation_getInstance(w->thread, w->context), s));
}

static void
checkOwnership
  (
    CheckWriter* w,
    Arcadia_PDLC_VariableSymbol* v
  )
{
  static const struct { const char* owner; const char* field; Arcadia_Natural8Value policy; } expected[] = {
    { "Resource", "context", Arcadia_PDLC_Ownership_Borrowed },
    { "VertexBufferResource", "bytes", Arcadia_PDLC_Ownership_Owned },
    { "VertexBufferResource", "vertexDescriptor", Arcadia_PDLC_Ownership_Managed },
    { "ModelResource", "localToWorldMatrix", Arcadia_PDLC_Ownership_Managed },
    { "ModelResource", "constantBuffer", Arcadia_PDLC_Ownership_Retained },
    { "ModelResource", "meshConstantBuffer", Arcadia_PDLC_Ownership_Retained },
    { "ModelResource", "meshVertexBuffer", Arcadia_PDLC_Ownership_Retained },
    { "ModelResource", "material", Arcadia_PDLC_Ownership_Retained },
    { "EnterPassResource", "worldToViewMatrix", Arcadia_PDLC_Ownership_Managed },
    { "EnterPassResource", "viewToProjectionMatrix", Arcadia_PDLC_Ownership_Managed },
    { "EnterPassResource", "clearColor", Arcadia_PDLC_Ownership_Managed },
    { "EnterPassResource", "viewerConstantBuffer", Arcadia_PDLC_Ownership_Retained },
  };
  if (!v->ownership && (!v->type || v->type->kind != Arcadia_PDLC_SymbolKind_Class)) return;
  Arcadia_BooleanValue found = Arcadia_BooleanValue_False;
  for (size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i) {
    Arcadia_String* owner = join(w, Arcadia_String_createFromCxxString(w->thread, "Arcadia.Engine.Visuals"), ".",
      Arcadia_String_createFromCxxString(w->thread, expected[i].owner));
    Arcadia_PDLC_Symbol* s = (Arcadia_PDLC_Symbol*)v;
    if (Arcadia_String_isEqualTo_pn(w->thread, s->enclosing->name, Arcadia_String_getBytes(w->thread, owner), Arcadia_String_getNumberOfBytes(w->thread, owner)) &&
        Arcadia_String_isEqualTo_pn(w->thread, s->name, expected[i].field, strlen(expected[i].field))) {
      Arcadia_Tests_assertTrue(w->thread, v->ownership == expected[i].policy);
      found = Arcadia_BooleanValue_True;
    }
  }
  Arcadia_Tests_assertTrue(w->thread, found);
}

static void
checkFields
  (
    CheckWriter* w,
    Arcadia_List* fields,
    Arcadia_String* prefix
  )
{
  for (Arcadia_SizeValue i = 0; i < count(w->thread, fields); ++i) {
    Arcadia_PDLC_Symbol* s = Arcadia_List_getObjectReferenceValueAt(w->thread, fields, i);
    if (s->kind != Arcadia_PDLC_SymbolKind_Variable) continue;
    Arcadia_PDLC_VariableSymbol* v = (Arcadia_PDLC_VariableSymbol*)s;
    if (v->isConstant) {
      w->constants++;
      text(w, "_Static_assert("); string(w, w->owner); text(w, "_"); string(w, s->name);
      text(w, " == "); string(w, w->model); text(w, "_"); string(w, s->name); text(w, ", \"constant value\");\n");
      continue;
    }
    Arcadia_String* path = prefix ? join(w, prefix, ".", s->name) : s->name;
    if (prefix) w->nested++; else { w->fields++; w->policies[v->ownership]++; checkOwnership(w, v); }
    text(w, "_Static_assert(offsetof("); string(w, w->owner); text(w, ", "); string(w, path);
    text(w, ") == offsetof("); string(w, w->model); text(w, ", "); string(w, path); text(w, "), \"field offset\");\n");
    text(w, "_Static_assert(sizeof((("); string(w, w->owner); text(w, "*)0)->"); string(w, path);
    text(w, ") == sizeof((("); string(w, w->model); text(w, "*)0)->"); string(w, path); text(w, "), \"field size\");\n");
    if (v->recordFields) { checkFields(w, v->recordFields, path); continue; }
    // Use the backend's type renderer, then check exact pointer compatibility,
    // rather than relying on equally sized but semantically different types.
    Arcadia_StringBuilder* aliasBuilder = Arcadia_StringBuilder_create(w->thread);
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, aliasBuilder);
    Arcadia_StringBuilder_insertBackString(w->thread, aliasBuilder, w->owner);
    Arcadia_StringBuilder_insertBackCxxString(w->thread, aliasBuilder, "_PDLField");
    const unsigned char* bytes = Arcadia_String_getBytes(w->thread, path);
    for (Arcadia_SizeValue j = 0; j < Arcadia_String_getNumberOfBytes(w->thread, path); ++j)
      Arcadia_StringBuilder_insertBackCodePoint(w->thread, aliasBuilder, bytes[j] == '.' ? '_' : bytes[j]);
    Arcadia_String* alias = Arcadia_String_create(w->thread, Arcadia_Value_makeObjectReferenceValue(aliasBuilder));
    Arcadia_ValueStack_pushObjectReferenceValue(w->thread, alias);
    text(w, "typedef "); Arcadia_PDLC_Backend_writeVariableType(w->thread, w->context, v, w->output); text(w, " "); string(w, alias); text(w, ";\n");
    text(w, "_Static_assert(_Generic(&(("); string(w, w->owner); text(w, "*)0)->"); string(w, path);
    text(w, ", "); string(w, alias); text(w, "*: 1, default: 0), \"field type\");\n");
  }
}

static void
checkMethods
  (
    CheckWriter* w,
    Arcadia_PDLC_ClassSymbol* c
  )
{
  for (Arcadia_SizeValue i = 0; i < count(w->thread, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = Arcadia_List_getObjectReferenceValueAt(w->thread, c->members, i);
    if (s->kind != Arcadia_PDLC_SymbolKind_Method) continue;
    Arcadia_PDLC_MethodSymbol* m = (Arcadia_PDLC_MethodSymbol*)s;
    w->methods++;
    Arcadia_String* target = Arcadia_PDLC_Symbols_getNativeTarget(w->thread, Arcadia_PDLC_Symbols_getInstance(w->thread, w->context), s);
    if (!target) target = cName(w, s);
    Arcadia_String* alias = join(w, join(w, w->owner, "_", s->name), "_PDLFunction", NULL);
    text(w, "typedef ");
    if (Arcadia_String_isEqualTo_pn(w->thread, m->returnValueType->name, "Arcadia.Void", sizeof("Arcadia.Void") - 1)) text(w, "void");
    else {
      Arcadia_PDLC_VariableSymbol* result = Arcadia_PDLC_VariableSymbol_create(w->thread, s->name);
      Arcadia_ValueStack_pushObjectReferenceValue(w->thread, result);
      ((Arcadia_PDLC_Symbol*)result)->enclosing = s;
      result->type = m->returnValueType;
      Arcadia_PDLC_Backend_writeVariableType(w->thread, w->context, result, w->output);
    }
    text(w, " (*"); string(w, alias); text(w, ")(Arcadia_Thread*, "); string(w, w->owner); text(w, "*");
    for (Arcadia_SizeValue j = 0; j < count(w->thread, m->parameters); ++j) {
      Arcadia_PDLC_VariableSymbol* p = Arcadia_List_getObjectReferenceValueAt(w->thread, m->parameters, j);
      text(w, ", "); Arcadia_PDLC_Backend_writeVariableType(w->thread, w->context, p, w->output);
      if (p->isReference) w->references++;
      if (p->isReadOnly) w->readonly++;
      if (p->isOutput) w->outputs++;
    }
    text(w, ");\nstatic "); string(w, alias); text(w, " volatile "); string(w, target); text(w, "_PDLLink = &"); string(w, target); text(w, ";\n");
    if (!m->isNonvirtual) {
      w->virtuals++;
      text(w, "_Static_assert(offsetof("); string(w, w->owner); text(w, "Dispatch, "); string(w, s->name);
      text(w, ") == offsetof("); string(w, w->model); text(w, "Dispatch, "); string(w, s->name); text(w, "), \"dispatch offset\");\n");
      text(w, "_Static_assert(_Generic((("); string(w, w->owner); text(w, "Dispatch*)0)->"); string(w, s->name);
      text(w, ", "); string(w, alias); text(w, ": 1, default: 0), \"dispatch type\");\n");
    }
  }
  text(w, "static void "); string(w, w->owner); text(w, "_PDLCheck(Arcadia_Thread* thread) {\n");
  for (Arcadia_SizeValue i = 0; i < count(w->thread, c->members); ++i) {
    Arcadia_PDLC_Symbol* s = Arcadia_List_getObjectReferenceValueAt(w->thread, c->members, i);
    if (s->kind != Arcadia_PDLC_SymbolKind_Method) continue;
    Arcadia_String* target = Arcadia_PDLC_Symbols_getNativeTarget(w->thread, Arcadia_PDLC_Symbols_getInstance(w->thread, w->context), s);
    if (!target) target = cName(w, s);
    text(w, "  Arcadia_Tests_assertTrue(thread, "); string(w, target); text(w, "_PDLLink != 0);\n");
  }
  text(w, "  (void)thread;\n}\n");
}

static void
generate
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_PDLC_CompilationTask* task = Arcadia_PDLC_CompilationTask_create(thread, context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, task);
  Arcadia_FilePath* root = Arcadia_FilePath_parseNative(thread, Arcadia_String_createFromCxxString(thread, stage));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, root);
  Arcadia_List* modules = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, modules);
  const char* names[] = { "Engine", "Media", "Math", "Collections", "ADL" };
  for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); ++i)
    Arcadia_List_insertBackObjectReferenceValue(thread, modules, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, names[i])));
  Arcadia_PDLC_CompilationTask_run(thread, task, root, modules);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Languages_Diagnostics_hasErrors(thread, context->diagnostics));
  Arcadia_List* infos = Arcadia_Map_getValues(thread, Arcadia_PDLC_Backend_Implementation_getInstance(thread, context)->symbolInfos);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, infos);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_SizeValue nativeCollections = 0, nativeADL = 0, nativeDescriptors = 0, generatedFactories = 0, generatedADL = 0;
  for (Arcadia_SizeValue i = 0; i < count(thread, infos); ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
    Arcadia_PDLC_Symbol* symbol = info->symbol;
    if (symbol->kind != Arcadia_PDLC_SymbolKind_Class || !symbol->enclosing) continue;
    if (Arcadia_String_isEqualTo_pn(thread, symbol->name, "Arcadia.Media.VertexDescriptor", sizeof("Arcadia.Media.VertexDescriptor") - 1)) {
      Arcadia_PDLC_ClassSymbol* c = (Arcadia_PDLC_ClassSymbol*)symbol;
      Arcadia_Tests_assertTrue(thread, c->isNative && c->isNativeImplementation);
      Arcadia_Tests_assertTrue(thread, Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info)
        && !Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info));
      ++nativeDescriptors;
    }
    if (Arcadia_String_isEqualTo_pn(thread, symbol->name, "Arcadia.Engine.Audials.NodeFactory", sizeof("Arcadia.Engine.Audials.NodeFactory") - 1)
      || Arcadia_String_isEqualTo_pn(thread, symbol->name, "Arcadia.Engine.Visuals.NodeFactory", sizeof("Arcadia.Engine.Visuals.NodeFactory") - 1)) {
      Arcadia_Tests_assertTrue(thread, !((Arcadia_PDLC_ClassSymbol*)symbol)->isNative
        && Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info)
        && Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info));
      ++generatedFactories;
    }
    Arcadia_BooleanValue collections = Arcadia_String_isEqualTo_pn(thread, symbol->enclosing->name, "Arcadia.Collections", sizeof("Arcadia.Collections") - 1);
    Arcadia_BooleanValue adl = Arcadia_String_isEqualTo_pn(thread, symbol->enclosing->name, "Arcadia.ADL", sizeof("Arcadia.ADL") - 1);
    if (!collections && !adl) continue;
    if (adl && !((Arcadia_PDLC_ClassSymbol*)symbol)->isNative) {
      const char* names[] = { "Arcadia.ADL.ColorDefinition", "Arcadia.ADL.WhiteNoiseDefinition", "Arcadia.ADL.ConstantDefinition", "Arcadia.ADL.SineWaveDefinition", "Arcadia.ADL.SawtoothWaveDefinition" };
      Arcadia_BooleanValue known = false;
      for (Arcadia_SizeValue j = 0; j < sizeof(names) / sizeof(names[0]); ++j) {
        if (Arcadia_String_isEqualTo_pn(thread, symbol->name, names[j], strlen(names[j]))) known = true;
      }
      Arcadia_Tests_assertTrue(thread, known);
      Arcadia_Tests_assertTrue(thread, Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info)
        && Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info));
      ++generatedADL;
      continue;
    }
    Arcadia_Tests_assertTrue(thread, ((Arcadia_PDLC_ClassSymbol*)symbol)->isNative);
    Arcadia_Tests_assertTrue(thread, !Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info)
      && !Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info));
    if (collections) ++nativeCollections;
    else ++nativeADL;
  }
  Arcadia_Tests_assertTrue(thread, nativeCollections == 8 && nativeADL == 11 && generatedADL == 5);
  Arcadia_Tests_assertTrue(thread, nativeDescriptors == 1 && generatedFactories == 2);
  Arcadia_StringBuilder* output = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, output);
  Arcadia_StringBuilder* calls = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, calls);
  CheckWriter w = { .thread = thread, .context = context, .output = output };
  const char* resources[] = { "Resource", "ConstantBufferResource", "EnterPassResource", "FrameBufferResource", "MaterialResource", "ModelResource", "ProgramResource", "TextureResource", "VertexBufferResource" };
  Arcadia_SizeValue found = 0;
  for (size_t r = 0; r < sizeof(resources) / sizeof(resources[0]); ++r) {
    Arcadia_String* name = join(&w, Arcadia_String_createFromCxxString(thread, "Arcadia.Engine.Visuals"), ".", Arcadia_String_createFromCxxString(thread, resources[r]));
    for (Arcadia_SizeValue i = 0; i < count(thread, infos); ++i) {
      Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
      if (!Arcadia_String_isEqualTo_pn(thread, info->symbol->name, Arcadia_String_getBytes(thread, name), Arcadia_String_getNumberOfBytes(thread, name))) continue;
      Arcadia_PDLC_ClassSymbol* c = (Arcadia_PDLC_ClassSymbol*)info->symbol;
      found++;
      w.owner = cName(&w, info->symbol);
      w.model = join(&w, w.owner, "_PDLModel", NULL);
      Arcadia_PDLC_Backend_writeNativeClassModel(thread, context, c, output);
      const char* suffixes[] = { "", "Dispatch" };
      for (size_t k = 0; k < 2; ++k) {
        text(&w, "\n_Static_assert(sizeof("); string(&w, w.owner); text(&w, suffixes[k]); text(&w, ") == sizeof("); string(&w, w.model); text(&w, suffixes[k]); text(&w, "), \"layout size\");\n");
        text(&w, "_Static_assert(_Alignof("); string(&w, w.owner); text(&w, suffixes[k]); text(&w, ") == _Alignof("); string(&w, w.model); text(&w, suffixes[k]); text(&w, "), \"layout alignment\");\n");
      }
      checkFields(&w, c->members, NULL);
      checkMethods(&w, c);
      Arcadia_StringBuilder_insertBackCxxString(thread, calls, "  ");
      Arcadia_StringBuilder_insertBackString(thread, calls, w.owner);
      Arcadia_StringBuilder_insertBackCxxString(thread, calls, "_PDLCheck(thread);\n");
    }
  }
  Arcadia_Tests_assertTrue(thread, found == 9 && w.fields == 27 && w.nested == 6 && w.constants == 10 && w.methods == 41 && w.virtuals == 39);
  Arcadia_Tests_assertTrue(thread, w.references == 4 && w.readonly == 4 && w.outputs == 2);
  Arcadia_Tests_assertTrue(thread, w.policies[Arcadia_PDLC_Ownership_Borrowed] == 1 && w.policies[Arcadia_PDLC_Ownership_Owned] == 1 && w.policies[Arcadia_PDLC_Ownership_Retained] == 5);
  Arcadia_BooleanValue foundEnum = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < count(thread, infos); ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
    Arcadia_Tests_assertTrue(thread, !Arcadia_String_isEqualTo_pn(thread, info->symbol->name,
      "Arcadia.Engine.Visuals.MaterialResource.AmbientColorSource", sizeof("Arcadia.Engine.Visuals.MaterialResource.AmbientColorSource") - 1));
    if (!Arcadia_String_isEqualTo_pn(thread, info->symbol->name, "Arcadia.Engine.Visuals.ColorSource", sizeof("Arcadia.Engine.Visuals.ColorSource") - 1)) continue;
    Arcadia_PDLC_EnumerationSymbol* e = (Arcadia_PDLC_EnumerationSymbol*)info->symbol;
    Arcadia_Tests_assertTrue(thread, !e->isNative && count(thread, e->members) == 3 && Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info));
    Arcadia_String* actual = cName(&w, info->symbol);
    Arcadia_String* model = join(&w, actual, "_PDLModel", NULL);
    text(&w, "typedef enum "); string(&w, model); text(&w, " {\n");
    for (Arcadia_SizeValue j = 0; j < count(thread, e->members); ++j) {
      Arcadia_PDLC_EnumerationConstantSymbol* m = Arcadia_List_getObjectReferenceValueAt(thread, e->members, j);
      string(&w, cName(&w, (Arcadia_PDLC_Symbol*)m)); text(&w, "_PDLModel = "); string(&w, m->ast->initializer->value); text(&w, ",\n");
    }
    text(&w, "} "); string(&w, model); text(&w, ";\n_Static_assert(sizeof("); string(&w, model); text(&w, ") == sizeof("); string(&w, actual); text(&w, "), \"color source enum size\");\n_Static_assert(_Alignof("); string(&w, model); text(&w, ") == _Alignof("); string(&w, actual); text(&w, "), \"color source enum alignment\");\n");
    for (Arcadia_SizeValue j = 0; j < count(thread, e->members); ++j) {
      Arcadia_PDLC_EnumerationConstantSymbol* m = Arcadia_List_getObjectReferenceValueAt(thread, e->members, j);
      text(&w, "_Static_assert((int)"); string(&w, cName(&w, (Arcadia_PDLC_Symbol*)m)); text(&w, " == "); string(&w, m->ast->initializer->value); text(&w, ", \"color source enum value\");\n");
    }
    foundEnum = Arcadia_BooleanValue_True;
  }
  Arcadia_Tests_assertTrue(thread, foundEnum);
  text(&w, "\nstatic void testResourceModels(Arcadia_Thread* thread) {\n");
  string(&w, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(calls)));
  text(&w, "}\n");
  Arcadia_ByteArrayBuilder* bytes = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, bytes);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, bytes, Arcadia_StringBuilder_getBytes(thread, output), Arcadia_StringBuilder_getNumberOfBytes(thread, output));
  Arcadia_FilePath* path = Arcadia_FilePath_parseNative(thread, Arcadia_String_createFromCxxString(thread, destination));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), path, bytes);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (argc != 3) return EXIT_FAILURE;
  stage = argv[1];
  destination = argv[2];
  return Arcadia_Tests_safeExecute(generate) ? EXIT_SUCCESS : EXIT_FAILURE;
}
