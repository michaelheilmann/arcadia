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

#include "Arcadia/MILC/Backend/Implementation.h"

#include "Arcadia/MILC/Backend/SymbolInfo.h"
#include "Arcadia/MILC/Completer.h"

#include "Arcadia/MILC/Backend/ClassSymbolWriter.h"
#include "Arcadia/MILC/Backend/EnumerationConstantSymbolInfo.h"
#include "Arcadia/MILC/Backend/EnumerationSymbolWriter.h"

#include "Arcadia/MILC/Backend/ClassSymbolInfo.h"
#include "Arcadia/MILC/Backend/EnumerationSymbolInfo.h"
#include "Arcadia/MILC/Backend/ModuleSymbolInfo.h"
#include "Arcadia/MILC/Backend/PrimitiveTypeSymbolInfo.h"
#include "Arcadia/MILC/Backend/ProcedureSymbolInfo.h"

#include <assert.h>

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ImplementationDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  );

static void
onWriteSymbolInfo
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_MILC_Context* context 
  );

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );
  
static void
onVisitScope
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Context* context,
    Arcadia_Languages_Scope* scope
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

Arcadia_defineObjectType(u8"Arcadia.MILC.Backend.Implementation", Arcadia_MILC_Backend_Implementation,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Backend_Implementation);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->context = (Arcadia_MILC_Context*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_Context_getType(thread));
  self->classSymbolWriter = (Arcadia_MILC_Backend_SymbolWriter*)Arcadia_MILC_Backend_ClassSymbolWriter_create(thread);
  self->enumerationSymbolWriter = (Arcadia_MILC_Backend_SymbolWriter*)Arcadia_MILC_Backend_EnumerationSymbolWriter_create(thread);
  self->symbolInfos = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));

  Arcadia_LeaveConstructor(Arcadia_MILC_Backend_Implementation);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ImplementationDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  )
{
  if (self->context) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->context);
  }
  if (self->classSymbolWriter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->classSymbolWriter);
  }
  if (self->enumerationSymbolWriter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->enumerationSymbolWriter);
  }
  if (self->symbolInfos) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbolInfos);
  }
}

static void
onVisitSymbol
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  )
{ 
  assert(NULL != symbol);
  /// Only top level symbols are considered here.
  switch (symbol->kind) {
    case Arcadia_MILC_SymbolKind_Module: {
      // If the info object was already created, do nothing.
      Arcadia_Value value = Arcadia_Map_get(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol));
      if (Arcadia_Value_isVoidValue(&value)) {
        Arcadia_MILC_Backend_SymbolInfo* symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_ModuleSymbolInfo_create(thread, context, symbol);
        Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
        Arcadia_MILC_Backend_SymbolInfo_dump(thread, symbolInfo, 2, stringBuilder);
        Arcadia_Log_information(thread, context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
        Arcadia_Map_set(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol), Arcadia_Value_makeObjectReferenceValue(symbolInfo), NULL, NULL);
      }
      onVisitScope(thread, self, context, ((Arcadia_MILC_ModuleSymbol*)symbol)->scope);
    } break;
    case Arcadia_MILC_SymbolKind_Class: {
      // If the info object was already created, do nothing.
      Arcadia_Value value = Arcadia_Map_get(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol));
      if (Arcadia_Value_isVoidValue(&value)) {
        Arcadia_MILC_Backend_SymbolInfo* symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_ClassSymbolInfo_create(thread, context, symbol);
        Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
        Arcadia_MILC_Backend_SymbolInfo_dump(thread, symbolInfo, 2, stringBuilder);
        Arcadia_Log_information(thread, context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
        Arcadia_Map_set(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol), Arcadia_Value_makeObjectReferenceValue(symbolInfo), NULL, NULL);
      }
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      // If the info object was already created, do nothing.
      Arcadia_Value value = Arcadia_Map_get(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol));
      if (Arcadia_Value_isVoidValue(&value)) {
        Arcadia_MILC_Backend_SymbolInfo* symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_EnumerationSymbolInfo_create(thread, context, symbol);
        Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
        Arcadia_MILC_Backend_SymbolInfo_dump(thread, symbolInfo, 2, stringBuilder);
        Arcadia_Log_information(thread, context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
        Arcadia_Map_set(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol), Arcadia_Value_makeObjectReferenceValue(symbolInfo), NULL, NULL);
      }
    } break;
    case Arcadia_MILC_SymbolKind_PrimitiveType: {
      // If the info object was already created, do nothing.
      Arcadia_Value value = Arcadia_Map_get(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol));
      if (Arcadia_Value_isVoidValue(&value)) {
        Arcadia_MILC_Backend_SymbolInfo* symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_PrimitiveTypeSymbolInfo_create(thread, context, symbol);
        Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
        Arcadia_MILC_Backend_SymbolInfo_dump(thread, symbolInfo, 2, stringBuilder);
        Arcadia_Log_information(thread, context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
        Arcadia_Map_set(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol), Arcadia_Value_makeObjectReferenceValue(symbolInfo), NULL, NULL);
      }
    } break;
    case Arcadia_MILC_SymbolKind_Procedure: {
      // If the info object was already created, do nothing.
      Arcadia_Value value = Arcadia_Map_get(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol));
      if (Arcadia_Value_isVoidValue(&value)) {
        Arcadia_MILC_Backend_SymbolInfo* symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_ProcedureSymbolInfo_create(thread, context, symbol);
        Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
        Arcadia_MILC_Backend_SymbolInfo_dump(thread, symbolInfo, 2, stringBuilder);
        Arcadia_Log_information(thread, context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
        Arcadia_Map_set(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol), Arcadia_Value_makeObjectReferenceValue(symbolInfo), NULL, NULL);
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  }
}

