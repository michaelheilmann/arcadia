// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Arcadia/Math/Include.h"
#include "Arcadia/Testing/Include.h"

#include <stddef.h>
#include <stdlib.h>

// Native values are byte-compact and contain no managed object header.
_Static_assert(sizeof(Arcadia_Math_RgbReal32) == 3 * sizeof(Arcadia_Real32Value), "RGB component layout");
_Static_assert(offsetof(Arcadia_Math_RgbReal32, r) == 0, "red offset");
_Static_assert(offsetof(Arcadia_Math_RgbReal32, g) == sizeof(Arcadia_Real32Value), "green offset");
_Static_assert(offsetof(Arcadia_Math_RgbReal32, b) == 2 * sizeof(Arcadia_Real32Value), "blue offset");

static void
testValues
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Math_RgbReal32 first, second;
  Arcadia_Math_RgbReal32_set(&first, 0.25f, 0.5f, 0.75f);
  Arcadia_Math_RgbReal32_set(&second, 0.25f, 0.5f, 0.75f);
  Arcadia_Tests_assertTrue(thread, Arcadia_Math_RgbReal32_isEqualTo(&first, &second));
  Arcadia_Math_RgbReal32_set(&second, 0.f, 0.5f, 0.75f);
  Arcadia_Tests_assertFalse(thread, Arcadia_Math_RgbReal32_isEqualTo(&first, &second));
  Arcadia_Math_RgbReal32_set(&second, 0.25f, 0.f, 0.75f);
  Arcadia_Tests_assertFalse(thread, Arcadia_Math_RgbReal32_isEqualTo(&first, &second));
  Arcadia_Math_RgbReal32_set(&second, 0.25f, 0.5f, 0.f);
  Arcadia_Tests_assertFalse(thread, Arcadia_Math_RgbReal32_isEqualTo(&first, &second));
  Arcadia_Math_RgbReal32_set(&second, -1.f, 2.f, 4.f);
  Arcadia_Tests_assertTrue(thread, second.r == -1.f && second.g == 2.f && second.b == 4.f);
}

static void
testInterpolation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Math_RgbReal32 first, second, result;
  Arcadia_Math_RgbReal32_set(&first, 0.f, 2.f, 4.f);
  Arcadia_Math_RgbReal32_set(&second, 4.f, 6.f, 8.f);
  Arcadia_Math_RgbReal32_lerp(&result, &first, &second, -1.f);
  Arcadia_Tests_assertTrue(thread, result.r == 0.f && result.g == 2.f && result.b == 4.f);
  Arcadia_Math_RgbReal32_lerp(&result, &first, &second, 2.f);
  Arcadia_Tests_assertTrue(thread, result.r == 4.f && result.g == 6.f && result.b == 8.f);
  Arcadia_Math_RgbReal32_lerp(&result, &first, &second, 0.25f);
  Arcadia_Tests_assertTrue(thread, result.r == 1.f && result.g == 3.f && result.b == 5.f);
  // Destination aliases first or second; no channel may use an overwritten input.
  Arcadia_Math_RgbReal32_lerp(&first, &first, &second, 0.5f);
  Arcadia_Tests_assertTrue(thread, first.r == 2.f && first.g == 4.f && first.b == 6.f);
  Arcadia_Math_RgbReal32_lerp(&second, &first, &second, 0.5f);
  Arcadia_Tests_assertTrue(thread, second.r == 3.f && second.g == 5.f && second.b == 7.f);
  Arcadia_Math_RgbReal32_lerp(&first, &first, &first, 0.3f);
  Arcadia_Tests_assertTrue(thread, first.r == 2.f && first.g == 4.f && first.b == 6.f);
}

static int
runValues
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testValues) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runInterpolation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterpolation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "NativeRgbReal32Values", &runValues },
    { "NativeRgbReal32Interpolation", &runInterpolation },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
