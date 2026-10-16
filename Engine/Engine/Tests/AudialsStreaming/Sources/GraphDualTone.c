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

#include "GraphDualTone.h"

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/Include.h"

#include <math.h>
#include <stdlib.h>

static const char* const batch =
  "["
  "{ type: \"GraphDefinition\", name: \"Dual\", nodes: ["
  "{ type: \"Graph.Parameter\", name: \"Low\", valueType: \"Integer32\" },"
  "{ type: \"Graph.Parameter\", name: \"High\", valueType: \"Integer32\" },"
  "{ type: \"DSP.SineWave\", name: \"low\", frequency: \"Low\" },"
  "{ type: \"DSP.SineWave\", name: \"high\", frequency: \"High\" },"
  "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"high\", \"low\"] },"
  "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }] },"
  "{ type: \"GraphInstance\", name: \"First\", graph: \"Dual\", bindings: ["
  "{ parameter: \"Low\", value: 1 }, { parameter: \"High\", value: 2 }] },"
  "{ type: \"GraphInstance\", name: \"Second\", graph: \"Dual\", bindings: ["
  "{ parameter: \"Low\", value: 1 }, { parameter: \"High\", value: 2 }] },"
  "{ type: \"DSPStreamProvider\", name: \"First.Provider\", dsp: \"First.output\", sampleRate: 8, duration: 2 },"
  "{ type: \"DSPStreamProvider\", name: \"Second.Provider\", dsp: \"Second.output\", sampleRate: 8, duration: 2 },"
  "{ type: \"StreamingSoundSourceNode\", name: \"First.Source\", provider: \"First.Provider\" },"
  "{ type: \"StreamingSoundSourceNode\", name: \"Second.Source\", provider: \"Second.Provider\" }"
  "]";

static Arcadia_List*
parseBatch
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_DDL_DefaultReader* reader = Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_ByteArray* bytes = Arcadia_String_toByteArray(thread, Arcadia_String_createFromCxxString(thread, batch));
  Arcadia_ByteReader* byteReader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, bytes);
  Arcadia_UnicodeCodePointReader* input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, byteReader);
  Arcadia_DDL_Node* root = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)root, _Arcadia_DDL_ListNode_getType(thread)));
  return ((Arcadia_DDL_ListNode*)root)->elements;
}

static Arcadia_ADL_Definition*
requireDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    const char* name,
    Arcadia_TypeValue type
  )
{
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, name));
  Arcadia_Tests_assertTrue(thread, definition != NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, type));
  return definition;
}

static Arcadia_Engine_Audials_StreamingSoundSourceNode*
createSource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* factory,
    Arcadia_ADL_Definitions* definitions,
    const char* name
  )
{
  Arcadia_ADL_StreamingSoundSourceNodeDefinition* definition = (Arcadia_ADL_StreamingSoundSourceNodeDefinition*)
    requireDefinition(thread, definitions, name, _Arcadia_ADL_StreamingSoundSourceNodeDefinition_getType(thread));
  // This public factory calls the real DSPFactory_createFromReference and
  // DSPStreamProvider constructors. A null backend avoids activating any device.
  Arcadia_Engine_Audials_StreamingSoundSourceNode* source =
    Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition(thread, factory, NULL, definition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  return source;
}

static Arcadia_Media_DSP_SineWave*
requireInput
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Mix* mix,
    Arcadia_SizeValue index,
    Arcadia_Real32Value frequency
  )
{
  Arcadia_Media_DSP_SineWave* sine = (Arcadia_Media_DSP_SineWave*)Arcadia_List_getObjectReferenceValueCheckedAt(
    thread, mix->inputs, index, _Arcadia_Media_DSP_SineWave_getType(thread));
  Arcadia_Tests_assertEqualReal64(thread, frequency, sine->frequency);
  return sine;
}

