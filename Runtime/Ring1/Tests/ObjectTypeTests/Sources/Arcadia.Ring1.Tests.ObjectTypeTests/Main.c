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

#include "Arcadia.Ring1.Tests.ObjectTypeTests/MyObjectA.h"
#include "Arcadia.Ring1.Tests.ObjectTypeTests/MyObjectB.h"

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_MyObjectB* o = Arcadia_MyObjectB_create(thread);
  Arcadia_Type const* t = Arcadia_Object_getType(thread, (Arcadia_Object*)o);
  if (t != _Arcadia_MyObjectB_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
  }
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ObjectTypeTests", &runTests },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
