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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_TRIGONOMETRY_H_INCLUDE)
#define ARCADIA_MATH_IMPLEMENTATION_TRIGONOMETRY_H_INCLUDE

#include "Arcadia/Ring2/Include.h"

/// Symbolic constant for the Arcadia_Real32Value representation of PI.
#define Arcadia_Math_PI_Real32Value 3.1415926f

/// Symbolic constant for the Arcadia_Real64Value representation of PI.
#define Arcadia_Math_PI_Real64Value 3.1415926

/// @{
/// @brief Convert an angle in radians into a corresponding angle in degrees.
/// @param a The angle in radians.
/// @return A corresponding angle in degrees.
/// @remarks
/// To convert an angle \f$a\f$ in radians to a corresponding angle in degrees,
/// we divide by \f$a\f$ by \f$2\pi\f$ and multiply \f$a\f$ by \f$360\f$.
/// That is, we compute \f$\frac{a\cdot 360}{2\pi}\f$.
/// This can be simplified to \f$\frac{180 a}{\pi}\f$ and to \f$a \frac{180}{\pi}\f$.
Arcadia_Real32Value
Arcadia_Math_radiansToDegreesReal32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value a
  );

Arcadia_Real64Value
Arcadia_Math_radiansToDegreesReal64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value a
  );

/// @}

/// @{
/// @brief Convert an angle in degrees into a corresponding angle in radians.
/// @param a The angle in degrees.
/// @return A corresponding angle in radians.
/// @remarks
/// To convert an angle \f$a\f$ in degrees to a corresponding angle in radians,
/// we divide \f$a\f$ by \f$360\f$ and multiply \f$a\f$ by \f$2\pi\f$.
/// That is, we compute \f$\frac{a \cdot 2\pi}{360}\f$.
/// This can be simplified to  \f$\frac{a\pi}{180}\f$ and to \f$a \frac{\pi}{180}\f$.
/// \f$\frac{\pi}{180}\f$ can be precomputed.
Arcadia_Real32Value
Arcadia_Math_degreesToRadiansReal32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value a
  );

Arcadia_Real64Value
Arcadia_Math_degreesToRadiansReal64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value a
  );

/// @}

#endif // ARCADIA_MATH_IMPLEMENTATION_TRIGONOMETRY_H_INCLUDE
