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

#if !defined(ARCADIA_VPL_PROGRAMS_H_INCLUDED)
#define ARCADIA_VPL_PROGRAMS_H_INCLUDED

#include "Arcadia/VPL/VerifiedModule.h"

#define Arcadia_VPL_ProgramFlags_MeshAmbientColor (0)
#define Arcadia_VPL_ProgramFlags_VertexAmbientColor (1)
#define Arcadia_VPL_ProgramFlags_TextureAmbientColor (2)
#define Arcadia_VPL_ProgramFlags_TextureVertexAmbientColor (3)

/// Read, verify, and publish a checked-in .vpl source embedded in the library at build time.
Arcadia_VPL_VerifiedModule*
Arcadia_VPL_Programs_create
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value flags
  );

#endif // ARCADIA_VPL_PROGRAMS_H_INCLUDED
