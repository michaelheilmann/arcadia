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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Arcadia/Media/Include.h"

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
    Arcadia_SizeValue numberOfSamples
  )
{
  Arcadia_Media_DSP_Buffer* buffer = Arcadia_Media_DSP_Buffer_create(thread, sampleRate, numberOfSamples);
  Arcadia_Object_lock(thread, (Arcadia_Object*)buffer);
  return buffer;
}

static void
expectArgumentValueInvalid
  (
    Arcadia_Thread* thread,
    void (*function)(Arcadia_Thread*, void*),
    void* context
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_BooleanValue receivedJump = Arcadia_BooleanValue_False;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    function(thread, context);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    receivedJump = Arcadia_BooleanValue_True;
  }
  Arcadia_Tests_assertTrue(thread, receivedJump);
  Arcadia_Tests_assertTrue(thread, Arcadia_Status_ArgumentValueInvalid == Arcadia_Thread_getStatus(thread));
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
}

struct GraphNodeContext {
  Arcadia_Media_DSP_Graph* graph;
  Arcadia_Media_DSP* node;
};

struct RenderGraphContext {
  Arcadia_Media_DSP_Graph* graph;
  Arcadia_Media_DSP_Buffer* buffer;
};

static void
addNodeContext
  (
    Arcadia_Thread* thread,
    void* context
  )
{
  struct GraphNodeContext* graphNodeContext = (struct GraphNodeContext*)context;
  Arcadia_Media_DSP_Graph_addNode(thread, graphNodeContext->graph, graphNodeContext->node);
}

static void
setOutputContext
  (
    Arcadia_Thread* thread,
    void* context
  )
{
  struct GraphNodeContext* graphNodeContext = (struct GraphNodeContext*)context;
  Arcadia_Media_DSP_Graph_setOutput(thread, graphNodeContext->graph, graphNodeContext->node);
}

static void
renderGraphContext
  (
    Arcadia_Thread* thread,
    void* context
  )
{
  struct RenderGraphContext* renderGraphContext = (struct RenderGraphContext*)context;
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)renderGraphContext->graph, renderGraphContext->buffer);
}

static void
createInvalidLowPassContext
  (
    Arcadia_Thread* thread,
    void* context
  )
{
  Arcadia_Media_DSP* input = (Arcadia_Media_DSP*)context;
  Arcadia_Media_DSP_LowPass_create(thread, input, 0.0f);
}

static void
testBuffer
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 48000, 4);
  Arcadia_Tests_assertTrue(thread, 48000 == Arcadia_Media_DSP_Buffer_getSampleRate(thread, buffer));
  Arcadia_Tests_assertTrue(thread, 4 == Arcadia_Media_DSP_Buffer_getNumberOfSamples(thread, buffer));
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    assertReal32Near(thread, samples[i], 0.0f);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
}

static void
testSawtoothWave
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)saw);
  Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)saw, buffer);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
  assertReal32Near(thread, samples[0], -0.5f);
  assertReal32Near(thread, samples[1], -0.25f);
  assertReal32Near(thread, samples[2], 0.0f);
  assertReal32Near(thread, samples[3], 0.25f);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)saw);
}

static void
testSineWaveValues
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* sine = Arcadia_Media_DSP_SineWave_create(thread, 440.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)sine);
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)sine));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)sine));
  Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 1760, 4);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)sine, buffer);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
  assertReal32Near(thread, samples[0], 0.0f);
  assertReal32Near(thread, samples[1], 1.0f);
  assertReal32Near(thread, samples[2], 0.0f);
  assertReal32Near(thread, samples[3], -1.0f);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)sine);
}

