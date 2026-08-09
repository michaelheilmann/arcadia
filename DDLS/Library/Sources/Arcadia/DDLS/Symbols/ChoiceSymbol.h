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

#if !defined(ARCADIA_DDLS_SYMBOLS_CHOICESYMBOL_H_INCLUDED)
#define ARCADIA_DDLS_SYMBOLS_CHOICESYMBOL_H_INCLUDED

#include "Arcadia/DDLS/Symbols/Symbol.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareObjectType(u8"Arcadia.DDLS.ChoiceSymbol", Arcadia_DDLS_ChoiceSymbol,
                          u8"Arcadia.DDLS.Symbol");

struct Arcadia_DDLS_ChoiceSymbolDispatch {
  Arcadia_DDLS_SymbolDispatch parent;
};

struct Arcadia_DDLS_ChoiceSymbol {
  Arcadia_DDLS_Symbol parent;
  /// @brief The choices.
  Arcadia_List* choices;
};

Arcadia_DDLS_ChoiceSymbol*
Arcadia_DDLS_ChoiceSymbol_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_DDLS_SYMBOLS_CHOICESYMBOL_H_INCLUDED
