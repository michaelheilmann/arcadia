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

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Visuals/Implementation/Linux/Icon.h"
#include <stdlib.h>

static void
testArgbPayload
  (
    Arcadia_Thread* thread
  )
{
  // A padded 2x2 image tests red/blue order, alpha and row placement.
  Arcadia_Media_PixelBuffer* pixels =
    Arcadia_Media_PixelBuffer_create(thread, 3, 2, 2,
                                     Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 0, 255, 0, 0, 255);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 0, 0, 0, 255, 128);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 0, 1, 0, 255, 0, 0);
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, pixels, 1, 1, 16, 32, 64, 64);

  Arcadia_Visuals_Linux_Icon* icon = Arcadia_Visuals_Linux_Icon_create(thread, pixels);
  Arcadia_Object_lock(thread, (Arcadia_Object*)icon);
  Arcadia_Tests_assertEqualInteger64(thread, 2, icon->width);
  Arcadia_Tests_assertEqualInteger64(thread, 2, icon->height);
  Arcadia_Tests_assertEqualSize(thread, 6 * sizeof(unsigned long), icon->numberOfBytes);
  Arcadia_Tests_assertTrue(thread, icon->bytes != NULL);
  Arcadia_Tests_assertEqualNatural64(thread, 2, icon->bytes[0]);
  Arcadia_Tests_assertEqualNatural64(thread, 2, icon->bytes[1]);
  Arcadia_Tests_assertEqualNatural64(thread, 0xffff0000UL, icon->bytes[2]);
  Arcadia_Tests_assertEqualNatural64(thread, 0x800000ffUL, icon->bytes[3]);
  Arcadia_Tests_assertEqualNatural64(thread, 0x0000ff00UL, icon->bytes[4]);
  Arcadia_Tests_assertEqualNatural64(thread, 0x40102040UL, icon->bytes[5]);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)icon);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
}

static void
testZeroDimensions
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_PixelBuffer* pixels =
    Arcadia_Media_PixelBuffer_create(thread, 0, 0, 1,
                                     Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)pixels);
  Arcadia_JumpTarget target;
  Arcadia_Status status = Arcadia_Status_Success;
  Arcadia_Thread_pushJumpTarget(thread, &target);
  if (Arcadia_JumpTarget_save(&target)) {
    Arcadia_Visuals_Linux_Icon_create(thread, pixels);
  } else {
    status = Arcadia_Thread_getStatus(thread);
  }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)pixels);
}

static int
runArgbPayload
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testArgbPayload) ? EXIT_SUCCESS : EXIT_FAILURE; }

static int
runZeroDimensions
  (
    void
  )
{ return Arcadia_Tests_safeExecute(&testZeroDimensions) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "ArgbPayload", &runArgbPayload },
    { "ZeroDimensions", &runZeroDimensions },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
