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

#include "Arcadia/PDLC/Backend/ClassSymbolInfo.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/AST/Include.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/ModuleSymbolInfo.h"
#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfoDispatch* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static void
Arcadia_PDLC_Backend_ClassSymbolInfo_dumpImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  );

static Arcadia_PDLC_Symbol*
Arcadia_PDLC_Backend_ClassSymbolInfo_getSymbolImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static Arcadia_String*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static Arcadia_String*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxNameUpperCaseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxHeaderFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxHeaderFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxSourceFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  );

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxSourceFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
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

Arcadia_defineObjectType(u8"Arcadia.PDLC.Backend.ClassSymbolInfo", Arcadia_PDLC_Backend_ClassSymbolInfo,
                         u8"Arcadia.PDLC.Backend.SymbolInfo", Arcadia_PDLC_Backend_SymbolInfo,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Backend_ClassSymbolInfo);
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

  Arcadia_PDLC_Context* context = (Arcadia_PDLC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_PDLC_Context_getType(thread));
  Arcadia_PDLC_Backend_Implementation* implementation = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);

  self->symbol = (Arcadia_PDLC_Symbol*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_Symbol_getType(thread));

  self->cxxName = Arcadia_PDLC_Backend_Implementation_computeCxxName(thread, implementation, self->symbol->name);
  self->cxxNameUpperCase = Arcadia_PDLC_Backend_Implementation_computeCxxNameUpperCase(thread, implementation, self->symbol->name);

  self->cxxHeaderFilePath = NULL;
  self->cxxHeaderFileAbsolutePath = NULL;

  self->cxxSourceFilePath = NULL;
  self->cxxSourceFileAbsolutePath = NULL;

  // Get the module symbol of this symbol.
  Arcadia_PDLC_Symbol* moduleSymbol = self->symbol->enclosing;
  if (moduleSymbol) {
    Arcadia_PDLC_Backend_ModuleSymbolInfo* moduleSymbolInfo =
      (Arcadia_PDLC_Backend_ModuleSymbolInfo*)Arcadia_PDLC_Backend_Implementation_getOrCreateInfo(thread, implementation, moduleSymbol);

    self->cxxHeaderFilePath = Arcadia_PDLC_Backend_Implementation_makeCxxHeaderFilePath(thread, implementation, self->symbol);
    self->cxxHeaderFileAbsolutePath = Arcadia_FilePath_clone(thread, moduleSymbolInfo->cxxFilesPath);
    Arcadia_FilePath_append(thread, self->cxxHeaderFileAbsolutePath, self->cxxHeaderFilePath);

    self->cxxSourceFilePath = Arcadia_PDLC_Backend_Implementation_makeCxxSourceFilePath(thread, implementation, self->symbol);
    self->cxxSourceFileAbsolutePath = Arcadia_FilePath_clone(thread, moduleSymbolInfo->cxxFilesPath);
    Arcadia_FilePath_append(thread, self->cxxSourceFileAbsolutePath, self->cxxSourceFilePath);
  }

  Arcadia_LeaveConstructor(Arcadia_PDLC_Backend_ClassSymbolInfo);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfoDispatch* self
  )
{
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->dump = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*, Arcadia_SizeValue, Arcadia_StringBuilder*))&Arcadia_PDLC_Backend_ClassSymbolInfo_dumpImpl;
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getSymbol = (Arcadia_PDLC_Symbol* (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*)) & Arcadia_PDLC_Backend_ClassSymbolInfo_getSymbolImpl;
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getCxxName = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*))&Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxNameImpl;
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getCxxNameUpperCase = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*))&Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxNameUpperCaseImpl;

  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getCxxHeaderFilePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*)) & Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxHeaderFilePathImpl;
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getCxxHeaderFileAbsolutePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*)) & Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxHeaderFileAbsolutePathImpl;
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getCxxSourceFilePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*)) & Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxSourceFilePathImpl;
  ((Arcadia_PDLC_Backend_SymbolInfoDispatch*)self)->getCxxSourceFileAbsolutePath = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolInfo*)) & Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxSourceFileAbsolutePathImpl;
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
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
Arcadia_PDLC_Backend_ClassSymbolInfo_dumpImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self,
    Arcadia_SizeValue indent,
    Arcadia_StringBuilder* target
  )
{
  Arcadia_PDLC_Backend_ModuleSymbolInfo* module = (Arcadia_PDLC_Backend_ModuleSymbolInfo*)Arcadia_PDLC_Backend_SymbolInfo_getModule(thread, (Arcadia_PDLC_Backend_SymbolInfo*)self);
  if (module == module) { }
  Arcadia_FilePath* filePath = NULL;
  Arcadia_PDLC_Symbol* symbol = ((Arcadia_PDLC_Backend_SymbolInfo*)self)->symbol;
  for (Arcadia_SizeValue i = 0, n = indent; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackString(thread, target, Arcadia_PDLC_SymbolKind_toString(thread, symbol->kind));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ");
  Arcadia_StringBuilder_insertBackString(thread, target, ((Arcadia_PDLC_Symbol*)symbol)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxSourceFilePath = ");
  filePath = Arcadia_PDLC_Backend_SymbolInfo_getCxxSourceFilePath(thread, (Arcadia_PDLC_Backend_SymbolInfo*)self);
  Arcadia_StringBuilder_insertBackString(thread, target, filePath ? Arcadia_FilePath_toGeneric(thread, filePath) : Arcadia_String_createFromCxxString(thread, u8"null"));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxHeaderFilePath = ");
  filePath = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, (Arcadia_PDLC_Backend_SymbolInfo*)self);
  Arcadia_StringBuilder_insertBackString(thread, target, filePath ? Arcadia_FilePath_toGeneric(thread, filePath) : Arcadia_String_createFromCxxString(thread, u8"null"));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
  //
  for (Arcadia_SizeValue i = 0, n = indent + 2; i < n; ++i) {
    Arcadia_StringBuilder_insertBackCodePoint(thread, target, ' ');
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8"[ cxxName = ");
  Arcadia_StringBuilder_insertBackString(thread, target, Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, (Arcadia_PDLC_Backend_SymbolInfo*)self));
  Arcadia_StringBuilder_insertBackCxxString(thread, target, u8" ]\n");
}

static Arcadia_PDLC_Symbol*
Arcadia_PDLC_Backend_ClassSymbolInfo_getSymbolImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->symbol; }

static Arcadia_String*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->cxxName; }

static Arcadia_String*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxNameUpperCaseImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->cxxNameUpperCase; }

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxHeaderFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->cxxHeaderFilePath ? Arcadia_FilePath_clone(thread, self->cxxHeaderFilePath) : NULL; }

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxHeaderFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->cxxHeaderFileAbsolutePath ? Arcadia_FilePath_clone(thread, self->cxxHeaderFileAbsolutePath) : NULL; }

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxSourceFilePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->cxxSourceFilePath ? Arcadia_FilePath_clone(thread, self->cxxSourceFilePath) : NULL; }

static Arcadia_FilePath*
Arcadia_PDLC_Backend_ClassSymbolInfo_getCxxSourceFileAbsolutePathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ClassSymbolInfo* self
  )
{ return self->cxxHeaderFileAbsolutePath ? Arcadia_FilePath_clone(thread, self->cxxSourceFileAbsolutePath) : NULL; }

Arcadia_PDLC_Backend_ClassSymbolInfo*
Arcadia_PDLC_Backend_ClassSymbolInfo_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Backend_ClassSymbolInfo);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (symbol) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)symbol); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_PDLC_Backend_ClassSymbolInfo);
}
