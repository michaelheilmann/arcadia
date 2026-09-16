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

#if !defined(ARCADIA_MILC_TYPERESOLUTIONPHASE_H_INCLUDED)
#define ARCADIA_MILC_TYPERESOLUTIONPHASE_H_INCLUDED

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Symbols/ModuleSymbol.h"
#include "Arcadia/MILC/Completer.h"
typedef struct Arcadia_MILC_VariableSymbol Arcadia_MILC_VariableSymbol;

/// @brief The type resolution phase.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.MILC.TypeResolutionPhase.run.
/// @remarks
/// - for enumeration constant symbols determine the type of their initializer.
///   LANGUAGE DEFINITION: The initializer of an enumeration constant, if present, must be an integer literal.
///   An integer literal is a non-empty sequence of decimal digits. Its type is the first of
///   Arcadia.Natural8, Arcadia.Natural16, Arcadia.Natural32, and Arcadia.Natural64 which is able to represent its value.
Arcadia_declareObjectType(u8"Arcadia.MILC.TypeResolutionPhase", Arcadia_MILC_TypeResolutionPhase,
                          u8"Arcadia.Object");

struct Arcadia_MILC_TypeResolutionPhaseDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_TypeResolutionPhase {
  Arcadia_Object _parent;
  /// @brief The context.
  Arcadia_MILC_Context* context;
  //
  Arcadia_MILC_Completer* classCompleter;
  Arcadia_MILC_Completer* constructorCompleter;
  Arcadia_MILC_Completer* enumerationCompleter;
  Arcadia_MILC_Completer* methodCompleter;
  Arcadia_MILC_Completer* procedureCompleter;
};

Arcadia_MILC_TypeResolutionPhase*
Arcadia_MILC_TypeResolutionPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

Arcadia_MILC_TypeResolutionPhase*
Arcadia_MILC_TypeResolutionPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

void
Arcadia_MILC_TypeResolutionPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  );

void
Arcadia_MILC_TypeResolutionPhase_resolveParameter
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self,
    Arcadia_MILC_VariableSymbol* variableSymbol
  );

void
Arcadia_MILC_TypeResolutionPhase_resolveReturnValue
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_TypeResolutionPhase* self
  );

#endif // ARCADIA_MILC_TYPERESOLUTIONPHASE_H_INCLUDED
