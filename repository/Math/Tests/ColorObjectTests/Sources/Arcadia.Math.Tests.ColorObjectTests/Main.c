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

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"

#include <math.h>
#include <stdlib.h>

#define Scalar Arcadia_Real32Value
#define Color Arcadia_Math_Color4Real32
#define C(name) Arcadia_Math_Color4Real32_##name
#define Test(name) real32_##name
#define Tolerance 1.e-5
#include "Cases.i"
#undef Scalar
#undef Color
#undef C
#undef Test
#undef Tolerance

#define Scalar Arcadia_Real64Value
#define Color Arcadia_Math_Color4Real64
#define C(name) Arcadia_Math_Color4Real64_##name
#define Test(name) real64_##name
#define Tolerance 1.e-12
#include "Cases.i"
#undef Scalar
#undef Color
#undef C
#undef Test
#undef Tolerance

#define CASE(precision, name) \
  static int precision##_##name##_case(void) { \
    return Arcadia_Tests_safeExecute(&precision##_##name) ? EXIT_SUCCESS : EXIT_FAILURE; \
  }
CASE(real32, construction)
CASE(real64, construction)
CASE(real32, arithmetic)
CASE(real64, arithmetic)
CASE(real32, nativeValuesAndClone)
CASE(real64, nativeValuesAndClone)
#undef CASE

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ConstructionReal32", &real32_construction_case },
    { "ConstructionReal64", &real64_construction_case },
    { "ArithmeticReal32", &real32_arithmetic_case },
    { "ArithmeticReal64", &real64_arithmetic_case },
    { "NativeValuesAndCloneReal32", &real32_nativeValuesAndClone_case },
    { "NativeValuesAndCloneReal64", &real64_nativeValuesAndClone_case },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
