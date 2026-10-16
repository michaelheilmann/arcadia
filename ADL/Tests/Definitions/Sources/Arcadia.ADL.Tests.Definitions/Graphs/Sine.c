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

#include "Arcadia.ADL.Tests.Definitions/Graphs/Sine.h"

#include "Arcadia/ADL/Include.h"

// Parse a top-level DDL list document and return the list of its elements.
static Arcadia_List*
parseNodeList
  (
    Arcadia_Thread* thread,
    const char* ddl
  )
{
  Arcadia_DDL_DefaultReader* reader = (Arcadia_DDL_DefaultReader*)Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_UnicodeCodePointReader* input =
    (Arcadia_UnicodeCodePointReader*)
    Arcadia_ByteReader_UnicodeCodePointReader_create
      (
        thread,
        (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, Arcadia_String_toByteArray(thread, Arcadia_String_createFromCxxString(thread, ddl)))
      );
  Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_ListNode_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  return ((Arcadia_DDL_ListNode*)node)->elements;
}

// Expand the specified DDL batch into a fresh definitions container.
static Arcadia_ADL_Definitions*
expandBatch
  (
    Arcadia_Thread* thread,
    const char* ddl
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_List* nodes = parseNodeList(thread, ddl);
  Arcadia_ADL_Graphs_readFromNodes(thread, context, definitions, nodes);
  Arcadia_ADL_Definitions_link(thread, definitions);
  return definitions;
}

// Assert that a definition of the specified name exists and return it.
static Arcadia_ADL_Definition*
requireDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    const char* name
  )
{
  Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, name));
  if (NULL == definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  return definition;
}

static void
requireTrue
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue condition
  )
{
  if (Arcadia_BooleanValue_True != condition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

// A single-sine graph with a typed Integer32 frequency parameter.
static const char* SINE_BATCH =
  "[\n"
  "  {\n"
  "    type : \"GraphDefinition\",\n"
  "    name : \"Audials.Tone\",\n"
  "    nodes : [\n"
  "      {\n"
  "        type : \"Graph.Parameter\",\n"
  "        name : \"Frequency\",\n"
  "        valueType : \"Integer32\",\n"
  "      },\n"
  "      {\n"
  "        type : \"DSP.SineWave\",\n"
  "        name : \"tone\",\n"
  "        frequency : \"Frequency\",\n"
  "      },\n"
  "      {\n"
  "        type : \"Graph.Output\",\n"
  "        name : \"output\",\n"
  "        referenceType : \"DSP.SineWave\",\n"
  "        source : \"tone\",\n"
  "      },\n"
  "    ],\n"
  "  },\n"
  "  {\n"
  "    type : \"GraphInstance\",\n"
  "    name : \"MyGame.Tone\",\n"
  "    graph : \"Audials.Tone\",\n"
  "    bindings : [\n"
  "      { parameter : \"Frequency\", value : 440 },\n"
  "    ],\n"
  "  },\n"
  "]\n"
  ;

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Definitions* definitions = expandBatch(thread, SINE_BATCH);

  // The sine node is the output source and is published directly under the instance name.
  Arcadia_ADL_SineWaveDefinition* tone =
    (Arcadia_ADL_SineWaveDefinition*)
    requireDefinition(thread, definitions, "MyGame.Tone.output");

  requireTrue(thread, (440 == tone->frequency) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);

  // The internal node name and the internal output name must not be visible.
  requireTrue(thread, NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "MyGame.Tone.__graph.tone")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "MyGame.Tone.__graph.output")) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
}

