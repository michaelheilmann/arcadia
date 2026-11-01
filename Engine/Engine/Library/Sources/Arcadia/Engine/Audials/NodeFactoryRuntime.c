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
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Engine/Audials/Nodes/SoundSourceNode.h"
#include "Arcadia/Engine/Audials/DSPFactory.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/Include.h"
#include <stdint.h>

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* source
  )
{ return Arcadia_Engine_Audials_SoundSourceNode_create(thread, backendContext, source); }

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinitionNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SoundSourceNodeDefinition* definition
  )
{
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition->sampleBuffer->definition, _Arcadia_ADL_SampleBufferDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Engine_Audials_SoundSourceNode* node =
    Arcadia_Engine_Audials_SoundSourceNode_create(thread, backendContext, (Arcadia_ADL_SampleBufferDefinition*)definition->sampleBuffer->definition);
  Arcadia_Engine_Audials_SoundSourceNode_setIsLooping(thread, node, definition->isLooping);
  Arcadia_Engine_Audials_SoundSourceNode_setVolume(thread, node, definition->volume);
  return node;
}

Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinitionNative
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_StreamingSoundSourceNodeDefinition* definition
  )
{
  Arcadia_ADL_Definition* providerDefinition = definition->provider->definition;
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)providerDefinition, _Arcadia_ADL_DSPStreamProviderDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_DSPStreamProviderDefinition* providerDefinitionTyped = (Arcadia_ADL_DSPStreamProviderDefinition*)providerDefinition;
  Arcadia_Media_DSP* dsp = Arcadia_Engine_Audials_DSPFactory_createFromReference(thread, providerDefinitionTyped->dsp);
  Arcadia_Engine_Audials_DSPStreamProvider* provider = NULL;
  if (providerDefinitionTyped->hasDuration) {
    Arcadia_SizeValue duration = providerDefinitionTyped->duration;
    Arcadia_SizeValue sampleRate = providerDefinitionTyped->sampleRate;
    if (sampleRate && duration > SIZE_MAX / sampleRate) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    }
    provider = Arcadia_Engine_Audials_DSPStreamProvider_create(thread, dsp, providerDefinitionTyped->sampleRate, duration * sampleRate);
  } else {
    provider = Arcadia_Engine_Audials_DSPStreamProvider_createInfinite(thread, dsp, providerDefinitionTyped->sampleRate);
  }
  Arcadia_Engine_Audials_StreamingSoundSourceNode* node =
    Arcadia_Engine_Audials_StreamingSoundSourceNode_create(thread, backendContext, (Arcadia_Engine_Audials_AudioStreamProvider*)provider);
  Arcadia_Engine_Audials_StreamingSoundSourceNode_setVolume(thread, node, definition->volume);
  return node;
}
