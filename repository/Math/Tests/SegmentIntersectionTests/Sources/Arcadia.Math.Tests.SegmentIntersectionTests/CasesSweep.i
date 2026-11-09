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

#include <stdint.h>

#define CAT1(a,b) a##b
#define CAT(a,b) CAT1(a,b)
#define T(name) CAT(name,S)
#define SEG T(Arcadia_Math_Segment2Real)
#define EVENT T(Arcadia_Math_SegmentSweep2EventReal)
#define ENUM CAT(SEG,_enumerateIntersections)
#define INTERSECT CAT(SEG,_intersect)

typedef struct T(SweepResults) {
  EVENT events[300];
  size_t size;
  size_t stopAt;
} T(SweepResults);

static Arcadia_BooleanValue
T(sweepCollect)
  (
    void* context,
    const EVENT* event
  )
{
  T(SweepResults)* results = context;
  if (results->size >= sizeof(results->events) / sizeof(results->events[0])) return false;
  results->events[results->size++] = *event;
  return !results->stopAt || results->size < results->stopAt;
}

static int
T(sweepMultiway)
  (
    void
  )
{
  const SEG segments[] = {
    {{0,0},{8,8}}, {{0,8},{8,0}}, {{4,-2},{4,10}},
    {{-2,4},{10,4}}, {{16,0},{20,4}}
  };
  T(SweepResults) results = {0};
  size_t count = 999;
  if (ENUM(segments, 5, T(sweepCollect), &results, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 6 || results.size != 6)
    return EXIT_FAILURE;
  const size_t left[] = {0,0,0,1,1,2};
  const size_t right[] = {1,2,3,2,3,3};
  for (size_t i = 0; i < 6; ++i) {
    const EVENT* e = results.events + i;
    if (e->firstIndex != left[i] || e->secondIndex != right[i] ||
        e->intersection.kind != Arcadia_Math_SegmentIntersection2Kind_Point ||
        e->intersection.a[0] != 4 || e->intersection.a[1] != 4) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
T(sweepDegenerate)
  (
    void
  )
{
  const SEG segments[] = {
    {{5,0},{0,0}}, {{2,0},{7,0}}, {{2,0},{7,0}},
    {{3,-2},{3,2}}, {{5,0},{5,0}}
  };
  T(SweepResults) results = {0};
  size_t count = 0, overlaps = 0, points = 0;
  if (ENUM(segments, 5, T(sweepCollect), &results, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 9) return EXIT_FAILURE;
  for (size_t i = 0; i < count; ++i) {
    const EVENT* e = results.events + i;
    if (e->intersection.kind == Arcadia_Math_SegmentIntersection2Kind_Overlap) {
      ++overlaps;
      if (e->intersection.a[1] != 0 || e->intersection.b[1] != 0 ||
          e->intersection.a[0] >= e->intersection.b[0]) return EXIT_FAILURE;
    } else if (e->intersection.kind == Arcadia_Math_SegmentIntersection2Kind_Point) {
      ++points;
    } else return EXIT_FAILURE;
    if (i) {
      const EVENT* p = results.events + i - 1;
      if (p->intersection.a[0] > e->intersection.a[0] ||
          (p->intersection.a[0] == e->intersection.a[0] &&
           p->intersection.a[1] > e->intersection.a[1])) return EXIT_FAILURE;
    }
  }
  return overlaps == 3 && points == 6 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
T(sweepStopInvalid)
  (
    void
  )
{
  SEG segments[] = {
    {{0,0},{4,4}}, {{0,4},{4,0}}, {{2,-1},{2,5}}
  };
  T(SweepResults) results = {0};
  results.stopAt = 2;
  size_t count = 42;
  if (ENUM(segments, 3, T(sweepCollect), &results, &count) !=
      Arcadia_Math_SegmentSweep2Status_Stopped || count != 2 || results.size != 2)
    return EXIT_FAILURE;
  if (ENUM(segments, 3, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 3) return EXIT_FAILURE;
  if (ENUM(NULL, 0, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 0) return EXIT_FAILURE;
  count = 42;
  if (ENUM(NULL, 1, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Invalid || count != 42) return EXIT_FAILURE;
  segments[2].a[0] = NAN;
  if (ENUM(segments, 3, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Invalid || count != 42) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

static int
T(sweepGrid)
  (
    void
  )
{
  SEG segments[24];
  for (size_t i = 0; i < 24; ++i) {
    segments[i].a[0] = (int)((i * 7) % 9) - 4;
    segments[i].a[1] = (int)((i * 5) % 9) - 4;
    segments[i].b[0] = (int)((i * 3) % 9) - 4;
    segments[i].b[1] = (int)((i * 11) % 9) - 4;
  }
  size_t expected = 0, actual = 0;
  for (size_t i = 0; i < 24; ++i) {
    for (size_t j = i + 1; j < 24; ++j) {
      if (INTERSECT(segments + i, segments + j, NULL) !=
          Arcadia_Math_SegmentIntersection2Kind_None) ++expected;
    }
  }
  return ENUM(segments, 24, NULL, NULL, &actual) ==
         Arcadia_Math_SegmentSweep2Status_Completed && actual == expected ?
         EXIT_SUCCESS : EXIT_FAILURE;
}

// Compare each pair against an independent, unswept all-pairs oracle.
// The seed and integer coordinate range are deliberately fixed so a failure
// is deterministic on every platform. Exercise reversal, duplicates, points,
// shared endpoints and vertical segments over many independent scenes.
static int
T(sweepSeededOracle)
  (
    void
  )
{
  SEG segments[48];
  unsigned int seed = 0x31415926u;
  for (size_t scene = 0; scene < 80; ++scene) {
    for (size_t i = 0; i < 48; ++i) {
      for (size_t k = 0; k < 4; ++k) {
        seed = seed * 1664525u + 1013904223u;
        int coordinate = (int)((seed >> 16) % 25) - 12;
        if (k == 0) segments[i].a[0] = coordinate;
        if (k == 1) segments[i].a[1] = coordinate;
        if (k == 2) segments[i].b[0] = coordinate;
        if (k == 3) segments[i].b[1] = coordinate;
      }
      if (i % 7 == 0) segments[i].b[0] = segments[i].a[0];
      if (i % 11 == 0) segments[i].b[1] = segments[i].a[1];
      if (i % 13 == 0 && i) segments[i] = segments[i - 1];
    }
    size_t expected = 0;
    for (size_t i = 0; i < 48; ++i)
      for (size_t j = i + 1; j < 48; ++j) {
        const Arcadia_Math_SegmentIntersection2Kind result =
          INTERSECT(segments + i, segments + j, NULL);
        if (result == Arcadia_Math_SegmentIntersection2Kind_Invalid) return EXIT_FAILURE;
        if (result != Arcadia_Math_SegmentIntersection2Kind_None) ++expected;
      }
    size_t actual = SIZE_MAX;
    if (ENUM(segments, 48, NULL, NULL, &actual) !=
        Arcadia_Math_SegmentSweep2Status_Completed || actual != expected)
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

// Disjoint slabs and parallel horizontal lanes should not report false hits.
static int
T(sweepSeparatedLanes)
  (
    void
  )
{
  SEG segments[128];
  for (size_t i = 0; i < 128; ++i) {
    segments[i].a[0] = 0;
    segments[i].a[1] = (int)i * 4;
    segments[i].b[0] = 100;
    segments[i].b[1] = (int)i * 4;
  }
  size_t count = SIZE_MAX;
  if (ENUM(segments, 128, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 0)
    return EXIT_FAILURE;
  // Touching bounding rectangles must not be pruned: every lane crosses
  // one vertical segment, including both extreme endpoints.
  SEG vertical = {{50,0},{50,508}};
  SEG crossing[129];
  for (size_t i = 0; i < 128; ++i) crossing[i] = segments[i];
  crossing[128] = vertical;
  if (ENUM(crossing, 129, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 128)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

// The callback is synchronous and must be able to stop after the first
// emitted intersection, without any duplicate notifications for one pair.
static int
T(sweepFirstHit)
  (
    void
  )
{
  SEG segments[40];
  for (size_t i = 0; i < 40; ++i) {
    segments[i].a[0] = 0;
    segments[i].a[1] = (int)i;
    segments[i].b[0] = 40;
    segments[i].b[1] = 40 - (int)i;
  }
  T(SweepResults) results = {0};
  results.stopAt = 1;
  size_t count = 0;
  if (ENUM(segments, 40, T(sweepCollect), &results, &count) !=
      Arcadia_Math_SegmentSweep2Status_Stopped || count != 1 || results.size != 1)
    return EXIT_FAILURE;
  if (results.events[0].firstIndex >= results.events[0].secondIndex)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}


// A sparse high-cardinality input should not require the quadratic active
// scan used by the first salvage increment. The entire collection stays
// active in X, but every lane has a disjoint Y interval.
static int
T(sweepLargeSparse)
  (
    void
  )
{
  SEG segments[4096];
  for (size_t i = 0; i < 4096; ++i) {
    segments[i].a[0] = -20;
    segments[i].b[0] = 20;
    segments[i].a[1] = (int)i * 4;
    segments[i].b[1] = (int)i * 4;
  }
  size_t count = SIZE_MAX;
  if (ENUM(segments, 4096, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 0)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

// Exercise AVL removals including nodes with two children. The final
// long-lived vertical segment intersects every preceding horizontal bar,
// while each earlier bar has already expired before the next one begins.
static int
T(sweepExpiry)
  (
    void
  )
{
  SEG segments[1001];
  for (size_t i = 0; i < 1000; ++i) {
    segments[i].a[0] = (int)i * 3;
    segments[i].b[0] = (int)i * 3 + 1;
    segments[i].a[1] = (int)(i % 91);
    segments[i].b[1] = (int)(i % 91);
  }
  size_t count = SIZE_MAX;
  if (ENUM(segments, 1000, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 0)
    return EXIT_FAILURE;
  // Equal right/left X boundaries are inclusive: a segment starting
  // precisely at the previous segment's end must still see that segment.
  for (size_t i = 0; i < 1000; ++i) {
    segments[i].a[0] = (int)i;
    segments[i].b[0] = (int)i + 1;
    segments[i].a[1] = 7;
    segments[i].b[1] = 7;
  }
  if (ENUM(segments, 1000, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 999)
    return EXIT_FAILURE;
  // Insert long-lived ranges of distinct heights in interleaved order,
  // then expire them in a different order while new intervals enter.
  for (size_t i = 0; i < 500; ++i) {
    segments[i].a[0] = 0;
    segments[i].b[0] = 100 + (int)(i % 17);
    segments[i].a[1] = (int)((i * 173) % 997) * 4;
    segments[i].b[1] = segments[i].a[1];
  }
  for (size_t i = 500; i < 1000; ++i) {
    segments[i].a[0] = 101 + (int)((i * 11) % 37);
    segments[i].b[0] = segments[i].a[0] + 1;
    segments[i].a[1] = (int)((i * 173) % 997) * 4 + 1;
    segments[i].b[1] = segments[i].a[1];
  }
  if (ENUM(segments, 1000, NULL, NULL, &count) !=
      Arcadia_Math_SegmentSweep2Status_Completed || count != 0)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

// Verify the complete identity/kind/coordinates of every emitted pair, not
// just the total count. The canonical predicate is deliberately used as the
// oracle, so this tests index completeness and ordered reporting separately.
typedef struct T(SweepAudit) {
  const SEG* segments;
  unsigned char seen[48][48];
  size_t delivered;
  int valid;
  EVENT previous;
} T(SweepAudit);

static Arcadia_BooleanValue
T(sweepAuditCallback)
  (
    void* context,
    const EVENT* event
  )
{
  T(SweepAudit)* audit = context;
  if (event->firstIndex >= 48 || event->secondIndex >= 48 ||
      event->firstIndex >= event->secondIndex) {
    audit->valid = 0;
    return true;
  }
  if (audit->seen[event->firstIndex][event->secondIndex]) audit->valid = 0;
  audit->seen[event->firstIndex][event->secondIndex] = 1;
  Arcadia_Math_SegmentIntersection2Kind kind;
  T(Arcadia_Math_SegmentIntersection2Real) hit;
  kind = INTERSECT(audit->segments + event->firstIndex,
                   audit->segments + event->secondIndex, &hit);
  if (kind != event->intersection.kind || kind == Arcadia_Math_SegmentIntersection2Kind_None ||
      hit.a[0] != event->intersection.a[0] || hit.a[1] != event->intersection.a[1] ||
      hit.b[0] != event->intersection.b[0] || hit.b[1] != event->intersection.b[1])
    audit->valid = 0;
  if (audit->delivered) {
    const EVENT* p = &audit->previous;
    if (p->intersection.a[0] > event->intersection.a[0] ||
        (p->intersection.a[0] == event->intersection.a[0] &&
         p->intersection.a[1] > event->intersection.a[1]) ||
        (p->intersection.a[0] == event->intersection.a[0] &&
         p->intersection.a[1] == event->intersection.a[1] &&
         (p->firstIndex > event->firstIndex ||
          (p->firstIndex == event->firstIndex && p->secondIndex >= event->secondIndex))))
      audit->valid = 0;
  }
  audit->previous = *event;
  ++audit->delivered;
  return true;
}

static int
T(sweepEventOracle)
  (
    void
  )
{
  SEG segments[48];
  uint32_t seed = 0x13579bdfu;
  for (size_t scene = 0; scene < 70; ++scene) {
    for (size_t i = 0; i < 48; ++i) {
      for (size_t k = 0; k < 4; ++k) {
        seed = seed * 1664525u + 1013904223u;
        int coordinate = (int)((seed >> 16) % 41) - 20;
        if (k == 0) segments[i].a[0] = coordinate;
        if (k == 1) segments[i].a[1] = coordinate;
        if (k == 2) segments[i].b[0] = coordinate;
        if (k == 3) segments[i].b[1] = coordinate;
      }
      if (i % 11 == 0) segments[i].a[0] = segments[i].b[0];
      if (i % 13 == 0) segments[i] = segments[i ? i - 1 : i];
    }
    T(SweepAudit) audit = {0};
    audit.segments = segments;
    audit.valid = 1;
    size_t count = SIZE_MAX;
    if (ENUM(segments, 48, T(sweepAuditCallback), &audit, &count) !=
        Arcadia_Math_SegmentSweep2Status_Completed || !audit.valid ||
        audit.delivered != count) return EXIT_FAILURE;
    for (size_t i = 0; i < 48; ++i)
      for (size_t j = i + 1; j < 48; ++j) {
        const int expected = INTERSECT(segments + i, segments + j, NULL) !=
          Arcadia_Math_SegmentIntersection2Kind_None;
        if ((int)audit.seen[i][j] != expected) return EXIT_FAILURE;
      }
  }
  return EXIT_SUCCESS;
}

#undef INTERSECT
#undef ENUM
#undef EVENT
#undef SEG
#undef T
#undef CAT
#undef CAT1
