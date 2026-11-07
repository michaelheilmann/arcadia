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


#include "Values.h"

#include "Arcadia/Media/Include.h"
#include "Arcadia/Testing/Include.h"

#include <stddef.h>
#include <stdlib.h>

// The layout must match the byte order specified by PixelFormat exactly.
_Static_assert(sizeof(Arcadia_Media_PixelAlphaBlueGreenRedNatural8) == 4, "ABGR byte count");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaBlueGreenRedNatural8, a) == 0, "ABGR a byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaBlueGreenRedNatural8, b) == 1, "ABGR b byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaBlueGreenRedNatural8, g) == 2, "ABGR g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaBlueGreenRedNatural8, r) == 3, "ABGR r byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelAlphaGrayNatural8) == 2, "AG byte count");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaGrayNatural8, a) == 0, "AG a byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaGrayNatural8, g) == 1, "AG g byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelAlphaRedGreenBlueNatural8) == 4, "ARGB byte count");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaRedGreenBlueNatural8, a) == 0, "ARGB a byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaRedGreenBlueNatural8, r) == 1, "ARGB r byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaRedGreenBlueNatural8, g) == 2, "ARGB g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelAlphaRedGreenBlueNatural8, b) == 3, "ARGB b byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelBlueGreenRedNatural8) == 3, "BGR byte count");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedNatural8, b) == 0, "BGR b byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedNatural8, g) == 1, "BGR g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedNatural8, r) == 2, "BGR r byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelBlueGreenRedAlphaNatural8) == 4, "BGRA byte count");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedAlphaNatural8, b) == 0, "BGRA b byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedAlphaNatural8, g) == 1, "BGRA g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedAlphaNatural8, r) == 2, "BGRA r byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelBlueGreenRedAlphaNatural8, a) == 3, "BGRA a byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelGrayNatural8) == 1, "G byte count");
_Static_assert(offsetof(Arcadia_Media_PixelGrayNatural8, g) == 0, "G g byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelGrayAlphaNatural8) == 2, "GA byte count");
_Static_assert(offsetof(Arcadia_Media_PixelGrayAlphaNatural8, g) == 0, "GA g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelGrayAlphaNatural8, a) == 1, "GA a byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelRedGreenBlueNatural8) == 3, "RGB byte count");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueNatural8, r) == 0, "RGB r byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueNatural8, g) == 1, "RGB g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueNatural8, b) == 2, "RGB b byte offset");
_Static_assert(sizeof(Arcadia_Media_PixelRedGreenBlueAlphaNatural8) == 4, "RGBA byte count");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueAlphaNatural8, r) == 0, "RGBA r byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueAlphaNatural8, g) == 1, "RGBA g byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueAlphaNatural8, b) == 2, "RGBA b byte offset");
_Static_assert(offsetof(Arcadia_Media_PixelRedGreenBlueAlphaNatural8, a) == 3, "RGBA a byte offset");

static void
testABGR
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelAlphaBlueGreenRedNatural8 first;
  Arcadia_Media_PixelAlphaBlueGreenRedNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37, 79, 173 };
  Arcadia_Media_PixelAlphaBlueGreenRedNatural8_set(&first, 11, 37, 79, 173);
  Arcadia_Media_PixelAlphaBlueGreenRedNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelAlphaBlueGreenRedNatural8_set(&second, 11, 37, 79, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelAlphaBlueGreenRedNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.a = (Arcadia_Natural8Value)(second.a + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.a = first.a;
  second.b = (Arcadia_Natural8Value)(second.b + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.b = first.b;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.r = (Arcadia_Natural8Value)(second.r + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.r = first.r;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaBlueGreenRedNatural8_isEqualTo(&first, &second));

}

static void
testAG
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelAlphaGrayNatural8 first;
  Arcadia_Media_PixelAlphaGrayNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37 };
  Arcadia_Media_PixelAlphaGrayNatural8_set(&first, 11, 37);
  Arcadia_Media_PixelAlphaGrayNatural8_set(&second, 11, 37);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelAlphaGrayNatural8_set(&second, 11, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelAlphaGrayNatural8_set(&second, 11, 37);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.a = (Arcadia_Natural8Value)(second.a + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo(&first, &second));
  second.a = first.a;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaGrayNatural8_isEqualTo(&first, &second));

}

