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

#include "Arcadia/Math/Implementation/SegmentIntersection2.h"
#include <math.h>

// Evaluate in double for Real32 to reduce intermediate rounding. For Real64
// use long double where it has extra precision. The API deliberately avoids
// epsilon-based equality: a heuristic epsilon would change endpoint and
// overlap topology according to the scale of a scene.
#define SEGMENT_IMPL(S, W) \
static int lex##S(W ax, W ay, W bx, W by) { \
  return ax < bx ? -1 : ax > bx ? 1 : ay < by ? -1 : ay > by ? 1 : 0; \
} \
static W cross##S(W ax, W ay, W bx, W by) { return ax * by - ay * bx; } \
static int inside##S(W x, W y, W ax, W ay, W bx, W by) { \
  return x >= (ax < bx ? ax : bx) && x <= (ax > bx ? ax : bx) && \
         y >= (ay < by ? ay : by) && y <= (ay > by ? ay : by); \
} \
Arcadia_Math_SegmentIntersection2Kind \
Arcadia_Math_Segment2Real##S##_intersect \
  (const Arcadia_Math_Segment2Real##S* first, \
   const Arcadia_Math_Segment2Real##S* second, \
   Arcadia_Math_SegmentIntersection2Real##S* result) { \
  if (!first || !second) return Arcadia_Math_SegmentIntersection2Kind_Invalid; \
  W ax = first->a[0], ay = first->a[1], bx = first->b[0], by = first->b[1]; \
  W cx = second->a[0], cy = second->a[1], dx = second->b[0], dy = second->b[1]; \
  if (!isfinite(ax) || !isfinite(ay) || !isfinite(bx) || !isfinite(by) || \
      !isfinite(cx) || !isfinite(cy) || !isfinite(dx) || !isfinite(dy)) \
    return Arcadia_Math_SegmentIntersection2Kind_Invalid; \
  W rx = bx - ax, ry = by - ay, sx = dx - cx, sy = dy - cy; \
  W qx = cx - ax, qy = cy - ay; \
  W den = cross##S(rx, ry, sx, sy); \
  W col = cross##S(qx, qy, rx, ry); \
  W ux = 0, uy = 0, vx = 0, vy = 0; \
  Arcadia_Math_SegmentIntersection2Kind kind = Arcadia_Math_SegmentIntersection2Kind_None; \
  if (!isfinite(den) || !isfinite(col)) \
    return Arcadia_Math_SegmentIntersection2Kind_Invalid; \
  if (rx == 0 && ry == 0 && sx == 0 && sy == 0) { \
    if (ax == cx && ay == cy) { ux = ax; uy = ay; kind = Arcadia_Math_SegmentIntersection2Kind_Point; } \
  } else if (rx == 0 && ry == 0) { \
    if (cross##S(ax-cx, ay-cy, sx, sy) == 0 && inside##S(ax, ay, cx, cy, dx, dy)) { \
      ux = ax; uy = ay; kind = Arcadia_Math_SegmentIntersection2Kind_Point; \
    } \
  } else if (sx == 0 && sy == 0) { \
    if (cross##S(cx-ax, cy-ay, rx, ry) == 0 && inside##S(cx, cy, ax, ay, bx, by)) { \
      ux = cx; uy = cy; kind = Arcadia_Math_SegmentIntersection2Kind_Point; \
    } \
  } else if (den == 0) { \
    if (col == 0) { \
      W a0x = ax, a0y = ay, a1x = bx, a1y = by; \
      W b0x = cx, b0y = cy, b1x = dx, b1y = dy; \
      if (lex##S(a0x,a0y,a1x,a1y) > 0) { W t=a0x; a0x=a1x; a1x=t; t=a0y; a0y=a1y; a1y=t; } \
      if (lex##S(b0x,b0y,b1x,b1y) > 0) { W t=b0x; b0x=b1x; b1x=t; t=b0y; b0y=b1y; b1y=t; } \
      ux = lex##S(a0x,a0y,b0x,b0y) >= 0 ? a0x : b0x; \
      uy = lex##S(a0x,a0y,b0x,b0y) >= 0 ? a0y : b0y; \
      vx = lex##S(a1x,a1y,b1x,b1y) <= 0 ? a1x : b1x; \
      vy = lex##S(a1x,a1y,b1x,b1y) <= 0 ? a1y : b1y; \
      int order = lex##S(ux,uy,vx,vy); \
      if (order == 0) kind = Arcadia_Math_SegmentIntersection2Kind_Point; \
      if (order < 0) kind = Arcadia_Math_SegmentIntersection2Kind_Overlap; \
    } \
  } else { \
    W t = cross##S(qx, qy, sx, sy) / den; \
    W u = col / den; \
    if (!isfinite(t) || !isfinite(u)) return Arcadia_Math_SegmentIntersection2Kind_Invalid; \
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) { \
      /* Use an original endpoint exactly when parameters identify one. */ \
      ux = t == 0 ? ax : t == 1 ? bx : u == 0 ? cx : u == 1 ? dx : ax + t*rx; \
      uy = t == 0 ? ay : t == 1 ? by : u == 0 ? cy : u == 1 ? dy : ay + t*ry; \
      kind = Arcadia_Math_SegmentIntersection2Kind_Point; \
    } \
  } \
  if (kind != Arcadia_Math_SegmentIntersection2Kind_None && \
      (!isfinite(ux) || !isfinite(uy) || \
       (kind == Arcadia_Math_SegmentIntersection2Kind_Overlap && (!isfinite(vx) || !isfinite(vy))))) \
    return Arcadia_Math_SegmentIntersection2Kind_Invalid; \
  if (result) { \
    result->kind = kind; \
    if (kind != Arcadia_Math_SegmentIntersection2Kind_None) { \
      result->a[0] = (Arcadia_Real##S##Value)ux; result->a[1] = (Arcadia_Real##S##Value)uy; \
      result->b[0] = (Arcadia_Real##S##Value)(kind == Arcadia_Math_SegmentIntersection2Kind_Overlap ? vx : ux); \
      result->b[1] = (Arcadia_Real##S##Value)(kind == Arcadia_Math_SegmentIntersection2Kind_Overlap ? vy : uy); \
    } \
  } \
  return kind; \
}

SEGMENT_IMPL(32, double)
SEGMENT_IMPL(64, long double)
#undef SEGMENT_IMPL
