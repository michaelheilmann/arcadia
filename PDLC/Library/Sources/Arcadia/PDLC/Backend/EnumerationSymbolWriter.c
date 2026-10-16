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

#include "Arcadia/PDLC/Backend/EnumerationSymbolWriter.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/AST/Include.h"
#include "Arcadia/PDLC/Backend/SymbolInfo.h"
#include "Arcadia/PDLC/Backend/Implementation.h"
#include "Arcadia/PDLC/Backend/EnumerationConstantSymbolInfo.h"

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
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteEnumerationHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriterDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self
  );

static void
Arcadia_PDLC_Backend_EnumerationSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_CXXFileType fileType,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
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

Arcadia_defineObjectType(u8"Arcadia.PDLC.Backend.EnumerationSymbolWriter", Arcadia_PDLC_Backend_EnumerationSymbolWriter,
                         u8"Arcadia.PDLC.Backend.SymbolWriter", Arcadia_PDLC_Backend_SymbolWriter,
                         &_typeOperations);

static void
onWriteEnumerationSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "static const Arcadia_Type_Operations _typeOperations = {\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  Arcadia_Type_Operations_Initializer,\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_defineEnumerationType(u8\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                              &_typeOperations);\n");
}

static void
onWriteEnumerationHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_Map* symbolInfos = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context)->symbolInfos;

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_declareEnumerationType");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "(");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ",\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                               ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ")");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ";\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "enum ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, " {\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_List* constantSymbols = ((Arcadia_PDLC_EnumerationSymbol*)symbolInfo->symbol)->members;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)constantSymbols); i < n; ++i) {
    Arcadia_PDLC_EnumerationConstantSymbol* constantSymbol = (Arcadia_PDLC_EnumerationConstantSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, constantSymbols, i, _Arcadia_PDLC_EnumerationConstantSymbol_getType(thread));
    Arcadia_Value temporary = Arcadia_Map_get(thread, symbolInfos, Arcadia_Value_makeObjectReferenceValue(constantSymbol));
    Arcadia_PDLC_Backend_EnumerationConstantSymbolInfo* constantSymbolInfo;
    if (Arcadia_Value_isVoidValue(&temporary)) {
      constantSymbolInfo = Arcadia_PDLC_Backend_EnumerationConstantSymbolInfo_create(thread, context, (Arcadia_PDLC_Symbol*)constantSymbol);
      Arcadia_Map_set(thread, symbolInfos, Arcadia_Value_makeObjectReferenceValue(constantSymbol), Arcadia_Value_makeObjectReferenceValue(constantSymbolInfo), NULL, NULL);
    } else {
      constantSymbolInfo = (Arcadia_PDLC_Backend_EnumerationConstantSymbolInfo*)Arcadia_Value_getObjectReferenceValueChecked(thread, temporary, _Arcadia_PDLC_Backend_EnumerationConstantSymbolInfo_getType(thread));
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
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  if (Arcadia_PDLC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->completer) {
    Arcadia_PDLC_Completer_complete(thread, symbolInfo->symbol->completer, context, Arcadia_PDLC_Backend_SymbolInfo_getSymbol(thread, symbolInfo));
  }

  Arcadia_PDLC_Backend_Implementation* implementation = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);
  Arcadia_PDLC_Backend_SymbolInfo* moduleSymbolInfo = Arcadia_PDLC_Backend_SymbolInfo_getModule(thread, symbolInfo);

  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#define ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE (1)");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#include \"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation,
    Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo)));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\"");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  onWriteEnumerationSourceFile(thread, self, context, symbolInfo, stringBuilder);
}

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  if (Arcadia_PDLC_Backend_SymbolInfo_getSymbol(thread, symbolInfo)->completer) {
    Arcadia_PDLC_Completer_complete(thread, symbolInfo->symbol->completer, context, Arcadia_PDLC_Backend_SymbolInfo_getSymbol(thread, symbolInfo));
  }

  Arcadia_PDLC_Backend_Implementation* implementation = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);

  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_PDLC_Backend_SymbolInfo* moduleSymbolInfo = Arcadia_PDLC_Backend_SymbolInfo_getModule(thread, symbolInfo);
  Arcadia_FilePath* includePath = NULL;
  Arcadia_String* guard = Arcadia_PDLC_Backend_SymbolInfo_getCxxNameUpperCase(thread, symbolInfo);
  if (((Arcadia_PDLC_EnumerationSymbol*)symbolInfo->symbol)->ast && ((Arcadia_PDLC_EnumerationSymbol*)symbolInfo->symbol)->ast->nativeHeaderPath) {
    Arcadia_String* headerPath = ((Arcadia_PDLC_EnumerationSymbol*)symbolInfo->symbol)->ast->nativeHeaderPath;
    Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, headerPath);
    const unsigned char* bytes = Arcadia_String_getBytes(thread, headerPath);
    Arcadia_StringBuilder* builder = Arcadia_StringBuilder_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, builder);
    for (Arcadia_SizeValue i = 0; i < length - 2; ++i) {
      unsigned char ch = bytes[i];
      if (ch >= 'a' && ch <= 'z') ch -= 'a' - 'A';
      if (!((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))) ch = '_';
      Arcadia_StringBuilder_insertBackCodePoint(thread, builder, ch);
    }
    guard = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(builder));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_ValueStack_pushObjectReferenceValue(thread, guard);
  // #if !defined(...)
  // #define
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#if !defined(");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, guard);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED)");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#define ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, guard);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  // #if !defined(...) || 1 != ...
  //   #error(...)
  // #endif
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#if !defined(");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ") || 1 != ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxNameUpperCase(thread, moduleSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_PRIVATE");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#error(\"do not include `");
  includePath = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, symbolInfo);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "` directly, include `");
  includePath = Arcadia_PDLC_Backend_SymbolInfo_getCxxHeaderFilePath(thread, moduleSymbolInfo);
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_Implementation_pathToCxxPath(thread, implementation, includePath));
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
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, guard);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Backend_EnumerationSymbolWriter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_Backend_EnumerationSymbolWriter);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriterDispatch* self
  )
{
  ((Arcadia_PDLC_Backend_SymbolWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolWriter*, Arcadia_PDLC_Context*, Arcadia_PDLC_Backend_CXXFileType, Arcadia_PDLC_Backend_SymbolInfo*,Arcadia_StringBuilder*)) & Arcadia_PDLC_Backend_EnumerationSymbolWriter_writeImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_Backend_EnumerationSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_EnumerationSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_CXXFileType fileType,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  switch (fileType) {
    case Arcadia_PDLC_Backend_CXXFileType_C: {
      onWriteSourceFile(thread, self, context, symbolInfo, stringBuilder);
    } break;
    case Arcadia_PDLC_Backend_CXXFileType_H: {
      onWriteHeaderFile(thread, self, context, symbolInfo, stringBuilder);
    } break;
  };
}

Arcadia_PDLC_Backend_EnumerationSymbolWriter*
Arcadia_PDLC_Backend_EnumerationSymbolWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Backend_EnumerationSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_Backend_EnumerationSymbolWriter);
}
