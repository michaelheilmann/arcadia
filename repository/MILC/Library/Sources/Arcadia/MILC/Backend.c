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

#include "Arcadia/MILC/CompilationTask.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/AST/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  );

static void
Arcadia_MILC_CompilationTask_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTaskDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_CompilationTask_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.CompilationTask", Arcadia_MILC_CompilationTask,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_CompilationTask);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Context_getType(thread));
  self->log = self->context->diagnostics->log;
  Arcadia_LeaveConstructor(Arcadia_MILC_CompilationTask);
}

static void
Arcadia_MILC_CompilationTask_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTaskDispatch* self
  )
{/*Intentionally empty.*/}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
  if (self->log) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->log);
  }
}

Arcadia_MILC_CompilationTask*
Arcadia_MILC_CompilationTask_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_CompilationTask);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_CompilationTask);
}

#if defined(Verbose)
  #error("`Verbose` already defined")
#endif
#define Verbose (1)

// (1.1) For each module path, create a module node storing that path.
// (1.2) If a module path is relative, make it absolute wrt the specified working directory path.
static void
step1
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  )
{
#if defined(Arcadia_MILC_Configuration_ListModuleDirectories) && 1 == Arcadia_MILC_Configuration_ListModuleDirectories
  Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"module directories:\n"));
#endif
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->context->moduleNodes); i < n; ++i) {
    Arcadia_MILC_AST_ModuleNode* moduleNode = (Arcadia_MILC_AST_ModuleNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->context->moduleNodes, i);
    Arcadia_FilePath* moduleDirectoryPath = moduleNode->moduleDirectoryPath;
    if (Arcadia_FilePath_isRelative(thread, moduleDirectoryPath)) {
      Arcadia_FilePath* newModuleDirectoryPath = Arcadia_FilePath_clone(thread, self->context->workingDirectoryPath);
      Arcadia_FilePath_append(thread, newModuleDirectoryPath, moduleDirectoryPath);
      moduleNode->moduleDirectoryPath = newModuleDirectoryPath;
    }
  #if defined(Arcadia_MILC_Configuration_ListModuleDirectories) && 1 == Arcadia_MILC_Configuration_ListModuleDirectories
    Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"- "));
    Arcadia_Log_information(thread, self->log, Arcadia_FilePath_toGeneric(thread, moduleNode->moduleDirectoryPath));
    Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"\n"));
  #endif
  }
}

// (2.1) For each module node, parse its `Module.mil` file in a compilation unit node.
//       Add that compilation unit node to the list of compilation unit nodes of that module node.
static void
step2
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->context->moduleNodes); i < n; ++i) {
    Arcadia_MILC_AST_ModuleNode* moduleNode= (Arcadia_MILC_AST_ModuleNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->context->moduleNodes, i);
    Arcadia_FilePath* moduleDirectoryPath = moduleNode->moduleDirectoryPath;
    Arcadia_FilePath* moduleFilePath = Arcadia_FilePath_clone(thread, moduleDirectoryPath);
    Arcadia_FilePath_append(thread, moduleFilePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Module.mil")));
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    if (!Arcadia_FileSystem_regularFileExists(thread, fileSystem, moduleFilePath)) {
      if (Arcadia_FileSystem_directoryFileExists(thread, fileSystem, moduleFilePath)) {
        Arcadia_Languages_Diagnostics_add(thread, self->context->diagnostics, (Arcadia_Languages_Diagnostic*)Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, Arcadia_MILC_FileType_CompilationUnit, moduleFilePath));
      } else {
        Arcadia_Languages_Diagnostics_add(thread, self->context->diagnostics, (Arcadia_Languages_Diagnostic*)Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create(thread, Arcadia_Languages_DiagnosticType_Error, Arcadia_MILC_FileType_CompilationUnit, moduleFilePath));
      }
      Arcadia_Languages_Diagnostics_emit(thread, self->context->diagnostics);
    } else {
      Arcadia_ByteArrayBuilder* fileContents = Arcadia_FileSystem_getFileContents(thread, fileSystem, moduleFilePath);
      Arcadia_ByteArray* byteArray = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, fileContents), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, fileContents)));
      Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, byteArray));
      Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)self->context->parser, reader);
      Arcadia_MILC_AST_CompilationUnitNode* compilationUnitNode = (Arcadia_MILC_AST_CompilationUnitNode*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)self->context->parser), _Arcadia_MILC_AST_CompilationUnitNode_getType(thread));
      compilationUnitNode->filePath = moduleFilePath;
      Arcadia_MILC_AST_ModuleNode_appendCompilationUnit(thread, moduleNode, compilationUnitNode);
      compilationUnitNode->moduleNode = moduleNode;
    }
  }
}

