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

#include "Arcadia/MILC/Backend/EnumerationSymbolWriter.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Backend/SymbolInfo.h"
#include "Arcadia/MILC/Backend/Implementation.h"
#include "Arcadia/MILC/Backend/EnumerationConstantSymbolInfo.h"

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
onWriteEnumerationSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteEnumerationHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriterDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self
  );

static void
Arcadia_MILC_Backend_EnumerationSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
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

Arcadia_defineObjectType(u8"Arcadia.MILC.Backend.EnumerationSymbolWriter", Arcadia_MILC_Backend_EnumerationSymbolWriter,
                         u8"Arcadia.MILC.Backend.SymbolWriter", Arcadia_MILC_Backend_SymbolWriter,
                         &_typeOperations);

static void
onWriteEnumerationSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "static const Arcadia_Type_Operations _typeOperations = {\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  Arcadia_Type_Operations_Initializer,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_defineEnumerationType(u8\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                              &_typeOperations);\n");
}

static void
onWriteEnumerationHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  Arcadia_Map* symbolInfos = Arcadia_MILC_Backend_Implementation_getInstance(thread, context)->symbolInfos;

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_declareEnumerationType");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "(");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                               ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ")");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ";\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "enum ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " {\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_List* constantSymbols = ((Arcadia_MILC_EnumerationSymbol*)symbolInfo->symbol)->members;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)constantSymbols); i < n; ++i) {
    Arcadia_MILC_EnumerationConstantSymbol* constantSymbol = (Arcadia_MILC_EnumerationConstantSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, constantSymbols, i, _Arcadia_MILC_EnumerationConstantSymbol_getType(thread));
    Arcadia_Value temporary = Arcadia_Map_get(thread, symbolInfos, Arcadia_Value_makeObjectReferenceValue(constantSymbol));
    Arcadia_MILC_Backend_EnumerationConstantSymbolInfo* constantSymbolInfo;
    if (Arcadia_Value_isVoidValue(&temporary)) {
      constantSymbolInfo = Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_create(thread, context, (Arcadia_MILC_Symbol*)constantSymbol);
      Arcadia_Map_set(thread, symbolInfos, Arcadia_Value_makeObjectReferenceValue(constantSymbol), Arcadia_Value_makeObjectReferenceValue(constantSymbolInfo), NULL, NULL);
    } else {
      constantSymbolInfo = (Arcadia_MILC_Backend_EnumerationConstantSymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, temporary, _Arcadia_MILC_Backend_EnumerationConstantSymbolInfo_getType(thread));
    }
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, constantSymbolInfo->cxxName);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " = ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, constantSymbol->ast->initializer->value);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  }

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
}

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  if (Arcadia_MILC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->completer) {
    Arcadia_MILC_Completer_complete(thread, symbolInfo->symbol->completer, context, Arcadia_MILC_Backend_SymbolInfo_getSymbol(thread, symbolInfo));
  }

  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);
  Arcadia_MILC_Backend_SymbolInfo* moduleSymbolInfo = Arcadia_MILC_Backend_SymbolInfo_getModule(thread, symbolInfo);

  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#define ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE (1)");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#include \"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_Implementation_nameToPathString(thread, implementation, symbolInfo->symbol->name));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ".h\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  onWriteEnumerationSourceFile(thread, self, context, symbolInfo, stringBuilder);
}

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{ 
  if (Arcadia_MILC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->completer) {
    Arcadia_MILC_Completer_complete(thread, symbolInfo->symbol->completer, context, Arcadia_MILC_Backend_SymbolInfo_getSymbol(thread, symbolInfo));
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
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "` directly, include `");
  includePath = Arcadia_MILC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, moduleSymbolInfo);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "` instead\")\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#endif\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  // #include ...
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#include \"Arcadia/Ring1/Include.h\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  onWriteEnumerationHeaderFile(thread, self, context, symbolInfo, stringBuilder);

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
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Backend_EnumerationSymbolWriter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_Backend_EnumerationSymbolWriter);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriterDispatch* self
  )
{
  ((Arcadia_MILC_Backend_SymbolWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolWriter*, Arcadia_MILC_Context*, Arcadia_MILC_Backend_CXXFileType, Arcadia_MILC_Backend_SymbolInfo*,Arcadia_StringBuilder*)) & Arcadia_MILC_Backend_EnumerationSymbolWriter_writeImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_Backend_EnumerationSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_EnumerationSymbolWriter* self,
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

Arcadia_MILC_Backend_EnumerationSymbolWriter*
Arcadia_MILC_Backend_EnumerationSymbolWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Backend_EnumerationSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_Backend_EnumerationSymbolWriter);
}
