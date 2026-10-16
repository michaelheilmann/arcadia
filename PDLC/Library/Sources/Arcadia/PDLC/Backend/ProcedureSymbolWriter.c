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

#include "Arcadia/PDLC/Backend/ProcedureSymbolWriter.h"

#include "Arcadia/PDLC/Include.h"
#include "Arcadia/PDLC/Backend/Implementation.h"

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
  "// along with this program. If not, see <https://www.gnu.org/licenses/>.\n";

static void
writeSignature
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_PDLC_Backend_Implementation* implementation = Arcadia_PDLC_Backend_Implementation_getInstance(thread, context);
  Arcadia_PDLC_ProcedureSymbol* procedureSymbol = (Arcadia_PDLC_ProcedureSymbol*)symbolInfo->symbol;
  Arcadia_PDLC_Backend_Implementation_writeTypeName(thread, implementation, procedureSymbol->returnValueType, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8" ");
  Arcadia_StringBuilder_insertBackString(thread, stringBuilder, Arcadia_PDLC_Backend_SymbolInfo_getCxxName(thread, symbolInfo));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"(Arcadia_Thread* thread");
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)procedureSymbol->parameters); i < n; ++i) {
    Arcadia_PDLC_VariableSymbol* parameter = (Arcadia_PDLC_VariableSymbol*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, procedureSymbol->parameters, i, _Arcadia_PDLC_VariableSymbol_getType(thread));
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8", ");
    Arcadia_PDLC_Backend_Implementation_writeTypeName(thread, implementation, parameter->type, stringBuilder);
    Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8" ");
    Arcadia_StringBuilder_insertBackString(thread, stringBuilder, ((Arcadia_PDLC_Symbol*)parameter)->name);
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8")");
}

static void
writeHeader
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ProcedureSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"\n#include \"Arcadia/Ring1/Include.h\"\n\n");
  writeSignature(thread, context, symbolInfo, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8";\n");
}

static void
writeSource
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ProcedureSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  Arcadia_StringBuilder_clear(thread, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, COPYRIGHT);
  Arcadia_PDLC_ProcedureSymbol* procedureSymbol = (Arcadia_PDLC_ProcedureSymbol*)symbolInfo->symbol;
  if (procedureSymbol->ast && procedureSymbol->ast->nativeName) {
    return;
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"\n#include \"Arcadia/Ring1/Include.h\"\n\n");
  writeSignature(thread, context, symbolInfo, stringBuilder);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuilder, u8"\n{\n  Arcadia_Thread_setStatus(thread, Arcadia_Status_NotImplemented);\n  Arcadia_Thread_jump(thread);\n}\n");
}

static void constructImpl(Arcadia_Thread* thread, Arcadia_PDLC_Backend_ProcedureSymbolWriter* self)
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Backend_ProcedureSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_Backend_ProcedureSymbolWriter);
}

static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_PDLC_Backend_ProcedureSymbolWriterDispatch* self);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.Backend.ProcedureSymbolWriter", Arcadia_PDLC_Backend_ProcedureSymbolWriter,
                         u8"Arcadia.PDLC.Backend.SymbolWriter", Arcadia_PDLC_Backend_SymbolWriter,
                         &_typeOperations);

static void
writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ProcedureSymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_CXXFileType fileType,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  )
{
  switch (fileType) {
    case Arcadia_PDLC_Backend_CXXFileType_C: writeSource(thread, self, context, symbolInfo, stringBuilder); break;
    case Arcadia_PDLC_Backend_CXXFileType_H: writeHeader(thread, self, context, symbolInfo, stringBuilder); break;
  };
}

static void initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_PDLC_Backend_ProcedureSymbolWriterDispatch* self)
{
  ((Arcadia_PDLC_Backend_SymbolWriterDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_PDLC_Backend_SymbolWriter*, Arcadia_PDLC_Context*, Arcadia_PDLC_Backend_CXXFileType, Arcadia_PDLC_Backend_SymbolInfo*, Arcadia_StringBuilder*)) & writeImpl;
}

Arcadia_PDLC_Backend_ProcedureSymbolWriter*
Arcadia_PDLC_Backend_ProcedureSymbolWriter_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_Backend_ProcedureSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_PDLC_Backend_ProcedureSymbolWriter);
}
