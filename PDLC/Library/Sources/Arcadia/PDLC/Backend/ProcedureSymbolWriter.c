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
#include "Arcadia/PDLC/Backend/CallableWriter.h"
#include "Arcadia/PDLC/Backend/SymbolInfo.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ProcedureSymbolWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_Backend_ProcedureSymbolWriter);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_Backend_ProcedureSymbolWriter);
}

static void
writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_SymbolWriter* self,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Backend_CXXFileType fileType,
    Arcadia_PDLC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* output
  )
{
  Arcadia_StringBuilder_clear(thread, output);
  Arcadia_PDLC_Backend_writeExecutable(thread, context, symbolInfo->symbol, fileType == Arcadia_PDLC_Backend_CXXFileType_H, output);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Backend_ProcedureSymbolWriterDispatch* self
  )
{ self->_parent.write = writeImpl; }

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.Backend.ProcedureSymbolWriter", Arcadia_PDLC_Backend_ProcedureSymbolWriter,
                        u8"Arcadia.PDLC.Backend.SymbolWriter", Arcadia_PDLC_Backend_SymbolWriter,
                        &typeOperations);

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
