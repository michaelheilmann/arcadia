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

#include <math.h>
#include <stdlib.h>

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

int
main
  (
    int argc,
    char **argv
  )
{
  (void)argc;
  (void)argv;
  if (!Arcadia_Tests_safeExecute(&test)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
