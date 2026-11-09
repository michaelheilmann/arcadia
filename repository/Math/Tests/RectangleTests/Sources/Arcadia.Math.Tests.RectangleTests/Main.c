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

static int
checkReal32Edges
  (
    void
  )
{
  Arcadia_Math_Rectangle2Real32 r;
  Arcadia_Math_Rectangle2Real32_setEdges(&r, 10, 20, 5, 12);
  if (Arcadia_Math_Rectangle2Real32_getLeft(&r) != 5 || Arcadia_Math_Rectangle2Real32_getRight(&r) != 10 ||
      Arcadia_Math_Rectangle2Real32_getBottom(&r) != 12 || Arcadia_Math_Rectangle2Real32_getTop(&r) != 20 ||
      r.extentX != -5 || r.extentY != -8) {
    return EXIT_FAILURE;
  }
  Arcadia_Math_Rectangle2Real32_setOffsetAndExtent(&r, 3, 7, 0, 0);
  if (Arcadia_Math_Rectangle2Real32_getLeft(&r) != 3 || Arcadia_Math_Rectangle2Real32_getRight(&r) != 3 ||
      Arcadia_Math_Rectangle2Real32_getBottom(&r) != 7 || Arcadia_Math_Rectangle2Real32_getTop(&r) != 7) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
checkReal32UnionAndAliases
  (
    void
  )
{
  Arcadia_Math_Rectangle2Real32 a, b, result;
  Arcadia_Math_Rectangle2Real32_setOffsetAndExtent(&a, 10, 10, -5, -7);
  Arcadia_Math_Rectangle2Real32_setEdges(&b, -2, 8, 7, 13);
  Arcadia_Math_Rectangle2Real32_union(&result, &a, &b);
  if (result.x != -2 || result.y != 3 || result.extentX != 12 || result.extentY != 10) {
    return EXIT_FAILURE;
  }
  Arcadia_Math_Rectangle2Real32_union(&a, &a, &b);
  if (a.x != result.x || a.y != result.y ||
      a.extentX != result.extentX || a.extentY != result.extentY) {
    return EXIT_FAILURE;
  }
  Arcadia_Math_Rectangle2Real32_setOffsetAndExtent(&a, 10, 10, -5, -7);
  Arcadia_Math_Rectangle2Real32_union(&b, &a, &b);
  if (b.x != result.x || b.y != result.y ||
      b.extentX != result.extentX || b.extentY != result.extentY) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
checkReal32Translation
  (
    void
  )
{
  Arcadia_Math_Rectangle2Real32 r;
  Arcadia_Math_Rectangle2Real32_setOffsetAndExtent(&r, 10, 20, -5, -8);
  Arcadia_Math_Rectangle2Real32_translate(&r, -2, 3);
  if (r.x != 8 || r.y != 23 || r.extentX != -5 || r.extentY != -8 ||
      Arcadia_Math_Rectangle2Real32_getLeft(&r) != 3 || Arcadia_Math_Rectangle2Real32_getTop(&r) != 23) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
checkReal64Edges
  (
    void
  )
{
  Arcadia_Math_Rectangle2Real64 r;
  Arcadia_Math_Rectangle2Real64_setEdges(&r, 10, 20, 5, 12);
  if (Arcadia_Math_Rectangle2Real64_getLeft(&r) != 5 || Arcadia_Math_Rectangle2Real64_getRight(&r) != 10 ||
      Arcadia_Math_Rectangle2Real64_getBottom(&r) != 12 || Arcadia_Math_Rectangle2Real64_getTop(&r) != 20 ||
      r.extentX != -5 || r.extentY != -8) {
    return EXIT_FAILURE;
  }
  Arcadia_Math_Rectangle2Real64_setOffsetAndExtent(&r, 3, 7, 0, 0);
  if (Arcadia_Math_Rectangle2Real64_getLeft(&r) != 3 || Arcadia_Math_Rectangle2Real64_getRight(&r) != 3 ||
      Arcadia_Math_Rectangle2Real64_getBottom(&r) != 7 || Arcadia_Math_Rectangle2Real64_getTop(&r) != 7) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
checkReal64UnionAndAliases
  (
    void
  )
{
  Arcadia_Math_Rectangle2Real64 a, b, result;
  Arcadia_Math_Rectangle2Real64_setOffsetAndExtent(&a, 10, 10, -5, -7);
  Arcadia_Math_Rectangle2Real64_setEdges(&b, -2, 8, 7, 13);
  Arcadia_Math_Rectangle2Real64_union(&result, &a, &b);
  if (result.x != -2 || result.y != 3 || result.extentX != 12 || result.extentY != 10) {
    return EXIT_FAILURE;
  }
  Arcadia_Math_Rectangle2Real64_union(&a, &a, &b);
  if (a.x != result.x || a.y != result.y ||
      a.extentX != result.extentX || a.extentY != result.extentY) {
    return EXIT_FAILURE;
  }
  Arcadia_Math_Rectangle2Real64_setOffsetAndExtent(&a, 10, 10, -5, -7);
  Arcadia_Math_Rectangle2Real64_union(&b, &a, &b);
  if (b.x != result.x || b.y != result.y ||
      b.extentX != result.extentX || b.extentY != result.extentY) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static int
checkReal64Translation
  (
    void
  )
{
  Arcadia_Math_Rectangle2Real64 r;
  Arcadia_Math_Rectangle2Real64_setOffsetAndExtent(&r, 10, 20, -5, -8);
  Arcadia_Math_Rectangle2Real64_translate(&r, -2, 3);
  if (r.x != 8 || r.y != 23 || r.extentX != -5 || r.extentY != -8 ||
      Arcadia_Math_Rectangle2Real64_getLeft(&r) != 3 || Arcadia_Math_Rectangle2Real64_getTop(&r) != 23) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Real32Edges", &checkReal32Edges },
    { "Real32UnionAndAliases", &checkReal32UnionAndAliases },
    { "Real32Translation", &checkReal32Translation },
    { "Real64Edges", &checkReal64Edges },
    { "Real64UnionAndAliases", &checkReal64UnionAndAliases },
    { "Real64Translation", &checkReal64Translation },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