// A batch with one graph template and two instances bound to different frequencies,
// plus a `SampleBuffer` that consumes the published output of the first instance.
static const char* SINE_TWO_INSTANCES_BATCH =
  "[\n"
  "  {\n"
  "    type : \"GraphDefinition\",\n"
  "    name : \"Audials.Tone\",\n"
  "    nodes : [\n"
  "      { type : \"Graph.Parameter\", name : \"Frequency\", valueType : \"Integer32\" },\n"
  "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
  "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
  "    ],\n"
  "  },\n"
  "  {\n"
  "    type : \"GraphInstance\",\n"
  "    name : \"MyGame.Low\",\n"
  "    graph : \"Audials.Tone\",\n"
  "    bindings : [ { parameter : \"Frequency\", value : 220 } ],\n"
  "  },\n"
  "  {\n"
  "    type : \"GraphInstance\",\n"
  "    name : \"MyGame.High\",\n"
  "    graph : \"Audials.Tone\",\n"
  "    bindings : [ { parameter : \"Frequency\", value : 880 } ],\n"
  "  },\n"
  "  {\n"
  "    type : \"SampleBuffer\",\n"
  "    name : \"MyGame.LowBuffer\",\n"
  "    duration : 1,\n"
  "    dsp : \"MyGame.Low.output\",\n"
  "  },\n"
  "]\n"
  ;

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Definitions* definitions = expandBatch(thread, SINE_TWO_INSTANCES_BATCH);

  Arcadia_ADL_SineWaveDefinition* low =
    (Arcadia_ADL_SineWaveDefinition*)
    requireDefinition(thread, definitions, "MyGame.Low.output");
  Arcadia_ADL_SineWaveDefinition* high =
    (Arcadia_ADL_SineWaveDefinition*)
    requireDefinition(thread, definitions, "MyGame.High.output");
  Arcadia_ADL_SampleBufferDefinition* buffer =
    (Arcadia_ADL_SampleBufferDefinition*)
    requireDefinition(thread, definitions, "MyGame.LowBuffer");

  requireTrue(thread, (220 == low->frequency) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
  requireTrue(thread, (880 == high->frequency) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);

  // The ordinary `SampleBuffer.dsp` reference is linked to the published output of the first instance.
  requireTrue(thread, (buffer->dsp->definition == (Arcadia_ADL_Definition*)low) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
}

// Run the specified batch expansion expecting a failure with the specified status.
static void
expectFailure
  (
    Arcadia_Thread* thread,
    const char* ddl,
    Arcadia_Status expected
  )
{
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_ADL_Graphs_readFromNodes(thread, context, definitions, parseNodeList(thread, ddl));
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (expected != Arcadia_Thread_getStatus(thread)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    }
  }
}

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest3
  (
    Arcadia_Thread* thread
  )
{
  // A value parameter bound with `source` (a reference) instead of `value` is rejected.
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Audials.Tone\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"Frequency\", valueType : \"Integer32\" },\n"
    "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Tone\",\n"
    "    graph : \"Audials.Tone\",\n"
    "    bindings : [ { parameter : \"Frequency\", source : \"Audials.Tone\" } ],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest4
  (
    Arcadia_Thread* thread
  )
{
  // A parameter declaring both `referenceType` and `valueType` is rejected.
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Audials.Tone\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"Frequency\", referenceType : \"Color\", valueType : \"Integer32\" },\n"
    "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Tone\",\n"
    "    graph : \"Audials.Tone\",\n"
    "    bindings : [ { parameter : \"Frequency\", value : 440 } ],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest5
  (
    Arcadia_Thread* thread
  )
{
  // A fractional value binding is rejected: `Integer32` cannot represent it.
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Audials.Tone\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"Frequency\", valueType : \"Integer32\" },\n"
    "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Tone\",\n"
    "    graph : \"Audials.Tone\",\n"
    "    bindings : [ { parameter : \"Frequency\", value : 440.5 } ],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest6
  (
    Arcadia_Thread* thread
  )
{
  // A negative frequency is rejected by the front end.
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Audials.Tone\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"Frequency\", valueType : \"Integer32\" },\n"
    "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Tone\",\n"
    "    graph : \"Audials.Tone\",\n"
    "    bindings : [ { parameter : \"Frequency\", value : -440 } ],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest7
  (
    Arcadia_Thread* thread
  )
{
  // A missing binding for the value parameter is rejected.
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Audials.Tone\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"Frequency\", valueType : \"Integer32\" },\n"
    "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
    "    ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Tone\",\n"
    "    graph : \"Audials.Tone\",\n"
    "    bindings : [],\n"
    "  },\n"
    "]\n"
    ;
  expectFailure(thread, DDL, Arcadia_Status_SemanticalError);
}

void
Arcadia_ADL_Tests_Definitions_Graphs_SineTest8
  (
    Arcadia_Thread* thread
  )
{
  // The same graph as SineTest1 but with the graph definition declared after the instance;
  // the expansion must be independent of declaration/load order.
  static const char* DDL =
    "[\n"
    "  {\n"
    "    type : \"GraphInstance\",\n"
    "    name : \"MyGame.Tone\",\n"
    "    graph : \"Audials.Tone\",\n"
    "    bindings : [ { parameter : \"Frequency\", value : 440 } ],\n"
    "  },\n"
    "  {\n"
    "    type : \"GraphDefinition\",\n"
    "    name : \"Audials.Tone\",\n"
    "    nodes : [\n"
    "      { type : \"Graph.Parameter\", name : \"Frequency\", valueType : \"Integer32\" },\n"
    "      { type : \"DSP.SineWave\", name : \"tone\", frequency : \"Frequency\" },\n"
    "      { type : \"Graph.Output\", name : \"output\", referenceType : \"DSP.SineWave\", source : \"tone\" },\n"
    "    ],\n"
    "  },\n"
    "]\n"
    ;
  Arcadia_ADL_Definitions* definitions = expandBatch(thread, DDL);
  Arcadia_ADL_SineWaveDefinition* tone =
    (Arcadia_ADL_SineWaveDefinition*)
    requireDefinition(thread, definitions, "MyGame.Tone.output");
  requireTrue(thread, (440 == tone->frequency) ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False);
}