static void
onVisitScope
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Context* context,
    Arcadia_Languages_Scope* scope
  )
{ 
  Arcadia_List* symbols = Arcadia_Map_getValues(thread, scope->entries);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbols); i < n; ++i) {
    Arcadia_MILC_Symbol* symbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbols, i, _Arcadia_MILC_Symbol_getType(thread));
    onVisitSymbol(thread, self, context, symbol);
  }
}

static void
onWriteSymbolInfo
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_MILC_Context* context
  )
{ 
  if (symbolInfo->symbol->kind == Arcadia_MILC_SymbolKind_Class) {
    if (symbolInfo->symbol->completer) {
      Arcadia_MILC_Completer_complete(thread, symbolInfo->symbol->completer, context, symbolInfo->symbol);
    }

    Arcadia_FilePath* filePath;

    Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* byteArrayBuilder = Arcadia_ByteArrayBuilder_create(thread);

    Arcadia_StringBuilder_clear(thread, stringBuilder);
    Arcadia_MILC_Backend_SymbolWriter_write(thread, self->classSymbolWriter, context, Arcadia_MILC_Backend_CXXFileType_C, symbolInfo, stringBuilder);
    Arcadia_ByteArrayBuilder_clear(thread, byteArrayBuilder);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteArrayBuilder, Arcadia_StringBuilder_getBytes(thread, stringBuilder), Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuilder));
    filePath = Arcadia_MILC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, symbolInfo);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, filePath, byteArrayBuilder);

    Arcadia_StringBuilder_clear(thread, stringBuilder);
    Arcadia_MILC_Backend_SymbolWriter_write(thread, self->classSymbolWriter, context, Arcadia_MILC_Backend_CXXFileType_H, symbolInfo, stringBuilder);
    Arcadia_ByteArrayBuilder_clear(thread, byteArrayBuilder);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteArrayBuilder, Arcadia_StringBuilder_getBytes(thread, stringBuilder), Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuilder));
    filePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, symbolInfo);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, filePath, byteArrayBuilder);
  }
  if (symbolInfo->symbol->kind == Arcadia_MILC_SymbolKind_Enumeration) {
    if (symbolInfo->symbol->completer) {
      Arcadia_MILC_Completer_complete(thread, symbolInfo->symbol->completer, context, symbolInfo->symbol);
    }

    Arcadia_FilePath* filePath;

    Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_ByteArrayBuilder* byteArrayBuilder = Arcadia_ByteArrayBuilder_create(thread);

    Arcadia_StringBuilder_clear(thread, stringBuilder);
    Arcadia_MILC_Backend_SymbolWriter_write(thread, self->enumerationSymbolWriter, context, Arcadia_MILC_Backend_CXXFileType_C, symbolInfo, stringBuilder);
    Arcadia_ByteArrayBuilder_clear(thread, byteArrayBuilder);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteArrayBuilder, Arcadia_StringBuilder_getBytes(thread, stringBuilder), Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuilder));
    filePath = Arcadia_MILC_Backend_SymbolInfo_getCxxSourceFileAbsolutePath(thread, symbolInfo);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, filePath, byteArrayBuilder);

    Arcadia_StringBuilder_clear(thread, stringBuilder);
    Arcadia_MILC_Backend_SymbolWriter_write(thread, self->enumerationSymbolWriter, context, Arcadia_MILC_Backend_CXXFileType_H, symbolInfo, stringBuilder);
    Arcadia_ByteArrayBuilder_clear(thread, byteArrayBuilder);
    Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteArrayBuilder, Arcadia_StringBuilder_getBytes(thread, stringBuilder), Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuilder));
    filePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFileAbsolutePath(thread, symbolInfo);
    Arcadia_FileSystem_setFileContents(thread, fileSystem, filePath, byteArrayBuilder);
  }
}