/// Recursively search `Library/Sources` for `*.mil` files.
/// @param moduleNode The module node.
/// @param list The list to add the files to.
static inline void
enumerateFiles
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self,
    Arcadia_MILC_AST_ModuleNode* moduleNode,
    Arcadia_List* files
  )
{
  // Stack to add discovered directories to.
  Arcadia_Stack* stack = (Arcadia_Stack*)Arcadia_ArrayStack_create(thread);

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);

  // Compute the absolute path to `Library/Sources`.
  Arcadia_FilePath* sourceDirectoryPath = Arcadia_FilePath_clone(thread, moduleNode->moduleDirectoryPath);
  Arcadia_FilePath_append(thread, sourceDirectoryPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Library/Sources")));
  // Compute the `..` and `.` paths.
  Arcadia_FilePath* dot = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8".")),
                  * dotdot = Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8".."));

  Arcadia_Stack_pushObjectReferenceValue(thread, stack, sourceDirectoryPath);

  while (!Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)stack)) {
    Arcadia_FilePath* directoryFilePath = (Arcadia_FilePath*)Arcadia_Stack_getObjectReferenceValue(thread, stack, 0);
    Arcadia_Stack_pop(thread, stack);

    Arcadia_DirectoryIterator* directoryIterator = Arcadia_FileSystem_createDirectoryIterator(thread, fileSystem, directoryFilePath);
    while (Arcadia_DirectoryIterator_hasValue(thread, directoryIterator)) {
      Arcadia_FilePath* elementPath = Arcadia_DirectoryIterator_getValue(thread, directoryIterator);
      if (Arcadia_FilePath_isEqualTo(thread, elementPath, dot) || Arcadia_FilePath_isEqualTo(thread, elementPath, dotdot)) {
        // Skip those.
      } else {
        Arcadia_FilePath* nextElementPath = Arcadia_FilePath_clone(thread, directoryFilePath);
        Arcadia_FilePath_append(thread, nextElementPath, elementPath);
        elementPath = nextElementPath;
        if (Arcadia_FileSystem_directoryFileExists(thread, fileSystem, elementPath)) {
          Arcadia_Stack_pushObjectReferenceValue(thread, stack, (Arcadia_Object*)elementPath);
        } else if (Arcadia_FileSystem_regularFileExists(thread, fileSystem, elementPath)) {
        #if defined(Verbose) && 0
          Arcadia_Log_info(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"discovered regular file `"));
          Arcadia_Log_info(thread, self->log, Arcadia_FilePath_toGeneric(thread, elementPath));
          Arcadia_Log_info(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"`\n"));
        #endif
          Arcadia_List_insertBackObjectReferenceValue(thread, files, (Arcadia_Object*)elementPath);
        }
      }
      Arcadia_DirectoryIterator_nextValue(thread, directoryIterator);
    }
  }
}

// (3) For each module node:
// (3.1) search its `Library/Sources` directory and its child directories `*.mil` files.
// (3.2) For each discovered `*.mil` file, parse that file into a compilation unit node.
// (3.3) Add that compilation unit node to the list of compilation unit nodes of that module node.
static void
step3
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_Value milExtension = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8".mil"));
  Arcadia_List* files = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->context->moduleNodes); i < n; ++i) {
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)files);
    Arcadia_MILC_AST_ModuleNode* moduleNode = (Arcadia_MILC_AST_ModuleNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->context->moduleNodes, i);
    enumerateFiles(thread, self, moduleNode, files);
  #if defined(Arcadia_MILC_Configuration_ListCompilationUnits) && 1 == Arcadia_MILC_Configuration_ListCompilationUnits
    Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"module `"));
    Arcadia_Log_information(thread, self->log, Arcadia_FilePath_toGeneric(thread, moduleNode->moduleDirectoryPath));
    Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"` files:\n"));
  #endif
    for (Arcadia_SizeValue j = 0, m = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)files); j < m; ++j) {
      Arcadia_FilePath* filePath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueAt(thread, files, j);
      Arcadia_String* extension = Arcadia_FilePath_getExtension(thread, filePath);
      if (extension && Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)extension, &milExtension)) {
      #if defined(Arcadia_MILC_Configuration_ListCompilationUnits) && 1 == Arcadia_MILC_Configuration_ListCompilationUnits
        Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8" - `"));
        Arcadia_Log_information(thread, self->log, Arcadia_FilePath_toGeneric(thread, filePath));
        Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"`\n"));
      #endif
        Arcadia_ByteArrayBuilder* x = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
        Arcadia_ByteArray* y = Arcadia_ByteArray_createByteArray(thread, Arcadia_RuntimeByteArray_create(thread, Arcadia_ByteArrayBuilder_getBytes(thread, x), Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, x)));
        Arcadia_UnicodeCodePointReader* z = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, y));
        Arcadia_Languages_Parser_setInput(thread, (Arcadia_Languages_Parser*)self->context->parser, z);
        Arcadia_MILC_AST_CompilationUnitNode* compilationUnitNode = (Arcadia_MILC_AST_CompilationUnitNode*)Arcadia_Value_getObjectReferenceValueChecked(thread, Arcadia_Languages_Parser_run(thread, (Arcadia_Languages_Parser*)self->context->parser), _Arcadia_MILC_AST_CompilationUnitNode_getType(thread));
        compilationUnitNode->filePath = filePath;
        compilationUnitNode->moduleNode = moduleNode;
