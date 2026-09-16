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

#if !defined(ARCADIA_MILC_MEMBERENTERPHASE_PROCEDURECOMPLETER_H_INCLUDED)
#define ARCADIA_MILC_MEMBERENTERPHASE_PROCEDURECOMPLETER_H_INCLUDED

#include "Arcadia/MILC/Completer.h"

Arcadia_declareObjectType(u8"Arcadia.MILC.MemberEnterPhase.ProcedureCompleter", Arcadia_MILC_MemberEnterPhase_ProcedureCompleter,
                          Arcadia_MILC_Completer);

struct Arcadia_MILC_MemberEnterPhase_ProcedureCompleterDispatch {
  Arcadia_MILC_CompleterDispatch _parent;
};

struct Arcadia_MILC_MemberEnterPhase_ProcedureCompleter {
  Arcadia_MILC_Completer _parent;
};

Arcadia_MILC_MemberEnterPhase_ProcedureCompleter*
Arcadia_MILC_MemberEnterPhase_ProcedureCompleter_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_MILC_MEMBERENTERPHASE_PROCEDURECOMPLETER_H_INCLUDED
