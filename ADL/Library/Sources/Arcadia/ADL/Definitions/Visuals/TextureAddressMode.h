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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"

// Enumeration of texture address modes for the U and V texture coordinates.
// The values correspond to the texture address modes of the engine.
// The value `None` is used as the default value and indicates that no
// address mode is specified and the engine's default address mode is used.
//
// @code
// enumeration Arcadia.ADL.TextureAddressMode {
//   None,
//   ClampToBorder,
//   ClampToEdge,
//   MirroredRepeat,
//   Repeat,
// };
// @endcode
Arcadia_declareEnumerationType(u8"Arcadia.ADL.TextureAddressMode", Arcadia_ADL_TextureAddressMode);

enum Arcadia_ADL_TextureAddressMode {
  // No address mode is specified. This is the default value.
  Arcadia_ADL_TextureAddressMode_None = 10,
  // Texture coordinates are clamped to the border.
  Arcadia_ADL_TextureAddressMode_ClampToBorder = 0,
  // Texture coordinates are clamped to the edge.
  Arcadia_ADL_TextureAddressMode_ClampToEdge = 1,
  // Texture coordinates are mirrored and then repeated.
  Arcadia_ADL_TextureAddressMode_MirroredRepeat = 2,
  // Texture coordinates are repeated.
  Arcadia_ADL_TextureAddressMode_Repeat = 3,
};

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED