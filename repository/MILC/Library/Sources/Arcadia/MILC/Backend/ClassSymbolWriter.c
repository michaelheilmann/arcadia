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

#include "Arcadia/MILC/Backend/ClassSymbolWriter.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Backend/SymbolInfo.h"
#include "Arcadia/MILC/Backend/Implementation.h"
#include "Arcadia/MILC/Backend/ClassSymbolInfo.h"
#include <assert.h>

static const char* COPYRIGHT =
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
  "// along with this program. If not, see <https://www.gnu.org/licenses/>.\n"
  ;

static void
onWriteClassSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteClassHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriterDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self
  );

static void
Arcadia_MILC_Backend_ClassSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_CXXFileType fileType,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
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

Arcadia_defineObjectType(u8"Arcadia.MILC.Backend.ClassSymbolWriter", Arcadia_MILC_Backend_ClassSymbolWriter,
                         u8"Arcadia.MILC.Backend.SymbolWriter", Arcadia_MILC_Backend_SymbolWriter,
                         &_typeOperations);

static void
onWriteFieldHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
  Arcadia_MILC_VariableSymbol* variableSymbol = (Arcadia_MILC_VariableSymbol*)symbol;
  Arcadia_MILC_Symbol* typeSymbol = variableSymbol->type;
  assert(NULL != typeSymbol);
  switch (typeSymbol->kind) {
    case Arcadia_MILC_SymbolKind_Class: {
      Arcadia_Value v = Arcadia_Map_get(thread, Arcadia_MILC_Backend_Implementation_getInstance(thread, context)->symbolInfos, Arcadia_Value_makeObjectReferenceValue(typeSymbol));
      if (Arcadia_Value_isVoidValue(&v)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_MILC_Backend_SymbolInfo* typeSymbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, _Arcadia_MILC_Backend_SymbolInfo_getType(thread));
      Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, typeSymbolInfo));
      Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"*");
    } break;
    case Arcadia_MILC_SymbolKind_Enumeration: {
      Arcadia_Value v = Arcadia_Map_get(thread, Arcadia_MILC_Backend_Implementation_getInstance(thread, context)->symbolInfos, Arcadia_Value_makeObjectReferenceValue(typeSymbol));
      if (Arcadia_Value_isVoidValue(&v)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_MILC_Backend_SymbolInfo* typeSymbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, _Arcadia_MILC_Backend_SymbolInfo_getType(thread));
      Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, typeSymbolInfo));
    } break;
    case Arcadia_MILC_SymbolKind_PrimitiveType: {
      Arcadia_Value v = Arcadia_Map_get(thread, Arcadia_MILC_Backend_Implementation_getInstance(thread, context)->symbolInfos, Arcadia_Value_makeObjectReferenceValue(typeSymbol));
      if (Arcadia_Value_isVoidValue(&v)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      Arcadia_MILC_Backend_SymbolInfo* typeSymbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, _Arcadia_MILC_Backend_SymbolInfo_getType(thread));
      Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, typeSymbolInfo));
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8" ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, symbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8";\n");
}

static void
onWriteMethodHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ";\n");
}

static void
onWriteClassSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "static void\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "constructImpl\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  (\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "    Arcadia_Thread* thread,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "    ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "*");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " self\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  );\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "static void\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "initializeDispatchImpl\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  (\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "    Arcadia_Thread* thread,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "    ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Dispatch*");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " self\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  );\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "static const Arcadia_ObjectType_Operations _objectTypeOperations = {\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  Arcadia_Type_Operations_Initializer,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "static const Arcadia_Type_Operations _typeOperations = {\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  Arcadia_Type_Operations_Initializer,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  .objectTypeOperations = &_objectTypeOperations,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_defineObjectType(u8\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, symbolInfo->symbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  Arcadia_MILC_ClassSymbol* parentClassSymbol = ((Arcadia_MILC_ClassSymbol*)symbolInfo->symbol)->parentClassSymbol;
  Arcadia_Value v = Arcadia_Map_get(thread, Arcadia_MILC_Backend_Implementation_getInstance(thread, context)->symbolInfos, Arcadia_Value_makeObjectReferenceValue(parentClassSymbol));
  if (Arcadia_Value_isVoidValue(&v)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_MILC_Backend_SymbolInfo* parentSymbolInfo = (Arcadia_MILC_Backend_SymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, _Arcadia_MILC_Backend_SymbolInfo_getType(thread));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                         u8\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, parentSymbolInfo->symbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, parentSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                         &_typeOperations);\n");
}

