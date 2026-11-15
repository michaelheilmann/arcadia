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

#include "NativeValues.h"

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"
#include <stdlib.h>

#define Scalar Arcadia_Real32Value
#define Vector Arcadia_Math_Vector2Real32
#define V(name) Arcadia_Math_Vector2Real32_##name
#define Test(name) Arcadia_Math_Tests_nativeVectorValues2Real32_##name
#define Dimensions 2
#include "NativeValues.i"
#undef Dimensions
#undef Test
#undef V
#undef Vector
#undef Scalar

#define Scalar Arcadia_Real64Value
#define Vector Arcadia_Math_Vector2Real64
#define V(name) Arcadia_Math_Vector2Real64_##name
#define Test(name) Arcadia_Math_Tests_nativeVectorValues2Real64_##name
#define Dimensions 2
#include "NativeValues.i"
#undef Dimensions
#undef Test
#undef V
#undef Vector
#undef Scalar

#define Scalar Arcadia_Real32Value
#define Vector Arcadia_Math_Vector3Real32
#define V(name) Arcadia_Math_Vector3Real32_##name
#define Test(name) Arcadia_Math_Tests_nativeVectorValues3Real32_##name
#define Dimensions 3
#include "NativeValues.i"
#undef Dimensions
#undef Test
#undef V
#undef Vector
#undef Scalar

#define Scalar Arcadia_Real64Value
#define Vector Arcadia_Math_Vector3Real64
#define V(name) Arcadia_Math_Vector3Real64_##name
#define Test(name) Arcadia_Math_Tests_nativeVectorValues3Real64_##name
#define Dimensions 3
#include "NativeValues.i"
#undef Dimensions
#undef Test
#undef V
#undef Vector
#undef Scalar

#define Scalar Arcadia_Real32Value
#define Vector Arcadia_Math_Vector4Real32
#define V(name) Arcadia_Math_Vector4Real32_##name
#define Test(name) Arcadia_Math_Tests_nativeVectorValues4Real32_##name
#define Dimensions 4
#include "NativeValues.i"
#undef Dimensions
#undef Test
#undef V
#undef Vector
#undef Scalar

#define Scalar Arcadia_Real64Value
#define Vector Arcadia_Math_Vector4Real64
#define V(name) Arcadia_Math_Vector4Real64_##name
#define Test(name) Arcadia_Math_Tests_nativeVectorValues4Real64_##name
#define Dimensions 4
#include "NativeValues.i"
#undef Dimensions
#undef Test
#undef V
#undef Vector
#undef Scalar
