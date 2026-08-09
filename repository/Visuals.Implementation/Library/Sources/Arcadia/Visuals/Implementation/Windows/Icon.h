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

#if !defined(ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_WINDOWS_ICON_H_INCLUDED)
#define ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_WINDOWS_ICON_H_INCLUDED

#include "Arcadia/Engine/Include.h"

#include <stdint.h>

#if !defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOCOMM)
  #define NOCOMM
#endif
#include <Windows.h>

/// @code
/// class Arcadia.Visuals.Windows.Icon {
///   construct(width:Integer32, height:Integer32, red:Natural8, green:Natural8, blue:Natural8, alpha:Natural8)
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Visuals.Implementation.Windows.Icon", Arcadia_Engine_Visuals_Implementation_Windows_Icon,
                          u8"Arcadia.Visuals.Icon");

struct Arcadia_Engine_Visuals_Implementation_Windows_IconDispatch {
  Arcadia_Engine_Visuals_IconDispatch _parent;
};

struct Arcadia_Engine_Visuals_Implementation_Windows_Icon {
  Arcadia_Engine_Visuals_Icon _parent;
  HICON hIcon;
};

Arcadia_Engine_Visuals_Implementation_Windows_Icon*
Arcadia_Engine_Visuals_Implementation_Windows_Icon_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* pixelBuffer
  );

#endif // ARCADIA_ENGINE_VISUALS_IMPLEMENTATION_WINDOWS_ICON_H_INCLUDED
