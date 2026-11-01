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

#if !defined(ARCADIA_ADL_DEFINITIONS_AUDIALS_STREAMINGSOUNDSOURCENODEDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_AUDIALS_STREAMINGSOUNDSOURCENODEDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"

Arcadia_declareObjectType(u8"Arcadia.ADL.StreamingSoundSourceNodeDefinition", Arcadia_ADL_StreamingSoundSourceNodeDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_StreamingSoundSourceNodeDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch parent;
};

struct Arcadia_ADL_StreamingSoundSourceNodeDefinition {
  Arcadia_ADL_Definition parent;
  Arcadia_ADL_Reference* provider;
  Arcadia_Real32Value volume;
};

Arcadia_ADL_StreamingSoundSourceNodeDefinition*
Arcadia_ADL_StreamingSoundSourceNodeDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_ADL_Reference* provider,
    Arcadia_Real32Value volume
  );

#endif
