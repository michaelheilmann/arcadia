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
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelFormat
Arcadia_declareEnumerationType(u8"Arcadia.Media.PixelFormat", Arcadia_Media_PixelFormat);

enum Arcadia_Media_PixelFormat {

  Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8,

  Arcadia_Media_PixelFormat_AlphaGrayNatural8,

  Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,

  Arcadia_Media_PixelFormat_BlueGreenRedNatural8,

  Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,

  Arcadia_Media_PixelFormat_GrayNatural8,

  Arcadia_Media_PixelFormat_GrayAlphaNatural8,

  Arcadia_Media_PixelFormat_RedGreenBlueNatural8,

  Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,

};

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelFormat_getNumberOfBytes
Arcadia_SizeValue
Arcadia_Media_PixelFormat_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelFormat self
  );

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_PixelFormat_getNumberComponents
Arcadia_SizeValue
Arcadia_Media_PixelFormat_getNumberOfComponents
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelFormat self
  );

#endif // ARCADIA_MEDIA_PIXELFORMAT_H_INCLUDED
