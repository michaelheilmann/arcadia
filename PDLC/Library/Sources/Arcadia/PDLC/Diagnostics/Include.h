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

#if !defined(ARCADIA_PDLC_DIAGNOSTICS_INCLUDE_H_INCLUDED)
#define ARCADIA_PDLC_DIAGNOSTICS_INCLUDE_H_INCLUDED

#include "Arcadia/PDLC/Diagnostics/ConfigurationFileInvalidDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/CyclicInheritanceDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/CyclicInterfaceInheritanceDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/FileNotFoundDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/IntegerLiteralOutOfRangeDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/InterfaceOperationHasBodyDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/InvalidPathDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/MissingInitializerDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/MissingModuleDefinitionDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/MultipleModuleDefinitionsDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/SymbolIsAlreadyDefinedDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/SymbolIsNotDefinedDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/SymbolIsNoClassDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/SymbolIsNoInterfaceDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/SymbolIsNoMethodDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/Lexical/UnexpectedSymbolDiagnostic.h"
#include "Arcadia/PDLC/Diagnostics/Syntactical/UnexpectedWordDiagnostic.h"

#endif // ARCADIA_PDLC_DIAGNOSTICS_INCLUDE_H_INCLUDED
