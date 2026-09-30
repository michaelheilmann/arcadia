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

#include "Arcadia/MILC/Backend/ModuleSymbolInfo.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Backend/Implementation.h"
#include <assert.h>

static Arcadia_FilePath*
makeCxxSourceFileAbsolutePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* implementation,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  );

static Arcadia_FilePath*
makeCxxHeaderFileAbsolutePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* implementation,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfoDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static void
Arcadia_MILC_Backend_ModuleSymbolInfo_dumpImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  );

static Arcadia_MILC_Symbol*
Arcadia_MILC_Backend_ModuleSymbolInfo_getSymbolImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static Arcadia_String*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static Arcadia_String*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxNameUpperCaseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxHeaderFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxHeaderFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxSourceFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxSourceFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.Backend.ModuleSymbolInfo", Arcadia_MILC_Backend_ModuleSymbolInfo,
                         u8"Arcadia.MILC.Backend.SymbolInfo", Arcadia_MILC_Backend_SymbolInfo,
                         &_typeOperations);

static Arcadia_FilePath*
makeCxxSourceFileAbsolutePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* implementation,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  )
{
  Arcadia_FilePath* suffix = Arcadia_MILC_Backend_Implementation_makeCxxSourceFilePath(thread, implementation, symbol);
  Arcadia_FilePath* cxxFilePath = Arcadia_FilePath_clone(thread, cxxFilesPath);
  Arcadia_FilePath_append(thread, cxxFilePath, suffix);
  return cxxFilePath;
}

static Arcadia_FilePath*
makeCxxHeaderFileAbsolutePath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* implementation,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_FilePath* cxxFilesPath
  )
{
  Arcadia_FilePath* suffix = Arcadia_MILC_Backend_Implementation_makeCxxHeaderFilePath(thread, implementation, symbol);
  Arcadia_FilePath* cxxFilePath = Arcadia_FilePath_clone(thread, cxxFilesPath);
  Arcadia_FilePath_append(thread, cxxFilePath, suffix);
  return cxxFilePath;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Backend_ModuleSymbolInfo);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
   {
    Arcadia_Value symbol = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_Value context = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &symbol);
    Arcadia_ValueStack_pushValue(thread, &context);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }

  Arcadia_MILC_Context* context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_Context_getType(thread));
  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);

  self->symbol = (Arcadia_MILC_Symbol*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Symbol_getType(thread));

  self->cxxName = Arcadia_MILC_Backend_Implementation_computeCxxName(thread, implementation, self->symbol->name);
  self->cxxNameUpperCase = Arcadia_MILC_Backend_Implementation_computeCxxNameUpperCase(thread, implementation, self->symbol->name);

  Arcadia_MILC_ModuleSymbol* moduleSymbol = (Arcadia_MILC_ModuleSymbol*)self->symbol;
  Arcadia_MILC_AST_CompilationUnitNode* compilationUnitNode = ((Arcadia_MILC_AST_DefinitionNode*)moduleSymbol->moduleDefinitionNode)->compilationUnitNode;
  Arcadia_MILC_AST_ModuleNode* moduleNode = compilationUnitNode->moduleNode;

  self->moduleDirectoryPath = moduleNode->moduleDirectoryPath;
  Arcadia_FilePath* cxxFilesPath = Arcadia_FilePath_clone(thread, self->moduleDirectoryPath);
  Arcadia_FilePath_append(thread, cxxFilesPath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, "Library/Sources")));
  self->cxxFilesPath = cxxFilesPath;

  self->cxxHeaderFilePath = Arcadia_MILC_Backend_Implementation_makeCxxHeaderFilePath(thread, implementation, self->symbol);
  self->cxxHeaderFileAbsolutePath = makeCxxHeaderFileAbsolutePath(thread, implementation, self->symbol, self->cxxFilesPath);
  self->cxxSourceFilePath = Arcadia_MILC_Backend_Implementation_makeCxxSourceFilePath(thread, implementation, self->symbol);
  self->cxxSourceFileAbsolutePath = makeCxxSourceFileAbsolutePath(thread, implementation, self->symbol, self->cxxFilesPath);

  Arcadia_LeaveConstructor(Arcadia_MILC_Backend_ModuleSymbolInfo);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfoDispatch* self
  )
{
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->dump = (void (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*, Arcadia_SizeValue, Arcadia_StringBuilder*))&Arcadia_MILC_Backend_ModuleSymbolInfo_dumpImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getSymbol = (Arcadia_MILC_Symbol * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) & Arcadia_MILC_Backend_ModuleSymbolInfo_getSymbolImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxName = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*))&Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxNameImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxNameUpperCase = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*))&Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxNameUpperCaseImpl;

  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxHeaderFilePath = (Arcadia_FilePath* (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) & Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxHeaderFilePathImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxHeaderFileAbsolutePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) &Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxHeaderFileAbsolutePathImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxSourceFilePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) &Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxSourceFilePathImpl;
  ((Arcadia_MILC_Backend_SymbolInfoDispatch*)self)->getCxxSourceFileAbsolutePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolInfo*)) &Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxSourceFileAbsolutePathImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{
  if (self->symbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbol);
  }

  if (self->cxxName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxName);
  }
  if (self->cxxNameUpperCase) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxNameUpperCase);
  }

  if (self->moduleDirectoryPath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleDirectoryPath);
  }
  if (self->cxxFilesPath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxFilesPath);
  }

  if (self->cxxSourceFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxSourceFilePath);
  }
  if (self->cxxSourceFileAbsolutePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxSourceFileAbsolutePath);
  }

  if (self->cxxHeaderFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxHeaderFilePath);
  }
  if (self->cxxHeaderFileAbsolutePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->cxxHeaderFileAbsolutePath);
  }
}

