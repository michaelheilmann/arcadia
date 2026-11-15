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

#include "Arcadia/Math/Implementation/Rectangle2Real64.h"

void
Arcadia_Math_Rectangle2Real64_setEdges
  (
    Arcadia_Math_Rectangle2Real64* self,
    Arcadia_Real64Value left,
    Arcadia_Real64Value bottom,
    Arcadia_Real64Value right,
    Arcadia_Real64Value top
  )
{
  self->x = left;
  self->y = bottom;
  self->extentX = right - left;
  self->extentY = top - bottom;
}

void
Arcadia_Math_Rectangle2Real64_setOffsetAndExtent
  (
    Arcadia_Math_Rectangle2Real64* self,
    Arcadia_Real64Value x,
    Arcadia_Real64Value y,
    Arcadia_Real64Value extentX,
    Arcadia_Real64Value extentY
  )
{
  self->x = x;
  self->y = y;
  self->extentX = extentX;
  self->extentY = extentY;
}

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getLeft
  (
    const Arcadia_Math_Rectangle2Real64* self
  )
{
  return self->x + (self->extentX < 0 ? self->extentX : 0);
}

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getRight
  (
    const Arcadia_Math_Rectangle2Real64* self
  )
{
  return self->x + (self->extentX > 0 ? self->extentX : 0);
}

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getBottom
  (
    const Arcadia_Math_Rectangle2Real64* self
  )
{
  return self->y + (self->extentY < 0 ? self->extentY : 0);
}

Arcadia_Real64Value
Arcadia_Math_Rectangle2Real64_getTop
  (
    const Arcadia_Math_Rectangle2Real64* self
  )
{
  return self->y + (self->extentY > 0 ? self->extentY : 0);
}

void
Arcadia_Math_Rectangle2Real64_union
  (
    Arcadia_Math_Rectangle2Real64* result,
    const Arcadia_Math_Rectangle2Real64* a,
    const Arcadia_Math_Rectangle2Real64* b
  )
{
  // Read all bounds before writing result so either source may alias it.
  Arcadia_Real64Value aLeft = Arcadia_Math_Rectangle2Real64_getLeft(a);
  Arcadia_Real64Value aBottom = Arcadia_Math_Rectangle2Real64_getBottom(a);
  Arcadia_Real64Value aRight = Arcadia_Math_Rectangle2Real64_getRight(a);
  Arcadia_Real64Value aTop = Arcadia_Math_Rectangle2Real64_getTop(a);
  Arcadia_Real64Value bLeft = Arcadia_Math_Rectangle2Real64_getLeft(b);
  Arcadia_Real64Value bBottom = Arcadia_Math_Rectangle2Real64_getBottom(b);
  Arcadia_Real64Value bRight = Arcadia_Math_Rectangle2Real64_getRight(b);
  Arcadia_Real64Value bTop = Arcadia_Math_Rectangle2Real64_getTop(b);

  Arcadia_Real64Value left = aLeft < bLeft ? aLeft : bLeft;
  Arcadia_Real64Value bottom = aBottom < bBottom ? aBottom : bBottom;
  Arcadia_Real64Value right = aRight > bRight ? aRight : bRight;
  Arcadia_Real64Value top = aTop > bTop ? aTop : bTop;
  Arcadia_Math_Rectangle2Real64_setEdges(result, left, bottom, right, top);
}

void
Arcadia_Math_Rectangle2Real64_translate
  (
    Arcadia_Math_Rectangle2Real64* self,
    Arcadia_Real64Value dx,
    Arcadia_Real64Value dy
  )
{
  self->x += dx;
  self->y += dy;
}