Arcadia_MILC_Backend_Implementation*
Arcadia_MILC_Backend_Implementation_create
  ( 
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Backend_Implementation);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, context); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_Backend_Implementation);
}

Arcadia_MILC_Backend_Implementation*
Arcadia_MILC_Backend_Implementation_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{ 
  Arcadia_Value k = Arcadia_Value_makeTypeValue(_Arcadia_MILC_Backend_Implementation_getType(thread));
  Arcadia_Value v = Arcadia_Map_get(thread, context->instances, k);
  if (Arcadia_Value_isVoidValue(&v)) {
    v = Arcadia_Value_makeObjectReferenceValue(Arcadia_MILC_Backend_Implementation_create(thread, context));
    Arcadia_Map_set(thread, context->instances, k, v, NULL, NULL);
  }
  return (Arcadia_MILC_Backend_Implementation*)Arcadia_Value_getObjectReferenceValue(&v);
}

void
Arcadia_MILC_Backend_Implementation_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self
  )
{
  // (1) Clear the collection of backend symbol infos.
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)self->symbolInfos);
  // (2) Enter the backend symbol infos.
  onVisitScope(thread, self, self->context, self->context->scope);
  Arcadia_MILC_Symbol* objectSymbol = Arcadia_MILC_Symbols_getInstance(thread, self->context)->objectSymbol;
  Arcadia_List* symbolInfos = Arcadia_Map_getValues(thread, self->symbolInfos);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)symbolInfos); i < n; ++i) {
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, symbolInfos, i, _Arcadia_MILC_Backend_SymbolInfo_getType(thread));
    if (objectSymbol != symbolInfo->symbol && (symbolInfo->symbol->kind == Arcadia_MILC_SymbolKind_Class || symbolInfo->symbol->kind == Arcadia_MILC_SymbolKind_Enumeration)) {
      onWriteSymbolInfo(thread, self, symbolInfo, self->context);
    }
  }
}

Arcadia_MILC_Backend_SymbolInfo*
Arcadia_MILC_Backend_Implementation_getOrCreateInfo
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_MILC_Symbol* symbol
  )
{
  assert(NULL != symbol);
  // If the info object was already created, return that info object.
  Arcadia_Value value = Arcadia_Map_get(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol));
  if (!Arcadia_Value_isVoidValue(&value)) {
    return (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, value, _Arcadia_MILC_Backend_SymbolInfo_getType(thread));
  }
  // Otherwise create the info object.
  Arcadia_MILC_Backend_SymbolInfo* symbolInfo = NULL;
  switch (symbol->kind) {
    case Arcadia_MILC_SymbolKind_Module: {
      symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_ModuleSymbolInfo_create(thread, self->context, symbol);
    } break;
    case Arcadia_MILC_SymbolKind_Class: {
      symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_ClassSymbolInfo_create(thread, self->context, symbol);
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_EnumerationSymbolInfo_create(thread, self->context, symbol);
    } break;
    case Arcadia_MILC_SymbolKind_PrimitiveType: {
      symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_PrimitiveTypeSymbolInfo_create(thread, self->context, symbol);
    } break;
    case Arcadia_MILC_SymbolKind_Procedure: {
      symbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_MILC_Backend_ProcedureSymbolInfo_create(thread, self->context, symbol);
    } break;
    default: {
      symbolInfo = Arcadia_MILC_Backend_SymbolInfo_create(thread, self->context, symbol);
    } break;
  }
  assert(NULL != symbolInfo);
  Arcadia_StringBuilder* stringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_MILC_Backend_SymbolInfo_dump(thread, symbolInfo, 2, stringBuilder);
  Arcadia_Log_information(thread, self->context->log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuilder)));
  Arcadia_Map_set(thread, self->symbolInfos, Arcadia_Value_makeObjectReferenceValue(symbol), Arcadia_Value_makeObjectReferenceValue(symbolInfo), NULL, NULL);
  return symbolInfo;
}

Arcadia_String*
Arcadia_MILC_Backend_Implementation_pathToCxxPath
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_Implementation* self,
    Arcadia_FilePath* path
  )
{ 
  /// @todo Cache in file system.
  Arcadia_String* directorySeparator = Arcadia_String_createFromCxxString(thread, u8"/");

  Arcadia_ByteArrayBuilder* temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)path->fileNames);
  if (path->root || n == 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value e = Arcadia_List_getAt(thread, path->fileNames, 0);
  Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
  Arcadia_Unicode_Encoder_encodeString(thread, encoder, fileName, temporaryBuffer);
  i++;

  for (; i < n; ++i) {
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, directorySeparator, temporaryBuffer);

    Arcadia_Value e = Arcadia_List_getAt(thread, path->fileNames, i);
    Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, fileName, temporaryBuffer);
  }
  Arcadia_Value temporaryValue;
  Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)temporaryBuffer);
  return Arcadia_String_create(thread, temporaryValue);
}
