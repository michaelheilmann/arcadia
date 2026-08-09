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

#if !defined(ARCADIA_MEDIA_VERTEXELEMENTSEMANTICS_H_INCLUDED)
#define ARCADIA_MEDIA_VERTEXELEMENTSEMANTICS_H_INCLUDED

#if !defined(ARCADIA_MEDIA_MODULE) || 1 != ARCADIA_MEDIA_MODULE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// https://michaelheilmann.com/Arcadia/Media/#Arcadia_Media_VertexElementSemantics
Arcadia_declareEnumerationType(u8"Arcadia.Media.VertexElementSemantics", Arcadia_Media_VertexElementSemantics);

enum Arcadia_Media_VertexElementSemantics {

  Arcadia_Media_VertexElementSemantics_PositionXYZ = 1,

  Arcadia_Media_VertexElementSemantics_AmbientRGBA = 2,

  Arcadia_Media_VertexElementSemantics_AmbientUV = 3,

};

#endif // ARCADIA_MEDIA_VERTEXELEMENTSEMANTICS_H_INCLUDED
