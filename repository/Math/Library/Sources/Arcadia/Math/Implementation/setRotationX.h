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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_SETROTATIONX_H_INCLUDE)
#define ARCADIA_MATH_IMPLEMENTATION_SETROTATIONX_H_INCLUDE

#include "Arcadia/Math/Implementation/Matrix4Real32.h"
#include "Arcadia/Math/Implementation/Matrix4Real64.h"

// https://michaelheilmann.com/Arcadia/Math/#Arcadia_Math_Matrix*x**_setRotationX
void
Arcadia_Math_Matrix4x4Real32Value_setRotationX
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Real32Value a
  );

// https://michaelheilmann.com/Arcadia/Math/#Arcadia_Math_Matrix*x**_setRotationX
void
Arcadia_Math_Matrix4x4Real64Value_setRotationX
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Real64Value a
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_SETROTATIONX_H_INCLUDE