static Arcadia_Engine_Audials_DSPStreamProvider*
requireProvider
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* source
  )
{
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)source->provider, _Arcadia_Engine_Audials_DSPStreamProvider_getType(thread)));
  Arcadia_Engine_Audials_DSPStreamProvider* provider = (Arcadia_Engine_Audials_DSPStreamProvider*)source->provider;
  Arcadia_Tests_assertEqualSize(thread, 16, provider->framesRemaining);
  Arcadia_Tests_assertEqualNatural64(thread, 8, provider->sampleRate);
  Arcadia_Tests_assertFalse(thread, provider->isInfinite);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)provider->dsp, _Arcadia_Media_DSP_Mix_getType(thread)));
  Arcadia_Media_DSP_Mix* mix = (Arcadia_Media_DSP_Mix*)provider->dsp;
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mix->inputs));
  Arcadia_Tests_assertTrue(thread, requireInput(thread, mix, 0, 2.0f) != requireInput(thread, mix, 1, 1.0f));
  return provider;
}

static void
assertNear
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value actual,
    Arcadia_Real64Value expected
  )
{
  // Absolute 1e-5 allows float pi/sinf rounding in two unit-amplitude sources
  // over 16 samples. At 8 Hz, 1/8 and 2/8 phase increments are exactly binary;
  // the reference instead uses double sin at each absolute sample index.
  // This is not a cross-platform bitwise reproducibility contract.
  Arcadia_Tests_assertTrue(thread, fabs((Arcadia_Real64Value)actual - expected) <= 0.00001);
}

