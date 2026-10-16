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

#if !defined(ARCADIA_PDLC_BACKEND_CLASSSYMBOLWRITER_H_INCLUDED)
#define ARCADIA_PDLC_BACKEND_CLASSSYMBOLWRITER_H_INCLUDED

#include "Arcadia/PDLC/Backend/SymbolWriter.h"

/// @brief A writer for a class symbol.
Arcadia_declareObjectType(u8"Arcadia.PDLC.Backend.ClassSymbolWriter", Arcadia_PDLC_Backend_ClassSymbolWriter,
                          u8"Arcadia.PDLC.Backend.SymbolWriter");

struct Arcadia_PDLC_Backend_ClassSymbolWriterDispatch {
  Arcadia_PDLC_Backend_SymbolWriterDispatch _parent;
};

struct Arcadia_PDLC_Backend_ClassSymbolWriter {
  Arcadia_PDLC_Backend_SymbolWriter _parent;
};

Arcadia_PDLC_Backend_ClassSymbolWriter*
Arcadia_PDLC_Backend_ClassSymbolWriter_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PDLC_BACKEND_CLASSSYMBOLWRITER_H_INCLUDED
