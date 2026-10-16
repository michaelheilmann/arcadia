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

#if !defined(ARCADIA_PDLC_TYPERESOLUTIONPHASE_H_INCLUDED)
#define ARCADIA_PDLC_TYPERESOLUTIONPHASE_H_INCLUDED

#include "Arcadia/PDLC/Context.h"
#include "Arcadia/PDLC/AST/Include.h"
#include "Arcadia/PDLC/Symbols/ModuleSymbol.h"
#include "Arcadia/PDLC/Completer.h"
typedef struct Arcadia_PDLC_VariableSymbol Arcadia_PDLC_VariableSymbol;

/// @brief The type resolution phase.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.PDLC.TypeResolutionPhase.run.
/// @remarks
/// - for enumeration constant symbols determine the type of their initializer.
///   LANGUAGE DEFINITION: The initializer of an enumeration constant, if present, must be an integer literal.
///   An integer literal is a non-empty sequence of decimal digits. Its type is the first of
///   Arcadia.Natural8, Arcadia.Natural16, Arcadia.Natural32, and Arcadia.Natural64 which is able to represent its value.
Arcadia_declareObjectType(u8"Arcadia.PDLC.TypeResolutionPhase", Arcadia_PDLC_TypeResolutionPhase,
                          u8"Arcadia.Object");

struct Arcadia_PDLC_TypeResolutionPhaseDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_PDLC_TypeResolutionPhase {
  Arcadia_Object _parent;
  /// @brief The context.
  Arcadia_PDLC_Context* context;
  //
  Arcadia_PDLC_Completer* classCompleter;
  Arcadia_PDLC_Completer* constructorCompleter;
  Arcadia_PDLC_Completer* enumerationCompleter;
  Arcadia_PDLC_Completer* interfaceCompleter;
  Arcadia_PDLC_Completer* methodCompleter;
  Arcadia_PDLC_Completer* procedureCompleter;
};

Arcadia_PDLC_TypeResolutionPhase*
Arcadia_PDLC_TypeResolutionPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

Arcadia_PDLC_TypeResolutionPhase*
Arcadia_PDLC_TypeResolutionPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

void
Arcadia_PDLC_TypeResolutionPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self
  );

void
Arcadia_PDLC_TypeResolutionPhase_resolveParameter
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_VariableSymbol* variableSymbol
  );

/// Resolve the type of the return value of @c symbol.
/// @param symbol [in] the method or procedure symbol.
/// @remark Raises an error diagnostic if the return type name is not a defined type name.
void
Arcadia_PDLC_TypeResolutionPhase_resolveReturnValue
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_TypeResolutionPhase* self,
    Arcadia_PDLC_Symbol* symbol
  );

#endif // ARCADIA_PDLC_TYPERESOLUTIONPHASE_H_INCLUDED
