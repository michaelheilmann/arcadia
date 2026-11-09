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
#include <stdbool.h>

#define TESTS(S) \
static int test##S(void) { \
  Arcadia_Math_Segment2Real##S a = {{0,0},{4,4}}; \
  Arcadia_Math_Segment2Real##S b = {{0,4},{4,0}}; \
  Arcadia_Math_SegmentIntersection2Real##S r; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Point || r.a[0] != 2 || r.a[1] != 2) return EXIT_FAILURE; \
  b.a[0]=5; b.a[1]=5; b.b[0]=6; b.b[1]=6; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_None) return EXIT_FAILURE; \
  b.a[0]=4; b.a[1]=4; b.b[0]=4; b.b[1]=8; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Point || r.a[0] != 4 || r.a[1] != 4) return EXIT_FAILURE; \
  b.a[0]=1; b.a[1]=1; b.b[0]=3; b.b[1]=3; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Overlap || r.a[0] != 1 || r.b[0] != 3) return EXIT_FAILURE; \
  b.a[0]=3; b.a[1]=3; b.b[0]=1; b.b[1]=1; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Overlap || r.a[0] != 1 || r.b[0] != 3) return EXIT_FAILURE; \
  a.b[0]=0; a.b[1]=0; b.a[0]=0; b.a[1]=0; b.b[0]=5; b.b[1]=5; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Point || r.a[0] != 0) return EXIT_FAILURE; \
  b.a[0]=8; b.a[1]=8; b.b[0]=8; b.b[1]=8; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_None) return EXIT_FAILURE; \
  a.a[0]=0; a.a[1]=0; a.b[0]=0; a.b[1]=6; \
  b.a[0]=-2; b.a[1]=3; b.b[0]=2; b.b[1]=3; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Point || r.a[0] != 0 || r.a[1] != 3) return EXIT_FAILURE; \
  b.a[0]=NAN; \
  r.a[0]=123; \
  if (Arcadia_Math_Segment2Real##S##_intersect(&a,&b,&r) != Arcadia_Math_SegmentIntersection2Kind_Invalid || r.a[0] != 123) return EXIT_FAILURE; \
  return EXIT_SUCCESS; \
}

#define S 32
#include "CasesSweep.i"
#include "CasesLegacyJava.i"
#undef S
#define S 64
#include "CasesSweep.i"
#include "CasesLegacyJava.i"
#undef S
TESTS(32)
TESTS(64)
#undef TESTS

int main(int argc, char** argv) {
  static const Arcadia_Testing_Case cases[] = {
    {"Real32SegmentIntersections", &test32},
    {"Real64SegmentIntersections", &test64},
    {"Real32SweepMultiway", &sweepMultiway32},
    {"Real64SweepMultiway", &sweepMultiway64},
    {"Real32SweepDegenerateAndOverlaps", &sweepDegenerate32},
    {"Real64SweepDegenerateAndOverlaps", &sweepDegenerate64},
    {"Real32SweepStopAndInvalid", &sweepStopInvalid32},
    {"Real64SweepStopAndInvalid", &sweepStopInvalid64},
    {"Real32SweepGridComparison", &sweepGrid32},
    {"Real64SweepGridComparison", &sweepGrid64},
    {"Real32SweepSeededOracle", &sweepSeededOracle32},
    {"Real64SweepSeededOracle", &sweepSeededOracle64},
    {"Real32SweepSeparatedLanes", &sweepSeparatedLanes32},
    {"Real64SweepSeparatedLanes", &sweepSeparatedLanes64},
    {"Real32SweepFirstHit", &sweepFirstHit32},
    {"Real64SweepFirstHit", &sweepFirstHit64},
    {"Real32SweepLargeSparse", &sweepLargeSparse32},
    {"Real64SweepLargeSparse", &sweepLargeSparse64},
    {"Real32SweepExpiry", &sweepExpiry32},
    {"Real64SweepExpiry", &sweepExpiry64},
    {"Real32SweepEventOracle", &sweepEventOracle32},
    {"Real32LegacyGeneric001", &legacyTest132},
    {"Real32LegacyGeneric002", &legacyTest232},
    {"Real32LegacyGeneric003", &legacyTest332},
    {"Real32LegacyGeneric004", &legacyTest432},
    {"Real32LegacyGeneric005", &legacyTest532},
    {"Real32LegacyGeneric006", &legacyTest632},
    {"Real32LegacyGeneric007", &legacyTest732},
    {"Real32LegacyGeneric008", &legacyTest832},
    {"Real32LegacyReversed", &legacyReversed32},
    {"Real64SweepEventOracle", &sweepEventOracle64},
    {"Real64LegacyGeneric001", &legacyTest164},
    {"Real64LegacyGeneric002", &legacyTest264},
    {"Real64LegacyGeneric003", &legacyTest364},
    {"Real64LegacyGeneric004", &legacyTest464},
    {"Real64LegacyGeneric005", &legacyTest564},
    {"Real64LegacyGeneric006", &legacyTest664},
    {"Real64LegacyGeneric007", &legacyTest764},
    {"Real64LegacyGeneric008", &legacyTest864},
    {"Real64LegacyReversed", &legacyReversed64},
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases)/sizeof(cases[0]));
}
