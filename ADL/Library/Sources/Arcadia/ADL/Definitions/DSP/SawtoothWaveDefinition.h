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

#if !defined(ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_PRIVATE) || 1 != ARCADIA_ADL_PRIVATE
  #error("do not include `Arcadia/ADL/Definitions/DSP/SawtoothWaveDefinition.h` directly, include `Arcadia/ADL/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/ADL/Definition.h"

typedef struct Arcadia_ADL_Definitions Arcadia_ADL_Definitions;
typedef struct Arcadia_ADL_SawtoothWaveDefinition Arcadia_ADL_SawtoothWaveDefinition;

Arcadia_declareObjectType(u8"Arcadia.ADL.SawtoothWaveDefinition", Arcadia_ADL_SawtoothWaveDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_SawtoothWaveDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_SawtoothWaveDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_Integer32Value frequency;
};

void
Arcadia_ADL_SawtoothWaveDefinition_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
  );

Arcadia_Integer32Value
Arcadia_ADL_SawtoothWaveDefinition_getFrequency
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
  );

Arcadia_ADL_SawtoothWaveDefinition*
Arcadia_ADL_SawtoothWaveDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value frequency
  );

#endif // ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEDEFINITION_H_INCLUDED
