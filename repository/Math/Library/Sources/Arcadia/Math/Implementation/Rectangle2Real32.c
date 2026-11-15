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

#include "Arcadia/Math/Implementation/Rectangle2Real32.h"

void
Arcadia_Math_Rectangle2Real32_setEdges
  (
    Arcadia_Math_Rectangle2Real32* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value right,
    Arcadia_Real32Value top
  )
{
  self->x = left;
  self->y = bottom;
  self->extentX = right - left;
  self->extentY = top - bottom;
}

void
Arcadia_Math_Rectangle2Real32_setOffsetAndExtent
  (
    Arcadia_Math_Rectangle2Real32* self,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value extentX,
    Arcadia_Real32Value extentY
  )
{
  self->x = x;
  self->y = y;
  self->extentX = extentX;
  self->extentY = extentY;
}

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getLeft
  (
    const Arcadia_Math_Rectangle2Real32* self
  )
{
  return self->x + (self->extentX < 0 ? self->extentX : 0);
}

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getRight
  (
    const Arcadia_Math_Rectangle2Real32* self
  )
{
  return self->x + (self->extentX > 0 ? self->extentX : 0);
}

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getBottom
  (
    const Arcadia_Math_Rectangle2Real32* self
  )
{
  return self->y + (self->extentY < 0 ? self->extentY : 0);
}

Arcadia_Real32Value
Arcadia_Math_Rectangle2Real32_getTop
  (
    const Arcadia_Math_Rectangle2Real32* self
  )
{
  return self->y + (self->extentY > 0 ? self->extentY : 0);
}

void
Arcadia_Math_Rectangle2Real32_union
  (
    Arcadia_Math_Rectangle2Real32* result,
    const Arcadia_Math_Rectangle2Real32* a,
    const Arcadia_Math_Rectangle2Real32* b
  )
{
  // Read all bounds before writing result so either source may alias it.
  Arcadia_Real32Value aLeft = Arcadia_Math_Rectangle2Real32_getLeft(a);
  Arcadia_Real32Value aBottom = Arcadia_Math_Rectangle2Real32_getBottom(a);
  Arcadia_Real32Value aRight = Arcadia_Math_Rectangle2Real32_getRight(a);
  Arcadia_Real32Value aTop = Arcadia_Math_Rectangle2Real32_getTop(a);
  Arcadia_Real32Value bLeft = Arcadia_Math_Rectangle2Real32_getLeft(b);
  Arcadia_Real32Value bBottom = Arcadia_Math_Rectangle2Real32_getBottom(b);
  Arcadia_Real32Value bRight = Arcadia_Math_Rectangle2Real32_getRight(b);
  Arcadia_Real32Value bTop = Arcadia_Math_Rectangle2Real32_getTop(b);

  Arcadia_Real32Value left = aLeft < bLeft ? aLeft : bLeft;
  Arcadia_Real32Value bottom = aBottom < bBottom ? aBottom : bBottom;
  Arcadia_Real32Value right = aRight > bRight ? aRight : bRight;
  Arcadia_Real32Value top = aTop > bTop ? aTop : bTop;
  Arcadia_Math_Rectangle2Real32_setEdges(result, left, bottom, right, top);
}

void
Arcadia_Math_Rectangle2Real32_translate
  (
    Arcadia_Math_Rectangle2Real32* self,
    Arcadia_Real32Value dx,
    Arcadia_Real32Value dy
  )
{
  self->x += dx;
  self->y += dy;
}
