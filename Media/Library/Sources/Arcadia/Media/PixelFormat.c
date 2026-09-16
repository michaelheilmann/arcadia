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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/PixelFormat.h"

#include "Arcadia/Ring2/Include.h"

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
};

Arcadia_defineEnumerationType(u8"Arcadia.Media.PixelFormat", Arcadia_Media_PixelFormat,
                              &_typeOperations);

Arcadia_SizeValue
Arcadia_Media_PixelFormat_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelFormat self
  )
{
  switch (self) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8:
      return 4;
    case Arcadia_Media_PixelFormat_AlphaGrayNatural8:
      return 2;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8:
      return 4;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8:
      return 3;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8:
      return 4;
    case Arcadia_Media_PixelFormat_GrayNatural8:
      return 1;
    case Arcadia_Media_PixelFormat_GrayAlphaNatural8:
      return 2;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8:
      return 3;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8:
      return 4;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}

Arcadia_SizeValue
Arcadia_Media_PixelFormat_getNumberOfComponents
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelFormat self
  )
{
  switch (self) {
    case Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8:
      return 3;
    case Arcadia_Media_PixelFormat_AlphaGrayNatural8:
      return 2;
    case Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8:
      return 4;
    case Arcadia_Media_PixelFormat_BlueGreenRedNatural8:
      return 3;
    case Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8:
      return 4;
    case Arcadia_Media_PixelFormat_GrayNatural8:
      return 1;
    case Arcadia_Media_PixelFormat_GrayAlphaNatural8:
      return 2;
    case Arcadia_Media_PixelFormat_RedGreenBlueNatural8:
      return 3;
    case Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8:
      return 4;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
}
