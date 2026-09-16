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

#include "Arcadia/Audials/Implementation/Include.h"
#include "Arcadia/Media/Include.h"

#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE (48000)
#define LENGTH_SECONDS (4)

static Arcadia_Engine_Audials_BackendContext*
createAudialsBackendContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine* engine
  )
{
  Arcadia_Engine_Audials_Implementation_registerBackends(thread, engine->audialsBackendTypes);

  Arcadia_List* backendTypes = NULL;
  Arcadia_Engine_Backend* backend = NULL;
  Arcadia_Engine_Audials_BackendContext* backendContext = NULL;
  Arcadia_BooleanValue backendTypesLocked = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue backendLocked = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue backendContextLocked = Arcadia_BooleanValue_False;

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    backendTypes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendTypes);
    backendTypesLocked = Arcadia_BooleanValue_True;
    Arcadia_Set_getAll(thread, engine->audialsBackendTypes, backendTypes);
    if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)backendTypes)) {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Object_unlock(thread, (Arcadia_Object*)backendTypes);
      return NULL;
    }

    Arcadia_Value backendTypeValue = Arcadia_List_getAt(thread, backendTypes, 0);
    if (!Arcadia_Value_isTypeValue(&backendTypeValue)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
    Arcadia_Type* backendType = Arcadia_Value_getTypeValue(&backendTypeValue);
    if (!Arcadia_Type_isDescendantType(thread, backendType, _Arcadia_Engine_Audials_Backend_getType(thread))) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }

    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    backend = (Arcadia_Engine_Backend*)_Arcadia_EndCreate0(thread, backendType, Arcadia_ValueStack_getSize(thread) - 1);
    Arcadia_Object_lock(thread, (Arcadia_Object*)backend);
    backendLocked = Arcadia_BooleanValue_True;
    backendContext = (Arcadia_Engine_Audials_BackendContext*)Arcadia_Engine_Backend_createBackendContext(thread, backend);
    Arcadia_Object_lock(thread, (Arcadia_Object*)backendContext);
    backendContextLocked = Arcadia_BooleanValue_True;
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (backendContextLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)backendContext);
    if (backendLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)backend);
    if (backendTypesLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)backendTypes);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)backend);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)backendTypes);
  engine->audialsBackendContext = (Arcadia_Engine_BackendContext*)backendContext;
  return backendContext;
}

static void
playSampleBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Process* process,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_Media_SampleBuffer* sampleBuffer
  )
{
  Arcadia_Engine_Audials_Implementation_SoundSourceResource* soundSourceResource =
    Arcadia_Engine_Audials_BackendContext_createSoundSourceResource(thread, backendContext, sampleBuffer);
  Arcadia_Engine_Audials_Implementation_Resource_ref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)soundSourceResource);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Engine_Audials_Implementation_Resource_render(thread, (Arcadia_Engine_Audials_Implementation_Resource*)soundSourceResource);
    Arcadia_Engine_Audials_Implementation_SoundSourceResource_play(thread, soundSourceResource);

    printf("Playing spaceship ambience through the audials backend.\n");
    Arcadia_Natural64Value startTick = Arcadia_getTickCount(thread);
    do {
      Arcadia_Process_stepARMS(process);
      Arcadia_Engine_BackendContext_update(thread, (Arcadia_Engine_BackendContext*)backendContext);
    } while (Arcadia_Engine_Audials_Implementation_SoundSourceResource_isPlaying(thread, soundSourceResource) &&
             Arcadia_getTickCount(thread) - startTick < (LENGTH_SECONDS + 2) * 1000);

    Arcadia_Engine_Audials_Implementation_SoundSourceResource_stop(thread, soundSourceResource);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Engine_Audials_Implementation_Resource_unref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)soundSourceResource);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Engine_Audials_Implementation_SoundSourceResource_stop(thread, soundSourceResource);
    Arcadia_Engine_Audials_Implementation_Resource_unref(thread, (Arcadia_Engine_Audials_Implementation_Resource*)soundSourceResource);
    Arcadia_Thread_jump(thread);
  }
}

