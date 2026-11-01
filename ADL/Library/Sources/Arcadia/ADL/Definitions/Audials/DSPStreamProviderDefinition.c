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
#include "Arcadia/ADL/Definitions/Audials/DSPStreamProviderDefinition.h"

static void
linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinition* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinition* self
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinition* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinitionDispatch* self
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

Arcadia_defineObjectType(u8"Arcadia.ADL.DSPStreamProviderDefinition", Arcadia_ADL_DSPStreamProviderDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &typeOperations);

static void
linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->dsp);
  Arcadia_ADL_Definition_link(thread, self->dsp->definition);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinition* self
  )
{
  if (self->dsp) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->dsp);
  }
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_DSPStreamProviderDefinition);
  {
    Arcadia_Value definitions = Arcadia_ValueStack_getValue(thread, 6);
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 5);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (6 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->dsp = (Arcadia_ADL_Reference*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_ADL_Reference_getType(thread));
  self->sampleRate = Arcadia_ValueStack_getNatural32Value(thread, 3);
  self->hasDuration = Arcadia_ValueStack_getBooleanValue(thread, 2);
  self->duration = Arcadia_ValueStack_getNatural32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_ADL_DSPStreamProviderDefinition);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DSPStreamProviderDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void(*)(Arcadia_Thread*, Arcadia_ADL_Definition*))&linkImpl;
}

Arcadia_ADL_DSPStreamProviderDefinition*
Arcadia_ADL_DSPStreamProviderDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_ADL_Reference* dsp,
    Arcadia_Natural32Value sampleRate,
    Arcadia_BooleanValue hasDuration,
    Arcadia_Natural32Value duration
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_DSPStreamProviderDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)dsp);
  Arcadia_ValueStack_pushNatural32Value(thread, sampleRate);
  Arcadia_ValueStack_pushBooleanValue(thread, hasDuration);
  Arcadia_ValueStack_pushNatural32Value(thread, duration);
  Arcadia_ValueStack_pushNatural8Value(thread, 6);
  _Arcadia_EndCreate(Arcadia_ADL_DSPStreamProviderDefinition);
}