static void
testSineWaveContinuity
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SineWave* single = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)single);
  Arcadia_Media_DSP_SineWave* chunked = Arcadia_Media_DSP_SineWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)chunked);
  Arcadia_Media_DSP_Buffer* singleBuffer = createLockedBuffer(thread, 8, 8);
  Arcadia_Media_DSP_Buffer* firstChunk = createLockedBuffer(thread, 8, 4);
  Arcadia_Media_DSP_Buffer* secondChunk = createLockedBuffer(thread, 8, 4);

  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)single, singleBuffer);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)chunked, firstChunk);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)chunked, secondChunk);

  Arcadia_Real32Value* singleSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, singleBuffer);
  Arcadia_Real32Value* firstSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, firstChunk);
  Arcadia_Real32Value* secondSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, secondChunk);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    assertReal32Near(thread, singleSamples[i], firstSamples[i]);
    assertReal32Near(thread, singleSamples[i + 4], secondSamples[i]);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)secondChunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)firstChunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)singleBuffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunked);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)single);
}

static void
testSawtoothWaveContinuity
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SawtoothWave* single = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)single);
  Arcadia_Media_DSP_SawtoothWave* chunked = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)chunked);
  Arcadia_Media_DSP_Buffer* singleBuffer = createLockedBuffer(thread, 8, 8);
  Arcadia_Media_DSP_Buffer* firstChunk = createLockedBuffer(thread, 8, 4);
  Arcadia_Media_DSP_Buffer* secondChunk = createLockedBuffer(thread, 8, 4);

  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)single, singleBuffer);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)chunked, firstChunk);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)chunked, secondChunk);

  Arcadia_Real32Value* singleSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, singleBuffer);
  Arcadia_Real32Value* firstSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, firstChunk);
  Arcadia_Real32Value* secondSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, secondChunk);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    assertReal32Near(thread, singleSamples[i], firstSamples[i]);
    assertReal32Near(thread, singleSamples[i + 4], secondSamples[i]);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)secondChunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)firstChunk);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)singleBuffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)chunked);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)single);
}

static void
testGainAffineMultiplyAndMix
  (
    Arcadia_Thread* thread
  )
{
  {
    Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)saw);
    Arcadia_Media_DSP_Gain* gain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)saw, 2.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)gain);
    Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
    Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)gain, buffer);
    Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
    assertReal32Near(thread, samples[0], -1.0f);
    assertReal32Near(thread, samples[1], -0.5f);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)gain);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)saw);
  }
  {
    Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)saw);
    Arcadia_Media_DSP_Affine* affine = Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)saw, 2.0f, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)affine);
    Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
    Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)affine, buffer);
    Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
    assertReal32Near(thread, samples[0], 0.0f);
    assertReal32Near(thread, samples[1], 0.5f);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)affine);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)saw);
  }
  {
    Arcadia_Media_DSP_SawtoothWave* left = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)left);
    Arcadia_Media_DSP_SawtoothWave* right = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)right);
    Arcadia_Media_DSP_Affine* affine = Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)right, 2.0f, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)affine);
    Arcadia_Media_DSP_Multiply* multiply = Arcadia_Media_DSP_Multiply_create(thread, (Arcadia_Media_DSP*)left, (Arcadia_Media_DSP*)affine);
    Arcadia_Object_lock(thread, (Arcadia_Object*)multiply);
    Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
    Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)multiply, buffer);
    Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
    assertReal32Near(thread, samples[0], 0.0f);
    assertReal32Near(thread, samples[1], -0.125f);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)multiply);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)affine);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)right);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)left);
  }
  {
    Arcadia_Media_DSP_SawtoothWave* sawA = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)sawA);
    Arcadia_Media_DSP_SawtoothWave* sawB = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)sawB);
    Arcadia_Media_DSP_Gain* gain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)sawA, 2.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)gain);
    Arcadia_Media_DSP_Affine* affine = Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)sawB, 2.0f, 1.0f);
    Arcadia_Object_lock(thread, (Arcadia_Object*)affine);
    Arcadia_Media_DSP_Mix* mix = Arcadia_Media_DSP_Mix_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)mix);
    Arcadia_Media_DSP_Mix_addInput(thread, mix, (Arcadia_Media_DSP*)gain);
    Arcadia_Media_DSP_Mix_addInput(thread, mix, (Arcadia_Media_DSP*)affine);
    Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
    Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)mix, buffer);
    Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
    assertReal32Near(thread, samples[0], -1.0f);
    assertReal32Near(thread, samples[1], 0.0f);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)mix);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)affine);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)gain);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)sawB);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)sawA);
  }
}