static void
main1
  (
    Arcadia_Process* process
  )
{
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_Engine* engine = NULL;
  Arcadia_Engine_Audials_BackendContext* backendContext = NULL;
  Arcadia_Media_DSP_Graph* graph = NULL;
  Arcadia_Media_DSP_SineWave* humLowGenerator = NULL;
  Arcadia_Media_DSP_SineWave* humHighGenerator = NULL;
  Arcadia_Media_DSP_Gain* humLowGain = NULL;
  Arcadia_Media_DSP_Gain* humHighGain = NULL;
  Arcadia_Media_DSP_Mix* humGenerator = NULL;
  Arcadia_Media_DSP_WhiteNoise* airNoiseSource = NULL;
  Arcadia_Media_DSP_Gain* airInputGain = NULL;
  Arcadia_Media_DSP_LowPass* airLowPass = NULL;
  Arcadia_Media_DSP_Gain* airGenerator = NULL;
  Arcadia_Media_DSP_BrownNoise* rumbleNoiseSource = NULL;
  Arcadia_Media_DSP_LowPass* rumbleGenerator = NULL;
  Arcadia_Media_DSP_Gain* rumbleGain = NULL;
  Arcadia_Media_DSP_SineWave* rumbleLfo = NULL;
  Arcadia_Media_DSP_Affine* rumbleLfoScale = NULL;
  Arcadia_Media_DSP_Multiply* modulatedRumble = NULL;
  Arcadia_Media_DSP_Mix* finalGenerator = NULL;
  Arcadia_Media_SampleBuffer* sampleBuffer = NULL;

  Arcadia_BooleanValue engineLocked = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue graphLocked = Arcadia_BooleanValue_False;
  Arcadia_BooleanValue sampleBufferLocked = Arcadia_BooleanValue_False;

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    engine = Arcadia_Engine_getOrCreate(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)engine);
    engineLocked = Arcadia_BooleanValue_True;
    backendContext = createAudialsBackendContext(thread, engine);
    graph = Arcadia_Media_DSP_Graph_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)graph);
    graphLocked = Arcadia_BooleanValue_True;

    // Build a soft spaceship-interior ambience graph:
    //
    //   Sine(55 Hz)  -> Gain(0.10)  --\
    //   Sine(110 Hz) -> Gain(0.035) --+--> Mix(hum) --\
    //                                                |
    //   WhiteNoise -> Gain(0.70) -> LowPass(850 Hz) -> Gain(0.025) --+--> Mix(final)
    //                                                                |
    //   BrownNoise(seed=2, amp=0.65) -> LowPass(70 Hz) -> Gain(0.09) -+--> Multiply --/
    //                                                                       ^
    //   Sine(0.08 Hz) -> Affine(scale=0.075, bias=0.925) -------------------/
    //
    // The sine pair creates the steady electrical hum. Filtered white noise adds
    // air circulation. Filtered brown noise adds engine-room mass, and the slow
    // sine control signal modulates that rumble so the ambience breathes.
    humLowGenerator = Arcadia_Media_DSP_SineWave_create(thread, 55);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)humLowGenerator);
    humHighGenerator = Arcadia_Media_DSP_SineWave_create(thread, 110);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)humHighGenerator);
    humLowGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)humLowGenerator, 0.10f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)humLowGain);
    humHighGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)humHighGenerator, 0.035f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)humHighGain);
    humGenerator = Arcadia_Media_DSP_Mix_create(thread);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)humGenerator);
    Arcadia_Media_DSP_Mix_addInput(thread, humGenerator, (Arcadia_Media_DSP*)humLowGain);
    Arcadia_Media_DSP_Mix_addInput(thread, humGenerator, (Arcadia_Media_DSP*)humHighGain);
    airNoiseSource = Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 1.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)airNoiseSource);
    airInputGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)airNoiseSource, 0.70f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)airInputGain);
    airLowPass = Arcadia_Media_DSP_LowPass_create(thread, (Arcadia_Media_DSP*)airInputGain, 850.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)airLowPass);
    airGenerator = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)airLowPass, 0.025f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)airGenerator);
    rumbleNoiseSource = Arcadia_Media_DSP_BrownNoise_create(thread, 2u, 0.65f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)rumbleNoiseSource);
    rumbleGenerator = Arcadia_Media_DSP_LowPass_create(thread, (Arcadia_Media_DSP*)rumbleNoiseSource, 70.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)rumbleGenerator);
    rumbleGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)rumbleGenerator, 0.09f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)rumbleGain);
    rumbleLfo = Arcadia_Media_DSP_SineWave_create(thread, 0.08f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)rumbleLfo);
    rumbleLfoScale = Arcadia_Media_DSP_Affine_create(thread, (Arcadia_Media_DSP*)rumbleLfo, 0.075f, 0.925f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)rumbleLfoScale);
    modulatedRumble = Arcadia_Media_DSP_Multiply_create(thread, (Arcadia_Media_DSP*)rumbleGain, (Arcadia_Media_DSP*)rumbleLfoScale);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)modulatedRumble);
    finalGenerator = Arcadia_Media_DSP_Mix_create(thread);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)finalGenerator);
    Arcadia_Media_DSP_Mix_addInput(thread, finalGenerator, (Arcadia_Media_DSP*)humGenerator);
    Arcadia_Media_DSP_Mix_addInput(thread, finalGenerator, (Arcadia_Media_DSP*)airGenerator);
    Arcadia_Media_DSP_Mix_addInput(thread, finalGenerator, (Arcadia_Media_DSP*)modulatedRumble);
    Arcadia_Media_DSP_Graph_setOutput(thread, graph, (Arcadia_Media_DSP*)finalGenerator);
    sampleBuffer = Arcadia_Media_SampleBuffer_create(thread, LENGTH_SECONDS, SAMPLE_RATE, Arcadia_Media_SampleFormat_Integer16);
    Arcadia_Object_lock(thread, (Arcadia_Object*)sampleBuffer);
    sampleBufferLocked = Arcadia_BooleanValue_True;

    Arcadia_Media_SampleBuffer_fill(thread, sampleBuffer, (Arcadia_Media_DSP*)graph);
    printf("Rendered %d seconds of spaceship ambience at %d Hz into an Arcadia sample buffer.\n", LENGTH_SECONDS, SAMPLE_RATE);
    if (backendContext) {
      playSampleBuffer(thread, process, backendContext, sampleBuffer);
    } else {
      printf("No audials backend is available. Configure with OpenAL enabled to play the generated sample buffer.\n");
    }

    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (sampleBufferLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)sampleBuffer);
    if (graphLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
    if (engine && engine->audialsBackendContext) {
      Arcadia_Object_unlock(thread, (Arcadia_Object*)engine->audialsBackendContext);
      engine->audialsBackendContext = NULL;
    }
    if (engineLocked) Arcadia_Object_unlock(thread, (Arcadia_Object*)engine);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Object_unlock(thread, (Arcadia_Object*)sampleBuffer);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
  if (engine->audialsBackendContext) {
    Arcadia_Object_unlock(thread, (Arcadia_Object*)engine->audialsBackendContext);
    engine->audialsBackendContext = NULL;
  }
  Arcadia_Object_unlock(thread, (Arcadia_Object*)engine);
}

int
main
  (
    int argc,
    char** argv
  )
{
  (void)argc;
  (void)argv;

  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    return EXIT_FAILURE;
  }

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    main1(process);
  }
  Arcadia_Thread_popJumpTarget(thread);

  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Process_relinquish(process);
  return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
