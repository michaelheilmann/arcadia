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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Definitions/Visuals/TextureAddressMode.h"
#include "Arcadia/ADL/Definitions/Visuals/TextureFilter.h"
#include "Arcadia/ADL/Reference.h"

// The definition of a texture.
//
// @code
// {
//   type  : "Texture",
//   name  : "MyGame.Textures.MyTexture",
//   pixelBuffer : "MyGame.PixelBuffers.MyPixelBuffer",
// }
// @endcode
//
// The values of the sampling properties `magnificationFilter`, `minificationFilter`,
// `addressModeU`, and `addressModeV` are optional. The value `None` is the default
// value and indicates that no sampling property is specified and the engine's
// default is used.
//
// @code
// {
//   type  : "Texture",
//   name  : "MyGame.Textures.MyTexture",
//   pixelBuffer : "MyGame.PixelBuffers.MyPixelBuffer",
//   magnificationFilter : "Nearest",
//   minificationFilter : "Nearest",
//   addressModeU : "Repeat",
//   addressModeV : "MirroredRepeat",
// }
// @endcode
//
// @code
// class Arcadia.ADL.TextureDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, name, pixelBufferName : Arcadia.String,
//               magnificationFilter : Arcadia.ADL.TextureFilter,
//               minificationFilter : Arcadia.ADL.TextureFilter,
//               addressModeU : Arcadia.ADL.TextureAddressMode,
//               addressModeV : Arcadia.ADL.TextureAddressMode)
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.TextureDefinition", Arcadia_ADL_TextureDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_TextureDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_TextureDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_ADL_Reference* pixelBuffer;
  // The magnification filter. Default is `Arcadia_ADL_TextureFilter_None`.
  Arcadia_ADL_TextureFilter magnificationFilter;
  // The minification filter. Default is `Arcadia_ADL_TextureFilter_None`.
  Arcadia_ADL_TextureFilter minificationFilter;
  // The texture address mode of the U texture coordinate. Default is `Arcadia_ADL_TextureAddressMode_None`.
  Arcadia_ADL_TextureAddressMode addressModeU;
  // The texture address mode of the V texture coordinate. Default is `Arcadia_ADL_TextureAddressMode_None`.
  Arcadia_ADL_TextureAddressMode addressModeV;
};

Arcadia_ADL_TextureDefinition*
Arcadia_ADL_TextureDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_String* pixelBufferName,
    Arcadia_ADL_TextureFilter magnificationFilter,
    Arcadia_ADL_TextureFilter minificationFilter,
    Arcadia_ADL_TextureAddressMode addressModeU,
    Arcadia_ADL_TextureAddressMode addressModeV
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_TEXTUREDEFINITION_H_INCLUDED
