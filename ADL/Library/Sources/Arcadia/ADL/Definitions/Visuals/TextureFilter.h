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

#if !defined(ARCADIA_ADL_PRIVATE) || 1 != ARCADIA_ADL_PRIVATE
  #error("do not include `Arcadia/ADL/Definitions/Visuals/TextureFilter.h` directly, include `Arcadia/ADL/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareEnumerationType("Arcadia.ADL.TextureFilter",
                               Arcadia_ADL_TextureFilter);

enum Arcadia_ADL_TextureFilter {

  Arcadia_ADL_TextureFilter_None = 10,

  Arcadia_ADL_TextureFilter_Linear = 0,

  Arcadia_ADL_TextureFilter_Nearest = 1,

};

#endif // ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREFILTER_H_INCLUDED
