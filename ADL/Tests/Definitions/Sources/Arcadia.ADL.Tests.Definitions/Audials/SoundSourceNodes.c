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

#include "Arcadia.ADL.Tests.Definitions/Audials/SoundSourceNodes.h"

#include "Arcadia/ADL/Include.h"
#include <stdio.h>

void
Arcadia_ADL_Tests_Audials_SoundSourceNodesTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);

  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"DSP.SineWave\", name: \"Test.DSP\", frequency: 220, }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"SoundSourceNode\", name: \"Test.DefaultSource\", sampleBuffer: \"Test.Buffer\", }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"DSP.WhiteNoise\", name: \"Test.Noise\", }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"DSP.Gain\", name: \"Test.NoiseGain\", input: \"Test.Noise\", gain: 0.025, }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"DSP.LowPass\", name: \"Test.Filter\", input: \"Test.NoiseGain\", cutoff: 850, }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"DSP.Mix\", name: \"Test.Mix\", inputs: [ \"Test.DSP\", \"Test.Filter\" ], }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"SampleBuffer\", name: \"Test.Buffer\", duration: 1, dsp: \"Test.DSP\", }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"SoundSourceNode\", name: \"Test.FiniteSource\", sampleBuffer: \"Test.Buffer\", isLooping: true, volume: 0.5, }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"DSPStreamProvider\", name: \"Test.Provider\", dsp: \"Test.DSP\", sampleRate: 48000, }"
        ),
      Arcadia_BooleanValue_False
    );
  Arcadia_ADL_Context_readFromString
    (
      thread,
      context,
      definitions,
      Arcadia_String_createFromCxxString
        (
          thread,
          u8"{ type: \"StreamingSoundSourceNode\", name: \"Test.StreamingSource\", provider: \"Test.Provider\", }"
        ),
      Arcadia_BooleanValue_False
    );

  Arcadia_ADL_Definitions_link(thread, definitions);

  Arcadia_ADL_SoundSourceNodeDefinition* finiteSource =
    (Arcadia_ADL_SoundSourceNodeDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull
      (
        thread,
        definitions,
        Arcadia_String_createFromCxxString(thread, u8"Test.FiniteSource")
      );
  Arcadia_Tests_assertTrue(thread, finiteSource->isLooping == Arcadia_BooleanValue_True);
  Arcadia_Tests_assertTrue(thread, finiteSource->volume == 0.5f);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)finiteSource->sampleBuffer->definition, _Arcadia_ADL_SampleBufferDefinition_getType(thread)));

  Arcadia_ADL_SoundSourceNodeDefinition* defaultSource =
    (Arcadia_ADL_SoundSourceNodeDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull
      (
        thread,
        definitions,
        Arcadia_String_createFromCxxString(thread, u8"Test.DefaultSource")
      );
  Arcadia_Tests_assertTrue(thread, defaultSource->isLooping == Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, defaultSource->volume == 1.f);

  Arcadia_ADL_DSPOperatorDefinition* mix =
    (Arcadia_ADL_DSPOperatorDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull
      (
        thread,
        definitions,
        Arcadia_String_createFromCxxString(thread, u8"Test.Mix")
      );
  Arcadia_Tests_assertTrue(thread, mix->kind == Arcadia_ADL_DSPOperatorKind_Mix);
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)mix->inputs) == 2);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_ADL_Reference*)Arcadia_List_getObjectReferenceValueAt(thread, mix->inputs, 1))->definition != NULL);

  Arcadia_ADL_DSPOperatorDefinition* lowPass =
    (Arcadia_ADL_DSPOperatorDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull
      (
        thread,
        definitions,
        Arcadia_String_createFromCxxString(thread, u8"Test.Filter")
      );
  Arcadia_Tests_assertTrue(thread, lowPass->kind == Arcadia_ADL_DSPOperatorKind_LowPass);
  Arcadia_Tests_assertTrue(thread, lowPass->parameter == 850.f);

  Arcadia_ADL_DSPStreamProviderDefinition* provider =
    (Arcadia_ADL_DSPStreamProviderDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull
      (
        thread,
        definitions,
        Arcadia_String_createFromCxxString(thread, u8"Test.Provider")
      );
  Arcadia_Tests_assertTrue(thread, provider->sampleRate == 48000);
  Arcadia_Tests_assertTrue(thread, provider->hasDuration == Arcadia_BooleanValue_False);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)provider->dsp->definition, _Arcadia_ADL_SineWaveDefinition_getType(thread)));

  Arcadia_ADL_StreamingSoundSourceNodeDefinition* streamingSource =
    (Arcadia_ADL_StreamingSoundSourceNodeDefinition*)Arcadia_ADL_Definitions_getDefinitionOrNull
      (
        thread,
        definitions,
        Arcadia_String_createFromCxxString(thread, u8"Test.StreamingSource")
      );
  Arcadia_Tests_assertTrue(thread, streamingSource->volume == 1.f);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)streamingSource->provider->definition, _Arcadia_ADL_DSPStreamProviderDefinition_getType(thread)));
}

