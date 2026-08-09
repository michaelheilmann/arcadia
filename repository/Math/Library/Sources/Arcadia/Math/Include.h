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

#if !defined(ARCADIA_MATH_INCLUDE_H_INCLUDED)
#define ARCADIA_MATH_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_MATH_MODULE")
#define ARCADIA_MATH_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_MATH_EXPORT")
#define ARCADIA_MATH_EXPORT (1)

#include "Arcadia/Math/Implementation/AxisAngle3Real32.h"
#include "Arcadia/Math/Implementation/AxisAngle3Real64.h"
#include "Arcadia/Math/Implementation/crossProduct.h"
#include "Arcadia/Math/Implementation/Configure.h"
#include "Arcadia/Math/Implementation/Color4Real32.h"
#include "Arcadia/Math/Implementation/Color4Real64.h"
#include "Arcadia/Math/Implementation/QuaternionReal32.h"
#include "Arcadia/Math/Implementation/setOrthographicProjection.h"
#include "Arcadia/Math/Implementation/setPerspectiveProjection.h"
#include "Arcadia/Math/Implementation/setRotationX.h"
#include "Arcadia/Math/Implementation/setRotationY.h"
#include "Arcadia/Math/Implementation/setRotationZ.h"
#include "Arcadia/Math/Implementation/setScaling.h"
#include "Arcadia/Math/Implementation/setTranslation.h"
#include "Arcadia/Math/Implementation/Vector3Real32.h"
#include "Arcadia/Math/Implementation/Vector3Real64.h"

#undef ARCADIA_MATH_EXPORT
#pragma pop_macro("ARCADIA_MATH_EXPORT")

#undef ARCADIA_MATH_MODULE
#pragma pop_macro("ARCADIA_MATH_MODULE")

#endif // ARCADIA_MATH_INCLUDE_H_INCLUDED