static void
testNodePinCounts
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)saw);
  Arcadia_Media_DSP_Gain* gain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)saw, 0.5f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)gain);
  Arcadia_Media_DSP_Affine* affine = Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)saw, 1.0f, 0.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)affine);
  Arcadia_Media_DSP_Multiply* multiply = Arcadia_Media_DSP_Multiply_create(thread, (Arcadia_Media_DSP*)saw, (Arcadia_Media_DSP*)affine);
  Arcadia_Object_lock(thread, (Arcadia_Object*)multiply);
  Arcadia_Media_DSP_Mix* mix = Arcadia_Media_DSP_Mix_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)mix);
  Arcadia_Media_DSP_LowPass* lowPass = Arcadia_Media_DSP_LowPass_create(thread, (Arcadia_Media_DSP*)saw, 440.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)lowPass);

  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)saw));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)saw));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)gain));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)gain));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)affine));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)affine));
  Arcadia_Tests_assertTrue(thread, 2 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)multiply));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)multiply));
  Arcadia_Tests_assertTrue(thread, 0 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)mix));
  Arcadia_Media_DSP_Mix_addInput(thread, mix, (Arcadia_Media_DSP*)gain);
  Arcadia_Media_DSP_Mix_addInput(thread, mix, (Arcadia_Media_DSP*)affine);
  Arcadia_Tests_assertTrue(thread, 2 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)mix));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)mix));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)lowPass));
  Arcadia_Tests_assertTrue(thread, 1 == Arcadia_Media_DSP_getNumberOfOutputPins(thread, (Arcadia_Media_DSP*)lowPass));

  Arcadia_Object_unlock(thread, (Arcadia_Object*)lowPass);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)mix);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)multiply);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)affine);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)gain);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)saw);
}

static void
testAmplitudeModulationEquivalent
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_Graph* graph = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)graph);
  Arcadia_Media_DSP_SineWave* audio = Arcadia_Media_DSP_SineWave_create(thread, 440.0f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)audio);
  Arcadia_Media_DSP_SineWave* modulation = Arcadia_Media_DSP_SineWave_create(thread, 440.0f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)modulation);
  Arcadia_Media_DSP_Affine* control = Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)modulation, 0.2f, 0.8f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)control);
  Arcadia_Media_DSP_Multiply* amplitudeModulation = Arcadia_Media_DSP_Multiply_create(thread, (Arcadia_Media_DSP*)audio, (Arcadia_Media_DSP*)control);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)amplitudeModulation);
  Arcadia_Media_DSP_Graph_setOutput(thread, graph, (Arcadia_Media_DSP*)amplitudeModulation);
  Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 1760, 4);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, buffer);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
  assertReal32Near(thread, samples[0], 0.0f);
  assertReal32Near(thread, samples[1], 1.0f);
  assertReal32Near(thread, samples[2], 0.0f);
  assertReal32Near(thread, samples[3], -0.6f);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
}

static void
testGraph
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_Graph* graph = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)graph);
  Arcadia_Media_DSP_Buffer* emptyGraphBuffer = createLockedBuffer(thread, 4, 4);
  struct RenderGraphContext emptyGraphContext = { .graph = graph, .buffer = emptyGraphBuffer };
  expectArgumentValueInvalid(thread, &renderGraphContext, &emptyGraphContext);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)emptyGraphBuffer);
  Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)saw);
  Arcadia_Media_DSP_Gain* gain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)saw, 2.0f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)gain);
  Arcadia_Media_DSP_Graph_setOutput(thread, graph, (Arcadia_Media_DSP*)gain);

  Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, buffer);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
  assertReal32Near(thread, samples[0], -1.0f);
  assertReal32Near(thread, samples[1], -0.5f);
  assertReal32Near(thread, samples[2], 0.0f);
  assertReal32Near(thread, samples[3], 0.5f);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
}