static void
testARGB
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelAlphaRedGreenBlueNatural8 first;
  Arcadia_Media_PixelAlphaRedGreenBlueNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37, 79, 173 };
  Arcadia_Media_PixelAlphaRedGreenBlueNatural8_set(&first, 11, 37, 79, 173);
  Arcadia_Media_PixelAlphaRedGreenBlueNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelAlphaRedGreenBlueNatural8_set(&second, 11, 37, 79, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelAlphaRedGreenBlueNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.a = (Arcadia_Natural8Value)(second.a + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.a = first.a;
  second.r = (Arcadia_Natural8Value)(second.r + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.r = first.r;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.b = (Arcadia_Natural8Value)(second.b + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.b = first.b;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelAlphaRedGreenBlueNatural8_isEqualTo(&first, &second));

}

static void
testBGR
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBlueGreenRedNatural8 first;
  Arcadia_Media_PixelBlueGreenRedNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37, 79 };
  Arcadia_Media_PixelBlueGreenRedNatural8_set(&first, 11, 37, 79);
  Arcadia_Media_PixelBlueGreenRedNatural8_set(&second, 11, 37, 79);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelBlueGreenRedNatural8_set(&second, 11, 37, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelBlueGreenRedNatural8_set(&second, 11, 37, 79);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.b = (Arcadia_Natural8Value)(second.b + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.b = first.b;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.r = (Arcadia_Natural8Value)(second.r + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));
  second.r = first.r;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBlueGreenRedNatural8_isEqualTo(&first, &second));

}

static void
testBGRA
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBlueGreenRedAlphaNatural8 first;
  Arcadia_Media_PixelBlueGreenRedAlphaNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37, 79, 173 };
  Arcadia_Media_PixelBlueGreenRedAlphaNatural8_set(&first, 11, 37, 79, 173);
  Arcadia_Media_PixelBlueGreenRedAlphaNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelBlueGreenRedAlphaNatural8_set(&second, 11, 37, 79, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelBlueGreenRedAlphaNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.b = (Arcadia_Natural8Value)(second.b + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  second.b = first.b;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.r = (Arcadia_Natural8Value)(second.r + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  second.r = first.r;
  second.a = (Arcadia_Natural8Value)(second.a + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));
  second.a = first.a;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBlueGreenRedAlphaNatural8_isEqualTo(&first, &second));

}

static void
testG
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelGrayNatural8 first;
  Arcadia_Media_PixelGrayNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11 };
  Arcadia_Media_PixelGrayNatural8_set(&first, 11);
  Arcadia_Media_PixelGrayNatural8_set(&second, 11);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelGrayNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelGrayNatural8_set(&second, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelGrayNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelGrayNatural8_set(&second, 11);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelGrayNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelGrayNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelGrayNatural8_isEqualTo(&first, &second));

}

static void
testGA
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelGrayAlphaNatural8 first;
  Arcadia_Media_PixelGrayAlphaNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37 };
  Arcadia_Media_PixelGrayAlphaNatural8_set(&first, 11, 37);
  Arcadia_Media_PixelGrayAlphaNatural8_set(&second, 11, 37);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelGrayAlphaNatural8_set(&second, 11, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelGrayAlphaNatural8_set(&second, 11, 37);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.a = (Arcadia_Natural8Value)(second.a + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo(&first, &second));
  second.a = first.a;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelGrayAlphaNatural8_isEqualTo(&first, &second));

}

