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

#if !defined(ARCADIA_MILC_DIAGNOSTICS_INCLUDE_H_INCLUDED)
#define ARCADIA_MILC_DIAGNOSTICS_INCLUDE_H_INCLUDED

#include "Arcadia/MILC/Diagnostics/ConfigurationFileInvalidDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/CyclicInheritanceDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/FileNotFoundDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/IntegerLiteralOutOfRangeDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/InvalidPathDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/MissingInitializerDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/MissingModuleDefinitionDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/MultipleModuleDefinitionsDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/SymbolIsAlreadyDefinedDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/SymbolIsNotDefinedDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/SymbolIsNoClassDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/SymbolIsNoMethodDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/Lexical/UnexpectedSymbolDiagnostic.h"
#include "Arcadia/MILC/Diagnostics/Syntactical/UnexpectedWordDiagnostic.h"

#endif // ARCADIA_MILC_DIAGNOSTICS_INCLUDE_H_INCLUDED
