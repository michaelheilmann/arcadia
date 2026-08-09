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
  #error("do not include directly, include `Arcadia/Engine/Include.h` instead")
#endif
#include "Arcadia/Engine/NodeFactory.h"
#include "Arcadia/Engine/Audials/BackendContext.h"
#include "Arcadia/Engine/Audials/Nodes/SoundSourceNode.h"

/// The implementation of a node factory for audials nodes.
Arcadia_declareObjectType(u8"Arcadia.Engine.Audials.NodeFactory", Arcadia_Engine_Audials_NodeFactory,
                          Arcadia_Engine_NodeFactory);

struct Arcadia_Engine_Audials_NodeFactoryDispatch {
  Arcadia_Engine_NodeFactoryDispatch parent;

  Arcadia_Engine_Audials_SoundSourceNode*
  (*createSoundSourceNode)
    (
      Arcadia_Thread* thread,
      Arcadia_Engine_Audials_NodeFactory* self,
      Arcadia_Engine_Audials_BackendContext* backendContext,
      Arcadia_ADL_SampleBufferDefinition* sampleBufferDefinition
    );
};

struct Arcadia_Engine_Audials_NodeFactory {
  Arcadia_Engine_NodeFactory parent;
};

Arcadia_Engine_Audials_NodeFactory*
Arcadia_Engine_Audials_NodeFactory_create
  (
    Arcadia_Thread* thread
  );

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* sampleBufferDefinition
  );

#endif // ARCADIA_ENGINE_AUDIALS_NODEFACTORY_H_INCLUDED
