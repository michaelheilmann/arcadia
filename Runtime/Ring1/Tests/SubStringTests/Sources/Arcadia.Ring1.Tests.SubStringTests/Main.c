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

#include "Arcadia/Testing/Include.h"

#include <stdlib.h>

#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.SubStringTests/findFirstOccurrenceTests.h"
#include "Arcadia.Ring1.Tests.SubStringTests/findLastOccurrenceTests.h"
#include "Arcadia.Ring1.Tests.SubStringTests/SubStringTests.h"

static int
runFindFirstAbsent
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&findFirstOccurrenceTests1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFindFirstSingle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&findFirstOccurrenceTests2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFindFirstRepeated
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&findFirstOccurrenceTests3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFindLastAbsent
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&findLastOccurrenceTests1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFindLastSingle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&findLastOccurrenceTests2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFindLastRepeated
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&findLastOccurrenceTests3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSubstringRanges
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&subStringTests1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSubstringSlices
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&subStringTests2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSubstringInvalidRanges
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&subStringTests3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "FindFirstAbsent", &runFindFirstAbsent },
    { "FindFirstSingle", &runFindFirstSingle },
    { "FindFirstRepeated", &runFindFirstRepeated },
    { "FindLastAbsent", &runFindLastAbsent },
    { "FindLastSingle", &runFindLastSingle },
    { "FindLastRepeated", &runFindLastRepeated },
    { "SubstringRanges", &runSubstringRanges },
    { "SubstringSlices", &runSubstringSlices },
    { "SubstringInvalidRanges", &runSubstringInvalidRanges },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
