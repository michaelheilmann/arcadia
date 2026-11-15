// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"

#include <stdlib.h>

#define Scalar Arcadia_Real32Value
#define Matrix Arcadia_Math_Matrix3Real32
#define Box Arcadia_Math_AABB3Real32
#define M(name) Arcadia_Math_Matrix3Real32_##name
#define B(name) Arcadia_Math_AABB3Real32_##name
#define Test(name) real32_##name
#include "Cases.i"
#undef Scalar
#undef Matrix
#undef Box
#undef M
#undef B
#undef Test

#define Scalar Arcadia_Real64Value
#define Matrix Arcadia_Math_Matrix3Real64
#define Box Arcadia_Math_AABB3Real64
#define M(name) Arcadia_Math_Matrix3Real64_##name
#define B(name) Arcadia_Math_AABB3Real64_##name
#define Test(name) real64_##name
#include "Cases.i"
#undef Scalar
#undef Matrix
#undef Box
#undef M
#undef B
#undef Test

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "MatrixReal32Arithmetic", &real32_matrixArithmetic },
    { "MatrixReal32Transform", &real32_matrixTransform },
    { "AABBReal32Union", &real32_aabbUnion },
    { "AABBReal32Intersection", &real32_aabbIntersection },
    { "MatrixReal64Arithmetic", &real64_matrixArithmetic },
    { "MatrixReal64Transform", &real64_matrixTransform },
    { "AABBReal64Union", &real64_aabbUnion },
    { "AABBReal64Intersection", &real64_aabbIntersection },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
