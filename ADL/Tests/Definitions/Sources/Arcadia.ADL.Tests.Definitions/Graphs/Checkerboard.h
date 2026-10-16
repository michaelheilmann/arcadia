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

#if !defined(ARCADIA_ADL_TESTS_DEFINITIONS_GRAPHS_CHECKERBOARD_H_INCLUDED)
#define ARCADIA_ADL_TESTS_DEFINITIONS_GRAPHS_CHECKERBOARD_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

/// @brief Expand a checkerboard graph with two instances and verify the generated definitions.
void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest1
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that the pixel bytes produced from a graph-expanded pixel buffer match an equivalent
/// hand-written definition.
void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest2
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that an instance with a missing binding is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest3
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that an instance with a duplicate binding is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest4
  (
    Arcadia_Thread* thread
  );

/// @brief Verify that a graph template without a `Graph.Output` node is rejected.
void
Arcadia_ADL_Tests_Definitions_Graphs_CheckerboardTest5
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ADL_TESTS_DEFINITIONS_GRAPHS_CHECKERBOARD_H_INCLUDED
