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
#include <stdlib.h>
#include <math.h>

#define Scalar Arcadia_Real32Value
#define N(name) Arcadia_Math_NURBS3Real32_##name
#define Case(name) real32_##name
#define Epsilon 0.00002
#include "Cases.i"
#undef Scalar
#undef N
#undef Case
#undef Epsilon

#define Scalar Arcadia_Real64Value
#define N(name) Arcadia_Math_NURBS3Real64_##name
#define Case(name) real64_##name
#define Epsilon 0.000000000002
#include "Cases.i"
#undef Scalar
#undef N
#undef Case
#undef Epsilon

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Real32OpenUniform", &real32_openUniform },
    { "Real32CurveEndpoints", &real32_curveEndpoints },
    { "Real32CurveRational", &real32_curveRational },
    { "Real32CurveRepeated", &real32_curveRepeated },
    { "Real32CurveDegreeZero", &real32_curveDegreeZero },
    { "Real32CurveInvalid", &real32_curveInvalid },
    { "Real32SurfaceBilinear", &real32_surfaceBilinear },
    { "Real32SurfaceRectangular", &real32_surfaceRectangular },
    { "Real32SurfaceRational", &real32_surfaceRational },
    { "Real32SurfaceDegreeZero", &real32_surfaceDegreeZero },
    { "Real32SurfaceConstant", &real32_surfaceConstant },
    { "Real32SurfaceInvalid", &real32_surfaceInvalid },
    { "Real64OpenUniform", &real64_openUniform },
    { "Real64CurveEndpoints", &real64_curveEndpoints },
    { "Real64CurveRational", &real64_curveRational },
    { "Real64CurveRepeated", &real64_curveRepeated },
    { "Real64CurveDegreeZero", &real64_curveDegreeZero },
    { "Real64CurveInvalid", &real64_curveInvalid },
    { "Real64SurfaceBilinear", &real64_surfaceBilinear },
    { "Real64SurfaceRectangular", &real64_surfaceRectangular },
    { "Real64SurfaceRational", &real64_surfaceRational },
    { "Real64SurfaceDegreeZero", &real64_surfaceDegreeZero },
    { "Real64SurfaceConstant", &real64_surfaceConstant },
    { "Real64SurfaceInvalid", &real64_surfaceInvalid },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
