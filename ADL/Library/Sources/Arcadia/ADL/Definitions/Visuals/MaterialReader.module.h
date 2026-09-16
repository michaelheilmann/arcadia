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

#if !defined(ARCADIA_ADL_DEFINITIONS_VISUALS_MATERIALREADER_MODULE_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_VISUALS_MATERIALREADER_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Reader.h"
#include "Arcadia/DDLS/Include.h"

Arcadia_declareObjectType(u8"Arcadia.ADL.MaterialReader", Arcadia_ADL_MaterialReader,
                          u8"Arcadia.ADL.Reader");

struct Arcadia_ADL_MaterialReaderDispatch {
  Arcadia_ADL_ReaderDispatch _parent;
};

struct Arcadia_ADL_MaterialReader {
  Arcadia_ADL_Reader _parent;

  // The string `type` cached here.
  Arcadia_String* TYPE;
  // The string `name` cached here.
  Arcadia_String* NAME;
  // The string `Material` cached here.
  Arcadia_String* SCHEMANAME;
  // The string `Material` cached here.
  Arcadia_String* TYPENAME;

  // The string `materialType`.
  Arcadia_String* MATERIALTYPE;
  // The string `Blinn-Phong`.
  Arcadia_String* BLINNPHONG;

  // The string `ambientColorSource`.
  Arcadia_String* AMBIENTCOLORSOURCE;
  // The string `Mesh`.
  Arcadia_String* MESH;
  // The string `Vertex`.
  Arcadia_String* VERTEX;
  // The string `Texture`.
  Arcadia_String* TEXTURE;

  // The string `ambientColorTexture`.
  Arcadia_String* AMBIENTCOLORTEXTURE;

  // The string `blendSourceFunction`.
  Arcadia_String* BLENDSOURCEFUNCTION;
  // The string `blendDestinationFunction`.
  Arcadia_String* BLENDDESTINATIONFUNCTION;

  // The string `Zero`.
  Arcadia_String* ZERO;
  // The string `One`.
  Arcadia_String* ONE;
  // The string `SourceColor`.
  Arcadia_String* SOURCECOLOR;
  // The string `OneMinusSourceColor`.
  Arcadia_String* ONEMINUSSOURCECOLOR;
  // The string `DestinationColor`.
  Arcadia_String* DESTINATIONCOLOR;
  // The string `OneMinusDestinationColor`.
  Arcadia_String* ONEMINUSDESTINATIONCOLOR;
  // The string `SourceAlpha`.
  Arcadia_String* SOURCEALPHA;
  // The string `OneMinusSourceAlpha`.
  Arcadia_String* ONEMINUSSOURCEALPHA;
  // The string `DestinationAlpha`.
  Arcadia_String* DESTINATIONALPHA;
  // The string `OneMinusDestinationAlpha`.
  Arcadia_String* ONEMINUSDESTINATIONALPHA;

  // The validation context. Cached here.
  Arcadia_DDLS_ValidationContext* validationContext;
};

Arcadia_ADL_MaterialReader*
Arcadia_ADL_MaterialReader_create
  (
    Arcadia_Thread* thread
  );

#endif  // ARCADIA_ADL_DEFINITIONS_VISUALS_MATERIALREADER_MODULE_H_INCLUDED
