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

#include "Arcadia.ADL.Tests.Definitions/DSP/SineWave.h"

#include "Arcadia/ADL/Include.h"
#include <stdio.h>

void
Arcadia_ADL_Tests_DSP_SineWaveDefinitionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  {
    static const char* DDL =
      "{\n"
      "  type : \"DSP.SineWave\",\n"
      "  name : \"MyGame.MyDSP.MySineWave\",\n"
      "  frequency : 220,\n"
      "}\n"
      ;
    Arcadia_ADL_Context_readFromString(thread, context, definitions, Arcadia_String_createFromCxxString(thread, DDL), Arcadia_BooleanValue_False);
  }
  Arcadia_ADL_Definitions_link(thread, definitions);
}

static void
testFrequencyDomain
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue sawtooth,
    Arcadia_BooleanValue graph,
    Arcadia_BooleanValue positive
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  const Arcadia_Integer32Value frequencies[] = { positive ? 1 : 0, positive ? 440 : -1,
    positive ? Arcadia_Integer32Value_Maximum : Arcadia_Integer32Value_Minimum };
  for (size_t i = 0; i < 3; ++i) {
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    char text[1024];
    int length;
    if (graph) {
      length = snprintf(text, sizeof(text),
        "[{ type: \"GraphDefinition\", name: \"ToneGraph\", nodes: ["
        "{ type: \"Graph.Parameter\", name: \"Frequency\", valueType: \"Integer32\" },"
        "{ type: \"DSP.SineWave\", name: \"tone\", frequency: \"Frequency\" },"
        "{ type: \"Graph.Output\", name: \"output\", referenceType: \"DSP.SineWave\", source: \"tone\" }] },"
        "{ type: \"GraphInstance\", name: \"Tone\", graph: \"ToneGraph\", bindings: ["
        "{ parameter: \"Frequency\", value: %d }] }]", (int)frequencies[i]);
    } else {
      length = snprintf(text, sizeof(text), "{ type: \"DSP.%s\", name: \"Tone\", frequency: %d }",
        sawtooth ? "SawtoothWave" : "SineWave", (int)frequencies[i]);
    }
    Arcadia_Tests_assertTrue(thread, length > 0 && (size_t)length < sizeof(text));
    // Parse outside the error boundary: syntax failures must not masquerade as
    // source-domain rejection. No Engine factory or Media constructor is called.
    Arcadia_DDL_DefaultReader* reader = Arcadia_DDL_DefaultReader_create(thread);
    Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, reader,
      (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread,
        (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread,
          Arcadia_String_toByteArray(thread, Arcadia_String_createFromCxxString(thread, text)))));
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node,
      graph ? _Arcadia_DDL_ListNode_getType(thread) : _Arcadia_DDL_MapNode_getType(thread)));
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      if (graph) {
        Arcadia_ADL_Graphs_readFromNodes(thread, context, definitions, ((Arcadia_DDL_ListNode*)node)->elements);
      } else {
        Arcadia_ADL_Context_readFromNode(thread, context, definitions, node, false);
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, positive);
      Arcadia_ADL_Definition* definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions,
        Arcadia_String_createFromCxxString(thread, graph ? "Tone.output" : "Tone"));
      Arcadia_Tests_assertTrue(thread, definition != NULL && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition,
        sawtooth ? _Arcadia_ADL_SawtoothWaveDefinition_getType(thread) : _Arcadia_ADL_SineWaveDefinition_getType(thread)));
      Arcadia_Integer32Value frequency = sawtooth ? ((Arcadia_ADL_SawtoothWaveDefinition*)definition)->frequency
        : ((Arcadia_ADL_SineWaveDefinition*)definition)->frequency;
      Arcadia_Tests_assertEqualInteger64(thread, frequencies[i], frequency);
      Arcadia_ADL_Definitions_link(thread, definitions);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    } else {
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Tests_assertTrue(thread, !positive);
      Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_SemanticalError, status);
      // Domain rejection must precede publication of the invalid definition.
      Arcadia_Tests_assertTrue(thread, Arcadia_ADL_Definitions_getDefinitionOrNull(thread, definitions,
        Arcadia_String_createFromCxxString(thread, graph ? "Tone.output" : "Tone")) == NULL);
    }
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  }
  Arcadia_ValueStack_popValues(thread, 1);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

#define FREQUENCY_CASE(Name, Sawtooth, Graph, Positive) \
static void \
test##Name \
  ( \
    Arcadia_Thread* thread \
  ) \
{ \
  testFrequencyDomain(thread, Sawtooth, Graph, Positive); \
} \
int \
Arcadia_ADL_Tests_##Name \
  ( \
    void \
  ) \
{ \
  return Arcadia_Tests_safeExecute(&test##Name) ? EXIT_SUCCESS : EXIT_FAILURE; \
}

FREQUENCY_CASE(SinePositiveFrequency, false, false, true)
FREQUENCY_CASE(SineNonpositiveFrequency, false, false, false)
FREQUENCY_CASE(SawtoothPositiveFrequency, true, false, true)
FREQUENCY_CASE(SawtoothNonpositiveFrequency, true, false, false)
FREQUENCY_CASE(GraphsSinePositiveFrequency, false, true, true)
FREQUENCY_CASE(GraphsSineNonpositiveFrequency, false, true, false)

#undef FREQUENCY_CASE
