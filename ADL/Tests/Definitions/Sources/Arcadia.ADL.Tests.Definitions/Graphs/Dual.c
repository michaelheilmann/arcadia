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

#include "Arcadia.ADL.Tests.Definitions/Graphs/Dual.h"

#include "Arcadia/ADL/Include.h"

static const char* const dualNodes[] = {
  "{ type: \"Graph.Parameter\", name: \"LowFrequency\", valueType: \"Integer32\" },",
  "{ type: \"Graph.Parameter\", name: \"HighFrequency\", valueType: \"Integer32\" },",
  "{ type: \"DSP.SineWave\", name: \"low\", frequency: \"LowFrequency\" },",
  "{ type: \"DSP.SineWave\", name: \"high\", frequency: \"HighFrequency\" },",
  "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"high\", \"low\"] },",
  "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" },",
};

static const char* const instances =
  "{ type: \"GraphInstance\", name: \"First\", graph: \"Dual\", bindings: ["
  "{ parameter: \"LowFrequency\", value: 220 }, { parameter: \"HighFrequency\", value: 880 }] },"
  "{ type: \"GraphInstance\", name: \"Second\", graph: \"Dual\", bindings: ["
  "{ parameter: \"HighFrequency\", value: 660 }, { parameter: \"LowFrequency\", value: 110 }] },"
  "{ type: \"SampleBuffer\", name: \"Consumer\", duration: 1, dsp: \"First.output\" },";

static Arcadia_List*
parseBatch
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* text
  )
{
  Arcadia_DDL_DefaultReader* reader = Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_ByteArray* bytes = Arcadia_ByteArray_createByteArray(thread,
    Arcadia_RuntimeByteArray_create(thread, Arcadia_StringBuilder_getBytes(thread, text), Arcadia_StringBuilder_getNumberOfBytes(thread, text)));
  Arcadia_ByteReader* byteReader = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, bytes);
  Arcadia_UnicodeCodePointReader* input = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, byteReader);
  Arcadia_DDL_Node* root = Arcadia_DDL_DefaultReader_run(thread, reader, input);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)root, _Arcadia_DDL_ListNode_getType(thread)));
  return ((Arcadia_DDL_ListNode*)root)->elements;
}

static Arcadia_ADL_Definitions*
expandBatch
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* text
  )
{
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ADL_Graphs_readFromNodes(thread, Arcadia_ADL_Context_getOrCreate(thread), definitions, parseBatch(thread, text));
  Arcadia_ADL_Definitions_link(thread, definitions);
  return definitions;
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

static Arcadia_ADL_Definition*
inputDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPOperatorDefinition* mix,
    Arcadia_SizeValue index
  )
{
  Arcadia_ADL_Reference* reference = (Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueCheckedAt(
    thread, mix->inputs, index, _Arcadia_ADL_Reference_getType(thread));
  return reference->definition;
}

static void
checkExpansion
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions
  )
{
  Arcadia_ADL_DSPOperatorDefinition* first = (Arcadia_ADL_DSPOperatorDefinition*)requireDefinition(thread, definitions, "First.output", _Arcadia_ADL_DSPOperatorDefinition_getType(thread));
  Arcadia_ADL_DSPOperatorDefinition* second = (Arcadia_ADL_DSPOperatorDefinition*)requireDefinition(thread, definitions, "Second.output", _Arcadia_ADL_DSPOperatorDefinition_getType(thread));
  Arcadia_ADL_SineWaveDefinition* low = (Arcadia_ADL_SineWaveDefinition*)requireDefinition(thread, definitions, "First.__graph.low", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_ADL_SineWaveDefinition* high = (Arcadia_ADL_SineWaveDefinition*)requireDefinition(thread, definitions, "First.__graph.high", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_ADL_SineWaveDefinition* otherLow = (Arcadia_ADL_SineWaveDefinition*)requireDefinition(thread, definitions, "Second.__graph.low", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_ADL_SineWaveDefinition* otherHigh = (Arcadia_ADL_SineWaveDefinition*)requireDefinition(thread, definitions, "Second.__graph.high", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_ADL_SampleBufferDefinition* consumer = (Arcadia_ADL_SampleBufferDefinition*)requireDefinition(thread, definitions, "Consumer", _Arcadia_ADL_SampleBufferDefinition_getType(thread));
  Arcadia_Tests_assertTrue(thread, first->kind == Arcadia_ADL_DSPOperatorKind_Mix && second->kind == Arcadia_ADL_DSPOperatorKind_Mix);
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)first->inputs));
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)second->inputs));
  // Deliberately opposite to sine declaration order; summation alone cannot prove order.
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, first, 0) == (Arcadia_ADL_Definition*)high);
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, first, 1) == (Arcadia_ADL_Definition*)low);
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, second, 0) == (Arcadia_ADL_Definition*)otherHigh);
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, second, 1) == (Arcadia_ADL_Definition*)otherLow);
  Arcadia_Tests_assertTrue(thread, first != second && low != otherLow && high != otherHigh && first->inputs != second->inputs);
  Arcadia_Tests_assertEqualInteger64(thread, 220, low->frequency);
  Arcadia_Tests_assertEqualInteger64(thread, 880, high->frequency);
  Arcadia_Tests_assertEqualInteger64(thread, 110, otherLow->frequency);
  Arcadia_Tests_assertEqualInteger64(thread, 660, otherHigh->frequency);
  Arcadia_Tests_assertTrue(thread, consumer->dsp->definition == (Arcadia_ADL_Definition*)first);
  // Exactly three definitions per instance and the consumer: no duplicate internal output.
  Arcadia_Tests_assertEqualSize(thread, 7, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)definitions->definitions));
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "First.__graph.mix")));
  Arcadia_Tests_assertTrue(thread, NULL == Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions, Arcadia_String_createFromCxxString(thread, "First.__graph.output")));
}

