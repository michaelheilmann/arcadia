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

#if !defined(ARCADIA_MILC_COMPLETER_H_INCLUDED)
#define ARCADIA_MILC_COMPLETER_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_MILC_Symbol Arcadia_MILC_Symbol;
typedef struct Arcadia_MILC_Context Arcadia_MILC_Context;

Arcadia_declareObjectType(u8"Arcadia.MILC.Completer", Arcadia_MILC_Completer,
                          Arcadia_Object);

struct Arcadia_MILC_CompleterDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*complete)(Arcadia_Thread* thread, Arcadia_MILC_Completer* self, Arcadia_MILC_Context* context, Arcadia_MILC_Symbol* symbol);
};

struct Arcadia_MILC_Completer {
  Arcadia_Object _parent;
};

void
Arcadia_MILC_Completer_complete
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Completer* self,
    Arcadia_MILC_Context* context,
    Arcadia_MILC_Symbol* symbol
  );

#endif // ARCADIA_MILC_COMPLETER_H_INCLUDED
