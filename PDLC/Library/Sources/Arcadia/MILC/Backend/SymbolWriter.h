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

#if !defined(ARCADIA_MILC_BACKEND_SYMBOLWRITER_H_INCLUDED)
#define ARCADIA_MILC_BACKEND_SYMBOLWRITER_H_INCLUDED

#include "Arcadia/MILC/Backend/SymbolInfo.h"
#include "Arcadia/MILC/Backend/CXXFileType.h"

/// @brief A writer for symbols.
Arcadia_declareObjectType(u8"Arcadia.MILC.Backend.SymbolWriter", Arcadia_MILC_Backend_SymbolWriter,
                          u8"Arcadia.Object");

struct Arcadia_MILC_Backend_SymbolWriterDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*write)(Arcadia_Thread* thread, Arcadia_MILC_Backend_SymbolWriter* self, Arcadia_MILC_Context* context, Arcadia_MILC_Backend_CXXFileType fileType,
                Arcadia_MILC_Backend_SymbolInfo* symbolInfo, Arcadia_StringBuilder* stringBuilder);
};

struct Arcadia_MILC_Backend_SymbolWriter {
  Arcadia_Object _parent;
};

Arcadia_MILC_Backend_SymbolWriter*
Arcadia_MILC_Backend_SymbolWriter_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_MILC_Backend_SymbolWriter_write
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Backend_SymbolWriter* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Backend_CXXFileType fileType,
    Arcadia_MILC_Backend_SymbolInfo* symbolInfo,
    Arcadia_StringBuilder* stringBuilder
  );

#endif // ARCADIA_MILC_BACKEND_SYMBOLWRITER_H_INCLUDED
