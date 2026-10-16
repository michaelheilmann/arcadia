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

#if !defined(ARCADIA_PDLC_SYMBOLS_SYMBOLKIND_H_INCLUDED)
#define ARCADIA_PDLC_SYMBOLS_SYMBOLKIND_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.PDLC.SymbolKind", Arcadia_PDLC_SymbolKind);

enum Arcadia_PDLC_SymbolKind {
  Arcadia_PDLC_SymbolKind_Class,
  Arcadia_PDLC_SymbolKind_Constructor,
  Arcadia_PDLC_SymbolKind_Enumeration,
  Arcadia_PDLC_SymbolKind_EnumerationConstant,
  Arcadia_PDLC_SymbolKind_Interface,
  Arcadia_PDLC_SymbolKind_Namespace,
  Arcadia_PDLC_SymbolKind_Method,
  Arcadia_PDLC_SymbolKind_Module,
  Arcadia_PDLC_SymbolKind_PrimitiveType,
  Arcadia_PDLC_SymbolKind_Procedure,
  Arcadia_PDLC_SymbolKind_Variable,
};

Arcadia_String*
Arcadia_PDLC_SymbolKind_toString
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_SymbolKind kind
  );

#endif // ARCADIA_PDLC_SYMBOLS_SYMBOLKIND_H_INCLUDED
