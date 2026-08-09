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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_SETORTHOGRAPHICPROJECTION_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_SETORTHOGRAPHICPROJECTION_H_INCLUDED

#include "Arcadia/Math/Implementation/Matrix4Real32.h"
#include "Arcadia/Math/Implementation/Matrix4Real64.h"

#pragma push_macro("near")
#undef near

#pragma push_macro("far")
#undef far

// https://michaelheilmann.com/Arcadia/Math/#Arcadia_Math_Matrix*x**_setOrthographicProjection
void
Arcadia_Math_Matrix4x4Real32_setOrthographicProjection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value right,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value top,
    Arcadia_Real64Value near,
    Arcadia_Real64Value far
  );

// https://michaelheilmann.com/Arcadia/Math/#Arcadia_Math_Matrix*x**_setOrthographicProjection
void
Arcadia_Math_Matrix4x4Real64_setOrthographicProjection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Real64Value left,
    Arcadia_Real64Value right,
    Arcadia_Real64Value bottom,
    Arcadia_Real64Value top,
    Arcadia_Real64Value near,
    Arcadia_Real64Value far
  );

#pragma pop_macro("far")

#pragma pop_macro("near")

#endif // ARCADIA_MATH_IMPLEMENTATION_SETORTHOGRAPHICPROJECTION_H_INCLUDED
