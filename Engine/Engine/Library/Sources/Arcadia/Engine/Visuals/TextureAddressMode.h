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

#ifndef ARCADIA_ENGINE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED
#define ARCADIA_ENGINE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Visuals/TextureAddressMode.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareEnumerationType("Arcadia.Engine.Visuals.TextureAddressMode",
                               Arcadia_Engine_Visuals_TextureAddressMode);

enum Arcadia_Engine_Visuals_TextureAddressMode {

  Arcadia_Engine_Visuals_TextureAddressMode_ClampToBorder = 0,

  Arcadia_Engine_Visuals_TextureAddressMode_ClampToEdge = 1,

  Arcadia_Engine_Visuals_TextureAddressMode_MirroredRepeat = 2,

  Arcadia_Engine_Visuals_TextureAddressMode_Repeat = 3,

};

#endif // ARCADIA_ENGINE_VISUALS_TEXTUREADDRESSMODE_H_INCLUDED
