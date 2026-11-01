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
#include "Arcadia/PDLC/Backend/CallableWriter.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/SymbolInfo.h"
#include <string.h>

static const char* directory;

static void
checkAbstractFactories
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_List* infos = Arcadia_Map_getValues(thread, Arcadia_PDLC_Backend_Implementation_getInstance(thread, context)->symbolInfos);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, infos);
  Arcadia_StringBuilder* output = Arcadia_StringBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, output);
  Arcadia_BooleanValue found = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)infos); ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
    Arcadia_PDLC_Symbol* symbol = info->symbol;
    if (symbol->kind != Arcadia_PDLC_SymbolKind_Class || !((Arcadia_PDLC_ClassSymbol*)symbol)->isAbstract) continue;
    found = Arcadia_BooleanValue_True;
    for (unsigned header = 0; header < 2; ++header) {
      Arcadia_StringBuilder_clear(thread, output);
      Arcadia_PDLC_Backend_writeExecutable(thread, context, symbol, header != 0, output);
      Arcadia_String* rendered = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(output));
      Arcadia_ValueStack_pushObjectReferenceValue(thread, rendered);
      char const* bytes = Arcadia_String_getBytes(thread, rendered);
      Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, rendered);
      Arcadia_String* name = Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, info);
      Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, name);
      for (Arcadia_SizeValue j = 0; j + length + sizeof("_create") - 1 <= n; ++j) {
        Arcadia_Tests_assertTrue(thread, memcmp(bytes + j, Arcadia_String_getBytes(thread, name), length) || memcmp(bytes + j + length, "_create", sizeof("_create") - 1));
      }
      Arcadia_ValueStack_popValues(thread, 1);
    }
  }
  Arcadia_Tests_assertTrue(thread, found);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
checkNativeHeader
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_List* infos = Arcadia_Map_getValues(thread, Arcadia_PDLC_Backend_Implementation_getInstance(thread, context)->symbolInfos);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, infos);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_BooleanValue found = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue foundEnum = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue foundOwned = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue foundMappedEnum = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)infos); ++i) {
    Arcadia_PDLC_Backend_SymbolInfo* info = Arcadia_List_getObjectReferenceValueAt(thread, infos, i);
    Arcadia_PDLC_Symbol* symbol = info->symbol;
    if (Arcadia_String_isEqualTo_pn(thread, symbol->name, "Test.Color", sizeof("Test.Color") - 1)) {
      Arcadia_PDLC_EnumerationSymbol* e = (void*)symbol;
      Arcadia_Tests_assertTrue(thread, !e->isNative && e->ast && e->ast->nativeHeaderPath);
      Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, e->ast->nativeHeaderPath,
        "Test/Palette/Color.h", sizeof("Test/Palette/Color.h") - 1));
      Arcadia_FilePath* paths[] = {
        Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, info),
        Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFilePath(thread, info),
      };
      const char* expected[] = { "Test/Palette/Color.h", "Test/Palette/Color.c" };
      for (unsigned j = 0; j < 2; ++j) {
        Arcadia_String* path = Arcadia_FilePath_toGeneric(thread, paths[j]);
        Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, path, expected[j], strlen(expected[j])));
      }
      Arcadia_FilePath* header = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info);
      Arcadia_FilePath* source = Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info);
      Arcadia_Tests_assertTrue(thread, header && source);
      Arcadia_FileSystem* fs = Arcadia_FileSystem_getOrCreate(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_FileSystem_regularFileExists(thread, fs, header) && Arcadia_FileSystem_regularFileExists(thread, fs, source));
      foundMappedEnum = Arcadia_BooleanValue_True;
    }
    if (Arcadia_String_isEqualTo_pn(thread, symbol->name, "Test.External.Kind", sizeof("Test.External.Kind") - 1)) {
      Arcadia_PDLC_EnumerationSymbol* e = (Arcadia_PDLC_EnumerationSymbol*)symbol;
      Arcadia_Tests_assertTrue(thread, e->isNative && e->ast && e->ast->nativeHeaderPath);
      Arcadia_Tests_assertTrue(thread, !Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info));
      Arcadia_Tests_assertTrue(thread, !Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info));
      Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, info),
        "Test_External_Kind", sizeof("Test_External_Kind") - 1));
      Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, e->ast->nativeHeaderPath,
        "NativeResources/External.h", sizeof("NativeResources/External.h") - 1));
      Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)e->members) == 3);
      Arcadia_PDLC_EnumerationConstantSymbol* second = Arcadia_List_getObjectReferenceValueAt(thread, e->members, 2);
      Arcadia_Tests_assertTrue(thread, Arcadia_String_toNatural64(thread, second->ast->initializer->value) == 7);
      foundEnum = Arcadia_BooleanValue_True;
    }
    if (Arcadia_String_isEqualTo_pn(thread, symbol->name, "Test.NativeDeclared", sizeof("Test.NativeDeclared") - 1)) {
      Arcadia_PDLC_ClassSymbol* c = (Arcadia_PDLC_ClassSymbol*)symbol;
      Arcadia_Tests_assertTrue(thread, c->isNative && c->isNativeImplementation && c->ast->isNativeImplementation);
      Arcadia_Tests_assertTrue(thread, !Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, info));
      Arcadia_FilePath* path = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
      Arcadia_Tests_assertTrue(thread, path && Arcadia_FileSystem_regularFileExists(thread, Arcadia_FileSystem_getOrCreate(thread), path));
      Arcadia_ValueStack_popValues(thread, 1);
      foundOwned = Arcadia_BooleanValue_True;
    }
    if (!Arcadia_String_isEqualTo_pn(thread, symbol->name, "Test.External", sizeof("Test.External") - 1)) continue;
    Arcadia_PDLC_ClassSymbol* c = (Arcadia_PDLC_ClassSymbol*)symbol;
    Arcadia_Tests_assertTrue(thread, c->isNative && c->ast && c->ast->nativeHeaderPath);
    Arcadia_Tests_assertTrue(thread, !Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, info));
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, c->ast->nativeHeaderPath,
      "NativeResources/External.h", sizeof("NativeResources/External.h") - 1));
    Arcadia_FilePath* path = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, info);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, path);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, Arcadia_FilePath_toGeneric(thread, path),
      "NativeResources/External.h", sizeof("NativeResources/External.h") - 1));
    Arcadia_ValueStack_popValues(thread, 1);
    found = Arcadia_BooleanValue_True;
  }
  Arcadia_Tests_assertTrue(thread, found && foundEnum && foundOwned && foundMappedEnum);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