static Arcadia_DDL_Node*
parseOperator
  (
    Arcadia_Thread* thread,
    const char* text
  )
{
  Arcadia_DDL_DefaultReader* reader = Arcadia_DDL_DefaultReader_create(thread);
  Arcadia_ByteReader* bytes = (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread,
    Arcadia_String_toByteArray(thread, Arcadia_String_createFromCxxString(thread, text)));
  Arcadia_DDL_Node* node = Arcadia_DDL_DefaultReader_run(thread, reader,
    (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, bytes));
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_DDL_MapNode_getType(thread)));
  return node;
}

static Arcadia_ADL_Reader*
requireOperatorReader
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    const char* type
  )
{
  Arcadia_Value reader = Arcadia_Map_get(thread, context->readers,
    Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, type)));
  return (Arcadia_ADL_Reader*)Arcadia_Value_getObjectReferenceValueChecked(thread, reader, _Arcadia_ADL_Reader_getType(thread));
}

static void
expectOperatorReadFailure
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context,
    Arcadia_ADL_Reader* reader,
    const char* text
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  // Parse before the expected-error boundary, so syntax failures cannot pass
  // as schema/variant validation. Use the registered reader directly to reach
  // its required-type/name schema even when context dispatch would reject first.
  Arcadia_DDL_Node* node = parseOperator(thread, text);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)node);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ADL_Reader_read(thread, reader, context, definitions, node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
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
checkOperatorLinking
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Context* context
  )
{
  static const char* const unresolved[] = {
    "{ type: \"DSP.Gain\", name: \"Operator\", input: \"Missing\", gain: 1 }",
    "{ type: \"DSP.LowPass\", name: \"Operator\", input: \"Missing\", cutoff: 850 }",
    "{ type: \"DSP.Mix\", name: \"Operator\", inputs: [\"Missing\"] }",
  };
  for (unsigned i = 0; i < 3; ++i) {
    Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
    Arcadia_ADL_Definition* definition = Arcadia_ADL_Context_readFromString(thread, context, definitions,
      Arcadia_String_createFromCxxString(thread, unresolved[i]), false);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definition);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ADL_Definition_link(thread, definition);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - entry);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_NotFound, status);
    }
  }
  // Ordinary operator linking resolves names, but does not enforce DSP-family
  // types. Do not import graph-local family restrictions into this reader.
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ADL_Definition* color = Arcadia_ADL_Context_readFromString(thread, context, definitions,
    Arcadia_String_createFromCxxString(thread, "{ type: \"Color\", name: \"Color\", red: 1, green: 2, blue: 3 }"), false);
  Arcadia_ADL_DSPOperatorDefinition* gain = (Arcadia_ADL_DSPOperatorDefinition*)Arcadia_ADL_Context_readFromString(thread, context, definitions,
    Arcadia_String_createFromCxxString(thread, "{ type: \"DSP.Gain\", name: \"Gain\", input: \"Color\", gain: -2 }"), false);
  Arcadia_ADL_Definitions_link(thread, definitions);
  Arcadia_Tests_assertTrue(thread, gain->input->definition == color);
  Arcadia_Tests_assertEqualReal64(thread, -2.0, gain->parameter);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