static void
testRGB
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelRedGreenBlueNatural8 first;
  Arcadia_Media_PixelRedGreenBlueNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37, 79 };
  Arcadia_Media_PixelRedGreenBlueNatural8_set(&first, 11, 37, 79);
  Arcadia_Media_PixelRedGreenBlueNatural8_set(&second, 11, 37, 79);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelRedGreenBlueNatural8_set(&second, 11, 37, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelRedGreenBlueNatural8_set(&second, 11, 37, 79);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.r = (Arcadia_Natural8Value)(second.r + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.r = first.r;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.b = (Arcadia_Natural8Value)(second.b + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));
  second.b = first.b;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&first, &second));

}

static void
testRGBA
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelRedGreenBlueAlphaNatural8 first;
  Arcadia_Media_PixelRedGreenBlueAlphaNatural8 second;
  const Arcadia_Natural8Value expected[] = { 11, 37, 79, 173 };
  Arcadia_Media_PixelRedGreenBlueAlphaNatural8_set(&first, 11, 37, 79, 173);
  Arcadia_Media_PixelRedGreenBlueAlphaNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertEqualBytes(thread, expected, sizeof(expected), &first, sizeof(first));
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelRedGreenBlueAlphaNatural8_set(&second, 11, 37, 79, 255);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  Arcadia_Media_PixelRedGreenBlueAlphaNatural8_set(&second, 11, 37, 79, 173);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  // Every component must participate in equality, not just the final one.
  second.r = (Arcadia_Natural8Value)(second.r + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  second.r = first.r;
  second.g = (Arcadia_Natural8Value)(second.g + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  second.g = first.g;
  second.b = (Arcadia_Natural8Value)(second.b + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  second.b = first.b;
  second.a = (Arcadia_Natural8Value)(second.a + 1u);
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));
  second.a = first.a;
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueAlphaNatural8_isEqualTo(&first, &second));

}

static void
testInterpolation
  (
    Arcadia_Thread* thread
  )
{
  // The original signed-difference implementation failed these downward cases.
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(255, 0, 1) == 254);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(255, 0, 254) == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(0, 255, 1) == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(0, 255, 254) == 254);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(0, 255, 128) == 128);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(255, 0, 128) == 127);

  const Arcadia_Natural8Value inputs[] = { 0, 1, 17, 127, 128, 254, 255 };
  for (Arcadia_SizeValue i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
    for (Arcadia_SizeValue j = 0; j < sizeof(inputs) / sizeof(inputs[0]); ++j) {
      Arcadia_Natural8Value a = inputs[i];
      Arcadia_Natural8Value b = inputs[j];
      Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(a, b, 0) == a);
      Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelNatural8_lerp(a, b, 255) == b);
      Arcadia_Natural8Value previous = a;
      for (unsigned int step = 0; step <= 255; ++step) {
        Arcadia_Natural8Value t = (Arcadia_Natural8Value)step;
        Arcadia_Natural8Value value = Arcadia_Media_PixelNatural8_lerp(a, b, t);
        Arcadia_Natural8Value reversed = Arcadia_Media_PixelNatural8_lerp(b, a, (Arcadia_Natural8Value)(255 - step));
        Arcadia_Tests_assertTrue(thread, value == reversed);
        Arcadia_Tests_assertTrue(thread, value >= (a < b ? a : b));
        Arcadia_Tests_assertTrue(thread, value <= (a > b ? a : b));
        if (step != 0) {
          Arcadia_Tests_assertTrue(thread, a <= b ? value >= previous : value <= previous);
        }
        previous = value;
      }
    }
  }
}

int
Arcadia_Media_Tests_PixelBuffer_pixelValues
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&testABGR)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testAG)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testARGB)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testBGR)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testBGRA)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testG)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testGA)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testRGB)) return EXIT_FAILURE;
  if (!Arcadia_Tests_safeExecute(&testRGBA)) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int
Arcadia_Media_Tests_PixelBuffer_pixelValueInterpolation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testInterpolation) ? EXIT_SUCCESS : EXIT_FAILURE;
}
