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

#include "Arcadia/Testing/Include.h"
#include "GraphDualTone.h"

#include <math.h>
#include <stdlib.h>

#include "Arcadia/Engine/Include.h"

static void
assertReal32Near
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value received,
    Arcadia_Real32Value expected
  )
{
  Arcadia_Tests_assertTrue(thread, fabsf(received - expected) < 0.0001f);
}

static Arcadia_Media_DSP_Buffer*
createLockedBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value sampleRate,
    Arcadia_SizeValue numberOfFrames
  )
{
  Arcadia_Media_DSP_Buffer* buffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfFrames);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  return buffer;
}

static void
testPartialFinalChunkAndContinuity
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* reference = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)reference);
  Arcadia_Media_DSP_SineWave* streamed = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)streamed);
  Arcadia_Engine_Audials_DSPStreamProvider* provider =
    Arcadia_Engine_Audials_DSPStreamProvider_create(thread, (Arcadia_Media_DSP*)streamed, 8, 10);
  Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
  Arcadia_Media_DSP_Buffer* referenceBuffer = createLockedBuffer(thread, 8, 10);
  Arcadia_Media_DSP_Buffer* chunk = createLockedBuffer(thread, 8, 4);

  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)reference, referenceBuffer);
  Arcadia_Real32Value* referenceSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, referenceBuffer);
  Arcadia_SizeValue expectedChunkSizes[] = { 4, 4, 2 };
  Arcadia_SizeValue offset = 0;
  for (Arcadia_SizeValue chunkIndex = 0; chunkIndex < 3; ++chunkIndex) {
    Arcadia_SizeValue framesWritten = 0;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
    Arcadia_Tests_assertTrue(thread, framesWritten == expectedChunkSizes[chunkIndex]);
    Arcadia_Real32Value* chunkSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, chunk);
    for (Arcadia_SizeValue i = 0; i < framesWritten; ++i) {
      assertReal32Near(thread, chunkSamples[i], referenceSamples[offset + i]);
    }
    offset += framesWritten;
  }
  Arcadia_Tests_assertTrue(thread, offset == 10);

  Arcadia_SizeValue framesWritten = 100;
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
  Arcadia_Tests_assertTrue(thread, framesWritten == 0);
  framesWritten = 100;
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
  Arcadia_Tests_assertTrue(thread, framesWritten == 0);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)referenceBuffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)streamed);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)reference);
}

static void
testExactChunkBoundary
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* sine = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sine);
  Arcadia_Engine_Audials_DSPStreamProvider* provider =
    Arcadia_Engine_Audials_DSPStreamProvider_create(thread, (Arcadia_Media_DSP*)sine, 8, 8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
  Arcadia_Media_DSP_Buffer* chunk = createLockedBuffer(thread, 8, 4);

  for (Arcadia_SizeValue i = 0; i < 2; ++i) {
    Arcadia_SizeValue framesWritten = 0;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
    Arcadia_Tests_assertTrue(thread, framesWritten == 4);
  }
  Arcadia_SizeValue framesWritten = 1;
  Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
  Arcadia_Tests_assertTrue(thread, framesWritten == 0);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sine);
}

static void
testInfiniteProvider
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* sine = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sine);
  Arcadia_Engine_Audials_DSPStreamProvider* provider =
    Arcadia_Engine_Audials_DSPStreamProvider_createInfinite(thread, (Arcadia_Media_DSP*)sine, 8);
  Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
  Arcadia_Media_DSP_Buffer* chunk = createLockedBuffer(thread, 8, 4);

  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    Arcadia_SizeValue framesWritten = 0;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)provider, chunk, &framesWritten));
    Arcadia_Tests_assertTrue(thread, framesWritten == 4);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sine);
}

