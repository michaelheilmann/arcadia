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
#include "Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h"

static void
Arcadia_Engine_Visuals_ConstantBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_ConstantBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_ConstantBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_ConstantBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_ConstantBufferResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_ConstantBufferResource_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_ConstantBufferResource_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_ConstantBufferResource_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.ConstantBufferResource", Arcadia_Engine_Visuals_ConstantBufferResource,
                         u8"Arcadia.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_ConstantBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_ConstantBufferResource);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_ConstantBufferResource);
}

static void
Arcadia_Engine_Visuals_ConstantBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_ConstantBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResourceDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_ConstantBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  )
{/*Intentionally empty.*/}

void
Arcadia_Engine_Visuals_ConstantBufferResource_setData
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ConstantBufferResource, setData, self, bytes, numberOfBytes); }

void
Arcadia_Engine_Visuals_ConstantBufferResource_clear
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ConstantBufferResource, clear, self); }

void
Arcadia_Engine_Visuals_ConstantBufferResource_writeColor4Real32
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    Arcadia_Math_Color4Real32 const* source
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ConstantBufferResource, writeColor4Real32, self, source); }

void
Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    Arcadia_BooleanValue transpose,
    Arcadia_Math_Matrix4Real32 const* source
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ConstantBufferResource, writeMatrix4x4Real32, self, transpose, source); }
