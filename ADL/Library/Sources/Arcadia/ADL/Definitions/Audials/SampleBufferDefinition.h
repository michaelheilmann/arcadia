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

#if !defined(ARCADIA_ADL_DEFINITIONS_AUDIALS_SAMPLEBUFFERDEFINITION_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_AUDIALS_SAMPLEBUFFERDEFINITION_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Reference.h"

// A definition of a sample buffer.
//
// @code
// {
//   type  : "SampleBuffer",
//   name  : "MyGame.MyScene.MySampleBuffer",
//   length : 2,
//   dsp : "MyGame.MyScene.MyDSP",
// }
// @endcode
//
// @code
// class Arcadia.ADL.SampleBufferDefinition extends Arcadia.ADL.Definition {
//
//   constructor(definitions : Arcadia.ADL.Definitions, qualifiedName : Arcadia.String)
//
// };
// @endcode
Arcadia_declareObjectType(u8"Arcadia.ADL.SampleBufferDefinition", Arcadia_ADL_SampleBufferDefinition,
                          u8"Arcadia.ADL.Definition");

struct Arcadia_ADL_SampleBufferDefinitionDispatch {
  Arcadia_ADL_DefinitionDispatch _parent;
};

struct Arcadia_ADL_SampleBufferDefinition {
  Arcadia_ADL_Definition _parent;
  /// The duration, in seconds, of the sample buffer.
  Arcadia_Natural32Value duration;
  /// Reference to the DSP to fill the sample buffer. 
  Arcadia_ADL_Reference* dsp;
};

Arcadia_ADL_SampleBufferDefinition*
Arcadia_ADL_SampleBufferDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Natural32Value duration,
    Arcadia_ADL_Reference* dsp
  );

#endif  // ARCADIA_ADL_DEFINITIONS_AUDIALS_SAMPLEBUFFERDEFINITION_H_INCLUDED
