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

#ifndef ARCADIA_ADL_TESTS_DEFINITIONS_GRAPHS_SINE_H_INCLUDED
#define ARCADIA_ADL_TESTS_DEFINITIONS_GRAPHS_SINE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// @brief Expand a single-sine graph with a typed `Integer32` frequency parameter and verify the
/// generated definitions and the published output.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest1
  (
    Arcadia_Thread* thread
  );

/// @brief Expand a single-sine graph template twice with different frequency bindings and verify
/// the two instances expand independently and identically to hand-written equivalents.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest2
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that a value-typed parameter bound with a string is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest3
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that a `Graph.Parameter` declaring both `referenceType` and `valueType` is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest4
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that a value-typed parameter bound with a fractional value is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest5
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that a negative frequency value is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest6
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that a missing binding for a value parameter is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest7
  (
    Arcadia_Thread* thread
  );

/// @brief Verify independent canonical expansion across declaration/load permutations.
void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest8
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ADL_TESTS_DEFINITIONS_GRAPHS_SINE_H_INCLUDED
