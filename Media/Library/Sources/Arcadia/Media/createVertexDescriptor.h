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

#if !defined(ARCADIA_MEDIA_CREATEVERTEXDESCRIPTOR_H_INCLUDED)
#define ARCADIA_MEDIA_CREATEVERTEXDESCRIPTOR_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include `Arcadia/Media/createVertexDescriptor.h` directly, include `Arcadia/Media/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

typedef struct Arcadia_List Arcadia_List;
typedef struct Arcadia_Media_VertexDescriptor Arcadia_Media_VertexDescriptor;

Arcadia_Media_VertexDescriptor*
Arcadia_Media_createVertexDescriptor
  (
    Arcadia_Thread* thread,
    Arcadia_List* elements,
    Arcadia_Natural64Value stride
  );

#endif // ARCADIA_MEDIA_CREATEVERTEXDESCRIPTOR_H_INCLUDED
