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

#if !defined(ARCADIA_VISUALS_IMPLEMENTATION_LINUX_ICON_H_INCLUDED)
#define ARCADIA_VISUALS_IMPLEMENTATION_LINUX_ICON_H_INCLUDED

#include "Arcadia/Engine/Include.h"

/// @code
/// class Arcadia.Visuals.Linux.Icon {
///   construct(width:Integer32, height:Integer32, red:Natural8, green:Natural8, blue:Natural8, alpha:Natural8)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Visuals.Linux.Icon", Arcadia_Visuals_Linux_Icon,
                          u8"Arcadia.Engine.Visuals.Icon")

struct Arcadia_Visuals_Linux_IconDispatch {
  Arcadia_Engine_Visuals_IconDispatch _parent;
};

struct Arcadia_Visuals_Linux_Icon {
  Arcadia_Engine_Visuals_Icon _parent;
  /// The width, in pixels, of the icon.
  /// Always non-negative.
  Arcadia_Integer32Value width;
  /// The height, in pixels, of the icon.
  /// Always non-negative.
  Arcadia_Integer32Value height;
  /// Pointer to an array of (width * height + 2) * sizeof(unsigned long) Bytes.
  /// The first two Bytes contain the width and the height of a pixel rectangle.
  /// The remaining Bytes contain width * height unsigned long XXXXARGB pixels.
  unsigned long* bytes;
  /// The number of Bytes in the array pointed to by @a bytes.
  Arcadia_Natural32Value numberOfBytes;
};

Arcadia_Visuals_Linux_Icon*
Arcadia_Visuals_Linux_Icon_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* pixelBuffer
  );

#endif // ARCADIA_VISUALS_IMPLEMENTATION_LINUX_ICON_H_INCLUDED