static void
testGraphOwnership
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_Graph* graphA = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)graphA);
  Arcadia_Media_DSP_Graph* graphB = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)graphB);
  Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)saw);
  Arcadia_Media_DSP_Graph_addNode(thread, graphA, (Arcadia_Media_DSP*)saw);

  struct GraphNodeContext duplicateContext = { .graph = graphA, .node = (Arcadia_Media_DSP*)saw };
  expectArgumentValueInvalid(thread, &addNodeContext, &duplicateContext);

  struct GraphNodeContext otherGraphContext = { .graph = graphB, .node = (Arcadia_Media_DSP*)saw };
  expectArgumentValueInvalid(thread, &addNodeContext, &otherGraphContext);

  struct GraphNodeContext graphAsNodeContext = { .graph = graphA, .node = (Arcadia_Media_DSP*)graphB };
  expectArgumentValueInvalid(thread, &addNodeContext, &graphAsNodeContext);

  Arcadia_Media_DSP_SawtoothWave* outsideSaw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)outsideSaw);
  struct GraphNodeContext outsideOutputContext = { .graph = graphA, .node = (Arcadia_Media_DSP*)outsideSaw };
  expectArgumentValueInvalid(thread, &setOutputContext, &outsideOutputContext);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)outsideSaw);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)saw);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graphB);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graphA);
}

static void
testGraphReset
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_Graph* graph = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)graph);
  Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)saw);
  Arcadia_Media_DSP_LowPass* lowPass = Arcadia_Media_DSP_LowPass_create(thread, (Arcadia_Media_DSP*)saw, 1.0f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)lowPass);
  Arcadia_Media_DSP_Graph_setOutput(thread, graph, (Arcadia_Media_DSP*)lowPass);

  Arcadia_Media_DSP_Buffer* first = createLockedBuffer(thread, 8, 4);
  Arcadia_Media_DSP_Buffer* second = createLockedBuffer(thread, 8, 4);
  Arcadia_Media_DSP_Buffer* afterReset = createLockedBuffer(thread, 8, 4);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, first);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, second);
  Arcadia_Media_DSP_Graph_reset(thread, graph);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, afterReset);

  Arcadia_Real32Value* firstSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, first);
  Arcadia_Real32Value* secondSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, second);
  Arcadia_Real32Value* afterResetSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, afterReset);
  Arcadia_Tests_assertTrue(thread, fabsf(firstSamples[0] - secondSamples[0]) > 0.0001f);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) {
    assertReal32Near(thread, afterResetSamples[i], firstSamples[i]);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)afterReset);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)second);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)first);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
}

static void
testGraphReseed
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_Graph* graph = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_Object_lock(thread, (Arcadia_Object*)graph);
  Arcadia_Media_DSP_WhiteNoise* white = Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 0.40f);
  Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)white);
  Arcadia_Media_DSP_Graph_setOutput(thread, graph, (Arcadia_Media_DSP*)white);

  Arcadia_Media_DSP_Buffer* first = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_Buffer* reset = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_Buffer* reseeded = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_Buffer* reseededReset = createLockedBuffer(thread, 48000, 8);

  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, first);
  Arcadia_Media_DSP_Graph_reset(thread, graph);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, reset);
  Arcadia_Media_DSP_Graph_reseed(thread, graph, 200u);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, reseeded);
  Arcadia_Media_DSP_Graph_reset(thread, graph);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, reseededReset);

  Arcadia_Real32Value* firstSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, first);
  Arcadia_Real32Value* resetSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, reset);
  Arcadia_Real32Value* reseededSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, reseeded);
  Arcadia_Real32Value* reseededResetSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, reseededReset);
  Arcadia_BooleanValue foundDifference = Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 8; ++i) {
    assertReal32Near(thread, resetSamples[i], firstSamples[i]);
    assertReal32Near(thread, reseededResetSamples[i], reseededSamples[i]);
    if (fabsf(firstSamples[i] - reseededSamples[i]) > 0.0001f) {
      foundDifference = Arcadia_BooleanValue_True;
    }
  }
  Arcadia_Tests_assertTrue(thread, foundDifference);

  Arcadia_Object_unlock(thread, (Arcadia_Object*)reseededReset);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)reseeded);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)reset);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)first);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
}

