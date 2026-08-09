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

#include <stdlib.h>

#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.SubStringTests/findFirstOccurrenceTests.h"
#include "Arcadia.Ring1.Tests.SubStringTests/findLastOccurrenceTests.h"
#include "Arcadia.Ring1.Tests.SubStringTests/SubStringTests.h"

int
main
  (
    int argc,
    char **argv
  )
{
  //
  if (!Arcadia_Tests_safeExecute(&findFirstOccurrenceTests1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&findFirstOccurrenceTests2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&findFirstOccurrenceTests3)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&findLastOccurrenceTests1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&findLastOccurrenceTests2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&findLastOccurrenceTests3)) {
    return EXIT_FAILURE;
  }
  //
  if (!Arcadia_Tests_safeExecute(&subStringTests1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&subStringTests2)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&subStringTests3)) {
    return EXIT_FAILURE;
  }
  //
  return EXIT_SUCCESS;
}
