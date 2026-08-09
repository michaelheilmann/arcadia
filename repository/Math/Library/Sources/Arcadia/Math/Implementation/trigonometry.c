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

#include "Arcadia/Math/Implementation/trigonometry.h"

// Symbolic constant for the Arcadia_Real32Value representation of PI.
#define Arcadia_PI_Real32Value 3.1415926f

// Symbolic constant for the Arcadia_Real64Value representation of PI.
#define Arcadia_PI_Real64Value 3.1415926

Arcadia_Real32Value
Arcadia_Math_radiansToDegreesReal32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value a
  )
{
  static const Arcadia_Real32Value x = 180.f / Arcadia_Math_PI_Real32Value;
  return a * x;
}

Arcadia_Real64Value
Arcadia_Math_radiansToDegreesReal64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value a
  )
{
  static const Arcadia_Real64Value x = 180. / Arcadia_Math_PI_Real64Value;
  return a * x;
}

Arcadia_Real32Value
Arcadia_Math_degreesToRadiansReal32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value a
  )
{
  static const Arcadia_Real32Value x = Arcadia_Math_PI_Real32Value / 180.f;
  return a * x;
}

Arcadia_Real64Value
Arcadia_Math_degreesToRadiansReal64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value a
  )
{
  static const Arcadia_Real64Value x = Arcadia_Math_PI_Real64Value / 180.;
  return a * x;
}
