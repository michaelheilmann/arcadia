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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Audials/DSPFactory.h"

static Arcadia_Media_DSP*
createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* definition
  );

Arcadia_Media_DSP*
Arcadia_Engine_Audials_DSPFactory_createFromReference
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* reference
  )
{
  if (!reference || !reference->definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return createFromDefinition(thread, reference->definition);
}

static Arcadia_Media_DSP*
createFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* definition
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_SineWaveDefinition_getType(thread))) {
    Arcadia_ADL_SineWaveDefinition* sine = (Arcadia_ADL_SineWaveDefinition*)definition;
    return (Arcadia_Media_DSP*)Arcadia_Media_DSP_SineWave_create(thread, (Arcadia_Real32Value)sine->frequency);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread))) {
    Arcadia_ADL_SawtoothWaveDefinition* sawtooth = (Arcadia_ADL_SawtoothWaveDefinition*)definition;
    return (Arcadia_Media_DSP*)Arcadia_Media_DSP_SawtoothWave_create(thread, (Arcadia_Real32Value)sawtooth->frequency);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_WhiteNoiseDefinition_getType(thread))) {
    return (Arcadia_Media_DSP*)Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 1.0f);
  }
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_DSPOperatorDefinition_getType(thread))) {
    Arcadia_ADL_DSPOperatorDefinition* operatorDefinition = (Arcadia_ADL_DSPOperatorDefinition*)definition;
    switch (operatorDefinition->kind) {
      case Arcadia_ADL_DSPOperatorKind_Gain: {
        Arcadia_Media_DSP* input = createFromDefinition(thread, operatorDefinition->input->definition);
        return (Arcadia_Media_DSP*)Arcadia_Media_DSP_Gain_create(thread, input, operatorDefinition->parameter);
      }
      case Arcadia_ADL_DSPOperatorKind_LowPass: {
        Arcadia_Media_DSP* input = createFromDefinition(thread, operatorDefinition->input->definition);
        return (Arcadia_Media_DSP*)Arcadia_Media_DSP_LowPass_create(thread, input, operatorDefinition->parameter);
      }
      case Arcadia_ADL_DSPOperatorKind_Mix: {
        Arcadia_Media_DSP_Mix* mix = Arcadia_Media_DSP_Mix_create(thread);
        Arcadia_Object_lock(thread, (Arcadia_Object*)mix);
        for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)operatorDefinition->inputs); i < n; ++i) {
          Arcadia_ADL_Reference* inputReference = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueCheckedAt
            (
              thread,
              operatorDefinition->inputs,
              i,
              _Arcadia_ADL_Reference_getType(thread)
            );
          Arcadia_Media_DSP* input = createFromDefinition(thread, inputReference->definition);
          Arcadia_Object_lock(thread, (Arcadia_Object*)input);
          Arcadia_Media_DSP_Mix_addInput(thread, mix, input);
          Arcadia_Object_unlock(thread, (Arcadia_Object*)input);
        }
        Arcadia_Object_unlock(thread, (Arcadia_Object*)mix);
        return (Arcadia_Media_DSP*)mix;
      }
      default:
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
    }
  }
  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
  Arcadia_Thread_jump(thread);
}
