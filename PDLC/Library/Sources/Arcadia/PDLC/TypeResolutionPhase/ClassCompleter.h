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

#if !defined(ARCADIA_PDLC_TYPERESOLUTIONPHASE_CLASSCOMPLETER_H_INCLUDED)
#define ARCADIA_PDLC_TYPERESOLUTIONPHASE_CLASSCOMPLETER_H_INCLUDED

#include "Arcadia/PDLC/Completer.h"
#include "Arcadia/Collections/Include.h"

Arcadia_declareObjectType(u8"Arcadia.PDLC.TypeResolutionPhase.ClassCompleter", Arcadia_PDLC_TypeResolutionPhase_ClassCompleter,
                          Arcadia_PDLC_Completer);

struct Arcadia_PDLC_TypeResolutionPhase_ClassCompleterDispatch {
  Arcadia_PDLC_CompleterDispatch _parent;
};

struct Arcadia_PDLC_TypeResolutionPhase_ClassCompleter {
  Arcadia_PDLC_Completer _parent;
  // A hash map used for checking for cycles.
  Arcadia_Map* cycleCheck;
};

Arcadia_PDLC_TypeResolutionPhase_ClassCompleter*
Arcadia_PDLC_TypeResolutionPhase_ClassCompleter_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_PDLC_TYPERESOLUTIONPHASE_CLASSCOMPLETER_H_INCLUDED
