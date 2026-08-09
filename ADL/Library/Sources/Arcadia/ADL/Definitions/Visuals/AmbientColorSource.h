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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_AMBIENTCOLORSOURCE_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_AMBIENTCOLORSOURCE_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"

// Enumeration of ambient color sources.
//
// @code
// enumeration Arcadia.ADL.AmbientSource {
//   Mesh,
//   Vertex,
//   Texture
// };
// @endcode
Arcadia_declareEnumerationType(u8"Arcadia.ADL.AmbientColorSource", Arcadia_ADL_AmbientColorSource);

enum Arcadia_ADL_AmbientColorSource {
  Arcadia_ADL_AmbientColorSource_Mesh,
  Arcadia_ADL_AmbientColorSource_Vertex,
  Arcadia_ADL_AmbientColorSource_Texture,
};

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_AMBIENTCOLORSOURCE_H_INCLUDED