static void
onWriteClassHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  Arcadia_MILC_Symbol* classSymbol = symbolInfo->symbol;
  Arcadia_MILC_Backend_SymbolInfo* classSymbolInfo = symbolInfo;

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_declareClassType");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "(");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, classSymbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, classSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  assert(NULL != classSymbol);
  Arcadia_MILC_ClassSymbol* parentClassSymbol = ((Arcadia_MILC_ClassSymbol*)classSymbol)->parentClassSymbol;
  assert(NULL != parentClassSymbol);

  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);
  Arcadia_MILC_Backend_SymbolInfo* parentClassSymbolInfo =
    (Arcadia_MILC_Backend_SymbolInfo*)
    Arcadia_Map_getObjectReferenceValueChecked
      (
        thread,
        implementation->symbolInfos,
        Arcadia_Value_makeObjectReferenceValue(parentClassSymbol),
        _Arcadia_MILC_Backend_SymbolInfo_getType(thread)
      );
  assert(NULL != parentClassSymbolInfo);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                         ");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, parentClassSymbolInfo->symbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ")");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ";\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "struct ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Dispatch {\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, parentClassSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Dispatch ");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " _parent;\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "struct ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " {\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, parentClassSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " ");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " _parent;\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  Arcadia_List* memberSymbols = ((Arcadia_MILC_ClassSymbol*)symbolInfo->symbol)->members;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)memberSymbols); i < n; ++i) {
    Arcadia_MILC_Symbol* memberSymbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, memberSymbols, i, _Arcadia_MILC_Symbol_getType(thread));
    switch (memberSymbol->kind) {
      case Arcadia_MILC_SymbolKind_Constructor: {
        /*Intentionally empty.*/
      } break;
      case Arcadia_MILC_SymbolKind_Variable: {
        onWriteFieldHeaderFile(thread, self, context, memberSymbol, stringBuilder);
      } break;
      case Arcadia_MILC_SymbolKind_Method: {
        onWriteMethodHeaderFile(thread, self, context, memberSymbol, stringBuilder);
      } break;
    };
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
}

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  if (symbolInfo->symbol->completer) {
    Arcadia_MILC_Completer_complete(thread, symbolInfo->symbol->completer, context, symbolInfo->symbol);
  }

  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);

  Arcadia_MILC_Backend_SymbolInfo* moduleSymbolInfo = Arcadia_MILC_Backend_SymbolInfo_getModule(thread, symbolInfo);
  Arcadia_String* upperCaseName = Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo);
  assert(NULL != upperCaseName);
  Arcadia_FilePath* includePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo);
  assert(NULL != includePath);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#define ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, upperCaseName);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE (1)");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#include \"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".h\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  onWriteClassSourceFile(thread, self, context, symbolInfo, stringBuilder);
}

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  if (symbolInfo->symbol->completer) {
    Arcadia_MILC_Completer_complete(thread, symbolInfo->symbol->completer, context, symbolInfo->symbol);
  }

  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);

  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_MILC_Backend_SymbolInfo* moduleSymbolInfo = Arcadia_MILC_Backend_SymbolInfo_getModule(thread, symbolInfo);
  Arcadia_FilePath* includePath = NULL;
  // #ifndef 
  // #define
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#ifndef ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#define ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  // #if !defined(...) || 1 != ...
  //   #error(...)
  // #endif
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#if !defined(");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ") || 1 != ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#error(\"do not include `");
  includePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder,  Arcadia_MILC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "` directly, include `");
  includePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, moduleSymbolInfo);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "` instead\")\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#endif\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  // #include ...
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#include \"Arcadia/Ring1/Include.h\"\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  onWriteClassHeaderFile(thread, self, context, symbolInfo, stringBuilder);

  // #endif
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#endif // ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Backend_ClassSymbolWriter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_Backend_ClassSymbolWriter);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriterDispatch* self
  )
{
  ((Arcadia_MILC_Backend_SymbolWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolWriter*, Arcadia_MILC_Context*, Arcadia_MILC_Backend_CXXFileType, Arcadia_MILC_Backend_SymbolInfo*,Arcadia_StringBuilder*)) & Arcadia_MILC_Backend_ClassSymbolWriter_writeImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_Backend_ClassSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_ClassSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_CXXFileType fileType,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  switch (fileType) {
    case Arcadia_MILC_Backend_CXXFileType_C: {
      onWriteSourceFile(thread, self, context, symbolInfo, stringBuilder);
    } break;
    case Arcadia_MILC_Backend_CXXFileType_H: {
      onWriteHeaderFile(thread, self, context, symbolInfo, stringBuilder);
    } break;
  };
}

Arcadia_MILC_Backend_ClassSymbolWriter*
Arcadia_MILC_Backend_ClassSymbolWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Backend_ClassSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_Backend_ClassSymbolWriter);
}
