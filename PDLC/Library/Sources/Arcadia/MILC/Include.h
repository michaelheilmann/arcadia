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

#if !defined(ARCADIA_MILC_INCLUDE_H_INCLUDED)
#define ARCADIA_MILC_INCLUDE_H_INCLUDED

#include "Arcadia/Languages/Include.h"

#include "Arcadia/MILC/Configure.h"
#include "Arcadia/MILC/AST/Include.h"

#include "Arcadia/MILC/FileType.h"

#include "Arcadia/MILC/CompilationTask.h"
#include "Arcadia/MILC/CompilationFailedException.h"

#include "Arcadia/MILC/Context.h"

#include "Arcadia/MILC/Symbols/Include.h"

#include "Arcadia/MILC/Diagnostics/Include.h"

#include "Arcadia/MILC/Keywords.h"
#include "Arcadia/MILC/Parser.h"
#include "Arcadia/MILC/Scanner.h"

#include "Arcadia/MILC/EnterPhase.h"
#include "Arcadia/MILC/MemberEnterPhase.h"
#include "Arcadia/MILC/TypeResolutionPhase.h"

#include "Arcadia/MILC/Symbols/Symbol.h"
#include "Arcadia/MILC/Symbols/SymbolKind.h"

#endif // ARCADIA_MILC_INCLUDE_H_INCLUDED
