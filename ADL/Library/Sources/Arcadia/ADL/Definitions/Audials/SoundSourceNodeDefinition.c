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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Definitions/Audials/SoundSourceNodeDefinition.h"

static void
linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinition* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinition* self
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinition* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.SoundSourceNodeDefinition", Arcadia_ADL_SoundSourceNodeDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &typeOperations);

static void
linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->sampleBuffer);
  Arcadia_ADL_Definition_link(thread, self->sampleBuffer->definition);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinition* self
  )
{
  if (self->sampleBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->sampleBuffer);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_SoundSourceNodeDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 5);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 4);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (5 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->sampleBuffer = (Arcadia_ADL_Reference*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_ADL_Reference_getType(thread));
  self->isLooping = Arcadia_ValueStack_getBooleanValue(thread, 2);
  self->volume = Arcadia_ValueStack_getReal32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_SoundSourceNodeDefinition);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SoundSourceNodeDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void(*)(Arcadia_Thread*, Arcadia_ADL_Definition*))&linkImpl;
}

Arcadia_ADL_SoundSourceNodeDefinition*
Arcadia_ADL_SoundSourceNodeDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_ADL_Reference* sampleBuffer,
    Arcadia_BooleanValue isLooping,
    Arcadia_Real32Value volume
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_SoundSourceNodeDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)sampleBuffer);
  Arcadia_ValueStack_pushBooleanValue(thread, isLooping);
  Arcadia_ValueStack_pushReal32Value(thread, volume);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  _Arcadia_EndCreate(Arcadia_ADL_SoundSourceNodeDefinition);
}
