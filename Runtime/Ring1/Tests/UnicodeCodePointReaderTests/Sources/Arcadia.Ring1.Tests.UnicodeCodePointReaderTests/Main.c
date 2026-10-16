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
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.UnicodeCodePointReaderTests/StandardTests.h"

static int
runEmptyInput
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_UnicodeCodePointReader_test1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runValidEncodings
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_UnicodeCodePointReader_test2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runOverlongEncodings
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&Arcadia_Ring1_Tests_UnicodeCodePointReader_test3) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "EmptyInput", &runEmptyInput },
    { "ValidEncodings", &runValidEncodings },
    { "OverlongEncodings", &runOverlongEncodings },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
