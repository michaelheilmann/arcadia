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

#include "Arcadia.Ring1.Tests.StringToReal/Tests.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"

// 1e1 and 1E10 are converted to 10.
void
regressionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Real64Value target;

  target = Arcadia_String_toReal64(thread, Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"1e1", strlen(u8"1e1"))));
  Arcadia_Tests_assertTrue(thread, 10.0 == target);
  target = Arcadia_String_toReal64(thread, Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"1E1", strlen(u8"1E1"))));
  Arcadia_Tests_assertTrue(thread, 10.0 == target);
}

// 1e0, 1E0 yielded and incorrect value.
void
regressionTest2
  (
    Arcadia_Thread* thread  
  )
{ 
  Arcadia_Real64Value target; 
  target = Arcadia_String_toReal64(thread, Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"1e0", strlen(u8"1e0"))));
  Arcadia_Tests_assertTrue(thread, 1.0 == target);
  target = Arcadia_String_toReal64(thread, Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"1E0", strlen(u8"1E0"))));
  Arcadia_Tests_assertTrue(thread, 1.0 == target);
}


static int
runSafeRange
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&safeRangeTests) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runConversion
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runExponentOne
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&regressionTest1) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runExponentZero
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&regressionTest2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "SafeRange", &runSafeRange },
    { "Conversion", &runConversion },
    { "ExponentOne", &runExponentOne },
    { "ExponentZero", &runExponentZero },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