static void
testGraphDualTone
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Graphs_readFromNodes(thread, Arcadia_ADL_Context_getOrCreate(thread), definitions, parseBatch(thread));
  Arcadia_ADL_Definitions_link(thread, definitions);

  Arcadia_ADL_DSPOperatorDefinition* definition = (Arcadia_ADL_DSPOperatorDefinition*)requireDefinition(thread, definitions, "First.output", _Arcadia_ADL_DSPOperatorDefinition_getType(thread));
  Arcadia_ADL_Definition* high = requireDefinition(thread, definitions, "First.__graph.high", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_ADL_Definition* low = requireDefinition(thread, definitions, "First.__graph.low", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_Tests_assertTrue(thread, definition->kind == Arcadia_ADL_DSPOperatorKind_Mix);
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)definition->inputs));
  Arcadia_ADL_Reference* first = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, definition->inputs, 0, _Arcadia_ADL_Reference_getType(thread));
  Arcadia_ADL_Reference* second = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, definition->inputs, 1, _Arcadia_ADL_Reference_getType(thread));
  Arcadia_Tests_assertTrue(thread, first->definition == high && second->definition == low && first != second);

  Arcadia_Engine_Audials_NodeFactory* factory = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)factory);
  Arcadia_Engine_Audials_DSPStreamProvider* whole = requireProvider(thread, createSource(thread, factory, definitions, "First.Source"));
  Arcadia_Engine_Audials_DSPStreamProvider* repeat = requireProvider(thread, createSource(thread, factory, definitions, "First.Source"));
  Arcadia_Engine_Audials_DSPStreamProvider* streamed = requireProvider(thread, createSource(thread, factory, definitions, "Second.Source"));
  Arcadia_Media_DSP_Mix* wholeMix = (Arcadia_Media_DSP_Mix*)whole->dsp;
  Arcadia_Media_DSP_Mix* repeatMix = (Arcadia_Media_DSP_Mix*)repeat->dsp;
  Arcadia_Media_DSP_Mix* streamedMix = (Arcadia_Media_DSP_Mix*)streamed->dsp;
  Arcadia_Tests_assertTrue(thread, wholeMix != repeatMix && wholeMix != streamedMix && repeatMix != streamedMix);
  Arcadia_Media_DSP_SineWave* wholeHigh = requireInput(thread, wholeMix, 0, 2.0f);
  Arcadia_Media_DSP_SineWave* wholeLow = requireInput(thread, wholeMix, 1, 1.0f);
  Arcadia_Tests_assertTrue(thread, wholeHigh != requireInput(thread, repeatMix, 0, 2.0f) && wholeHigh != requireInput(thread, streamedMix, 0, 2.0f));
  Arcadia_Tests_assertTrue(thread, wholeLow != requireInput(thread, repeatMix, 1, 1.0f) && wholeLow != requireInput(thread, streamedMix, 1, 1.0f));

  Arcadia_Media_DSP_Buffer* full = Arcadia_Media_DSP_Buffer_create(thread, 8, 16);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)full);
  Arcadia_Media_DSP_Buffer* repeated = Arcadia_Media_DSP_Buffer_create(thread, 8, 16);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)repeated);
  Arcadia_SizeValue written = 0;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)whole, full, &written));
  Arcadia_Tests_assertEqualSize(thread, 16, written);
  Arcadia_Tests_assertEqualSize(thread, 0, whole->framesRemaining);
  // Rendering the first graph must not advance the independent second instance.
  Arcadia_Tests_assertEqualReal64(thread, 0.0, requireInput(thread, streamedMix, 0, 2.0f)->phase);
  Arcadia_Tests_assertEqualReal64(thread, 0.0, requireInput(thread, streamedMix, 1, 1.0f)->phase);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)repeat, repeated, &written));
  Arcadia_Tests_assertEqualSize(thread, 16, written);
  Arcadia_Real32Value* samples = Arcadia_Media_DSP_Buffer_getSamples(thread, full);
  Arcadia_Real32Value* repeatedSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, repeated);
  Arcadia_Real64Value reference[16];
  Arcadia_BooleanValue exceedsOne = false;
  const Arcadia_Real64Value pi = 3.14159265358979323846;
  for (Arcadia_SizeValue i = 0; i < 16; ++i) {
    reference[i] = sin(2.0 * pi * 2.0 * (Arcadia_Real64Value)i / 8.0) + sin(2.0 * pi * (Arcadia_Real64Value)i / 8.0);
    assertNear(thread, samples[i], reference[i]);
    // Fresh construction from the same definition repeats within this binary/runtime.
    Arcadia_Tests_assertEqualReal64(thread, samples[i], repeatedSamples[i]);
    if (samples[i] > 1.0f) exceedsOne = true;
  }
  Arcadia_Tests_assertTrue(thread, exceedsOne);
  assertNear(thread, samples[1], 1.0 + sqrt(0.5));

  Arcadia_SizeValue requested[] = { 3, 5, 6, 6 };
  Arcadia_SizeValue expectedWritten[] = { 3, 5, 6, 2 };
  Arcadia_SizeValue offset = 0;
  for (Arcadia_SizeValue c = 0; c < 4; ++c) {
    Arcadia_Media_DSP_Buffer* chunk = Arcadia_Media_DSP_Buffer_create(thread, 8, requested[c]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)chunk);
    Arcadia_Real32Value* chunkSamples = Arcadia_Media_DSP_Buffer_getSamples(thread, chunk);
    for (Arcadia_SizeValue i = 0; i < requested[c]; ++i) chunkSamples[i] = 123.0f;
    written = 99;
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)streamed, chunk, &written));
    Arcadia_Tests_assertEqualSize(thread, expectedWritten[c], written);
    for (Arcadia_SizeValue i = 0; i < written; ++i) {
      assertNear(thread, chunkSamples[i], reference[offset + i]);
      assertNear(thread, chunkSamples[i], samples[offset + i]);
    }
    for (Arcadia_SizeValue i = written; i < requested[c]; ++i) {
      Arcadia_Tests_assertEqualReal64(thread, 123.0, chunkSamples[i]);
    }
    offset += written;
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_Tests_assertEqualSize(thread, 16, offset);
  Arcadia_Tests_assertEqualSize(thread, 0, streamed->framesRemaining);
  // The stored runtime children are still the original ordered pair after rendering.
  Arcadia_Tests_assertTrue(thread, requireInput(thread, wholeMix, 0, 2.0f) == wholeHigh);
  Arcadia_Tests_assertTrue(thread, requireInput(thread, wholeMix, 1, 1.0f) == wholeLow);
  for (unsigned i = 0; i < 2; ++i) {
    written = 99;
    Arcadia_Tests_assertFalse(thread, Arcadia_Engine_Audials_AudioStreamProvider_read(thread, (Arcadia_Engine_Audials_AudioStreamProvider*)streamed, repeated, &written));
    Arcadia_Tests_assertEqualSize(thread, 0, written);
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

int
Arcadia_Engine_Tests_AudialsStreaming_graphDualTone
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testGraphDualTone) ? EXIT_SUCCESS : EXIT_FAILURE;
}
