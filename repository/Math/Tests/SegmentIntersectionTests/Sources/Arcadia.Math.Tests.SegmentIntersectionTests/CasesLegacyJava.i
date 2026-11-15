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

// Historical source: toport/main@ec0dbc6a3f:
// language_infrastructure_lib/java/testdata/generic001.seg ... generic008.seg.
// Preserve their *segment coordinates*, not the broken Java file parser.
// The README's "six true intersections" for generic005 counts a different
// notion of a "proper" hit; Arcadia enumerates all nine segment-pair hits.
// generic003 has six intersecting pairs, including three at (40/9,40/9).

#define LC1(a,b) a##b
#define LC(a,b) LC1(a,b)
#define LN(name) LC(name,S)
#define LSEG LN(Arcadia_Math_Segment2Real)
#define LEVENT LN(Arcadia_Math_SegmentSweep2EventReal)
#define LRESULT LN(Arcadia_Math_SegmentIntersection2Real)
#define LINTERSECT LC(LSEG,_intersect)
#define LENUMB LC(LSEG,_enumerateIntersections)

static const int LN(legacy001)[][4] = {
  {1,1,4,4}, {1,4,4,1}
};
static const int LN(legacy002)[][4] = {
  {2,2,4,4}, {2,1,4,3}
};
static const int LN(legacy003)[][4] = {
  {0,0,10,10}, {0,10,10,0}, {4,0,5,10}, {0,4,10,5}
};
static const int LN(legacy004)[][4] = {
  {5,0,5,10}, {0,5,10,5}
};
static const int LN(legacy005)[][4] = {
  {1,0,1,10}, {2,0,2,10}, {3,0,3,10}, {4,0,4,10},
  {5,0,5,10}, {6,0,6,10}, {7,0,7,10}, {8,0,8,10},
  {9,0,9,10}, {0,5,10,5}
};
static const int LN(legacy006)[][4] = {
  {1,1,1,9}, {9,1,9,9}, {1,1,9,1}, {1,9,9,9}
};
static const int LN(legacy007)[][4] = {
  {1,1,1,9}, {1,1,1,9}
};
static const int LN(legacy008)[][4] = {
  {1,5,3,5}, {2,5,4,5}
};

typedef struct LN(LegacyAudit) {
  const LSEG* segments;
  size_t numberOfSegments;
  size_t delivered;
  unsigned char seen[10][10];
  int valid;
  LEVENT previous;
} LN(LegacyAudit);

static Arcadia_BooleanValue
LN(legacyCollect)
  (
    void* context,
    const LEVENT* event
  )
{
  LN(LegacyAudit)* audit = context;
  const size_t i = event->firstIndex;
  const size_t j = event->secondIndex;
  if (i >= audit->numberOfSegments || j >= audit->numberOfSegments ||
      i >= j || audit->seen[i][j]) {
    audit->valid = 0;
    return true;
  }
  audit->seen[i][j] = 1;
  LRESULT expected;
  const Arcadia_Math_SegmentIntersection2Kind kind =
      LINTERSECT(audit->segments + i, audit->segments + j, &expected);
  if (kind != event->intersection.kind ||
      kind == Arcadia_Math_SegmentIntersection2Kind_None ||
      kind == Arcadia_Math_SegmentIntersection2Kind_Invalid ||
      expected.a[0] != event->intersection.a[0] ||
      expected.a[1] != event->intersection.a[1] ||
      expected.b[0] != event->intersection.b[0] ||
      expected.b[1] != event->intersection.b[1]) audit->valid = 0;
  if (audit->delivered) {
    const LEVENT* old = &audit->previous;
    if (old->intersection.a[0] > event->intersection.a[0] ||
        (old->intersection.a[0] == event->intersection.a[0] &&
         old->intersection.a[1] > event->intersection.a[1]) ||
        (old->intersection.a[0] == event->intersection.a[0] &&
         old->intersection.a[1] == event->intersection.a[1] &&
         (old->firstIndex > i ||
          (old->firstIndex == i && old->secondIndex >= j)))) audit->valid = 0;
  }
  audit->previous = *event;
  ++audit->delivered;
  return true;
}