#if 0
        Arcadia_FilePath* temporary = Arcadia_FilePath_clone(thread, filePath);
        Arcadia_FilePath_addOrReplaceExtension(thread, temporary, Arcadia_String_createFromCxxString(thread, u8"h"));
      #if defined(Verbose)
        Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"  -> `"));
        Arcadia_Log_information(thread, self->log, Arcadia_FilePath_toGeneric(thread, temporary));
        Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"`\n"));
      #endif
        Arcadia_FilePath_addOrReplaceExtension(thread, temporary, Arcadia_String_createFromCxxString(thread, u8"c"));
      #if defined(Verbose)
        Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"  -> `"));
        Arcadia_Log_information(thread, self->log, Arcadia_FilePath_toGeneric(thread, temporary));
        Arcadia_Log_information(thread, self->log, Arcadia_String_createFromCxxString(thread, u8"`\n"));
      #endif
#endif
        Arcadia_MILC_AST_ModuleNode_appendCompilationUnit(thread, moduleNode, compilationUnitNode);
      }
    } 
  }
}

static Arcadia_String*
nameToString
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  );

static Arcadia_FilePath*
makeCxxSourceFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  );

static Arcadia_FilePath*
makeCxxHeaderFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  );

static Arcadia_String*
symbolKindToString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_SymbolKind kind
  );

static void
dumpClassSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_ClassSymbol* classSymbol
  );

static void
dumpEnumerationSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_EnumerationSymbol* enumerationSymbol
  );

static void
dumpModuleSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_ModuleSymbol* moduleSymbol
  );

static void
dumpProcedureSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_ProcedureSymbol* procedureSymbol
  );

static void
dumpSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_Symbol* symbol
  );

static Arcadia_String*
nameToString
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_StringBuilder* sb = Arcadia_StringBuilder_create(thread);
  Arcadia_UnicodeCodePointReader* ucpr = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, string));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, ucpr)) {
    Arcadia_Natural32Value uc = Arcadia_UnicodeCodePointReader_getValue(thread, ucpr);
    if (uc == '.') {
      uc = '/';
    }
    Arcadia_StringBuilder_insertBackCodePoint(thread, sb, uc);
    Arcadia_UnicodeCodePointReader_nextValue(thread, ucpr);
  }
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(sb));
}

static Arcadia_FilePath*
makeCxxSourceFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  )
{
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_FilePath* cxxFilePath = Arcadia_FilePath_clone(thread, cxxFilesPath);
  Arcadia_FilePath_append(thread, cxxFilePath, Arcadia_FilePath_parseGeneric(thread, nameToString(thread, ((Arcadia_MILC_Symbol*)symbol)->name)));
  Arcadia_String* fileName = Arcadia_List_getObjectReferenceValueAt(thread, cxxFilePath->fileNames, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)cxxFilePath->fileNames) - 1);
  Arcadia_List_removeBack(thread, cxxFilePath->fileNames, 1);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, fileName);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".c");
  Arcadia_List_insertBackObjectReferenceValue(thread, cxxFilePath->fileNames, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
  return cxxFilePath;
}

