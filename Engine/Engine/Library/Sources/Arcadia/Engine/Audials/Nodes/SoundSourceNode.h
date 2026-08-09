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

#if !defined(ARCADIA_ENGINE_AUDIALS_NODES_SOUNDSOURCENODE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_NODES_SOUNDSOURCENODE_H_INCLUDED

#include "Arcadia/Engine/Audials/Node.h"
typedef struct Arcadia_Engine_Audials_BackendContext Arcadia_Engine_Audials_BackendContext;

// A simple mono sound source with position and velocity.
// Think of it like someone carrying a boombox: At any point of time, it as a location and a velocity.
// Unlike a real boombox, it emanates sound in all directions the same way.
// It operates like a cassette player: Start, stop, pause, fast forward, and rewind.
Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.SoundSourceNode", Arcadia_Engine_Audials_SoundSourceNode,
                          u8"Arcadia.Engine.Audials.Node");

struct Arcadia_Engine_Audials_SoundSourceNodeDispatch {
  Arcadia_Engine_Audials_NodeDispatch parent;

  Arcadia_BooleanValue(*isPlaying)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*);
  void (*pause)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*);
  void (*play)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*);
  void (*stop)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*);
  void (*setIsLooping)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*, Arcadia_BooleanValue);
  Arcadia_BooleanValue(*getIsLooping)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*);
  void (*setVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*, Arcadia_Real32Value);
  Arcadia_Real32Value(*getVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_SoundSourceNode*);
};

struct Arcadia_Engine_Audials_SoundSourceNode {
  Arcadia_Engine_Audials_Node _parent;
  // The sample buffer definition used by this sound source.
  Arcadia_ADL_SampleBufferDefinition* source;
  Arcadia_Engine_Audials_BackendContext* backendContext;
  Arcadia_Engine_Audials_Implementation_SoundSourceResource* soundSourceResource;
  Arcadia_Natural8Value dirtyBits;
  Arcadia_BooleanValue isLooping;
  Arcadia_Real32Value volume;
};

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_SoundSourceNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* source
  );

// Get if this sound source is playing.
// Raises an error if this is invoked before the resource was rendered.
Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

// Pause: If playing, halt playback.
// Raises an error if this is invoked before the resource was rendered. */
void
Arcadia_Engine_Audials_SoundSourceNode_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

// Play: If stopped or paused. Start playback at current position.
// Raises an error if this is invoked before the resource was rendered. */
void
Arcadia_Engine_Audials_SoundSourceNode_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

// Stop: If playing, halt playback. Rewind.
void
Arcadia_Engine_Audials_SoundSourceNode_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

// Sets if looping.
void
Arcadia_Engine_Audials_SoundSourceNode_setIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_BooleanValue isLooping
  );

// Gets if looping. Default is Arcadia_BooleanValue_False.
Arcadia_BooleanValue
Arcadia_Engine_Audials_SoundSourceNode_getIsLooping
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

// Sets the volume. The parameter value is clamped to [0,1]. Arcadia_Status_ArgumentValue is raised if volume is not a number.
void
Arcadia_Engine_Audials_SoundSourceNode_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self,
    Arcadia_Real32Value volume
  );

// Gets the volume. The return value is within [0,1]. Default is 1.
Arcadia_Real32Value
Arcadia_Engine_Audials_SoundSourceNode_getVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_SoundSourceNode* self
  );

#endif // ARCADIA_ENGINE_AUDIALS_NODES_SOUNDSOURCENODE_H_INCLUDED
