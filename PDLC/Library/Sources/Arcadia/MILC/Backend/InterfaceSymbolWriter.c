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

#include "Arcadia/MILC/Backend/InterfaceSymbolWriter.h"

#include "Arcadia/MILC/Include.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Backend/SymbolInfo.h"
#include "Arcadia/MILC/Backend/Implementation.h"
#include "Arcadia/MILC/Backend/ModuleSymbolInfo.h"
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
onWriteInterfaceSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteInterfaceHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self
  );

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriterDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self
  );

static void
Arcadia_MILC_Backend_InterfaceSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
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

Arcadia_defineObjectType(u8"Arcadia.MILC.Backend.InterfaceSymbolWriter", Arcadia_MILC_Backend_InterfaceSymbolWriter,
                         u8"Arcadia.MILC.Backend.SymbolWriter", Arcadia_MILC_Backend_SymbolWriter,
                         &_typeOperations);

/// Write the parameter list of @c operation, enclosed in parenthesis, to @c stringBuilder.
static void
onWriteParameterList
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_MethodSymbol* operation,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"Arcadia_Thread* thread, Arcadia_Object* self");
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)operation->parameters); i < n; ++i) {
    Arcadia_MILC_Symbol* parameter = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, operation->parameters, i, _Arcadia_MILC_Symbol_getType(thread));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8", ");
    Arcadia_MILC_Backend_Implementation_writeTypeName(thread, implementation, ((Arcadia_MILC_VariableSymbol*)parameter)->type, stringBuilder);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8" ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, parameter->name);
  }
}

/// Write the signature of a single interface operation to @c stringBuilder.
/// @todo The return value type of an operation is not resolved by the type resolution phase yet,
/// so @c void is emitted for every operation. See the return value type resolution in
/// `Arcadia_MILC_TypeResolutionPhase_MethodCompleter`.
static void
onWriteOperationSignature
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_MethodSymbol* operation,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"void (*");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, ((Arcadia_MILC_Symbol*)operation)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8")(");
  onWriteParameterList(thread, self, context, operation, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8");\n");
}

/// Write the prototype of a single interface operation to @c stringBuilder.
static void
onWriteOperationPrototype
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* interfaceSymbolInfo,
    Arcadia_MILC_MethodSymbol* operation,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"void ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, interfaceSymbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"_");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, ((Arcadia_MILC_Symbol*)operation)->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"(");
  onWriteParameterList(thread, self, context, operation, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8");\n");
}

static void
onWriteInterfaceSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_MILC_Backend_Implementation* implementation = Arcadia_MILC_Backend_Implementation_getInstance(thread, context);
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_defineInterfaceType(u8\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, symbolInfo->symbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  // The extended interface types, followed by the mandatory NULL terminator.
  Arcadia_List* extendedInterfaceSymbols = ((Arcadia_MILC_InterfaceSymbol*)symbolInfo->symbol)->extendedInterfaceSymbols;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)extendedInterfaceSymbols); i < n; ++i) {
    Arcadia_MILC_Symbol* extendedInterfaceSymbol = (Arcadia_MILC_Symbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, extendedInterfaceSymbols, i, _Arcadia_MILC_Symbol_getType(thread));
    Arcadia_MILC_Backend_SymbolInfo* extendedInterfaceSymbolInfo =
      Arcadia_MILC_Backend_Implementation_getOrCreateInfo(thread, implementation, extendedInterfaceSymbol);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                         ,\n                         _");
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, extendedInterfaceSymbolInfo));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_getType(thread)");
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n                         , NULL\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "                         )\n");
}

static void
onWriteInterfaceHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_MILC_Symbol* interfaceSymbol = symbolInfo->symbol;
  //
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Arcadia_declareInterfaceType(u8\"");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, interfaceSymbol->name);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\", ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, ")\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  // The dispatch of the interface type. The leading "Arcadia.InterfaceDispatch" is mandatory. The
  // operations of an interface type are independent of the operations of the interface types it
  // extends, so no ancestor dispatch is embedded here.
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "struct ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "Dispatch {\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  Arcadia_InterfaceDispatch parent;\n");
  Arcadia_List* operations = ((Arcadia_MILC_InterfaceSymbol*)interfaceSymbol)->operations;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)operations); i < n; ++i) {
    Arcadia_MILC_MethodSymbol* operation = (Arcadia_MILC_MethodSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, operations, i, _Arcadia_MILC_MethodSymbol_getType(thread));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "  ");
    onWriteOperationSignature(thread, self, context, operation, stringBuilder);
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "};\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
  //
  // The operations as they are invoked on a value of the interface type.
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)operations); i < n; ++i) {
    Arcadia_MILC_MethodSymbol* operation = (Arcadia_MILC_MethodSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, operations, i, _Arcadia_MILC_MethodSymbol_getType(thread));
    onWriteOperationPrototype(thread, self, context, symbolInfo, operation, stringBuilder);
  }
}

static void
onWriteSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
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

  onWriteInterfaceSourceFile(thread, self, context, symbolInfo, stringBuilder);
}

static void
onWriteHeaderFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
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
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#include \"Arcadia/Ring1/Include.h\"\n");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");

  onWriteInterfaceHeaderFile(thread, self, context, symbolInfo, stringBuilder);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "#endif // ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_MILC_Backend_SymbolInfo_getCxxNameUpperCase(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "_H_INCLUDED");
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, "\n");
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Backend_InterfaceSymbolWriter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_Backend_InterfaceSymbolWriter);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriterDispatch* self
  )
{
  ((Arcadia_MILC_Backend_SymbolWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_MILC_Backend_SymbolWriter*, Arcadia_MILC_Context*, Arcadia_MILC_Backend_CXXFileType, Arcadia_MILC_Backend_SymbolInfo*,Arcadia_StringBuilder*)) & Arcadia_MILC_Backend_InterfaceSymbolWriter_writeImpl;
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_Backend_InterfaceSymbolWriter_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_InterfaceSymbolWriter* self,
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

Arcadia_MILC_Backend_InterfaceSymbolWriter*
Arcadia_MILC_Backend_InterfaceSymbolWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Backend_InterfaceSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_Backend_InterfaceSymbolWriter);
}