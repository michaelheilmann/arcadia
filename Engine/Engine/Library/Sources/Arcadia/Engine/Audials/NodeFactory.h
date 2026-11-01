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

#if !defined(ARCADIA_ENGINE_AUDIALS_NODEFACTORY_H_INCLUDED)
#define ARCADIA_ENGINE_AUDIALS_NODEFACTORY_H_INCLUDED

#if !defined(ARCADIA_ENGINE_PRIVATE) || 1 != ARCADIA_ENGINE_PRIVATE
  #error("do not include `Arcadia/Engine/Audials/NodeFactory.h` directly, include `Arcadia/Engine/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/NodeFactory.h"

typedef struct Arcadia_ADL_SampleBufferDefinition Arcadia_ADL_SampleBufferDefinition;
typedef struct Arcadia_ADL_SoundSourceNodeDefinition Arcadia_ADL_SoundSourceNodeDefinition;
typedef struct Arcadia_ADL_StreamingSoundSourceNodeDefinition Arcadia_ADL_StreamingSoundSourceNodeDefinition;
typedef struct Arcadia_Engine_Audials_BackendContext Arcadia_Engine_Audials_BackendContext;
typedef struct Arcadia_Engine_Audials_NodeFactory Arcadia_Engine_Audials_NodeFactory;
typedef struct Arcadia_Engine_Audials_SoundSourceNode Arcadia_Engine_Audials_SoundSourceNode;
typedef struct Arcadia_Engine_Audials_StreamingSoundSourceNode Arcadia_Engine_Audials_StreamingSoundSourceNode;

Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.NodeFactory", Arcadia_Engine_Audials_NodeFactory,
                          u8"Arcadia.Engine.NodeFactory");

struct Arcadia_Engine_Audials_NodeFactoryDispatch {
  Arcadia_Engine_NodeFactoryDispatch _parent;

  Arcadia_Engine_Audials_SoundSourceNode* (*createSoundSourceNode)(Arcadia_Thread*, Arcadia_Engine_Audials_NodeFactory*, Arcadia_Engine_Audials_BackendContext*, Arcadia_ADL_SampleBufferDefinition*);

  Arcadia_Engine_Audials_SoundSourceNode* (*createSoundSourceNodeFromDefinition)(Arcadia_Thread*, Arcadia_Engine_Audials_NodeFactory*, Arcadia_Engine_Audials_BackendContext*, Arcadia_ADL_SoundSourceNodeDefinition*);

  Arcadia_Engine_Audials_StreamingSoundSourceNode* (*createStreamingSoundSourceNodeFromDefinition)(Arcadia_Thread*, Arcadia_Engine_Audials_NodeFactory*, Arcadia_Engine_Audials_BackendContext*, Arcadia_ADL_StreamingSoundSourceNodeDefinition*);
};

struct Arcadia_Engine_Audials_NodeFactory {
  Arcadia_Engine_NodeFactory _parent;
};

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* sampleBufferDefinition
  );

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SoundSourceNodeDefinition* definition
  );

Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_StreamingSoundSourceNodeDefinition* definition
  );

Arcadia_Engine_Audials_NodeFactory*
Arcadia_Engine_Audials_NodeFactory_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ENGINE_AUDIALS_NODEFACTORY_H_INCLUDED
