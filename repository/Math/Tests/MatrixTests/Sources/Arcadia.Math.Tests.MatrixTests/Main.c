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

#include "Arcadia/Math/Include.h"

#define Scalar Arcadia_Real32Value
#define Matrix Arcadia_Math_Matrix4Real32
#define Vector Arcadia_Math_Vector3Real32
#define M(name) Arcadia_Math_Matrix4Real32_##name
#define V(name) Arcadia_Math_Vector3Real32_##name
#define G(name) Arcadia_Math_Matrix4x4Real32_##name
#define Test(name) real32_##name
#define Tolerance 1.e-5
#include "Cases.i"
#undef Scalar
#undef Matrix
#undef Vector
#undef M
#undef V
#undef G
#undef Test
#undef Tolerance

#define Scalar Arcadia_Real64Value
#define Matrix Arcadia_Math_Matrix4Real64
#define Vector Arcadia_Math_Vector3Real64
#define M(name) Arcadia_Math_Matrix4Real64_##name
#define V(name) Arcadia_Math_Vector3Real64_##name
#define G(name) Arcadia_Math_Matrix4x4Real64_##name
#define Test(name) real64_##name
#define Tolerance 1.e-12
#include "Cases.i"
#undef Scalar
#undef Matrix
#undef Vector
#undef M
#undef V
#undef G
#undef Test
#undef Tolerance

static int
runReal32
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&real32_run) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runReal64
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&real64_run) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Real32", &runReal32 },
    { "Real64", &runReal64 },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
