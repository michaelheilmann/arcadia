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
#define RAY Arcadia_Math_Ray3Real32
#define PLANE Arcadia_Math_Plane3Real32
#define SPHERE Arcadia_Math_Sphere3Real32
#define AABB Arcadia_Math_AABB3Real32
#define R(name) Arcadia_Math_Ray3Real32_##name
#define P(name) Arcadia_Math_Plane3Real32_##name
#define S(name) Arcadia_Math_Sphere3Real32_##name
#define A(name) Arcadia_Math_AABB3Real32_##name
#define Test(name) real32_##name
#include "Cases.i"
#undef Scalar
#undef RAY
#undef PLANE
#undef SPHERE
#undef AABB
#undef R
#undef P
#undef S
#undef A
#undef Test

#define Scalar Arcadia_Real64Value
#define RAY Arcadia_Math_Ray3Real64
#define PLANE Arcadia_Math_Plane3Real64
#define SPHERE Arcadia_Math_Sphere3Real64
#define AABB Arcadia_Math_AABB3Real64
#define R(name) Arcadia_Math_Ray3Real64_##name
#define P(name) Arcadia_Math_Plane3Real64_##name
#define S(name) Arcadia_Math_Sphere3Real64_##name
#define A(name) Arcadia_Math_AABB3Real64_##name
#define Test(name) real64_##name
#include "Cases.i"
#undef Scalar
#undef RAY
#undef PLANE
#undef SPHERE
#undef AABB
#undef R
#undef P
#undef S
#undef A
#undef Test

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Real32RayValues", &real32_rayValues },
    { "Real32PlaneValues", &real32_planeValues },
    { "Real32SphereValues", &real32_sphereValues },
    { "Real32RayPlane", &real32_rayPlane },
    { "Real32RaySphere", &real32_raySphere },
    { "Real32RayAABB", &real32_rayAABB },
    { "Real32RayTriangle", &real32_rayTriangle },
    { "Real32OptionalOutputs", &real32_optionalOutputs },
    { "Real64RayValues", &real64_rayValues },
    { "Real64PlaneValues", &real64_planeValues },
    { "Real64SphereValues", &real64_sphereValues },
    { "Real64RayPlane", &real64_rayPlane },
    { "Real64RaySphere", &real64_raySphere },
    { "Real64RayAABB", &real64_rayAABB },
    { "Real64RayTriangle", &real64_rayTriangle },
    { "Real64OptionalOutputs", &real64_optionalOutputs },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