testDSPOperatorValidation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_ADL_Context* context = Arcadia_ADL_Context_getOrCreate(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ADL_Reader* reader = requireOperatorReader(thread, context, "DSP.Gain");
  Arcadia_Tests_assertTrue(thread, reader == requireOperatorReader(thread, context, "DSP.LowPass"));
  Arcadia_Tests_assertTrue(thread, reader == requireOperatorReader(thread, context, "DSP.Mix"));

  static const struct {
    const char* reason;
    const char* text;
  } invalid[] = {
    { "missing type", "{ name: \"X\", input: \"Tone\", gain: 1 }" },
    { "wrong type tag", "{ type: 7, name: \"X\", input: \"Tone\", gain: 1 }" },
    { "missing name", "{ type: \"DSP.Gain\", input: \"Tone\", gain: 1 }" },
    { "wrong name tag", "{ type: \"DSP.Gain\", name: false, input: \"Tone\", gain: 1 }" },
    { "wrong input tag", "{ type: \"DSP.Gain\", name: \"X\", input: 7, gain: 1 }" },
    { "gain missing input", "{ type: \"DSP.Gain\", name: \"X\", gain: 1 }" },
    { "gain missing parameter", "{ type: \"DSP.Gain\", name: \"X\", input: \"Tone\" }" },
    { "wrong gain tag", "{ type: \"DSP.Gain\", name: \"X\", input: \"Tone\", gain: \"1\" }" },
    { "gain inappropriate inputs", "{ type: \"DSP.Gain\", name: \"X\", input: \"Tone\", gain: 1, inputs: [\"Tone\"] }" },
    { "gain inappropriate cutoff", "{ type: \"DSP.Gain\", name: \"X\", input: \"Tone\", gain: 1, cutoff: 850 }" },
    { "lowpass missing input", "{ type: \"DSP.LowPass\", name: \"X\", cutoff: 850 }" },
    { "lowpass missing parameter", "{ type: \"DSP.LowPass\", name: \"X\", input: \"Tone\" }" },
    { "wrong cutoff tag", "{ type: \"DSP.LowPass\", name: \"X\", input: \"Tone\", cutoff: false }" },
    { "lowpass inappropriate gain", "{ type: \"DSP.LowPass\", name: \"X\", input: \"Tone\", cutoff: 850, gain: 1 }" },
    { "lowpass inappropriate inputs", "{ type: \"DSP.LowPass\", name: \"X\", input: \"Tone\", cutoff: 850, inputs: [\"Tone\"] }" },
    { "mix missing inputs", "{ type: \"DSP.Mix\", name: \"X\" }" },
    { "mix empty inputs", "{ type: \"DSP.Mix\", name: \"X\", inputs: [] }" },
    { "mix wrong list tag", "{ type: \"DSP.Mix\", name: \"X\", inputs: \"Tone\" }" },
    { "mix wrong later element", "{ type: \"DSP.Mix\", name: \"X\", inputs: [\"Tone\", 7] }" },
    { "mix inappropriate input", "{ type: \"DSP.Mix\", name: \"X\", inputs: [\"Tone\"], input: \"Tone\" }" },
    { "mix inappropriate gain", "{ type: \"DSP.Mix\", name: \"X\", inputs: [\"Tone\"], gain: 1 }" },
    { "mix inappropriate cutoff", "{ type: \"DSP.Mix\", name: \"X\", inputs: [\"Tone\"], cutoff: 850 }" },
    { "unsupported operator", "{ type: \"DSP.Unknown\", name: \"X\", input: \"Tone\", gain: 1 }" },
    { "unknown property", "{ type: \"DSP.Gain\", name: \"X\", input: \"Tone\", gain: 1, extra: 0 }" },
    { "duplicate property", "{ type: \"DSP.Gain\", name: \"X\", input: \"Tone\", gain: 1, gain: 2 }" },
  };
  for (unsigned i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    printf("operator negative: %s\n", invalid[i].reason);
    expectOperatorReadFailure(thread, context, reader, invalid[i].text);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  }

  // Numeric domain restrictions are not reader policy. In particular, a zero
  // gain is valid and cutoff positivity is checked by the native DSP later.
  static const char* const valid[] = {
    "{ type: \"DSP.Gain\", name: \"Gain\", input: \"Later\", gain: 0 }",
    "{ type: \"DSP.LowPass\", name: \"LowPass\", input: \"Later\", cutoff: 0 }",
    "{ type: \"DSP.Mix\", name: \"Mix\", inputs: [\"Later\", \"Later\"] }",
  };
  for (unsigned i = 0; i < 3; ++i) {
    Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
    Arcadia_ADL_Definition* definition = Arcadia_ADL_Context_readFromString(thread, context, definitions,
      Arcadia_String_createFromCxxString(thread, valid[i]), false);
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)definition, _Arcadia_ADL_DSPOperatorDefinition_getType(thread)));
    Arcadia_ADL_DSPOperatorDefinition* operatorDefinition = (Arcadia_ADL_DSPOperatorDefinition*)definition;
    Arcadia_Tests_assertTrue(thread, operatorDefinition->kind == (Arcadia_ADL_DSPOperatorKind)i);
    if (i < 2) {
      Arcadia_Tests_assertEqualReal64(thread, 0.0, operatorDefinition->parameter);
      Arcadia_Tests_assertTrue(thread, operatorDefinition->input != NULL && operatorDefinition->input->definition == NULL && operatorDefinition->inputs == NULL);
    } else {
      Arcadia_Tests_assertTrue(thread, operatorDefinition->input == NULL);
      Arcadia_Tests_assertEqualSize(thread, 2, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)operatorDefinition->inputs));
    }
    Arcadia_ValueStack_popValues(thread, 1);
  }
  checkOperatorLinking(thread, context);
  Arcadia_ValueStack_popValues(thread, 1);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

int
Arcadia_ADL_Tests_Audials_dspOperatorValidation
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testDSPOperatorValidation) ? EXIT_SUCCESS : EXIT_FAILURE;
}