static void
testWhiteNoise
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_WhiteNoise* a = Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 0.40f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)a);
  Arcadia_Media_DSP_WhiteNoise* b = Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 0.40f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)b);
  Arcadia_Media_DSP_Buffer* bufferA = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_Buffer* bufferB = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)a, bufferA);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)b, bufferB);
  Arcadia_Real32Value* samplesA = Arcadia_Media_DSP_Buffer_getSamples(thread, bufferA);
  Arcadia_Real32Value* samplesB = Arcadia_Media_DSP_Buffer_getSamples(thread, bufferB);
  for (Arcadia_SizeValue i = 0; i < 8; ++i) {
    assertReal32Near(thread, samplesA[i], samplesB[i]);
    Arcadia_Tests_assertTrue(thread, samplesA[i] >= -0.40f && samplesA[i] <= 0.40f);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)bufferB);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)bufferA);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)b);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)a);
}

static void
testBrownNoise
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_BrownNoise* a = Arcadia_Media_DSP_BrownNoise_create(thread, 2u, 0.65f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)a);
  Arcadia_Media_DSP_BrownNoise* b = Arcadia_Media_DSP_BrownNoise_create(thread, 2u, 0.65f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)b);
  Arcadia_Media_DSP_Buffer* bufferA = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_Buffer* bufferB = createLockedBuffer(thread, 48000, 8);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)a, bufferA);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)b, bufferB);
  Arcadia_Real32Value* samplesA = Arcadia_Media_DSP_Buffer_getSamples(thread, bufferA);
  Arcadia_Real32Value* samplesB = Arcadia_Media_DSP_Buffer_getSamples(thread, bufferB);
  for (Arcadia_SizeValue i = 0; i < 8; ++i) {
    assertReal32Near(thread, samplesA[i], samplesB[i]);
    Arcadia_Tests_assertTrue(thread, samplesA[i] >= -0.65f && samplesA[i] <= 0.65f);
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)bufferB);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)bufferA);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)b);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)a);
}

static void
testLowPass
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Media_DSP_SawtoothWave* saw = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)saw);
  expectArgumentValueInvalid(thread, &createInvalidLowPassContext, saw);
  Arcadia_Media_DSP_LowPass* lowPass = Arcadia_Media_DSP_LowPass_create(thread, (Arcadia_Media_DSP*)saw, 1.0f);
  Arcadia_Object_lock(thread, (Arcadia_Object*)lowPass);
  Arcadia_Media_DSP_Buffer* buffer = createLockedBuffer(thread, 4, 4);
  Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)lowPass, buffer);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
  Arcadia_Real32Value alpha = 1.0f - expf(-2.0f * 3.14159265358979323846f * 1.0f / 4.0f);
  assertReal32Near(thread, samples[0], -0.5f * alpha);
  Arcadia_Tests_assertTrue(thread, samples[1] > samples[0]);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)buffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)lowPass);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)saw);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  testBuffer(thread);
  testSawtoothWave(thread);
  testSineWaveValues(thread);
  testSineWaveContinuity(thread);
  testSawtoothWaveContinuity(thread);
  testGainAffineMultiplyAndMix(thread);
  testNodePinCounts(thread);
  testAmplitudeModulationEquivalent(thread);
  testGraph(thread);
  testGraphOwnership(thread);
  testGraphReset(thread);
  testGraphReseed(thread);
  testWhiteNoise(thread);
  testBrownNoise(thread);
  testLowPass(thread);
}

