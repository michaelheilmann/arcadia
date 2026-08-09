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

#if !defined(ARCADIA_VISUALS_WINDOWS_BITMAP_H_INCLUDED)
#define ARCADIA_VISUALS_WINDOWS_BITMAP_H_INCLUDED

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Media/Include.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/// @code
/// class Arcadia.Visuals.Windows.Bitmap {
///   construct(width:Integer32, height: Integer32)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Visuals.Windows.Bitmap", Arcadia_Engine_Visuals_Windows_Bitmap,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Visuals_Windows_BitmapDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Engine_Visuals_Windows_Bitmap {
  Arcadia_Object _parent;
  HDC hDeviceContext;
  HBITMAP hBitmap;
  Arcadia_Integer32Value width;
  Arcadia_Integer32Value height;
  Arcadia_Integer32Value lineStride;
  Arcadia_Integer32Value linePadding;
  Arcadia_Integer32Value numberOfBitsPerPixel;
  Arcadia_Natural8Value pixelFormat;
};

Arcadia_Engine_Visuals_Windows_Bitmap*
Arcadia_Engine_Visuals_Windows_Bitmap_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

void
Arcadia_Engine_Visuals_Windows_Bitmap_fill
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b
  );

Arcadia_Media_PixelBuffer*
Arcadia_Engine_Visuals_Windows_Bitmap_toPixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_Bitmap* self
  );

#endif // ARCADIA_VISUALS_WINDOWS_BITMAP_H_INCLUDED