static int
LN(legacyFixture)
  (
    unsigned fixture,
    int reverseEndpoints
  )
{
  const int (*source)[4] = NULL;
  size_t length = 0;
  size_t expected = 0;
  switch (fixture) {
    case 1: source = LN(legacy001); length = 2; expected = 1; break;
    case 2: source = LN(legacy002); length = 2; expected = 0; break;
    case 3: source = LN(legacy003); length = 4; expected = 6; break;
    case 4: source = LN(legacy004); length = 2; expected = 1; break;
    case 5: source = LN(legacy005); length = 10; expected = 9; break;
    case 6: source = LN(legacy006); length = 4; expected = 4; break;
    case 7: source = LN(legacy007); length = 2; expected = 1; break;
    case 8: source = LN(legacy008); length = 2; expected = 1; break;
    default: return EXIT_FAILURE;
  }
  LSEG segments[10];
  for (size_t i = 0; i < length; ++i) {
    const unsigned first = reverseEndpoints ? 2 : 0;
    const unsigned second = reverseEndpoints ? 0 : 2;
    for (size_t axis = 0; axis < 2; ++axis) {
      segments[i].a[axis] = source[i][first + axis];
      segments[i].b[axis] = source[i][second + axis];
    }
  }
  LN(LegacyAudit) audit = {0};
  audit.segments = segments;
  audit.numberOfSegments = length;
  audit.valid = 1;
  size_t actual = SIZE_MAX;
  if (LENUMB(segments, length, LN(legacyCollect), &audit, &actual) !=
      Arcadia_Math_SegmentSweep2Status_Completed || !audit.valid ||
      actual != expected || audit.delivered != actual) return EXIT_FAILURE;
  for (size_t i = 0; i < length; ++i) {
    for (size_t j = i + 1; j < length; ++j) {
      const Arcadia_Math_SegmentIntersection2Kind kind =
          LINTERSECT(segments + i, segments + j, NULL);
      if ((kind != Arcadia_Math_SegmentIntersection2Kind_None) !=
          (audit.seen[i][j] != 0)) return EXIT_FAILURE;
    }
  }
  size_t countOnly = SIZE_MAX;
  if (LENUMB(segments, length, NULL, NULL, &countOnly) !=
      Arcadia_Math_SegmentSweep2Status_Completed || countOnly != expected)
    return EXIT_FAILURE;
  // Explicit legacy edge cases are independent of the pairwise oracle.
  if (fixture == 1) {
    const LRESULT* r = &audit.previous.intersection;
    if (r->kind != Arcadia_Math_SegmentIntersection2Kind_Point ||
        r->a[0] != 2.5 || r->a[1] != 2.5) return EXIT_FAILURE;
  }
  if (fixture == 4) {
    const LRESULT* r = &audit.previous.intersection;
    if (r->kind != Arcadia_Math_SegmentIntersection2Kind_Point ||
        r->a[0] != 5 || r->a[1] != 5) return EXIT_FAILURE;
  }
  if (fixture == 7 || fixture == 8) {
    const LRESULT* r = &audit.previous.intersection;
    if (r->kind != Arcadia_Math_SegmentIntersection2Kind_Overlap ||
        r->a[0] != (fixture == 7 ? 1 : 2) ||
        r->a[1] != (fixture == 7 ? 1 : 5) ||
        r->b[0] != (fixture == 7 ? 1 : 3) ||
        r->b[1] != (fixture == 7 ? 9 : 5)) return EXIT_FAILURE;
  }
  if (fixture == 3) {
    // Three pairs meet at (40/9,40/9). The original README describes
    // this imprecisely as four lines meeting at one point.
    unsigned triple = 0;
    for (size_t i = 0; i < length; ++i)
      for (size_t j = i + 1; j < length; ++j) {
        LRESULT r;
        if (LINTERSECT(segments + i, segments + j, &r) ==
            Arcadia_Math_SegmentIntersection2Kind_Point &&
            fabs((double)r.a[0] - 40.0 / 9.0) < (S == 32 ? 0.00001 : 0.000000000001) &&
            fabs((double)r.a[1] - 40.0 / 9.0) < (S == 32 ? 0.00001 : 0.000000000001)) ++triple;
      }
    if (triple != 3) return EXIT_FAILURE;
  }
  if (fixture == 5) {
    for (size_t i = 0; i < 9; ++i)
      if (!audit.seen[i][9]) return EXIT_FAILURE;
  }
  if (fixture == 6) {
    const size_t expectedPairs[4][2] = {{0,2},{0,3},{1,2},{1,3}};
    for (size_t i = 0; i < 4; ++i)
      if (!audit.seen[expectedPairs[i][0]][expectedPairs[i][1]]) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

#define LCASE(n) static int LN(legacyTest##n)(void) { return LN(legacyFixture)(n, 0); }
LCASE(1)
LCASE(2)
LCASE(3)
LCASE(4)
LCASE(5)
LCASE(6)
LCASE(7)
LCASE(8)
#undef LCASE

static int
LN(legacyReversed)
  (
    void
  )
{
  for (unsigned i = 1; i <= 8; ++i)
    if (LN(legacyFixture)(i, 1) != EXIT_SUCCESS) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

#undef LENUMB
#undef LINTERSECT
#undef LRESULT
#undef LEVENT
#undef LSEG
#undef LN
#undef LC
#undef LC1
