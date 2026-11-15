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

#include "NamedColors.h"

#include "Arcadia/Media/Include.h"
#include "Arcadia/Testing/Include.h"

#include <stddef.h>
#include <stdlib.h>

_Static_assert(sizeof(Arcadia_Media_PixelRedGreenBlueNatural8) == 3, "RGB palette value must occupy three bytes");

static void
testNamedColors
  (
    Arcadia_Thread* thread
  )
{
  const Arcadia_Media_PixelRedGreenBlueNatural8 capri = Arcadia_Media_ColorPaletteNatural8_getCapri();
  const Arcadia_Media_PixelRedGreenBlueNatural8 malachite = Arcadia_Media_ColorPaletteNatural8_getMalachite();
  const Arcadia_Media_PixelRedGreenBlueNatural8 amber = Arcadia_Media_ColorPaletteNatural8_getAmber();

  // The original dx/core/math.h declared these values but did not define them.
  const Arcadia_Natural8Value expectedCapri[] = { 0, 191, 255 };
  const Arcadia_Natural8Value expectedMalachite[] = { 0, 255, 64 };
  const Arcadia_Natural8Value expectedAmber[] = { 255, 192, 0 };
  Arcadia_Tests_assertEqualBytes(thread, expectedCapri, sizeof(expectedCapri), &capri, sizeof(capri));
  Arcadia_Tests_assertEqualBytes(thread, expectedMalachite, sizeof(expectedMalachite), &malachite, sizeof(malachite));
  Arcadia_Tests_assertEqualBytes(thread, expectedAmber, sizeof(expectedAmber), &amber, sizeof(amber));

  Arcadia_Media_PixelRedGreenBlueNatural8 copy = Arcadia_Media_ColorPaletteNatural8_getCapri();
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&capri, &copy));
  copy.b = 0;
  Arcadia_Tests_assertFalse(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&capri, &copy));
  copy = Arcadia_Media_ColorPaletteNatural8_getCapri();
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelRedGreenBlueNatural8_isEqualTo(&capri, &copy));

  // Existing normalized color conversion must accept the palette values.
  Arcadia_Real32Value rgba[4] = { -1.f, -1.f, -1.f, -1.f };
  Arcadia_Media_PixelRedGreenBlueNatural8_toColor4Real32Values(&capri, 0.25f, rgba);
  Arcadia_Tests_assertTrue(thread, rgba[0] == 0.f);
  Arcadia_Tests_assertTrue(thread, rgba[1] == 191.f / 255.f);
  Arcadia_Tests_assertTrue(thread, rgba[2] == 1.f);
  Arcadia_Tests_assertTrue(thread, rgba[3] == 0.25f);

  Arcadia_Media_PixelRedGreenBlueNatural8_toColor4Real32Values(&malachite, 1.f, rgba);
  Arcadia_Tests_assertTrue(thread, rgba[0] == 0.f);
  Arcadia_Tests_assertTrue(thread, rgba[1] == 1.f);
  Arcadia_Tests_assertTrue(thread, rgba[2] == 64.f / 255.f);
  Arcadia_Tests_assertTrue(thread, rgba[3] == 1.f);

  Arcadia_Media_PixelRedGreenBlueNatural8_toColor4Real32Values(&amber, 0.f, rgba);
  Arcadia_Tests_assertTrue(thread, rgba[0] == 1.f);
  Arcadia_Tests_assertTrue(thread, rgba[1] == 192.f / 255.f);
  Arcadia_Tests_assertTrue(thread, rgba[2] == 0.f);
  Arcadia_Tests_assertTrue(thread, rgba[3] == 0.f);
}

int
Arcadia_Media_Tests_PixelBuffer_namedColors
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNamedColors) ? EXIT_SUCCESS : EXIT_FAILURE;
}
