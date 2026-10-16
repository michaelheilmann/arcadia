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

#if !defined(ARCADIA_ENGINE_AUDIALS_NODES_STREAMINGSOUNDSOURCENODE_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_NODES_STREAMINGSOUNDSOURCENODE_H_INCLUDED

#include "Arcadia/Engine/Audials/Node.h"
#include "Arcadia/Engine/Audials/Providers/AudioStreamProvider.h"
typedef struct Arcadia_Engine_Audials_BackendContext Arcadia_Engine_Audials_BackendContext;
typedef struct Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource;

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.StreamingSoundSourceNode", Arcadia_Engine_Audials_StreamingSoundSourceNode,
                          u8"Arcadia.Engine.Audials.Node");

struct Arcadia_Engine_Audials_StreamingSoundSourceNodeDispatch {
  Arcadia_Engine_Audials_NodeDispatch parent;
  Arcadia_BooleanValue (*isPlaying)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*);
  void (*play)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*);
  void (*pause)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*);
  void (*stop)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*);
  void (*setVolume)(Arcadia_Thread*, Arcadia_Engine_Audials_StreamingSoundSourceNode*, Arcadia_Real32Value);
};

struct Arcadia_Engine_Audials_StreamingSoundSourceNode {
  Arcadia_Engine_Audials_Node parent;
  Arcadia_Engine_Audials_AudioStreamProvider* provider;
  Arcadia_Engine_Audials_BackendContext* backendContext;
  Arcadia_Engine_Audials_Implementation_StreamingSoundSourceResource* resource;
  Arcadia_Real32Value volume;
};

Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_StreamingSoundSourceNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_Engine_Audials_AudioStreamProvider* provider
  );

Arcadia_BooleanValue
Arcadia_Engine_Audials_StreamingSoundSourceNode_isPlaying
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

/// Start or resume playback. Render the node first to create its backend resource.
void
Arcadia_Engine_Audials_StreamingSoundSourceNode_play
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

/// Pause playback without advancing the provider while paused.
void
Arcadia_Engine_Audials_StreamingSoundSourceNode_pause
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

/// Stop and discard queued audio. The provider position is not rewound; create a
/// new provider and node to play the stream again from its beginning.
void
Arcadia_Engine_Audials_StreamingSoundSourceNode_stop
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self
  );

void
Arcadia_Engine_Audials_StreamingSoundSourceNode_setVolume
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_StreamingSoundSourceNode* self,
    Arcadia_Real32Value volume
  );

#endif