static void
Arcadia_MILC_Backend_ModuleSymbolInfo_dumpImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  )
{ 
  Arcadia_FilePath* filePath = NULL;
  Arcadia_MILC_Symbol* symbol = ((Arcadia_MILC_Backend_SymbolInfo*)self)->symbol;
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackString(thread, target, Arcadia_MILC_SymbolKind_toString(thread, symbol->kind));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ");
  Arcadia_StringBuilder_insertBackString(thread, target, ((Arcadia_MILC_Symbol*)symbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ modulePath = ");
  filePath = self->moduleDirectoryPath;
  Arcadia_StringBuilder_insertBackString(thread, target, filePath ? Arcadia_FilePath_toGeneric(thread, filePath) : Arcadia_String_createFromCxxString(thread, u8"null"));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxFilesPath = ");
  filePath = self->cxxFilesPath;
  Arcadia_StringBuilder_insertBackString(thread, target, filePath ? Arcadia_FilePath_toGeneric(thread, filePath) : Arcadia_String_createFromCxxString(thread, u8"null"));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxSourceFilePath = ");
  filePath = Arcadia_MILC_Backend_SymbolInfo_getCxxSourceFilePath(thread, (Arcadia_MILC_Backend_SymbolInfo*)self);
  Arcadia_StringBuilder_insertBackString(thread, target, filePath ? Arcadia_FilePath_toGeneric(thread, filePath) : Arcadia_String_createFromCxxString(thread, u8"null"));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxHeaderFilePath = ");
  filePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, (Arcadia_MILC_Backend_SymbolInfo*)self);
  Arcadia_StringBuilder_insertBackString(thread, target, filePath ? Arcadia_FilePath_toGeneric(thread, filePath) : Arcadia_String_createFromCxxString(thread, u8"null"));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxName = ");
  Arcadia_StringBuilder_insertBackString(thread, target, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, (Arcadia_MILC_Backend_SymbolInfo*)self));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
}

static Arcadia_MILC_Symbol*
Arcadia_MILC_Backend_ModuleSymbolInfo_getSymbolImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->symbol; }

static Arcadia_String*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->cxxName; }

static Arcadia_String*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxNameUpperCaseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->cxxNameUpperCase; }

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxHeaderFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->cxxHeaderFilePath; }

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxHeaderFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->cxxHeaderFileAbsolutePath; }

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxSourceFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->cxxSourceFilePath; }

static Arcadia_FilePath*
Arcadia_MILC_Backend_ModuleSymbolInfo_getCxxSourceFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ModuleSymbolInfo* self
  )
{ return self->cxxSourceFileAbsolutePath; }

Arcadia_MILC_Backend_ModuleSymbolInfo*
Arcadia_MILC_Backend_ModuleSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Backend_ModuleSymbolInfo);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (symbol) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)symbol); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_MILC_Backend_ModuleSymbolInfo); 
}