static int
runTests
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(&test)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static void
testWaveFrequencyDomain
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  const Arcadia_Real32Value frequencies[] = { 0.f, -1.f };
  for (unsigned sawtooth = 0; sawtooth < 2; ++sawtooth) {
    for (unsigned i = 0; i < 2; ++i) {
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        if (sawtooth) Arcadia_Media_DSP_SawtoothWave_create(thread, frequencies[i]);
        else Arcadia_Media_DSP_SineWave_create(thread, frequencies[i]);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Status status = Arcadia_Thread_getStatus(thread);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
        Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_ArgumentValueInvalid, status);
      }
      Arcadia_Tests_assertEqualSize(thread, base + 300, Arcadia_ValueStack_getSize(thread));
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    }
  }
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static int
runWaveFrequencyDomain
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testWaveFrequencyDomain) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
checkMixSum
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value level,
    Arcadia_SizeValue numberOfSources,
    Arcadia_SizeValue numberOfInputs,
    Arcadia_BooleanValue duplicate,
    Arcadia_Real32Value expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Media_DSP_Mix* mix = Arcadia_Media_DSP_Mix_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)mix);
  Arcadia_Media_DSP* sources[2] = { NULL, NULL };
  for (Arcadia_SizeValue i = 0; i < numberOfSources; ++i) {
    Arcadia_Media_DSP_SawtoothWave* wave = Arcadia_Media_DSP_SawtoothWave_create(thread, 1.f);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)wave);
    // A zero-scale affine signal is constant even when a duplicate input is
    // rendered again. Do not assume Mix caches a stateful node's first render.
    sources[i] = (Arcadia_Media_DSP*)Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)wave, 0.f, level);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sources[i]);
  }
  for (Arcadia_SizeValue i = 0; i < numberOfInputs; ++i) {
    Arcadia_Media_DSP_Mix_addInput(thread, mix, sources[duplicate ? 0 : i]);
  }
  Arcadia_Tests_assertEqualSize(thread, numberOfInputs, Arcadia_Media_DSP_getNumberOfInputPins(thread, (Arcadia_Media_DSP*)mix));
  for (Arcadia_SizeValue i = 0; i < numberOfInputs; ++i) {
    Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, mix->inputs, i) == (Arcadia_Object*)sources[duplicate ? 0 : i]);
  }
  Arcadia_Media_DSP_Buffer* buffer = Arcadia_Media_DSP_Buffer_create(thread, 8, 17);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)buffer);
  for (unsigned repeat = 0; repeat < 2; ++repeat) {
    Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, buffer);
    for (Arcadia_SizeValue i = 0; i < 17; ++i) samples[i] = 123.f;
    Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)mix, buffer);
    for (Arcadia_SizeValue i = 0; i < 17; ++i) {
      // Dyadic levels and at most two unity additions are exactly representable
      // in Real32. 1e-6 allows a few low-order ulps, far below clipping/gain errors.
      Arcadia_Tests_assertTrue(thread, fabsf(samples[i] - expected) <= 0.000001f);
      if (expected > 1.f) Arcadia_Tests_assertTrue(thread, samples[i] > 1.f);
      if (expected < -1.f) Arcadia_Tests_assertTrue(thread, samples[i] < -1.f);
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static void
testMixSummation
  (
    Arcadia_Thread* thread
  )
{
  checkMixSum(thread, 0.f, 0, 0, false, 0.f);       // Empty native mixer: silence.
  checkMixSum(thread, 0.75f, 1, 1, false, 0.75f);   // Unity-gain pass-through.
  checkMixSum(thread, 0.75f, 2, 2, false, 1.5f);    // Distinct identical inputs.
  checkMixSum(thread, 0.75f, 1, 2, true, 1.5f);     // Same input counted twice.
  checkMixSum(thread, -0.75f, 1, 2, true, -1.5f);   // No lower-bound clamping either.
}

static int
runMixSummation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testMixSummation) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// Returns one rooted graph. Kinds 0..3 select individual streaming sources;
// kind 4 is a tree of distinct sources, each evaluated once per output block.
static Arcadia_Media_DSP_Graph*
createRepeatableGraph
  (
    Arcadia_Thread* thread,
    unsigned kind,
    Arcadia_Natural32Value seed
  )
{
  Arcadia_Media_DSP_Graph* graph = Arcadia_Media_DSP_Graph_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)graph);
  Arcadia_Media_DSP* output;
  if (kind < 4) {
    switch (kind) {
      case 0: output = (Arcadia_Media_DSP*)Arcadia_Media_DSP_SineWave_create(thread, 440.f); break;
      case 1: output = (Arcadia_Media_DSP*)Arcadia_Media_DSP_SawtoothWave_create(thread, 440.f); break;
      case 2: output = (Arcadia_Media_DSP*)Arcadia_Media_DSP_WhiteNoise_create(thread, 11u, 0.4f); break;
      default: output = (Arcadia_Media_DSP*)Arcadia_Media_DSP_BrownNoise_create(thread, 22u, 0.65f); break;
    }
    Arcadia_Media_DSP_Graph_addNode(thread, graph, output);
  } else {
    Arcadia_Media_DSP* white = (Arcadia_Media_DSP*)Arcadia_Media_DSP_WhiteNoise_create(thread, 11u, 0.4f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, white);
    Arcadia_Media_DSP* brown = (Arcadia_Media_DSP*)Arcadia_Media_DSP_BrownNoise_create(thread, 22u, 0.65f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, brown);
    Arcadia_Media_DSP* sine = (Arcadia_Media_DSP*)Arcadia_Media_DSP_SineWave_create(thread, 440.f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, sine);
    Arcadia_Media_DSP* saw = (Arcadia_Media_DSP*)Arcadia_Media_DSP_SawtoothWave_create(thread, 220.f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, saw);
    Arcadia_Media_DSP* gain = (Arcadia_Media_DSP*)Arcadia_Media_DSP_Gain_create(thread, white, 0.5f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, gain);
    Arcadia_Media_DSP* affine = (Arcadia_Media_DSP*)Arcadia_Media_DSP_Affine_create(thread, sine, 0.25f, 0.5f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, affine);
    Arcadia_Media_DSP* lowPass = (Arcadia_Media_DSP*)Arcadia_Media_DSP_LowPass_create(thread, brown, 850.f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, lowPass);
    Arcadia_Media_DSP* multiply = (Arcadia_Media_DSP*)Arcadia_Media_DSP_Multiply_create(thread, affine, saw);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, multiply);
    Arcadia_Media_DSP_Mix* mix = Arcadia_Media_DSP_Mix_create(thread);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)mix);
    Arcadia_Media_DSP_Mix_addInput(thread, mix, gain);
    Arcadia_Media_DSP_Mix_addInput(thread, mix, lowPass);
    Arcadia_Media_DSP_Mix_addInput(thread, mix, multiply);
    Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, mix->inputs, 0) == (Arcadia_Object*)gain);
    Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, mix->inputs, 1) == (Arcadia_Object*)lowPass);
    Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, mix->inputs, 2) == (Arcadia_Object*)multiply);
    output = (Arcadia_Media_DSP*)mix;
  }
  Arcadia_Media_DSP_Graph_setOutput(thread, graph, output);
  Arcadia_Media_DSP_Graph_reseed(thread, graph, seed);
  return graph;
}

