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

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"

// A DSP generating a sine wave signal of a constant frequency.
//
// @code
// {
//   type  : "DSP.SawtoothWave",
//   name  : "MyGame.MyDSPs.MySawtoothWave",
//   frequency : 220, /* Frequency in Hz. */   
// }
// @endcode
//
// @code
// class Arcadia.ADL.SawtoothWaveDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, name : Arcadia.String, frequency : Arcadia.Natural32)
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.SawtoothWaveDefinition", Arcadia_ADL_SawtoothWaveDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_SawtoothWaveDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_SawtoothWaveDefinition {
  Arcadia_ADL_Definition _parent;
  Arcadia_Integer32Value frequency;
};

Arcadia_ADL_SawtoothWaveDefinition*
Arcadia_ADL_SawtoothWaveDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Integer32Value frequency
  );

Arcadia_Integer32Value
Arcadia_ADL_SawtoothWaveDefinition_getFrequency
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SawtoothWaveDefinition* self
  );

#endif  // ARCADIA_ADL_DEFINITIONS_DSP_SAWTOOTHWAVEDEFINITION_H_INCLUDED