static void
runExpansion
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue rotation,
    Arcadia_BooleanValue reverse
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "[");
  if (reverse) Arcadia_StringBuilder_insertBackCxxString(thread, text, instances);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "{ type: \"GraphDefinition\", name: \"Dual\", nodes: [");
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    Arcadia_SizeValue index = reverse ? (rotation + 6 - i) % 6 : (rotation + i) % 6;
    Arcadia_StringBuilder_insertBackCxxString(thread, text, dualNodes[index]);
  }
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "] },");
  if (!reverse) Arcadia_StringBuilder_insertBackCxxString(thread, text, instances);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "]");
  Arcadia_ADL_Definitions* definitions = expandBatch(thread, text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  checkExpansion(thread, definitions);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  checkExpansion(thread, definitions);
  Arcadia_ValueStack_popValues(thread, 1);
}

// Invalid templates are checked without an instance or ordinary readers. The
// positive control in each case prevents unsupported DSP.Mix from masking an edge bug.
static void
validateTemplate
  (
    Arcadia_Thread* thread,
    const char* nodes
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "[{ type: \"GraphDefinition\", name: \"Invalid\", nodes: [");
  Arcadia_StringBuilder_insertBackCxxString(thread, text, nodes);
  Arcadia_StringBuilder_insertBackCxxString(thread, text, "] }]");
  Arcadia_List* roots = parseBatch(thread, text);
  Arcadia_ADL_GraphCollector* collector = Arcadia_ADL_GraphCollector_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)collector);
  Arcadia_ADL_GraphCollector_addNode(thread, collector, (Arcadia_DDL_Node*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, roots, 0, _Arcadia_DDL_MapNode_getType(thread)));
  Arcadia_ADL_GraphCollector_validate(thread, collector);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
expectInvalidTemplate
  (
    Arcadia_Thread* thread,
    const char* nodes
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    validateTemplate(thread, nodes);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_SemanticalError, status);
  }
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

static void
dualExpansion
  (
    Arcadia_Thread* thread
  )
{
  runExpansion(thread, 0, false);
}

static void
dualDeclarationOrder
  (
    Arcadia_Thread* thread
  )
{
  for (Arcadia_SizeValue i = 0; i < 6; ++i) {
    runExpansion(thread, i, false);
    runExpansion(thread, i, true);
  }
}

