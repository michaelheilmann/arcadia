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

#if !defined(ARCADIA_MEDIA_PIXELFORMAT_H_INCLUDED)
#define ARCADIA_MEDIA_PIXELFORMAT_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/PixelFormat.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareEnumerationType("Arcadia.Media.PixelFormat",
                               Arcadia_Media_PixelFormat);

enum Arcadia_Media_PixelFormat {

  Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8 = 0,

  Arcadia_Media_PixelFormat_AlphaGrayNatural8 = 1,

  Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8 = 2,

  Arcadia_Media_PixelFormat_BlueGreenRedNatural8 = 3,

  Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8 = 4,

  Arcadia_Media_PixelFormat_GrayNatural8 = 5,

  Arcadia_Media_PixelFormat_GrayAlphaNatural8 = 6,

  Arcadia_Media_PixelFormat_RedGreenBlueNatural8 = 7,

  Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8 = 8,

};

#endif // ARCADIA_MEDIA_PIXELFORMAT_H_INCLUDED
