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

#define Scalar Arcadia_Real32Value
#define L(name) Arcadia_Math_Lagrange3Real32_##name
#define B(name) Arcadia_Math_Bezier3Real32_##name
#define Test(name) real32_##name
#include "Cases.i"
#undef Scalar
#undef L
#undef B
#undef Test

#define Scalar Arcadia_Real64Value
#define L(name) Arcadia_Math_Lagrange3Real64_##name
#define B(name) Arcadia_Math_Bezier3Real64_##name
#define Test(name) real64_##name
#include "Cases.i"
#undef Scalar
#undef L
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
    { "Real32UniformKnots", &real32_uniformKnots },
    { "Real32ChordKnots", &real32_chordKnots },
    { "Real32LagrangeLine", &real32_lagrangeLine },
    { "Real32LagrangePolynomial", &real32_lagrangePolynomial },
    { "Real32LagrangeInvalid", &real32_lagrangeInvalid },
    { "Real32BezierCubic", &real32_bezierCubic },
    { "Real32BezierDegenerate", &real32_bezierDegenerate },
    { "Real32BezierInvalid", &real32_bezierInvalid },
    { "Real64UniformKnots", &real64_uniformKnots },
    { "Real64ChordKnots", &real64_chordKnots },
    { "Real64LagrangeLine", &real64_lagrangeLine },
    { "Real64LagrangePolynomial", &real64_lagrangePolynomial },
    { "Real64LagrangeInvalid", &real64_lagrangeInvalid },
    { "Real64BezierCubic", &real64_bezierCubic },
    { "Real64BezierDegenerate", &real64_bezierDegenerate },
    { "Real64BezierInvalid", &real64_bezierInvalid },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