static void
dualWrongFamily
  (
    Arcadia_Thread* thread
  )
{
  runExpansion(thread, 0, false);
  expectInvalidTemplate(thread,
    "{ type: \"Color\", name: \"color\" },"
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"color\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"Graph.Parameter\", name: \"color\", referenceType: \"Color\" },"
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"color\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"Graph.Parameter\", name: \"frequency\", valueType: \"Integer32\" },"
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"frequency\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"Graph.Parameter\", name: \"external\", referenceType: \"DSP.Mix\" },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"external\" }");
}

static void
dualMissingInput
  (
    Arcadia_Thread* thread
  )
{
  runExpansion(thread, 0, false);
  expectInvalidTemplate(thread,
    "{ type: \"Graph.Parameter\", name: \"frequency\", valueType: \"Integer32\" },"
    "{ type: \"DSP.SineWave\", name: \"tone\", frequency: \"frequency\" },"
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"tone\", \"missing\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"output\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
}

static void
dualInputShape
  (
    Arcadia_Thread* thread
  )
{
  runExpansion(thread, 0, false);
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"mix\" },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: \"tone\" },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [42] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
}

static void
dualCycles
  (
    Arcadia_Thread* thread
  )
{
  runExpansion(thread, 0, false);
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"mix\", inputs: [\"mix\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"mix\" }");
  expectInvalidTemplate(thread,
    "{ type: \"DSP.Mix\", name: \"first\", inputs: [\"second\"] },"
    "{ type: \"DSP.Mix\", name: \"second\", inputs: [\"first\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"first\" }");
}

static void
dualNestedMix
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_StringBuilder* text = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, text,
    "[{ type: \"GraphDefinition\", name: \"Nested\", nodes: ["
    "{ type: \"Graph.Parameter\", name: \"frequency\", valueType: \"Integer32\" },"
    "{ type: \"DSP.SineWave\", name: \"tone\", frequency: \"frequency\" },"
    "{ type: \"DSP.Mix\", name: \"inner\", inputs: [\"tone\", \"tone\"] },"
    "{ type: \"DSP.Mix\", name: \"outer\", inputs: [\"inner\", \"tone\", \"inner\"] },"
    "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.Mix\", source: \"outer\" }] },"
    "{ type: \"GraphInstance\", name: \"NestedInstance\", graph: \"Nested\", bindings: [{ parameter: \"frequency\", value: 330 }] }]");
  Arcadia_ADL_Definitions* definitions = expandBatch(thread, text);
  Arcadia_ADL_DSPOperatorDefinition* outer = (Arcadia_ADL_DSPOperatorDefinition*)requireDefinition(thread, definitions, "NestedInstance.output", _Arcadia_ADL_DSPOperatorDefinition_getType(thread));
  Arcadia_ADL_DSPOperatorDefinition* inner = (Arcadia_ADL_DSPOperatorDefinition*)requireDefinition(thread, definitions, "NestedInstance.__graph.inner", _Arcadia_ADL_DSPOperatorDefinition_getType(thread));
  Arcadia_ADL_Definition* tone = requireDefinition(thread, definitions, "NestedInstance.__graph.tone", _Arcadia_ADL_SineWaveDefinition_getType(thread));
  Arcadia_Tests_assertEqualSize(thread, 3, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)outer->inputs));
  Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)inner->inputs));
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, outer, 0) == (Arcadia_ADL_Definition*)inner);
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, outer, 1) == tone);
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, outer, 2) == (Arcadia_ADL_Definition*)inner);
  Arcadia_Tests_assertTrue(thread, inputDefinition(thread, inner, 0) == tone && inputDefinition(thread, inner, 1) == tone);
}

int
Arcadia_ADL_Tests_Graphs_dualExpansion
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualExpansion) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_dualDeclarationOrder
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualDeclarationOrder) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_dualWrongFamily
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualWrongFamily) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_dualMissingInput
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualMissingInput) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_dualInputShape
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualInputShape) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_dualCycles
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualCycles) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
Arcadia_ADL_Tests_Graphs_dualNestedMix
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&dualNestedMix) ? EXIT_SUCCESS : EXIT_FAILURE;
}
