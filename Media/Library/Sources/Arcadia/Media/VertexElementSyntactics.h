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

#ifndef ARCADIA_MEDIA_VERTEXELEMENTSYNTACTICS_H_INCLUDED
#define ARCADIA_MEDIA_VERTEXELEMENTSYNTACTICS_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/VertexElementSyntactics.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareEnumerationType("Arcadia.Media.VertexElementSyntactics",
                               Arcadia_Media_VertexElementSyntactics);

enum Arcadia_Media_VertexElementSyntactics {

  Arcadia_Media_VertexElementSyntactics_Real32 = 1,

  Arcadia_Media_VertexElementSyntactics_Real32Real32 = 2,

  Arcadia_Media_VertexElementSyntactics_Real32Real32Real32 = 3,

  Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32 = 4,

};

#endif // ARCADIA_MEDIA_VERTEXELEMENTSYNTACTICS_H_INCLUDED