generate
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_PDLC_Context* context = Arcadia_PDLC_Context_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_PDLC_Symbols* registry = Arcadia_PDLC_Symbols_getInstance(thread, context);
  Arcadia_PDLC_ClassSymbol* object = (void*)Arcadia_PDLC_Symbols_lookupBuiltin(thread, registry, "Arcadia.Object");
  object->scope = Arcadia_Languages_Scope_create(thread, NULL);
  Arcadia_PDLC_MethodSymbol* ping = Arcadia_PDLC_MethodSymbol_create(thread, Arcadia_String_createFromCxxString(thread, "registryPing"));
  ((Arcadia_PDLC_Symbol*)ping)->enclosing = (void*)object;
  ping->returnValueType = Arcadia_PDLC_Symbols_lookupBuiltin(thread, registry, "Arcadia.Natural64");
  Arcadia_List_insertBackObjectReferenceValue(thread, object->members, ping);
  Arcadia_Languages_Scope_enter(thread, object->scope, ((Arcadia_PDLC_Symbol*)ping)->name, (void*)ping);
  Arcadia_PDLC_Symbols_setNativeTarget(thread, registry, (void*)ping, Arcadia_String_createFromCxxString(thread, "nativeRegistryPing"));
  Arcadia_List* paths = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, paths);
  Arcadia_List_insertBackObjectReferenceValue(thread, paths, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, ".")));
  Arcadia_PDLC_CompilationTask* task = Arcadia_PDLC_CompilationTask_create(thread, context);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, task);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_PDLC_CompilationTask_run(thread, task, Arcadia_FilePath_parseNative(thread, Arcadia_String_createFromCxxString(thread, directory)), paths);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_logf(Arcadia_LogFlags_Error, "generator status = %u\n", (unsigned)status);
    Arcadia_Languages_Diagnostics_emit(thread, context->diagnostics);
    Arcadia_Thread_setStatus(thread, status);
    Arcadia_Thread_jump(thread);
  }
  checkNativeHeader(thread, context);
  checkAbstractFactories(thread, context);
  Arcadia_ValueStack_popValues(thread, 3);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (argc != 2) return EXIT_FAILURE;
  directory = argv[1];
  return Arcadia_Tests_safeExecute(generate) ? EXIT_SUCCESS : EXIT_FAILURE;
}
