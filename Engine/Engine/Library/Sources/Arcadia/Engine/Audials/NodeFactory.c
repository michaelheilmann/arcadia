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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Audials/NodeFactory.h"

static void
Arcadia_Engine_Audials_NodeFactory_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  );

static void
Arcadia_Engine_Audials_NodeFactory_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  );

static void
Arcadia_Engine_Audials_NodeFactory_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactoryDispatch* self
  );

static void
Arcadia_Engine_Audials_NodeFactory_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  );

static Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* source
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_Audials_NodeFactory_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_NodeFactory_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_NodeFactory_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_NodeFactory_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_NodeFactory_visitImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_Audials_NodeFactory_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_Audials_NodeFactory_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.NodeFactory", Arcadia_Engine_Audials_NodeFactory,
                         u8"Arcadia.Engine.NodeFactory", Arcadia_Engine_NodeFactory,
                         &_Arcadia_Engine_Audials_NodeFactory_typeOperations);

static void
Arcadia_Engine_Audials_NodeFactory_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_NodeFactory);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_NodeFactory);
}

static void
Arcadia_Engine_Audials_NodeFactory_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Audials_NodeFactory_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactoryDispatch* self
  )
{
  ((Arcadia_Engine_Audials_NodeFactoryDispatch*)self)->createSoundSourceNode = (Arcadia_Engine_Audials_SoundSourceNode * (*)(Arcadia_Thread*, Arcadia_Engine_Audials_NodeFactory*, Arcadia_Engine_Audials_BackendContext*, Arcadia_ADL_SampleBufferDefinition*)) & Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeImpl;
}

static void
Arcadia_Engine_Audials_NodeFactory_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  )
{/*Intentionally empty.*/}

static Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* source
  )
{ return Arcadia_Engine_Audials_SoundSourceNode_create(thread, backendContext, source); }

Arcadia_Engine_Audials_NodeFactory*
Arcadia_Engine_Audials_NodeFactory_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Audials_NodeFactory);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Engine_Audials_NodeFactory);
}

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* backendContext,
    Arcadia_ADL_SampleBufferDefinition* sampleBufferDefinition
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Audials_NodeFactory, createSoundSourceNode, self, backendContext, sampleBufferDefinition); }
