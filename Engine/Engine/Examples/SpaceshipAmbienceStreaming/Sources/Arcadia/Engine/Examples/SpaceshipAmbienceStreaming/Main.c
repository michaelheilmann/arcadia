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

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Media/Include.h"

#include "Arcadia/Engine/Examples/SpaceshipAmbienceStreaming/Application.h"

#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE (48000)

static void
main1
  (
    Arcadia_Process* process,
    int argc,
    char** argv
  )
{
  (void)argc;
  (void)argv;
  Arcadia_Engine_Examples_SpaceshipAmbienceStreaming_Application* application = NULL;
  Arcadia_Media_DSP_Graph* graph = NULL;
  Arcadia_Engine_Audials_DSPStreamProvider* provider = NULL;
  Arcadia_Engine_Audials_StreamingSoundSourceNode* streamNode = NULL;
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    application = Arcadia_Engine_Examples_SpaceshipAmbienceStreaming_Application_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)application);
    Arcadia_Engine_Application_startup(thread, (Arcadia_Engine_Application*)application);
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Application*)application)->windows); i < n; ++i) {
      Arcadia_Engine_Visuals_Window* window = (Arcadia_Engine_Visuals_Window*)Arcadia_List_getObjectReferenceValueCheckedAt
        (
          thread,
          ((Arcadia_Engine_Application*)application)->windows,
          i,
          _Arcadia_Engine_Visuals_Window_getType(thread)
        );
      Arcadia_Engine_Visuals_Window_setTitle
        (
          thread,
          window,
          Arcadia_String_createFromCxxString(thread, u8"Spaceship Ambience Streaming (Escape to quit)")
        );
    }

    // Build the ambience DSP graph:
    // a low hum (55 Hz) and a high hum (110 Hz),
    // both filtered white noise ("air"), mixed into the graph output.
    graph = Arcadia_Media_DSP_Graph_create(thread);
    Arcadia_Object_lock(thread, (Arcadia_Object*)graph);
    Arcadia_Media_DSP_SineWave* lowHum = Arcadia_Media_DSP_SineWave_create(thread, 55.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)lowHum);
    Arcadia_Media_DSP_SineWave* highHum = Arcadia_Media_DSP_SineWave_create(thread, 110.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)highHum);
    Arcadia_Media_DSP_Gain* lowHumGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)lowHum, 0.10f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)lowHumGain);
    Arcadia_Media_DSP_Gain* highHumGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)highHum, 0.035f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)highHumGain);
    Arcadia_Media_DSP_WhiteNoise* airNoise = Arcadia_Media_DSP_WhiteNoise_create(thread, 1u, 1.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)airNoise);
    Arcadia_Media_DSP_LowPass* filteredAir = Arcadia_Media_DSP_LowPass_create(thread, (Arcadia_Media_DSP*)airNoise, 850.0f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)filteredAir);
    Arcadia_Media_DSP_Gain* airGain = Arcadia_Media_DSP_Gain_create(thread, (Arcadia_Media_DSP*)filteredAir, 0.025f);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)airGain);
    Arcadia_Media_DSP_Mix* ambience = Arcadia_Media_DSP_Mix_create(thread);
    Arcadia_Media_DSP_Graph_addNode(thread, graph, (Arcadia_Media_DSP*)ambience);
    Arcadia_Media_DSP_Mix_addInput(thread, ambience, (Arcadia_Media_DSP*)lowHumGain);
    Arcadia_Media_DSP_Mix_addInput(thread, ambience, (Arcadia_Media_DSP*)highHumGain);
    Arcadia_Media_DSP_Mix_addInput(thread, ambience, (Arcadia_Media_DSP*)airGain);
    Arcadia_Media_DSP_Graph_setOutput(thread, graph, (Arcadia_Media_DSP*)ambience);

    provider = Arcadia_Engine_Audials_DSPStreamProvider_createInfinite(thread, (Arcadia_Media_DSP*)graph, SAMPLE_RATE);
    Arcadia_Object_lock(thread, (Arcadia_Object*)provider);
    streamNode = Arcadia_Engine_Audials_StreamingSoundSourceNode_create
      (
        thread,
        (Arcadia_Engine_Audials_BackendContext*)((Arcadia_Engine_Application*)application)->engine->audialsBackendContext,
        (Arcadia_Engine_Audials_AudioStreamProvider*)provider
      );
    Arcadia_Object_lock(thread, (Arcadia_Object*)streamNode);
    // Publish the ambience objects so the application visits them during garbage collection
    // and stops the stream during application shutdown.
    application->graph = graph;
    application->provider = provider;
    application->streamNode = streamNode;
    Arcadia_Engine_Audials_Node_render(thread, (Arcadia_Engine_Audials_Node*)streamNode);
    Arcadia_Engine_Audials_StreamingSoundSourceNode_play(thread, streamNode);

    printf("Streaming spaceship ambience. Press Escape to quit.\n");
    Arcadia_Process_stepARMS(process);
    while (!Arcadia_Engine_Application_getQuitRequested(thread, (Arcadia_Engine_Application*)application)) {
      Arcadia_Process_stepARMS(process);
      Arcadia_Engine_BackendContext_update(thread, (Arcadia_Engine_BackendContext*)((Arcadia_Engine_Application*)application)->engine->audialsBackendContext);
      Arcadia_Engine_BackendContext_update(thread, (Arcadia_Engine_BackendContext*)((Arcadia_Engine_Application*)application)->engine->visualsBackendContext);
      for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Application*)application)->windows); i < n; ++i) {
        Arcadia_Engine_Visuals_Window* window = (Arcadia_Engine_Visuals_Window*)Arcadia_List_getObjectReferenceValueCheckedAt
          (
            thread,
            ((Arcadia_Engine_Application*)application)->windows,
            i,
            _Arcadia_Engine_Visuals_Window_getType(thread)
          );
        Arcadia_Engine_Visuals_Window_beginRender(thread, window);
        Arcadia_Engine_Visuals_Window_endRender(thread, window);
      }
      Arcadia_Engine_Event* event;
      while (NULL != (event = Arcadia_Engine_dequeEvent(thread, ((Arcadia_Engine_Application*)application)->engine))) {
        if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)event, _Arcadia_Engine_Visuals_WindowClosedEvent_getType(thread))) {
          Arcadia_Engine_Examples_SpaceshipAmbienceStreaming_Application_onWindowClosedEvent(thread, application, (Arcadia_Engine_Visuals_WindowClosedEvent*)event);
        } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)event, _Arcadia_Engine_Input_KeyboardKeyEvent_getType(thread))) {
          Arcadia_Engine_Input_KeyboardKeyEvent* keyEvent = (Arcadia_Engine_Input_KeyboardKeyEvent*)event;
          if (Arcadia_Engine_Input_KeyboardKeyEvent_getKey(thread, keyEvent) == Arcadia_Engine_Input_KeyboardKey_Escape) {
            Arcadia_Engine_Application_setQuitRequested(thread, (Arcadia_Engine_Application*)application, Arcadia_BooleanValue_True);
          }
        }
      }
    }
    // Application shutdown stops the ambience stream, saves the configuration,
    // closes the windows, and releases the backend contexts.
    Arcadia_Engine_Application_shutdown(thread, (Arcadia_Engine_Application*)application);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)streamNode);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)application);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (application) {
      Arcadia_Engine_Application_shutdown(thread, (Arcadia_Engine_Application*)application);
    }
    if (streamNode) Arcadia_Object_unlock(thread, (Arcadia_Object*)streamNode);
    if (provider) Arcadia_Object_unlock(thread, (Arcadia_Object*)provider);
    if (graph) Arcadia_Object_unlock(thread, (Arcadia_Object*)graph);
    if (application) Arcadia_Object_unlock(thread, (Arcadia_Object*)application);
    Arcadia_Thread_jump(thread);
  }
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
  if (Arcadia_Process_get(&process)) return EXIT_FAILURE;
  Arcadia_Thread* thread = Arcadia_Process_getThread(process);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) main1(process, argc, argv);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Process_relinquish(process);
  return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
