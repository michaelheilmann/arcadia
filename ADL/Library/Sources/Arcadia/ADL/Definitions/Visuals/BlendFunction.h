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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_BLENDFUNCTION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_BLENDFUNCTION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"

// Enumeration of blend functions.
// The values correspond to the blend functions of the engine and to the blend factors of OpenGL.
// The value `None` is used as the default value and indicates that blending is disabled and
// hence no blending functions are applied.
//
// @code
// enumeration Arcadia.ADL.BlendFunction {
//   None,
//   Zero,
//   One,
//   SourceColor,
//   OneMinusSourceColor,
//   DestinationColor,
//   OneMinusDestinationColor,
//   SourceAlpha,
//   OneMinusSourceAlpha,
//   DestinationAlpha,
//   OneMinusDestinationAlpha,
// };
// @endcode
Arcadia_declareEnumerationType(u8"Arcadia.ADL.BlendFunction", Arcadia_ADL_BlendFunction);

enum Arcadia_ADL_BlendFunction {
  // Blending is disabled. This is the default value.
  Arcadia_ADL_BlendFunction_None = 10,
  Arcadia_ADL_BlendFunction_Zero = 0,
  Arcadia_ADL_BlendFunction_One = 1,
  Arcadia_ADL_BlendFunction_SourceColor = 2,
  Arcadia_ADL_BlendFunction_OneMinusSourceColor = 3,
  Arcadia_ADL_BlendFunction_DestinationColor = 4,
  Arcadia_ADL_BlendFunction_OneMinusDestinationColor = 5,
  Arcadia_ADL_BlendFunction_SourceAlpha = 6,
  Arcadia_ADL_BlendFunction_OneMinusSourceAlpha = 7,
  Arcadia_ADL_BlendFunction_DestinationAlpha = 8,
  Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha = 9,
};

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_BLENDFUNCTION_H_INCLUDED