static Arcadia_FilePath*
makeCxxHeaderFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  )
{
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_FilePath* cxxFilePath = Arcadia_FilePath_clone(thread, cxxFilesPath);
  Arcadia_FilePath_append(thread, cxxFilePath, Arcadia_FilePath_parseGeneric(thread, nameToString(thread, ((Arcadia_MILC_Symbol*)symbol)->name)));
  Arcadia_String* fileName = Arcadia_List_getObjectReferenceValueAt(thread, cxxFilePath->fileNames, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)cxxFilePath->fileNames) - 1);
  Arcadia_List_removeBack(thread, cxxFilePath->fileNames, 1);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, fileName);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".h");
  Arcadia_List_insertBackObjectReferenceValue(thread, cxxFilePath->fileNames, (Arcadia_Object*)Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
  return cxxFilePath;
}

static Arcadia_String*
symbolKindToString
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_SymbolKind kind
  )
{
  switch (kind) {
    case Arcadia_MILC_SymbolKind_Class: {
      return Arcadia_String_createFromCxxString(thread, u8"class");
    } break;
    case Arcadia_MILC_SymbolKind_Constructor: {
      return Arcadia_String_createFromCxxString(thread, u8"constructor");
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      return Arcadia_String_createFromCxxString(thread, u8"enumeration");
    } break;
    case Arcadia_MILC_SymbolKind_Field: {
      return Arcadia_String_createFromCxxString(thread, u8"field");
    } break;
    case Arcadia_MILC_SymbolKind_Method: {
      return Arcadia_String_createFromCxxString(thread, u8"method");
    } break;
    case Arcadia_MILC_SymbolKind_Module: {
      return Arcadia_String_createFromCxxString(thread, u8"module");
    } break;
    case Arcadia_MILC_SymbolKind_Procedure: {
      return Arcadia_String_createFromCxxString(thread, u8"procedure");
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

static void
dumpClassSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_ClassSymbol* classSymbol
  )
{
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"class ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, ((Arcadia_MILC_Symbol*)classSymbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
}

static void
dumpEnumerationSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_EnumerationSymbol* symbol
  )
{
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"enumeration ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, ((Arcadia_MILC_Symbol*)symbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");

  Arcadia_MILC_Symbol* enclosingSymbol = ((Arcadia_MILC_Symbol*)symbol)->enclosing;
  if (Arcadia_MILC_SymbolKind_Module == enclosingSymbol->kind) {
    Arcadia_MILC_ModuleSymbol* enclosingModuleSymbol = (Arcadia_MILC_ModuleSymbol*)enclosingSymbol;
    Arcadia_MILC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_MILC_AST_DefinitionNode*)enclosingModuleSymbol->moduleDefinitionNode)->compilationUnitNode;
    Arcadia_MILC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;

    Arcadia_FilePath* modulePath = moduleNode->moduleDirectoryPath;
    Arcadia_FilePath* cxxFilesPath = Arcadia_FilePath_clone(thread, modulePath);
    Arcadia_FilePath_append(thread, cxxFilesPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Library/Sources")));
    for (Arcadia_SizeValue i = 0, n = indent*2; i < n; ++i) {
      Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
    }
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[ modulePath = ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuffer, Arcadia_FilePath_toGeneric(thread, modulePath));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ]\n");
    for (Arcadia_SizeValue i = 0, n = indent * 2; i < n; ++i) {
      Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
    }
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[ cxxFilesPath = ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuffer, Arcadia_FilePath_toGeneric(thread, cxxFilesPath));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ]\n");

    //
    for (Arcadia_SizeValue i = 0, n = indent * 2; i < n; ++i) {
      Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
    }
    Arcadia_FilePath* cxxSourceFilePath = makeCxxSourceFilePath(thread, (Arcadia_MILC_Symbol*)symbol, cxxFilesPath);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[ cxxSourceFilePath = ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuffer, Arcadia_FilePath_toGeneric(thread, cxxSourceFilePath));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ]\n");

    //
    for (Arcadia_SizeValue i = 0, n = indent * 2; i < n; ++i) {
      Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
    }
    Arcadia_FilePath* cxxHeaderFilePath = makeCxxHeaderFilePath(thread, (Arcadia_MILC_Symbol*)symbol, cxxFilesPath);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"[ cxxHeaderFilePath = ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuffer, Arcadia_FilePath_toGeneric(thread, cxxHeaderFilePath));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8" ]\n");
  }
}

static void
dumpProcedureSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_ProcedureSymbol* symbol
  )
{ 
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"procedure ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, ((Arcadia_MILC_Symbol*)symbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");
}

