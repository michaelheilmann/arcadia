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

#if !defined(ARCADIA_DDLS_SYMBOLS_SCOPE_H_INCLUDED)
#define ARCADIA_DDLS_SYMBOLS_SCOPE_H_INCLUDED

#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_DDLS_Symbol Arcadia_DDLS_Symbol;

/* Map from names to symbols. An error symbol e replace conflicting symbols e1 and e2. */
Arcadia_declareObjectType(u8"Arcadia.DDLS.Scope", Arcadia_DDLS_Scope,
                          u8"Arcadia.Object");

struct Arcadia_DDLS_ScopeDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_DDLS_Scope {
  Arcadia_Object parent;
  /* The enclosing scope or null. */
  Arcadia_DDLS_Scope* enclosing;
  /* Name to symbol map. */
  Arcadia_Map* symbols;
};

Arcadia_DDLS_Scope*
Arcadia_DDLS_Scope_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Scope* enclosing
  );

#endif // ARCADIA_DDLS_SYMBOLS_SCOPE_H_INCLUDED
