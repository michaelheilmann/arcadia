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

#if !defined(ARCADIA_PDLC_INCLUDE_H_INCLUDED)
#define ARCADIA_PDLC_INCLUDE_H_INCLUDED

#include "Arcadia/Languages/Include.h"

#include "Arcadia/PDLC/Configure.h"
#include "Arcadia/PDLC/AST/Include.h"

#include "Arcadia/PDLC/FileType.h"

#include "Arcadia/PDLC/CompilationTask.h"
#include "Arcadia/PDLC/CompilationFailedException.h"

#include "Arcadia/PDLC/Context.h"

#include "Arcadia/PDLC/Symbols/Include.h"

#include "Arcadia/PDLC/Diagnostics/Include.h"

#include "Arcadia/PDLC/Keywords.h"
#include "Arcadia/PDLC/Parser.h"
#include "Arcadia/PDLC/Scanner.h"

#include "Arcadia/PDLC/EnterPhase.h"
#include "Arcadia/PDLC/MemberEnterPhase.h"
#include "Arcadia/PDLC/TypeResolutionPhase.h"

#include "Arcadia/PDLC/Symbols/Symbol.h"
#include "Arcadia/PDLC/Symbols/SymbolKind.h"

#endif // ARCADIA_PDLC_INCLUDE_H_INCLUDED