static void
dumpModuleSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_ModuleSymbol* symbol
  )
{
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"module ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, ((Arcadia_MILC_Symbol*)symbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"\n");

  Arcadia_List* enclosedSymbols = Arcadia_Map_getValues(thread, symbol->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)enclosedSymbols); i < n; ++i) {
    Arcadia_MILC_Symbol* enclosedSymbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, enclosedSymbols, i, _Arcadia_MILC_Symbol_getType(thread));
    dumpSymbol(thread, indent + 2, stringBuffer, enclosedSymbol);
  }
}

static void
dumpSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* stringBuffer,
    Arcadia_MILC_Symbol* symbol
  )
{
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, ' ');
  }
  switch (symbol->kind) {
    case Arcadia_MILC_SymbolKind_Class: {
      dumpClassSymbol(thread, indent + 2, stringBuffer, (Arcadia_MILC_ClassSymbol*)symbol);
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      dumpEnumerationSymbol(thread, indent + 2, stringBuffer, (Arcadia_MILC_EnumerationSymbol*)symbol);
    } break;
    case Arcadia_MILC_SymbolKind_Module: {
      dumpModuleSymbol(thread, indent + 2, stringBuffer, (Arcadia_MILC_ModuleSymbol*)symbol);
    } break;
    case Arcadia_MILC_SymbolKind_Procedure: {
      dumpProcedureSymbol(thread, indent + 2, stringBuffer, (Arcadia_MILC_ProcedureSymbol*)symbol);
    } break;
  }
}

void
Arcadia_MILC_CompilationTask_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_CompilationTask* self,
    Arcadia_FilePath* workingDirectoryPath,
    Arcadia_List* moduleDirectoryPaths
  )
{
  self->context->workingDirectoryPath = workingDirectoryPath;
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->context->moduleNodes);
  self->context->scanner = self->context->scanner ? self->context->scanner : Arcadia_MILC_Scanner_create(thread, self->context);
  self->context->parser = self->context->parser ? self->context->parser : Arcadia_MILC_Parser_create(thread, self->context);


  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)moduleDirectoryPaths); i < n; ++i) {
    // Get the module directory path from the list.
    Arcadia_FilePath* moduleDirectoryPath = (Arcadia_FilePath*)Arcadia_List_getObjectReferenceValueAt(thread, moduleDirectoryPaths, i);
    // Create a module from it.
    Arcadia_MILC_AST_ModuleNode* moduleNode = Arcadia_MILC_AST_ModuleNode_create(thread);
    // Add the module to the list of modules.
    moduleNode->moduleDirectoryPath = Arcadia_FilePath_clone(thread, moduleDirectoryPath);
    Arcadia_List_insertBackObjectReferenceValue(thread, self->context->moduleNodes, moduleNode);
  }
  // (1)
  step1(thread, self);
  // (2)
  step2(thread, self);
  // (3)
  step3(thread, self);
  // (4) Enter the top-level symbols (classes, enumerations, procedures, modules).
  self->context->enterPhase = self->context->enterPhase ? self->context->enterPhase : Arcadia_MILC_EnterPhase_create(thread, self->context);
  Arcadia_MILC_EnterPhase_run(thread, self->context->enterPhase);
  // (5) Enter the member-level symbols (constructors, methods, fields, enumeration elements).
  // This must be deferred because types are resolved.
  self->context->memberEnterPhase = self->context->memberEnterPhase ? self->context->memberEnterPhase : Arcadia_MILC_MemberEnterPhase_create(thread, self->context);
  // Dump top-level symbols.
#if defined(Arcadia_MILC_Configuration_ListTopLevelSymbols) && 1 == Arcadia_MILC_Configuration_ListTopLevelSymbols
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_List* symbols = Arcadia_Map_getValues(thread, self->context->scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) { 
    Arcadia_MILC_Symbol* symbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_MILC_Symbol_getType(thread));
    dumpSymbol(thread, 0, stringBuffer, symbol);
  }
  Arcadia_Log_information(thread, self->context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));
#endif
  //
  Arcadia_Languages_Diagnostics_emit(thread, self->context->diagnostics);
  if (Arcadia_Languages_Diagnostics_hasErrors(thread, self->context->diagnostics)) {
    Arcadia_Thread_raise(thread, Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_CompilationFailedException_create(thread)));
  }
}

#undef Verbose