static void
renderPartition
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* graph,
    Arcadia_Real32Value* samples,
    Arcadia_SizeValue const* lengths,
    Arcadia_SizeValue count
  )
{
  Arcadia_SizeValue offset = 0;
  for (Arcadia_SizeValue i = 0; i < count; ++i) {
    Arcadia_Media_DSP_Buffer* buffer = Arcadia_Media_DSP_Buffer_create(thread, 48000, lengths[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)buffer);
    Arcadia_Media_DSP_render(thread, (Arcadia_Media_DSP*)graph, buffer);
    if (lengths[i]) memcpy(samples + offset, Arcadia_Media_DSP_Buffer_getSamples(thread, buffer), lengths[i] * sizeof(*samples));
    offset += lengths[i];
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Tests_assertEqualSize(thread, 257, offset);
}

static void
testReproducibility
  (
    Arcadia_Thread* thread
  )
{
  const Arcadia_SizeValue one[] = { 257 }, chunks[] = { 0, 1, 7, 0, 31, 64, 3, 151 };
  for (unsigned kind = 0; kind < 5; ++kind) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    Arcadia_Media_DSP_Graph* a = createRepeatableGraph(thread, kind, 200u);
    Arcadia_Media_DSP_Graph* b = createRepeatableGraph(thread, kind, 200u);
    Arcadia_Media_DSP_Graph* c = createRepeatableGraph(thread, kind, 200u);
    Arcadia_Real32Value whole[257], fresh[257], partitioned[257], reset[257], seeded[257];
    renderPartition(thread, a, whole, one, 1);
    renderPartition(thread, b, fresh, one, 1);
    renderPartition(thread, c, partitioned, chunks, sizeof(chunks) / sizeof(chunks[0]));
    Arcadia_Tests_assertEqualBytes(thread, whole, sizeof(whole), fresh, sizeof(fresh));
    Arcadia_Tests_assertEqualBytes(thread, whole, sizeof(whole), partitioned, sizeof(partitioned));
    Arcadia_Media_DSP_Graph_reset(thread, a);
    renderPartition(thread, a, reset, one, 1);
    Arcadia_Tests_assertEqualBytes(thread, whole, sizeof(whole), reset, sizeof(reset));
    Arcadia_Media_DSP_Graph_reseed(thread, a, 201u);
    renderPartition(thread, a, seeded, one, 1);
    Arcadia_Media_DSP_Graph* d = createRepeatableGraph(thread, kind, 201u);
    renderPartition(thread, d, fresh, one, 1);
    Arcadia_Tests_assertEqualBytes(thread, seeded, sizeof(seeded), fresh, sizeof(fresh));
    Arcadia_Media_DSP_Graph_reset(thread, a);
    renderPartition(thread, a, reset, chunks, sizeof(chunks) / sizeof(chunks[0]));
    Arcadia_Tests_assertEqualBytes(thread, seeded, sizeof(seeded), reset, sizeof(reset));
    if (kind >= 2) {
      Arcadia_Tests_assertTrue(thread, memcmp(whole, seeded, sizeof(whole)) != 0);
    } else {
      Arcadia_Tests_assertEqualBytes(thread, whole, sizeof(whole), seeded, sizeof(seeded));
    }
    // Separate finite-source numerical references. Short 257-frame horizons
    // bound phase/recurrence drift; these are not universal DSP epsilons.
    Arcadia_Natural32Value randomState = 0;
    if (kind == 2) randomState = ((Arcadia_Media_DSP_WhiteNoise*)b->output)->initialSeed;
    if (kind == 3) randomState = ((Arcadia_Media_DSP_BrownNoise*)b->output)->initialSeed;
    double brownState = 0.0;
    for (Arcadia_SizeValue i = 0; i < 257; ++i) {
      Arcadia_Tests_assertTrue(thread, isfinite(whole[i]));
      if (kind < 2) {
        double phase = (double)i * 440.0 / 48000.0;
        double expected = kind == 0 ? sin(2.0 * 3.14159265358979323846 * phase) : phase - floor(phase) - 0.5;
        Arcadia_Tests_assertTrue(thread, fabs((double)whole[i] - expected) <= (kind == 0 ? 0.0001 : 0.00001));
      } else if (kind < 4) {
        randomState ^= randomState << 13;
        randomState ^= randomState >> 17;
        randomState ^= randomState << 5;
        double normalized = (double)(randomState & 0x00ffffffu) / 8388607.5 - 1.0;
        if (kind == 2) {
          double expected = normalized * (double)((Arcadia_Media_DSP_WhiteNoise*)b->output)->amplitude;
          // No recursive state: bound only the Real32 normalize/scale rounding.
          Arcadia_Tests_assertTrue(thread, fabs((double)whole[i] - expected) <= 0.0000001);
        } else {
          brownState = brownState * 0.995 + normalized * 0.02;
          if (brownState < -1.0) brownState = -1.0;
          if (brownState > 1.0) brownState = 1.0;
          double expected = brownState * (double)((Arcadia_Media_DSP_BrownNoise*)b->output)->amplitude;
          // 257 steps of a contractive .995 recurrence accumulate more rounding.
          Arcadia_Tests_assertTrue(thread, fabs((double)whole[i] - expected) <= 0.000002);
        }
      }
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  }
}

static int
runReproducibility
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testReproducibility) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "DSP", &runTests },
    { "WaveFrequencyDomain", &runWaveFrequencyDomain },
    { "MixSummation", &runMixSummation },
    { "Reproducibility", &runReproducibility },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
