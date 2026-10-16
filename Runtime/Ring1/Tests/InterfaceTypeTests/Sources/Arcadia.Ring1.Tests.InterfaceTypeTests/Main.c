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
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceTypes.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceOperation.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceOperationOverride.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceImplementationMustBeClosedUnderAncestors.h"

static int
runInterfaceTypes
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterfaceTypes) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runInterfaceOperation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterfaceOperation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runInterfaceOperationOverride
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterfaceOperationOverride) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAncestorClosure
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterfaceImplementationMustBeClosedUnderAncestors) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "InterfaceTypes", &runInterfaceTypes },
    { "InterfaceOperation", &runInterfaceOperation },
    { "InterfaceOperationOverride", &runInterfaceOperationOverride },
    { "AncestorClosure", &runAncestorClosure },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