static void
testAudialsNodeFactoryDefinitions
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.SineWave\", name: \"Factory.DSP\", frequency: 220, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"SampleBuffer\", name: \"Factory.Buffer\", duration: 1, dsp: \"Factory.DSP\", }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"SoundSourceNode\", name: \"Factory.Source\", sampleBuffer: \"Factory.Buffer\", isLooping: true, volume: 0.25, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.SineWave\", name: \"Factory.HighDSP\", frequency: 110, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.Gain\", name: \"Factory.LowGain\", input: \"Factory.DSP\", gain: 0.10, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.Gain\", name: \"Factory.HighGain\", input: \"Factory.HighDSP\", gain: 0.035, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.WhiteNoise\", name: \"Factory.Noise\", }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.LowPass\", name: \"Factory.Filter\", input: \"Factory.Noise\", cutoff: 850, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.Gain\", name: \"Factory.AirGain\", input: \"Factory.Filter\", gain: 0.025, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSP.Mix\", name: \"Factory.Mix\", inputs: [ \"Factory.LowGain\", \"Factory.HighGain\", \"Factory.AirGain\" ], }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSPStreamProvider\", name: \"Factory.InfiniteProvider\", dsp: \"Factory.Mix\", sampleRate: 48000, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"StreamingSoundSourceNode\", name: \"Factory.InfiniteSource\", provider: \"Factory.InfiniteProvider\", volume: 0.5, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"DSPStreamProvider\", name: \"Factory.FiniteProvider\", dsp: \"Factory.DSP\", sampleRate: 48000, duration: 2, }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, u8"{ type: \"StreamingSoundSourceNode\", name: \"Factory.FiniteSource\", provider: \"Factory.FiniteProvider\", }"), Arcadia_BooleanValue_False);
  Arcadia_ADL_Definitions_link(thread, definitions);

  Arcadia_Engine_Audials_NodeFactory* factory = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)factory);
  Arcadia_ADL_SoundSourceNodeDefinition* finiteBufferDefinition =
    (Arcadia_ADL_SoundSourceNodeDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Factory.Source"));
  Arcadia_Engine_Audials_SoundSourceNode* finiteBufferNode =
    Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition(thread, factory, NULL, finiteBufferDefinition);
  Arcadia_Object_lock(thread, (Arcadia_Object*)finiteBufferNode);
  Arcadia_Tests_assertTrue(thread, (Arcadia_ADL_Definition*)finiteBufferNode->source == finiteBufferDefinition->sampleBuffer->definition);
  Arcadia_Tests_assertTrue(thread, finiteBufferNode->isLooping == Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, finiteBufferNode->volume == 0.25f);

  Arcadia_ADL_StreamingSoundSourceNodeDefinition* infiniteDefinition =
    (Arcadia_ADL_StreamingSoundSourceNodeDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Factory.InfiniteSource"));
  Arcadia_Engine_Audials_StreamingSoundSourceNode* infiniteNode =
    Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition(thread, factory, NULL, infiniteDefinition);
  Arcadia_Object_lock(thread, (Arcadia_Object*)infiniteNode);
  Arcadia_Engine_Audials_DSPStreamProvider* infiniteProvider = (Arcadia_Engine_Audials_DSPStreamProvider*)infiniteNode->provider;
  Arcadia_Tests_assertTrue(thread, infiniteProvider->isInfinite == Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, infiniteProvider->sampleRate == 48000);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)infiniteProvider->dsp, _Arcadia_Media_DSP_Mix_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Media_DSP_Mix*)infiniteProvider->dsp)->inputs) == 3);
  Arcadia_Tests_assertTrue(thread, infiniteNode->volume == 0.5f);

  Arcadia_ADL_StreamingSoundSourceNodeDefinition* finiteStreamDefinition =
    (Arcadia_ADL_StreamingSoundSourceNodeDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, u8"Factory.FiniteSource"));
  Arcadia_Engine_Audials_StreamingSoundSourceNode* finiteStreamNode =
    Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition(thread, factory, NULL, finiteStreamDefinition);
  Arcadia_Object_lock(thread, (Arcadia_Object*)finiteStreamNode);
  Arcadia_Engine_Audials_DSPStreamProvider* finiteProvider = (Arcadia_Engine_Audials_DSPStreamProvider*)finiteStreamNode->provider;
  Arcadia_Tests_assertTrue(thread, finiteProvider->isInfinite == Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, finiteProvider->framesRemaining == 96000);
  Arcadia_Tests_assertTrue(thread, finiteStreamNode->volume == 1.f);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)finiteStreamNode);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)infiniteNode);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)finiteBufferNode);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)factory);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)definitions);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  testPartialFinalChunkAndContinuity(thread);
  testExactChunkBoundary(thread);
  testInfiniteProvider(thread);
  testAudialsNodeFactoryDefinitions(thread);
}

static int
runTests
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "AudialsStreaming", &runTests },
    { "GraphDualTone", &Arcadia_Engine_Tests_AudialsStreaming_graphDualTone },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
