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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREFILTER_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREFILTER_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"

// Enumeration of texture magnification and minification filters.
// The values correspond to the texture filters of the engine.
// The value `None` is used as the default value and indicates that no
// filter is specified and the engine's default filter is used.
//
// @code
// enumeration Arcadia.ADL.TextureFilter {
//   None,
//   Linear,
//   Nearest,
// };
// @endcode
Arcadia_declareEnumerationType(u8"Arcadia.ADL.TextureFilter", Arcadia_ADL_TextureFilter);

enum Arcadia_ADL_TextureFilter {
  // No filter is specified. This is the default value.
  Arcadia_ADL_TextureFilter_None = 10,
  // Linear filtering (e.g. bilinear interpolation).
  Arcadia_ADL_TextureFilter_Linear = 0,
  // Nearest-neighbor filtering.
  Arcadia_ADL_TextureFilter_Nearest = 1,
};

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREFILTER_H_INCLUDED