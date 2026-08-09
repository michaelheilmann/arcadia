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
#include "Arcadia/Engine/NodeFactory.h"

static void
Arcadia_Engine_NodeFactory_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactory* self
  );

static void
Arcadia_Engine_NodeFactory_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactoryDispatch* self
  );

static void
Arcadia_Engine_NodeFactory_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactory* self
  );

static void
Arcadia_Engine_NodeFactory_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactory* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Engine_NodeFactory_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_NodeFactory_construct,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_NodeFactory_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_NodeFactory_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_NodeFactory_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Engine_NodeFactory_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Engine_NodeFactory_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.NodeFactory", Arcadia_Engine_NodeFactory,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Engine_NodeFactory_typeOperations);

static void
Arcadia_Engine_NodeFactory_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactory* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_NodeFactory);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_NodeFactory);
}

static void
Arcadia_Engine_NodeFactory_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactoryDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_NodeFactory_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactory* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_NodeFactory_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_NodeFactory* self
  )
{/*Intentionally empty.*/}
