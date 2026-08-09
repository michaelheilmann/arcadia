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
#include "Arcadia/Engine/Audials/Resource.h"

#include "Arcadia/Engine/Audials/BackendContext.h"

static void
Arcadia_Engine_Audials_Implementation_Resource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  );

static void
Arcadia_Engine_Audials_Implementation_Resource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_ResourceDispatch* self
  );

static void
Arcadia_Engine_Audials_Implementation_Resource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  );

static void
Arcadia_Engine_Audials_Implementation_Resource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_Resource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Audials_Implementation_Resource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Audials_Implementation_Resource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Audials_Implementation_Resource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.Implementation.Resource", Arcadia_Engine_Audials_Implementation_Resource,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Audials_Implementation_Resource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_Implementation_Resource);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->context = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Audials_BackendContext_getType(thread));
  self->referenceCount = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_Implementation_Resource);
}

static void
Arcadia_Engine_Audials_Implementation_Resource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_ResourceDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Audials_Implementation_Resource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Audials_Implementation_Resource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{/*Intentionally empty.*/}

void
Arcadia_Engine_Audials_Implementation_Resource_load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_Resource, load, self); }

void
Arcadia_Engine_Audials_Implementation_Resource_unload
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_Resource, unload, self); }

void
Arcadia_Engine_Audials_Implementation_Resource_unlink
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_Resource, unlink, self); }

void
Arcadia_Engine_Audials_Implementation_Resource_render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Audials_Implementation_Resource, render, self); }

void
Arcadia_Engine_Audials_Implementation_Resource_ref
  (
    Arcadia_Thread* thread, Arcadia_Engine_Audials_Implementation_Resource* self
  )
{
  if (self->referenceCount == Arcadia_Integer32Value_Maximum) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->referenceCount++;
}

void
Arcadia_Engine_Audials_Implementation_Resource_unref
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Implementation_Resource* self
  )
{
  if (self->referenceCount == Arcadia_Integer32Value_Literal(0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->referenceCount--;
}
